/* --------------------------------------------------------------------------
 button_cairo.c: button drawing using cairo graphics
 (c) 2017 - Yuri Dario
-------------------------------------------------------------------------- */

// includes -----------------------------------------------------------------
#include <stdlib.h>

#include "main.h"
#include "button.h"
#include "debug.h"
#include <selColor.h>

#include <stlrCommon.h>



/*
* Paint the button.
*/
VOID paintCairoButton( HWND hwnd,
                       PBTNDATA p, PBTNDRAW pbd,
                       PUIOPTIONS o) {
    RECTL r;
    SIZEL size;
    LONG color;
    cairo_surface_t *surface = NULL;
    cairo_t *cr = NULL;
    cairo_text_extents_t font_extents;
    CHAR    font[CCH_FONTDATA];
    char* dot, *face;
    int     fx, fy;
    int     flags;
    HINI hini = NULLHANDLE;
    CHAR achPath[CCHMAXPATH];
    cairo_pattern_t *pattern = NULL;
    cairo_surface_t *image = NULL;

    WinQueryWindowRect( hwnd, &r);
    size.cx = r.xRight + 1; // +1 for real size
    size.cy = r.yTop + 1;

    // initialize cairo surface
    surface = cairo_os2_surface_create_for_window( hwnd, size.cx, size.cy);
    cr = cairo_create( surface);
    // disable aliasing for rendering primitives, but still used for fonts
    cairo_set_antialias( cr, CAIRO_ANTIALIAS_NONE);

    // background ---------------------------------------------------------

    // solid color button --------------
    if (o->btn.solid) {
        // fill background
        cairo_set_source_rgb( cr,
                              RED(p->lbk.l) / 255.0,
                              GREEN(p->lbk.l) / 255.0,
                              BLUE(p->lbk.l) / 255.0);
        cairo_set_source_rgb( cr,
                              RED(0xffff00) / 255.0,
                              GREEN(0xffff00) / 255.0,
                              BLUE(0xffff00) / 255.0);
        cairo_rectangle( cr, 0, 0, size.cx, size.cy);
        cairo_fill( cr);
    } else {
        // image background ----------------
        //WinDrawBitmap(pbd->hps, g.pUiData->pOpts->btn.bmpp.hbmp, NULL, (PPOINTL)&r,
        //              0, 0, DBM_STRETCH);
#if 0
        cairo_save( cr);
        image = cairo_image_surface_create_from_png( g.pUiData->tba.pszCur);
        // scaled bitmap
        fx = cairo_image_surface_get_width( image);
        fy = cairo_image_surface_get_height( image);
        cairo_scale( cr, (double) pSize->cx / fx,(double) pSize->cy / fy);
        cairo_set_source_surface( cr, image, 0, 0);
        cairo_paint( cr);
        cairo_restore( cr);
        cairo_surface_destroy (image);
#endif
    }

    // foreground ---------------------------------------------------------

    // draw text shadow
    if (pbd->flStyle & WS_DISABLED) {
        // set disabled color, no halftoned pattern
        color = p->lshadow;
    } else {
        color = p->lfgnd;
    }

    // cairo window coords are from 1,1 to cx,cy
    mSetBtnBorderRect( &r, &size, 1, 1, -1, -1);
    // reduce by 1 pixel
    RectInflate(&r, -1, -1);

    flags = DT_CENTER | DT_VCENTER
            | (o->btn.dis3D ? DT_3D : 0)
            | (pbd->bcd.fsHiliteState ? DT_HILITE : 0);
    drawCairoTitle( cr, o->dlg.achFont, &r, color, p->llite, p->pszText,
                    flags);

    // set the border rectangle, remove 1 pixels (used for def button border)
    RectInflate( &r, -1, -1);

    // if the button is in pressed state
    if (pbd->bcd.fsHiliteState) {

        // draw the outer border first
        if (o->btn.flat)                  // flat style button
            drawCairo3Dborder( cr, &r, p->lshadow, p->llite, 1);
        else                             // non-flat style button
            drawCairo3Dborder( cr, &r, 0, p->lshadow, 1);
        // draw the inner border
        drawCairo3Dborder( cr, &r, p->lshadow, p->llite, 1 + o->btn.border);

    } else {

        // normal state button
        // outer border
        if (o->btn.flat)                  // flat style button
            drawCairo3Dborder( cr, &r, p->lshadow, p->llite, 1);
        else                      // non-flat style button
            drawCairo3Dborder( cr, &r, p->lshadow, 0, 1);

        // draw the inner border
        drawCairo3Dborder( cr, &r, p->llite, p->lshadow, 1 + o->btn.border);

    }

    // set rect to max size
    mSetBtnBorderRect(&r, &size, 1, 1, -1, -1);
    // reduce by 1 pixel
    RectInflate(&r, -1, -1);

    // the button has the BS_DEFAULT style (draw a 3D or a thick black frame)
    if (pbd->flStyle & BS_DEFAULT) {
        if (o->btn.def3D) {               // 3D style
            drawCairo3Dborder( cr, &r, 0, 0xffffff, 1);
            drawCairo3Dborder( cr, &r, p->lbk.l, p->lbk.l, 1);
        } else {                // normal (thick black) style
            drawCairo3Dborder( cr, &r, p->ldef, p->ldef, 2);
        } /* endif */
        // the BS_DEFAULT emphasis is painted outside the button rectangle
        // so when the button does not have the BS_DEFAULT emphasis a frame
        // with the color of the button parent background must be painted
        // around the button to erase a possible previous BS_DEFAULT emphasis
    } else {
        color = mParentBkgndColor(hwnd, pbd->hps);
        drawCairo3Dborder( cr, &r, color, color, 1);
    }

    // draw a halftoned pattern to show the WS_DISABLED style
    if ((pbd->flStyle & WS_DISABLED) && !o->btn.dis3D) {
        //mSetBtnBorderRect(&size, &r, 0, 0, -1, -1);
        //drawDisabledRect(pbd->hps, &r, btn.ldis);
    }

    // render surface on entire window
    cairo_os2_surface_paint_window( surface, pbd->hps, NULL, 0);

    // destroy cairo surface
    cairo_destroy (cr);
    cairo_surface_destroy (surface);

}


/* --------------------------------------------------------------------------
 Draw a 3D border within the rectangle pr the bottom/left corner of the
 rectangle is moved up and right of 'cpBorder' pixels, while the upper/right
 corner is moved down and left, so the resulting rectangle width and height
 are decreased of cpBorder * 2 pixels.
- Parameters -------------------------------------------------------------
 pbd->hps pbd->hps     : presentation space handle
 PRECTL pr   : address of rectangle to be drawn with a 3D border
 ULONG clrul : upper-left border color
 ULONG clrbr : bottom-right border color
 ULONG cpbrd : border thickness in pixels
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID drawCairo3Dborder(cairo_t *cr, PRECTL pr, LONG clrul, LONG clrbr, ULONG cpBorder) {
   POINTL apt[2];

   while (cpBorder--) {
       // top/left border
       cairo_set_line_width(cr, 1.0);
       cairo_set_source_rgb( cr, RED(clrul) / 255.0,
                             GREEN(clrul) / 255.0,
                             BLUE(clrul) / 255.0);
       cairo_move_to( cr, pr->xLeft, pr->yTop);
       cairo_line_to( cr, pr->xLeft, pr->yBottom);
       cairo_line_to( cr, pr->xRight, pr->yBottom);
       cairo_stroke (cr);
       // right/bottom
       cairo_set_source_rgb( cr, RED(clrbr) / 255.0,
                             GREEN(clrbr) / 255.0,
                             BLUE(clrbr) / 255.0);
       cairo_move_to( cr, pr->xRight, pr->yBottom);
       cairo_line_to( cr, pr->xRight, pr->yTop);
       cairo_line_to( cr, pr->xLeft + 0, pr->yTop);
       cairo_stroke (cr);
       RectInflate(pr, -1, -1);
   }

}

/*
  Draw text centered in the given rectangle
  */
void drawCairoTitle( cairo_t* cr, char* fontFace, PRECTL pr,
                     ULONG colorText, ULONG colorShadow,
                     char* title, int fl)
{
    cairo_text_extents_t font_extents;
    CHAR    font[CCH_FONTDATA];
    char*   dot, *face;
    int     fx, fy;

    // select a default font
    cairo_select_font_face( cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                            CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size( cr, 14);

    // using strchr/stddup() on o.ui.tb.achFont crashes this code...
    memset( font, 0, sizeof(font));
    memcpy( font, fontFace, sizeof(font));
    // use font if defined
    if (strchr( font, '.')) {
        dot = strchr( font, '.');
        face = dot + 1;
        *dot = 0;
        // set size of font
        cairo_set_font_size( cr, atoi(font) * 1.5);
        // set font name
        cairo_select_font_face( cr, face, CAIRO_FONT_SLANT_NORMAL,
                                CAIRO_FONT_WEIGHT_NORMAL);
    }

    // calculate the text position
    cairo_text_extents(cr, title, &font_extents);
    if (fl & DT_CENTER)
        fx = (pr->xRight - pr->xLeft) / 2 - (font_extents.width / 2 + font_extents.x_bearing);
    else
        fx = pr->xLeft + 5;

    if (fl & DT_VCENTER)
        fy = (pr->yTop - pr->yBottom) / 2 - (font_extents.height / 2 + font_extents.y_bearing);
    else
        fy = pr->yTop - 5;

    if (fl & DT_HILITE) {
        fx++;
        fy++;
    }

    // draw text shadow
    if (fl & DT_3D) {
        cairo_move_to( cr, fx + 1, fy + 1);
        cairo_set_source_rgb( cr, RED(colorShadow) / 255.0,
                              GREEN(colorShadow) / 255.0,
                              BLUE(colorShadow) / 255.0);
        cairo_show_text( cr, title);
    }

    // draw text
    cairo_move_to( cr, fx, fy);
    cairo_set_source_rgb( cr, RED(colorText) / 255.0,
                          GREEN(colorText) / 255.0,
                          BLUE(colorText) / 255.0);
    cairo_show_text( cr, title);
}


/* --------------------------------------------------------------------------
 Get a color value checking the presentation parameter and the system
 colors.
- Parameters -------------------------------------------------------------
 HWND hwnd : button window handle.
 pbd->hps pbd->hps   : presentation space handle.
 ULONG ulid1 : presentation parameter index of pure RGB color.
 ULONG ulid2 : presentation parameter index of color index.
 LONG ldef   : default color as SYSCLR_* or RGB value.
- Return value -----------------------------------------------------------
 LONG : color as 24 bit RGB value.
-------------------------------------------------------------------------- */
LONG getCtlClr(HWND hwnd, HPS hps, ULONG ulid1, ULONG ulid2, LONG ldef) {
   LONG lclr;
   if (WinQueryPresParam(hwnd, ulid1, ulid2, NULL, 4UL, (PVOID)&lclr,
                         QPF_NOINHERIT | QPF_PURERGBCOLOR |
                         QPF_PURERGBCOLOR | QPF_ID2COLORINDEX))
      return lclr;
   if ((ldef >= SYSCLR_SHADOWHILITEBGND) && (ldef <= SYSCLR_HELPHILITE))
      return mSysColor(ldef);
   return ldef;
}

/* --------------------------------------------------------------------------
 Get the colors to be used to paint the button.
- Parameters -------------------------------------------------------------
 HWND hwnd  : button window handle.
 PBTNDATA p : button data
 pbd->hps pbd->hps    : presentation space handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID getBtnColors(HWND hwnd, HPS hps, PBTNDATA p) {
   p->lbk.l = mBkgndColor(hwnd, hps);
   p->lfgnd = mFgndColor(hwnd, hps);
   p->ldis = mDisabledBkgndColor(hwnd, hps);
   p->ldisf = mDisabledFgndColor(hwnd, hps);
   p->ldef = mDefaultBorderColor(hwnd, hps);
   p->llite = mHiliteBorderColor(hwnd, hps);
   p->lshadow = mDarkBorderColor(hwnd, hps);
}