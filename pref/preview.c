/* --------------------------------------------------------------------------
 preview.c : create a preview window displaying the effect of the selected
             eStyler settings

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"
#include "preview.h"
#include "stlrTitlebar.h"
#include "stlrGraphics.h"
#include "button.h"

#include <stlrCommon.h>

// definitions --------------------------------------------------------------

// change the bitmap used to paint a frame window control -------------------
#define _setActiveDlgCtrlBmp(_hmenu_, _id_, _hbmp_) \
(activeDlgProc((_hmenu_), MM_SETITEMHANDLE, (MPARAM)(_id_), (MPARAM)(_hbmp_)))
#define _setInactiveDlgCtrlBmp(_hmenu_, _id_, _hbmp_) \
(inactiveDlgProc((_hmenu_), MM_SETITEMHANDLE, (MPARAM)(_id_), (MPARAM)(_hbmp_)))

// prototypes ---------------------------------------------------------------
static VOID showPreviewWindow(BOOL bShow, BOOL bSetFlag);
static BOOL createPreviewWindow(VOID);
static MRESULT EXPENTRY mainClientProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static VOID mainClientPaint(HWND hwnd);
static VOID mainClientSize(HWND hwnd, ULONG cx, ULONG cy);
static MRESULT EXPENTRY activeDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static VOID rearrangeControls(HWND hwnd, PSWP pswp);
static MRESULT EXPENTRY inactiveDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static VOID subClassTitlebar(HWND hwnd, PTBARHILITE ptbo);
static VOID subClassButtons(HWND hwnd);
static VOID setDialogFont(HWND hwnd);
static MRESULT EXPENTRY newTitlebarProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static MRESULT EXPENTRY newFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static VOID handleTitlebarPaintEvent(HWND hwnd);
static VOID paintTitlebar(HWND hwnd, HPS hps, PTBARHILITE ptbo, PSIZEL pSize);
static MRESULT EXPENTRY newBtnProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static VOID handlePaintRequest(HWND hwnd);
static VOID paintBtn(HWND hwnd, HPS hps);
static BOOL drawDisabledRect(HPS hps, PRECTL prcl, LONG color);
static MRESULT overrideBtnProc(HWND hwnd, ULONG msg, MPARAM mp1);

#ifdef __KLIBC__
#define min( __a, __b) ((__a < __b) ? __a : __b)
#define max( __a, __b) ((__a > __b) ? __a : __b)
#endif

// global variables ---------------------------------------------------------
PFNWP defTitlebarProc;
PFNWP defFrameProc;
PFNWP defBtnProc;

/* --------------------------------------------------------------------------
 Toggle the visibility of the preview window creating it if needed.
- Parameters -------------------------------------------------------------
 BOOL pageTurned: TRUE if the visibility is changed as a consequence of
                  a notebook page turn.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID togglePreviewWindow(BOOL pageTurned) {
   if (pageTurned) {
      showPreviewWindow(g.aNBpageFlags[g.pageIdx] & PGFL_PREVIEW_VISIBLE, FALSE);
   } else {
      showPreviewWindow(!(g.state & STLRIS_PREVIEWVISIBLE), TRUE);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Show/hide the preview windown. If the preview window doesn't exist create it.
- Parameters -------------------------------------------------------------
 BOOL bShow    : TRUE/FALSE (show/hide the preview window).
 BOOL bSetFlag : TRUE/FALSE (reset/ignore the preview window visibility flag).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID showPreviewWindow(BOOL bShow, BOOL bSetFlag) {
   if (bShow) {
      if (!g.pUiData->preview.hwnd) {
         if (createPreviewWindow())
            g.state |= STLRIS_PREVIEWVISIBLE;
      } else {
         WinShowWindow(g.pUiData->preview.hwnd, TRUE);
         g.state |= STLRIS_PREVIEWVISIBLE;
      } /* endif */
   } else {
      WinShowWindow(g.pUiData->preview.hwnd, FALSE);
      if (bSetFlag) g.state &= ~STLRIS_PREVIEWVISIBLE;
   } /* endif */
}


/* --------------------------------------------------------------------------
 Update a component of the preview window.
- Parameters -------------------------------------------------------------
 ULONG updateFlag : flag determining which component(s) must be updated.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID updatePreviewWindow(ULONG updateFlag) {
   HWND hwndMainClient, hwndActive, hwndInactive;
   CHAR achFont[CCH_FONTDATA];
   PSZ pszFont;
   if (!g.pUiData->preview.hwnd) return;
   hwndMainClient = WinWindowFromID(g.pUiData->preview.hwnd, FID_CLIENT);
   hwndActive = WinWindowFromID(hwndMainClient, ID_PREVIEWACTIVE);
   hwndInactive = WinWindowFromID(hwndMainClient, ID_PREVIEWINACTIVE);
   if (updateFlag & PVUPD_TITLEFONT) {
      if (!g.pUiData->pOpts->tb.on) {
         PrfQueryProfileString(HINI_USER, SZPRO_SYSFONT, SZPRO_WINTITLES,
                               SZPRO_WSBOLD, achFont, CCH_FONTDATA);
         pszFont = achFont;
      } else {
         pszFont = g.pUiData->pOpts->tb.achFont;
      } /* endif */
      m_setFont(WinWindowFromID(hwndActive, FID_TITLEBAR), pszFont);
      m_setFont(WinWindowFromID(hwndInactive, FID_TITLEBAR), pszFont);
   } /* endif */
   if (updateFlag & PVUPD_TITLEACTIVE)
      mUpdatePreviewControl(hwndActive, FID_TITLEBAR);
   if (updateFlag & PVUPD_TITLEINACTIVE)
      mUpdatePreviewControl(hwndInactive, FID_TITLEBAR);
   if (updateFlag & PVUPD_BTNNORMAL)
      mUpdatePreviewControl(hwndActive, ID_PVBTNNORMAL);
   if (updateFlag & PVUPD_BTNDEFAULT)
      mUpdatePreviewControl(hwndActive, ID_PVBTNDEFAULT);
   if (updateFlag & PVUPD_BTNDISABLED)
      mUpdatePreviewControl(hwndActive, ID_PVBTNDISABLED);
   if (updateFlag & PVUPD_DLGFONT)
      setDialogFont(hwndActive);
   // set titlebar bitmap buttons
   _setActiveDlgCtrlBmp( WinHWND( hwndActive, FID_MINMAX), SC_MINIMIZE,
                     g.pUiData->min.hbitmap);
   _setActiveDlgCtrlBmp( WinHWND( hwndActive, FID_MINMAX), SC_MAXIMIZE,
                     g.pUiData->max.hbitmap);
   _setActiveDlgCtrlBmp( WinHWND( hwndActive, FID_MINMAX), SC_CLOSE,
                     g.pUiData->close.hbitmap);
   _setInactiveDlgCtrlBmp( WinHWND( hwndInactive, FID_MINMAX), SC_MINIMIZE,
                     g.pUiData->min.hbitmap);
   _setInactiveDlgCtrlBmp( WinHWND( hwndInactive, FID_MINMAX), SC_MAXIMIZE,
                     g.pUiData->max.hbitmap);
   _setInactiveDlgCtrlBmp( WinHWND( hwndInactive, FID_MINMAX), SC_CLOSE,
                     g.pUiData->close.hbitmap);

}


/* --------------------------------------------------------------------------
 Create the preview window.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL createPreviewWindow(VOID) {
   ULONG frameFlags = FCF_TITLEBAR | FCF_SIZEBORDER | FCF_CLOSEBUTTON;
   CHAR achTitle[256];
   HWND hwnd;
   SWP swp;
   SIZEL border;

   loadString(IDS_PREVIEWTITLE, achTitle);
   // register the window classes
   if (!WinRegisterClass(g.appl.hab, WC_MAINPVCLIENT,
                         mainClientProc, CS_SIZEREDRAW, 0))
      return FALSE;
   g.pUiData->preview.hwnd = WinCreateStdWindow(HWND_DESKTOP, 0, &frameFlags,
                                 WC_MAINPVCLIENT, achTitle, WS_VISIBLE,
                                 NULLHANDLE, ID_PREVIEWMAIN, &hwnd);
   if (g.pUiData->preview.hwnd) {
      WinSetOwner(g.pUiData->preview.hwnd, g.appl.hwnd);
      defFrameProc = WinSubclassWindow(g.pUiData->preview.hwnd, newFrameProc);
//      WinSetParent(hwndInactive, hwnd, FALSE);
//      WinSetParent(hwndActive, hwnd, FALSE);
      if (!g.pUiData->preview.cx) {
         g.pUiData->preview.cx = 180, g.pUiData->preview.cy = 94;
         WinMapDlgPoints(g.pUiData->preview.hwnd,
                         (PPOINTL)&g.pUiData->preview.cx, 1, TRUE);
         // find an optimal location for the preview window
         WinQueryWindowPos(g.appl.hwnd, &swp);
         WinSendMsg(g.appl.hwnd, WM_QUERYBORDERSIZE, (MPARAM)&border, MPVOID);
         if (swp.x > (g.scr.cx - swp.x - swp.cx)) {
            g.pUiData->preview.x = max(-border.cx, swp.x - g.pUiData->preview.cx);
         } else {
            g.pUiData->preview.x = min(g.scr.cx + border.cx
                                       - g.pUiData->preview.cx,
                                       swp.x + swp.cx);
         } /* endif */
         g.pUiData->preview.y = swp.y + ((swp.cy - g.pUiData->preview.cy) / 2);
      } /* endif */
      WinSetWindowPos(g.pUiData->preview.hwnd, HWND_TOP,
                      g.pUiData->preview.x, g.pUiData->preview.y,
                      g.pUiData->preview.cx, g.pUiData->preview.cy,
                      SWP_SIZE | SWP_MOVE | SWP_ZORDER | SWP_SHOW);
   } /* endif */
   return (BOOL)g.pUiData->preview.hwnd;
}


/* --------------------------------------------------------------------------
 Procedure of the main client of the preview window.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
static
MRESULT EXPENTRY mainClientProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_CREATE:
         WinLoadDlg(hwnd, hwnd, inactiveDlgProc, g.hResLib,
                    ID_PREVIEWINACTIVE, NULL);
         WinLoadDlg(hwnd, hwnd, activeDlgProc, g.hResLib,
                    ID_PREVIEWACTIVE, NULL);
         break;
      case WM_PAINT:
         mainClientPaint(hwnd);
         break;
      case WM_SIZE:
         mainClientSize(hwnd, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
         break;
      default:
         return WinDefWindowProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}


/* --------------------------------------------------------------------------
 Procedure of the main client of the preview window.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
//static
//MRESULT EXPENTRY activeClientProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
//   switch (msg) {
//      case WM_PAINT:
//         clientPaint(hwnd, SYSCLR_WINDOW);
//         break;
//      default:
//         return WinDefWindowProc(hwnd, msg, mp1, mp2);
//   } /* endswitch */
//   return MRFALSE;
//}


/* --------------------------------------------------------------------------
 Procedure of the main client of the preview window.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
//static
//MRESULT EXPENTRY inactiveClientProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
//   switch (msg) {
//      case WM_PAINT:
//         clientPaint(hwnd, SYSCLR_DIALOGBACKGROUND);
//         break;
//      case WM_SIZE:
//         mainClientSize(hwnd, SHORT1FROMMP(mp2), SHORT2FROMMP(mp2));
//         break;
//      default:
//         return WinDefWindowProc(hwnd, msg, mp1, mp2);
//   } /* endswitch */
//   return MRFALSE;
//}


/* --------------------------------------------------------------------------
 Paint the background of the client window of the preview window with
 SYSCLR_BACKGROUND.
- Parameters -------------------------------------------------------------
 HWND hwnd : handle of the preview window client.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID mainClientPaint(HWND hwnd) {
   RECTL rcl;
   HPS hps = WinBeginPaint(hwnd, NULLHANDLE, &rcl);
   WinFillRect(hps, &rcl, SYSCLR_BACKGROUND);
   WinEndPaint(hps);
}


/* --------------------------------------------------------------------------
 Change the size and position of the child dialogs.
- Parameters -------------------------------------------------------------
 HWND hwnd : handle of the preview window client.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID mainClientSize(HWND hwnd, ULONG cx, ULONG cy) {
   ULONG cyTitleBar, cxSizeBorder, cySizeBorder;
   if ((cx > 200) && (cy > 125)) {
      cyTitleBar = WinSysVal(SV_CYTITLEBAR);
      cxSizeBorder = WinSysVal(SV_CXSIZEBORDER);
      cySizeBorder = WinSysVal(SV_CYSIZEBORDER);
      cx -= 16 + cyTitleBar + cxSizeBorder;
      cy -= 16 + cyTitleBar + cySizeBorder;
      WinSetWindowPos(WinWindowFromID(hwnd, ID_PREVIEWINACTIVE),
                      HWND_BOTTOM, 6, 10 + cySizeBorder + cyTitleBar, cx, cy,
                      SWP_SIZE | SWP_MOVE);
      WinSetWindowPos(WinWindowFromID(hwnd, ID_PREVIEWACTIVE),
                      HWND_TOP, 10 + cxSizeBorder + cyTitleBar, 6, cx, cy,
                      SWP_SIZE | SWP_MOVE);
//      WinInvalidateRect(WinWindowFromID(hwnd, ID_PREVIEWINACTIVE), NULL, TRUE);
//      WinInvalidateRect(WinWindowFromID(hwnd, ID_PREVIEWACTIVE), NULL, TRUE);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Procedure of active window preview.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
static
MRESULT EXPENTRY activeDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         subClassTitlebar(hwnd, &g.pUiData->pOpts->tb.a);
         subClassButtons(hwnd);
         setDialogFont(hwnd);
         break;
      case WM_SYSCOMMAND:
      case WM_COMMAND:
         break;
      case WM_WINDOWPOSCHANGED:
         if (((PSWP)mp1)->fl & SWP_SIZE) rearrangeControls(hwnd, (PSWP)mp1);
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;

}


/* --------------------------------------------------------------------------
 Change the size and position of the inner controls of the preview window.
- Parameters -------------------------------------------------------------
 HWND hwnd : handle of the active window preview:
 PSWP pswp : new window size and position.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID rearrangeControls(HWND hwnd, PSWP pswp) {
   SWP aswp[5];
   HWND ahwnd[5];
   POINTL ptl = {1,1};
   INT i;
   WinMapDlgPoints(hwnd, &ptl, 1, TRUE);
   WinMultWindowFromIDs(hwnd, ahwnd, ID_PVTEXT, ID_PVBTNDISABLED);
   memset(aswp, 0, sizeof(aswp));
   for (i = 0; i < 5; ++i) {
      aswp[i].hwnd = ahwnd[i];
      aswp[i].fl = SWP_SIZE | SWP_MOVE;
   } /* endfor */
   // static text sample
   aswp[0].x = 4 * ptl.x;
   aswp[0].y = 36 * ptl.y;
   aswp[0].cx = pswp->cx - aswp[0].x - aswp[0].x;
   aswp[0].cy = 8 * ptl.y;
   // group box (button samples)
   aswp[1].x = aswp[0].x;
   aswp[1].y = 4 * ptl.y;
   aswp[1].cx = aswp[0].cx;
   aswp[1].cy = 30 * ptl.y;
   // normal button sample
   aswp[2].x = 10 * ptl.x;
   aswp[2].y = 8 * ptl.y;
   aswp[2].cx = (pswp->cx - (28 * ptl.x)) / 3;
   aswp[2].cy = 13 * ptl.y;
   // default button sample
   aswp[3].x = aswp[2].x + aswp[2].cx + 4 * ptl.x;
   aswp[3].y = aswp[2].y;
   aswp[3].cx = aswp[2].cx;
   aswp[3].cy = aswp[2].cy;
   // disabled button sample
   aswp[4].x = aswp[3].x + aswp[3].cx + 4 * ptl.x;
   aswp[4].y = aswp[2].y;
   aswp[4].cx = aswp[2].cx;
   aswp[4].cy = aswp[2].cy;
   WinSetMultWindowPos(g.appl.hab, aswp, 5);
}


/* --------------------------------------------------------------------------
 Procedure of the inactive window preview.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
static
MRESULT EXPENTRY inactiveDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         subClassTitlebar(hwnd, &g.pUiData->pOpts->tb.i);
         break;
      case WM_ADJUSTWINDOWPOS:
         if (((PSWP)mp1)->fl & (SWP_ZORDER | SWP_ACTIVATE))
            ((PSWP)mp1)->fl &= ~(SWP_ZORDER | SWP_ACTIVATE);
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
      case WM_SYSCOMMAND:
      case WM_COMMAND:
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}


/* --------------------------------------------------------------------------
 Subclass the titlebar procedure.
 The titlebar is always painted according to the current settings.
- Parameters -------------------------------------------------------------
 HWND hwnd        : dialog window handle.
 PTBARHILITE ptbo : titlebar hiliting options.
- Return value -----------------------------------------------------------
 BOOL
   TRUE/FALSE (success/error)
 PSZ
 VOID
-------------------------------------------------------------------------- */
static VOID subClassTitlebar(HWND hwnd, PTBARHILITE ptbo) {
   hwnd = WinWindowFromID(hwnd, FID_TITLEBAR);
   WinSetWindowPtr(hwnd, QWL_USER, ptbo);
   defTitlebarProc = WinSubclassWindow(hwnd, newTitlebarProc);
}


/* --------------------------------------------------------------------------
 Subclass the sample pushbuttons.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID subClassButtons(HWND hwnd) {
   defBtnProc = WinSubclassWindow(DlgItemHwnd(hwnd, ID_PVBTNNORMAL), newBtnProc);
   WinSubclassWindow(DlgItemHwnd(hwnd, ID_PVBTNDEFAULT), newBtnProc);
   WinSubclassWindow(DlgItemHwnd(hwnd, ID_PVBTNDISABLED), newBtnProc);
}


/* --------------------------------------------------------------------------
 Change the font displayed in the dialog preview.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setDialogFont(HWND hwnd) {
   if (g.pUiData->pOpts->dlg.on) {
      m_setFont(hwnd, g.pUiData->pOpts->dlg.achFont);
   } else {
      WinRemovePresParam(hwnd, PP_FONTNAMESIZE);
   } /* endif */
}


/* --------------------------------------------------------------------------
 New titlebar procedure.
 Ignore all mouse events, paint the titlebar according to the current
 settings.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
static
MRESULT EXPENTRY newTitlebarProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case TBM_SETHILITE:
         return MRTRUE;
      case WM_BUTTON1DOWN:
      case WM_BUTTON1UP:
      case WM_BUTTON1DBLCLK:
      case WM_BUTTON2DOWN:
      case WM_BUTTON2UP:
      case WM_BUTTON2DBLCLK:
      case WM_BUTTON3DOWN:
      case WM_BUTTON3UP:
      case WM_BUTTON3DBLCLK:
      case WM_CHORD:
      case WM_BUTTON1CLICK:
      case WM_BUTTON2CLICK:
      case WM_BUTTON3CLICK:
         break;
      case WM_PAINT:
         handleTitlebarPaintEvent(hwnd);
         break;
      default:
         return defTitlebarProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}


/* --------------------------------------------------------------------------
 Superclassed procedure of the main frame of the preview window.
 This procedure intercepts:
 - WM_CLOSE to hide the preview window.
 - WM_QUERYTRACKINFO to set the minimial size of the preview window.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
static
MRESULT EXPENTRY newFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if ((msg == WM_SYSCOMMAND) && ((ULONG)mp1 == SC_CLOSE)) {
      togglePreviewWindow(FALSE);
      return MRFALSE;
   } else if (msg == WM_QUERYTRACKINFO) {
      MRESULT mr = defFrameProc(hwnd, msg, mp1, mp2);
      POINTL ptl = {180, 96};
      if (WS_MINIMIZED & WinQueryWindowULong(hwnd, QWL_STYLE)) return mr;
      WinMapDlgPoints(g.pUiData->preview.hwnd, &ptl, 1, TRUE);
      ((PTRACKINFO)mp2)->ptlMinTrackSize.x = ptl.x;
      ((PTRACKINFO)mp2)->ptlMinTrackSize.y = ptl.y;
      return mr;
   } /* endif */
   return defFrameProc(hwnd, msg, mp1, mp2);
}


/* --------------------------------------------------------------------------
 Paint the titlebar according to the current settings.
- Parameters -------------------------------------------------------------
 HWND hwnd : titlebar window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static
VOID handleTitlebarPaintEvent(HWND hwnd) {
   PTBARHILITE ptbo;
   TBARHILITE tbh;
   HPS hps;
   POINTL aptl[2];
   if (g.pUiData->pOpts->tb.on) {
      ptbo = WinQueryWindowPtr(hwnd, QWL_USER);
   } else {
      memset(&tbh, 0, sizeof(tbh));
      if (ID_PREVIEWACTIVE == WinID(WinParent(hwnd))) {
         tbh.fl = TBO_BORDER;
         tbh.clrLeftTop = mSysColor(SYSCLR_ACTIVETITLE);
         tbh.clrTopLeftBorder = 0x808080;
         tbh.clrRightBottomBorder = 0xffffff;
         tbh.clrFgTxt = mSysColor(SYSCLR_ACTIVETITLETEXT);
      } else {
         tbh.clrLeftTop = mSysColor(SYSCLR_INACTIVETITLE);
         tbh.clrFgTxt = mSysColor(SYSCLR_INACTIVETITLETEXT);
      } /* endif */
      ptbo = &tbh;
   } /* endif */
   if (ptbo
       &&
       (NULLHANDLE != (hps = WinBeginPaint(hwnd, NULLHANDLE, NULL)))) {
      WinQueryWindowRect(hwnd, (PRECTL)aptl);
      paintTitlebar(hwnd, hps, ptbo, (PSIZEL)(aptl + 1));
      WinEndPaint(hps);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Paint the titlebar.
- Parameters -------------------------------------------------------------
 HWND hwnd        : titlebar window handle.
 HPS hps          : presentation space handle.
 PTBARHILITE ptbo : titlebar options.
 PSIZEL           : titlebar width/height.
- Return value -----------------------------------------------------------
 VOID:
-------------------------------------------------------------------------- */
static
VOID paintTitlebar(HWND hwnd, HPS hps, PTBARHILITE ptbo, PSIZEL pSize) {
   CHAR achText[64];
   INT cbText;
   cairo_surface_t *surface = NULL;
   cairo_t *cr = NULL;
   int     fx, fy;
   cairo_pattern_t *pattern = NULL;
   cairo_surface_t *image = NULL;
   int     flags;
   RECTL r;

   // query window rect
   WinQueryWindowRect( hwnd, &r);
   // adjust rect, cairo starts from 1,1
   r.xLeft = 1;
   r.yBottom = 1;

   // query window title
   cbText = WinQueryWindowText(hwnd, sizeof(achText), achText);

   // initialize cairo surface
   surface = cairo_os2_surface_create_for_window( hwnd, pSize->cx, pSize->cy);
   cr = cairo_create( surface);
   // disable aliasing for rendering primitives, but still used for fonts
   cairo_set_antialias( cr, CAIRO_ANTIALIAS_NONE);

   // draw background
   switch (ptbo->bkgnd) {
       // gradient background
   case TBARBKGNDSHADE:
       if (ptbo->fl & TBO_VERTSHADE) { // vertical gradient
           pattern = cairo_pattern_create_linear( 0.0, 0.0, 0, pSize->cy);
       } else {
           pattern = cairo_pattern_create_linear(0.0, 0.0, pSize->cx, 0);
       }
       cairo_pattern_add_color_stop_rgb( pattern, 0.1,
                                         RED(ptbo->clrLeftTop) / 255.0,
                                         GREEN(ptbo->clrLeftTop) / 255.0,
                                         BLUE(ptbo->clrLeftTop) / 255.0);
       if (ptbo->fl & TBO_SHADEDOUBLE) {
           cairo_pattern_add_color_stop_rgb( pattern, 0.5,
                                             RED(ptbo->clrRghtBtm) / 255.0,
                                             GREEN(ptbo->clrRghtBtm) / 255.0,
                                             BLUE(ptbo->clrRghtBtm) / 255.0);
           cairo_pattern_add_color_stop_rgb( pattern, 0.9,
                                             RED(ptbo->clrLeftTop) / 255.0,
                                             GREEN(ptbo->clrLeftTop) / 255.0,
                                             BLUE(ptbo->clrLeftTop) / 255.0);
       } else {
           cairo_pattern_add_color_stop_rgb( pattern, 0.9,
                                             RED(ptbo->clrRghtBtm) / 255.0,
                                             GREEN(ptbo->clrRghtBtm) / 255.0,
                                             BLUE(ptbo->clrRghtBtm) / 255.0);
       }
       cairo_rectangle( cr, 0.0, 0.0, pSize->cx, pSize->cy);
       cairo_set_source( cr, pattern);
       cairo_fill( cr);
       cairo_pattern_destroy( pattern);
       break;
       // bitmap background -----------------------------------------------
   case TBARBKGNDBMP:
       cairo_save( cr);
       if (hwnd == g.pUiData->hwndActiveTbar)
           image = cairo_image_surface_create_from_png( g.pUiData->tba.pszCur);
       else
           image = cairo_image_surface_create_from_png( g.pUiData->tbi.pszCur);
       if (ptbo->fl & TBO_STRETCHBMP) {
           // scaled bitmap
           fx = cairo_image_surface_get_width( image);
           fy = cairo_image_surface_get_height( image);
           cairo_scale( cr, (double) pSize->cx / fx,(double) pSize->cy / fy);
           cairo_set_source_surface( cr, image, 0, 0);
           cairo_paint( cr);
       } else {
           // tiled bitmap
           pattern = cairo_pattern_create_for_surface( image);
           cairo_pattern_set_extend( pattern, CAIRO_EXTEND_REPEAT);
           cairo_set_source( cr, pattern);
           cairo_paint( cr);
           cairo_pattern_destroy( pattern);
       }
       cairo_restore( cr);
       cairo_surface_destroy (image);
       break;
       // solid color background ------------------------------------------
   default:
       // fill background
       cairo_set_source_rgb( cr,
                             RED(ptbo->clrLeftTop) / 255.0,
                             GREEN(ptbo->clrLeftTop) / 255.0,
                             BLUE(ptbo->clrLeftTop) / 255.0);
       cairo_rectangle( cr, 0, 0, pSize->cx, pSize->cy);
       cairo_fill( cr);
       break;
   }

   // draw title
   flags = DT_VCENTER
           | (g.pUiData->pOpts->tb.center ? DT_CENTER : 0)
           | (ptbo->fl & TBO_TEXTSHADOW ? DT_3D : 0);
   drawCairoTitle( cr, g.pUiData->pOpts->tb.achFont, &r, ptbo->clrFgTxt,
                   ptbo->clrBgTxt, achText, flags);

   // titlebar border
   if (ptbo->fl & TBO_BORDER)
       drawCairo3DBorder( cr, &r, ptbo->clrTopLeftBorder,
                          ptbo->clrRightBottomBorder, 1, 0);

   // render surface on entire window
   cairo_os2_surface_paint_window( surface, hps, NULL, 0);

   // destroy cairo surface
   if (cr)
       cairo_destroy (cr);
   if (surface)
       cairo_surface_destroy (surface);

}


/* --------------------------------------------------------------------------
 New pushbutton procedure.
 Paint the button according to the current button options and button ID.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
static
MRESULT EXPENTRY newBtnProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_PAINT:
         handlePaintRequest(hwnd);
         return MRFALSE;
      // button state change ---------------------------------------------
      case BM_SETDEFAULT:
      case BM_SETHILITE:
         return overrideBtnProc(hwnd, msg, mp1);
   } /* endswitch */
   return defBtnProc(hwnd, msg, mp1, mp2);
}


/* --------------------------------------------------------------------------
 WM_PAINT message handler.
- Parameters -------------------------------------------------------------
 HWND hwnd  : button window handle.
 PBTNDATA p : button data
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID handlePaintRequest(HWND hwnd) {
   HPS hps;
   if (NULLHANDLE != (hps = WinBeginPaint(hwnd, NULLHANDLE, NULL))) {
      paintBtn(hwnd, hps);
      WinEndPaint(hps);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Paint the button.
- Parameters -------------------------------------------------------------
 HWND hwnd    : button window handle.
 HPS hps      : presentation space handle.
 ULONG style  : button style.
 PPVBTN pbtn  : current button settings.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID paintBtn(HWND hwnd, HPS hps) {
   CHAR buf[64];
   ULONG cbText;
   BTNDATA bdata;
   BTNDRAW bdraw;

   // initialization -----------------------------------------------------
   switch (WinID(hwnd)) {
      case ID_PVBTNNORMAL:   bdraw.flStyle = 0;           break;
      case ID_PVBTNDEFAULT:  bdraw.flStyle = BS_DEFAULT;  break;
      case ID_PVBTNDISABLED: bdraw.flStyle = WS_DISABLED; break;
   } /* endswitch */

   // get button text
   cbText = WinQueryWindowText(hwnd, sizeof(buf), buf);

   // setup data structures for preview
   bdata.pszText = buf;
   bdraw.hps = hps;
   bdraw.bcd.fsHiliteState = (ULONG)defBtnProc(hwnd, BM_QUERYHILITE,
                                               MPVOID, MPVOID);
   getBtnColors(hwnd, hps, &bdata);

   // do painting
   paintCairoButton( hwnd, &bdata, &bdraw, g.pUiData->pOpts);

}


/* --------------------------------------------------------------------------
 Draw an halftoned rectangle using the color 'color'.
- Parameters -------------------------------------------------------------
 HPS hps     : presentation space handle.
 PRECTL prcl : rectangle to draw.
 LONG color  : halftone pattern color.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL drawDisabledRect(HPS hps, PRECTL prcl, LONG color) {
   RECTL rcl = *prcl;
   rcl.xRight--;
   rcl.yTop--;
   return GpiMove(hps, (PPOINTL)&rcl) &&
          GpiSetPattern(hps, (((prcl->xLeft % 2) != (prcl->yBottom % 2))?
                                          PATSYM_DENSE5: PATSYM_HALFTONE)) &&
          GpiSetColor(hps, color) &&
          GpiBox(hps, DRO_FILL, (PPOINTL)&rcl + 1, 0L, 0L);
}


/* --------------------------------------------------------------------------
 Lock window update, call the system default button window procedure,
 unlock the window update and paint the button.
- Parameters -------------------------------------------------------------
 HWND hwnd  : button window handle.
 ULONG msg  : PM message to be executed by the system button procedure.
 MPARAM mp1 : first message parameter.
- Return value -----------------------------------------------------------
 MRESULT : value returned by the default button procedure.
-------------------------------------------------------------------------- */
static MRESULT overrideBtnProc(HWND hwnd, ULONG msg, MPARAM mp1) {
   MRESULT mr;
   HPS hps;

   if (WinIsWindowShowing(hwnd)) {
      WinEnableWindowUpdate(hwnd, FALSE);
      mr = defBtnProc(hwnd, msg, mp1, MPVOID);
      WinEnableWindowUpdate(hwnd, TRUE);
      if (NULLHANDLE != (hps = WinGetPS(hwnd))) {
         paintBtn(hwnd, hps);
         WinReleasePS(hps);
      } /* endif */
      return mr;
   } /* endif */
   return defBtnProc(hwnd, msg, mp1, MPVOID);
}

