/* --------------------------------------------------------------------------
 stlrWins.h : structures used by the enhanced WC_TITLEBAR and WC_BUTTON
              window procedures.
-------------------------------------------------------------------------- */

#ifndef STLR_FRAME_H
   #define STLR_FRAME_H

#pragma pack(2)
// struttura dati bitmap (senza palette):
typedef struct {
   ULONG      cbFix;            /*  Length of fixed portion of structure. */
   ULONG      cx;               /*  Bitmap width in pels. */
   ULONG      cy;               /*  Bitmap height in pels. */
   USHORT     cPlanes;          /*  Number of bit planes. */
   USHORT     cBitCount;        /*  Number of bits per pel within a plane. */
} BMPINF, * PBMPINF;

// struttura dati bitmap:
typedef struct {
   ULONG      cbFix;            /*  Length of fixed portion of structure. */
   ULONG      cx;               /*  Bitmap width in pels. */
   ULONG      cy;               /*  Bitmap height in pels. */
   USHORT     cPlanes;          /*  Number of bit planes. */
   USHORT     cBitCount;        /*  Number of bits per pel within a plane. */
   RGB2       argbColor[256];   /*  Array of RGB values. */
} BMPHDR, * PBMPHDR;

// struttura dati titlebar
typedef struct {
   PSZ psz;               // testo titlebar
   SIZEL s;               // dimensioni titlebar
   HBITMAP ash;              // handle bitmap shade attiva
   HBITMAP ish;              // handle bitmap shade inattiva
   SHORT yfont;
   SHORT cyfont;
   SHORT cxfont;
   USHORT cbtxt;          // lunghezza testo
   UINT xshift    : 8;    // distanza da sinistra del testo
   UINT hilited   : 1;    // flag titlebar attiva(1)/inattiva(0)
   UINT actpp     : 1;    // ACTIVETITLE persparm presente
   UINT ictpp     : 1;    // INACTIVETITLE persparm presente
   UINT chkclass  : 1;    // flag controllo classe non winos2
} TBDATA, * PTBDATA;

// dati bottoni
// stile immagine (bitmap, icon, miniicon
#define BS_IMAGE          (BS_BITMAP | BS_ICON | BS_MINIICON)
// abbreviazioni PP e SYSCLR
#define PPBTN_BK         PP_BACKGROUNDCOLOR
#define PPBTN_BKI        PP_BACKGROUNDCOLORINDEX
#define PPBTN_FG         PP_FOREGROUNDCOLOR
#define PPBTN_FGI        PP_FOREGROUNDCOLORINDEX
#define PPBTN_DBK        PP_DISABLEDBACKGROUNDCOLOR
#define PPBTN_DBKI       PP_DISABLEDBACKGROUNDCOLORINDEX
#define PPBTN_DFG        PP_DISABLEDFOREGROUNDCOLOR
#define PPBTN_DFGI       PP_DISABLEDFOREGROUNDCOLORINDEX
#define PPBTN_BDEF       PP_BORDERDEFAULTCOLOR
#define PPBTN_BLIT       PP_BORDERLIGHTCOLOR
#define PPBTN_BDRK       PP_BORDERDARKCOLOR
#define SCBTN_MID        SYSCLR_BUTTONMIDDLE
#define SCBTN_TXT        SYSCLR_MENUTEXT
#define SCBTN_DEF        SYSCLR_BUTTONDEFAULT
#define SCBTN_LIT        SYSCLR_BUTTONLIGHT
#define SCBTN_DRK        SYSCLR_BUTTONDARK

#pragma pack(1)
// struttura dati bottone
// colori bottone
typedef struct {
   PSZ pszText;                 // testo bottone incluso mnemonico
   USHORT cbText;               // lunghezza testo
   SHORT mnemOffset;            // offset carattere mnemonico
   SIZEL size;                     // dimensioni bottone
   CLR lbk;                     // colore background
   LONG lfgnd;                  // colore foreground
   LONG ldis;                   // colore disabilitato
   LONG ldisf;                  // disabled foreground color
   LONG ldef;                   // bordo bottone default
   LONG llite;                  // bordo superiore e sinistro
   LONG lshadow;                // bordo inferiore destro
} BTNDATA, * PBTNDATA;

// struttura dati per disegnare bottone
typedef struct {
   BMPHDR bh;                   // per patching palette
   BTNCDATA bcd;                // hilite state e handle eventuale immagine
   RECTL rd;                    // rettangolo usato da funzioni grafiche
   ULONG uls;                   // stile bottone
   HBITMAP hbkgnd;              // handle bitmap background
   LONG lr, lg, lb;             // usati per patchare palette in memoria e altro
   // i seguenti dati sono utilizzati per risolvere il bug dei driver GRADD
   WNDPARAMS wp;                // window parameters
#ifndef GRADDBUG
   POINTL aptl[2];              // usati per calcolare coordinate testo
#else
   POINTL aptl[6];              // usati per calcolare coordinate testo
                                // E PER BUG DRIVER GRADD
   BMPHDRM bhm;
   HDC hdc;
   HPS hpsm;
   SIZEL sizl;
   HBITMAP hbmpm;
#endif
} BTNDRAW, * PBTNDRAW;

#pragma pack()


// FLAG stato frame window

typedef struct {
   PPIB ppib;
} PROCDATA, *PPROCDATA;

#endif
