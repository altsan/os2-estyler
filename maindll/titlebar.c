/* --------------------------------------------------------------------------
 titlebar.c: enhanced WC_TITLEBAR window procedure.
 2004/05/08 - Alessandro Cantatore - Italy

 naming conventions:
 p            is the address of the TBDATA structure
 m*           (followed by upper case) is the name of the macros defined in
              .\titlebar.h.
 m_*          is the name of the global macros defined in ..\headers\macros.h
-------------------------------------------------------------------------- */

#pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"
#include "stlrTitlebar.h"
#include "stlrGraphics.h"

#ifndef min
#define min(a, b)       (((a) < (b))? (a) : (b))
#endif

// prototypes ---------------------------------------------------------------
static BOOL isWinOS2Window(HWND hwnd, PTBDATA p);
ULONG copyStripReturns(PSZ target, PSZ source);
VOID drawTitlebar(HPS hps, PTBDATA p);

// global variables ---------------------------------------------------------
extern PID pid;

/* --------------------------------------------------------------------------
 Enhanced WC_TITLEBAR procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT
-------------------------------------------------------------------------- */
MRESULT EXPENTRY stlrTitleBarProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PTBDATA p;
//   return g.cd.tbar.pfnwp(hwnd, msg, mp1, mp2);
   switch (msg) {
      // hilite state change ---------------------------------------------
      case TBM_SETHILITE:
//**/     break;
         if (pid && (NULL != (p = mTitlebarData(hwnd)))) {
            p->hilited = (mp1 != NULL);
            if (mTitlebarEnhancementsOn()) {
               if (mMustPaintTitlebar(p)) {
                  HPS hps;
                  if (WinIsWindowShowing(hwnd)
                      &&
                      (NULLHANDLE != (hps = WinGetPS(hwnd)))) {
                     drawTitlebar(hps, p);
                     WinReleasePS(hps);
                  } /* endif */
                  return MRTRUE;
               } else {
                  WinInvalidateRect(hwnd, NULL, FALSE);
               } /* endif */
            } /* endif */
         } /* endif */
         break;
      // titlebar paint --------------------------------------------------
      case WM_PAINT:
//**/     break;
         if (mMustProcessWMpaint(p)) {
            HPS hps;
            if (NULLHANDLE != (hps = WinBeginPaint(hwnd, NULLHANDLE, NULL))) {
               drawTitlebar(hps, p);
               WinEndPaint(hps);
            } /* endif */
            return (MRESULT)FALSE;
         } /* endif */
         break;
      // presentation parameters changed ---------------------------------
      case WM_PRESPARAMCHANGED:
         if (pid && (NULL != (p = mTitlebarData(hwnd)))) {
            ULONG ul;
            switch ((LONG)mp1) {
               case PP_FONTNAMESIZE:
                  p->cyfont = 0;
                  if (o.ui.disabled) break;
                  if (WinIsWindowShowing(hwnd))
                     WinInvalidateRect(hwnd, NULL, FALSE);
                  return (MRESULT)FALSE;
               case PP_ACTIVETEXTBGNDCOLOR:
               case PP_ACTIVETEXTBGNDCOLORINDEX:
               case PP_ACTIVECOLOR:
               case PP_ACTIVECOLORINDEX:
                  p->actpp = mIsPresParmSet(hwnd, mp1, ul);
                  break;
               case PP_INACTIVETEXTBGNDCOLOR:
               case PP_INACTIVETEXTBGNDCOLORINDEX:
               case PP_INACTIVECOLOR:
               case PP_INACTIVECOLORINDEX:
                  p->ictpp = mIsPresParmSet(hwnd, mp1, ul);
                  break;
            } /* endswitch */
         } /* endif */
         break;
      // the window text is being changed --------------------------------
   case WM_SETWINDOWPARAMS:
         if (pid && (NULL != (p = mTitlebarData(hwnd)))) {
            // if this is a WinOS2 window unsubclass it
            if (!p->chkclass && isWinOS2Window(hwnd, p))
               break;
            if (mp1 && (((PWNDPARAMS)(mp1))->fsStatus & WPM_TEXT)) {
               p->cyfont = 0;
               heapLockPM(hwnd);
               memFree(p->pszText);
               if (((PWNDPARAMS)(mp1))->pszText) {
                  p->cbText = strlen(((PWNDPARAMS)(mp1))->pszText);
                  if (NULL != (p->pszText = memAlloc(p->cbText + 1))) {
                     p->cbText = copyStripReturns(p->pszText,
                                                  ((PWNDPARAMS)(mp1))->pszText);
                  } else {
                     p->cbText = 0;
                  } /* endif */
               } else {
                  p->pszText = NULL;
                  p->cbText = 0;
               } /* endif */
               heapUnlock();
            } /* endif */
         } /* endif */
         break;
      // window size/position changed ------------------------------------
      case WM_WINDOWPOSCHANGED:
         if ((((PSWP)mp1)->fl & SWP_SIZE)
             &&
             pid
             &&
             (NULL != (p = mTitlebarData(hwnd)))) {
            p->size.cx = ((PSWP)mp1)->cx;
            p->size.cy = ((PSWP)mp1)->cy;
         } /* endif */
         break;
      // titlebar creation -----------------------------------------------
      case WM_CREATE:
         if ((m_creatParm(mp2)->id != FID_TITLEBAR)
             ||
             !pid
             ||
             (NULL == (p = (PTBDATA)memLkAlloc(sizeof(TBDATA)))))
            break;
         memset(p, 0, sizeof(TBDATA));
         // store the TBDATA structure in the window words
         WinSetWindowPtr(hwnd, g.cd.tbar.cbData, (PVOID)p);
         // if not PROTSHELL process do not check (later) FID_CLIENT class
         if (g.pid.protshell != pid) p->chkclass = 1;
         // titlebar text
         if ((m_creatParm(mp2)->pszText)
             &&
             (NULL
                !=
             (p->pszText = memLkAlloc(strlen(m_creatParm(mp2)->pszText) + 1))))
            p->cbText = copyStripReturns(p->pszText, m_creatParm(mp2)->pszText);
         p->size.cx = m_creatParm(mp2)->cx; // titlebar size
         p->size.cy = m_creatParm(mp2)->cy;
         // update the titlebar font
         if ((o.ui.tb.overridePP || o.ui.tb.overrideFont) && o.ui.tb.on && !o.ui.disabled)
            m_setFont(hwnd, o.ui.tb.achFont);
         break;
      case WM_DESTROY:
         if (pid && (NULL != (p = mTitlebarData(hwnd)))) {
            WinSetWindowPtr(hwnd, g.cd.tbar.cbData, NULL);
            heapLockPM(hwnd);
            memFree(p->pszText);
            memFree(p);
            heapUnlock();
         } /* endif */
         break;
   } /* endswitch */
   return g.cd.tbar.pfnwp(hwnd, msg, mp1, mp2);
}


/* --------------------------------------------------------------------------
 Return TRUE if the titlebar belongs to a WINOS2 window.
- Parameters -------------------------------------------------------------
 HWND hwnd : titlebar window handle
 PTBDATA p : address of the TBDATA structure.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (a winOS2 window / real titlebar)
-------------------------------------------------------------------------- */
static BOOL isWinOS2Window(HWND hwnd, PTBDATA p) {
   char buf[16];
   p->chkclass = 1;
   mGetClientWindowClass(hwnd, buf);
   if (!memcmp(buf, "SeamlessClass", 14)) {
      if (p->pszText) memLkFree(p->pszText);
      memLkFree(p);
      WinSetWindowPtr(hwnd, g.cd.tbar.cbData, NULL);
      return TRUE;
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Copy the 'source' string to the 'target' string converting sequences of
 '\r' and-or '\n' characters to a single blank space.
- Parameters -------------------------------------------------------------
 PSZ target : buffer where to write the converted text.
 PSZ source : text to be converted and copied to 'target'.
- Return value -----------------------------------------------------------
 ULONG length of the resulting text string.
-------------------------------------------------------------------------- */
ULONG copyStripReturns(PSZ target, PSZ source) {
   PSZ pstart = target;
   while (*source) {
      if (*source == '\r' || *source == '\n') {
         while(*source == '\r' || *source == '\n') ++source;
         *target++ = ' ';
      } else {
         *target++ = *source++;
      } /* endif */
   } /* endwhile */
   *target = 0;
   return (ULONG)(target - pstart);
}


/* --------------------------------------------------------------------------
 Paint the titlebar.
- Parameters -------------------------------------------------------------
 HPS hps   : presentation space handle.
 PTBDATA p : address of the TBDATA structure.
- Return value -----------------------------------------------------------
 VOID:
-------------------------------------------------------------------------- */
VOID drawTitlebar(HPS hps, PTBDATA p) {
   PTBARHILITE ptbo = p->hilited? &o.ui.tb.a: &o.ui.tb.i;
   POINTL aptl[4];
   INT i;
   GpiCreateLogColorTable(hps, 0, LCOLF_RGB, 0, 0, NULL);
   switch (ptbo->bkgnd) {
      // gradient background ---------------------------------------------
      case TBARBKGNDSHADE:
         if (ptbo->fl & TBO_VERTSHADE) {             // vertical gradient
            mScaleBmpToRect(hps, ptbo->bmpp.hbmp,
                            aptl, 0, 0, p->size.cx, p->size.cy);
         } else {
            RectSet(aptl, 0, 0, p->size.cx, SHADEH_CY);
            for (i = 0; i < p->size.cy; i += 2) {
               mScaleBitmap(hps, ptbo->bmpp.hbmp, NULL, aptl);
               ((PRECTL)aptl)->yBottom += SHADEH_CY;
               ((PRECTL)aptl)->yTop += SHADEH_CY;
            } /* endfor */
         } /* endif */
         break;
      // bitmap background -----------------------------------------------
      case TBARBKGNDBMP:
         // scaled bitmap
         if (ptbo->fl & TBO_STRETCHBMP) {
#if 1
            // [ALT 2022-08-30] If bitmap is larger than titlebar, crop it
            mScaleBmpRectAt(hps, ptbo->bmpp.hbmp, aptl,
                            0, 0, min(p->size.cx, ptbo->bmpp.cx),
                                  min(p->size.cy, ptbo->bmpp.cy),
                            0, 0, p->size.cx, p->size.cy);
#else
            mScaleBmpToRect(hps, ptbo->bmpp.hbmp,
                            aptl, 0, 0, p->size.cx, p->size.cy);
#endif
         // tiled bitmap
         } else {
#if 1
            // [ALT 2022-08-31] Also tile vertically if necessary
            mSetBmpSrcRectDestRect( aptl, 0, 0,
                                    min(p->size.cx, ptbo->bmpp.cx), min(p->size.cy, ptbo->bmpp.cy),
                                    0, 0, min(p->size.cx, ptbo->bmpp.cx), p->size.cy );
            while (aptl[2].x <= p->size.cx) {
               mScaleBitmap(hps, ptbo->bmpp.hbmp, aptl, aptl + 2);
               aptl[2].x += ptbo->bmpp.cx;
               aptl[3].x = min(p->size.cx, aptl[2].x + ptbo->bmpp.cx);
               aptl[1].x = min(p->size.cx - aptl[2].x, ptbo->bmpp.cx);
            } // endwhile
#else
            mSetBmpSrcRectDestPos(aptl, 0, 0, p->size.cx, p->size.cy, 0, 0);
            while (aptl[2].x <= p->size.cx) {
               mDrawBitmap(hps, ptbo->bmpp.hbmp, aptl, aptl + 2);
               aptl[2].x += ptbo->bmpp.cx;
               aptl[1].x = p->size.cx - aptl[2].x;
            } /* endwhile */
#endif
         } /* endif */
         break;
      // solid color background ------------------------------------------
      default:
         PointSet(aptl, p->size.cx - 1, p->size.cy - 1);
         GpiSetColor(hps, ptbo->clrLeftTop);
         GpiBox(hps, DRO_OUTLINEFILL, aptl, 0L, 0L);
         break;
   } /* endswitch */
   // titlebar text ------------------------------------------------------
   if (!p->cyfont) { // the text has to be measured again
      GpiQueryTextBox(hps, p->cbText, p->pszText, 3, aptl);
      p->yfont = - aptl[1].y;
      p->cyfont = aptl[2].y - aptl[1].y;
      p->cxfont = aptl[2].x - aptl[1].x;
   } /* endif */
   // if the text is wider than the window it is anyway left aligned
   p->xshift = (p->cxfont <= p->size.cx)? 8: 0;
   aptl[0].x = (o.ui.tb.center && p->xshift)?
               (p->size.cx - p->cxfont) / 2 + 1: p->xshift + 3;
   aptl[0].y = (p->size.cy + 1 - p->cyfont) / 2 + p->yfont;
   aptl[1].x = 0;
   aptl[1].y = 0;
   aptl[2].x = p->size.cx - 1;
   aptl[2].y = p->size.cy - 1;
   if (ptbo->fl & TBO_TEXTSHADOW) {      // draw a shadow behind the text
      GpiSetColor(hps, ptbo->clrBgTxt);
      GpiCharStringPosAt(hps, aptl, (PRECTL)(aptl + 1), CHS_CLIP,
                         p->cbText, p->pszText, NULL);
   } // end if
   aptl[0].x--;
   aptl[0].y++;
   GpiSetColor(hps, ptbo->clrFgTxt);
   GpiCharStringPosAt(hps, aptl, (PRECTL)(aptl + 1), CHS_CLIP,
                      p->cbText, p->pszText, NULL);
   // titlebar border ----------------------------------------------------
   if (ptbo->fl & TBO_BORDER)
      draw3Dborder(hps, (PRECTL)(aptl+1), ptbo->clrTopLeftBorder,
                   ptbo->clrRightBottomBorder, 1);
}
