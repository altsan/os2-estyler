/* --------------------------------------------------------------------------
 pageUIDlg.c : dialogs settings dialog procedure.

 2004-6-4 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------



// prototypes ---------------------------------------------------------------
static BOOL onDlgInit(HWND hwnd);
static VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl);
static VOID onCmdMsg(HWND hwnd, ULONG id);
static VOID setControlsState(HWND hwnd);
static VOID shiftButtonAfterCheckbox(HWND hwnd, USHORT usBtnID, USHORT usChkID );
static VOID setEnableDependencies(HWND hwnd);
static VOID checkOptionsChanged(VOID);
static VOID checkApplyState(VOID);
static VOID checkUndoState(VOID);
static VOID checkDefaultState(VOID);
static VOID applyOptions(HWND hwnd);
static VOID undoOptions(HWND hwnd);
static VOID defaultOptions(HWND hwnd);
static VOID selectFont(HWND hwnd);

// global variables ---------------------------------------------------------

/* --------------------------------------------------------------------------
 Dialogs settings dialog procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY uiDlgPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         onDlgInit(hwnd);
         break;
      case WM_CONTROL:
         if (!(g.state & STLRIS_SKIPNOTIFICATION))
            onCtrlMsg(hwnd,  SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), (HWND)mp2);
         break;
      case WM_COMMAND:
         onCmdMsg(hwnd, (ULONG)mp1);
         break;
      case WM_PRESPARAMCHANGED:
         if ((LONG)mp1 == PP_FONTNAMESIZE)
             shiftButtonAfterCheckbox(hwnd, BTN_DLGFONT, CHK_DLGFONTON);
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
   setControlsState(hwnd);
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
   switch (id) {
      case CHK_DLGFONTON:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pUiData->pOpts->dlg.on = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
            setEnableDependencies(hwnd);
            updatePreviewWindow(PVUPD_DLGFONT);
         } /* endif */
         break;
      case CHK_DLGOVERPP:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pUiData->pOpts->dlg.overridePP = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
         } /* endif */
         break;
   } /* endswitch */
   if (bSettingsChanged) checkOptionsChanged();
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
      case BTN_APPLY:
         applyOptions(hwnd);
         break;
      case BTN_UNDO:
         undoOptions(hwnd);
         break;
      case BTN_DEFAULT:
         defaultOptions(hwnd);
         break;
      case BTN_DLGFONT:
         selectFont(hwnd);
         break;
   } /* endswitch */
}


/* --------------------------------------------------------------------------
 Set the state of the dialog controls according to the current value of
 the options.
- Parameters -------------------------------------------------------------
 HWND hwnd        : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setControlsState(HWND hwnd) {
   g.state |= STLRIS_SKIPNOTIFICATION;
   setCtrlTextParm(hwnd, CHK_DLGFONTON, IDS_DLGFONTON, g.pUiData->pOpts->dlg.achFont);

   // slide the font dialog btn leftward to the end of the checkbox
   shiftButtonAfterCheckbox(hwnd, BTN_DLGFONT, CHK_DLGFONTON);

   dBtnCheckSet(hwnd, CHK_DLGFONTON, g.pUiData->pOpts->dlg.on);
   dBtnCheckSet(hwnd, CHK_DLGOVERPP, g.pUiData->pOpts->dlg.overridePP);
   setEnableDependencies(hwnd);
   g.state &= ~STLRIS_SKIPNOTIFICATION;
}


/* --------------------------------------------------------------------------
 Reposition a button leftward to the end of a checkbox
- Parameters -------------------------------------------------------------
 HWND hwnd        : dialog window handle.
 USHORT usBtnID   : ID of button control
 USHORT usChkID   : ID of checkbox control
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID shiftButtonAfterCheckbox(HWND hwnd, USHORT usBtnID, USHORT usChkID ) {
   HWND hCtl;
   LONG x;
   SWP  swp;

   hCtl = WinWindowFromID(hwnd, usChkID);
   WinSendMsg(hCtl, BM_AUTOSIZE, 0, 0);
   WinQueryWindowPos(hCtl, &swp);
   x = swp.x + swp.cx + 8;
   hCtl = WinWindowFromID(hwnd, usBtnID);
   WinQueryWindowPos(hCtl, &swp);
   WinSetWindowPos(hCtl, 0, x, swp.y, 0, 0, SWP_MOVE | SWP_NOREDRAW);
}


/* --------------------------------------------------------------------------
 Enable/disable the secondary controls according to the check state of the
 'Use <font> for dialog windows' checkbox.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setEnableDependencies(HWND hwnd) {
   BOOL fEnable = g.pUiData->pOpts->dlg.on;
   BOOL fShow = g.pUiData->pOpts->dlg.overrideFont;

   WinEnableWindow(WinWindowFromID(hwnd, CHK_DLGOVERPP), fEnable);
   WinEnableWindow(WinWindowFromID(hwnd, BTN_DLGFONT), (fShow && fEnable));
   WinShowWindow(  WinWindowFromID(hwnd, BTN_DLGFONT), fShow);
}


/* --------------------------------------------------------------------------
 Check if according to the current settings the state of the "Apply", "Undo"
 and "Default" buttons must be changed.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID checkOptionsChanged(VOID) {
   checkApplyState();
   checkUndoState();
   checkDefaultState();
}


/* --------------------------------------------------------------------------
 Compare the options set in the dialog with the active options.
 If there is any different option enable the 'Apply' button.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID checkApplyState(VOID) {
   BOOL bEnable;
   bEnable = (g.pUiData->pOpts->dlg.overrideFont &&
              strcmp(g.pUiData->pOpts->dlg.achFont, g.pCurOpts->ui.dlg.achFont))
             ||
             (g.pUiData->pOpts->dlg.on != g.pCurOpts->ui.dlg.on)
             ||
             (g.pUiData->pOpts->dlg.overridePP != g.pCurOpts->ui.dlg.overridePP)
             ;
   if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_APPLY))
      resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED,
                        (bEnable ? PGFL_APPLY_ENABLED : 0));
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
   bEnable = (g.pUiData->pOpts->dlg.overrideFont &&
              strcmp(g.pUiData->pOpts->dlg.achFont, g.pUndoUiOpts->dlg.achFont))
             ||
             (g.pUiData->pOpts->dlg.on != g.pUndoUiOpts->dlg.on)
             ||
             (g.pUiData->pOpts->dlg.overridePP != g.pUndoUiOpts->dlg.overridePP)
             ;
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
   bEnable = (g.pUiData->pOpts->dlg.overrideFont &&
              strcmp(g.pUiData->pOpts->dlg.achFont, SZ_DEFDLGFONT))
             ||
             !g.pUiData->pOpts->dlg.on
             ||
             g.pUiData->pOpts->dlg.overridePP
             ;
   if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_DEFAULT))
      resetCommonButton(BTN_DEFAULT, PGFL_DEFAULT_ENABLED,
                        (bEnable ? PGFL_DEFAULT_ENABLED : 0));
}


/* --------------------------------------------------------------------------
 Apply the current settings.
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID applyOptions(HWND hwnd) {
   memcpy(&g.pCurOpts->ui.dlg, &g.pUiData->pOpts->dlg, sizeof(DLGOPTIONS));
#ifndef _STLR_TEST_MODE_
   m_stlrMsgBroadcast(STRLM_DLGWINFONT, 0);
#endif
   resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED, 0);
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
   if (g.pUiData->pOpts->dlg.overrideFont)
      strcpy(g.pUiData->pOpts->dlg.achFont, g.pUndoUiOpts->dlg.achFont);
   g.pUiData->pOpts->dlg.on = g.pUndoUiOpts->dlg.on;
   g.pUiData->pOpts->dlg.overridePP = g.pUndoUiOpts->dlg.overridePP;
   setControlsState(hwnd);
   updatePreviewWindow(PVUPD_DLGFONT);
   checkApplyState();
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
   stlrSetDlgDefaults(&g.pUiData->pOpts->dlg);
   setControlsState(hwnd);
   updatePreviewWindow(PVUPD_DLGFONT);
   checkApplyState();
   checkUndoState();
   resetCommonButton(BTN_DEFAULT, PGFL_DEFAULT_ENABLED, 0);
}


/* --------------------------------------------------------------------------
 Show the font dialog to allow the selection of the default dialog font.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID selectFont(HWND hwnd) {
   if (fontDlg(hwnd, IDS_DIALOGFONT, IDS_FONTDLGSAMPLE,
               g.pUiData->pOpts->dlg.achFont)) {
      setCtrlTextParm(hwnd, CHK_DLGFONTON, IDS_DLGFONTON, g.pUiData->pOpts->dlg.achFont);
      checkOptionsChanged();
      updatePreviewWindow(PVUPD_DLGFONT);
   } /* endif */
}
