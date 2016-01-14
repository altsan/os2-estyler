/* --------------------------------------------------------------------------
 pageUiGen.c : User interface general settings dialog procedure.

 2004-6-4 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------



// prototypes ---------------------------------------------------------------
BOOL onDlgInit(HWND hwnd);
VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl);
VOID onCmdMsg(HWND hwnd, ULONG id);
VOID enablePageControls(HWND hwnd, BOOL flag);
BOOL initExceptionsList(HWND hwnd);
BOOL saveExceptions(HWND hwnd);
VOID addExe(HWND hwnd);
VOID remExe(HWND hwnd);
VOID findExe(HWND hwnd);

// global variables ---------------------------------------------------------

/* --------------------------------------------------------------------------
 User interface general settings dialog procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY uiGenPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         onDlgInit(hwnd);
         break;
      case WM_CONTROL:
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
   dBtnCheckSet(hwnd, CHK_UIENABLE, !g.pCurOpts->ui.disabled);
   enablePageControls(hwnd, !g.pCurOpts->ui.disabled);
   enableNotebookTabs(g.hwndNoteBook, !g.pCurOpts->ui.disabled);
   initExceptionsList(hwnd);
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
   switch (id) {
      case CHK_UIENABLE:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            event = wBtnCheckState(hCtrl);
            mNotifyRunTime(STLRM_DISABLE, event, g.hwndRunTimeWorker);
            enablePageControls(hwnd, event);
            enableNotebookTabs(g.hwndNoteBook, event);
         } /* endif */
         break;
      case LBOX_UIEXCP:
         if (event == LN_SELECT) {
            WinEnableControl(hwnd, BTN_UIEXCPDEL,
                             (LIT_NONE != wLbxItemSelected(hCtrl)));
         } /* endif */
         break;
      case EF_UIEXCP:
         if (event == EN_CHANGE) {
            WinEnableControl(hwnd, BTN_UIEXCPADD,
                             WinQueryWindowTextLength(hCtrl));
         } /* endif */
         break;
   } /* endswitch */
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
         saveExceptions(hwnd);
         break;
      case BTN_UIEXCPADD:
         addExe(hwnd);
         break;
      case BTN_UIEXCPDEL:
         remExe(hwnd);
         break;
      case BTN_UIEXCPMORE:
         findExe(hwnd);
         break;
   } /* endswitch */
}


/* --------------------------------------------------------------------------
 Enable/disable the dialog controls according to the state of the
 "Enable user interface enhancements" checkbox.
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
 BOOL flag : TRUE/FALSE (enable/diable controls).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID enablePageControls(HWND hwnd, BOOL flag) {
   WinEnableControl(hwnd, GRP_UIEXCP, flag);
   WinEnableControl(hwnd, LBOX_UIEXCP, flag);
   WinEnableControl(hwnd, EF_UIEXCP, flag);
   WinEnableControl(hwnd, BTN_UIEXCPADD,
                    flag && WinQueryDlgItemTextLength(hwnd, EF_UIEXCP));
   WinEnableControl(hwnd, BTN_UIEXCPDEL,
                    flag && (LIT_NONE != dLbxItemSelected(hwnd, LBOX_UIEXCP)));
   WinEnableControl(hwnd, BTN_UIEXCPMORE, flag);
   resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED,
                     PGFL_MASK_IGNORE
                     |
                     (flag? (g.aNBpageFlags[0] & PGFL_APPLY_ENABLED) : 0));
}


/* --------------------------------------------------------------------------
 Read the current list of exceptions and fills the listbox.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL initExceptionsList(HWND hwnd) {
   ULONG cb;
   PSZ pData, p;
   hwnd = WinWindowFromID(hwnd, LBOX_UIEXCP);
   cb = 0x10000;
   if (NULL == (pData = (PSZ)malloc(cb)))
      return handleError(ISERR_ALLOCATION, FALSE);
   if (PrfQueryProfileData(HINI_USER, SZPRO_APP, SZEXC_GENERAL, pData, &cb)) {
      for (p = pData; *p; p+= strlen(p) + 1)
         wLbxItemIns(hwnd, LIT_SORTASCENDING, p);
   } /* endif */
   free(pData);
   return TRUE;
}


/* --------------------------------------------------------------------------
 Save the current exceptions list.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL saveExceptions(HWND hwnd) {
   INT cItems, iItem;
   PSZ pData = NULL;
   PSZ p;
   BOOL rc = FALSE;
   ULONG cb = 0;
   // get the listbox window handle
   hwnd = WinWindowFromID(hwnd, LBOX_UIEXCP);
   // if the listbox contains any items copy the item text to a buffer
   cItems = wLbxItemCount(hwnd);
   if (cItems) {
      // allocate the buffer to store the items text
      if (NULL == (pData = (PSZ)malloc(0x10000))) {
         errorBox(ISERR_ALLOCATION);
         goto exit_0;
      } /* endif */
      // read the items text into the buffer (each item is separated by '\x00'
      for (p = pData, iItem = 0; iItem < cItems; ++iItem) {
         p += wLbxItemText(hwnd, iItem, CCHMAXPATHCOMP, p);
         *p++ = '\x00';
         if ((ULONG)(p - pData + CCHMAXPATHCOMP) > 0x10000) {
            errorBox(ISERR_TOOMANYENTRIES);
            goto exit_0;
         } /* endif */
      } /* endfor */
      *p++ = '\x00';
      cb = (ULONG)(p - pData);
   } /* endif */
   rc = setProfileData(HINI_USER, SZPRO_APP, SZEXC_GENERAL, pData, cb);
   if (!rc) {
      errorBox(ISERR_SAVEEXCPLIST);
   } else {
      g.pCurOpts->ui.exception = (cb > 0);
      resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED, 0);
   } /* endif */
exit_0:
   free(pData);
   return rc;
}


/* --------------------------------------------------------------------------
 Add an executable name to the list of the exceptions.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID addExe(HWND hwnd) {
   CHAR achEntryField[CCHMAXPATH];
   if (WinQueryDlgItemText(hwnd, EF_UIEXCP, CCHMAXPATH, achEntryField)
       &&
       addUniqueFileName(hwnd, LBOX_UIEXCP, achEntryField, LIT_SORTASCENDING)) {
      resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED, PGFL_APPLY_ENABLED);
      WinSetDlgItemText(hwnd, EF_UIEXCP, "");
   } /* endif */
}


/* --------------------------------------------------------------------------
 Add an executable name to the list of the exceptions.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID remExe(HWND hwnd) {
   INT iItem, cItems;
   iItem = dLbxItemSelected(hwnd, LBOX_UIEXCP);
   if (iItem >= 0) {
      cItems = (INT)WinSendDlgItemMsg(hwnd, LBOX_UIEXCP,
                                      LM_DELETEITEM, (MPARAM)iItem, MPVOID);
      // if all the items have been removed and no exception was previously
      // stored in OS2.INI disable the 'Apply' button, otherwise enable it
      resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED,
               (cItems || g.pCurOpts->ui.exception) ? PGFL_APPLY_ENABLED : 0);
      WinEnableControl(hwnd, BTN_UIEXCPDEL, FALSE);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Open the file dialog to allow the user add executable files to the list.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID findExe(HWND hwnd) {
   if (addFileDlg(hwnd, "*.EXE")
       &&
       addUniqueFileName(hwnd, LBOX_UIEXCP, g.achFileSel, LIT_SORTASCENDING))
      resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED, PGFL_APPLY_ENABLED);
}




