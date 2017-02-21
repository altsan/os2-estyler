#ifndef STLRCOMMON_H
#define STLRCOMMON_H

// since cairo includes os2.h, put after default headers
#include <cairo.h>
#include <cairo-os2.h>

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

#define RED(a) ((a >> 16) & 0xff)
#define GREEN(a) ((a >> 8) & 0xff)
#define BLUE(a) ((a) & 0xff)

#define DT_3D       0x0001
#define DT_HILITE   0x0004

#define mSysColor(_iClr_) \
   (WinQuerySysColor(HWND_DESKTOP, (_iClr_), 0))

#ifndef _STLR_BUTTONS_H_
typedef struct BTNDATA* PBTNDATA;
typedef struct BTNDRAW* PBTNDRAW;
#endif

VOID getBtnColors(HWND hwnd, HPS hps, PBTNDATA p);
LONG getCtlClr(HWND hwnd, HPS hps, ULONG ulid1, ULONG ulid2, LONG ldef);

VOID paintCairoButton( HWND hwnd, PBTNDATA p, PBTNDRAW pbd, PUIOPTIONS o);
VOID drawCairo3DBorder(cairo_t *cr, PRECTL pr, LONG clrul,
                       LONG clrbr, ULONG cpBorder, LONG radius);
void drawCairoTitle( cairo_t* cr, char* font, PRECTL pr,
                     ULONG colorText, ULONG colorShadow,
                     char* title, int fl);

#endif // STLRCOMMON_H
