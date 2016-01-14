/* --------------------------------------------------------------------------
 process.c: process initialization/termination
            this is called by the system each time a PM process is created

-------------------------------------------------------------------------- */


#pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"
#include "error.h"
#include <umalloc.h>
#include <stdlib.h>


// prototypes ---------------------------------------------------------------
VOID APIENTRY processExitHandler(ULONG ul);
BOOL isException(PSZ pszModule);
BOOL meminit(VOID);
BOOL memterm(VOID);
void* heapIncrease(Heap_t uhp, size_t* plen, int* pfl);
void heapDecrease(Heap_t uhp, PVOID p, size_t size);
PVOID memLkRealloc(PVOID p, ULONG cb);
VOID memHeapMin(VOID);

// data segment specfication ------------------------------------------------
#pragma data_seg(STLRINSTANCE_DATA)

// global variables ---------------------------------------------------------
PID pid;                     // store the process ID
static PVOID pbasemem;
static Heap_t hp;
static volatile INT isem;

// definitions --------------------------------------------------------------
#ifndef RNDUP
   #define RNDUP(n, m)   (((ULONG)(n) + (m) - 1) & ~((m) - 1))
#endif
// RAM semaphore lock/unlock flags
#define HEAP_LOCK    1
#define HEAP_UNLOCK  0

#define CB_HEAPBLOCK     0x10000

/* --------------------------------------------------------------------------
 This is loaded by the system via the OS2.INI SYS_DLLS - LoadPerProcess
 method as it is exported as ordinal number is 1.
 - initialize the process heap
 - get the process data
 - check if this is the PROTSHELL process
 - check if the program is listed in the exceptions
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _Export APIENTRY stlrProcessInit(VOID) {
   CHAR buf[CCHMAXPATH];
   PTIB ptib;
   PPIB ppib;
   if (!meminit()) return;                  // memory heap initialization
   DosGetInfoBlocks(&ptib, &ppib);          // get process details

dbgPrintf6(">>> pid: %d par:%d (WPS1=%d- WPS2=%d hmod=%d)",
           ppib->pib_ulpid, ppib->pib_ulppid, g.pid.protshell, g.pid.pmshell,
           ppib->pib_hmte);

   // check if this is the PROTSHELL process -----------------------------
   if (!g.pid.protshell) {
dbgPrintf1("\n");
      g.pid.protshell = ppib->pib_ulpid;
      // get HWND_DESKTOP handle
      g.hwnd.desktop = WinQueryDesktopWindow(NULLHANDLE, NULLHANDLE);
dbgPrintf2("desktop window handle:%08x\n", g.hwnd.desktop);
      // if command-window/win-OS2 exception
      if (isException(SZXCPT_PROTSHELL)) goto exit_1;
   // other kind of process ----------------------------------------------
   } else {
      if (DosQueryModuleName(ppib->pib_hmte, CCHMAXPATH, buf)) {
         stlrlog(IDERR_QRYMODNAME);
         goto exit_1;
      } /* endif */
dbgPrintf2(" mod:%s\n", buf);
   // check if this is the PMSHELL process
      if (!g.pid.pmshell
          &&
          (ppib->pib_ulppid == g.pid.protshell)
          &&
          matchExeName(buf, "PMSHELL.EXE")) {
         g.pid.pmshell = ppib->pib_ulpid;
      } /* endif */
      if (isException(buf)) goto exit_1;
   } /* endif */
   // register the exit procedure only if the current process is not PROTSHELL
   if ((g.pid.protshell != ppib->pib_ulpid)
       &&
       DosExitList(EXLST_ADD | 0x0000AA00, (PFNEXITLIST)processExitHandler)) {
      stlrlog(IDERR_REGEXITFUNC);
      goto exit_1;
   } /* endif */
   pid = ppib->pib_ulpid;
   return;

   // error handling -----------------------------------------------------
exit_1:
   memterm();
}


/* --------------------------------------------------------------------------
 process exit handler:
 - if this is the PMSHELL process reset some flags
 - destroy the memory heap
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID:
-------------------------------------------------------------------------- */
static VOID APIENTRY processExitHandler(ULONG ul) {
   // if PMSHELL (RUNWORKPLACE) termination reset some variables
   if (g.pid.pmshell == pid) {
      g.pid.pmshell = 0;
      g.hwnd.desktopFolder = NULLHANDLE;
      g.hwnd.warpCenter = NULLHANDLE;
   } /* endif */
   // free process memory heap
   memterm();
   DosExitList(EXLST_EXIT, (PFNEXITLIST)NULL);
   ul;
}


/* --------------------------------------------------------------------------
 Check if 'pszModule' is listed in the program exceptions.
- Parameters -------------------------------------------------------------
 PSZ pszModule: name of the executable to check.
- Return value -----------------------------------------------------------
 BOOL: TRUE/FALSE (listed as exception / not listed)
-------------------------------------------------------------------------- */
static BOOL isException(PSZ pszModule) {
   PSZ pszExceptions, psz;
   BOOL rc = FALSE;
   ULONG ul = 0x10000;

   // if the global flag tells that no exception is registered just return FALSE
   if (!o.ui.exception) return FALSE;
   if (!m_sysMemAlloc(pszExceptions,ul))
      return m_logError(IDERR_CHKEXCALLOC, FALSE);
   // read exceptions data from OS2.INI
   if (!PrfQueryProfileData(HINI_USER,
                            SZPRO_APP, SZEXC_GENERAL, pszExceptions, &ul)) {
      stlrlog(IDERR_CHKEXCREAD); // error as o.ui.exception flag is true
      goto end;
   } // end if
   if (ul) {
      for (psz = pszExceptions; *psz; psz += strlen(psz) + 1)
         if (matchExeName(pszModule, psz)) break;
      rc = *psz;        // 0 se eccezione non trovata
   } // end if
end:
   m_sysMemFree(pszExceptions);
   return rc;
}

/* --------------------[ MEMORY MANAGEMENT ROUTINES ]--------------------- */

/* --------------------------------------------------------------------------
 Initialize the heap used by _umalloc.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL meminit(VOID) {
   if (!m_sysMemAlloc(pbasemem, CB_HEAPBLOCK)
       ||
       (NULL == (hp = _ucreate(pbasemem, CB_HEAPBLOCK, _BLOCK_CLEAN,
                               _HEAP_REGULAR, heapIncrease, heapDecrease)))
       ||
       _uopen(hp)) {
      return m_logError(IDERR_INITPROCHEAP, FALSE);
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 Free the resources used for the process heap.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL memterm(VOID) {
   return !(_uclose(hp) || _udestroy(hp, _FORCE) || DosFreeMem(pbasemem));
}


/* --------------------------------------------------------------------------
 Callback procedure used to increase the user heap size.
- Return value -----------------------------------------------------------
 PVOID newly allocated additional heap
-------------------------------------------------------------------------- */
static void* heapIncrease(Heap_t uhp, size_t* plen, int* pfl) {
   PVOID p = NULL;
   *plen = (size_t)RNDUP(*plen, CB_HEAPBLOCK);
   *pfl = _BLOCK_CLEAN;
   DosAllocMem(&p, *plen, PAG_COMMIT | PAG_READ | PAG_WRITE);
   return p;
}


/* --------------------------------------------------------------------------
 Callback procedure used to decrease the user heap size.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static void heapDecrease(Heap_t uhp, PVOID p, size_t size) {
   DosFreeMem(p);
}


/* --------------------------------------------------------------------------
 Suspend the current thread, giving CPU time to other threads, until the
 HEAP access semaphore is unlocked by the thread currently owning it.
 This routine does not block the PM message queue.
- Parameters -------------------------------------------------------------
 HAB hab   : anchor block handle
 HWND hwnd : window handle
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID heapLockPM(HWND hwnd) {
   QMSG qmsg;
   HAB hab = (HWND)WinQueryAnchorBlock(hwnd);
   while (__lxchg(&isem, HEAP_LOCK)) {
      DosSleep(1);
      while (WinPeekMsg(hab, &qmsg, NULLHANDLE, 0, 0, PM_REMOVE))
         WinDispatchMsg(hab, &qmsg);
   } /* endwhile */
}


/* --------------------------------------------------------------------------
 Suspend the current thread, giving CPU time to other threads, until the
 HEAP access semaphore is unlocked by the thread currently owning it.
 This routine blocks the PM message queue.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID heapLock(VOID) {
   QMSG qmsg;
   while (__lxchg(&isem, HEAP_LOCK)) DosSleep(1);
}


/* --------------------------------------------------------------------------
 heapUnlock : unlocks the integer used to serialize the HEAP access
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID heapUnlock(VOID) {
   __lxchg(&isem, HEAP_UNLOCK);
}


/* --------------------------------------------------------------------------
 Allocates memory as malloc() without serializing the heap access.
- Parameters -------------------------------------------------------------
 ULONG cb : size of storage to be allocated.
- Return value -----------------------------------------------------------
 PVOID : allocated storage address or NULL in case of error.
-------------------------------------------------------------------------- */
PVOID memAlloc(ULONG cb) {
   return _umalloc(hp, cb);
}


/* --------------------------------------------------------------------------
 Allocates memory as malloc() serializing the heap access.
- Parameters -------------------------------------------------------------
 ULONG cb : size of storage to be allocated.
- Return value -----------------------------------------------------------
 PVOID : allocated storage address or NULL in case of error.
-------------------------------------------------------------------------- */
PVOID memLkAlloc(ULONG cb) {
   PVOID pv = NULL;
   if (!cb) return NULL;
   // if the RAM semaphore is locked gives CPU time to the other threads
   // so they can finish their job and release the semaphore
   while (__lxchg(&isem, HEAP_LOCK)) DosSleep(1);
   pv = _umalloc(hp, cb);
   __lxchg(&isem, HEAP_UNLOCK);
   return pv;
}


/* --------------------------------------------------------------------------
 Release the storage allocated via memAlloc.
- Parameters -------------------------------------------------------------
 PVOID : address of the storage to be freed.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID memFree(PVOID pv) {
   if (pv) free(pv);
}


/* --------------------------------------------------------------------------
 Release the storage allocated via memLkAlloc.
- Parameters -------------------------------------------------------------
 PVOID : address of the storage to be freed.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID memLkFree(PVOID pv) {
   if (pv) {
      while (__lxchg(&isem, HEAP_LOCK)) DosSleep(1);
      free(pv);
      __lxchg(&isem, HEAP_UNLOCK);
   } /* endif */
}


/* --------------------------------------------------------------------------
 memLkRealloc : exported multithread memory allocation function
                (works as realloc)
                Serialized HEAP access.
- Parameters -------------------------------------------------------------
 PVOID p  : address to be reallocated.
 ULONG cb : count of bytes of memory to be allocated
- Return value -----------------------------------------------------------
 PVOID : address of allocated memory or NULL in case of error
-------------------------------------------------------------------------- */
PVOID memLkRealloc(PVOID p, ULONG cb) {
   // if the RAM semaphore is locked gives CPU time to the other threads
   // so they can finish their job and release the semaphore
   while (__lxchg(&isem, HEAP_LOCK)) DosSleep(1);
   realloc(p, cb);
   __lxchg(&isem, HEAP_UNLOCK);
   return p;
}


/* --------------------------------------------------------------------------
 Minimize the heap usage (not-serialized HEAP access).
- Parameters -------------------------------------------------------------
 VOID.
- Return value -----------------------------------------------------------
 VOID.
-------------------------------------------------------------------------- */
VOID memHeapMin(VOID) {
   _uheapmin(hp);
}
