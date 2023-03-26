/* --------------------------------------------------------------------------
 pageUITbar.c : active/inactive titlebar settings dialog procedure.

 2004-6-4 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------

#define mGradientIndex(_flag_) \
   ((((_flag_) & TBO_VERTSHADE) >> 1) | (((_flag_) & TBO_SHADEDOUBLE) >> 2))



// prototypes ---------------------------------------------------------------
static BOOL onDlgInit(HWND hwnd);
static VOID setControlsState(HWND hwnd, PTBARHILITE ptbo, PSZ pszBmpFile);
static VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl);
static VOID onCmdMsg(HWND hwnd, ULONG id);
static VOID onWorkerJobEnd(HWND hwnd, ULONG jobId, ULONG rc);
static VOID setBkgndControls(HWND hwnd, ULONG flBkgnd);
static VOID setBorderControls(HWND hwnd, BOOL bEnable);
static VOID setTextControls(HWND hwnd, BOOL bEnable);
static BOOL handleBkgndOptionChange(HWND hwnd, HWND hCtrl, PTBARHILITESETS p);
static VOID setShadeList(HWND hwnd, PTBARHILITE ptbo);
static VOID setShadeControls(HWND hwnd, INT index);
static BOOL handleShadeOptionChange(HWND hwnd, HWND hCtrl, PTBARHILITESETS p);
static BOOL handleBkgndImageChange(HWND hwnd, HWND hCtrl, PTBARHILITESETS p);
static VOID checkApplyState(PTBARHILITESETS p);
static VOID checkUndoState(PTBARHILITESETS p);
static VOID checkDefaultState(PTBARHILITESETS p);
static VOID applyOptions(HWND hwnd, PTBARHILITESETS p);
static BOOL deferredApplyOptions(HWND hwnd, ULONG rc, PAPPLYBMP pa);
static BOOL applyBkgndBitmap(HWND hwnd, PAPPLYBMP pa);
static BOOL applyBkgndShade(HWND hwnd, PAPPLYBMP pa);
static VOID applyBkgndColor(HWND hwnd, PAPPLYBMP pa);
static VOID undoOptions(HWND hwnd, PTBARHILITESETS p);
static VOID defaultOptions(HWND hwnd, PTBARHILITESETS p);
static VOID swapColors(HWND hwnd, ULONG id1, ULONG id2);
static BOOL getTitlebarBitmapData(HWND hwnd, PTBARHILITESETS p);
static VOID setNewTitlebarBackground(HWND hwnd, PTBARHILITESETS p,
                              PGETBMPDATA pgb, ULONG rc);

// global variables ---------------------------------------------------------

/* --------------------------------------------------------------------------
 Active/inactive titlebar settings dialog procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY uiTbarPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
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
      case STLRM_WORKERTHREADJOBEND:
         onWorkerJobEnd(hwnd, (ULONG)mp1, (ULONG)mp2);
         break;
      case WM_DESTROY:
         free(WinQueryWindowPtr(hwnd, QWL_USER));
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
   PTBARHILITESETS p;
   if (NULL == (p = malloc(sizeof(TBARHILITESETS))))
      return handleError(ISERR_ALLOCATION, FALSE);
   // active titlebar
   if (g.pageIdx == NBP_UITITLEBARACTIVE) {
      g.pUiData->hwndActiveTbar = hwnd;
      p->pSetsCur = &g.pUiData->pOpts->tb.a;
      p->pSetsApplied = &g.pCurOpts->ui.tb.a;
      p->pSetsUndo = &g.pUndoUiOpts->tb.a;
      p->pBmp = &g.pUiData->tba;
   // inactive titlebar
   } else {
      g.pUiData->hwndInactiveTbar = hwnd;
      p->pSetsCur = &g.pUiData->pOpts->tb.i;
      p->pSetsApplied = &g.pCurOpts->ui.tb.i;
      p->pSetsUndo = &g.pUndoUiOpts->tb.i;
      p->pBmp = &g.pUiData->tbi;
   } /* endif */
   WinSetWindowPtr(hwnd, QWL_USER, p);
   initPage(hwnd);
   listBitmaps(hwnd, SZ_BMPPATHTBAR, p->pBmp->pszCur, NULL);
   setControlsState(hwnd, p->pSetsCur, NULL);
   checkDefaultState(p);
   return TRUE;
}


/* --------------------------------------------------------------------------
 Set the state of the dialog controls according to the current value of
 the options.
- Parameters -------------------------------------------------------------
 HWND hwnd        : dialog window handle.
 PTBARHILITE ptbo : active/inactive titlebar options.
 PSZ pszBmpFile   : if not NULL select pszBmpFile in the image list.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setControlsState(HWND hwnd, PTBARHILITE ptbo, PSZ pszBmpFile) {
   g.state |= STLRIS_SKIPNOTIFICATION;
   dLbxItemSelect(hwnd, COMBO_TBBKGND, ptbo->bkgnd);
   setShadeList(hwnd, ptbo);
   dClrBtnColorSet(hwnd, BTN_TBSOLID, ptbo->clrLeftTop);
   dClrBtnColorSet(hwnd, BTN_TBSHADECLR1, ptbo->clrLeftTop);
   dClrBtnColorSet(hwnd, BTN_TBSHADECLR2, ptbo->clrRghtBtm);
   dBtnCheckSet(hwnd, CHK_TBSCALE, 0 != (ptbo->fl & TBO_STRETCHBMP));
   setBkgndControls(hwnd, ptbo->bkgnd);
   dBtnCheckSet(hwnd, CHK_TBBORDER, 0 != (ptbo->fl & TBO_BORDER));
   dClrBtnColorSet(hwnd, BTN_TBLEFTTOP, ptbo->clrTopLeftBorder);
   dClrBtnColorSet(hwnd, BTN_TBRIGHTBTM, ptbo->clrRightBottomBorder);
   setBorderControls(hwnd, ptbo->fl & TBO_BORDER);
   dBtnCheckSet(hwnd, CHK_TBSHADOW, 0 != (ptbo->fl & TBO_TEXTSHADOW));
   dClrBtnColorSet(hwnd, BTN_TBTXTCLR, ptbo->clrFgTxt);
   dClrBtnColorSet(hwnd, BTN_TBSHADOWCLR, ptbo->clrBgTxt);
   setTextControls(hwnd, ptbo->fl & TBO_TEXTSHADOW);
   if (pszBmpFile)
      selectListItem(DlgItemHwnd(hwnd, COMBO_TBIMAGE), pszBmpFile);
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
   PTBARHILITESETS p = WinQueryWindowPtr(hwnd, QWL_USER);
   if (!p) return;
   switch (id) {
      case COMBO_TBBKGND:
         if (event == LN_SELECT) {
            if (handleBkgndOptionChange(hwnd, hCtrl, p))
               bSettingsChanged = TRUE;
         } /* endif */
         break;
      case COMBO_TBSHADE:
         if (event == LN_SELECT) {
            if (handleShadeOptionChange(hwnd, hCtrl, p))
               bSettingsChanged = TRUE;
         } /* endif */
         break;
      case COMBO_TBIMAGE:
         if (event == LN_SELECT) {
            if (handleBkgndImageChange(hwnd, hCtrl, p))
               bSettingsChanged = TRUE;
         } /* endif */
         break;
      case CHK_TBSCALE:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            if (wBtnCheckState(hCtrl)) {
               p->pSetsCur->fl |= TBO_STRETCHBMP;
            } else {
               p->pSetsCur->fl &= ~TBO_STRETCHBMP;
            } /* endif */
            bSettingsChanged = TRUE;
            updatePreviewWindow(p->pBmp->flPreview);
         } /* endif */
         break;
      case CHK_TBBORDER:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            if (wBtnCheckState(hCtrl)) {
               p->pSetsCur->fl |= TBO_BORDER;
               setBorderControls(hwnd, TRUE);
            } else {
               p->pSetsCur->fl &= ~TBO_BORDER;
               setBorderControls(hwnd, FALSE);
            } /* endif */
            bSettingsChanged = TRUE;
            updatePreviewWindow(p->pBmp->flPreview);
         } /* endif */
         break;
      case CHK_TBSHADOW:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            if (wBtnCheckState(hCtrl)) {
               p->pSetsCur->fl |= TBO_TEXTSHADOW;
               setTextControls(hwnd, TRUE);
            } else {
               p->pSetsCur->fl &= ~TBO_TEXTSHADOW;
               setTextControls(hwnd, FALSE);
            } /* endif */
            bSettingsChanged = TRUE;
            updatePreviewWindow(p->pBmp->flPreview);
         } /* endif */
         break;
      case BTN_TBSOLID:
         if (event == BN_COLORCHANGED) {
            p->pSetsCur->clrLeftTop = (LONG)hCtrl;
            if (!(g.state & STLRIS_COLORSELDLG)) bSettingsChanged = TRUE;
            updatePreviewWindow(p->pBmp->flPreview);
         } /* endif */
         break;
      case BTN_TBSHADECLR1:
         if (event == BN_COLORCHANGED) {
            p->pSetsCur->clrLeftTop = (LONG)hCtrl;
            if (!(g.state & STLRIS_COLORSELDLG)) bSettingsChanged = TRUE;
            if (g.state & STLRIS_COLORSWAPPED) {
               g.state &= ~STLRIS_COLORSWAPPED;
            } else {
               setNewTitlebarBackground(hwnd, p, NULL, 0);
            } /* endif */
         } /* endif */
         break;
      case BTN_TBSHADECLR2:
         if (event == BN_COLORCHANGED) {
            p->pSetsCur->clrRghtBtm = (LONG)hCtrl;
            if (!(g.state & STLRIS_COLORSELDLG)) bSettingsChanged = TRUE;
            if (g.state & STLRIS_COLORSWAPPED) {
               g.state &= ~STLRIS_COLORSWAPPED;
            } else {
               setNewTitlebarBackground(hwnd, p, NULL, 0);
            } /* endif */
         } /* endif */
         break;
      case BTN_TBLEFTTOP:
         if (event == BN_COLORCHANGED) {
            p->pSetsCur->clrTopLeftBorder = (LONG)hCtrl;
            if (!(g.state & STLRIS_COLORSELDLG)) bSettingsChanged = TRUE;
            if (g.state & STLRIS_COLORSWAPPED) {
               g.state &= ~STLRIS_COLORSWAPPED;
            } else {
               updatePreviewWindow(p->pBmp->flPreview);
            } /* endif */
         } /* endif */
         break;
      case BTN_TBRIGHTBTM:
         if (event == BN_COLORCHANGED) {
            p->pSetsCur->clrRightBottomBorder = (LONG)hCtrl;
            if (!(g.state & STLRIS_COLORSELDLG)) bSettingsChanged = TRUE;
            if (g.state & STLRIS_COLORSWAPPED) {
               g.state &= ~STLRIS_COLORSWAPPED;
            } else {
               updatePreviewWindow(p->pBmp->flPreview);
            } /* endif */
         } /* endif */
         break;
      case BTN_TBTXTCLR:
         if (event == BN_COLORCHANGED) {
            p->pSetsCur->clrFgTxt = (LONG)hCtrl;
            if (!(g.state & STLRIS_COLORSELDLG)) bSettingsChanged = TRUE;
            if (g.state & STLRIS_COLORSWAPPED) {
               g.state &= ~STLRIS_COLORSWAPPED;
            } else {
               updatePreviewWindow(p->pBmp->flPreview);
            } /* endif */
         } /* endif */
         break;
      case BTN_TBSHADOWCLR:
         if (event == BN_COLORCHANGED) {
            p->pSetsCur->clrBgTxt = (LONG)hCtrl;
            if (!(g.state & STLRIS_COLORSELDLG)) bSettingsChanged = TRUE;
            if (g.state & STLRIS_COLORSWAPPED) {
               g.state &= ~STLRIS_COLORSWAPPED;
            } else {
               updatePreviewWindow(p->pBmp->flPreview);
            } /* endif */
         } /* endif */
         break;
   } /* endswitch */
   if (bSettingsChanged) {
      checkApplyState(p);
      checkUndoState(p);
      checkDefaultState(p);
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
   PTBARHILITESETS p = WinQueryWindowPtr(hwnd, QWL_USER);
   switch (id) {
      case BTN_APPLY:
         applyOptions(hwnd, p);
         break;
      case BTN_UNDO:
         undoOptions(hwnd, p);
         break;
      case BTN_DEFAULT:
         defaultOptions(hwnd, p);
         break;
      case BTN_TBSOLID:
         selectColor(hwnd, BTN_TBSOLID, p->pSetsCur->clrLeftTop);
         break;
      case BTN_TBSWAPSHADE:
         swapColors(hwnd, BTN_TBSHADECLR1, BTN_TBSHADECLR2);
         break;
      case BTN_TBSHADECLR1:
         selectColor(hwnd, BTN_TBSHADECLR1, p->pSetsCur->clrLeftTop);
         break;
      case BTN_TBSHADECLR2:
         selectColor(hwnd, BTN_TBSHADECLR2, p->pSetsCur->clrRghtBtm);
         break;
      case BTN_TBADDIMAGE:
         addImage(hwnd, SZ_BMPPATHTBAR, NULL);
         break;
      case BTN_TBSWAPBORDER:
         swapColors(hwnd, BTN_TBLEFTTOP, BTN_TBRIGHTBTM);
         break;
      case BTN_TBLEFTTOP:
         selectColor(hwnd, BTN_TBLEFTTOP, p->pSetsCur->clrTopLeftBorder);
         break;
      case BTN_TBRIGHTBTM:
         selectColor(hwnd, BTN_TBRIGHTBTM, p->pSetsCur->clrRightBottomBorder);
         break;
      case BTN_TBSWAPTXT:
         swapColors(hwnd, BTN_TBTXTCLR, BTN_TBSHADOWCLR);
         break;
      case BTN_TBTXTCLR:
         selectColor(hwnd, BTN_TBTXTCLR, p->pSetsCur->clrFgTxt);
         break;
      case BTN_TBSHADOWCLR:
         selectColor(hwnd, BTN_TBSHADOWCLR, p->pSetsCur->clrBgTxt);
         break;
   } /* endswitch */
}


/* --------------------------------------------------------------------------
 Handle termination of a worker thread job.
- Parameters -------------------------------------------------------------
 HWND hwnd   : dialog window handle.
 ULONG jobId : job identifier.
 ULONG rc    : return code.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID onWorkerJobEnd(HWND hwnd, ULONG jobId, ULONG rc) {
   switch (jobId) { // end of directory scan for bitmap files
      case STLRWID_LISTBMPFILES:
         fillBitmapList(DlgItemHwnd(hwnd, COMBO_TBIMAGE),
                        (PLISTBMPFILES)g.worker.pCurJob->pData,
                        TRUE);
         break;
      case STLRWID_APPLYBMP:
         deferredApplyOptions(hwnd, rc, (PAPPLYBMP)g.worker.pCurJob->pData);
         break;
      case STLRWID_GETBMPDATA:
         setNewTitlebarBackground(hwnd, NULL,
                                  (PGETBMPDATA)g.worker.pCurJob->pData, rc);
         break;
      case STLRWID_ADDBMP:
         addImageJobEnd(DlgItemHwnd(hwnd, COMBO_TBIMAGE),
                        rc, (PADDBMPFILE)g.worker.pCurJob->pData);
         break;
   } /* endswitch */
   workerJobUnlink();
}


/* --------------------------------------------------------------------------
 Show the proper set of controls according to the current titlebar background
 setting.
- Parameters -------------------------------------------------------------
 HWND hwnd     : dialog window handle.
 ULONG flBkgnd : type of titlebar background.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setBkgndControls(HWND hwnd, ULONG flBkgnd) {
   WinShowControl(hwnd, TXT_TBBKGNDCLR, flBkgnd == TBARBKGNDSOLIDCLR);
   WinShowControl(hwnd, BTN_TBSOLID, flBkgnd == TBARBKGNDSOLIDCLR);
   WinShowControl(hwnd, TXT_TBDIRECTION, flBkgnd == TBARBKGNDSHADE);
   WinShowControl(hwnd, COMBO_TBSHADE, flBkgnd == TBARBKGNDSHADE);
   WinShowControl(hwnd, BTN_TBSWAPSHADE, flBkgnd == TBARBKGNDSHADE);
   WinShowControl(hwnd, TXT_TBSHADECLR1, flBkgnd == TBARBKGNDSHADE);
   WinShowControl(hwnd, BTN_TBSHADECLR1, flBkgnd == TBARBKGNDSHADE);
   WinShowControl(hwnd, TXT_TBSHADECLR2, flBkgnd == TBARBKGNDSHADE);
   WinShowControl(hwnd, BTN_TBSHADECLR2, flBkgnd == TBARBKGNDSHADE);
   WinShowControl(hwnd, TXT_TBIMAGE, flBkgnd == TBARBKGNDBMP);
   WinShowControl(hwnd, COMBO_TBIMAGE, flBkgnd == TBARBKGNDBMP);
   WinShowControl(hwnd, CHK_TBSCALE, flBkgnd == TBARBKGNDBMP);
   WinShowControl(hwnd, BTN_TBADDIMAGE, flBkgnd == TBARBKGNDBMP);
}


/* --------------------------------------------------------------------------
 Enable the proper set of controls according to the current titlebar
 border setting.
- Parameters -------------------------------------------------------------
 HWND hwnd        : dialog window handle.
 BOOL bEnable : TRUE/FALSE (enable/disable).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setBorderControls(HWND hwnd, BOOL bEnable) {
   WinEnableControl(hwnd, BTN_TBSWAPBORDER, bEnable);
   WinEnableControl(hwnd, TXT_TBLEFTTOP, bEnable);
   WinEnableControl(hwnd, BTN_TBLEFTTOP, bEnable);
   WinEnableControl(hwnd, TXT_TBRIGHTBTM, bEnable);
   WinEnableControl(hwnd, BTN_TBRIGHTBTM, bEnable);
}


/* --------------------------------------------------------------------------
 Show the proper set of controls according to the current titlebar text
 setting.
- Parameters -------------------------------------------------------------
 HWND hwnd    : dialog window handle.
 BOOL bEnable : TRUE/FALSE (enable/disable).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setTextControls(HWND hwnd, BOOL bEnable) {
   WinEnableControl(hwnd, BTN_TBSWAPTXT, bEnable);
   WinEnableControl(hwnd, TXT_TBSHADOWCLR, bEnable);
   WinEnableControl(hwnd, BTN_TBSHADOWCLR, bEnable);
}


/* --------------------------------------------------------------------------
 On background settings change update the state of the background controls.
- Parameters -------------------------------------------------------------
 HWND hwnd  : dialog window handle.
 HWND hCtrl : background combobox handle.
 PTBARHILITESETS p : active/inactive titlebar settings.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL handleBkgndOptionChange(HWND hwnd, HWND hCtrl, PTBARHILITESETS p) {
   INT iitem;
   if (LIT_NONE != (iitem = wLbxItemSelected(hCtrl))) {
      p->pSetsCur->bkgnd = iitem;
      if (p->pSetsCur->bkgnd == TBARBKGNDBMP) {
         getTitlebarBitmapData(hwnd, p);
      } else {
         setNewTitlebarBackground(hwnd, p, NULL, 0);
      } /* endif */
      setBkgndControls(hwnd, p->pSetsCur->bkgnd);
      return TRUE;
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Select the current kind of gradient in the combination box.
- Parameters -------------------------------------------------------------
 HWND hwnd        : dialog window handle.
 PTBARHILITE ptbo : active/inactive titlebar options.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setShadeList(HWND hwnd, PTBARHILITE ptbo) {
   INT iitem = mGradientIndex(ptbo->fl);
   dLbxItemSelect(hwnd, COMBO_TBSHADE, iitem);
   if (iitem == 3) iitem = 2;
   setShadeControls(hwnd, iitem);
}


/* --------------------------------------------------------------------------
 Update the text of the color controls according to the current gradient
 setting.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
 INT index : 0 (horizontal) 1 (vertical) 2 (double) gradient.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setShadeControls(HWND hwnd, INT index) {
   CHAR buf[64];
   loadString(IDS_CLRSHADELEFT + index, buf);
   setCtrlTextParm(hwnd, TXT_TBSHADECLR1, IDS_GRADIENTCOLOR, buf);
   loadString(IDS_CLRSHADRIGHT + index, buf);
   setCtrlTextParm(hwnd, TXT_TBSHADECLR2, IDS_GRADIENTCOLOR, buf);
}


/* --------------------------------------------------------------------------
 On gradient settings change update the text of the shade color controls.
- Parameters -------------------------------------------------------------
 HWND hwnd  : dialog window handle.
 HWND hCtrl : background combobox handle.
 PTBARHILITESETS p : active/inactive titlebar settings.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL handleShadeOptionChange(HWND hwnd, HWND hCtrl, PTBARHILITESETS p) {
   INT iitem;
   if (LIT_NONE != (iitem = wLbxItemSelected(hCtrl))) {
      p->pSetsCur->fl &= ~(TBO_VERTSHADE | TBO_SHADEDOUBLE);
      switch (iitem) {
         case 1:
            p->pSetsCur->fl |= TBO_VERTSHADE;
            break;
         case 2:
            p->pSetsCur->fl |= TBO_SHADEDOUBLE;
            break;
         case 3:
            iitem = 2;
            p->pSetsCur->fl |= TBO_VERTSHADE | TBO_SHADEDOUBLE;
            break;
      } /* endswitch */
      setShadeControls(hwnd, iitem);
      setNewTitlebarBackground(hwnd, p, NULL, 0);
      return TRUE;
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 On background image change:
 - if the selected item is the first item (default) set to NULL the name
   of the bitmap file.
 - get the bitmap handle of the new image via the worker thread.
 - on worker thread job completion update the preview window (see
   onWorkerJobEnd() ).
- Parameters -------------------------------------------------------------
 HWND hwnd  : dialog window handle.
 HWND hCtrl : background combobox handle.
 PTBARHILITESETS p : active/inactive titlebar settings.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL handleBkgndImageChange(HWND hwnd, HWND hCtrl, PTBARHILITESETS p) {
   INT iitem;
   if (LIT_NONE != (iitem = wLbxItemSelected(hCtrl))) {
      if (!iitem) {
         *p->pBmp->pszCur = 0;
      } else {
         wLbxItemText(hCtrl, iitem, CCHMAXPATHCOMP, p->pBmp->pszCur);
      } /* endif */
      return getTitlebarBitmapData(hwnd, p);
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Compare the options set in the dialog with the active options.
 If there is any different option enable the 'Apply' button.
- Parameters -------------------------------------------------------------
 PTBARHILITESETS p : active/inactive titlebar settings.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID checkApplyState(PTBARHILITESETS p) {
   BOOL bEnable;
   bEnable = (memcmp(p->pSetsCur, p->pSetsApplied,
              sizeof(TBARHILITE) - sizeof(BMPPAINT))
              ||
              ((p->pSetsCur->bkgnd == TBARBKGNDBMP)
                && !mStringMatch(p->pBmp->pszCur, p->pBmp->pszApplied))
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
 PTBARHILITESETS p : active/inactive titlebar settings.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID checkUndoState(PTBARHILITESETS p) {
   BOOL bEnable;
   bEnable = (memcmp(p->pSetsCur, p->pSetsUndo,
              sizeof(TBARHILITE) - sizeof(BMPPAINT))
              ||
              ((p->pSetsCur->bkgnd == TBARBKGNDBMP)
                && !mStringMatch(p->pBmp->pszCur, p->pBmp->pszUndo))
             );
   if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_UNDO))
      resetCommonButton(BTN_UNDO, PGFL_UNDO_ENABLED,
                        (bEnable ? PGFL_UNDO_ENABLED : 0));
}


/* --------------------------------------------------------------------------
 Compare the options set in the dialog with the default options.
 If there is any different option enable the 'Default' button.
- Parameters -------------------------------------------------------------
 PTBARHILITESETS p : active/inactive titlebar settings.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID checkDefaultState(PTBARHILITESETS p) {
   BOOL bEnable;
   TBARHILITE tbodef;
   memset(&tbodef, 0, sizeof(tbodef));
   stlrSetTitlebarHiliteDefaults(&tbodef, (p->pBmp->flPreview == PVUPD_TITLEACTIVE));
   bEnable = (memcmp(p->pSetsCur, &tbodef, sizeof(TBARHILITE) - sizeof(BMPPAINT))
              ||
              ((p->pSetsCur->bkgnd == TBARBKGNDBMP) && *p->pBmp->pszCur)
             );
   if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_DEFAULT))
      resetCommonButton(BTN_DEFAULT, PGFL_DEFAULT_ENABLED,
                        (bEnable ? PGFL_DEFAULT_ENABLED : 0));
}


/* --------------------------------------------------------------------------
 Apply the current settings: init the APPLYBMP structure and
 complete the apply operation via the worker thread or directly
 via deferredApplyOptions().
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
 PTBARHILITESETS p : active/inactive titlebar settings.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID applyOptions(HWND hwnd, PTBARHILITESETS p) {
   APPLYBMP abmp;
   abmp.pBmp = p->pBmp;
   abmp.pNew = p->pSetsCur;
   abmp.pPrev = p->pSetsApplied;
   if (((PTBARHILITE)abmp.pNew)->bkgnd == TBARBKGNDBMP) {
      applyBitmap(hwnd, SZ_BMPPATHTBAR, p->pBmp->pszCur, &abmp);
   } else {
      deferredApplyOptions(hwnd, APPLYBMP_SUCCESS, &abmp);
   } /* endif */
   resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED, 0);
}


/* --------------------------------------------------------------------------
 Complete the apply operation.
- Parameters -------------------------------------------------------------
 HWND hwnd    : dialog window handle.
 ULONG rc     : worker thread job return value (0 = success).
 PAPPLYBMP pa : apply-bitmap-options data structure.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL deferredApplyOptions(HWND hwnd, ULONG rc, PAPPLYBMP pa) {
   HBITMAP hBmpPrev;
   if (rc) {
      handleApplyBitmapJobError(rc, pa);
      return FALSE;
   } /* endif */
#ifdef _STLR_TEST_MODE_
   memcpy(pa->pPrev, pa->pNew, sizeof(TBARHILITE));
   if (((PTBARHILITE)pa->pNew)->bkgnd == TBARBKGNDBMP) {
      if (*pa->achBmpFile) {
         strcpy(pa->pBmp->pszApplied, strrchr(pa->achBmpFile, '\\') + 1);
      } else {
         *pa->pBmp->pszApplied = '\x00';
      } /* endif */
   } /* endif */
#else
   hBmpPrev = ((PTBARHILITE)pa->pPrev)->bmpp.hbmp;
   switch (((PTBARHILITE)pa->pNew)->bkgnd) {
      case TBARBKGNDBMP:
         if (!applyBkgndBitmap(hwnd, pa)) return FALSE;
         break;
      case TBARBKGNDSHADE:
         if (!applyBkgndShade(hwnd, pa)) return FALSE;
         break;
      case TBARBKGNDSOLIDCLR:
         applyBkgndColor(hwnd, pa);
         break;
   } /* endswitch */
   freeGlobalBitmap(hwnd, hBmpPrev);
   m_stlrMsgBroadcast(STLRM_REDRAWTITLE, pa->pBmp->updateMsg);
#endif
   return TRUE;
}


/* --------------------------------------------------------------------------
 Apply a new background bitmap.
- Parameters -------------------------------------------------------------
 HWND hwnd    : dialog window handle.
 PAPPLYBMP pa : apply-bitmap-options data structure.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL applyBkgndBitmap(HWND hwnd, PAPPLYBMP pa) {
   PBYTE pData;
   BMPPAINT bmpp;
   HPS hps;
   BOOL rc = FALSE;
   pData = *pa->achBmpFile ? pa->aBmpData: NULL;
   // get the new bitmap handle
   if (NULLHANDLE != (hps = WinGetPS(hwnd))) {
      bmpp.hbmp = stlrHBmp(hps, pData, &bmpp, pa->pBmp->idRes, NULL);
      if (bmpp.hbmp && !stlrMakeGlobalBitmap(hps, bmpp.hbmp)) {
         GpiDeleteBitmap(bmpp.hbmp);
         bmpp.hbmp = NULLHANDLE;
      } /* endif */
      WinReleasePS(hps);
   } /* endif */
   if (hps && bmpp.hbmp) {
      memcpy(pa->pPrev, pa->pNew, sizeof(TBARHILITE) - sizeof(BMPPAINT));
      memcpy(&((PTBARHILITE)pa->pPrev)->bmpp, &bmpp, sizeof(bmpp));
      if (*pa->achBmpFile) {
         strcpy(pa->pBmp->pszApplied, strrchr(pa->achBmpFile, '\\') + 1);
      } else {
         *pa->pBmp->pszApplied = '\x00';
      } /* endif */
      rc = TRUE;
   } else {
      errorBox(ISERR_APPLYBMP);
      resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED, PGFL_APPLY_ENABLED);
   } /* endif */
   return rc;
}


/* --------------------------------------------------------------------------
 Apply a new background shade.
- Parameters -------------------------------------------------------------
 HWND hwnd    : dialog window handle.
 PAPPLYBMP pa : apply-bitmap-options data structure.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL applyBkgndShade(HWND hwnd, PAPPLYBMP pa) {
   PSHDCREATE psc;
   HPS hps;
   BOOL rc = FALSE;
   HBITMAP hbmp;
   if (NULL == (psc = malloc(CB_SHADEALLOC))) {
      resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED, PGFL_APPLY_ENABLED);
      return handleError(ISERR_ALLOCATION, FALSE);
   } /* endif */
   // get the new bitmap handle
   if (NULLHANDLE != (hps = WinGetPS(hwnd))) {
      hbmp = stlrShadeHBmp(hps, psc, ((PTBARHILITE)pa->pNew)->fl,
                           ((PTBARHILITE)pa->pNew)->clrLeftTop,
                           ((PTBARHILITE)pa->pNew)->clrRghtBtm);
      if (hbmp && !stlrMakeGlobalBitmap(hps, hbmp)) {
         GpiDeleteBitmap(hbmp);
      } /* endif */
      WinReleasePS(hps);
   } /* endif */
   free(psc);
   if (hps && hbmp) {
      memcpy(pa->pPrev, pa->pNew, sizeof(TBARHILITE));
      ((PTBARHILITE)pa->pPrev)->bmpp.hbmp = hbmp;
      rc = TRUE;
   } else {
      errorBox(ISERR_APPLYBMP);
      resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED, PGFL_APPLY_ENABLED);
   } /* endif */
   return rc;
}


/* --------------------------------------------------------------------------
 Apply a new background (solid) color.
- Parameters -------------------------------------------------------------
 HWND hwnd    : dialog window handle.
 PAPPLYBMP pa : apply-bitmap-options data structure.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID applyBkgndColor(HWND hwnd, PAPPLYBMP pa) {
   // get the new bitmap handle
   memcpy(pa->pPrev, pa->pNew, sizeof(TBARHILITE));
   memset(&((PTBARHILITE)pa->pPrev)->bmpp, 0, sizeof(BMPPAINT));
}


/* --------------------------------------------------------------------------
 Reset the current settings to the values they had when the preferences
 notebook was opened.
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
 PTBARHILITESETS p : active/inactive titlebar settings.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID undoOptions(HWND hwnd, PTBARHILITESETS p) {
   // free the current bitmap handle
   GpiDeleteBitmap(p->pSetsCur->bmpp.hbmp);
   // copy the undo settings
   memcpy(p->pSetsCur, p->pSetsUndo, sizeof(TBARHILITE));
   memset(&p->pSetsCur->bmpp, 0, sizeof(BMPPAINT));
   // get the undo bitmap handle and data (if needed)
   getTitlebarHbmp(NULLHANDLE, p->pBmp->pDataUndo, p->pSetsCur,
                   NULL, p->pBmp->idRes);
   // copy the bitmap file name
   strcpy(p->pBmp->pszCur, p->pBmp->pszUndo);
   setControlsState(hwnd, p->pSetsCur, p->pBmp->pszCur);
   updatePreviewWindow(p->pBmp->flPreview);
   checkApplyState(p);
   checkDefaultState(p);
   resetCommonButton(BTN_UNDO, PGFL_UNDO_ENABLED, 0);
}


/* --------------------------------------------------------------------------
 Reset the current settings to the default values.
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
 PTBARHILITESETS p : active/inactive titlebar settings.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID defaultOptions(HWND hwnd, PTBARHILITESETS p) {
   // free the current bitmap handle
   GpiDeleteBitmap(p->pSetsCur->bmpp.hbmp);
   stlrSetTitlebarHiliteDefaults(p->pSetsCur,
                                 (p->pBmp->flPreview == PVUPD_TITLEACTIVE));
   getTitlebarHbmp(NULLHANDLE, NULL, p->pSetsCur, NULL, p->pBmp->idRes);
   setControlsState(hwnd, p->pSetsCur, p->pBmp->pszCur);
   updatePreviewWindow(p->pBmp->flPreview);
   checkApplyState(p);
   checkUndoState(p);
   resetCommonButton(BTN_DEFAULT, PGFL_DEFAULT_ENABLED, 0);
}


/* --------------------------------------------------------------------------
 Swap the colors of 2 buttons.
- Parameters -------------------------------------------------------------
 HWND hwnd    : dialog window handle.
 ULONG id1    : id of the first button.
 ULONG id2    : id of the second button.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID swapColors(HWND hwnd, ULONG id1, ULONG id2) {
   LONG lColor1, lColor2;
   HWND hBtn1, hBtn2;
   hBtn1 = WinWindowFromID(hwnd, id1);
   hBtn2 = WinWindowFromID(hwnd, id2);
   lColor1 = wClrBtnColor(hBtn1);
   lColor2 = wClrBtnColor(hBtn2);
   g.state |= STLRIS_COLORSWAPPED;
   wClrBtnColorSet(hBtn1, lColor2);
   wClrBtnColorSet(hBtn2, lColor1);
}


/* --------------------------------------------------------------------------
 Get the selected titlebar bitmap data via the worker thread.
- Parameters -------------------------------------------------------------
 HWND hwnd  : dialog window handle.
 PTBARHILITESETS p : active/inactive titlebar settings.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL getTitlebarBitmapData(HWND hwnd, PTBARHILITESETS p) {
   PGETBMPDATA pGetBmp;
   if (NULL == (pGetBmp = malloc(sizeof(GETBMPDATA) - 4)))
      return handleError(ISERR_ALLOCATION, FALSE);
   memset(pGetBmp, 0, sizeof(GETBMPDATA) - 4);
   pGetBmp->pBmp = p->pBmp;
   pGetBmp->pOpts = p->pSetsCur;
   if (*p->pBmp->pszCur) {
      makeFullPathName(pGetBmp->achFile, SZ_BMPPATHTBAR);
      strcat(pGetBmp->achFile, p->pBmp->pszCur);
   } /* endif */
   return workerJobAdd(hwnd, STLRWID_GETBMPDATA,
                       (PTHREADJOBFN)getBitmapDataJob,
                       (PDELJOBFN)freeGetBitmapDataRes, pGetBmp);
}


/* --------------------------------------------------------------------------
 Set a new titlebar background:
- Parameters -------------------------------------------------------------
 HWND hwnd        : dialog window handle.
 PTBARHILITESETS p : active/inactive titlebar settings.
 PGETBMPDATA pgb  : GETBMPDATA structure address (NULL if this is not
                    a result of worker thread job end.
 ULONG rc         : worker thread job return code.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setNewTitlebarBackground(HWND hwnd, PTBARHILITESETS p,
                                     PGETBMPDATA pgb, ULONG rc) {
   HBITMAP hBmpPrev;
   ULONG flUpdate;
   // if called as result of worker thread job end
   if (pgb) {
      if (rc) {
         handleGetBitmapJobError(rc, pgb);
         return;
      } else {
         flUpdate = pgb->pBmp->flPreview;
         hBmpPrev = ((PTBARHILITE)pgb->pOpts)->bmpp.hbmp;
      } /* endif */
      ((PTBARHILITE)pgb->pOpts)->bmpp = pgb->bmpp;
   } else {
      flUpdate = p->pBmp->flPreview;
      if (p->pSetsCur->bkgnd == TBARBKGNDSHADE) {
         hBmpPrev = p->pSetsCur->bmpp.hbmp;
         getTitlebarHbmp(NULLHANDLE, NULL, p->pSetsCur, NULL, 0);
      } else {
         hBmpPrev = NULLHANDLE;
      } /* endif */
   } /* endif */
   updatePreviewWindow(flUpdate);
   GpiDeleteBitmap(hBmpPrev);
}

