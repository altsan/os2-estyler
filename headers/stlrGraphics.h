/* --------------------------------------------------------------------------
 stlrGraphics.h : structures used to draw titlebar shades and button bitmaps
 2004/05/08 - Alessandro Cantatore - Italy
 Notes: ------------------------------------------------------------------
 the shades are created by:
 - allocating memory to hold the data of a true color bitmap of the proper
   size,
 - setting in a loop the bits of the bitmap data according to the shade
   settings
 - using the bitmap data with a standard bitmap header to get the handle
   of the resulting bitmap.
 Contents: ---------------------------------------------------------------
 CLR               union of color as LONG and RGB color structure.
 BMPPAINT          data used to paint a bitmap.
 SHADE_BITMAP_SIZE vertical/horizontal bitmap sizes
 BMPINF          minimal BITMAPINFO2 structure
 BMPHDR          minimal BITMAPINFO2 structure including the color palette
 SHDCREATE         data structure of the bitmap created in memory
 ALLOCSIZES        data to be allocated for the vertical/horizontal bitmap
 MACROS            definition of macros used to create the shade
 CREATESHADEFLAGS  shade creation flags
 PROTOTYPES        function prototypes
-------------------------------------------------------------------------- */


#ifndef STLR_SHDDRAW_H_
   #define STLR_SHDDRAW_H_



#define ROUND_BORDER 4

/* --------------------------------------------------------------------------
 CLR               union of color as LONG and RGB color structure.
*/
#pragma pack(1)
typedef union {
   LONG l;
   struct {
      UCHAR blu, grn, red, x;
   } rgb;
} CLR, * PCLR;
#pragma pack()

/* --------------------------------------------------------------------------
 BMPPAINT          data used to paint a bitmap.

                   These data allow to draw a bitmap with fixed margins
                   and a variable size stretched or tiled inner part.
                   The bitmap is divided in 3 or 9 parts and those parts
                   are individually painted with different attributes.
                   +--------+-------------------------------------+--------+
                   +--------+-------------------------------------+--------+
                   |<- dx ->|   this part is stretched or tiled   |<- dx ->|
                   +--------+-------------------------------------+--------+
               dy  +--------+-------------------------------------+--------+
*/
typedef struct {
   HBITMAP hbmp;       // bitmap handle
   LONG cx, cy;        // bitmap width and height
//   LONG dx, dy;        // data used to stretch only part of a bitmap
} BMPPAINT, * PBMPPAINT;

/* --------------------------------------------------------------------------
 SHADE_BITMAP_SIZE vertical/horizontal bitmap sizes
*/

#define SHADEV_CX  256             // vertical shade - bitmap width
#define SHADEV_CY   64             // vertical shade - bitmap height

#define SHADEH_CX 2048             // horizontal shade - bitmap width
#define SHADEH_CY    2             // horizontal shade - bitmap height

/* --------------------------------------------------------------------------
 BMPINF            minimal BITMAPINFO2 structure
*/
#pragma pack(2)
typedef struct {
   ULONG  cbFix;            /* Length of fixed portion of structure   */
   ULONG  cx;               /* Bit-map width in pels                  */
   ULONG  cy;               /* Bit-map height in pels                 */
   USHORT cPlanes;          /* Number of bit planes                   */
   USHORT cBitCount;        /* Number of bits per pel within a plane  */
} BMPINF, * PBMPINF;

/* --------------------------------------------------------------------------
 BMPHDR            minimal BITMAPINFO2 structure including the color palette
*/
// bitmap data including the color palette
typedef struct {
   ULONG      cbFix;            /*  Length of fixed portion of structure. */
   ULONG      cx;               /*  Bitmap width in pels. */
   ULONG      cy;               /*  Bitmap height in pels. */
   USHORT     cPlanes;          /*  Number of bit planes. */
   USHORT     cBitCount;        /*  Number of bits per pel within a plane. */
   RGB2       argbColor[256];   /*  Array of RGB values. */
} BMPHDR, * PBMPHDR;

/* --------------------------------------------------------------------------
 BMPDAT            bitmap parameters and data
*/
// bitmap data including the color palette
typedef struct {
   ULONG      cbFix;            /*  Length of fixed portion of structure. */
   ULONG      cx;               /*  Bitmap width in pels. */
   ULONG      cy;               /*  Bitmap height in pels. */
   USHORT     cPlanes;          /*  Number of bit planes. */
   USHORT     cBitCount;        /*  Number of bits per pel within a plane. */
   RGB2       argbColor[256];   /*  Array of RGB values. */
   BYTE       ab[4];            // variable size array
} BMPDAT, * PBMPDAT;

#pragma pack()
/* --------------------------------------------------------------------------
 SHDCREATE         data structure of the bitmap created in memory
*/
typedef struct {
   BMPINF bi;      // minimal bitmp header informations needed to get
                     // a bitmap handle
   BYTE pb[4];       // bitmap data as a sequence of RGB structures
} SHDCREATE, * PSHDCREATE;

/* --------------------------------------------------------------------------
 ALLOCSIZES        data to be allocated for the vertical/horizontal bitmap
*/
#define SHADEV_CB           (SHADEV_CX * SHADEV_CY * 3 + sizeof(BMPINF))
#define SHADEH_CB           (SHADEH_CX * SHADEH_CY * 3 + sizeof(BMPINF))
#define CB_SHADEALLOC       SHADEV_CB

/* --------------------------------------------------------------------------
 MACROS            definition of macros used to create the shade
*/
// extract RGB values from LONG
#define L2RED(l)  (((l) & 0x00ff0000) >> 16)
#define L2GRN(l)  (((l) & 0x0000ff00) >> 8)
#define L2BLU(l)  ((l) & 0x000000ff)

// bitmap header initialization
#define m_shadeInitBmpHdr(_pbi_, _cx_, _cy_)  \
   (_pbi_)->cbFix = 16,                      \
   (_pbi_)->cx = (_cx_),                     \
   (_pbi_)->cy = (_cy_),                     \
   (_pbi_)->cPlanes = 1,                     \
   (_pbi_)->cBitCount = 24

// maximum color value
#define SHADECLRMAX       ((0xff) << 22)

// ensure that the color value is withing the valid limits
#define m_shadeChkClrLimit(_l_) \
   if ((_l_) < 0)  (_l_) = 0;  else if ((_l_) > SHADECLRMAX) \
      (_l_) = SHADECLRMAX

// single colors delta calculation macros
#define m_shadeDeltaR(l1, l2, c) (((L2RED(l1) - L2RED(l2)) << 22) / (c))
#define m_shadeDeltaG(l1, l2, c) (((L2GRN(l1) - L2GRN(l2)) << 22) / (c))
#define m_shadeDeltaB(l1, l2, c) (((L2BLU(l1) - L2BLU(l2)) << 22) / (c))

// init RGB deltas
#define m_shadeInitDeltas(_dr_, _dg_, _db_, _l1_, _l2_, _c_) \
   (_dr_) = m_shadeDeltaR((_l1_), (_l2_), (_c_)), \
   (_dg_) = m_shadeDeltaG((_l1_), (_l2_), (_c_)), \
   (_db_) = m_shadeDeltaB((_l1_), (_l2_), (_c_))

// init RGB start values (shifted left by 22)
#define m_shadeInitRGB(_lr_, _lg_, _lb_, _clr_)   \
   (_lr_) = (L2RED(_clr_)) << 22;                \
   (_lg_) = (L2GRN(_clr_)) << 22;                \
   (_lb_) = (L2BLU(_clr_)) << 22

// increase the RGB value in the bitmap
#define m_shadeHIncr(_p_, _r_, _g_, _b_, _dr_, _dg_, _db_) \
   *(_p_)++ = (UCHAR)((_b_) >> 22), (_b_) += (_db_); m_shadeChkClrLimit((_b_)); \
   *(_p_)++ = (UCHAR)((_g_) >> 22), (_g_) += (_dg_); m_shadeChkClrLimit((_g_)); \
   *(_p_)++ = (UCHAR)((_r_) >> 22), (_r_) += (_dr_); m_shadeChkClrLimit((_r_))

// increase the RGB value in the bitmap with dithering
#define m_shadeHDitherIncr(_p_, _d_, _i_, _r_, _g_, _b_, _dr_, _dg_, _db_) \
   (_d_) = ((_i_) & 1) ? 0x800000: -0x800000;                             \
   *(_p_)++ = (UCHAR)((_b_) >> 22), (_b_) += (_db_) + (_d_);              \
   m_shadeChkClrLimit((_b_));                                              \
   *(_p_)++ = (UCHAR)((_g_) >> 22), (_g_) += (_dg_) + (_d_);              \
   m_shadeChkClrLimit((_g_));                                              \
   *(_p_)++ = (UCHAR)((_r_) >> 22), (_r_) += (_dr_) + (_d_);              \
   m_shadeChkClrLimit((_r_))

#define m_shadeVSetPixel(_p_, _r_, _g_, _b_) \
   *(_p_)-- = (UCHAR)((_r_) >> 22),         \
   *(_p_)-- = (UCHAR)((_g_) >> 22),         \
   *(_p_)-- = (UCHAR)((_b_) >> 22)

#define m_shadeVIncr(_r_, _g_, _b_, _dr_, _dg_, _db_) \
   (_b_) += (_db_); m_shadeChkClrLimit((_b_));        \
   (_g_) += (_dg_); m_shadeChkClrLimit((_g_));        \
   (_r_) += (_dr_); m_shadeChkClrLimit((_r_))


/* --------------------------------------------------------------------------
 CREATESHADEFLAGS  shade creation flags
 NOTE:
       ensure that the values are compatible with the titlebar options
       flags defined in options.h
*/
#define CREATESHADE_HORZ          0x00    // draw an horizontal shade
#define CREATESHADE_VERT          0x02    // draw a vertical shade
#define CREATESHADE_SIMPLE        0x00    // draw a simple gradient
#define CREATESHADE_DOUBLE        0x08    // draw a double gradient


/* --------------------------------------------------------------------------
 PROTOTYPES        function prototypes
*/
#ifndef _STLRPREF_H_
   BOOL stlrGlobalHBmp(HINI hini, PSZ key, PBYTE p, HPS hps,
                       PBMPPAINT pbp, HMODULE hmod, ULONG id, PBMPDAT pbd);
   HBITMAP stlrGlobalShadeHBmp(HPS hps, PSHDCREATE psc, ULONG fl,
                               LONG clrLeftTop, LONG clrRightBottom);
   VOID stlrFreeGlobalBitmaps(HPS hps, HWND hwnd, PID pid, ULONG flDelete);
   VOID stlrSetBtnPalette(PBMPHDR pbh, PCLR pClrBkgnd);
   HBITMAP stlrHBmpFromBmpData(HPS hps, PBYTE pbmp, PBMPPAINT pbp, PBMPDAT pbd);
   HBITMAP stlrHBmpFromModule(HPS hps, HMODULE hmod, ULONG id,
                              PBMPPAINT pbp, PBMPDAT pbd);
   BOOL stlrBmpData(HINI hini, PSZ pszKey, PBYTE pdata);
   HBITMAP stlrShadeHBmp(HPS hps, PSHDCREATE psc, ULONG fl,
                         LONG clrLeftTop, LONG clrRightBottom);
#endif

#endif // #ifndef STLR_SHDDRAW_H_
