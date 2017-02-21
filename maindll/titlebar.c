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

#include <stdlib.h>
#include <string.h>

// includes -----------------------------------------------------------------
#include "main.h"
#include "stlrTitlebar.h"
#include "stlrGraphics.h"
#include "stlrExport.h"
#include "stlrCommon.h"

// prototypes ---------------------------------------------------------------
static BOOL isWinOS2Window(HWND hwnd, PTBDATA p);
ULONG copyStripReturns(PSZ target, PSZ source);
static VOID drawTitlebar(HWND hwnd, HPS hps, PTBDATA p);

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
                     drawTitlebar(hwnd, hps, p);
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
               drawTitlebar(hwnd, hps, p);
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
         if (o.ui.tb.on && !o.ui.disabled) m_setFont(hwnd, o.ui.tb.achFont);
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
static VOID drawTitlebar(HWND hwnd, HPS hps, PTBDATA p)
{
    PTBARHILITE ptbo = p->hilited ? &o.ui.tb.a: &o.ui.tb.i;
    POINTL aptl[4];
    INT i;
    cairo_surface_t *surface = NULL;
    cairo_t *cr = NULL;
    cairo_text_extents_t font_extents;
    char* dot, *size, *face;
    int     fx, fy;
    HINI hini = NULLHANDLE;
    CHAR achPath[CCHMAXPATH];
    CHAR    font[CCH_FONTDATA];
    cairo_pattern_t *pattern;
    int     flags;
    RECTL r;

    // query window rect
    WinQueryWindowRect( hwnd, &r);
    // adjust rect, cairo starts from 1,1
    r.xLeft = 1;
    r.yBottom = 1;

    // initialize cairo surface
    surface = cairo_os2_surface_create_for_window( hwnd, p->size.cx, p->size.cy);
    cr = cairo_create( surface);
    // disable aliasing for rendering primitives, but still used for fonts
    cairo_set_antialias( cr, CAIRO_ANTIALIAS_NONE);

    // draw background
    switch (ptbo->bkgnd) {
        // gradient background
    case TBARBKGNDSHADE:
        if (ptbo->fl & TBO_VERTSHADE) { // vertical gradient
            pattern = cairo_pattern_create_linear( 0.0, 0.0, 0, p->size.cy);
        } else {
            pattern = cairo_pattern_create_linear(0.0, 0.0, p->size.cx, 0);
        }
        cairo_pattern_add_color_stop_rgb( pattern, 0.1,
                                          RED(ptbo->clrLeftTop) / 255.0,
                                          GREEN(ptbo->clrLeftTop) / 255.0,
                                          BLUE(ptbo->clrLeftTop) / 255.0);
        if (ptbo->fl & TBO_SHADEDOUBLE) {
            cairo_pattern_add_color_stop_rgb( pattern, 0.5,
                                              RED(ptbo->clrRghtBtm) / 255.0,
                                              GREEN(ptbo->clrRghtBtm) / 255.0,
                                              BLUE(ptbo->clrRghtBtm) / 255.0);
            cairo_pattern_add_color_stop_rgb( pattern, 0.9,
                                              RED(ptbo->clrLeftTop) / 255.0,
                                              GREEN(ptbo->clrLeftTop) / 255.0,
                                              BLUE(ptbo->clrLeftTop) / 255.0);
        } else {
            cairo_pattern_add_color_stop_rgb( pattern, 0.9,
                                              RED(ptbo->clrRghtBtm) / 255.0,
                                              GREEN(ptbo->clrRghtBtm) / 255.0,
                                              BLUE(ptbo->clrRghtBtm) / 255.0);
        }
        cairo_rectangle( cr, 0.0, 0.0, p->size.cx, p->size.cy);
        cairo_set_source( cr, pattern);
        cairo_fill( cr);
        cairo_pattern_destroy( pattern);
        break;
        // bitmap background -----------------------------------------------
    case TBARBKGNDBMP:
        // get file name full path from ini file if image is NULL
        if (p->image == NULL) {
            // get file name full path from ini file
            if (NULLHANDLE == (hini = stlrOpenProfile()))
                break;
            if (PrfQueryProfileString(hini, SZPRO_OPTIONS,
                                      p->hilited ? SZPRO_BMPATBAR: SZPRO_BMPITBAR,
                                      NULL, achPath, CCHMAXPATHCOMP)) {
                p->image = cairo_image_surface_create_from_png( achPath);
                p->image_fx = cairo_image_surface_get_width( p->image);
                p->image_fy = cairo_image_surface_get_height( p->image);
            }
            PrfCloseProfile( hini);
        }
        // scaling is local to bitmap transformation only
        cairo_save( cr);
        if (ptbo->fl & TBO_STRETCHBMP) {
            // scaled bitmap
            cairo_scale( cr, (double) p->size.cx / p->image_fx,
                         (double) p->size.cy / p->image_fy);
            cairo_set_source_surface( cr, p->image, 0, 0);
            cairo_paint( cr);
        } else {
            // tiled bitmap
            pattern = cairo_pattern_create_for_surface( p->image);
            cairo_pattern_set_extend( pattern, CAIRO_EXTEND_REPEAT);
            cairo_set_source( cr, pattern);
            cairo_paint( cr);
            cairo_pattern_destroy( pattern);
        }
        cairo_restore( cr);
        break;
        // solid color background ------------------------------------------
    default:
        // fill background
        cairo_set_source_rgb( cr,
                              RED(ptbo->clrLeftTop) / 255.0,
                              GREEN(ptbo->clrLeftTop) / 255.0,
                              BLUE(ptbo->clrLeftTop) / 255.0);
        cairo_rectangle( cr, 0, 0, p->size.cx, p->size.cy);
        cairo_fill( cr);
        break;
    }

    // draw title
    flags = DT_VCENTER
            | (o.ui.tb.center ? DT_CENTER : 0)
            | (ptbo->fl & TBO_TEXTSHADOW ? DT_3D : 0);
    drawCairoTitle( cr, o.ui.tb.achFont, &r, ptbo->clrFgTxt,
                    ptbo->clrBgTxt, p->pszText, flags);

    // titlebar border
    if (ptbo->fl & TBO_BORDER)
        drawCairo3DBorder( cr, &r, ptbo->clrTopLeftBorder,
                           ptbo->clrRightBottomBorder, 1, 0);

    // render surface on entire window
    cairo_os2_surface_paint_window( surface, hps, NULL, 0);

    // destroy cairo surface
    if (cr)
        cairo_destroy (cr);
    if (surface)
        cairo_surface_destroy (surface);

}
