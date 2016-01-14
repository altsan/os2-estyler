/* --------------------------------------------------------------------------
 definitions.h : common usage data types and constants

 2004/05/08 - Alessandro Cantatore - Italy

 Contents: ---------------------------------------------------------------
 STLRVER           structure holding the program version
 TITLEBAR          titlebar enhancement constants
 INMESSAGES        PM messages sent to the object window
 APMFLAGS          flags passed as parameter to the procedure executing APM
 TIMER_IDS         timer IDs
 GLOBPM_MESSAGES   global PM messages
 CLASSDATA         structure used to get the most relevant window class data
 SCRRES            structure describing screen resolution and color depth
 SYSHWND           relevant window handles
 WINKEYFLAGS       ID of actions executable via the special Windows keys.
-------------------------------------------------------------------------- */


#ifndef _ESTYLER_DEFINITIONS_H_
   #define _ESTYLER_DEFINITIONS_H_

   #ifndef RC_INVOKED
   #endif

/* --------------------------------------------------------------------------
 STLRVER           structure holding the program version
*/
#pragma pack(1)
typedef struct {
   UCHAR major;     // primary version number
   UCHAR minor;     // secondary version number
   USHORT build;    // build number
} STLRVER, * PSTLRVER;
#pragma pack()

/* --------------------------------------------------------------------------
 TITLEBAR          titlebar enhancement constants
*/
#define TBARBKGNDDEF      0        // solid color background
#define TBARBKGNDSOLIDCLR 0        // solid color background
#define TBARBKGNDSHADE    1        // shade background
#define TBARBKGNDBMP      2        // bitmap background
// obsolete data
#define TBMPSIZE       8192        // max size titlebar bitmap
#define TBMPDEFCX       300        // width of the default titlebar bitmap
#define BTNBMPSIZE     1600        // max size pushbutton bitmap
// bitmap resources IDs
#define IDBMP_TBA         12       // active titlebar (true color)
#define IDBMP_TBI         13       // inactive titlebar (true color)
#define IDBMP_TBA8        16       // active titlebar (256 colors)
#define IDBMP_TBI8        17       // inactive titlebar (256 colors)
#define IDBMP_BTN         15       // pushbutton (true color)
#define IDBMP_BTN8        19       // pushbutton (256 colors)

// modified titlebar settings flags
#define MODIF_TBAR       0x0001     // titlebar (general)
#define MODIF_FONT       0x0002     // font
#define MODIF_ACTWIN     0x0004     // active window
#define MODIF_INACTWIN   0x0008     // inactive window
#define MODIF_PUSHBTN    0x1000     // pushbutton
#define MODIF_ALL        0x0100     // redraw everything
// allocate/free all bitmaps
#define MODIF_ALLBMPS    (MODIF_ACTWIN | MODIF_INACTWIN | MODIF_PUSHBTN)

/* --------------------------------------------------------------------------
 INMESSAGES        PM messages sent to the object window
*/
#define STLRM_DISABLE          (WM_USER + 11)  // enable/disable eStylerLite
#define STLRM_CHECKINSTANCE    (WM_USER + 12)  // unique instance check
#define STLRM_SHUTDOWNREQ      (WM_USER + 13)  // user requested to shutdown
#define STLRM_SHUTDOWNERR      (WM_USER + 14)  // shutdown - load dlg error
#define STLRM_SHUTDOWNSYS      (WM_USER + 15)  // user dismissed shutdown dlg
#define STLRM_LOCKUPSYS        (WM_USER + 16)  // system lockup via hotkey
#define STLRM_SHOWSYSCLOSEBOX  (WM_USER + 17)  // popup shutdown messages
#define STLRM_EXITSHDWNTHREAD  (WM_USER + 18)  // restart shutdown thread
#define STLRM_RESETSHUTDOWNFL  (WM_USER + 19)
// message sent to shutdown background window to start the animation
#define STLRM_STARTANIM        (WM_USER + 20)


/* --------------------------------------------------------------------------
 APMFLAGS          flags passed as parameter to the procedure executing APM
*/
#ifndef APM_SUSPEND
   #define APM_SUSPEND             1       // suspend via APM
#endif
#ifndef APM_POWEROFF
   #define APM_POWEROFF            3       // poweroff via APM
#endif

/* --------------------------------------------------------------------------
 TIMER_IDS         timer IDs
*/
#define TID_DELLKSUSP      0x04ac              // lockup/suspend delay timer
#define TID_DARKSCRN       0x0300              // shutdown animation timer

/* --------------------------------------------------------------------------
 GLOBPM_MESSAGES   global PM messages
 These messages are sent to the frame windows only.
 For this purpose the WM_QUERYFRAMEINFO messages is used with special
 parameters which identify the message as belonging to eStylerLite
*/
// repaint the titlebar
#define STLRM_REDRAWTITLE    ((MPARAM)0x53580212)
// subclass the frame window of the desktop folder
#define STLRM_SUBCLDESKFLDR  ((MPARAM)0x53580213)
// subclass the frame window of the launchpad
#define STRLM_SUBCLLAUNCHPAD ((MPARAM)0x53580214)
// subclass the warp center
#define STLRM_SUBCLWC        ((MPARAM)0x53580215)
// respond to the hide/show all windows command
#define STLRM_HIDEALL        ((MPARAM)0x53580216)
// emulates mouse clicks on the warp center
#define STLRM_WCCMD          ((MPARAM)0x53580217)
// update the font of dialog windows
#define STRLM_DLGWINFONT     ((MPARAM)0x53580218)
// message broadcasted to allow Styler/2 (if installed) to unroll windows
#define STLR2M_SHDWNUNRL     ((MPARAM)0xafc00216)       // on shutdown

/* --------------------------------------------------------------------------
 CLASSDATA         structure used to get the most relevant window class data
*/
typedef struct {
   PFNWP pfnwp;             // default window procedure
   ULONG cbData;            // default size of the window words
} CLASSDATA, * PCLASSDATA;


/* --------------------------------------------------------------------------
 SCRRES            structure describing screen resolution and color depth
*/
typedef struct {
   LONG cx;
   LONG cy;
   LONG cclr;
} SCRRES, * PSCRRES;

/* --------------------------------------------------------------------------
 SYSHWND           relevant window handles
*/

#define IDX_USERINTERFACEPREF           0
#define IDX_SHUTDOWNPREF                1
#define IDX_WINDOWSKEYSPREF             2

/* --------------------------------------------------------------------------
 WINKEYFLAGS       ID of actions executable via the special Windows keys.
*/
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
      IWKEY_WCINFOMENU,         // Show WarpCenter information menu
      IWKEY_ECTOPECSMENU,       // Show the eCS menu (top eCenter)
      IWKEY_ECBOTTOMECSMENU,    // Show the eCS menu (bottom eCenter)
      IWKEY_ECTRAYLIST,         // Show the eCenter tray list
      IWKEY_ECNEXTTRAY,         // Switch to the next eCenter tray
      IWKEY_ECTRAY1,            // Show eCenter tray 1
      IWKEY_ECTRAY2,            // Show eCenter tray 2
      IWKEY_ECTRAY3,            // Show eCenter tray 3
      IWKEY_ECTRAY4,            // Show eCenter tray 4
      IWKEY_ECTRAY5,            // Show eCenter tray 5
      IWKEY_ECTRAY6,            // Show eCenter tray 6
      IWKEY_ECTRAY7,            // Show eCenter tray 7
      IWKEY_ECTRAY8,            // Show eCenter tray 8
      IWKEY_ECTRAY9,            // Show eCenter tray 9
      IWKEY_ECTRAY10,           // Show eCenter tray 10
      IWKEY_ECZORDER,           // Move the eCenter behind or on-top-of the
                                // other windows
      IWKEY_COUNT};



#endif // #ifndef _ESTYLER_DEFINITIONS_H_
