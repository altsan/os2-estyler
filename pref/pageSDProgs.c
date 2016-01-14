/* --------------------------------------------------------------------------
 pageSDProgs.c : shutdown - programs list - settings dialog procedure.

 2004-6-4 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------
#define CCHMAXITEMTEXT     561
#define CCHITEMTEXTBUF     (CCHMAXITEMTEXT + 3)
#define CHECK_APPLYSTATE     1
#define CHECK_UNDOSTATE      2

// prototypes ---------------------------------------------------------------
BOOL onDlgInit(HWND hwnd);
VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl);
VOID onCmdMsg(HWND hwnd, ULONG id);
VOID fillRunProgramList(HWND hwnd);
VOID setControlsState(HWND hwnd);
VOID setEnableDependencies(HWND hwnd);
VOID handleEntryFieldContentChange(HWND hwnd, HWND hCtrl);
VOID checkOptionsChanged(HWND hwnd, ULONG fl);
VOID applyOptions(HWND hwnd);
VOID undoOptions(HWND hwnd);
VOID addItem(HWND hwnd);
VOID delItem(HWND hwnd);
VOID editItem(HWND hwnd);
PSZ getListContent(HWND hwnd, PULONG pCb);

// global variables ---------------------------------------------------------
static BOOL g_edit = FALSE;

/* --------------------------------------------------------------------------
 Shutdown - programs list - settings dialog procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY sdProgsPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
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
   fillRunProgramList(hwnd);
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
      case CHK_SDWNRUNPROG:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pCurOpts->sd.adv.runPrograms = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
            setEnableDependencies(hwnd);
         } /* endif */
         break;
      case LBX_SDWNPROG:
         if (event == LN_SELECT)
            handleItemStateChange(hwnd, hCtrl, NULL, 0,
                                  BTN_SDWNPROGREM, BTN_SDWNPROGUP);
         break;
      case EF_SDWNPROGNAME:
         if (event == EN_CHANGE)
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
      case BTN_SDWNPROGUP:
         moveItem(hwnd, LBX_SDWNPROG, -1);
         bSettingsChanged = TRUE;
         break;
      case BTN_SDWNPROGDOWN:
         moveItem(hwnd, LBX_SDWNPROG, 1);
         bSettingsChanged = TRUE;
         break;
      case BTN_SDWNPROGADD:
         addItem(hwnd);
         bSettingsChanged = TRUE;
         break;
      case BTN_SDWNPROGREM:
         delItem(hwnd);
         bSettingsChanged = TRUE;
         break;
      case BTN_SDWNPROGEDIT:
         editItem(hwnd);
         break;
      case BTN_SDWNPROGFIND:
         if (addFileDlg(hwnd, "*.EXE"))
            WinSetDlgItemText(hwnd, EF_SDWNPROGNAME, g.achFileSel);
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
static VOID fillRunProgramList(HWND hwnd) {
   PSZ p;
   ULONG exeFlag;
   hwnd = DlgItemHwnd(hwnd, LBX_SDWNPROG);
   if (g.pShdwnData->pRunProgs) {
      for (p = g.pShdwnData->pRunProgs; *p; p += strlen(p) + 1) {
         switch (*p) {
            // program to be executed in minimized state
            case '<': exeFlag = 1, ++p; break;
            // program to be executed in maximized state
            case '>': exeFlag = 2, ++p; break;
            // program to be executed in default state
            default:  exeFlag = 0; break;
         } /* endswitch */
         // insert the item storing the associated exeFlag
         wLbxItemAndHndIns(hwnd, LIT_END, p, exeFlag);
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
   HWND hwndLbox;
   g.state |= STLRIS_SKIPNOTIFICATION;
   dBtnCheckSet(hwnd, CHK_SDWNRUNPROG, g.pCurOpts->sd.adv.runPrograms);
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
   dlgItemMultiEnable(hwnd, LBX_SDWNPROG, COMBO_SDWNPROG,
                      g.pCurOpts->sd.adv.runPrograms);
   // feature being enabled
   if (g.pCurOpts->sd.adv.runPrograms) {
      handleItemStateChange(hwnd, DlgItemHwnd(hwnd, LBX_SDWNPROG), NULL, 0,
                            BTN_SDWNPROGREM, BTN_SDWNPROGUP);
      DlgItemEnable(hwnd, BTN_SDWNPROGADD,
                    WinQueryDlgItemTextLength(hwnd, EF_SDWNPROGNAME));
      DlgItemEnable(hwnd, BTN_SDWNPROGFIND, TRUE);
   // feature being disabled
   } else {
      dlgItemMultiEnable(hwnd, BTN_SDWNPROGUP, BTN_SDWNPROGDOWN, FALSE);
      dlgItemMultiEnable(hwnd, BTN_SDWNPROGADD, BTN_SDWNPROGFIND, FALSE);
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
   HWND hAddBtn = DlgItemHwnd(hwnd, BTN_SDWNPROGADD);
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
         bEnable = (cb != g.pShdwnData->cbAppliedRunProgs)
                   ||
                   memcmp(pList, g.pShdwnData->pAppliedRunProgs, cb);
         if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_APPLY))
            resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED,
                              (bEnable ? PGFL_APPLY_ENABLED : 0));
      } /* endif */
      if (fl & CHECK_UNDOSTATE) {
         bEnable = (g.pCurOpts->sd.adv.runPrograms
                                           != g.pUndoSdOpts->adv.runPrograms)
                   ||
                   (cb != g.pShdwnData->cbRunProgs)
                   ||
                   memcmp(pList, g.pShdwnData->pRunProgs, cb);
         if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_UNDO))
            resetCommonButton(BTN_UNDO, PGFL_UNDO_ENABLED,
                              (bEnable ? PGFL_UNDO_ENABLED : 0));
      } /* endif */
   } /* endif */
   free(pList);
}


/* --------------------------------------------------------------------------
 Save the list of the programs to be executed on shutdown in eStyler.ini.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID applyOptions(HWND hwnd) {
   HINI hini;
   // free the previously applied list (if any)
   if (g.pShdwnData->pAppliedRunProgs)
      free(g.pShdwnData->pAppliedRunProgs);
   g.pShdwnData->cbAppliedRunProgs = 1;
   g.pShdwnData->pAppliedRunProgs = getListContent(hwnd,
                                              &g.pShdwnData->cbAppliedRunProgs);
   // if the listbox content has been succesfully read or is empty
   if (g.pShdwnData->pAppliedRunProgs || !g.pShdwnData->cbAppliedRunProgs) {
      if (NULLHANDLE != (hini = stlrOpenProfile())) {
         if (setProfileData(hini, SZPRO_SHUTDWON, SZPRO_SDWNPRGLIST,
                            g.pShdwnData->pAppliedRunProgs,
                            g.pShdwnData->cbAppliedRunProgs)) {
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
   hwndLbox = DlgItemHwnd(hwnd, LBX_SDWNPROG);
   g.pCurOpts->sd.adv.runPrograms = g.pUndoSdOpts->adv.runPrograms;
   // empty the listbox displaying the list of the bootable volumes
   wLbxEmpty(hwndLbox);
   // fills it with the data retrieved on program initialization
   fillRunProgramList(hwnd);
   setControlsState(hwnd);
   // reset the state of the Apply and Undo buttons
   resetCommonButton(BTN_UNDO, PGFL_UNDO_ENABLED, 0);
   checkOptionsChanged(hwnd, CHECK_APPLYSTATE);
   handleItemStateChange(hwnd, hwndLbox, NULL, 0,
                         BTN_SDWNPROGREM, BTN_SDWNPROGUP);
}


/* --------------------------------------------------------------------------
 Add a bootable volume to the list.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID addItem(HWND hwnd) {
   CHAR buf[CCHITEMTEXTBUF];
   ULONG cb;
   PSZ pExtension;
   HWND hwndLbox;
   INT exeFlag;
   // get the program name and convert it to uppercase
   cb = WinQueryDlgItemText(hwnd, EF_SDWNPROGNAME, CCHMAXPATH, buf);
   WinUpper(g.appl.hab, 0, 0, buf);
   // find the program extension
   pExtension = strrchr(buf, '.');
   // if the extension is missing append ".exe"
   if (!pExtension) {
      memcpy(buf + cb, ".EXE ", 5);
      cb += 5;
   // if the extension is CMD prepend "CMD.EXE /C "
   } else if (!memcmp(pExtension, ".CMD", 5)) {
      memmove(buf + 11, buf, cb);
      memcpy(buf, "CMD.EXE /C ", 11);
      cb += 11;
      buf[cb++] = ' ';
   // regular "EXE" extension add a space to separate the parameters
   } else if (!memcmp(pExtension, ".EXE", 5)
              ||
              !memcmp(pExtension, ".COM", 5)) {
      buf[cb++] = ' ';
   // invalid extension: display an error message
   } else {
      errorBox(ISERR_INVALIDEXE, buf);
      return;
   } /* endif */
   // get the program parameters (if any)
   cb += WinQueryDlgItemText(hwnd, EF_SDWNPROGPARMS, sizeof(buf) - cb, buf + cb);
   // check the execution mode (default/minimized/maximized)
   exeFlag = dLbxItemSelected(hwnd, COMBO_SDWNPROG);
   if (exeFlag < 0) exeFlag = 0; // no selection == default mode
   hwndLbox = DlgItemHwnd(hwnd, LBX_SDWNPROG);
   // item modification mode
   if (g_edit) {
      handleItemStateChange(hwnd, hwndLbox, buf, exeFlag,
                            BTN_SDWNPROGREM, BTN_SDWNPROGUP);
      g_edit = FALSE;
   // a new item is being inserted
   } else {
      wLbxItemAndHndIns(hwndLbox, LIT_END, buf, exeFlag);
      handleItemStateChange(hwnd, hwndLbox, NULL, 0,
                            BTN_SDWNPROGREM, BTN_SDWNPROGUP);
   } /* endif */
   WinSetDlgItemText(hwnd, EF_SDWNPROGNAME, "");
   WinSetDlgItemText(hwnd, EF_SDWNPROGPARMS, "");
//   WinSetDlgItemText(hwnd, COMBO_SDWNPROG, "");
   WinSetFocus(HWND_DESKTOP, DlgItemHwnd(hwnd, EF_SDWNPROGNAME));
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
   hwndLbox = DlgItemHwnd(hwnd, LBX_SDWNPROG);
   iItem = wLbxItemSelected(hwndLbox);
   WinDeleteLboxItem(hwndLbox, iItem);
   dlgItemMultiEnable(hwnd, BTN_SDWNPROGREM, BTN_SDWNPROGEDIT, FALSE);
   dlgItemMultiEnable(hwnd, BTN_SDWNPROGUP, BTN_SDWNPROGDOWN, FALSE);
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
   CHAR buf[CCHITEMTEXTBUF];
   PSZ p;
   INT iItem;
   ULONG exeFlag;
   hwndLbox = DlgItemHwnd(hwnd, LBX_SDWNPROG);
   if (0 <= (iItem = wLbxItemSelected(hwndLbox))) {
      g_edit = TRUE;
      WinEnableWindow(hwndLbox, FALSE);
      wLbxItemText(hwndLbox, iItem, sizeof(buf), buf);
      exeFlag = wLbxItemHnd(hwndLbox, iItem);
      // separate the executable name from the parameters
      if (NULL == (p = strstr(buf, ".EXE")))
         p = strstr(buf, ".COM");
      *(p + 4) = 0;
      WinSetDlgItemText(hwnd, EF_SDWNPROGNAME, buf);
      WinSetDlgItemText(hwnd, EF_SDWNPROGPARMS, p + 5);
      dLbxItemSelect(hwnd, COMBO_SDWNPROG, exeFlag);
      dlgItemMultiEnable(hwnd, BTN_SDWNPROGREM, BTN_SDWNPROGEDIT, FALSE);
      dlgItemMultiEnable(hwnd, BTN_SDWNPROGUP, BTN_SDWNPROGDOWN, FALSE);
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
   ULONG exeFlag, cbLeft, cbItem;
   hwnd = DlgItemHwnd(hwnd, LBX_SDWNPROG);
   if (!(cItems = wLbxItemCount(hwnd))) {
      *pCb = 0;
      return NULL;
   } /* endif */
   if (NULL == (pList = malloc(0xffff)))
      return handleError(ISERR_ALLOCATION, NULL);
   for (p = pList, i = 0, cbLeft = 0xffff; i < cItems; ++i) {
      if (cbLeft < CCHMAXITEMTEXT) {
         errorBox(ISERR_PROGLISTCOUNT);
         break;
      } else {
         if (0 != (exeFlag = wLbxItemHnd(hwnd, i)))
            *p++ = (exeFlag == 1) ? '<' : '>', cbLeft --;
         cbItem = wLbxItemText(hwnd, i, CCHMAXITEMTEXT, p) + 1;
         p += cbItem, cbLeft -= cbItem;
      } /* endif */
   } /* endfor */
   *p = 0;
   *pCb = (ULONG)(p - pList) + 1;
   return pList;
}


