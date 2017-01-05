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
   INT i;
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
   dBtnCheckSet(hwnd, CHK_DLGFONTON, g.pUiData->pOpts->dlg.on);
   dBtnCheckSet(hwnd, CHK_DLGOVERPP, g.pUiData->pOpts->dlg.overridePP);
   setCtrlTextParm(hwnd, TXT_DLGFONT, IDS_FONT_, g.pUiData->pOpts->dlg.achFont);
   setEnableDependencies(hwnd);
   g.state &= ~STLRIS_SKIPNOTIFICATION;
}


/* --------------------------------------------------------------------------
 Enable/disable the secondary controls according to the check state of the
 'Set a default font for the dialog windows' checkbox.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setEnableDependencies(HWND hwnd) {
   INT i;
   for (i = CHK_DLGOVERPP; i <= BTN_DLGFONT; ++i)
      DlgItemEnable(hwnd, i, g.pUiData->pOpts->dlg.on);
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
   bEnable = strcmp(g.pUiData->pOpts->dlg.achFont, g.pCurOpts->ui.dlg.achFont)
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
   bEnable = strcmp(g.pUiData->pOpts->dlg.achFont, g.pUndoUiOpts->dlg.achFont)
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
   bEnable = strcmp(g.pUiData->pOpts->dlg.achFont, SZ_DEFDLGFONT)
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
      setCtrlTextParm(hwnd, TXT_DLGFONT, IDS_FONT_, g.pUiData->pOpts->dlg.achFont);
      checkOptionsChanged();
      updatePreviewWindow(PVUPD_DLGFONT);
   } /* endif */
}
