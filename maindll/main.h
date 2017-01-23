/* --------------------------------------------------------------------------
 main.h : eStylerLite runtime DLL main header
 2004/05/08 - Alessandro Cantatore - Italy
-------------------------------------------------------------------------- */



#ifndef ESTYLER_H_
   #define ESTYLER_H_
   #define INCL_DOSDEVICES
   #define INCL_DOSDEVIOCTL
   #define INCL_DOSERRORS
   #define INCL_DOSMISC
   #define INCL_DOSMODULEMGR
   #define INCL_DOSNLS
   #define INCL_DOSPROCESS
   #define INCL_DOSRESOURCES
   #define INCL_DOSSEMAPHORES
   #define INCL_GPI
   #define INCL_SHLERRORS
   #define INCL_WIN
   #define INCL_WINWORKPLACE
   #include <os2.h>
   #include <stdio.h>
   #include <string.h>
#ifdef USE_APIEX
   #include "ApiExPM.h"
   #include "ctrlutil.h"
#else
   #include "apixwrap.h"
#endif
   #include "stlrVersion.h"       // version number
   #include "stlrDefs.h"          // common usage data types and constants
   #include "types.h"             // innner usage data types
   #include "stlrGraphics.h"
   #include "stlrOptions.h"
   #include "stlrStrings.h"
   #include "frame.h"
   #include "stlrMacros.h"

   #ifndef RC_INVOKED
   #endif

/* --------------------------------------------------------------------------
 when compiled in debuggin mode the dbg* functions are used to print
 some data to the debug log file
*/
#ifdef DEBUGGING
   #include <stdio.h>
#endif

/* --------------------------------------------------------------------------
ACPI32 ordinals for ACPI power-off support
*/
// compatibility ordinals (for older ACPI versions)
#define _ACPI_START_API             16
#define _ACPI_END_API               17
#define _ACPI_GO_TO_SLEEP           19

// Tk* ordinals (proper functions for ACPI v3.21.x)
#define _ACPI_TK_START_API          67
#define _ACPI_TK_END_API            68
#define _ACPI_TK_GO_TO_SLEEP        70
#define _ACPI_TK_PREPARE_TO_SLEEP   89


/* --------------------------------------------------------------------------
*/

extern GLOBAL g;
extern OPTIONS o;

#ifdef DEBUGGING
   extern HFILE hf;
#endif

   #ifndef RC_INVOKED
      #include "debug.h"
// bitmaps.c
VOID setGlobalBitmaps(HINI hini);
// button.c
MRESULT EXPENTRY stlrButtonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// frame.c
MRESULT EXPENTRY stlrFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// menu.c
MRESULT EXPENTRY stlrMenuProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// hooks.c
BOOL EXPENTRY postMsgHook(HAB hab, PQMSG pqmsg, USHORT fs);
// process.c
VOID heapLockPM(HWND hwnd);
VOID heapLock(VOID);
VOID heapUnlock(VOID);
PVOID memAlloc(ULONG cb);
PVOID memLkAlloc(ULONG cb);
VOID memFree(PVOID pv);
VOID memLkFree(PVOID pv);
// titlebar.c
MRESULT EXPENTRY stlrTitleBarProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
// utils.c
ULONG logwrite(HFILE hf, PSZ psz);
BOOL stlrDateTime(PSZ psz);
BOOL stlrlog(ULONG ulmsg);
BOOL matchExeName(PSZ pszexe, PSZ pszcmp);
ULONG sysInfo(ULONG fl);
VOID draw3Dborder(HPS hps, PRECTL pr, LONG clrul, LONG clrbr, ULONG cpBorder);
// workthread.c
VOID _System threadWorker(ULONG ul);
VOID stopWorker(VOID);

   #endif // #ifndef RC_INVOKED

#endif // #ifndef ESTYLER_H_
