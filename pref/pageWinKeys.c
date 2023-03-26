/* --------------------------------------------------------------------------
 pageWinKeys.c : Windows keys settings dialog procedure.

 2004-6-4 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------
#define CHECK_UNDOSTATE      2
#define CHECK_DEFAULTSTATE   4



// prototypes ---------------------------------------------------------------
static BOOL onDlgInit(HWND hwnd);
static VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl);
static VOID onCmdMsg(HWND hwnd, ULONG id);
static VOID initDropDownList(HWND hwnd, ULONG id);
static VOID setControlsState(HWND hwnd);
static VOID checkOptionsChanged(HWND hwnd, ULONG fl);
static VOID undoOptions(HWND hwnd);
static VOID defaultOptions(HWND hwnd);

// global variables ---------------------------------------------------------

/* --------------------------------------------------------------------------
 Windows keys settings dialog procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY wkPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
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
   initPage(hwnd);
   setCommonButtons(g.aNBpageFlags[0]);
   initDropDownList(hwnd, COMBO_WKLEFT);
   initDropDownList(hwnd, COMBO_WKRIGHT);
   initDropDownList(hwnd, COMBO_WKMENU);
   setControlsState(hwnd);
   checkOptionsChanged(hwnd, CHECK_DEFAULTSTATE);
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
      case CHK_WINKEYON:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pCurOpts->wk.on = wBtnCheckState(hCtrl);
            dlgItemMultiEnable(hwnd, TXT_WKEYLEFT, COMBO_WKMENU,
                               g.pCurOpts->wk.on);
            bSettingsChanged = TRUE;
         } /* endif */
         break;
      case COMBO_WKLEFT:
         if (event == CBN_LBSELECT) {
            g.pCurOpts->wk.left = wLbxItemSelected(hCtrl);
            bSettingsChanged = TRUE;
         } /* endif */
         break;
      case COMBO_WKRIGHT:
         if (event == CBN_LBSELECT) {
            g.pCurOpts->wk.right = wLbxItemSelected(hCtrl);
            bSettingsChanged = TRUE;
         } /* endif */
         break;
      case COMBO_WKMENU:
         if (event == CBN_LBSELECT) {
            g.pCurOpts->wk.menu = wLbxItemSelected(hCtrl);
            bSettingsChanged = TRUE;
         } /* endif */
         break;
   } /* endswitch */
   if (bSettingsChanged)
      checkOptionsChanged(hwnd, CHECK_UNDOSTATE | CHECK_DEFAULTSTATE);
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
   } /* endswitch */
}


/* --------------------------------------------------------------------------
 Fill a drop down list with the valid options.
- Parameters -------------------------------------------------------------
 HWND hwnd : window handle
 ULONG id  : drop down list id
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID initDropDownList(HWND hwnd, ULONG id) {
   CHAR buf[256];
   hwnd = WinWindowFromID(hwnd, id);
   for (id = IDS_WKEYITEM01; id <= IDS_WKEYITEM29; ++id) {
      if (loadString(id, buf)) wLbxItemIns(hwnd, LIT_END, buf);
   } /* endfor */
}


/* --------------------------------------------------------------------------
 Set the state of the dialog controls according to the current value of
 the options.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setControlsState(HWND hwnd) {
   g.state |= STLRIS_SKIPNOTIFICATION;
   dBtnCheckSet(hwnd, CHK_WINKEYON, g.pCurOpts->wk.on);
   dLbxItemSelect(hwnd, COMBO_WKLEFT, g.pCurOpts->wk.left);
   dLbxItemSelect(hwnd, COMBO_WKRIGHT, g.pCurOpts->wk.right);
   dLbxItemSelect(hwnd, COMBO_WKMENU, g.pCurOpts->wk.menu);
   dlgItemMultiEnable(hwnd, TXT_WKEYLEFT, COMBO_WKMENU, g.pCurOpts->wk.on);
   g.state &= ~STLRIS_SKIPNOTIFICATION;
}


/* --------------------------------------------------------------------------
 Compare the options set in the dialog with the active options.
 If there is any different option enable the 'Undo' and or the 'Default'
 button.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
 ULONG fl  : options to check.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID checkOptionsChanged(HWND hwnd, ULONG fl) {
   BOOL bEnable;
   WINKEY wk;
   if (fl & CHECK_UNDOSTATE) {
      bEnable = memcmp(&g.pCurOpts->wk, g.pUndoWkOpts, sizeof(WINKEY));
      if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_UNDO))
         resetCommonButton(BTN_UNDO, PGFL_UNDO_ENABLED,
                           (bEnable ? PGFL_UNDO_ENABLED : 0));
   } /* endif */
   if (fl & CHECK_DEFAULTSTATE) {
      memset(&wk, 0, sizeof(wk));
      bEnable = memcmp(&g.pCurOpts->wk, &wk, sizeof(WINKEY));
      if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_DEFAULT))
         resetCommonButton(BTN_DEFAULT, PGFL_DEFAULT_ENABLED,
                           (bEnable ? PGFL_DEFAULT_ENABLED : 0));
   } /* endif */
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
   memcpy(&g.pCurOpts->wk, g.pUndoWkOpts, sizeof(WINKEY));
   setControlsState(hwnd);
   checkOptionsChanged(hwnd, CHECK_DEFAULTSTATE);
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
   memset(&g.pCurOpts->wk, 0, sizeof(WINKEY));
   setControlsState(hwnd);
   checkOptionsChanged(hwnd, CHECK_UNDOSTATE);
   resetCommonButton(BTN_DEFAULT, PGFL_DEFAULT_ENABLED, 0);
}

