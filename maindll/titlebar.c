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

    // initialize cairo surface
    surface = cairo_os2_surface_create_for_window( hwnd, p->size.cx, p->size.cy);
    cr = cairo_create( surface);

#define RED(a) ((a >> 16) & 0xff)
#define GREEN(a) ((a >> 8) & 0xff)
#define BLUE(a) ((a) & 0xff)

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

    // select a default font
    cairo_select_font_face( cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                            CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size( cr, 14);

    // using strchr() on o.ui.tb.achFont crashes this code...
    memset( font, 0, sizeof(font));
    memcpy( font, o.ui.tb.achFont, sizeof(font));
    //dbgPrintf4( "drawTitlebar hwnd %x hps %x, font '%s'\n", hwnd, hps, font);

    // use font if defined
    if (strchr( font, '.')) {
        dot = strchr( font, '.');
        face = dot + 1;
        *dot = 0;
        //dbgPrintf4( "drawTitlebar hwnd %x hps %x, face '%s'\n", hwnd, hps, face);
        //dbgPrintf4( "drawTitlebar hwnd %x hps %x, size '%s'\n", hwnd, hps, font);
        //dbgPrintf4( "drawTitlebar hwnd %x hps %x, size %d\n", hwnd, hps, atoi(font));
        // set size of font
        cairo_set_font_size( cr, atoi(font) * 1.5);
        // set font name
        cairo_select_font_face( cr, face, CAIRO_FONT_SLANT_NORMAL,
                                CAIRO_FONT_WEIGHT_NORMAL);
    }

    // set title position
    fx = 8;
    fy = p->size.cy - 5;
    if (o.ui.tb.center) {
        cairo_text_extents(cr, p->pszText, &font_extents);
        // if the text is wider than the window it is anyway left aligned
        if (font_extents.width < p->size.cx)
            fx = (p->size.cx - font_extents.width) / 2;
    }

    // draw a shadow behind the text
    if (ptbo->fl & TBO_TEXTSHADOW) {
        cairo_set_source_rgb( cr, RED(ptbo->clrBgTxt) / 255.0,
                              GREEN(ptbo->clrBgTxt) / 255.0,
                              BLUE(ptbo->clrBgTxt) / 255.0);
        cairo_move_to( cr, fx + 1, fy + 1);
        cairo_show_text( cr, p->pszText);
    }

    // draw text
    cairo_set_source_rgb( cr, RED(ptbo->clrFgTxt) / 255.0,
                          GREEN(ptbo->clrFgTxt) / 255.0,
                          BLUE(ptbo->clrFgTxt) / 255.0);
    cairo_move_to( cr, fx, fy);
    cairo_show_text( cr, p->pszText);

    // titlebar border
    if (ptbo->fl & TBO_BORDER) {
        // top/left border
        cairo_set_line_width(cr, 1.0);
        cairo_set_source_rgb( cr, RED(ptbo->clrTopLeftBorder) / 255.0,
                              GREEN(ptbo->clrTopLeftBorder) / 255.0,
                              BLUE(ptbo->clrTopLeftBorder) / 255.0);
        cairo_move_to( cr, 0, p->size.cy);
        cairo_line_to( cr, 0, 0);
        cairo_line_to( cr, p->size.cx, 0);
        cairo_stroke (cr);
        // right/bottom
        cairo_set_source_rgb( cr, RED(ptbo->clrRightBottomBorder) / 255.0,
                              GREEN(ptbo->clrRightBottomBorder) / 255.0,
                              BLUE(ptbo->clrRightBottomBorder) / 255.0);
        cairo_move_to( cr, p->size.cx, 0);
        cairo_line_to( cr, p->size.cx, p->size.cy);
        cairo_line_to( cr, 0, p->size.cy);
        cairo_stroke (cr);
    }

    // render surface on entire window
    cairo_os2_surface_paint_window( surface, hps, NULL, 0);

    // destroy cairo surface
    if (cr)
        cairo_destroy (cr);
    if (surface)
        cairo_surface_destroy (surface);

}
