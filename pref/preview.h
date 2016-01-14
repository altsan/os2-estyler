/* --------------------------------------------------------------------------
 preview.h :

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */



#ifndef _STLR_PREVIEW_H_
   #define _STLR_PREVIEW_H_

// window classes
#define WC_MAINPVCLIENT  "stlrMainPreviewClient"
#define WC_CHILDPVCLIENT "stlrChildPreviewClient"


#define mUpdatePreviewControl(_hwnd_, _id_) \
   (WinInvalidateRect(WinWindowFromID((_hwnd_), (_id_)), NULL, FALSE))


// flags used to paint the sample button
#define PVBTN_THICK           0x0001
#define PVBTN_FLAT            0x0002
#define PVBTN_OVERRIDEPP      0x0004
#define PVBTN_3DDEFAULT       0x0008
#define PVBTN_3DDISABLED      0x0010
#define PVBTN_SOLIDCOLOR      0x0020

typedef struct {
   UINT border  : 1;           // thick border
   UINT flat    : 1;           // flat border
   UINT overPP  : 1;           // override color
   UINT def3D   : 1;           // draw default button as 3D frame
   UINT dis3D   : 1;           // draw disabled text as 3D
   UINT solid   : 1;           // draw bkgnd as solid color
   UINT         : 0;
   CLR lbk;                     // background color
   LONG lfgnd;                  // foreground color
   LONG ldis;                   // disabled state background color
   LONG ldisf;                  // disabled state foreground color
   LONG ldef;                   // default state border color
   LONG llite;                  // left-top border color
   LONG lshadow;                // bottom-right border color
} PVBTN, * PPVBTN;

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

#endif // #ifndef _STLR_PREVIEW_H_
