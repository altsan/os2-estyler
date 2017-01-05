/* --------------------------------------------------------------------------
 button.c: enhanced WC_BUTTON window procedure.
 2004/05/08 - Alessandro Cantatore - Italy

 naming conventions:
 p            is the address of the BTNDATA structure
 pbd          is the address of the structure allocated to draw the button
 m*           (followed by upper case) is the name of the macros defined in
              .\button.h.
 m_*          is the name of the global macros defined in ..\headers\macros.h
-------------------------------------------------------------------------- */

#pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"
#include "button.h"

// prototypes ---------------------------------------------------------------

// global variables ---------------------------------------------------------
extern PID pid;


// definitions --------------------------------------------------------------

#define WPMENUID_SHUTDOWN  704

// prototypes ---------------------------------------------------------------
static BOOL setBtnText(HWND hwnd, PCREATESTRUCT pCreateParm, PBTNDATA p);
static BOOL initBtnColors(HWND hwnd, PBTNDATA p);
static VOID resetDefBtnWndProc(HWND hwnd, PBTNDATA p);
static BOOL updatePresParms(HWND hwnd, ULONG idPresParm, PBTNDATA p);
static BOOL handleSysColorChange(HWND hwnd, PBTNDATA p);
static MRESULT overrideBtnProc(HWND hwnd, ULONG msg,
                        MPARAM mp1, MPARAM mp2, PBTNDATA p);
static BOOL handlePaintRequest(HWND hwnd, PBTNDATA p);
static VOID paintBtn(HWND hwnd, PBTNDATA p, PBTNDRAW pbd);
static VOID drawTempBitmap(PBTNDATA p, PBTNDRAW pbd);
VOID stlrSetBtnPalette(PBMPHDR pbh, PCLR pClrBkgnd);
static BOOL setFgndClipRectangle(HPS hps, PRECTL pr);
static VOID drawFgndBitmap(PBTNDATA p, PBTNDRAW pbd);
static VOID drawFgndMiniIcon(PBTNDATA p, PBTNDRAW pbd);
static VOID drawFgndIcon(PBTNDATA p, PBTNDRAW pbd);
static ULONG drawBtnText(PBTNDATA p, PBTNDRAW pbd, ULONG cyimg);
static VOID drawBorder(HWND hwnd, PBTNDATA p, PBTNDRAW pbd);
static BOOL drawDisabledRect(HPS hps, PRECTL prcl, LONG color);
static VOID getBtnColors(HWND hwnd, PBTNDATA p, HPS hps);
static LONG getCtlClr(HWND hwnd, HPS hps, ULONG ulid1, ULONG ulid2, LONG ldef);
static VOID deferWarpCenterSubclassing(HWND hwnd);
static BOOL EXPENTRY hookWcProc(HAB hab, PQMSG pqmsg, USHORT fs);
static MRESULT EXPENTRY newWcProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static VOID warpCenterCmd(ULONG fl);
static VOID openWCProcessList(ULONG cyWarpCenter);


/* --------------------------------------------------------------------------
 Enhanced pushbutton procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT
-------------------------------------------------------------------------- */
MRESULT EXPENTRY stlrButtonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PBTNDATA p;
//**/  return (MRESULT)g.cd.btn.pfnwp(hwnd, msg, mp1, mp2);
   switch (msg) {
      case WM_PAINT:
         if (pid
             &&
             mStlrBtnOn()
             &&
             (NULL != (p = mBtnData(hwnd)))
             &&
             handlePaintRequest(hwnd, p))
            return MRFALSE;
         break;
      // button state change ---------------------------------------------
      case BM_SETDEFAULT:
      case BM_SETHILITE:
         if (pid && mStlrBtnOn()
             &&
             (NULL != (p = mBtnData(hwnd))) && mHasBorder(hwnd))
            return overrideBtnProc(hwnd, msg, mp1, mp2, p);
         break;
      // a system color changed ------------------------------------------
      case WM_SYSCOLORCHANGE:
         if (pid && (NULL != (p = mBtnData(hwnd))))
            if (handleSysColorChange(hwnd, p)) return MRFALSE;
         break;
      // presentation parameters changed ---------------------------------
      case WM_PRESPARAMCHANGED:
         if (pid
             &&
             (NULL != (p = mBtnData(hwnd)))
             &&
             updatePresParms(hwnd, (ULONG)mp1, p))
            return MRFALSE;
         break;
      // button window text change ---------------------------------------
      case WM_SETWINDOWPARAMS:
         if (pid && (NULL != (p = mBtnData(hwnd))) && mp1) {
            if (mp1 && m_wndParm(mp1)->fsStatus & WPM_TEXT) {
               heapLockPM(hwnd);
               memFree(p->pszText);
               if (m_wndParm(mp1)->pszText) {
                  p->cbText = strlen(m_wndParm(mp1)->pszText);
                  if (NULL != (p->pszText = memAlloc(p->cbText + 1))) {
                     memcpy(p->pszText, m_wndParm(mp1)->pszText, p->cbText + 1);
                  } else {
                     p->cbText = 0;
                  } /* endif */
               } else {
                  p->pszText = NULL;
               } /* endif */
               heapUnlock();
            } /* endif */
            if ((((PWNDPARAMS)mp1)->fsStatus & (WPM_TEXT | WPM_CTLDATA)) &&
                mHasBorder(hwnd))
               return overrideBtnProc(hwnd, msg, mp1, mp2, p);
         } /* endif */
         break;
      // window creation -------------------------------------------------
      case WM_CREATE:
dbgPrintf4("Button: %08x - style:%08x pid:%d\n",
           hwnd, m_creatParm(mp2)->flStyle, pid);
         // if this is the WarpCenter subclass its procedure
         if (mIsWarpCenter(mp2)) deferWarpCenterSubclassing(hwnd);

         // if this is not a real pushbutton, is an exception or in case
         // of failed memory allocation go to the default procedure
         if (
               !mIsPushBtn(m_creatParm(mp2)->flStyle)
               ||
               !pid
               ||
               !(p = (PBTNDATA)memLkAlloc(sizeof(BTNDATA)))
            )
         {
            // if the button has not been subclassed restore the default
            // button procedure
            if (WinQueryWindowPtr(hwnd, QWP_PFNWP) == (PVOID)stlrButtonProc)
               WinSetWindowPtr(hwnd, QWP_PFNWP, (PVOID)g.cd.btn.pfnwp);
            break;
         } /* endif */


         memset(p, 0, sizeof(BTNDATA));
         // store the button text
         if (m_creatParm(mp2)->pszText
             &&
             !setBtnText(hwnd, (PCREATESTRUCT)mp2, p))
            break;
         // get the button colors and store the button data address
         if (initBtnColors(hwnd, p))
            WinSetWindowPtr(hwnd, g.cd.btn.cbData, p);
         break;
      case WM_DESTROY:
         if (pid && (NULL != (p = mBtnData(hwnd))))
            resetDefBtnWndProc(hwnd, p);
         break;
   } /* endswitch */
   return (MRESULT)g.cd.btn.pfnwp(hwnd, msg, mp1, mp2);
}


/* --------------------------------------------------------------------------
 Store the button text.
- Parameters -------------------------------------------------------------
 HWND hwnd                 : window handle
 PCREATESTRUCT pCreateParm : button creation parameters
 PBTNDATA p                : button data
- Return value -----------------------------------------------------------
 BOOL TRUE/FALSE (continue/undo superclassing)
-------------------------------------------------------------------------- */
static BOOL setBtnText(HWND hwnd, PCREATESTRUCT pCreateParm, PBTNDATA p) {
   PSZ psz;
   // get the correct button text (if BS_IMAGE skip the resource id)
   if ((pCreateParm->flStyle & BS_IMAGE) && (*pCreateParm->pszText == '#')) {
      if (NULL != (psz = strchr(pCreateParm->pszText, '\t'))) ++psz;
   } else {
      psz = pCreateParm->pszText;
   } /* endif */
   if (psz) {
      p->cbText = strlen(psz);
      if (!(p->pszText = memLkAlloc(p->cbText + 1))) {
         memLkFree(p);
         return FALSE;
      } /* endif */
      memcpy(p->pszText, psz, p->cbText + 1);
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 Initialize the button colors.
- Parameters -------------------------------------------------------------
 HWND hwnd  : button window handle.
 PBTNDATA p : button data
- Return value -----------------------------------------------------------
 BOOL TRUE/FALSE (continue/undo superclassing)
-------------------------------------------------------------------------- */
static BOOL initBtnColors(HWND hwnd, PBTNDATA p) {
   HPS hps;
   if (NULLHANDLE != (hps = WinGetPS(hwnd))) {
      getBtnColors(hwnd, p, hps);
      WinReleasePS(hps);
      return TRUE;
   } /* endif */
   resetDefBtnWndProc(hwnd, p);
   return FALSE;
}


/* --------------------------------------------------------------------------
 Free the storage allocated for the button enhancements and restore the
 operating system default button window procedure.
- Parameters -------------------------------------------------------------
 HWND hwnd  : button window handle.
 PBTNDATA p : button data
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID resetDefBtnWndProc(HWND hwnd, PBTNDATA p) {
   if (p) {
      if (p->pszText) memLkFree(p->pszText);
      memLkFree(p);
      WinSetWindowPtr(hwnd, g.cd.btn.cbData, NULL);
   } /* endif */
   // if the button has not been subclassed restore the default button procedure
   if (WinQueryWindowPtr(hwnd, QWP_PFNWP) == (PVOID)stlrButtonProc)
      WinSetWindowPtr(hwnd, QWP_PFNWP, (PVOID)g.cd.btn.pfnwp);
}


/* --------------------------------------------------------------------------
 As a consequence of a presentation parameter change update the colors
 and re-draw the button.
- Parameters -------------------------------------------------------------
 HWND hwnd        : button window handle.
 PBTNDATA p       : button data
 ULONG idPresParm : id of the presentation parameter which has been changed
- Return value -----------------------------------------------------------
 BOOL TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL updatePresParms(HWND hwnd, ULONG idPresParm, PBTNDATA p) {
   HPS hps = WinGetPS(hwnd);
   if (!hps) return FALSE;
   switch (idPresParm) {
      case PP_BACKGROUNDCOLOR:
      case PP_BACKGROUNDCOLORINDEX:
         p->lbk.l = mBkgndColor(hwnd, hps);
         break;
      case PP_FOREGROUNDCOLOR:
      case PP_FOREGROUNDCOLORINDEX:
         p->lfgnd = mFgndColor(hwnd, hps);
         break;
      case PP_DISABLEDBACKGROUNDCOLOR:
      case PP_DISABLEDBACKGROUNDCOLORINDEX:
         p->ldis = mDisabledBkgndColor(hwnd, hps);
         break;
      case PP_DISABLEDFOREGROUNDCOLOR:
      case PP_DISABLEDFOREGROUNDCOLORINDEX:
         p->ldisf = mDisabledFgndColor(hwnd, hps);
         break;
      case PP_BORDERDEFAULTCOLOR:
         p->ldef = mDefaultBorderColor(hwnd, hps);
         break;
      case PP_BORDERLIGHTCOLOR:
         p->llite = mHiliteBorderColor(hwnd, hps);
         break;
      case PP_BORDERDARKCOLOR:
         p->lshadow = mDarkBorderColor(hwnd, hps);
         break;
   } /* endswitch */
   WinReleasePS(hps);
   if (mStlrBtnOn()) CtrlUpdate(hwnd, FALSE);
   return mStlrBtnOn();
}


/* --------------------------------------------------------------------------
 When a system color changes, update the button colors and re-draw the button.
- Parameters -------------------------------------------------------------
 HWND hwnd  : button window handle.
 PBTNDATA p : button data.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (don't call/call the default WC_BUTTON procedure).
-------------------------------------------------------------------------- */
static BOOL handleSysColorChange(HWND hwnd, PBTNDATA p) {
   HPS hps;
   BOOL handled = FALSE;
   if (NULLHANDLE == (hps = WinGetPS(hwnd))) return FALSE;
   getBtnColors(hwnd, p, hps);
   WinReleasePS(hps);
   if (mStlrBtnOn() && mHasBorder(hwnd)) {
      handled = TRUE;
      CtrlUpdate(hwnd, FALSE);
   } /* endif */
   return handled;
}


/* --------------------------------------------------------------------------
 Lock window update, call the system default button window procedure,
 unlock the window update and paint the button.
- Parameters -------------------------------------------------------------
 HWND hwnd  : button window handle.
 ULONG msg  : PM message to be executed by the system button procedure.
 MPARAM mp1 : first message parameter.
 MPARAM mp2 : second message parameter.
 PBTNDATA p : button data.
- Return value -----------------------------------------------------------
 MRESULT : value returned by the default button procedure.
-------------------------------------------------------------------------- */
static MRESULT overrideBtnProc(HWND hwnd, ULONG msg,
                               MPARAM mp1, MPARAM mp2, PBTNDATA p) {
   MRESULT mr;
   HPS hps;
   ULONG flShowing, flStyle;
   PBTNDRAW pbd;

   // if the button style is not compatible with the enhancements restore
   // the system default button procedure
   if ((flStyle = WinStyle(hwnd)) & (BS_NOBORDER | BS_USERBUTTON)) {
      resetDefBtnWndProc(hwnd, p);
      return g.cd.btn.pfnwp(hwnd, msg, mp1, mp2);
   } /* endif */

   if (FALSE != (flShowing = WinIsWindowShowing(hwnd)))
      WinEnableWindowUpdate(hwnd, FALSE);
   mr = g.cd.btn.pfnwp(hwnd, msg, mp1, mp2);
   if (flShowing) {
      WinEnableWindowUpdate(hwnd, TRUE);
      if (NULL != (pbd = memLkAlloc(sizeof(BTNDRAW)))) {
         if (NULLHANDLE != (pbd->hps = WinGetPS(hwnd))) {
            pbd->flStyle = flStyle;
            paintBtn(hwnd, p, pbd);
            WinReleasePS(pbd->hps);
         } /* endif */
         memLkFree(pbd);
      } /* endif */
   } /* endif */
   return mr;
}


/* --------------------------------------------------------------------------
 WM_PAINT message handler. If for any reason cannot paint the button return
 FALSE and allows the button to be painted by the default procedure.
- Parameters -------------------------------------------------------------
 HWND hwnd  : button window handle.
 PBTNDATA p : button data
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (button succesfully/unsuccesfully painted)
-------------------------------------------------------------------------- */
static BOOL handlePaintRequest(HWND hwnd, PBTNDATA p) {
   PBTNDRAW pbd;
   ULONG flStyle;
   if ((flStyle = WinStyle(hwnd)) & (BS_NOBORDER | BS_USERBUTTON)) {
      resetDefBtnWndProc(hwnd, p);
      return FALSE;
   } /* endif */
   if (NULL != (pbd = memLkAlloc(sizeof(BTNDRAW)))) {
      if (NULLHANDLE != (pbd->hps = WinBeginPaint(hwnd, NULLHANDLE, NULL))) {
         pbd->flStyle = flStyle;
         paintBtn(hwnd, p, pbd);
         WinEndPaint(pbd->hps);
      } /* endif */
      memLkFree(pbd);
      return TRUE;
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Paint the button.
- Parameters -------------------------------------------------------------
 HWND hwnd    : button window handle.
 PBTNDATA p   : button data
 PBTNDRAW pbd : data used to paint the button.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID paintBtn(HWND hwnd, PBTNDATA p, PBTNDRAW pbd) {
   // initialization -----------------------------------------------------
   WinQueryWindowRect(hwnd, &pbd->rd);
   pbd->size.cx = pbd->rd.xRight;
   pbd->size.cy = pbd->rd.yTop;
   // if an image must be displayed get the image data
   if (pbd->flStyle & BS_IMAGE) {
      memset(pbd, 0, sizeof(WNDPARAMS) + sizeof(BTNCDATA));
      pbd->wp.fsStatus = WPM_CTLDATA;
      pbd->wp.cbCtlData = sizeof(BTNCDATA);
      pbd->wp.pCtlData = &pbd->bcd;
      if (!mGetBtnWndParms(hwnd, &pbd->wp)) return;
   // otherwise just get the hilite state
   } else {
      pbd->bcd.fsHiliteState = mBtnHiliteState(hwnd);
   } /* endif */
   GpiCreateLogColorTable(pbd->hps, 0, LCOLF_RGB, 0, 0, NULL);
   // background ---------------------------------------------------------
   // initialize the size of the background rectangle
   if ((pbd->flStyle & BS_IMAGE) && !(pbd->flStyle & BS_DEFAULT)) {
      mSetBtnBkgndRect(&pbd->rd, &pbd->size, -2 - o.ui.btn.border,
                        -2 - o.ui.btn.border);
   } else {
      mSetBtnBkgndRect(&pbd->rd, &pbd->size, -3 - o.ui.btn.border,
                         -3 - o.ui.btn.border);
   } /* endif */
   // solid color button --------------
   if (o.ui.btn.solid) {
      WinFillRect(pbd->hps, &pbd->rd, p->lbk.l);
   // image background ----------------
   } else {
      // if a non-default background color is used create and paint a temp bmp
      if ((p->lbk.l != o.ui.btn.color) && !o.ui.btn.overPP && o.ui.btn.bh.cbFix) {
         drawTempBitmap(p, pbd);
      } else {
         WinDrawBitmap(pbd->hps, o.ui.btn.bmpp.hbmp, NULL, (PPOINTL)&pbd->rd,
                       0, 0, DBM_STRETCH);
      } /* endif */
   } /* endif */
   // foreground ---------------------------------------------------------
   setFgndClipRectangle(pbd->hps, &pbd->rd);
   // bitmap style
   if (pbd->flStyle & BS_BITMAP) {
      drawFgndBitmap(p, pbd);
   // miniicon style
   } else if (pbd->flStyle & BS_MINIICON) {
      drawFgndMiniIcon(p, pbd);
   // icon style
   } else if (pbd->flStyle & BS_ICON) {
      drawFgndIcon(p, pbd);
   // text style
   } else {
      if (p->cbText) drawBtnText(p, pbd, 0);
   } /* endif */
   mResetClipRect(pbd->hps);
   // borders ------------------------------------------------------------
   drawBorder(hwnd, p, pbd);
   // draw a halftoned pattern to show the WS_DISABLED style
   if ((pbd->flStyle & WS_DISABLED) && !o.ui.btn.dis3D) {
      mSetBtnBorderRect(&pbd->size, &pbd->rd, 0, 0, -1, -1);
      drawDisabledRect(pbd->hps, &pbd->rd, p->ldis);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Create a temporary bitmap with a non-default dominant color.
 The bitmap is immediately painted and then destroyed.
- Parameters -------------------------------------------------------------
 PBTNDATA p   : button data.
 PBTNDRAW pbd : data used to paint the button.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID drawTempBitmap(PBTNDATA p, PBTNDRAW pbd) {
   HBITMAP hbmp;
   memcpy(&pbd->bh, &o.ui.btn.bh, sizeof(BMPHDR));
   stlrSetBtnPalette(&pbd->bh, &p->lbk);
   hbmp = m_createBmp(pbd->hps, &pbd->bh, o.ui.btn.ab);
   WinDrawBitmap(pbd->hps, hbmp, NULL, (PPOINTL)&pbd->rd, 0, 0, DBM_STRETCH);
   GpiDeleteBitmap(hbmp);
}


/* --------------------------------------------------------------------------
 Modify the button palette according to the difference between the light
 grey color and the value stored in 'pClrBkgnd'.
- Parameters -------------------------------------------------------------
 PBMPHDR pbh    : reduced bitmap header including RGB palette.
 PCLR pClrBkgnd : new background color.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID stlrSetBtnPalette(PBMPHDR pbh, PCLR pClrBkgnd) {
   INT i, max;
   LONG red, green, blue;
   // check if there is a palette
   for (i = 0, max = (1 << pbh->cBitCount); i < max; ++i) {
      red = pbh->argbColor[i].bRed + pClrBkgnd->rgb.red - 0xcc;
      green = pbh->argbColor[i].bGreen + pClrBkgnd->rgb.grn - 0xcc;
      blue = pbh->argbColor[i].bBlue + pClrBkgnd->rgb.blu - 0xcc;
      pbh->argbColor[i].bRed = m_limitVal(red, 0, 0xff);
      pbh->argbColor[i].bGreen = m_limitVal(green, 0, 0xff);
      pbh->argbColor[i].bBlue = m_limitVal(blue, 0, 0xff);
   } /* endfor */
}


/* --------------------------------------------------------------------------
 Set a clip rectangle within which the button foreground must be painted.
- Parameters -------------------------------------------------------------
 HPS hps   : presentation space handle.
 PRECTL pr : coordinates of the clipping rectangle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL setFgndClipRectangle(HPS hps, PRECTL pr) {
   POINTL aptl[3];
   aptl[0].x = pr->xLeft;
   aptl[0].y = pr->yTop - 1;
   aptl[1].x = pr->xRight - 1;
   aptl[1].y = aptl[0].y;
   aptl[2].x = aptl[1].x;
   aptl[2].y = pr->yBottom;
   return GpiBeginPath(hps, 1) &&
          GpiMove(hps, (PPOINTL)pr) &&
          GpiPolyLine(hps, 3, aptl) &&
          GpiEndPath(hps) &&
          GpiSetClipPath(hps, 1, SCP_AND);
}


/* --------------------------------------------------------------------------
 When the button has the BS_BITMAP style draw the button text (if present)
 and then the bitmap image.
- Parameters -------------------------------------------------------------
 PBTNDATA p   : button data.
 PBTNDRAW pbd : data used to paint the button.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID drawFgndBitmap(PBTNDATA p, PBTNDRAW pbd) {
   BMPINF bi;
   ULONG cyText;
   // get the image size
   if (pbd->bcd.hImage) {
      bi.cbFix = 16;
      GpiQueryBitmapInfoHeader(pbd->bcd.hImage, (PBITMAPINFOHEADER2)&bi);
   } else {
      bi.cx = bi.cy = 0;
   } /* endif */
   // draw the button text
   cyText = (pbd->flStyle & BS_TEXT)? drawBtnText(p, pbd, bi.cy + 2) : 0;
   // draw the button bitmap
   if (bi.cx && bi.cy) {
   // center the bitmap
      pbd->aptl[0].x = (pbd->size.cx - bi.cx) / 2 + pbd->bcd.fsHiliteState;
      pbd->aptl[0].y = cyText? pbd->rd.yTop + 2:
                           (pbd->size.cy - bi.cy) / 2 - pbd->bcd.fsHiliteState;
      // if the button is disabled draw the bitmap with DBM_HALFTONE flag
      if (pbd->flStyle & WS_DISABLED)
         WinDrawBitmap(pbd->hps, pbd->bcd.hImage, NULL, pbd->aptl,
                       p->ldis, 0, DBM_NORMAL | DBM_HALFTONE);
      else
         WinDrawBitmap(pbd->hps, pbd->bcd.hImage, NULL, pbd->aptl,
                       0, 0, DBM_NORMAL);
   } /* endif */
}


/* --------------------------------------------------------------------------
 When the button has the BS_MINIICON style draw the button text (if present)
 and then mini icon image.
- Parameters -------------------------------------------------------------
 PBTNDATA p   : button data.
 PBTNDRAW pbd : data used to paint the button.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID drawFgndMiniIcon(PBTNDATA p, PBTNDRAW pbd) {
   ULONG cyText;
   // draw the button text if present
   cyText = (pbd->flStyle & BS_TEXT)? drawBtnText(p, pbd, g.cyIco / 2 + 1): 0;
   if (pbd->bcd.hImage) {
      // calculate the coordinates of the bottom-left corner of the icon
      pbd->aptl[0].x = (pbd->size.cx - g.cyIco / 2) / 2 + pbd->bcd.fsHiliteState;
      pbd->aptl[0].y = cyText? pbd->rd.yTop + 1:
                    (pbd->size.cy - g.cyIco / 2) / 2 - pbd->bcd.fsHiliteState;
      if (pbd->flStyle & WS_DISABLED)
         WinDrawPointer(pbd->hps, pbd->aptl[0].x, pbd->aptl[0].y,
                        pbd->bcd.hImage, DP_MINI | DP_HALFTONED);
      else
         WinDrawPointer(pbd->hps, pbd->aptl[0].x, pbd->aptl[0].y,
                        pbd->bcd.hImage, DP_MINI);
   } /* endif */
}


/* --------------------------------------------------------------------------
 When the button has the BS_ICON style draw the button text (if present)
 and then mini icon image.
- Parameters -------------------------------------------------------------
 PBTNDATA p   : button data.
 PBTNDRAW pbd : data used to paint the button.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID drawFgndIcon(PBTNDATA p, PBTNDRAW pbd) {
   ULONG cyText;
   cyText = (pbd->flStyle & BS_TEXT)? drawBtnText(p, pbd, g.cyIco + 1): 0;
   if (pbd->bcd.hImage) {
      // calculate the coordinates of the bottom-left corner of the icon
      pbd->aptl[0].x = (pbd->size.cx - g.cyIco) / 2 + pbd->bcd.fsHiliteState;
      pbd->aptl[0].y = cyText? pbd->rd.yTop + 1:
                       (pbd->size.cy - g.cyIco) / 2 - pbd->bcd.fsHiliteState;
      if (pbd->flStyle & WS_DISABLED)
         WinDrawPointer(pbd->hps, pbd->aptl[0].x, pbd->aptl[0].y,
                        pbd->bcd.hImage, DP_NORMAL | DP_HALFTONED);
      else
         WinDrawPointer(pbd->hps, pbd->aptl[0].x, pbd->aptl[0].y,
                        pbd->bcd.hImage, DP_NORMAL);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Draw the button text.
- Parameters -------------------------------------------------------------
 PBTNDATA p    : button data.
 PBTNDRAW pbd  : data used to paint the button.
 ULONG cyimg   : button bitmap or icon height (0 when there is no image).
- Return value -----------------------------------------------------------
 ULONG : text height in pixels.
-------------------------------------------------------------------------- */
static ULONG drawBtnText(PBTNDATA p, PBTNDRAW pbd, ULONG cyimg) {
   LONG clrtxt;
   ULONG cyText;
   // calculate the text height
   GpiQueryTextBox(pbd->hps, 1, " ", 2, pbd->aptl);
   cyText = pbd->aptl[0].y - pbd->aptl[1].y;
   // calculate the coordinates of the text rectangle
   if (cyimg) {
      pbd->rd.yBottom = (((pbd->size.cy - 1) > (cyimg + cyText))?
                         (pbd->size.cy - 1 - cyimg - cyText) / 2: 0);
      pbd->rd.yTop = pbd->rd.yBottom + cyText;
   } /* endif */
   if (pbd->bcd.fsHiliteState) RectShift(&pbd->rd, 1, -1);
   if (pbd->flStyle & WS_DISABLED) {
      // 3D style text: use light/dark border colors
      if (o.ui.btn.dis3D) {
         RectShift(&pbd->rd, 1, -1);
         WinDrawText(pbd->hps, p->cbText, p->pszText, &pbd->rd, p->llite, 0,
                     DT_CENTER | DT_VCENTER | DT_MNEMONIC);
         RectShift(&pbd->rd, -1, 1);
         clrtxt = p->lshadow;
      } else {
         clrtxt = p->ldisf;
      } /* endif */
   } else {
      clrtxt = o.ui.btn.overPP? 0: p->lfgnd;
   } /* endif */
   WinDrawText(pbd->hps, p->cbText, p->pszText, &pbd->rd, clrtxt, 0,
               DT_CENTER | DT_VCENTER | DT_MNEMONIC);
   if (pbd->bcd.fsHiliteState) RectShift(&pbd->rd, -1, 1);
   return cyText;
}


/* --------------------------------------------------------------------------
 Draw the button border.
- Parameters -------------------------------------------------------------
 PBTNDATA p   : button data.
 PBTNDRAW pbd : data used to paint the button.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID drawBorder(HWND hwnd, PBTNDATA p, PBTNDRAW pbd) {
   LONG lclr;
   // set the border rectangle
   if ((pbd->flStyle & BS_IMAGE) && !(pbd->flStyle & BS_DEFAULT)) {
      mSetBtnBorderRect(&pbd->rd, &pbd->size, 0, 0, -1, -1);
   } else {
      mSetBtnBorderRect(&pbd->rd, &pbd->size, 1, 1, -3, -3);
   } /* endif */
   // if the button is in pressed state
   if (pbd->bcd.fsHiliteState) {
      // draw the outer border first
      if (o.ui.btn.flat)                  // flat style button
         draw3Dborder(pbd->hps, &pbd->rd, p->lshadow, p->llite, 1);
      else                                // non-flat style button
         draw3Dborder(pbd->hps, &pbd->rd, 0, p->lshadow, 1);
      // draw the inner border
      draw3Dborder(pbd->hps, &pbd->rd, p->lshadow, p->llite,
                   1 + o.ui.btn.border);
   // normal state button
   } else {
      // outer border
      if (o.ui.btn.flat)                  // flat style button
         draw3Dborder(pbd->hps, &pbd->rd, p->lshadow, p->llite, 1);
      else                                // non-flat style button
         draw3Dborder(pbd->hps, &pbd->rd, p->lshadow, 0, 1);
      // draw the inner border
      draw3Dborder(pbd->hps, &pbd->rd, p->llite, p->lshadow,
                   1 + o.ui.btn.border);
   } /* endif */
   mSetBtnBorderRect(&pbd->rd, &pbd->size, 0, 0, -1, -1);
   // the button has the BS_DEFAULT style (draw a 3D or a thick black frame)
   if (pbd->flStyle & BS_DEFAULT) {
      if (o.ui.btn.def3D) {               // 3D style
         draw3Dborder(pbd->hps, &pbd->rd, 0, 0xffffff, 1);
         draw3Dborder(pbd->hps, &pbd->rd, p->lbk.l, p->lbk.l, 1);
      } else {                            // normal (thick black) style
         draw3Dborder(pbd->hps, &pbd->rd, p->ldef, p->ldef, 2);
      } /* endif */
   // the BS_DEFAULT emphasis is painted outside the button rectangle
   // so when the button does not have the BS_DEFAULT emphasis a frame
   // with the color of the button parent background must be painted
   // around the button to erase a possible previous BS_DEFAULT emphasis
   } else if (!(pbd->flStyle & BS_IMAGE)) {
      lclr = mParentBkgndColor(hwnd, pbd->hps);
      draw3Dborder(pbd->hps, &pbd->rd, lclr, lclr, 1);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Draw an halftoned rectangle using the color 'color'.
- Parameters -------------------------------------------------------------
 HPS hps     : presentation space handle.
 PRECTL prcl : rectangle to draw.
 LONG color  : halftone pattern color.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL drawDisabledRect(HPS hps, PRECTL prcl, LONG color) {
   RECTL rcl = *prcl;
   rcl.xRight--;
   rcl.yTop--;
   return GpiMove(hps, (PPOINTL)&rcl) &&
          GpiSetPattern(hps, (((prcl->xLeft % 2) != (prcl->yBottom % 2))?
                                          PATSYM_DENSE5: PATSYM_HALFTONE)) &&
          GpiSetColor(hps, color) &&
          GpiBox(hps, DRO_FILL, (PPOINTL)&rcl + 1, 0L, 0L);
}


/* --------------------------------------------------------------------------
 Get the colors to be used to paint the button.
- Parameters -------------------------------------------------------------
 HWND hwnd  : button window handle.
 PBTNDATA p : button data
 HPS hps    : presentation space handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID getBtnColors(HWND hwnd, PBTNDATA p, HPS hps) {
   p->lbk.l = mBkgndColor(hwnd, hps);
   p->lfgnd = mFgndColor(hwnd, hps);
   p->ldis = mDisabledBkgndColor(hwnd, hps);
   p->ldisf = mDisabledFgndColor(hwnd, hps);
   p->ldef = mDefaultBorderColor(hwnd, hps);
   p->llite = mHiliteBorderColor(hwnd, hps);
   p->lshadow = mDarkBorderColor(hwnd, hps);
}


/* --------------------------------------------------------------------------
 Get a color value checking the presentation parameter and the system
 colors.
- Parameters -------------------------------------------------------------
 HWND hwnd : button window handle.
 HPS hps   : presentation space handle.
 ULONG ulid1 : presentation parameter index of pure RGB color.
 ULONG ulid2 : presentation parameter index of color index.
 LONG ldef   : default color as SYSCLR_* or RGB value.
- Return value -----------------------------------------------------------
 LONG : color as 24 bit RGB value.
-------------------------------------------------------------------------- */
static LONG getCtlClr(HWND hwnd, HPS hps, ULONG ulid1, ULONG ulid2, LONG ldef) {
   LONG lclr;
   if (WinQueryPresParam(hwnd, ulid1, ulid2, NULL, 4UL, (PVOID)&lclr,
                         QPF_NOINHERIT | QPF_PURERGBCOLOR |
                         QPF_PURERGBCOLOR | QPF_ID2COLORINDEX))
      return lclr;
   if ((ldef >= SYSCLR_SHADOWHILITEBGND) && (ldef <= SYSCLR_HELPHILITE))
      return WinQuerySysColor(HWND_DESKTOP, ldef, 0L);
   return ldef;
}


/* --------------------------------------------------------------------------
 Use a trick (set an input hook and post a faked WM_PRESPARAMCHANGED message)
 to subclass the warp center procedure only after the window creation
 completed succesfully.
- Parameters -------------------------------------------------------------
 HWND hwnd : WarpCenter window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID deferWarpCenterSubclassing(HWND hwnd) {
   g.hwnd.warpCenter = hwnd;
   WinSetHook(WinQueryAnchorBlock(hwnd), HMQ_CURRENT,
              HK_INPUT, (PFN)hookWcProc, NULLHANDLE);
   WinPostMsg(hwnd, WM_PRESPARAMCHANGED,
              (MPARAM)0xffffffff, (MPARAM)0xffffffff);
}


/* --------------------------------------------------------------------------
 This is the input hook set to defer WarpCenter subclassing (see the previous
 procedure).
- Parameters -------------------------------------------------------------
 Ordinary input hook parameters.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (pass/do not pass the message to the next hook).
-------------------------------------------------------------------------- */
static BOOL EXPENTRY hookWcProc(HAB hab, PQMSG pqmsg, USHORT fs) {
   if ((pqmsg->hwnd == g.hwnd.warpCenter)
       &&
       (pqmsg->msg == WM_PRESPARAMCHANGED)
       &&
       ((ULONG)pqmsg->mp1 == 0xffffffff)
       &&
       ((ULONG)pqmsg->mp2 == 0xffffffff)) {
dbgPrintf1("*** warp center subclassed !!!\n");

      g.pfn.warpCenter = WinSubclassWindow(pqmsg->hwnd, newWcProc);
      WinReleaseHook(WinQueryAnchorBlock(pqmsg->hwnd), HMQ_CURRENT,
                     HK_INPUT, (PFN)hookWcProc, NULLHANDLE);
      return TRUE;
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Subclassed WarpCenter procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure messages.
- Return value -----------------------------------------------------------
 MRESULT
-------------------------------------------------------------------------- */
static MRESULT EXPENTRY newWcProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   // left mouse button pressed on the shutdown icon
   // query the window rectangle and store the WarpCenter height in g.is.cywc
   if (msg == WM_BUTTON1DOWN) {
      RECTL r;
      WinQueryWindowRect(hwnd, &r);
      if (mIsMouseOverShutDownIconX(r.yTop, mp1))
         g.is.cywc = r.yTop;
   // shutdown via the WarpCenter icon
   } else if (msg == WM_BUTTON1UP) {
      if (mIsMouseOverShutDownIcon(g.is.cywc, mp1)) {
         g.is.cywc = 0;
dbgPrintf1("*** shutdown from warp center\n");
         if (o.sd.gen.on) {
             m_postWorkerMsg(STLRM_SHUTDOWNREQ, g.worker.hwnd, 0);
         } else {
            HOBJECT hoDesk;
            CHAR    szSetup[32];
            if (( hoDesk = WinQueryObject("<WP_DESKTOP>")) != NULLHANDLE ) {
                sprintf( szSetup, "MENUITEMSELECTED=%u", WPMENUID_SHUTDOWN );
                WinSetObjectData( hoDesk, szSetup );
            }
         } /* endif */
         mp1 = MPVOID;
      } /* endif */
   // execute a WarpCenter command via mouse event emulation
   } else if (msg == WM_QUERYFRAMEINFO) {
      if (mp1 == STLRM_WCCMD) warpCenterCmd((ULONG)mp2);
      return MRFALSE;
   // context menu event on the WarpCenter clock
   } else if (msg == WM_CONTEXTMENU) {
      if (mIsMouseOverClock(mp1)) {
         // if fails to open the eCS clock object tries to open the default one
         if (
            !m_setWpsObject(OBJID_ECS11CLOCK, OBJ_OPENDEF)
            &&
            !m_setWpsObject(OBJID_OS2CLOCK, OBJ_OPENPROPERTY)
            )
            m_setWpsObject(OBJID_ECS10CLOCK, OBJ_OPENDEF);
         return MRTRUE;
      } /* endif */
   } /* endif */
   return (MRESULT)g.pfn.warpCenter(hwnd, msg, mp1, mp2);
}


/* --------------------------------------------------------------------------
 Execute a WarpCenter command by emulating a mouse event on the WarpCenter.
- Parameters -------------------------------------------------------------
 ULONG fl : WarpCenter command to execute.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID warpCenterCmd(ULONG fl) {
   RECTL r;
   ULONG ul1;
   WinQueryWindowRect(g.hwnd.warpCenter, &r);
   switch (fl) {
      // desktop menu
      case IWKEY_WCDESKMENU:
         ul1 = 0x00020002;
         break;
      // windows list
      case IWKEY_WCWINLIST:
         ul1 = r.yTop > 22? 0x0002003c: 0x00020034;
         break;
      // processes list
      case IWKEY_WCPROCLIST:
         openWCProcessList(r.yTop);
         return;
      // tray list
      case IWKEY_WCTRAYLIST:
         ul1 = r.yTop > 22? 0x0002017d: 0x00020141;
         break;
      // next tray
      case IWKEY_WCNEXTTRAY:
         ul1 = 0x00020000 + r.xRight - (r.yTop > 22? 122: 118) -
               (r.xRight > 1280? 14: 0);
         break;
      case IWKEY_WCINFOMENU:
         ul1 = 0x00020000 + r.xRight - 106 - (r.xRight > 1280? 14: 0);
         break;
      default:
         return;
   } /* endswitch */
   mWarpCenterSendMouseClick(ul1, 0);
   // in case of tray switch give the focus back to the previous window
   if (fl == IWKEY_WCNEXTTRAY) {
      WinSetFocus(g.hwnd.desktop, g.hwnd.wkey);
      g.hwnd.wkey = NULLHANDLE;
   } /* endif */
}


/* --------------------------------------------------------------------------
 Open the WarpCenter process list.
- Parameters -------------------------------------------------------------
 ULONG cyWarpCenter : WarpCenter window height.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID openWCProcessList(ULONG cyWarpCenter) {
   CHAR buf[256];
   ULONG ul1;
   ul1 = cyWarpCenter > 22? 0x0002003c: 0x00020034;
   WinSetKeyboardStateTable(g.hwnd.desktop, buf, FALSE);
   buf[VK_CTRL] |= 0x80;
   WinSetKeyboardStateTable(g.hwnd.desktop, buf, TRUE);
   mWarpCenterSendMouseClick(ul1, 0x00100000);
   WinSetKeyboardStateTable(g.hwnd.desktop, buf, FALSE);
   buf[VK_CTRL] = 0;
   WinSetKeyboardStateTable(g.hwnd.desktop, buf, TRUE);
}
