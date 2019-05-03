/* --------------------------------------------------------------------------
 pageUITbarGen.c : titlebar - general options - settings dialog procedure.

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
static BOOL onDlgInit(HWND hwnd);
static VOID setControlsState(HWND hwnd, BOOL bSetSubPages);
static VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl);
static VOID onCmdMsg(HWND hwnd, ULONG id);
static VOID enablePageControls(HWND hwnd, BOOL flag);
static VOID checkOptionsChanged(VOID);
static VOID checkApplyState(VOID);
static VOID checkUndoState(VOID);
static VOID checkDefaultState(VOID);
static VOID applyOptions(HWND hwnd);
static VOID undoOptions(HWND hwnd);
static VOID defaultOptions(HWND hwnd);
static VOID selectFont(HWND hwnd);
static BOOL setSubPages(HWND hwnd, BOOL bIns);
static ULONG insertPage(ULONG idIns, ULONG idStatusText, ULONG idMinTabText,
                 HWND hwndPage);
static VOID updateStatusLineText(ULONG idPage, ULONG idText);

// global variables ---------------------------------------------------------

/* --------------------------------------------------------------------------
 Titlebar - general options - settings dialog procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY uiTbarGenPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
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
   setControlsState(hwnd, FALSE);
   checkDefaultState();
   return TRUE;
}


/* --------------------------------------------------------------------------
 Set the state of the dialog controls according to the current value of
 the options.
- Parameters -------------------------------------------------------------
 HWND hwnd         : dialog window handle.
 BOOL bSetSubPages : TRUE/FALSE (change/ignore the state of the sub-pages).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setControlsState(HWND hwnd, BOOL bSetSubPages) {
   g.state |= STLRIS_SKIPNOTIFICATION;
   dBtnCheckSet(hwnd, CHK_TBENABLE, g.pUiData->pOpts->tb.on);
   if (bSetSubPages) setSubPages(hwnd, g.pUiData->pOpts->tb.on);
   enablePageControls(hwnd, g.pUiData->pOpts->tb.on);
   dBtnCheckSet(hwnd, CHK_OVERRIDEPP, g.pUiData->pOpts->tb.overridePP);
   setCtrlTextParm(hwnd, TXT_TBFONT, IDS_FONT_, g.pUiData->pOpts->tb.achFont);
   dLbxItemSelect(hwnd, COMBO_TBALIGN, g.pUiData->pOpts->tb.center);
   g.state &= ~STLRIS_SKIPNOTIFICATION;
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
      case CHK_TBENABLE:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pUiData->pOpts->tb.on = wBtnCheckState(hCtrl);
            enablePageControls(hwnd, g.pUiData->pOpts->tb.on);
            setSubPages(hwnd, g.pUiData->pOpts->tb.on);
            bSettingsChanged = TRUE;
            updatePreviewWindow(PVUPD_ALL);
         } /* endif */
         break;
      case CHK_OVERRIDEPP:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pUiData->pOpts->tb.overridePP = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
         } /* endif */
         break;
      case COMBO_TBALIGN:
         if (event == LN_SELECT) {
            INT iitem;
            if (LIT_NONE != (iitem = wLbxItemSelected(hCtrl))) {
               g.pUiData->pOpts->tb.center = iitem;
               bSettingsChanged = TRUE;
               updatePreviewWindow(PVUPD_TITLEBARS);
            } /* endif */
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
      case BTN_TBFONT:
         selectFont(hwnd);
         break;
   } /* endswitch */
}


/* --------------------------------------------------------------------------
 Enable/disable the dialog controls according to the state of the
 "Enable titlebar enhancements" checkbox.
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
 BOOL flag : TRUE/FALSE (enable/diable controls).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID enablePageControls(HWND hwnd, BOOL flag) {
   WinEnableControl(hwnd, CHK_OVERRIDEPP, flag);
   WinEnableControl(hwnd, TXT_TBFONT, flag);
   WinEnableControl(hwnd, BTN_TBFONT, flag);
   WinEnableControl(hwnd, TXT_TBALIGN, flag);
   WinEnableControl(hwnd, COMBO_TBALIGN, flag);
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
   bEnable = ((g.pUiData->pOpts->tb.on != g.pCurOpts->ui.tb.on)
              ||
             (g.pUiData->pOpts->tb.overridePP != g.pCurOpts->ui.tb.overridePP)
              ||
              (g.pUiData->pOpts->tb.center != g.pCurOpts->ui.tb.center)
              ||
              strcmp(g.pUiData->pOpts->tb.achFont, g.pCurOpts->ui.tb.achFont)
             );
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
   bEnable = ((g.pUiData->pOpts->tb.on != g.pUndoUiOpts->tb.on)
              ||
             (g.pUiData->pOpts->tb.overridePP != g.pUndoUiOpts->tb.overridePP)
              ||
              (g.pUiData->pOpts->tb.center != g.pUndoUiOpts->tb.center)
              ||
              strcmp(g.pUiData->pOpts->tb.achFont, g.pUndoUiOpts->tb.achFont)
             );
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
   bEnable = (!g.pUiData->pOpts->tb.on
              ||
              !g.pUiData->pOpts->tb.overridePP
              ||
              !g.pUiData->pOpts->tb.center
              ||
              strcmp(g.pUiData->pOpts->tb.achFont, "9.WarpSans Bold")
             );
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
   g.pCurOpts->ui.tb.on = g.pUiData->pOpts->tb.on;
   g.pCurOpts->ui.tb.overridePP = g.pUiData->pOpts->tb.overridePP;
   g.pCurOpts->ui.tb.center = g.pUiData->pOpts->tb.center;
   strcpy(g.pCurOpts->ui.tb.achFont, g.pUiData->pOpts->tb.achFont);
   m_stlrMsgBroadcast(STLRM_REDRAWTITLE, MODIF_FONT);
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
   g.pUiData->pOpts->tb.on = g.pUndoUiOpts->tb.on;
   g.pUiData->pOpts->tb.overridePP = g.pUndoUiOpts->tb.overridePP;
   g.pUiData->pOpts->tb.center = g.pUndoUiOpts->tb.center;
   strcpy(g.pUiData->pOpts->tb.achFont, g.pUndoUiOpts->tb.achFont);
   setControlsState(hwnd, TRUE);
   updatePreviewWindow(PVUPD_TITLEFONT | PVUPD_TITLEBARS);
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
   stlrSetTitlebarGeneralDefaults(&g.pUiData->pOpts->tb);
   setControlsState(hwnd, TRUE);
   updatePreviewWindow(PVUPD_TITLEFONT | PVUPD_TITLEBARS);
   checkApplyState();
   checkUndoState();
   resetCommonButton(BTN_DEFAULT, PGFL_DEFAULT_ENABLED, 0);
}


/* --------------------------------------------------------------------------
 Show the font dialog to allow the selection of the titlebar font.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID selectFont(HWND hwnd) {
   if (fontDlg(hwnd, IDS_TITLEBARFONT, IDS_FONTDLGSAMPLE,
               g.pUiData->pOpts->tb.achFont)) {
      setCtrlTextParm(hwnd, TXT_TBFONT, IDS_FONT_, g.pUiData->pOpts->tb.achFont);
      checkOptionsChanged();
      updatePreviewWindow(PVUPD_TITLEFONT | PVUPD_TITLEBARS);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Add/remove the Active/Inactive titlebar pages page according to the state
 of the "Enable titlebar enhancements" option.
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
 BOOL bIns : TRUE/FALSE (insert/remove the sub-pages).
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL setSubPages(HWND hwnd, BOOL bIns) {
   ULONG id;
   // page insertion
   if (bIns) {
      id = insertPage(g.aNBpageIDs[NBP_UITITLEBARGEN], IDS_PAGE2OF3,
                      IDS_ACTIVEWIN, g.pUiData->hwndActiveTbar);
      if (!id) goto error;
      g.aNBpageIDs[NBP_UITITLEBARACTIVE] = id;
      id = insertPage(id, IDS_PAGE3OF3, IDS_INACTIVEWIN,
                      g.pUiData->hwndInactiveTbar);
      if (id) {
         g.aNBpageIDs[NBP_UITITLEBARINACTIVE] = id;
         updateStatusLineText(g.aNBpageIDs[NBP_UITITLEBARGEN], IDS_PAGE1OF3);
      } else {
         wnbkDeletePage(g.hwndNoteBook, NBP_UITITLEBARACTIVE);
         g.aNBpageIDs[NBP_UITITLEBARACTIVE] = 0;
         goto error;
      } /* endif */
   // page deletion
   } else {
      wnbkDeletePage(g.hwndNoteBook, g.aNBpageIDs[NBP_UITITLEBARACTIVE]);
      g.aNBpageIDs[NBP_UITITLEBARACTIVE] = 0;
      wnbkDeletePage(g.hwndNoteBook, g.aNBpageIDs[NBP_UITITLEBARINACTIVE]);
      g.aNBpageIDs[NBP_UITITLEBARINACTIVE] = 0;
      updateStatusLineText(g.aNBpageIDs[NBP_UITITLEBARGEN], 0);
   } /* endif */
   return TRUE;
   // error handling : show error message and reset dialog to previous state
error:
   errorBox(ISERR_INSPAGE);
   g.state |= STLRIS_SKIPNOTIFICATION;
   g.pUiData->pOpts->tb.on = 0;
   dBtnCheckSet(hwnd, CHK_TBENABLE, 0);
   g.state &= ~STLRIS_SKIPNOTIFICATION;
   return FALSE;
}


/* --------------------------------------------------------------------------
 Insert a notebook page.
- Parameters -------------------------------------------------------------
 ULONG idIns        : id of the previous page (insertion point).
 ULONG idStatusText : string ID of the status text.
 ULONG idMinTabText : string ID of the minor tab text.
 HWND hwndPage      : handle of the dialog page (if previously initialized).
- Return value -----------------------------------------------------------
 ULONG id : id of the inserted page, 0 in case of error
-------------------------------------------------------------------------- */
static ULONG insertPage(ULONG idIns, ULONG idStatusText, ULONG idMinTabText,
                        HWND hwndPage) {
   BOOKPAGEINFO bpi;
   CHAR achStatusLine[256];
   CHAR achMinTabText[256];
   ULONG id;
   id = wnbkInsertPage(g.hwndNoteBook, BKA_STATUSTEXTON | BKA_MINOR,
                       idIns, BKA_NEXT);
   if (id) {
      memset(&bpi, 0, sizeof(bpi));
      bpi.cb = sizeof(bpi);
      bpi.cbStatusLine = loadString(idStatusText, achStatusLine);
      bpi.pszStatusLine = achStatusLine;
      bpi.cbMinorTab = loadString(idMinTabText, achMinTabText);
      bpi.pszMinorTab = achMinTabText;
      bpi.fl = BFA_MINORTABTEXT | BFA_STATUSLINE;
      // dialog already loaded : just set the page window handle
      if (hwndPage) {
         bpi.fl |= BFA_PAGEFROMHWND;
         bpi.hwndPage = hwndPage;
      // set the page information and let the notebook load the dialog
      } else {
         bpi.fl |= BFA_PAGEFROMDLGRES;
         bpi.pfnPageDlgProc = (PFN)uiTbarPageProc;
         bpi.idPageDlg = DLG_TITLEBAR;
         bpi.hmodPageDlg = g.hResLib;
      } /* endif */
      if (!wnbkSetPageInfo(g.hwndNoteBook, id, &bpi)) {
         wnbkDeletePage(g.hwndNoteBook, id);
         return 0;
      } /* endif */
   } /* endif */
   return id;
}


/* --------------------------------------------------------------------------
 Update the status line of a notebook page.
- Parameters -------------------------------------------------------------
 ULONG idPage : notebook page ID.
 ULONG idText : resource string ID of the status line text.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID updateStatusLineText(ULONG idPage, ULONG idText) {
   CHAR buf[256];
   if (idText) {
      loadString(idText, buf);
   } else {
      buf[0] = 0;
   } /* endif */
   wnbkSetStatuslineText(g.hwndNoteBook, idPage, buf);
}
