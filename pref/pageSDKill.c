/* --------------------------------------------------------------------------
 pageSDKill.c : shutdown - kill programs list - settings dialog procedure.

 2004-6-4 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------
#define CHECK_APPLYSTATE     1
#define CHECK_UNDOSTATE      2

// prototypes ---------------------------------------------------------------
static BOOL onDlgInit(HWND hwnd);
static VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl);
static VOID onCmdMsg(HWND hwnd, ULONG id);
static VOID fillKillList(HWND hwnd);
static VOID setControlsState(HWND hwnd);
static VOID setEnableDependencies(HWND hwnd);
static VOID handleEntryFieldContentChange(HWND hwnd, HWND hCtrl);
static VOID checkOptionsChanged(HWND hwnd, ULONG fl);
static VOID applyOptions(HWND hwnd);
static VOID undoOptions(HWND hwnd);
static VOID addItem(HWND hwnd);
static VOID delItem(HWND hwnd);
static VOID editItem(HWND hwnd);
static PSZ getListContent(HWND hwnd, PULONG pCb);

// global variables ---------------------------------------------------------
static BOOL g_edit = FALSE;

/* --------------------------------------------------------------------------
 Shutdown - kill programs list - settings dialog procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY sdKillPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
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
   fillKillList(hwnd);
   setControlsState(hwnd);
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
      case CHK_SDWNKILLPROG:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pCurOpts->sd.adv.skipKillList = !wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
            setEnableDependencies(hwnd);
         } /* endif */
         break;
      case LBX_SDWNKILLPROG:
         if (event == LN_SELECT)
            handleItemStateChange(hwnd, hCtrl, NULL, 0, BTN_SDWNKILLDEL, 0);
         break;
      case EF_SDWNKILLPROG:
            handleEntryFieldContentChange(hwnd, hCtrl);
         break;
   } /* endswitch */
   if (bSettingsChanged)
      checkOptionsChanged(hwnd, CHECK_APPLYSTATE | CHECK_UNDOSTATE);
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
   BOOL bSettingsChanged = FALSE;
   switch (id) {
      case BTN_APPLY:
         applyOptions(hwnd);
         break;
      case BTN_UNDO:
         undoOptions(hwnd);
         break;
      case BTN_SDWNKILLADD:
         addItem(hwnd);
         bSettingsChanged = TRUE;
         break;
      case BTN_SDWNKILLDEL:
         delItem(hwnd);
         bSettingsChanged = TRUE;
         break;
      case BTN_SDWNKILLEDIT:
         editItem(hwnd);
         break;
      case BTN_SDWNKILLMORE:
         if (addFileDlg(hwnd, "*.EXE"))
            WinSetDlgItemText(hwnd, EF_SDWNKILLPROG, g.achFileSel);
         break;
   } /* endswitch */
   if (bSettingsChanged)
      checkOptionsChanged(hwnd, CHECK_APPLYSTATE | CHECK_UNDOSTATE);
}


/* --------------------------------------------------------------------------
 Fill the list of the programs to be executed on shutdown.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID fillKillList(HWND hwnd) {
   PSZ p;
   hwnd = DlgItemHwnd(hwnd, LBX_SDWNKILLPROG);
   if (g.pShdwnData->pKillProgs) {
      for (p = g.pShdwnData->pKillProgs; *p; p += strlen(p) + 1) {
         wLbxItemIns(hwnd, LIT_END, p);
      } /* endfor */
   } /* endif */
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
   dBtnCheckSet(hwnd, CHK_SDWNKILLPROG, !g.pCurOpts->sd.adv.skipKillList);
   setEnableDependencies(hwnd);
   g.state &= ~STLRIS_SKIPNOTIFICATION;
}


/* --------------------------------------------------------------------------
 Enable/disable the secondary controls according to the check state of the
 'Run the following programs on shutdown:' check box
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setEnableDependencies(HWND hwnd) {
   dlgItemMultiEnable(hwnd, LBX_SDWNKILLPROG, EF_SDWNKILLPROG,
                      !g.pCurOpts->sd.adv.skipKillList);
   // feature being enabled
   if (g.pCurOpts->sd.adv.skipKillList) {
      dlgItemMultiEnable(hwnd, BTN_SDWNKILLADD, BTN_SDWNKILLMORE, FALSE);
   // feature being disabled
   } else {
      handleItemStateChange(hwnd, DlgItemHwnd(hwnd, LBX_SDWNKILLPROG), NULL, 0,
                            BTN_SDWNKILLDEL, 0);
      DlgItemEnable(hwnd, BTN_SDWNKILLADD,
                    WinQueryDlgItemTextLength(hwnd, EF_SDWNKILLPROG));
      DlgItemEnable(hwnd, BTN_SDWNKILLMORE, TRUE);
   } /* endif */
}


/* --------------------------------------------------------------------------
 On entryfield content change, update the enable state of the 'Add' button.
- Parameters -------------------------------------------------------------
 HWND hwnd  : dialog window handle.
 HWND hCtrl : entry field window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID handleEntryFieldContentChange(HWND hwnd, HWND hCtrl) {
   HWND hAddBtn = DlgItemHwnd(hwnd, BTN_SDWNKILLADD);
   BOOL bTextPresent = WinQueryWindowTextLength(hCtrl);
   WinEnableWindow(hAddBtn, bTextPresent);
   WinSendMsg(hAddBtn, BM_SETDEFAULT, (MPARAM)bTextPresent, MPVOID);
}


/* --------------------------------------------------------------------------
 Compare the options set in the dialog with the active options.
 If there is any different option enable the 'Apply' and or the 'Undo' button.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
 ULONG fl  : options to check.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID checkOptionsChanged(HWND hwnd, ULONG fl) {
   PSZ pList;
   ULONG cb;
   BOOL bEnable;
   cb = 1;
   pList = getListContent(hwnd, &cb);
   if (pList || !cb) {
      if (fl & CHECK_APPLYSTATE) {
         bEnable = (cb != g.pShdwnData->cbAppliedKillProgs)
                   ||
                   memcmp(pList, g.pShdwnData->pAppliedKillProgs, cb);
         if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_APPLY))
            resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED,
                              (bEnable ? PGFL_APPLY_ENABLED : 0));
      } /* endif */
      if (fl & CHECK_UNDOSTATE) {
         bEnable = (g.pCurOpts->sd.adv.skipKillList
                                           != g.pUndoSdOpts->adv.skipKillList)
                   ||
                   (cb != g.pShdwnData->cbKillProgs)
                   ||
                   memcmp(pList, g.pShdwnData->pKillProgs, cb);
         if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_UNDO))
            resetCommonButton(BTN_UNDO, PGFL_UNDO_ENABLED,
                              (bEnable ? PGFL_UNDO_ENABLED : 0));
      } /* endif */
   } /* endif */
   free(pList);
}


/* --------------------------------------------------------------------------
 Save the list of the programs to be killed on shutdown in eStyler.ini.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID applyOptions(HWND hwnd) {
   HINI hini;
   // free the previously applied list (if any)
   if (g.pShdwnData->pAppliedKillProgs)
      free(g.pShdwnData->pAppliedKillProgs);
   g.pShdwnData->cbAppliedKillProgs = 1;
   g.pShdwnData->pAppliedKillProgs = getListContent(hwnd,
                                              &g.pShdwnData->cbAppliedKillProgs);
   // if the listbox content has been succesfully read or is empty
   if (g.pShdwnData->pAppliedKillProgs || !g.pShdwnData->cbAppliedKillProgs) {
      if (NULLHANDLE != (hini = stlrOpenProfile())) {
         if (setProfileData(hini, SZPRO_SHUTDWON, SZPRO_KILLPROGS,
                            g.pShdwnData->pAppliedKillProgs,
                            g.pShdwnData->cbAppliedKillProgs)) {
            resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED, 0);
         } else {
            errorBox(ISERR_SAVEDATA);
         } /* endif */
         PrfCloseProfile(hini);
      } else {
         errorBox(ISERR_OPENPRF);
      } /* endif */
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
   HWND hwndLbox;
   hwndLbox = DlgItemHwnd(hwnd, LBX_SDWNKILLPROG);
   g.pCurOpts->sd.adv.skipKillList = g.pUndoSdOpts->adv.skipKillList;
   // empty the listbox displaying the list of the bootable volumes
   wLbxEmpty(hwndLbox);
   // fills it with the data retrieved on program initialization
   fillKillList(hwnd);
   setControlsState(hwnd);
   // reset the state of the Apply and Undo buttons
   resetCommonButton(BTN_UNDO, PGFL_UNDO_ENABLED, 0);
   checkOptionsChanged(hwnd, CHECK_APPLYSTATE);
   handleItemStateChange(hwnd, hwndLbox, NULL, 0, BTN_SDWNKILLDEL, 0);
}


/* --------------------------------------------------------------------------
 Add a bootable volume to the list.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID addItem(HWND hwnd) {
   CHAR buf[CCHMAXPATH];
   HWND hwndLbox;
   INT iItem;
   BOOL resetControls = FALSE;
   // get the program name and convert it to uppercase
   hwndLbox = DlgItemHwnd(hwnd, LBX_SDWNKILLPROG);
   if (WinQueryDlgItemText(hwnd, EF_SDWNKILLPROG, CCHMAXPATH, buf)) {
      g.state |= STLRIS_SKIPNOTIFICATION;
      // item modification mode
      if (g_edit) {
         if ((0 <= (iItem = wLbxItemSelected(hwndLbox)))
             &&
             addUniqueFileName(hwnd, LBX_SDWNKILLPROG, buf, iItem + 1)) {
            WinDeleteLboxItem(hwndLbox, iItem);
            wLbxItemSelect(hwndLbox, iItem);
            resetControls = TRUE;
            g_edit = FALSE;
         } /* endif */
      // a new item is being inserted
      } else {
         if (addUniqueFileName(hwnd, LBX_SDWNKILLPROG, buf, LIT_END))
            resetControls = TRUE;
      } /* endif */
      g.state &= ~STLRIS_SKIPNOTIFICATION;
      if (resetControls) {
         WinSetDlgItemText(hwnd, EF_SDWNKILLPROG, "");
         WinSetFocus(HWND_DESKTOP, DlgItemHwnd(hwnd, EF_SDWNKILLPROG));
      } /* endif */
   } /* endif */
}


/* --------------------------------------------------------------------------
 Remove a bootable volume to the list.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID delItem(HWND hwnd) {
   HWND hwndLbox;
   INT iItem;
   g.state |= STLRIS_SKIPNOTIFICATION;
   hwndLbox = DlgItemHwnd(hwnd, LBX_SDWNKILLPROG);
   iItem = wLbxItemSelected(hwndLbox);
   WinDeleteLboxItem(hwndLbox, iItem);
   dlgItemMultiEnable(hwnd, BTN_SDWNKILLDEL, BTN_SDWNKILLEDIT, FALSE);
   g.state &= ~STLRIS_SKIPNOTIFICATION;
}


/* --------------------------------------------------------------------------
 Modify the selected item.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID editItem(HWND hwnd) {
   HWND hwndLbox;
   CHAR buf[CCHMAXPATHCOMP];
   INT iItem;
   hwndLbox = DlgItemHwnd(hwnd, LBX_SDWNKILLPROG);
   if (0 <= (iItem = wLbxItemSelected(hwndLbox))) {
      g_edit = TRUE;
      WinEnableWindow(hwndLbox, FALSE);
      wLbxItemText(hwndLbox, iItem, sizeof(buf), buf);
      WinSetDlgItemText(hwnd, EF_SDWNKILLPROG, buf);
      dlgItemMultiEnable(hwnd, BTN_SDWNKILLDEL, BTN_SDWNKILLEDIT, FALSE);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Translate the listbox content to the data format used by eStyler.ini.
- Parameters -------------------------------------------------------------
 HWND hwnd  : dialog window handle.
 PULONG pCb : formatted data size.
- Return value -----------------------------------------------------------
 PSZ : address of the formatted data.
-------------------------------------------------------------------------- */
static PSZ getListContent(HWND hwnd, PULONG pCb) {
   INT cItems, i;
   PSZ pList, p;
   ULONG cbLeft, cbItem;
   hwnd = DlgItemHwnd(hwnd, LBX_SDWNKILLPROG);
   if (!(cItems = wLbxItemCount(hwnd))) {
      *pCb = 0;
      return NULL;
   } /* endif */
   if (NULL == (pList = malloc(0xffff)))
      return handleError(ISERR_ALLOCATION, NULL);
   for (p = pList, i = 0, cbLeft = 0xffff; i < cItems; ++i) {
      if (cbLeft < CCHMAXPATHCOMP) {
         errorBox(ISERR_PROGLISTCOUNT);
         break;
      } else {
         cbItem = wLbxItemText(hwnd, i, CCHMAXPATHCOMP, p) + 1;
         p += cbItem, cbLeft -= cbItem;
      } /* endif */
   } /* endfor */
   *p = 0;
   *pCb = (ULONG)(p - pList) + 1;
   return pList;
}


