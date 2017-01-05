/* --------------------------------------------------------------------------
 workThread.c: worker thread and object window procedure

 2004-05-12 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

#pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"
#include "error.h"
#include "stlrShutdown.h"
#include "stlrExport.h"

// definitions --------------------------------------------------------------
#define WC_ESTLRWORKER          "eStylerWorker"

// prototypes ---------------------------------------------------------------
static MRESULT EXPENTRY fnwpWorker(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static HAPP runShutdownProgrListItem(PSHDWNPRGLIST pProgramList);
static BOOL systemShutdown(VOID);
static VOID setNextBootVolume(VOID);

// global variables ---------------------------------------------------------
static PSHDWNPRGLIST psdpl;

/* --------------------------------------------------------------------------
 Worker thread. Create an object window to manage all time intensive stuff.
- Parameters -------------------------------------------------------------
 ULONG ul : parameter passed by DosCreateThread() (unused)
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _System threadWorker(ULONG ul) {
   QMSG qmsg;

   // initialization
   g.worker.hmq = WinCreateMsgQueue((g.worker.hab = WinInitialize(0)), 0);
   WinCancelShutdown(g.worker.hmq, TRUE);
   // register the object window class
   if (!WinRegisterClass(g.worker.hab, WC_ESTLRWORKER, (PFNWP)fnwpWorker, 0L, 0L))
      stopWorker();
   // create the object window
   if (!(g.worker.hwnd = m_createObjectWindow(WC_ESTLRWORKER, 0, 100)))
      stopWorker();
   // never ending message loop
   for (;;) {
      if (WinGetMsg(g.worker.hab, &qmsg, NULLHANDLE, 0, 0))
         WinDispatchMsg(g.worker.hab, &qmsg);
   } /* endfor */
   // termination
   stopWorker();
}


/* --------------------------------------------------------------------------
 Object window procedure.
- Parameters -------------------------------------------------------------
 ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT
-------------------------------------------------------------------------- */
static
MRESULT EXPENTRY fnwpWorker(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   // enabling/disabling event -------------------------------------------
   if (msg == STLRM_DISABLE) {
      if ((HWND)mp2 == hwnd) {
         o.ui.disabled = (mp1 == NULL);
         m_stlrMsgBroadcast(STRLM_DLGWINFONT, 0);
         m_stlrMsgBroadcast(STLRM_REDRAWTITLE, 0);
         WinInvalidateRect(HWND_DESKTOP, NULL, TRUE);
      } /* endif */
   // ensure single instance of the preferences application --------------
   } else if (msg == STLRM_CHECKINSTANCE) {
      // mp2 contains the preferences mode (>= 0 && <= 2)
      if (((ULONG)mp2) <= IDX_WINDOWSKEYSPREF) {
         PHWND phwnd = ((PHWND)&g.hwnd.interfacePref) + (ULONG)mp2;
         // mp1 contains the handle of the new instance
         // no other instance is already running : give OK to the NEW instance
         if (!*phwnd) {
            *phwnd = (HWND)mp1;
            WinPostMsg((HWND)mp1, STLRM_CHECKINSTANCE, MPVOID, MPVOID);
         // current instance is being terminated, reset the stored window handle
         } else if (*phwnd == (HWND)mp1) {
            *phwnd = NULLHANDLE;
         // a previous instance is already running : give the focus to this
         } else {
            WinPostMsg((HWND)mp1, STLRM_CHECKINSTANCE, (MPARAM)*phwnd, MPVOID);
         } /* endif */
      } /* endif */
   // termination of a shutdown application ------------------------------
   } else if (msg == WM_APPTERMINATENOTIFY) {
      if (psdpl
          &&
          ((HAPP)mp1 == psdpl->happ)
          &&
          (NULLHANDLE == (psdpl->happ = runShutdownProgrListItem(psdpl)))) {
         memLkFree(psdpl);
         psdpl = NULL;
         systemShutdown();
      } /* endif */
   // shutdown request : open the shutdown dialog ------------------------
   } else if (msg == STLRM_SHUTDOWNREQ) {
      if (((HWND)mp1 == hwnd) && !g.is.startShutdown) {
         TID tid;
dbgPrintf4("*** shutdown request, psdpl=%p, windowlist=%08x, g.achsd=%s\n",
           psdpl, g.hwnd.winList, g.achsd);
         // shutdown request while another shutdown is already being performed
         // delete execution of the shutdown programs
         if (psdpl) {
            memLkFree(psdpl);
            psdpl = NULL;
         } /* endif */
         // delete current shutdown thread
         if (g.hwnd.shutdown) {
            WinSendMsg(g.hwnd.shutdown, STLRM_EXITSHDWNTHREAD,
                       (MPARAM)g.hwnd.shutdown, MPVOID);
            DosSleep(1);
         } /* endif */
         // create the shutdown thread
         if (NO_ERROR == DosCreateThread(&tid, (PFNTHREAD)shutdownThread,
                                         (ULONG)mp2, 0, 8192)) {
            g.is.startShutdown = 1;
         } else {
            WinSendMsg(hwnd, STLRM_SHUTDOWNERR, MPVOID, MPVOID);
         } /* endif */
      } /* endif */
   // reset the system-being-shutdown flag -------------------------------
   } else if (msg == STLRM_RESETSHUTDOWNFL) {
      if ((HWND)mp1 == hwnd) g.is.startShutdown = 0;
   // shutdwon dialog error ----------------------------------------------
   } else if (msg == STLRM_SHUTDOWNERR) {
dbgPrintf1("*** shutdown error\n");
      // failed to create the shutdown dialog : display a simple message box
      if (MBID_YES == WinMessageBox(HWND_DESKTOP, hwnd, "Shutdown?",
                                    NULL, 0, MB_YESNO | MB_QUERY |
                                    MB_SYSTEMMODAL | MB_MOVEABLE))
         systemShutdown();
   // lockup & suspend delay timer ---------------------------------------
   } else if (msg == WM_TIMER) {
      if ((ULONG)mp1 == TID_DELLKSUSP) {
         WinStopTimer(g.worker.hab, g.worker.hwnd, TID_DELLKSUSP);
         doPowerManagement(APM_SUSPEND);
      } /* endif */
   // power-off/ordinary shutdown/reboot ---------------------------------
   } else if (msg == STLRM_SHUTDOWNSYS) {
      if ((HWND)mp1 == hwnd) {
         BOOL bWaitAppTerm = FALSE;
         g.is.startShutdown = 0;
         // execute the shutdown programs
         if (o.sd.adv.runProgOnce
             &&
             (NULL != (psdpl = shdwnReadProgList(TRUE)))) {
            // reset the runProgUnattended flag
            if (o.sd.adv.runProgOnce) o.sd.adv.runProgOnce = 0;
            // if cannot run any program in the list terminates
            if (!(psdpl->happ = runShutdownProgrListItem(psdpl))) {
               memLkFree(psdpl);
               psdpl = NULL;
            } else {
               bWaitAppTerm = TRUE;
            } /* endif */
         } /* endif */
dbgPrintf2("*** STLRM_SHUTDOWNSYS - psdpl=%p\n", psdpl);
         // if the warp center window had ever been opened save it
         if (g.hwnd.warpCenter) {
            m_setWpsObject(OBJID_WARPCENTER, OBJ_SAVEWC);
            DosSleep(o.sd.wait.warpCenter);
         } /* endif */
         // if a volume must be rebooted set the next boot volume
         if (g.achsd[1]) setNextBootVolume();
         // if must not wait for a shutdown program close the system
         if (!bWaitAppTerm) systemShutdown();
      } /* endif */
   // system lockedup via hotkey -----------------------------------------
   } else if (msg == STLRM_LOCKUPSYS) {
      if ((HWND)mp1 == hwnd) WinLockupSystem(g.worker.hab);
   // other messages -----------------------------------------------------
   } else {
      return WinDefWindowProc(hwnd, msg, mp1, mp2);
   } /* endif */
   return MRFALSE;
}


/* --------------------------------------------------------------------------
 Terminate the worker thread releasing the previously allocated resources.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID stopWorker(VOID) {
   if (g.worker.hwnd) WinDestroyWindow(g.worker.hwnd);
   if (g.worker.hmq) WinDestroyMsgQueue(g.worker.hmq);
   WinTerminate(g.worker.hab);
   DosExit(EXIT_THREAD, 0);
}


/* --------------------------------------------------------------------------
 Execute the current program from the list of the programs to be executed
 on shutdown.
- Parameters -------------------------------------------------------------
 PSHDWNPRGLIST pProgramList : list of programs to be executed on shutdown.
- Return value -----------------------------------------------------------
 HAPP handle of the started application.
-------------------------------------------------------------------------- */
static HAPP runShutdownProgrListItem(PSHDWNPRGLIST pProgramList) {
   HAPP happ = NULLHANDLE;
   PROGDETAILS prgdet;
   PSZ p;
   // /*pProgramList->pprog == '\x00') indicates the end of the list
   while (*pProgramList->pprog) {
      memset(&prgdet, 0, sizeof(PROGDETAILS));
      prgdet.Length = sizeof(PROGDETAILS);
      prgdet.progt.progc = PROG_DEFAULT;
      prgdet.progt.fbVisible = SHE_VISIBLE;
      // set execution mode:
      if (*pProgramList->pprog == '<') {             // run minimized
         prgdet.swpInitial.hwndInsertBehind = HWND_BOTTOM;
         prgdet.swpInitial.fl = SWP_HIDE | SWP_MINIMIZE | SWP_ZORDER;
         pProgramList->pprog++;
      } else if (*pProgramList->pprog == '>') {      // run maximized
         prgdet.swpInitial.hwndInsertBehind = HWND_TOP;
         prgdet.swpInitial.fl = SWP_ACTIVATE | SWP_ZORDER | SWP_MAXIMIZE;
         pProgramList->pprog++;
      } else {                                       // run normal
         prgdet.swpInitial.hwndInsertBehind = HWND_TOP;
         prgdet.swpInitial.fl = SWP_SHOW | SWP_ZORDER;
      } /* endif */
      // separate the executable name from the parameters
      prgdet.pszExecutable = pProgramList->pprog;
      if ((NULL != (p = strstr(pProgramList->pprog, ".EXE")))
           ||
          (NULL != (p = strstr(pProgramList->pprog, ".COM")))) {
         *(p + 4) = 0;
         happ = WinStartApp(g.worker.hwnd, &prgdet, (p += 5), NULL, 0L);
      } /* endif */
      // get to the next program
      pProgramList->pprog = p + strlen(p) + 1;
      pProgramList->icurprog++;
      // if the program was succesfully started exit from the loop
      // otherwise go on with the next program in the list
      if (happ) break;
   } /* endwhile */
   return happ;
}


/* --------------------------------------------------------------------------
 If the list of the programs to be killed before shutdown is defined,
 kill all the programs and shutdown the system via WinShutdownSystem().
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
#pragma import(DosQuerySysState,,"DOSCALL1",368)
static BOOL systemShutdown(VOID) {
   HINI hini;
   ULONG ul;
   PKILL_LIST pkl;
   g.is.hookShutdown = 1;
   // unrolls the windows just in case Styler/2 is running
   if (g.is.styler2v1) {
      WinBroadcastMsg(g.hwnd.desktop, WM_QUERYFRAMEINFO, STLR2M_SHDWNUNRL,
                      MPVOID, BMSG_POST | BMSG_DESCENDANTS | BMSG_FRAMEONLY);
      DosSleep(o.sd.wait.unrollWindows);
   } /* endif */
   // reads the kill list from ESTYLER.INI -> Shutdown ->KillList
   if (!(hini = stlrOpenProfile())) goto error;
   if (PrfQueryProfileSize(hini, SZPRO_SHUTDWON, SZPRO_KILLPROGS, &ul)) {
      if (!(pkl = memLkAlloc(ul + CBFIX_KILLST))) goto error;
      memset(pkl, 0, ul + CBFIX_KILLST);
      if (!PrfQueryProfileData(hini, SZPRO_SHUTDWON, SZPRO_KILLPROGS,
                               pkl->achlist, &ul))
         goto error;
      PrfCloseProfile(hini);
      // calculates the allocation needed for retrieving the processes data
      DosQuerySysState(QSS_PROCESS, 0, 0, 0, (PVOID)pkl->ach, 260);
      ul = ((PQSSDATA)pkl->ach)->pcounts->cThreads * 88 + 48;
      // allocates the memory needed to retrieve the processes data
      if (!(pkl->p = (PQSSDATA)memLkAlloc(ul))) goto error;
      memset(pkl->p, 0, ul);
      // gets all processes data
      if (DosQuerySysState(QSS_PROCESS, 0, 0, 0, (PVOID)pkl->p, ul))
         goto error;
      // loops through all processes killing them as needed
      while (pkl->p->pprocdat->ulType == 1) {
         if (pkl->p->pprocdat->hndmod) {
            if (DosQueryModuleName(pkl->p->pprocdat->hndmod, 260, pkl->ach))
               goto error;
            // if the current process name matches any list entry kills it
            for (pkl->pexe = pkl->achlist; *pkl->pexe;
                 pkl->pexe += strlen(pkl->pexe) + 1) {
               if (matchExeName(pkl->ach, pkl->pexe)) {
dbgPrintf3("Calling DosKillProcess() on pid %u (%s)\n", pkl->p->pprocdat->pid, pkl->ach );
                  if (DosKillProcess(DKP_PROCESS, pkl->p->pprocdat->pid))
                     goto error;
                  break;
               } /* endif */
            } /* endfor */
         } /* endif */
         pkl->p->pprocdat = (PQSSPROCDAT)((PBYTE)pkl->p->pprocdat +
                          sizeof(QSSPROCDAT) +
                          (sizeof(QSSTHRDDAT) * pkl->p->pprocdat->cthreads));
      } /* endwhile */
      memLkFree(pkl->p);
      memLkFree(pkl);
      // pause for a moment to make sure (esp. non-PM) processes are closed
dbgPrintf2("Pausing for %u ms\n", o.sd.wait.killProcs );
      DosSleep(o.sd.wait.killProcs);
   } /* endif */
dbgPrintf1("*** issuing WinShutdownSystem\n");

   return WinShutdownSystem(g.worker.hab,
                            WinQueryWindowULong(g.worker.hwnd, QWL_HMQ));
error:
   doCtrlAltDel();
   return FALSE;
}


/* --------------------------------------------------------------------------
 Set the volume to be booted on the next reboot via the undocumented "/4"
 option of SETBOOT.EXE.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setNextBootVolume(VOID) {
   CHAR buf[48];
   RESULTCODES rcs;
   ULONG cb = strlen(g.achsd);
   memcpy(buf, "SETBOOT.EXE\x0/4:", 15);
   if (g.achsd[0] == '4') {
      buf[15] = '\"';
      memcpy(buf + 16, &g.achsd[1], cb);
      buf[cb += 15] = '\"';
      buf[++cb] = 0;
   } else {     // unset the next boot volume
      cb = 14;
   } /* endif */
   buf[++cb] = 0;
   dbgPrintf2("Attempting reboot using command \"%s\"\n", buf );
   DosExecPgm(NULL, 0, EXEC_SYNC, buf, NULL, &rcs, "SETBOOT.EXE");
}


