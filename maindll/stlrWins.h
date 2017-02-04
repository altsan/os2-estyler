/* --------------------------------------------------------------------------
 stlrWins.h : structures used by the enhanced WC_TITLEBAR and WC_BUTTON
              window procedures.
-------------------------------------------------------------------------- */

#ifndef STLR_FRAME_H
   #define STLR_FRAME_H


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


#endif
