/* --------------------------------------------------------------------------
 button.h : data structures, definitions and macros used by the enhanced
            WC_BUTTON window procedure.

 2004-- - Alessandro Felice Cantatore
- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------
 BTNDATA           data structure used to implement pushbutton enhancements.
 BTNDRAW           data structure used to draw the button.
 MACROS
 BTNRECT           button rectangle macros
 WARPCENTER        WarpCenter event macros
 definitions
-------------------------------------------------------------------------- */

#ifndef _STLR_BUTTONS_H_
   #define _STLR_BUTTONS_H_


/* --------------------------------------------------------------------------
 BTNDATA           data structure used to implement pushbutton enhancements.
*/

#pragma pack(1)
typedef struct {
   PSZ pszText;                 // button text (including the mnemonic)
   USHORT cbText;               // text length
   SHORT mnemOffset;            // mnemonic character offset
   CLR lbk;                     // background color
   LONG lfgnd;                  // foreground color
   LONG ldis;                   // disabled state background color
   LONG ldisf;                  // disabled state foreground color
   LONG ldef;                   // default state border color
   LONG llite;                  // left-top border color
   LONG lshadow;                // bottom-right border color
} BTNDATA, * PBTNDATA;

/* --------------------------------------------------------------------------
 BTNDRAW           data structure used to draw the button.
*/
typedef struct {
   WNDPARAMS wp;                // window parameters
   BTNCDATA bcd;                // hilite state and optional image handle
   HPS hps;                     // presentation space handle
   ULONG flStyle;               // button style
   SIZEL size;                  // button size
   RECTL rd;                    // rectangle used by the paint routines
   POINTL aptl[2];              // used to calculate the text coordinates
   BMPHDR bh;                   // used for palette patching
} BTNDRAW, * PBTNDRAW;

#pragma pack()

/* --------------------------------------------------------------------------
 MACROS
*/

// get the address of the button data from the window words
#define mBtnData(_hwnd_) \
   ((PBTNDATA)WinQueryWindowPtr(_hwnd_, g.cd.btn.cbData))

// if this condition is TRUE the enhanced procedure must be executed
#define mStlrBtnOn()         (o.ui.btn.on && !o.ui.disabled)

// if this condition is TRUE the enhanced procedure must be executed
#define mIsPushBtn(_style_)                         \
  (                                                 \
     (                                              \
        (((_style_) & 0xf) == BS_PUSHBUTTON)        \
        ||                                          \
        (((_style_) & 0xf) == BS_NOTEBOOKBUTTON)    \
     )                                              \
     &&                                             \
     !((_style_) & BS_NOBORDER)                     \
  )

// TRUE if the button has not the BS_NOBORDER style
#define mHasBorder(_hwnd_) \
   (!(WinQueryWindowULong((_hwnd_), QWL_STYLE) & BS_NOBORDER))

// TRUE if the default pushbutton procedure must be reset
//#define mMustResetDefProc(_hwnd_) \
//   (WinQueryWindowULong((_hwnd_), QWL_STYLE) & (BS_NOBORDER | BS_USERBUTTON))

// get the WNDPARAMS of the button
#define mGetBtnWndParms(hwnd, pparms) \
   (g.cd.btn.pfnwp((hwnd), WM_QUERYWINDOWPARAMS, (MPARAM)(pparms), MPVOID))

// get the button hilite state
#define mBtnHiliteState(hwnd) \
   ((BOOL)g.cd.btn.pfnwp((hwnd), BM_QUERYHILITE, MPVOID, MPVOID))

// reset the clipping rectangle
#define mResetClipRect(hps) (GpiSetClipPath((hps), 0, SCP_RESET))

// return TRUE if the current button is the WarpCenter
#define mIsWarpCenter(_mp_)                           \
   (g.is.wcRegistered                                 \
    &&                                                \
    !g.hwnd.warpCenter                                \
    &&                                                \
    (g.pid.pmshell == pid)                            \
    &&                                                \
    (m_creatParm(_mp_)->id == 0x555)                  \
    &&                                                \
    (m_creatParm(_mp_)->hwndParent == g.hwnd.desktop) \
    &&                                                \
    (m_creatParm(_mp_)->hwndOwner == g.hwnd.desktop))


// macro to get the button colors
#define mBkgndColor(_hwnd_, _hps_) \
   getCtlClr(_hwnd_, _hps_, PP_BACKGROUNDCOLOR, PP_BACKGROUNDCOLORINDEX, \
             SYSCLR_BUTTONMIDDLE)

#define mFgndColor(_hwnd_, _hps_) \
   getCtlClr(_hwnd_, _hps_, PP_FOREGROUNDCOLOR, PP_FOREGROUNDCOLORINDEX, \
             SYSCLR_MENUTEXT)
#define mDisabledBkgndColor(_hwnd_, _hps_) \
   getCtlClr(_hwnd_, _hps_, PP_DISABLEDBACKGROUNDCOLOR, \
             PP_DISABLEDBACKGROUNDCOLORINDEX, SYSCLR_BUTTONMIDDLE)

#define mDisabledFgndColor(_hwnd_, _hps_) \
   getCtlClr(_hwnd_, _hps_, PP_DISABLEDFOREGROUNDCOLOR, \
             PP_DISABLEDFOREGROUNDCOLORINDEX, SYSCLR_MENUTEXT)

#define mDefaultBorderColor(_hwnd_, _hps_) \
   getCtlClr(_hwnd_, _hps_, PP_BORDERDEFAULTCOLOR, 0, SYSCLR_BUTTONDEFAULT)

#define mHiliteBorderColor(_hwnd_, _hps_) \
   getCtlClr(_hwnd_, _hps_, PP_BORDERLIGHTCOLOR, 0, SYSCLR_BUTTONLIGHT)

#define mDarkBorderColor(_hwnd_, _hps_) \
   getCtlClr(_hwnd_, _hps_, PP_BORDERDARKCOLOR, 0, SYSCLR_BUTTONDARK)

#define mParentBkgndColor(_hwnd_, _hps_) \
   getCtlClr(WinParent(_hwnd_), (_hps_), \
   PP_BACKGROUNDCOLOR, PP_BACKGROUNDCOLORINDEX, SYSCLR_DIALOGBACKGROUND)

/* --------------------------------------------------------------------------
 BTNRECT           button rectangle macros
*/

// initialize a rectangle from a SIZEL and increase its size
#define mSetBtnBkgndRect(_pr_, _psizel_, _dx_, _dy_)              \
   ((PRECTL)(_pr_))->xLeft = -(_dx_),                             \
   ((PRECTL)(_pr_))->yBottom = -(_dy_),                           \
   ((PRECTL)(_pr_))->xRight = ((PSIZEL)(_psizel_))->cx + (_dx_),  \
   ((PRECTL)(_pr_))->yTop = ((PSIZEL)(_psizel_))->cy + (_dy_)

// set the rectangle coordinates to draw the button border
#define mSetBtnBorderRect(prectl, psizel, x, y, dcx, dcy)              \
   ((PRECTL)(prectl))->xLeft = (x),                                    \
   ((PRECTL)(prectl))->yBottom = (y),                                  \
   ((PRECTL)(prectl))->xRight = ((PSIZEL)(psizel))->cx + (dcx) + (x),  \
   ((PRECTL)(prectl))->yTop = ((PSIZEL)(psizel))->cy + (dcy) + (y)


/* --------------------------------------------------------------------------
 WARPCENTER        WarpCenter event macros
*/
// the X coordinate of a message event is over the WarpCenter shutdown icon
#define mIsMouseOverShutDownIconX(_cy_, _mouPos_) \
   (                                       \
      (                                    \
         ((_cy_) == 26)                    \
         && (MOUSEX(_mouPos_) >= 0x83)     \
         && (MOUSEX(_mouPos_) <= 0x9d)     \
      )                                    \
      ||                                   \
      (                                    \
         ((_cy_) == 22)                    \
         && (MOUSEX(_mouPos_) >= 0x6f)     \
         && (MOUSEX(_mouPos_) <= 0x84)     \
                                           \
      )                                    \
   )

// a mouse event occurred over the WarpCenter shutdown icon
#define mIsMouseOverShutDownIcon(_cy_, _mouPos_) \
   (                                       \
      (                                    \
         ((_cy_) == 26)                    \
         && (MOUSEX(_mouPos_) >= 0x83)     \
         && (MOUSEX(_mouPos_) <= 0x9d)     \
         && MOUSEY(_mouPos_)               \
         && (MOUSEY(_mouPos_) <= 0x1a)     \
      )                                    \
      ||                                   \
      (                                    \
         ((_cy_) == 22)                    \
         && (MOUSEX(_mouPos_) >= 0x6f)     \
         && (MOUSEX(_mouPos_) <= 0x84)     \
         && MOUSEY(_mouPos_)               \
         && (MOUSEY(_mouPos_) <= 0x16)     \
                                           \
      )                                    \
   )

// a mouse event occurred over the WarpCenter clock
#define mIsMouseOverClock(_mouPos_)                                  \
   (                                                                 \
      ((g.scr.cx < 1600) && (MOUSEX(_mouPos_) > (g.scr.cx - 93)))    \
      ||                                                             \
      ((g.scr.cx >= 1600) && (MOUSEX(_mouPos_) > (g.scr.cx - 107)))  \
   )

// emulate WM_BUTTON1 + WM_BUTTON2 events
#define mWarpCenterSendMouseClick(_mp1_, _mp2_)                       \
   g.pfn.warpCenter(g.hwnd.warpCenter,                                \
                    WM_BUTTON1DOWN, (MPARAM)(_mp1_), (MPARAM)(_mp2_)),\
   g.pfn.warpCenter(g.hwnd.warpCenter,                                \
                    WM_BUTTON1UP, (MPARAM)(_mp1_), (MPARAM)(_mp2_))




/* --------------------------------------------------------------------------
 definitions
*/

// dati bottoni
// stile immagine (bitmap, icon, miniicon
#define BS_IMAGE          (BS_BITMAP | BS_ICON | BS_MINIICON)

VOID cairoPaintButton(HWND hwnd, PBTNDATA p, PBTNDRAW pbd);

#endif // #ifndef _STLR_BUTTONS_H_
