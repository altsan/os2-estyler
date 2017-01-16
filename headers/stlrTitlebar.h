/* --------------------------------------------------------------------------
 stlrTitlebar.h : data structures, definitions and macros used by the enhanced
                  WC_TITLEBAR window procedure.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */



#ifndef _STLR_WC_TITLEBAR_H_
   #define _STLR_WC_TITLEBAR_H_

#include <cairo.h>
#include <cairo-os2.h>

/* --------------------------------------------------------------------------

*/
#pragma pack(2)
typedef struct {
   PSZ pszText;           // titlebar text
   SIZEL size;            // titlebar size
//   HBITMAP ash;              // handle bitmap shade attiva
//   HBITMAP ish;              // handle bitmap shade inattiva
   SHORT yfont;           // font descender
   SHORT cyfont;          // font height
   SHORT cxfont;
   USHORT cbText;         // text length
   UINT xshift    : 8;    // centered text offset (pixels) from the left
   UINT hilited   : 1;    // active/incative titlebar flag
   UINT actpp     : 1;    // ACTIVETITLE persparm detected
   UINT ictpp     : 1;    // INACTIVETITLE persparm detected
   UINT chkclass  : 1;    // to check this is not a WinOS2 titlebar
   cairo_surface_t *image;      // cairo bitmap handle
   int              image_fx;
   int              image_fy;
} TBDATA, * PTBDATA;
#pragma pack()

#define mTitlebarData(hwnd) \
   ((PTBDATA)WinQueryWindowPtr((hwnd), g.cd.tbar.cbData))

#define mTitlebarEnhancementsOn() \
   (!o.ui.disabled && o.ui.tb.on)

// TRUE if according to the current override-PresParm settings the titlebar
// must be painted via the enhanced procedure
#define mMustPaintTitlebar(_p_)            \
   (                                       \
      o.ui.tb.overridePP                   \
      ||                                   \
      ((_p_)->hilited && !(_p_)->actpp)    \
      ||                                   \
      !((_p_)->hilited || (_p_)->ictpp)    \
   )

// TRUE if the WM_PAINT message must be processed by the enhanced procedure
#define mMustProcessWMpaint(_p_)              \
   (                                          \
      pid                                     \
      &&                                      \
      mTitlebarEnhancementsOn()               \
      &&                                      \
      (NULL != ((_p_) = mTitlebarData(hwnd))) \
      &&                                      \
      ((_p_)->size.cx > 0)                    \
      &&                                      \
      ((_p_)->size.cy > 1)                    \
      &&                                      \
      mMustPaintTitlebar(_p_)                 \
   )

// TRUE if the presentation parameter '_pp_' is set
#define mIsPresParmSet(_hwnd_, _pp_, _ul_)                  \
   (WinQueryPresParam((_hwnd_), (ULONG)(_pp_), 0, NULL,     \
                      sizeof(_ul_), &(_ul_), QPF_NOINHERIT) \
    != 0)

#define mGetClientWindowClass(_hwnd_, _buf_) \
   (WinQueryClassName(WinWindowFromID(WinParent(_hwnd_), FID_CLIENT), \
                      sizeof(_buf_), (_buf_)))

// draw a bitmap without scaling it
#define mDrawBitmap(hps, hbmp, pSrcRect, pDestPoint) \
   WinDrawBitmap((hps), (hbmp), (PRECTL)(pSrcRect), (PPOINTL)(pDestPoint), \
                 0, 0, DBM_NORMAL)

// draw a bitmap without scaling it to a destination rectangle
#define mScaleBitmap(hps, hbmp, pSrcRect, pDestRect) \
   WinDrawBitmap((hps), (hbmp), (PRECTL)(pSrcRect), (PPOINTL)(pDestRect), \
                 0, 0, DBM_STRETCH)

// set an array of 3 POINTL structure defining the source rectangle
// (part of the bitmap to be drawn) and the destination point (where the
// bitmap must be drawn)
#define mSetBmpSrcRectDestPos(ppt, x0s, y0s, x1s, y1s, x0d, y0d) \
   (ppt)[0].x = x0s, (ppt)[0].y = y0s,\
   (ppt)[1].x = x1s, (ppt)[1].y = y1s,\
   (ppt)[2].x = x0d, (ppt)[2].y = y0d

// draw a part of a bitmap to a given position
#define mDrawBmpRectAt(hps, hbmp, ppt, x0s, y0s, x1s, y1s, x0d, y0d) \
   mSetBmpSrcRectDestPos((ppt), (x0s), (y0s), (x1s), (y1s), (x0d), (y0d)), \
   mDrawBitmap((hps), (hbmp), (ppt), ((ppt) + 2))

// set an array of 4 POINTL structure defining the source rectangle
// (part of the bitmap to be drawn) and the destination rectangle
// (where the bitmap must be drawn)
#define mSetBmpSrcRectDestRect(ppt, x0s, y0s, x1s, y1s, x0d, y0d, x1d, y1d) \
   (ppt)[0].x = x0s, (ppt)[0].y = y0s,\
   (ppt)[1].x = x1s, (ppt)[1].y = y1s,\
   (ppt)[2].x = x0d, (ppt)[2].y = y0d,\
   (ppt)[3].x = x1d, (ppt)[3].y = y1d

// draw a part of a bitmap ( (x0s,y0s) - (x1s,y1s) ) scaling it to a given
// rectangle ( (x0d,y0d) - (x1d,y1d) )
#define mScaleBmpRectAt(hps, hbmp, ppt, x0s, y0s, x1s, y1s, x0d, y0d, x1d, y1d) \
   mSetBmpSrcRectDestRect((ppt), (x0s), (y0s), (x1s), (y1s), (x0d), (y0d), \
                          (x1d), (y1d)),                                   \
   mScaleBitmap((hps), (hbmp), (ppt), ((ppt) + 2))

#define mScaleBmpToRect(hps, hbmp, ppt, x0d, y0d, x1d, y1d) \
   RectSet((ppt), (x0d), (y0d), (x1d), (y1d)), \
   mScaleBitmap((hps), (hbmp), NULL, (ppt))

#endif // #ifndef _STLR_WC_TITLEBAR_H_
