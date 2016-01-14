/* --------------------------------------------------------------------------
 estlrOptions.h : structures used to hold the eStylerLite settings
 2004/05/08 - Alessandro Cantatore - Italy

 Contents: ---------------------------------------------------------------

 TBARHILITE        structure used to store the titlebar hilighting options
 TITLEBAR          structure used to store the overall titlebar options
 BTNOPT            structure used for the pushbutton options
 UIOPTIONS         overall interface options
 SDGENERAL         general shutdown options
 SDADVANCED        advanced shutdown options
 SDTIMING          shutdown timers and delay settings
 SHUTDOWN          overall shutdown options
 WINKEY            windows keys options
 OPTIONS           overall options data structure
-------------------------------------------------------------------------- */

#ifndef _ESTYLER_OPTIONS_H_
   #define _ESTYLER_OPTIONS_H_

/* --------------------------------------------------------------------------
 TBARHILITE        structure used to store the titlebar hilighting options
                   i.e those options depending on the titlebar active/inactive
                   state.
 bkgnd (background): 0 (solid color), 1 (shade), 2 (bitmap)
 border            : 0 (no border), 1 (3D border)
 shade (direction) : 0 (horizontal), 1 (vertical)
 _3DTxt            : 0 (normal), 1 (3D)
*/

/* titlebar options flags
 NOTE:
       ensure that the values are compatible with the shade creation flags
       defined in stlrGraphics.h
*/
#define TBO_NOBORDER           0x00    // no border
#define TBO_BORDER             0x01    // draw a border around the titlebar
#define TBO_HORZSHADE          0x00    // draw an horizontal shade
#define TBO_VERTSHADE          0x02    // draw a vertical shade
#define TBO_TEXTNORMAL         0x00    // draw ordinary text
#define TBO_TEXTSHADOW         0x04    // draw a shadow under the text
#define TBO_SHADESIMPLE        0x00    // draw a simple gradient
#define TBO_SHADEDOUBLE        0x08    // draw a double gradient
#define TBO_TILEBMP            0x00    // tile the background bitmap
#define TBO_STRETCHBMP         0x10    // stretch the background bitmap
//#define TBO_LEFTTEXT           0x00    // align text on the left
//#define TBO_CENTERTEXT         0x20    // center the text
// titlebar default flags
//#define TBO_FLDEFAULTS    (TBO_TEXTSHADOW | TBO_SHADEDOUBLE | TBO_CENTERTEXT)
#define TBO_FLDEFAULTS    (TBO_TEXTSHADOW | TBO_SHADEDOUBLE)

typedef struct {
   UINT bkgnd      : 8;        // titlebar background type (0/1/2)
   UINT fl         : 8;        // titlebar flags (see above)
//   UINT border     : 1;        // titlebar border (0/1 -> on/off)
//   UINT shade      : 1;        // shade direction (0/1 -> horizontal/vertical)
//   UINT _3DTxt     : 1;        // 3D text (0/1 -> on/off)
//   UINT shd3       : 1;        // double gradient shade
//   UINT stretch    : 1;        // stretch the bitmap
//   UINT center     : 1;        // center align the text
   UINT            : 0;
   // colori
   LONG clrLeftTop;            // solid background/left-shade/top-shade color
   LONG clrRghtBtm;            // right/bottom shade color
   LONG clrTopLeftBorder;      // 3D hilite color
   LONG clrRightBottomBorder;  // 3D shadow color
   LONG clrFgTxt;              // text foreground color
   LONG clrBgTxt;              // text background color
   BMPPAINT bmpp;              // bitmap paint data
} TBARHILITE, * PTBARHILITE;   // variable names o.ui.tb.a and o.ui.tb.i

/* --------------------------------------------------------------------------
 TITLEBAR          structure used to store the overall titlebar options
*/
#define CCH_FONTDATA 128
typedef struct {
   TBARHILITE a;               // active titlebar options
   TBARHILITE i;               // inactive titlebar options
   CHAR achFont[CCH_FONTDATA]; // default titlebar font
   UINT on            : 1;     // 1/0 titlebar enhancements on/off
   UINT overridePP    : 1;     // 1/0 titlebar override presentation parameters
   UINT center        : 1;     // 1/0 align text to center/left
   UINT               : 0;
} TITLEBAR, * PTITLEBAR;       // variable name o.ui.tb

/* --------------------------------------------------------------------------
 BTNOPT            structure used for the pushbutton options
*/
typedef struct {
   BMPHDR bh;                  // reduced bitmap header
   BYTE ab[1600];              // bitmap data
   UINT on      : 1;           // 1/0 pushbutton enhancements on/off
   UINT border  : 1;           // thick border
   UINT flat    : 1;           // flat border
   UINT overPP  : 1;           // override color
   UINT def3D   : 1;           // draw default button as 3D frame
   UINT dis3D   : 1;           // draw disabled text as 3D
   UINT solid   : 1;           // draw bkgnd as solid color
//   UINT gradd   : 1;           // driver GRADD bug option
   UINT         : 0;
   LONG color;                 // default background color
   BMPPAINT bmpp;              // bitmap paint data
} BTNOPT, * PBTNOPT;           // variable name o.ui.btn

/* --------------------------------------------------------------------------
 DLGOPTIONS        dialog options
*/
#pragma pack(1)
typedef struct {
   CHAR achFont[CCH_FONTDATA]; // default dialog font
   UCHAR on;                   // if TRUE apply the font to all dialogs
   UCHAR overridePP;           // override dialog font pres parms
   UCHAR res1;                 // padding
   UCHAR res2;                 // padding
} DLGOPTIONS, * PDLGOPTIONS;   // variable name o.ui.dlg
#pragma pack()


/* --------------------------------------------------------------------------
 UIOPTIONS         overall interface options
*/
#pragma pack(1)
typedef struct {
   TITLEBAR tb;                // overall titlebar options
   BTNOPT btn;                 // overall pushbutton options
   DLGOPTIONS dlg;             // dialog options
   UCHAR disabled;             // 0/1 : enable/disable all the enhancements
   UCHAR exception;            // 0/1 : no exceptions/ exceptions found
   UCHAR res1;                 // padding
   UCHAR res2;                 // padding
} UIOPTIONS, * PUIOPTIONS;     // variable name o.ui
#pragma pack()

/* --------------------------------------------------------------------------
 SDGENERAL         general shutdown options
*/
typedef struct {
   UINT steps   : 16;          // animations steps (9-9999);
   UINT on      : 1;           // extended shutdown on
   UINT lksusp  : 1;           // lockup and suspend
   UINT susp    : 1;           // suspend
   UINT off     : 1;           // power off
   UINT reboot  : 1;           // reboot
   UINT ord     : 1;           // ordinary shutdown
   UINT anim    : 1;           // animates shutdown dialog
   UINT         : 0;
} SDGENERAL, * PSDGENERAL;     // variable name o.sd.gen

/* --------------------------------------------------------------------------
 SDADVANCED        advanced shutdown options
*/
#pragma pack(1)
typedef struct {
   UCHAR runPrograms;          // executes programs at shutdown
   UCHAR runProgOnce;          // as above (just once or unattended shutdown)
   UCHAR skipKillList;         // do not kill the listed programs on shutdown
   UCHAR reserved;             // just for padding
} SDADVANCED, * PSDADVANCED;   // variable name o.sd.adv
#pragma pack()

/* --------------------------------------------------------------------------
 SDTIMING          shutdown timers and delay settings
*/
#pragma pack(2)
typedef struct {
   USHORT lkupSusp;            // lockup and suspend delay (8000 mS)
   USHORT suspend;             // suspend delay (0mS)
   USHORT warpCenter;          // time to wait to save the WarpCenter settings
                               // (1000 mS)
   USHORT unrollWindows;       // time to wait to unroll any rolled window
                               // (1000 mS)
   USHORT closeBox;            // time to wait to stop the default
                               // system-is-being-shutdown message (6000 mS)
   USHORT sysShutdown;         // time to wait to stop the execution
                               // of WinShutdownSystem() (3000 mS)
   USHORT switchOffMsg;        // time to wait on ordinary shutdown before
                               // displaying the now-is-safe-to-switchoff
                               // message (2000 mS)
   USHORT onPowerOff;          // time to wait before closing the APM driver
                               // on issuing a PowerOff (3000 mS)
   USHORT killProcs;           // time to wait after killing all processes
                               // being terminated, before calling
                               // WinShutdownSystem()
} SDTIMING, * PSDTIMING;       // variable name o.sd.wait
#pragma pack()

/* --------------------------------------------------------------------------
 SHUTDOWN          overall shutdown options
*/
typedef struct {
   SDGENERAL gen;              // shutdown general options
   SDADVANCED adv;             // shutdown advanced options
   SDTIMING wait;              // shutdown timing options
} SHUTDOWN, * PSHUTDOWN;       // variable name o.sd.gen

/* --------------------------------------------------------------------------
 WINKEY            windows keys options
*/
#pragma pack(1)
typedef struct {
   UCHAR on;                   // 0/1 options enabled/disabled
   UCHAR left;                 // left Windows key
   UCHAR right;                // right Windows key
   UCHAR menu;                 // menu key
} WINKEY, * PWINKEY;

/* --------------------------------------------------------------------------
 OPTIONS           overall options data structure
*/
typedef struct {
   UIOPTIONS ui;               // overall user interface options
   SHUTDOWN sd;                // overall shutdown options
   WINKEY wk;                  // overall windows key options
} OPTIONS, * POPTIONS;

#endif // #ifndef _ESTYLER_OPTIONS_H_
