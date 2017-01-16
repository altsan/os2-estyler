/* --------------------------------------------------------------------------
 main.c : eStyler settings main module.

 2004-6-4 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"
#include "notebook.h"

// definitions --------------------------------------------------------------



// prototypes ---------------------------------------------------------------
static VOID runApplication(VOID);
static MRESULT EXPENTRY mainDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static VOID onDlgInit(HWND hwnd);
static BOOL onDlgDeferredInit(HWND hwnd, HWND hwndPrevious);
static BOOL addToWinList(HWND hwnd);
static VOID restoreWinPos(HWND hwnd);
static BOOL setNotebookPages(HWND hwnd);
static VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, PPAGESELECTNOTIFY ppsn);
BOOL onSaveApplication(HWND hwnd);
static BOOL insertPage(ULONG idx, ULONG idStatusText,
                ULONG idMajTabText, ULONG idMinTabText,
                PFNWP pfnDlg, ULONG dlgID,
                ULONG BFAflag, ULONG BKAflag);


// global variables ---------------------------------------------------------
GLOBAL g;

// entry point --------------------------------------------------------------

INT _Optlink main(INT argc, PSZ* argv) {
   parseArgs(argc, argv);
   if (initApplication()) {
      g.appl.hwnd = WinLoadDlg(HWND_DESKTOP,  // parent
                               NULLHANDLE,    // owner
                               mainDlgProc,   // dialog window proc
                               g.hResLib,     // module handle
                               DLG_MAIN,      // dialog template ID
                               NULL);         // application data pointer
      if (g.appl.hwnd) runApplication();
   } /* endif */
   endApplication();
   return 0;
}


/* --------------------------------------------------------------------------
 Application message loop. Intercept termination caused by system shutdown
 or tasklist to properly close any pending worker thread jobs.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID runApplication(VOID) {
   QMSG qmsg;
   initHelp();
   forever {
      if (WinGetMsg(g.appl.hab, &qmsg, NULLHANDLE, 0, 0)) {
         WinDispatchMsg(g.appl.hab, &qmsg);
      } else if ((qmsg.hwnd == NULLHANDLE)                   // SHUTDOWN
                 ||
                 (qmsg.hwnd == (HWND)qmsg.mp2)) {            // TASKLIST
         quit();
      } else {
         break;
      } /* endif */
   } /* endforever */
}


/* --------------------------------------------------------------------------
 Application window procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY mainDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         onDlgInit(hwnd);
         break;
      case STLRM_CHECKINSTANCE:
         onDlgDeferredInit(hwnd, (HWND)mp1);
         break;
      case WM_CONTROL:
         onCtrlMsg(hwnd,  SHORT1FROMMP(mp1),
                      SHORT2FROMMP(mp1), (PPAGESELECTNOTIFY)mp2);
         break;
      case WM_COMMAND:
         if ((ULONG)mp1 == BTN_PREVIEW) {
            togglePreviewWindow(FALSE);
         } else {
            WinSendMsg(wnbkQueryTopPageHwnd(g.hwndNoteBook), msg, mp1, mp2);
         } /* endif */
         break;
      case WM_CLOSE:
         quit();
         break;
      case WM_ENABLE:
         wnbkSetPageHelp(g.hwndHelp, g.appl.hwnd, g.hwndNoteBook,
                         (mp1 ? wnbkQueryTopPageHwnd(g.hwndNoteBook) : 0));
         // fall through the next statement
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}

/* --------------------------------------------------------------------------
 Initialization of the main window of the application.
 Only one instance of the current mode is allowed. To ensure this a message
 is posted to the worker thread of the runtime DLL and initialization is
 deferred until a message is posted back from the worker thread (see
 onDlgDeferredInit() ).
- Parameters -------------------------------------------------------------
 HWND hwnd      : main application window handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static VOID onDlgInit(HWND hwnd) {
#ifndef _STLR_TEST_MODE_
   // post a message to the worker thread to check if this is the unique
   // instance of the current mode
   mNotifyRunTime(STLRM_CHECKINSTANCE, hwnd, g.mode);
#else
   WinPostMsg(hwnd, STLRM_CHECKINSTANCE, MPVOID, MPVOID);
#endif
}


/* --------------------------------------------------------------------------
 Initialization of the main window of the application.
 If a previous instance of the application is running give it the focus
 and terminate, otherwise go on with initialization:
- Parameters -------------------------------------------------------------
 HWND hwnd      : main application window handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL onDlgDeferredInit(HWND hwnd, HWND hwndPrevious) {
   // previous instance running : give it the focus and terminate
   if (hwndPrevious) {
      WinSetWindowPos(hwndPrevious, HWND_TOP, 0, 0, 0, 0,
                      SWP_ZORDER | SWP_SHOW | SWP_FOCUSACTIVATE | SWP_RESTORE);
      quit();
   // no previous instance : complete initialization
   } else {
      // register an exit procedure so the previous instance handle is
      // reset even if the application is killed by DosKillProcess()
      // in case of error immediately reset the previous instance handle
      // and terminate
      if (DosExitList(EXLST_ADD, (PFNEXITLIST)onAppExit)) {
         onAppExit(0xffff);
         quit();
         return handleError(ISERR_REGEXITLIST, FALSE);
      } /* endif */
      // add the window to the window list
      addToWinList(hwnd);
      // restore the window position
      restoreWinPos(hwnd);
      // fill the settings notebook
      return setNotebookPages(hwnd);
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 Add the application to the window list.
- Parameters -------------------------------------------------------------
 HWND hwnd : application window handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL addToWinList(HWND hwnd) {
   SWCNTRL swc;
   memset(&swc, 0, sizeof(swc));
   swc.hwnd = hwnd;
   swc.uchVisibility = SWL_VISIBLE;
   swc.fbJump = SWL_JUMPABLE;
   // set the proper icon
   WinSendMsg(hwnd, WM_SETICON,
              (MPARAM)WinLoadPointer(HWND_DESKTOP, g.hResLib, g.mode + 2),
              MPVOID);
   setCtrlText(hwnd, FID_TITLEBAR, IDS_UITITLE + g.mode);
   return (BOOL)WinAddSwitchEntry(&swc);
}


/* --------------------------------------------------------------------------
 Restore the window position.
- Parameters -------------------------------------------------------------
 HWND hwnd : application window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID restoreWinPos(HWND hwnd) {
   RECTL r;
   // get the window size and the window border thickness
   WinQueryWindowRect(hwnd, &r);
   WinSendMsg(hwnd, WM_QUERYBORDERSIZE, (MPARAM)&r, MPVOID);
   // if the window is not fully visible or the position is uninitialized
   // center it on the screen
   // note: the xLeft and yBottom members of r are the window border sizes
   //       while the other 2 members are the window width and height
   if (!(g.winPos.x && g.winPos.y)
       ||
       !((g.winPos.x >= -r.xLeft)
         &&
         (g.winPos.x <= (g.scr.cx - r.xRight))
         &&
         (g.winPos.y >= -r.yBottom)
         &&
         (g.winPos.y <= (g.scr.cy - r.yTop)))) {
      if (g.scr.cx > 800) {
         g.winPos.x = (g.scr.cx - r.xRight) / 2;
         g.winPos.y = (g.scr.cy - r.yTop) / 2;
      } else {
         g.winPos.x = -r.xLeft;
         g.winPos.y = -r.yBottom;
      } /* endif */
   } /* endif */
   WinSetWindowPos(hwnd, HWND_TOP, g.winPos.x, g.winPos.y, 0, 0,
                   SWP_MOVE | SWP_SHOW | SWP_ACTIVATE | SWP_ZORDER);
}


/* --------------------------------------------------------------------------
 Fill the notebook according to the current operating mode.
- Parameters -------------------------------------------------------------
 HWND hwnd : application window handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL setNotebookPages(HWND hwnd) {
   // fill the notebook control
   g.state |= STLRIS_INSERTINGPAGES;
   g.hwndNoteBook = DlgItemHwnd(hwnd, PREF_NOTEBOOK);
   switch (g.mode) {
      case IDX_USERINTERFACEPREF:           // user interface settings
         DlgItemShow(hwnd, BTN_PREVIEW, TRUE);
         if (!insertPage(0, 0, IDS_GENOPTIONS, 0,
                         uiGenPageProc, DLG_UIGENERAL,
                         BFA_MAJORTABTEXT, BKA_MAJOR)
             ||
             !insertPage(1,
                         (g.pCurOpts->ui.tb.on? IDS_PAGE1OF3 : 0),
                         IDS_TITLEBAR, IDS_GENOPTIONS,
                         uiTbarGenPageProc, DLG_TITLEBARGEN,
                         BFA_MINORTABTEXT | BFA_MAJORTABTEXT | BFA_STATUSLINE,
                         BKA_MAJOR)
             ||
             (
               g.pCurOpts->ui.tb.on
               &&
               (  !insertPage(2, IDS_PAGE2OF3, 0, IDS_ACTIVEWIN,
                              uiTbarPageProc, DLG_TITLEBAR,
                              BFA_MINORTABTEXT | BFA_STATUSLINE, 0)
                  ||
                  !insertPage(3, IDS_PAGE3OF3, 0, IDS_INACTIVEWIN,
                              uiTbarPageProc, DLG_TITLEBAR,
                              BFA_MINORTABTEXT | BFA_STATUSLINE, 0)
               )
             )
             ||
             !insertPage(4, 0, IDS_PUSHBUTTON, 0,
                         uiBtnPageProc, DLG_BTNOPT,
                         BFA_MAJORTABTEXT, BKA_MAJOR)
             ||
             !insertPage(5, 0, IDS_DIALOGOPT, 0,
                         uiDlgPageProc, DLG_DIALOG,
                         BFA_MAJORTABTEXT, BKA_MAJOR)
             ||
             !insertPage(6, 0, IDS_PRODINFO, 0,
                         prodInfoPageProc, DLG_PRODINFO,
                         BFA_MAJORTABTEXT, BKA_MAJOR))
            goto error;
         break;
      case IDX_SHUTDOWNPREF:     // shutdown settings
         if (!insertPage(0, 0, IDS_SHUTDOWN, 0,
                         sdGenPageProc, DLG_SDWNGEN,
                         BFA_MAJORTABTEXT, BKA_MAJOR)
             ||
             (g.pCurOpts->sd.gen.reboot
              &&
              !insertPage(1, 0, IDS_REBOOT, 0,
                          sdRebootPageProc, DLG_SDWNBOOTLIST,
                          BFA_MAJORTABTEXT, BKA_MAJOR))
             ||
             !insertPage(2, 0, IDS_SHUTDOWNPROGS, 0,
                         sdProgsPageProc, DLG_SDWNPROGS,
                         BFA_MAJORTABTEXT, BKA_MAJOR)
             ||
             !insertPage(3, 0, IDS_KILLLIST, 0,
                         sdKillPageProc, DLG_SDWNKILLPROG,
                         BFA_MAJORTABTEXT, BKA_MAJOR)
             ||
             !insertPage(4, 0, IDS_PRODINFO, 0,
                         prodInfoPageProc, DLG_PRODINFO,
                         BFA_MAJORTABTEXT, BKA_MAJOR))
            goto error;
         break;
      case IDX_WINDOWSKEYSPREF:      // Windows keys settings
         if (!insertPage(0, 0, IDS_WINKEYS, 0,
                         wkPageProc, DLG_WINKEY, BFA_MAJORTABTEXT, BKA_MAJOR)
             ||
             !insertPage(1, 0, IDS_PRODINFO, 0,
                         prodInfoPageProc, DLG_PRODINFO,
                         BFA_MAJORTABTEXT, BKA_MAJOR))
            goto error;
         break;
   } /* endswitch */
   g.state &= ~STLRIS_INSERTINGPAGES;
   return TRUE;
error:
   errorBox(ISERR_SETNOTEBOOKPAGES);
   return FALSE;
}


/* --------------------------------------------------------------------------
 Insert a notebook page.
- Parameters -------------------------------------------------------------
 ULONG idx          : zero based page index.
 ULONG idStatusText : string ID of the status text.
 ULONG idMajTabText : string ID of the major tab text.
 ULONG idMinTabText : string ID of the minor tab text.
 PFNWP pfnDlg       : page dialog procedure.
 ULONG dlgID        : page dialog id.
 ULONG flag         : BKA_* | (BFA_* << 16) flags.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL insertPage(ULONG idx, ULONG idStatusText,
                       ULONG idMajTabText, ULONG idMinTabText,
                       PFNWP pfnDlg, ULONG dlgID,
                       ULONG BFAflag, ULONG BKAflag) {
   BOOKPAGEINFO bpi;
   CHAR achStatus[256];
   CHAR achMajTab[256];
   CHAR achMinTab[256];
   memset(&bpi, 0, sizeof(bpi));
   if (0 == (g.aNBpageIDs[idx] = wnbkAppendPage(g.hwndNoteBook, BKAflag)))
      return FALSE;
   bpi.cb = sizeof(bpi);
   bpi.fl = BFA_PAGEFROMDLGRES | BFAflag;
   bpi.pfnPageDlgProc = (PFN)pfnDlg;
   bpi.idPageDlg = dlgID;
   bpi.hmodPageDlg = g.hResLib;
   if (idStatusText) {
      bpi.cbStatusLine = loadString(idStatusText, achStatus);
      bpi.pszStatusLine = achStatus;
   } /* endif */
   if (idMajTabText) {
      bpi.cbMajorTab = loadString(idMajTabText, achMajTab);
      bpi.pszMajorTab = achMajTab;
   } /* endif */
   if (idMinTabText) {
      bpi.cbMinorTab = loadString(idMinTabText, achMinTab);
      bpi.pszMinorTab = achMinTab;
   } /* endif */
   return wnbkSetPageInfo(g.hwndNoteBook, g.aNBpageIDs[idx], &bpi);
}


/* --------------------------------------------------------------------------
 Process page turned notification messages.
- Parameters -------------------------------------------------------------
 HWND hwnd : window handle
 ULONG id       : control ID
 ULONG event : notify code
 PPAGESELECTNOTIFY ppsn : page select notification structure.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static
VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, PPAGESELECTNOTIFY ppsn) {
   ULONG prevId;
   if (id == PREF_NOTEBOOK) {
      if (event == BKN_PAGESELECTEDPENDING) {
         // if the pages are disabled do not allow page selection
         if ((g.state & STLRIS_FEATUREDISABLED)
             &&
             (ppsn->ulPageIdNew != ppsn->ulPageIdCur)) {
            WinAlarm(HWND_DESKTOP, WA_WARNING);
            ppsn->ulPageIdNew = 0;
         } /* endif */
      // find the new page index and reset the buttons state
      } else if (event == BKN_PAGESELECTED) {
         MRESULT mr;
//         wnbkSetPageHelp(g.hwndHelp, g.appl.hwnd,
//                         g.hwndNoteBook, ppsn->ulPageIdNew);
         if (NULLHANDLE
             !=
             (hwnd = wnbkQueryPageHwnd(g.hwndNoteBook, ppsn->ulPageIdNew)))
         WinSendMsg(g.hwndHelp, HM_SET_ACTIVE_WINDOW,
                    (MPARAM)hwnd, (MPARAM)g.appl.hwnd);
         prevId = g.pageIdx;
         for (id = 0; id < NBP_UICOUNT; ++id) {
            if (g.aNBpageIDs[id] == ppsn->ulPageIdNew) {
               g.pageIdx = id;
               break;
            } /* endif */
         } /* endfor */
         // show/hide the preview window if needed
         if ((g.state & STLRIS_PREVIEWVISIBLE)
             &&
             mPreviewVisibilityChange(g.pageIdx, prevId)) {
            togglePreviewWindow(TRUE);
         } /* endif */
         if (id < NBP_UICOUNT) setCommonButtons(g.aNBpageFlags[id]);
      } /* endif */
   } /* endif */
}
