/* --------------------------------------------------------------------------
 estlrOptions.h : structures used to hold the eStylerLite settings
 2004/05/08 - Alessandro Cantatore - Italy

 Contents: ---------------------------------------------------------------

-------------------------------------------------------------------------- */


#ifndef _ESTYLER_OPTIONS_H_
   #define _ESTYLER_OPTIONS_H_

/* --------------------------------------------------------------------------
// - 1.7) dati vari bitmap                                                
-------------------------------------------------------------------------- */

// tipo di background usato x la barra del titolo --------------------------|

#define TBARBKGNDDEF      0        // background titlebar = default
#define TBARBKGNDSHADE    1        // background titlebar = shade
#define TBARBKGNDBMP      2        // background titlebar = bitmap

// dimensioni (byte) e larghezza (pixel) bmp titlebar e controlli finestra

#define TBMPSIZE       8192        // massima dimensione bmp titlebar
#define TBMPDEFCX       300        // larghezza bmp default barra titolo
#define BTNBMPSIZE     1600        // massima dimensione bmp button

// bitmap presenti nella DLL come risorse
#define IDBMP_TBA         12
#define IDBMP_TBI         13


/* --------------------------------------------------------------------------
// - dati_modificati_settaggi_titlebar                                    
-------------------------------------------------------------------------- */

// dati barra titolo
#define MODIF_TBAR       0x0001     // dati barra titolo modificati
#define MODIF_FONT       0x0002     // font modificato
#define MODIF_ACTWIN     0x0004     // finestra attiva
#define MODIF_INACTWIN   0x0008     // finestra inattiva
#define MODIF_ALLTBAR    0x07ff     // poi ripristinare valore di sotto
#define MODIF_ALL        0x0100     // ridisegna tutto il desktop
//#define MODIF_ALLTBAR    0x000f     // usato x abilitazione/disabilitazione

#define STDFFL            0x01000033


/* --------------------------------------------------------------------------
// - 1.9) messaggi privati                                                
-------------------------------------------------------------------------- */

#define STLRM_DISABLE            (WM_USER + 11)
#define STLRM_SHUTDOWN           (WM_USER + 12)
#define STLRM_SHUTDOWNERR        (WM_USER + 13)
#define STLRM_SHUTDOWNSYS        (WM_USER + 14)
#define STLRM_LOCKUPSYS          (WM_USER + 15)
#define STLRM_POPUPSHDWNWND      (WM_USER + 16)
#define STLRM_EXITSHDWNTHREAD    (WM_USER + 17)
#define STLRM_SHDWNPROCLIST      (WM_USER + 18)

#define APM_SUSP                1
#define APM_PWROFF              0
#define TID_DELLKSUSP      0x04ac
#define TID_DARKSCRN       0x0300

/* --------------------------------------------------------------------------
// - define flag per WM_QUERYFRAMEINFO e WM_SYSCOMMAND privati            
-------------------------------------------------------------------------- */

// valore di mp2 per ridisegnare titlebar
#define STLRM_REDRAWTITLE      ((MPARAM)0x53580212)
// subclassa procedura frame desktop folder
#define STLRM_SUBCLDESK        ((MPARAM)0x53580213)
// subclassa procedura frame launchpad
#define STRLM_SUBCLLNCHPD      ((MPARAM)0x53580214)
// subclassa procedura warp center
#define STLRM_SUBCLWC          ((MPARAM)0x53580215)
// processa comando hide/show all windows
#define STLRM_HIDEALL          ((MPARAM)0x53580216)
// spedito alla procedura del warp center simula click mouse in particolari
// posizioni
#define STLRM_WCCMD         ((MPARAM)0x53580217)
// broadcastato allo shutdown di sistema per srotolare finestre Styler/2
#define STLR2M_SHDWNUNRL        ((MPARAM)0x53570216)

/* --------------------------------------------------------------------------
// -1.11) struttura ridotta dati classi finestre                          
-------------------------------------------------------------------------- */

// struttura x memorizzare alcuni dati della struttura CLASSINFO
typedef struct {
   PFNWP pfnwp;             // indirizzo procedura default finestra
   ULONG cbData;            // dimensione window words originali
} CLASSDATA, * PCLASSDATA;


/* --------------------------------------------------------------------------
// -1.12) struttura stato globale sistema                                 
-------------------------------------------------------------------------- */

typedef struct {
   UINT version       : 16; // version minor del S.O.
   UINT bootdrv       : 8;  // ID drive di boot
//   UINT slow          : 1;  // indica macchina < P100
   UINT cywc          : 8;  // altezza warp center:
                            // indica intercettazione button down warp center
   UINT resetWPS      : 1;  // indica che la WPS Š stata resettata
   UINT hideall       : 1;  // indica di nascondere tutte le finestre
   UINT wcbtn         : 1;  // flag che indica di monitorare i msg dei
                            // bottoni per riposizionare pseudomenu warpcenter
                            // sotto il mouse
   UINT winkfldr      : 1;  // flag che indica che una cartella Š stata aperta
                            // tramite una delle windows key per cui deve essere
                            // rilevato WM_ADJUSTWINDOWPOS aggiungendo le flag
                            // SWP_ACTIVATE | SWP_ZORDER
   UINT winkshld      : 1;  // flag che indica che una finestra di comandi Š
                            // stata aperta da una delle window keys per cui
                            // deve essere rilevato WM_ADJUSTWINDOWPOS per
                            // aggiungere flag SWP_ACTIVATE | SWP_ZORDER
   UINT locked        : 1;  // indica attivazione Lockup
   UINT hookshdwn     : 1;  // hooka WM_DESTROY window list per eseguire
                            // shutdown
   UINT replsdwnwin   : 1;  // impedisce apertura messaggi shutdown default
                            // sostituendoli con il proprio dialogo
//   UINT hookshdwn2    : 1;  // hooka WM_DESTROY frame desktop per mostrare
//                            // finestra shutdown alternativa
//                            // shutdown
//   UINT delayopen     : 1;
} SYSSTATE, * PSYSSTATE;


/* --------------------------------------------------------------------------
// -1.13) struttura opzioni titlebar (una x finestra attiva e una inattiva) |
// titlebar background: 0 = tinta unita, 1 = sfumatura, 2 = bitmap        
// border: 0 = piatto, 1 = in rilievo                                     
// direzione sfumatura: 0 = orizzontale, 1 = verticale                    
// testo 3D: 0 = piatto, 1 = 3D                                           
-------------------------------------------------------------------------- */

typedef struct {
   BMPHDR bh;               // header bitmap
   BYTE bmp[TBMPSIZE];      // dati bmp bkgnd titlebar
   // opzioni
   UINT enabled    : 1;     // indica se opzioni attive
   UINT bkgnd      : 2;     // titlebar background
   UINT border     : 1;     // titlebar presenza bordo
   UINT shade      : 1;     // titlebar direzione sfumatura (1 = orizzontale)
   UINT _3DTxt     : 1;     // titlebar testo 3D
   UINT shd3       : 1;     // usa pseudo shade 3 colori (1 = 3 colori)
   UINT strch      : 1;     //* adatta la bitmap alla dimensione della titlebar
   UINT center     : 1;     // indica di centrare il testo
   UINT            : 0;
   // colori
   CLR clrLeftTop;          // colore solid background/left-shade/top-shade
   CLR clrRghtBtm;          // colore right/bottom shade
   CLR clrTopLeftBorder;          // colore hilite background
   CLR clrRightBottomBorder;          // colore shadow background
   CLR clrFgTxt;            // colore foreground testo
   CLR clrBgTxt;            // colore background testo
   // altri dati titolo
//   BYTE bmp[TBMPSIZE];      // dati bmp bkgnd titlebar
//   SIZEL bmpsize;           // dimensioni bmp background
} TBARHILITE, * PTBARHILITE;

// struttura dati opzioni bottone

/* --------------------------------------------------------------------------
// - struttura_opzioni_bottoni                                            
-------------------------------------------------------------------------- */
typedef struct {
   BMPHDR bh;                  // header bitmap usata per sfondo bottoni
   BYTE ab[1600];              // dati bitmap usata per sfondo bottoni
   UINT on      : 1;           // opzioni abilitate
   UINT border  : 1;           // thick border
   UINT flat    : 1;           // stile bordo warp 4
   UINT overPP  : 1;           // override color
   UINT def3D   : 1;           // draw default button as 3D frame
   UINT dis3D   : 1;           // draw disabled text as 3D
   UINT solid   : 1;           // draw bkgnd as solid color
   UINT gradd   : 1;           // flag bug driver GRADD
} BTNOPT, * PBTNOPT;


/* --------------------------------------------------------------------------
// - struttura_opzioni_shutdown                                           
-------------------------------------------------------------------------- */

typedef struct {
   UINT on      : 1;           // extended shutdown on
   UINT lksusp  : 1;           // lockup and suspend
   UINT susp    : 1;           // suspend
   UINT off     : 1;           // power off
   UINT reboot  : 1;           // reboot
   UINT ord     : 1;           // shutdown ordinario
} SDGENERAL, * PSDGENERAL;


/* --------------------------------------------------------------------------
// - struttura_opzioni_shutdown_avanzate                                  
-------------------------------------------------------------------------- */

typedef struct {
   UINT anim    : 1;           // animates shutdown dialog
   UINT prg     : 1;           // executes programs at shutdown
} SDADVANCED, * PSDADVANCED;

/* --------------------------------------------------------------------------
// - struttura_opzioni_attive                                             
-------------------------------------------------------------------------- */

typedef struct {
   TBARHILITE atb;             // opzioni titlebar finestra attiva
   TBARHILITE itb;             // opzioni titlebar finestra inattiva
   CHAR achFont[128];       // font default barra titolo
   BTNOPT btn;              // opzioni bottoni
   SDGENERAL sd;               // opzioni shutdown
   SDADVANCED sda;             // opzioni shutdown avanzate
   // opzioni generali
   UINT disabled      : 1;  // disabilita tutte funzioni smart windows
   UINT applyonexit   : 1;  // applica nuovi settaggi alla chiusura preferenze
   UINT saveonexit    : 1;  // salva nuovi settaggi alla chiusura preferenze
   UINT genexc        : 1;  // indica presenza eccezioni generali
   UINT loaded        : 1;  // indica che la dll Š stata caricata
                            // utilizzato dalla seconda dll
   UINT overridePP         : 1;
   UINT               : 0;
   int lastpage;            // indice ultima pagina selezionata (-1 = disabil)

   // opzioni accelleratori win95 key
   UINT W95lw         : 8;  // operazione associata con left win key
   UINT W95rw         : 8;  // operazione associata con right win key
   UINT W95menu       : 8;  // operazione associata con menu key
   UINT W95on         : 1;  // acceleratori win95 key attivi
   UINT               : 0;
} OPTIONS, * POPTIONS;


// risoluzione schermo:
#define PRF_SCRAPP       "PM_DISPLAYDRIVERS"
#define PRF_SCRKEY       "DEFAULTSYSTEMRESOLUTION"
typedef struct {
   ULONG cx;
   ULONG cy;
   ULONG cclr;
} SCRRES, * PSCRRES;


/* --------------------------------------------------------------------------
// -1.16) struttura dati globali                                          
-------------------------------------------------------------------------- */

typedef struct {
   CLASSDATA cdFrm;    // info class Frame
   CLASSDATA cdTbr;    // info class titlebar
   CLASSDATA cdBtn;    // info class Button
   STLRVER smwver;      // struttura versione corrente
   SHELL_PIDS gpd;   // struttura dati processi attivi
   SYSSTATE is;       // flag stato globale
   CHAR achsd[24];     // dati precedente shutdown
   HAB hab;            // anchor block object window controllo timing
   TID tidTmr;         // TID thread     "       "       "        "
   HWND hwndTmrObj;    // handle         "       "       "        "
   ULONG hmod;         // handle DLL
   // valori sistema
   ULONG cyIco;      // altezza barra titolo
//   ULONG mousemsgs[5]; // array messaggi mouse intercettati
   PFNWP pfnDesktop;   // procedura originale desktop
   PFNWP pfnLnchp;     // procedura originaria launchpad
   PFNWP pfnWc;        // procedura originaria warp center
   HWND hwndDesktopFldr;   // handle frame desktop folder
   HWND hwndDesktop;   // HWND_DESKTOP
   HWND hwndWlist;     // handle window list
   HWND hwndPref;      // handle finestra preferenze
   HWND hwndWC;        // handle warp center
   HWND hwndPrev;      // handle finestra che ha intercettato windows key
   HWND hwndShdwn;     // handle finestra visualizzata allo shutdown
// questo Š aggiunto alla fine cosi' non c'Š pericolo che sovrascriva
// altre variabili nel caso aumenti di dimensione
   OPTIONS set;     // flag opzioni globale
   SCRRES scr;         // dati risoluzione/colori schermo
} GLOBAL, * PGLOBAL;


/* --------------------------------------------------------------------------
// - OBJECT_SETUP_STRINGS                                                 
-------------------------------------------------------------------------- */

#define OBJ_OPENSINGLE       "CCVIEW=NO;MENUITEMSELECTED=1"
#define OBJ_OPENDEF          "MENUITEMSELECTED=1"
#define OBJ_OPENPROPERTY     "MENUITEMSELECTED=112"
#define OBJ_SAVEWC           "SAVENOW=YES"

#define OBJID_ECSCLOCK       "<ECSCLOCK_CLOCK>"
#define OBJID_OLDCLOCK       "<WP_CLOCK>"
#define OBJID_WARPCENTER     "<WP_WARPCENTER>"
#define OBJID_DRIVES         "<WP_DRIVES>"
#define OBJID_SYSSETUP       "<WP_CONFIG>"
#define OBJID_OS2COMMAND     "<WP_OS2WIN>"


/* --------------------------------------------------------------------------
// -1.20)                            macro                                
-------------------------------------------------------------------------- */

// loop eterno
#define forever for(;;)

// inizializzazione version number
#define INITVERNUM  { g.version.Mver = CURMAJVER;\
                      g.version.mver = CURMINVER;\
                      g.version.rver = CURRELVER;\
                      g.version.bver = CURBLDVER;}

#define ABORTLOAD(x)   {stlrlog(x); return 0L;}

// identifica i vari processi:
#define isProcProtshell(ppd) \
   ((ppd)->ppib->pib_ulpid == g.pid.protshell)
#define isProcWps(ppd) \
   ((ppd)->ppib->pib_ulpid == g.pid.pmshell)
#define isProcOther(ppd) \
   (!(((ppd)->ppib->pib_ulpid == g.pid.protshell) || \
      ((ppd)->ppib->pib_ulpid == g.pid.pmshell)))



// restituisce un handle da un puntatore ai dati di una bitmap            

#define loadBmp(hps, pbh, pb) \
 ((HBITMAP)GpiCreateBitmap((hps), (PBITMAPINFOHEADER2)(pbh), CBM_INIT, \
                           (pb), (PBITMAPINFO2)(pbh)))

#define MOUSEX(mp)          ((SHORT)(ULONG)(mp))
#define MOUSEY(mp)          ((SHORT)((ULONG)mp >> 16))

// extended frame flags:
// setta flag hide all nelle window words della frame
#define frameSetHideAllFl(hwnd)                               \
(WinSetWindowULong((hwnd), g.cd.frame.cbData, FRM_HIDEALL))

// resetta flag hide all nelle window words della frame
#define frameResetHideAllFl(hwnd)                               \
(WinSetWindowBits((hwnd), g.cd.frame.cbData, 0, FRM_HIDEALL))
// restituisce TRUE se ha la flag hideall
#define frameIsHideAllFl(hwnd)                                \
(WinQueryWindowULong(hwnd, g.cd.frame.cbData) & FRM_HIDEALL)
// setta flag finestra lockup
#define frameSetLkupFl(hwnd)                                   \
(WinSetWindowULong((hwnd), g.cd.frame.cbData, FRM_LOCKUP))
// identifica frame finestra lockup
#define frameIsLockup(hwnd)                                    \
(WinQueryWindowULong(hwnd, g.cd.frame.cbData) & FRM_LOCKUP)

// ottiene dati titlebar
#define tbarGetdata(hwnd)                                        \
   ((PTBDATA)WinQueryWindowPtr((hwnd), g.cd.tbar.cbData))


// ottiene l'handle della frame della cartella Desktop
//#define getDeskFldrFrmHnd()   (WinQueryWindow(g.hwnd.desktop, QW_BOTTOM))


// setta le opzioni di default dello sfondo della titlebar secondo la
// profondit… di colore clr
#define tbbkgndSetDef(rc, clr)                \
   (((rc) || ((clr) < 256))? TBARBKGNDDEF:      \
                           (((clr) == 256)? TBARBKGNDBMP: TBARBKGNDSHADE))

// restituisce il nome della voce di styler.ini in cui memorizzare le
// opzioni corrispondenti alla profondit… di colore cclrs
#define iniAppCclrs(cclrs) \
   ((cclrs) < 256? 0: ((cclrs) > 256? 2: 1))


// macro cancella bitmap handle se non NULLHANDLE
#define FreeHBMP(x)    {if (x) {GpiDeleteBitmap(x); x = 0;}}

// macro cast mp2 a PCREATESTRUCT
#define getCreate(x)  (((PCREATESTRUCT)mp2)->x)
// macro cast mp2 a PCREATESTRUCT
#define setCreate(x, y)  (((PCREATESTRUCT)mp2)->x = (y))

// messaggio autopostato alla frame per postporre alcune azioni
#define m_stlrMsgPost(hwnd, mp1, mp2) \
(WinPostMsg((hwnd), WM_QUERYFRAMEINFO, (MPARAM)(mp1), (MPARAM)(mp2)))
// messaggio autopostato per postporre subclassing warpcenter
#define postSubBtn(hwnd) \
(WinPostMsg((hwnd), WM_PRESPARAMCHANGED, (MPARAM)(-1L), (MPARAM)(-1L)))

// implementazione acceleratori: -------------------------------------------
#define KC_BASICFLAGS          (KC_ALT | KC_CTRL | KC_SHIFT)

// estrae puntatore struttura PCHRMSG dal parametro 1 di WM_TRANSLATEACCEL
#define PCHMFROMMP(mp) ((PCHRMSG)&(((PQMSG)(mp))->mp1))
// estrae i dati significativi del mp1 del WM_CHAR di WM_TRANSLATEACCEL
#define TRANSACCMP1(mp)    ((ULONG)(((PQMSG)(mp))->mp1) & 0xffff00ff)
// ricava handle della finestra che ha causato il WM_TRANSLATEACCEL
#define HWNDFROMACCMSG(mp) (((PQMSG)(mp))->hwnd)
// come sopra ma castato a MPARAM
#define MPHWNDFROMACCMSG(mp) ((MPARAM)(((PQMSG)(mp))->hwnd))

// mp1 del WM_CHAR generato dalle windows key
#define MP1LEFTWINKEY      0x7e010144
#define MP1RIGHTWINKEY     0x7f010144
#define MP1MENUKEY         0x7c010144


// - azioni previste per le windows_key                                   

enum {IWKEY_NONE,               // Do not perform any action
      IWKEY_HIDEWIN,            // Hide/show all windows
      IWKEY_WINLIST,            // Show window list
      IWKEY_CTXTMENU,           // Show context menu
      IWKEY_LOCKUP,             // Lockup the desktop
      IWKEY_FLDDRIVE,           // Open drive folder
      IWKEY_FLDSYSSETUP,        // Open system setup folder
      IWKEY_OS2CMD,             // Open OS/2 command window
      IWKEY_WCDESKMENU,         // Show WarpCenter desktop menu
      IWKEY_WCWINLIST,          // Show WarpCenter window list
      IWKEY_WCPROCLIST,         // Show WarpCenter process list
      IWKEY_WCTRAYLIST,         // Show WarpCenter tray list
      IWKEY_WCNEXTTRAY,         // Switch to the next WarpCenter tray
      IWKEY_WCINFOMENU};        // Show WarpCenter information menu

   #ifndef RC_INVOKED
   #endif



#endif // #ifndef _ESTYLER_OPTIONS_H_
