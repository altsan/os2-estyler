/* --------------------------------------------------------------------------
 util.c : utility functions.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include <stdarg.h>
#include "main.h"
#include "notebook.h"

// definitions --------------------------------------------------------------


// prototypes ---------------------------------------------------------------


// global variables ---------------------------------------------------------


/* --------------------------------------------------------------------------
 Standard notebook page initialization:
 - reset the page owner,
 - notify the page handle to the help manager.
- Parameters -------------------------------------------------------------
 HWND hwnd : notebook page dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID initPage(HWND hwnd) {
   mResetPageOwner(hwnd);
   wnbkSetPageHelp(g.hwndHelp, g.appl.hwnd, g.hwndNoteBook, hwnd);
}


/* --------------------------------------------------------------------------
 Load a string from the resources.
- Parameters -------------------------------------------------------------
 ULONG id   : string id.
 PSZ pszBuf : address of the laoded string buffer.
- Return value -----------------------------------------------------------
 ULONG : length of the loaded string.
-------------------------------------------------------------------------- */
ULONG loadString(ULONG id, PSZ pszBuf) {
   return WinLoadString(g.appl.hab, g.hResLib, id, 256, pszBuf);
}


/* --------------------------------------------------------------------------
 Set the text of a control from a resource string.
- Parameters -------------------------------------------------------------
 HWND hwnd    :  dialog window handle.
 ULONG idCtrl : control id.
 ULONG idText : string id.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
BOOL setCtrlText(HWND hwnd, ULONG idCtrl, ULONG idText) {
   CHAR buf[256];
   if (loadString(idText, buf))
      return WinSetDlgItemText(hwnd, idCtrl, buf);
   return FALSE;
}


/* --------------------------------------------------------------------------
 Set the text of a control taking the formatting informations from a
 resource string.
- Parameters -------------------------------------------------------------
 HWND hwnd    : dialog window handle.
 ULONG idCtrl : control id.
 ULONG idText : string id.
 ...          : variable type and number of parameters.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
BOOL setCtrlTextParm(HWND hwnd, ULONG idCtrl, ULONG idText, ...) {
   CHAR buf[256];
   CHAR achText[1024];
   va_list ap;
   if (loadString(idText, buf)) {
      va_start(ap, idText);
      vsprintf(achText, buf, ap);
      va_end(ap);
      return WinSetDlgItemText(hwnd, idCtrl, achText);
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Display an error message in a dialog box.
- Parameters -------------------------------------------------------------
 ULONG idMsg : error message ID.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID errorBox(ULONG idMsg, ...) {
   CHAR buf[2048];
   CHAR strResBuf[256];
   HAB hab;
   HMQ hmq;
   va_list ap;
   va_start(ap, idMsg);
   if (idMsg < 0x10000) {
      loadString(idMsg, strResBuf);
      idMsg = (ULONG)strResBuf;
   } /* endif */
   vsprintf(buf, (PSZ)idMsg, ap);
   va_end(ap);
   if (!g.appl.hmq)
      hmq = WinCreateMsgQueue((hab = WinInitialize(0)), 0);
   WinMessageBox(HWND_DESKTOP, g.appl.hwnd, buf, NULL, 0,
                 MB_OK | MB_ERROR | MB_MOVEABLE);
   if (!g.appl.hmq) {
      WinDestroyMsgQueue(hmq);
      WinTerminate(hab);
   } /* endif */
}

/* --------------------------------------------------------------------------
 Updates the application title in the titlebar and in the switch list.
- Parameters -------------------------------------------------------------
 HWND hwnd     : main window handle.
 ULONG idTitle : resource string used for the new title.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
//VOID setApplicationTitle(HWND hwnd, ULONG idTitle) {
//   CHAR buf[256];
//   PID pid;
//   TID tid;
//   HSWITCH hswitch;
//   SWCNTRL swcntrl;
//   if (!loadString(idTitle, buf)) return;
//   WinSetDlgItemText(hwnd, FID_TITLEBAR, buf);
//   if (WinQueryWindowProcess(hwnd, &pid, &tid)
//       &&
//       (NULLHANDLE != (hswitch = WinQuerySwitchHandle(hwnd, pid)))
//       &&
//       !WinQuerySwitchEntry(hswitch, &swcntrl)) {
//
//      memcpy(swcntrl.szSwtitle, buf, MAXNAMEL + 3);
//      swcntrl.szSwtitle[MAXNAMEL + 3] = 0;
//      WinChangeSwitchEntry(hswitch, &swcntrl);
//   } /* endif */
//}


/* --------------------------------------------------------------------------
 Change the state of a button and of the corresponding page flag.
- Parameters -------------------------------------------------------------
 ULONG idBtn : common button id (BTN_APPLY/BTN_UNDO/BTN_DEFAULT/BTN_PREVIEW).
 ULONG mask  : button state mask flag (PGFL_*_VISIBLE | PGFL_*_ENABLED).
 ULONG reset : new button state flag.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID resetCommonButton(ULONG idBtn, ULONG mask, ULONG reset) {
   if (mask & PGFL_MASK_VISIBLE)
      WinShowControl(g.appl.hwnd, idBtn, mask & reset);
   if (mask & PGFL_MASK_ENABLED)
      WinEnableControl(g.appl.hwnd, idBtn, mask & reset);
   if (!(reset & PGFL_MASK_IGNORE)) {
      g.aNBpageFlags[g.pageIdx] &= ~mask;
      g.aNBpageFlags[g.pageIdx] |= reset;
   } /* endif */
}



/* --------------------------------------------------------------------------
 Change the visibile and enabled state of the common buttons.
- Parameters -------------------------------------------------------------
 ULONG flag : new button state.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID setCommonButtons(ULONG fl) {
   WinShowControl(g.appl.hwnd, BTN_APPLY, fl & PGFL_APPLY_VISIBLE);
   WinEnableControl(g.appl.hwnd, BTN_APPLY, fl & PGFL_APPLY_ENABLED);
   WinShowControl(g.appl.hwnd, BTN_UNDO, fl & PGFL_UNDO_VISIBLE);
   WinEnableControl(g.appl.hwnd, BTN_UNDO, fl & PGFL_UNDO_ENABLED);
   WinShowControl(g.appl.hwnd, BTN_DEFAULT, fl & PGFL_DEFAULT_VISIBLE);
   WinEnableControl(g.appl.hwnd, BTN_DEFAULT, fl & PGFL_DEFAULT_ENABLED);
   WinShowControl(g.appl.hwnd, BTN_PREVIEW, fl & PGFL_PREVIEW_VISIBLE);
   WinEnableControl(g.appl.hwnd, BTN_PREVIEW, fl & PGFL_PREVIEW_ENABLED);
}


/* --------------------------------------------------------------------------
 Enable/disable the notebook tabs preventing the user to turn the pages.
- Parameters -------------------------------------------------------------
 HWND hwnd    : notebook window handle.
 BOOL bEnable : TRUE/FALSE (enable/disable the notebook tabs).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID enableNotebookTabs(HWND hwndNoteBook, BOOL bEnable) {
   INT i;
   if (bEnable) {
      g.state &= ~STLRIS_FEATUREDISABLED;
      wnbkSetTabColor(hwndNoteBook, 0, BKA_AUTOCOLOR);
      wnbkSetTabTextColor(hwndNoteBook, SYSCLR_WINDOWTEXT);
   } else {
      g.state |= STLRIS_FEATUREDISABLED;
      for (i = 1; i < NBP_MAXCOUNT; ++i) {
         if (g.aNBpageIDs[i])
            wnbkSetTabColor(hwndNoteBook, g.aNBpageIDs[i], 0xcccccc);
      } /* endfor */
//      wnbkSetTabColor(hwndNoteBook, 0, 0xcccccc);
      wnbkSetTabTextColor(hwndNoteBook, CLR_DARKGRAY);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Get the path of the executable and append pszFile name to it.
- Parameters -------------------------------------------------------------
 PSZ pszBuf  : buffer to store the full path name.
 PSZ pszFile : file name to be appended to path.
- Return value -----------------------------------------------------------
 PSZ pszBuf address.
-------------------------------------------------------------------------- */
PSZ makeFullPathName(PSZ pszBuf, PSZ pszFile) {
   PPIB ppib;
   PTIB ptib;
   PSZ endPath;
   DosGetInfoBlocks(&ptib, &ppib);
   DosQueryModuleName(ppib->pib_hmte, CCHMAXPATH, pszBuf);
   endPath = strrchr(pszBuf, '\\') + 1;
   strcpy(endPath, pszFile);
   return pszBuf;
}


/* --------------------------------------------------------------------------
 Handle spin button notification messages.
 When a new value has been set return TRUE.
- Parameters -------------------------------------------------------------
 HWND hwnd     : spin button handle.
 ULONG ulEvent : WM_CONTROL notification code.
 PLONG pVal    : value to be updated
 LONG lCurVal  : initial value.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (pValue has/has-not been updated).
-------------------------------------------------------------------------- */
BOOL handleSpinbtnEvent(HWND hwnd, ULONG ulEvent, PLONG pVal, LONG lCurVal) {
   static BOOL bSpinning;
   switch (ulEvent) {
      case SPBN_SETFOCUS:
         bSpinning = FALSE;
         break;
      case SPBN_UPARROW:
      case SPBN_DOWNARROW:
         bSpinning = TRUE;
         break;
      case SPBN_CHANGE:
      case SPBN_ENDSPIN:
         if (ulEvent == SPBN_ENDSPIN)
             bSpinning = FALSE;
         return !bSpinning
                &&
                (BOOL)WinSendMsg(hwnd, SPBM_QUERYVALUE, (MPARAM)pVal,
                                 MPFROM2SHORT(0, SPBQ_ALWAYSUPDATE))
                &&
                (*pVal != lCurVal);
   } /* endswitch */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Display the color selection dialog.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
 ULONG id  : id of the button which originated the message.
 LONG color: curren color value.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID selectColor(HWND hwnd, ULONG id, LONG lColor) {
   hwnd = WinWindowFromID(hwnd, id);
   g.state |= STLRIS_COLORSELDLG;
   lColor = editColorDlg(hwnd, lColor);
   g.state &= ~STLRIS_COLORSELDLG;
   if (lColor != CLR_ERROR)
      wClrBtnColorSet(hwnd, lColor);
}


/* --------------------------------------------------------------------------
 Draw a 3D border within the rectangle pr the bottom/left corner of the
 rectangle is moved up and right of 'cpBorder' pixels, while the upper/right
 corner is moved down and left, so the resulting rectangle width and height
 are decreased of cpBorder * 2 pixels.
- Parameters -------------------------------------------------------------
 HPS hps     : presentation space handle
 PRECTL pr   : address of rectangle to be drawn with a 3D border
 ULONG clrul : upper-left border color
 ULONG clrbr : bottom-right border color
 ULONG cpbrd : border thickness in pixels
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID draw3Dborder(HPS hps, PRECTL pr, LONG clrul, LONG clrbr, ULONG cpBorder) {
   POINTL apt[2];
   while (cpBorder--) {
      // bordo sinistro e superiore
      GpiSetColor(hps, clrul);
      PointSet(apt, pr->xLeft, pr->yBottom);
      GpiMove(hps, apt);
      PointSet(apt, pr->xLeft, pr->yTop);
      PointSet(apt + 1, pr->xRight, pr->yTop);
      GpiPolyLine(hps, 2, apt);
      // bordo destro e inferiore
      GpiSetColor(hps, clrbr);
      PointSet(apt, apt[1].x, apt[1].y);
      GpiMove(hps, apt);
      PointSet(apt, pr->xRight, pr->yBottom);
      PointSet(apt + 1, pr->xLeft + 1, pr->yBottom);
      GpiPolyLine(hps, 2, apt);
      RectInflate(pr, -1, -1);
   } /* endfor */
}


/* --------------------------------------------------------------------------
 Show an "Add to the list" file dialog.
- Parameters -------------------------------------------------------------
 HWND hwnd       : dialog window handle.
 PSZ pszFileMask : in  -> file mask,
                   out -> selected file.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (dialog dismissed with Add/Cancel)
-------------------------------------------------------------------------- */
BOOL addFileDlg(HWND hwnd, PSZ pszFileMask) {
   FILEDLG fdlg;
   CHAR achTitle[256];
   CHAR achBtn[256];
   PSZ pFileName;
   pFileName = strrchr(g.achFileSel, '\\');
   strcpy(pFileName + 1, pszFileMask);
   memset((PVOID)&fdlg, 0, sizeof(FILEDLG));
   loadString(IDS_ADDTOTHELIST, achTitle);
   loadString(IDS__ADD, achBtn);
   fdlg.cbSize = sizeof(FILEDLG);
   fdlg.fl = FDS_CENTER | FDS_OPEN_DIALOG;
   fdlg.pszTitle = achTitle;
   fdlg.pszOKButton = achBtn;
   strcpy(fdlg.szFullFile, g.achFileSel);
   if (WinFileDlg(HWND_DESKTOP, hwnd, &fdlg) && (fdlg.lReturn == DID_OK)) {
      strcpy(g.achFileSel, fdlg.szFullFile);
      return TRUE;
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Enable/disable multiple dialog items.
- Parameters -------------------------------------------------------------
 HWND hwnd    : dialog window handle.
 ULONG id1st  : first item whose enable state must be changed.
 ULONG idLast : last item whose enable state must be changed.
 BOOL fl      : TRUE/FALSE (enable/disable).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID dlgItemMultiEnable(HWND hwnd, ULONG id1st, ULONG idLast, BOOL fl) {
   while (id1st <= idLast) DlgItemEnable(hwnd, id1st++, fl);
}


/* --------------------------------------------------------------------------
 Duplicate the content of a memory buffer by allocating a new buffer and
 copying the data of the original buffer.
- Parameters -------------------------------------------------------------
 PBYTE pData   : source data.
 ULONG cbData  : data size.
 PULONG pCbDup : in case of success copy the data size here.
- Return value -----------------------------------------------------------
 PBYTE : new allocated buffer or NULL in case of error.
-------------------------------------------------------------------------- */
PBYTE dataDup(PBYTE pData, ULONG cbData, PULONG pCbDup) {
   PBYTE pNew;
   if (pData && cbData) {
      if (NULL == (pNew = malloc(cbData)))
         return handleError(ISERR_ALLOCATION, NULL);
      memcpy(pNew, pData, cbData);
      if (pCbDup) *pCbDup = cbData;
      return pNew;
   } /* endif */
   return NULL;
}


/* --------------------------------------------------------------------------
 Move a listbox item one position up.
- Parameters -------------------------------------------------------------
 HWND hwnd  : dialog window handle.
 ULONG id   : listbox id.
 INT offset : if greater than 0 move the item downward otherwise move
              the item upward.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID moveItem(HWND hwnd, ULONG id, INT offset) {
   INT iItem;
   CHAR buf[1024];
   ULONG handle;
   hwnd = DlgItemHwnd(hwnd, id);
   iItem = wLbxItemSelected(hwnd);
   wLbxItemText(hwnd, iItem, sizeof(buf), buf);
   handle = wLbxItemHnd(hwnd, iItem);
   WinDeleteLboxItem(hwnd, iItem);
   wLbxItemAndHndIns(hwnd, (iItem += offset), buf, handle);
   wLbxItemSelect(hwnd, iItem);
}


/* --------------------------------------------------------------------------
 On listbox item selection change the enable state of the dialog buttons.
 When pszItem is not NULL change the selected item text to pszItem.
 The move up/down buttons and the remove/edit buttons enable state is
 changed according to the item state.
- Parameters -------------------------------------------------------------
 HWND hwnd      : dialog window handle.
 HWND hCtrl     : listbox window handle.
 PSZ pszItem    : if not NULL the selected item text is changed to pszItem.
 ULONG handle   : item handle.
 ULONG idRemBtn : id of the 'remove' button (the id of the edit button
                  must be idRemBtn + 1).
 ULONG idUpBtn  : id of the move-up button (the id of the move-down button
                  must be idUpBtn + 1).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID handleItemStateChange(HWND hwnd, HWND hCtrl, PSZ pszItem, ULONG handle,
                           ULONG idRemBtn, ULONG idUpBtn) {
   INT iMax = wLbxItemCount(hCtrl) - 1;
   INT iItem = wLbxItemSelected(hCtrl);
   if (pszItem) {
      WinSetLboxItemText(hCtrl, iItem, pszItem);
      wLbxItemHndSet(hCtrl, iItem, handle);
      WinEnableWindow(hCtrl, TRUE);
   } /* endif */
   dlgItemMultiEnable(hwnd, idRemBtn, idRemBtn + 1, iItem >= 0);
   if (idUpBtn) {
      DlgItemEnable(hwnd, idUpBtn, (iItem > 0));
      DlgItemEnable(hwnd, idUpBtn + 1, (iItem >= 0) && (iItem < iMax));
   } /* endif */
}


/* --------------------------------------------------------------------------
 Add pszFileName to the list only if it is not a duplicated string.
- Parameters -------------------------------------------------------------
 HWND hwnd    : dialog window handle.
 ULONG lboxId : list box id.
 PSZ pszItem  : item text.
 INT offset   : insertion offset.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (item added/not-added).
-------------------------------------------------------------------------- */
BOOL addUniqueFileName(HWND hwnd, ULONG lboxId, PSZ pszItem, INT offset) {
   PSZ pFileName;
   pFileName = strrchr(pszItem, '\\');
   pFileName = pFileName ? ++pFileName : pszItem;
   // if the file is not yet in the list add it
   if (LIT_NONE
       ==
       dLbxItemTextSearch(hwnd, lboxId, 0, LIT_FIRST, pFileName)) {
      WinUpper(g.appl.hab, 0, 0, pFileName);
      dLbxItemIns(hwnd, lboxId, offset, pFileName);
      return TRUE;
   } /* endif */
   WinAlarm(HWND_DESKTOP, WA_WARNING);
   return FALSE;
}


/* --------------------------------------------------------------------------
 Select the listbox item 'pszItem'.
 If 'pszItem' is "" select the first item.
- Parameters -------------------------------------------------------------
 HWND hLbox  : listbox/combobox window handle.
 PSZ pszItem : item text.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID selectListItem(HWND hLbox, PSZ pszItem) {
   wLbxItemSelect(hLbox,
             (*pszItem ? wLbxItemTextSearch(hLbox, 0, LIT_FIRST, pszItem): 0));
}


/* --------------------------------------------------------------------------
 Wrapper around PrfWriteProfileString ().
 This is needed since PrfWriteProfileString () return FALSE when deleting a
 non-existing key (i.e. when the key does not exist and pData is NULL).
- Parameters -------------------------------------------------------------
 PrfWriteProfileString () parameters.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
BOOL setProfileString(HINI hini, PSZ pszApp, PSZ pszKey, PSZ pData) {
   ULONG cbQuery;
   if (pData || PrfQueryProfileSize(hini, pszApp, pszKey, &cbQuery))
      return PrfWriteProfileString (hini, pszApp, pszKey, pData);
   return TRUE;
}


/* --------------------------------------------------------------------------
 Wrapper around PrfWriteProfileData().
 This is needed since PrfWriteProfileData() return FALSE when deleting a
 non-existing key (i.e. when the key does not exist and pData is NULL).
- Parameters -------------------------------------------------------------
 PrfWriteProfileData() parameters.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
BOOL setProfileData(HINI hini, PSZ pszApp, PSZ pszKey, PVOID pData, ULONG cb) {
   ULONG cbQuery;
   if (pData || PrfQueryProfileSize(hini, pszApp, pszKey, &cbQuery))
      return PrfWriteProfileData(hini, pszApp, pszKey, pData, cb);
   return TRUE;
}

