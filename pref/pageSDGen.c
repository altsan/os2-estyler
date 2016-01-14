/* --------------------------------------------------------------------------
 pageSDGen.c : shutdown - general options - settings dialog procedure.

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
BOOL onDlgInit(HWND hwnd);
VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl);
VOID onCmdMsg(HWND hwnd, ULONG id);
VOID setControlsState(HWND hwnd, BOOL bSetRebootPage);
VOID checkUndoState(VOID);
VOID checkDefaultState(VOID);
VOID undoOptions(HWND hwnd);
VOID defaultOptions(HWND hwnd);
BOOL setRebootPage(HWND hwnd, BOOL bShow);

// global variables ---------------------------------------------------------

/* --------------------------------------------------------------------------
 Shutdown - general options - settings dialog procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY sdGenPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         onDlgInit(hwnd);
         break;
      case WM_CONTROL:
         if (!(g.state & STLRIS_SKIPNOTIFICATION))
            onCtrlMsg(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), (HWND)mp2);
         break;
      case WM_COMMAND:
         onCmdMsg(hwnd, (ULONG)mp1);
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}

/* --------------------------------------------------------------------------
 initialization.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL onDlgInit(HWND hwnd) {
   initPage(hwnd);
   setCommonButtons(g.aNBpageFlags[0]);
   setControlsState(hwnd, FALSE);
   checkDefaultState();
   return TRUE;
}


/* --------------------------------------------------------------------------
 Process notification messages.
- Parameters -------------------------------------------------------------
 HWND hwnd : window handle
 ULONG id       : control ID
 ULONG event : notify code
 HWND hCtrl     : control handle
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl) {
   BOOL bSettingsChanged = FALSE;
   LONG lValue;
   switch (id) {
      case CHK_SDWNON:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pCurOpts->sd.gen.on = wBtnCheckState(hCtrl);
            dlgItemMultiEnable(hwnd, GRP_SDWNINCLUDE, CHK_SDWNANIMATE,
                               g.pCurOpts->sd.gen.on);
            dlgItemMultiEnable(hwnd, TXT_SDNWSTEPS, SPIN_SDWNSTEPS,
                               g.pCurOpts->sd.gen.on && g.pCurOpts->sd.gen.anim);
            DlgItemEnable(hwnd, BTN_SDWNADVANCDED, g.pCurOpts->sd.gen.on);
            enableNotebookTabs(g.hwndNoteBook, g.pCurOpts->sd.gen.on);
            bSettingsChanged = TRUE;
         } /* endif */
         break;
      case CHK_SDWNLKSUSP:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pCurOpts->sd.gen.lksusp = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
         } /* endif */
         break;
      case CHK_SDWNSUSP:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pCurOpts->sd.gen.susp = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
         } /* endif */
         break;
      case CHK_SDWNOFF:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pCurOpts->sd.gen.off = wBtnCheckState(hCtrl);
            // at least one of the two checkboxes : "Power off" and
            // "Shut down" must be enabled
            if (!(g.pCurOpts->sd.gen.off || g.pCurOpts->sd.gen.ord)) {
               g.state |= STLRIS_SKIPNOTIFICATION;
               dBtnCheckSet(hwnd, CHK_SDWNORD, (g.pCurOpts->sd.gen.ord = 1));
               g.state &= ~STLRIS_SKIPNOTIFICATION;
            } /* endif */
            bSettingsChanged = TRUE;
         } /* endif */
         break;
      case CHK_SDWNORD:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pCurOpts->sd.gen.ord = wBtnCheckState(hCtrl);
            // at least one of the two checkboxes : "Power off" and
            // "Shut down" must be enabled
            if (!(g.pCurOpts->sd.gen.off || g.pCurOpts->sd.gen.ord)) {
               g.state |= STLRIS_SKIPNOTIFICATION;
               dBtnCheckSet(hwnd, CHK_SDWNOFF, (g.pCurOpts->sd.gen.off = 1));
               g.state &= ~STLRIS_SKIPNOTIFICATION;
            } /* endif */
            bSettingsChanged = TRUE;
         } /* endif */
         break;
      case CHK_SDWNREBOOT:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pCurOpts->sd.gen.reboot = wBtnCheckState(hCtrl);
            setRebootPage(hwnd, g.pCurOpts->sd.gen.reboot);
            bSettingsChanged = TRUE;
         } /* endif */
         break;
      case CHK_SDWNANIMATE:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pCurOpts->sd.gen.anim = wBtnCheckState(hCtrl);
            dlgItemMultiEnable(hwnd, TXT_SDNWSTEPS, SPIN_SDWNSTEPS,
                               g.pCurOpts->sd.gen.anim);
            bSettingsChanged = TRUE;
         } /* endif */
         break;
      case SPIN_SDWNSTEPS:
         if (handleSpinbtnEvent(hCtrl, event, &lValue,
                                g.pCurOpts->sd.gen.steps)) {
            g.pCurOpts->sd.gen.steps = (USHORT)lValue;
            bSettingsChanged = TRUE;
         } /* endif */
         break;
   } /* endswitch */
   if (bSettingsChanged) {
      checkUndoState();
      checkDefaultState();
   } /* endif */
}


/* --------------------------------------------------------------------------
 Process command events.
- Parameters -------------------------------------------------------------
 HWND hwnd : window handle
 ULONG id  : button ID.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID onCmdMsg(HWND hwnd, ULONG id) {
   switch (id) {
      case BTN_UNDO:
         undoOptions(hwnd);
         break;
      case BTN_DEFAULT:
         defaultOptions(hwnd);
         break;
      case BTN_SDWNADVANCDED:
         WinDlgBox(HWND_DESKTOP, hwnd, sdTimeOptsProc, g.hResLib, DLG_SDTIMING,
                   NULL);
         break;
   } /* endswitch */
}


/* --------------------------------------------------------------------------
 Set the state of the dialog controls according to the current value of
 the options.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
 BOOL bSetRebootPage: TRUE/FALSE (change/ignore Reboot page state).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setControlsState(HWND hwnd, BOOL bSetRebootPage) {
   g.state |= STLRIS_SKIPNOTIFICATION;
   dBtnCheckSet(hwnd, CHK_SDWNON, g.pCurOpts->sd.gen.on);
   dBtnCheckSet(hwnd, CHK_SDWNLKSUSP, g.pCurOpts->sd.gen.lksusp);
   dBtnCheckSet(hwnd, CHK_SDWNSUSP, g.pCurOpts->sd.gen.susp);
   dBtnCheckSet(hwnd, CHK_SDWNOFF, g.pCurOpts->sd.gen.off);
   dBtnCheckSet(hwnd, CHK_SDWNORD, g.pCurOpts->sd.gen.ord);
   dBtnCheckSet(hwnd, CHK_SDWNREBOOT, g.pCurOpts->sd.gen.reboot);
   if (bSetRebootPage) setRebootPage(hwnd, g.pCurOpts->sd.gen.reboot);
   dBtnCheckSet(hwnd, CHK_SDWNANIMATE, g.pCurOpts->sd.gen.anim);
   dSpinBtnSetValue(hwnd, SPIN_SDWNSTEPS, g.pCurOpts->sd.gen.steps);
   dlgItemMultiEnable(hwnd, GRP_SDWNINCLUDE, CHK_SDWNANIMATE,
                      g.pCurOpts->sd.gen.on);
   dlgItemMultiEnable(hwnd, TXT_SDNWSTEPS, SPIN_SDWNSTEPS,
                      g.pCurOpts->sd.gen.on && g.pCurOpts->sd.gen.anim);
   DlgItemEnable(hwnd, BTN_SDWNADVANCDED, g.pCurOpts->sd.gen.on);
   enableNotebookTabs(g.hwndNoteBook, g.pCurOpts->sd.gen.on);
   g.state &= ~STLRIS_SKIPNOTIFICATION;
}


/* --------------------------------------------------------------------------
 Compare the options set in the dialog with the options active when the
 settings dialog was started.
 If there is any different option enable the 'Undo' button.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID checkUndoState(VOID) {
   BOOL bEnable;
   bEnable = memcmp(&g.pCurOpts->sd.gen, &g.pUndoSdOpts->gen, sizeof(SDGENERAL));
   if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_UNDO))
      resetCommonButton(BTN_UNDO, PGFL_UNDO_ENABLED,
                        (bEnable ? PGFL_UNDO_ENABLED : 0));
}


/* --------------------------------------------------------------------------
 Compare the options set in the dialog with the default options.
 If there is any different option enable the 'Default' button.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID checkDefaultState(VOID) {
   BOOL bEnable;
   SDGENERAL sdgen;
   memset(&sdgen, 0, sizeof(sdgen));
   stlrSetShutdownDefaults(NULLHANDLE, &sdgen);
   bEnable = memcmp(&g.pCurOpts->sd.gen, &sdgen, sizeof(SDGENERAL));
   if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_DEFAULT))
      resetCommonButton(BTN_DEFAULT, PGFL_DEFAULT_ENABLED,
                        (bEnable ? PGFL_DEFAULT_ENABLED : 0));
}


/* --------------------------------------------------------------------------
 Reset the current settings to the values they had when the preferences
 notebook was opened.
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID undoOptions(HWND hwnd) {
   memcpy(&g.pCurOpts->sd.gen, &g.pUndoSdOpts->gen, sizeof(SDGENERAL));
   setControlsState(hwnd, TRUE);
   checkDefaultState();
   resetCommonButton(BTN_UNDO, PGFL_UNDO_ENABLED, 0);
}


/* --------------------------------------------------------------------------
 Reset the current settings to the default values.
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID defaultOptions(HWND hwnd) {
   stlrSetShutdownDefaults(NULLHANDLE, &g.pCurOpts->sd.gen);
   setControlsState(hwnd, TRUE);
   checkUndoState();
   resetCommonButton(BTN_DEFAULT, PGFL_DEFAULT_ENABLED, 0);
}


/* --------------------------------------------------------------------------
 Display/hide the reboot page according to the state of the reboot option.
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
 BOOL bShow : TRUE/FALSE (show/hide the reboot page).
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL setRebootPage(HWND hwnd, BOOL bShow) {
   BOOKPAGEINFO bpi;
   CHAR buf[256];
   ULONG id;
   // page insertion
   if (bShow) {
      id = wnbkInsertPage(g.hwndNoteBook, BKA_MAJOR,
                          g.aNBpageIDs[NBP_SDGEN], BKA_NEXT);
      if (!id) goto error;
      memset(&bpi, 0, sizeof(bpi));
      bpi.cb = sizeof(bpi);
      bpi.cbMajorTab = loadString(IDS_REBOOT, buf);
      bpi.pszMajorTab = buf;
      // dialog already loaded : just set the page window handle
      if (g.pShdwnData->hwndReboot) {
         bpi.fl = BFA_PAGEFROMHWND | BFA_MAJORTABTEXT;
         bpi.hwndPage = g.pShdwnData->hwndReboot;
      // set the page information and let the notebook load the dialog
      } else {
         bpi.fl = BFA_PAGEFROMDLGRES | BFA_MAJORTABTEXT;
         bpi.pfnPageDlgProc = (PFN)sdRebootPageProc;
         bpi.idPageDlg = DLG_SDWNBOOTLIST;
         bpi.hmodPageDlg = g.hResLib;
      } /* endif */
      if (!wnbkSetPageInfo(g.hwndNoteBook, id, &bpi))
         goto error;
      g.aNBpageIDs[NBP_SDREBOOT] = id;
   // page deletion
   } else {
      wnbkDeletePage(g.hwndNoteBook, g.aNBpageIDs[NBP_SDREBOOT]);
      g.aNBpageIDs[NBP_SDREBOOT] = 0;
   } /* endif */
   return TRUE;
   // error handling : show error message and reset dialog to previous state
error:
   if (id) wnbkDeletePage(g.hwndNoteBook, id);
   errorBox(ISERR_INSPAGE);
   g.state |= STLRIS_SKIPNOTIFICATION;
   g.pCurOpts->sd.gen.reboot = 0;
   dBtnCheckSet(hwnd, CHK_SDWNREBOOT, g.pCurOpts->sd.gen.reboot);
   g.state &= ~STLRIS_SKIPNOTIFICATION;
   return FALSE;
}


