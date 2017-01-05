/* --------------------------------------------------------------------------
 pageUIBtn.c : push button settings dialog procedure.

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
static VOID onWorkerJobEnd(HWND hwnd, ULONG jobId, ULONG rc);
static VOID setControlsState(HWND hwnd, PSZ pszBmpFile);
static VOID setEnableDependencies(HWND hwnd);
static VOID checkApplyState(VOID);
static VOID checkUndoState(VOID);
static VOID checkDefaultState(VOID);
static VOID applyOptions(HWND hwnd);
static BOOL deferredApplyOptions(HWND hwnd, ULONG rc, PAPPLYBMP pa);
static VOID addButtonImage(HWND hwnd);
static BOOL deferredApplyOptions(HWND hwnd, ULONG rc, PAPPLYBMP pa);
static VOID undoOptions(HWND hwnd);
static VOID defaultOptions(HWND hwnd);
static BOOL getBtnImageData(HWND hwnd, HWND hCtrl);
static VOID setNewBtnBackground(HWND hwnd, PGETBMPDATA pgb, ULONG rc);

// global variables ---------------------------------------------------------

/* --------------------------------------------------------------------------
 Push button settings dialog procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY uiBtnPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
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
   BMPLIMITS bmpLimits = {8, 40, 40};
   initPage(hwnd);
   listBitmaps(hwnd, SZ_BMPPATHBTN, g.pUiData->btn.pszCur, &bmpLimits);
   setControlsState(hwnd, NULL);
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
      case CHK_BTNON:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pUiData->pOpts->btn.on = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
            setEnableDependencies(hwnd);
            updatePreviewWindow(PVUPD_BTNALL);
         } /* endif */
         break;
      case CHK_BTNTHICK:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pUiData->pOpts->btn.border = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
            updatePreviewWindow(PVUPD_BTNALL);
         } /* endif */
         break;
      case CHK_BTNFLAT:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pUiData->pOpts->btn.flat = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
            updatePreviewWindow(PVUPD_BTNALL);
         } /* endif */
         break;
      case CHK_BTNOVRCLR:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pUiData->pOpts->btn.overPP = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
            updatePreviewWindow(PVUPD_BTNALL);
         } /* endif */
         break;
      case CHK_BTN3DDEF:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pUiData->pOpts->btn.def3D = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
            updatePreviewWindow(PVUPD_BTNDEFAULT);
         } /* endif */
         break;
      case CHK_BTN3DDIS:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pUiData->pOpts->btn.dis3D = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
            updatePreviewWindow(PVUPD_BTNDISABLED);
         } /* endif */
         break;
      case CHK_BTNSOLID:
         if ((event == BN_CLICKED) || (event == BN_DBLCLICKED)) {
            g.pUiData->pOpts->btn.solid = wBtnCheckState(hCtrl);
            bSettingsChanged = TRUE;
            dlgItemMultiEnable(hwnd, TXT_BTNIMAGE, BTN_BTNADDIMAGE,
                               g.pUiData->pOpts->btn.on && !g.pUiData->pOpts->btn.solid);
            if (g.pUiData->pOpts->btn.solid) {
               setNewBtnBackground(hwnd, NULL, 0);
            } else {
               // in case of error set the button background to solid color
               if (!getBtnImageData(hwnd, DlgItemHwnd(hwnd, COMBO_BTNIMAGE))) {
                  bSettingsChanged = FALSE;
                  g.state |= STLRIS_SKIPNOTIFICATION;
                  wBtnCheckSet(hCtrl, 1);
                  g.pUiData->pOpts->btn.solid = 1;
                  g.state &= ~STLRIS_SKIPNOTIFICATION;
               } /* endif */
            } /* endif */
         } /* endif */
         break;
      case COMBO_BTNIMAGE:
         if (event == LN_SELECT) {
            if (getBtnImageData(hwnd, hCtrl))
               bSettingsChanged = TRUE;
         } /* endif */
         break;
//      case BTN_BTNBKGNDCLR:
//         if (event == BN_COLORCHANGED) {
//            g.pUiData->pOpts->btn.color = (LONG)hCtrl;
//            if (!(g.state & STLRIS_COLORSELDLG)) bSettingsChanged = TRUE;
//            updatePreviewWindow(PVUPD_BTNALL);
//         } /* endif */
//         break;
   } /* endswitch */
   if (bSettingsChanged) {
      checkApplyState();
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
      case BTN_APPLY:
         applyOptions(hwnd);
         break;
      case BTN_UNDO:
         undoOptions(hwnd);
         break;
      case BTN_DEFAULT:
         defaultOptions(hwnd);
         break;
      case BTN_BTNADDIMAGE:
         addButtonImage(hwnd);
         break;
//      case BTN_BTNBKGNDCLR:
//         selectColor(hwnd, BTN_BTNBKGNDCLR, g.pUiData->pOpts->btn.color);
//         break;
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
         fillBitmapList(DlgItemHwnd(hwnd, COMBO_BTNIMAGE),
                        (PLISTBMPFILES)g.worker.pCurJob->pData,
                        g.pUiData->pOpts->btn.on && !g.pUiData->pOpts->btn.solid);
         break;
      case STLRWID_APPLYBMP:
         deferredApplyOptions(hwnd, rc, (PAPPLYBMP)g.worker.pCurJob->pData);
         break;
      case STLRWID_GETBMPDATA:
         setNewBtnBackground(hwnd, (PGETBMPDATA)g.worker.pCurJob->pData, rc);
         break;
      case STLRWID_ADDBMP:
         addImageJobEnd(DlgItemHwnd(hwnd, COMBO_BTNIMAGE),
                        rc, (PADDBMPFILE)g.worker.pCurJob->pData);
         break;
   } /* endswitch */
   workerJobUnlink();
}


/* --------------------------------------------------------------------------
 Set the state of the dialog controls according to the current value of
 the options.
- Parameters -------------------------------------------------------------
 HWND hwnd        : dialog window handle.
 PSZ pszBmpFile   : if not NULL select pszBmpFile in the image list.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setControlsState(HWND hwnd, PSZ pszBmpFile) {
   HWND hwndLbox;
   g.state |= STLRIS_SKIPNOTIFICATION;
   dBtnCheckSet(hwnd, CHK_BTNON, g.pUiData->pOpts->btn.on);
   dBtnCheckSet(hwnd, CHK_BTNTHICK, g.pUiData->pOpts->btn.border);
   dBtnCheckSet(hwnd, CHK_BTNFLAT, g.pUiData->pOpts->btn.flat);
   dBtnCheckSet(hwnd, CHK_BTNOVRCLR, g.pUiData->pOpts->btn.overPP);
   dBtnCheckSet(hwnd, CHK_BTN3DDEF, g.pUiData->pOpts->btn.def3D);
   dBtnCheckSet(hwnd, CHK_BTN3DDIS, g.pUiData->pOpts->btn.dis3D);
   dBtnCheckSet(hwnd, CHK_BTNSOLID, g.pUiData->pOpts->btn.solid);
   if (pszBmpFile)
      selectListItem(DlgItemHwnd(hwnd, COMBO_BTNIMAGE), pszBmpFile);
//   dClrBtnColorSet(hwnd, BTN_BTNBKGNDCLR, g.pUiData->pOpts->btn.color);
   setEnableDependencies(hwnd);
   g.state &= ~STLRIS_SKIPNOTIFICATION;
}


/* --------------------------------------------------------------------------
 Enable/disable the secondary controls according to the check state of the
 'Pushbuttons enhancements' check box
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setEnableDependencies(HWND hwnd) {
   dlgItemMultiEnable(hwnd, CHK_BTNTHICK, CHK_BTNSOLID, g.pUiData->pOpts->btn.on);
   dlgItemMultiEnable(hwnd, TXT_BTNIMAGE, BTN_BTNADDIMAGE,
                      g.pUiData->pOpts->btn.on && !g.pUiData->pOpts->btn.solid);
//   dlgItemMultiEnable(hwnd, TXT_BTNBKGNDCLR, BTN_BTNBKGNDCLR, g.pUiData->pOpts->btn.on);
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
   bEnable = memcmp(&g.pUiData->pOpts->btn, &g.pCurOpts->ui.btn,
                    sizeof(BTNOPT) - sizeof(BMPPAINT))
             ||
             !mStringMatch(g.pUiData->btn.pszCur, g.pUiData->btn.pszApplied);
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
   bEnable = memcmp(&g.pUiData->pOpts->btn, &g.pUndoUiOpts->btn,
                    sizeof(BTNOPT) - sizeof(BMPPAINT))
             ||
             !mStringMatch(g.pUiData->btn.pszCur, g.pUiData->btn.pszUndo);
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
   bEnable = (!g.pUiData->pOpts->btn.on
              ||
              g.pUiData->pOpts->btn.border != (g.scr.cx > 1280)
              ||
              g.pUiData->pOpts->btn.flat
              ||
              g.pUiData->pOpts->btn.overPP
              ||
              g.pUiData->pOpts->btn.def3D
              ||
              !g.pUiData->pOpts->btn.dis3D
              ||
              g.pUiData->pOpts->btn.solid != (g.scr.cclr < 256)
              ||
              g.pUiData->pOpts->btn.color != 0xcccccc
              ||
              *g.pUiData->btn.pszCur
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
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID applyOptions(HWND hwnd) {
   APPLYBMP abmp;
   abmp.pBmp = &g.pUiData->btn;
   abmp.pNew = &g.pUiData->pOpts->btn;
   abmp.pPrev = &g.pCurOpts->ui.btn;
   // solid color: there is no need to use the worker thread so directly
   // call the apply-option completion procedure
   if (g.pUiData->pOpts->btn.solid) {
      deferredApplyOptions(hwnd, APPLYBMP_SUCCESS, &abmp);
   } else {
      applyBitmap(hwnd, SZ_BMPPATHBTN, g.pUiData->btn.pszCur, &abmp);
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
   PBYTE pData;
   HPS hps;
   BTNOPT bo; // temporary storage for the new button options
   if (rc) {
      handleApplyBitmapJobError(rc, pa);
      return FALSE;
   } /* endif */
#ifdef _STLR_TEST_MODE_
   memcpy(pa->pPrev, pa->pNew, sizeof(BTNOPT));
   if (!((PBTNOPT)pa->pNew)->solid) {
      if (*pa->achBmpFile) {
         strcpy(pa->pBmp->pszApplied, strrchr(pa->achBmpFile, '\\') + 1);
      } else {
         *pa->pBmp->pszApplied = '\x00';
      } /* endif */
   } /* endif */
#else
   hBmpPrev = ((PBTNOPT)pa->pPrev)->bmpp.hbmp;
   // a solid color is used as button background
   if (((PBTNOPT)pa->pNew)->solid) {
      memcpy(pa->pPrev, pa->pNew, sizeof(BTNOPT));
   // a bitmap is used as button background
   } else {
      memcpy(&bo, pa->pNew, sizeof(BTNOPT));
      if (NULLHANDLE != (hps = WinGetPS(hwnd))) {
         pData = *pa->achBmpFile ? pa->aBmpData: NULL;
         bo.bh.cbFix = 0, bo.bh.cx = bo.bh.cy = 40;
         bo.bmpp.hbmp = stlrHBmp(hps, pData, &bo.bmpp, pa->pBmp->idRes,
                                 (PBMPDAT)&bo.bh);
         if (bo.bmpp.hbmp && !stlrMakeGlobalBitmap(hps, bo.bmpp.hbmp)) {
            GpiDeleteBitmap(bo.bmpp.hbmp);
            bo.bmpp.hbmp = NULLHANDLE;
         } /* endif */
         WinReleasePS(hps);
      } /* endif */
      if (hps && bo.bmpp.hbmp) {
         memcpy(pa->pPrev, &bo, sizeof(BTNOPT));
         if (*pa->achBmpFile) {
            strcpy(pa->pBmp->pszApplied, strrchr(pa->achBmpFile, '\\') + 1);
         } else {
            *pa->pBmp->pszApplied = '\x00';
         } /* endif */
      } else {
         resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED, PGFL_APPLY_ENABLED);
         return handleError(ISERR_APPLYBMP, FALSE);
      } /* endif */
   } /* endif */
   freeGlobalBitmap(hwnd, hBmpPrev);
   WinInvalidateRect(HWND_DESKTOP, NULL, TRUE);
#endif
   return TRUE;
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
   // free the current bitmap handle
   GpiDeleteBitmap(g.pUiData->pOpts->btn.bmpp.hbmp);
   // copy the undo settings
   memcpy(&g.pUiData->pOpts->btn, &g.pUndoUiOpts->btn, sizeof(BTNOPT));
   memset(&g.pUiData->pOpts->btn.bmpp, 0, sizeof(BMPPAINT));
   // get the undo bitmap handle and data (if needed)
   if (!g.pUiData->pOpts->btn.solid)
      getBtnHbmp(NULLHANDLE, g.pUiData->btn.pDataUndo, &g.pUiData->pOpts->btn);
   // copy the bitmap file name
   strcpy(g.pUiData->btn.pszCur, g.pUiData->btn.pszUndo);
   setControlsState(hwnd, g.pUiData->btn.pszCur);
   updatePreviewWindow(PVUPD_BTNALL);
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
   // free the current bitmap handle
   GpiDeleteBitmap(g.pUiData->pOpts->btn.bmpp.hbmp);
   memset(&g.pUiData->pOpts->btn.bmpp, 0, sizeof(BMPPAINT));
   g.pUiData->btn.pszCur[0] = 0;
   // set the default values
   stlrSetButtonDefaults(&g.pUiData->pOpts->btn);
   // get the new button bitamp
   if (!g.pUiData->pOpts->btn.solid)
      getBtnHbmp(NULLHANDLE, NULL, &g.pUiData->pOpts->btn);
   setControlsState(hwnd, g.pUiData->btn.pszCur);
   updatePreviewWindow(PVUPD_BTNALL);
   checkApplyState();
   checkUndoState();
   resetCommonButton(BTN_DEFAULT, PGFL_DEFAULT_ENABLED, 0);
}


/* --------------------------------------------------------------------------
 Show the file dialog to let the user choose an image to be added in the
 list of the images to be used for the pushbutton background.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID addButtonImage(HWND hwnd) {
   BMPLIMITS bmpLimits = {8, 40, 40};
   addImage(hwnd, SZ_BMPPATHBTN, &bmpLimits);
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
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL getBtnImageData(HWND hwnd, HWND hCtrl) {
   INT iitem;
   PGETBMPDATA pGetBmp;
   ULONG cb;
   if (LIT_NONE != (iitem = wLbxItemSelected(hCtrl))) {
      if (!iitem) {
         g.pUiData->btn.pszCur[0] = 0;
      } else {
         wLbxItemText(hCtrl, iitem, CCHMAXPATHCOMP, g.pUiData->btn.pszCur);
      } /* endif */
      cb = sizeof(GETBMPDATA) + sizeof(BMPHDR) + 1600 - 4;
      if (NULL == (pGetBmp = malloc(cb)))
         return handleError(ISERR_ALLOCATION, FALSE);
      memset(pGetBmp, 0, sizeof(GETBMPDATA) - 4);
      pGetBmp->pBmp = &g.pUiData->btn;
      pGetBmp->pOpts = &g.pUiData->pOpts->btn;
      if (iitem) {
         makeFullPathName(pGetBmp->achFile, SZ_BMPPATHBTN);
         strcat(pGetBmp->achFile, g.pUiData->btn.pszCur);
      } /* endif */
      pGetBmp->cbData = cb;
      ((PBMPHDR)pGetBmp->ab)->cx = 40;
      ((PBMPHDR)pGetBmp->ab)->cy = 40;
      return workerJobAdd(hwnd, STLRWID_GETBMPDATA,
                          (PTHREADJOBFN)getBitmapDataJob,
                          (PDELJOBFN)freeGetBitmapDataRes, pGetBmp);
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Set a new button background:
- Parameters -------------------------------------------------------------
 HWND hwnd        : dialog window handle.
 PGETBMPDATA pgb  : GETBMPDATA structure address.
 ULONG rc         : worker thread job return code.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setNewBtnBackground(HWND hwnd, PGETBMPDATA pgb, ULONG rc) {
   HBITMAP hBmpPrev;
   if (rc) {
      handleGetBitmapJobError(rc, pgb);
   } else {
      hBmpPrev = g.pUiData->pOpts->btn.bmpp.hbmp;
      if (pgb) {
         g.pUiData->pOpts->btn.bmpp = pgb->bmpp;
      } else {
         memset(&g.pUiData->pOpts->btn.bmpp, 0, sizeof(g.pUiData->pOpts->btn.bmpp));
      } /* endif */
      updatePreviewWindow(PVUPD_BTNALL);
      GpiDeleteBitmap(hBmpPrev);
   } /* endif */
}
