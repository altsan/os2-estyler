/* --------------------------------------------------------------------------
 frame.c: enhanced WC_FRAME window procedure.
 2004/05/08 - Alessandro Cantatore - Italy

 naming conventions:
 m*           (followed by upper case) is the name of the macros defined in
              .\frame.h.
 m_*          is the name of the global macros defined in ..\headers\macros.h
-------------------------------------------------------------------------- */

#pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"
#include "frame.h"
#include "stlrShutdown.h"
#include "stlrTitlebar.h"


// global variables ---------------------------------------------------------
extern PID pid;

// prototypes ---------------------------------------------------------------
MRESULT EXPENTRY newDeskProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY newLnchpProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
BOOL isWinInScreen(HWND hwnd);
static VOID setDefFrameCtrlsImage( HWND);

// change the bitmap used to paint a frame window control -------------------
#define _setFrameCtrlBmp(_hmenu_, _id_, _hbmp_) \
(g.cd.menu.pfnwp((_hmenu_), MM_SETITEMHANDLE, (MPARAM)(_id_), (MPARAM)(_hbmp_)))

/* --------------------------------------------------------------------------
 Enhanced WC_FRAME procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT
-------------------------------------------------------------------------- */
MRESULT EXPENTRY stlrFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
//**/  return (MRESULT)g.cd.frame.pfnwp(hwnd, msg, mp1, mp2);
   PTBDATA p;

   switch (msg) {
      case WM_ADJUSTWINDOWPOS:
         if (
               (((PSWP)mp1)->fl & SWP_SHOW)
               &&
               !o.ui.disabled
               &&
               mIsExFlag(hwnd, FRM_INITFONT)
            ) {
            // use the 'msg' variable for other purposes:
            // check if the dialog already use a non-default font
            msg = mHasFont(hwnd, &msg) ? FRM_HASOWNFONT : 0;
            // if override-non-default-font is set or the dialog does not use
            // a non-default font set a new font and update the font flags
            if (o.ui.dlg.overridePP || !msg) {
               m_setFont(hwnd, o.ui.dlg.achFont);
               msg |= FRM_STLRFONTSET;
            } /* endif */
            mSetExFrameFlagMask(hwnd, msg, FRM_INITFONTMASK);
dbgPrintf3("Styler font initialized - hwnd:%08x, xframeflag: %08x\n",
           hwnd, mExFrameFlag(hwnd));
            msg = WM_ADJUSTWINDOWPOS;

            // it is not necessary to superclass to replace bitmaps
            // update menu bitmap buttons
            setDefFrameCtrlsImage( WinHWND( hwnd, FID_MINMAX));

         } /* endif */
         break;
      // inner messages sent via WM_QUERYFRAMINFO special parameters
      case WM_QUERYFRAMEINFO:
         if (!pid || !mp1) break;
dbgPrintf3("WM_QUERYFRAMEINFO hwnd = %08x - mp1 = %08x\n", hwnd, mp1);
         // titlebar update
         if (mp1 == STLRM_REDRAWTITLE) {
            HWND hTbar = WinWindowFromID(hwnd, FID_TITLEBAR);
            if ((ULONG)mp2 & MODIF_FONT)
               m_setFont(hTbar, o.ui.tb.achFont);

            // destroy cairo image
            if ((p = mTitlebarData( hTbar)) != NULL) {
               cairo_surface_destroy( p->image);
               // force reloading
               p->image = NULL;
            }

            // it is not necessary to superclass to replace bitmaps
            // update menu bitmap buttons
            setDefFrameCtrlsImage( WinHWND( hwnd, FID_MINMAX));

            if (WinIsWindowShowing(hTbar))
               WinInvalidateRect(hTbar, NULL, FALSE);

         // deferred subclassing of the frame of the desktop folder
         } else if (mp1 == STLRM_SUBCLDESKFLDR) {
            g.pfn.desktopFolder = WinSubclassWindow(hwnd, newDeskProc);
         // deferred subclassing of the frame of the lauchpad
         } else if (mp1 == STRLM_SUBCLLAUNCHPAD) {
            g.pfn.launchpad = WinSubclassWindow(hwnd, newLnchpProc);
         // hide/unhide all windows command
         } else if (mp1 == STLRM_HIDEALL) {
            // HWND_DESKTOP child but not the frame of the desktop folder
            if ((WinParent(hwnd) == g.hwnd.desktop)
                &&
                (hwnd != g.hwnd.desktopFolder)
                &&
                g.is.locked) {
               // hide all windows command
               if (g.is.hideall) {
                  // if the window is visible and in the current screen
                  // hide it
                  if (isWinInScreen(hwnd) && WinIsWindowVisible(hwnd)) {
                     mSetExFrameFlags(hwnd,
                                      ((WinStyle(hwnd) & WS_MAXIMIZED) ?
                                      FRM_HIDEALLMAX : FRM_HIDEALLREST));
                     WinSetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_MINIMIZE);
                     mSetExFrameFlags(hwnd, FRM_HIDEALL);
                  } /* endif */
               // restore previously minimized windows
               } else {
                  ULONG flag;
                  if (0 != (flag = mIsExFlag(hwnd, FRM_HIDEALL))) {
                     mResetExFrameFlags(hwnd, flag);
                     WinSetWindowPos(hwnd, 0, 0, 0, 0, 0,
                                     ((flag & FRM_HIDEALLMAX) ?
                                      SWP_MAXIMIZE : SWP_RESTORE));
                  } /* endif */
               } /* endif */
            } /* endif */
         // set the dialog font
         } else if (mp1 == STRLM_DLGWINFONT) {
            if (!o.ui.disabled) {
               // get the extra frame flags
               msg = mExFrameFlag(hwnd);
               // if the frame is a dialog window
               if (msg & FRM_INITFONTMASK) {
                  // if the dlg font has not yet been initialized do it now
                  if (msg & FRM_INITFONT)
                     msg = mHasFont(hwnd, &msg) ?
                           FRM_HASOWNFONT: FRM_STLRFONTSET;
                  // if previous font was set or the dialog use its own font
                  // but the override-font option is enabled set a new font
                  if ((msg & FRM_STLRFONTSET) || o.ui.dlg.overridePP) {
                     m_setFont(hwnd, o.ui.dlg.achFont);
                     msg |= FRM_STLRFONTSET;
                  } /* endif */
                  mSetExFrameFlagMask(hwnd, msg, FRM_INITFONTMASK);
               } /* endif */
            } /* endif */
         // unknown parameters
         } else {
            break;
         } /* endif */
         return MRTRUE;

      case WM_CREATE:
dbgPrintf6("* create: %08x (%08x) pid:%d mp(%08x-%08x)\n",
           hwnd, ((PCREATESTRUCT)mp2)->hwndParent, pid, mp1, mp2);
dbgPrintf1("  ");
dbgPrintf1(m_creatParm(mp2)->pszText);
dbgPrintf1("\n");
         // replace system shutdown message boxes
         if (mIsSysShutdownBox(mp2)) {
dbgPrintf2("   ******* isSystemShutdownBox - wait=%u\n", o.sd.wait.closeBox);
            DosSleep(o.sd.wait.closeBox);
            return MRTRUE;
         } /* endif */

         // store the window list window handle
         if (!g.hwnd.winList) g.hwnd.winList = hwnd;
         if (pid) {
            // call the system WC_FRAME procedure
            MRESULT mr = g.cd.frame.pfnwp(hwnd, msg, mp1, mp2);
dbgPrintf4("   creation result:%08x  mp=(%08x-%08x)\n", mr, mp1,mp2);
            if (mr) return mr; // creation error !
            // check if the dialog font must be reset
            if (o.ui.dlg.on
                &&
                (WinQueryWindowUShort(hwnd, QWS_FLAGS) & FF_DIALOGBOX))
               mSetExFrameFlags(hwnd, FRM_INITFONT);
            // WORKPLACE process
            if (g.pid.pmshell == pid) {
dbgPrintf4("   - workplace shell process - winid:%d class:%s (%08x)\n",
           ((PCREATESTRUCT)mp2)->id,
           ((((PCREATESTRUCT)mp2)->pszClass > ((PSZ)0x20000000)) ?
              "WC_*" : ((PCREATESTRUCT)mp2)->pszClass),
              g.hwnd.desktopFolder);
               // if the Dekstop folder frame
               if (mIsDesktopFolderFrame(mp2)) {
                  m_stlrMsgPost(hwnd, STLRM_SUBCLDESKFLDR, 0);
                  g.hwnd.desktopFolder = hwnd;
dbgPrintf1("   - desktop folder\n");
               // if the launchpad frame
               } else if (mIsLaunchPadFrame(hwnd, mp2)) {
                  m_stlrMsgPost(hwnd, STRLM_SUBCLLAUNCHPAD, 0);
dbgPrintf1("   - launchpad\n");
               // system lockup frame
               } else if (mIsSysLockupFrame(hwnd, mp2)) {
dbgPrintf1("   - lockup frame\n");
                  g.is.locked = 1;
                  mSetExFrameFlags(hwnd, FRM_LOCKUP);
               } /* endif */
            } /* endif */
            return MRFALSE;
         } /* endif */
         break;


      case WM_DESTROY:
dbgPrintf2("!!! DESTROY WND: %08x\n", hwnd);
         // if this is the windows list window do power-off or reboot
         if (g.is.hookShutdown && (hwnd == g.hwnd.winList)) {
dbgPrintf3("*** destroy winlist - g.hwnd.shutdown=%08x mode=%c\n",
           g.hwnd.shutdown, g.achsd[0]);
            DosSetPriority(PRTYS_THREAD, PRTYC_TIMECRITICAL, PRTYD_MAXIMUM, 0);
            g.is.replShutdownBox = 1;
            // show the "system shutting down..." message box
            WinSendMsg(g.hwnd.shutdown, STLRM_SHOWSYSCLOSEBOX, MPVOID, MPVOID);
            if (g.achsd[0] == '2') {
               doPowerManagement(APM_POWEROFF);
            } else if (g.achsd[0] == '4') {
               doCtrlAltDel();
            } /* endif */
            DosSetPriority(PRTYS_THREAD, PRTYC_REGULAR, 0, 0);
            DosSleep(o.sd.wait.sysShutdown);
         // lockup window destruction : reset the lockup flag
         } else if (g.is.locked && mIsExFlag(hwnd, FRM_LOCKUP)) {
            g.is.locked = 0;
         } /* endif */
         break;
   } /* endswitch */

   return (MRESULT)g.cd.frame.pfnwp(hwnd, msg, mp1, mp2);
}


/* --------------------------------------------------------------------------
 Subclassed procedure of the frame of the desktop folder.
 The purpose of this is to intercept the shutdown command.
- Parameters -------------------------------------------------------------
 ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT
-------------------------------------------------------------------------- */
MRESULT EXPENTRY newDeskProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   // if enhanced shutdown is enabled post command to the worker thread
   if (mIsDeskFolderShutdownCmd(msg, mp1) && o.sd.gen.on) {
dbgPrintf1("*** shutdown from the desktop\n");
      m_postWorkerMsg(STLRM_SHUTDOWNREQ, g.worker.hwnd, 0);
      return MRFALSE;
   } /* endif */
   return (MRESULT)g.pfn.desktopFolder(hwnd, msg, mp1, mp2);
}


/* --------------------------------------------------------------------------
 Subclassed procedure of the frame of the launchpad window.
 The purpose of this is to intercept the shutdown command.
- Parameters -------------------------------------------------------------
 ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT
-------------------------------------------------------------------------- */
MRESULT EXPENTRY newLnchpProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   // se shutdown esteso attivo posta messaggio alla object window
   if (mIsLaunchpadShutdownCmd(msg, mp1) && o.sd.gen.on) {
dbgPrintf1("*** shutdown from the launchpad\n");
      m_postWorkerMsg(STLRM_SHUTDOWNREQ, g.worker.hwnd, 0);
      return MRFALSE;
   } /* endif */
   return (MRESULT)g.pfn.launchpad(hwnd, msg, mp1, mp2);
}


/* --------------------------------------------------------------------------
 Return TRUE if at least a part of the window is within the screen boundaries.
- Parameters -------------------------------------------------------------
 HWND hwnd : window handle.
- Return value -----------------------------------------------------------
 BOOL TRUE/FALSE (within/outside screen boundaries).
-------------------------------------------------------------------------- */
BOOL isWinInScreen(HWND hwnd) {
   RECTL r;
   WinQueryWindowRect(hwnd, &r);
   WinMapWindowPoints(hwnd, g.hwnd.desktop, (PPOINTL)&r, 1);
   // note:
   // the bottom-left window corner position in screeen cordinates is stored
   // in r.xLeft and r.yBottom, while r.xRight and r.yTop are still the
   // window width and window height (they are not mapped).
   return   (
                  (r.xLeft > -r.xRight)
               && (r.xLeft < g.scr.cx)
               && (r.yBottom > -r.yTop)
               && (r.yBottom < g.scr.cy)
            );
}

//===========================================================================
// Change the image used by FID_SYSMENU and FID_MINMAX.
// Parameters --------------------------------------------------------------
// PSTLRFRAME p : WC_FRAME data.
// Return value ------------------------------------------------------------
// VOID.
//===========================================================================
static
VOID setDefFrameCtrlsImage( HWND hMinMax) {

    _setFrameCtrlBmp( hMinMax, SC_MINIMIZE,
                      (o.ui.tb.bmp.min ? o.ui.tb.bmp.min : g.bmpDef.min));
    _setFrameCtrlBmp( hMinMax, SC_HIDE,
                      (o.ui.tb.bmp.hide ? o.ui.tb.bmp.hide : g.bmpDef.hide));
    _setFrameCtrlBmp( hMinMax, SC_MAXIMIZE,
                      (o.ui.tb.bmp.max ? o.ui.tb.bmp.max : g.bmpDef.max));
    _setFrameCtrlBmp( hMinMax, SC_RESTORE,
                      (o.ui.tb.bmp.rest ? o.ui.tb.bmp.rest : g.bmpDef.rest));
    _setFrameCtrlBmp( hMinMax, SC_CLOSE,
                      (o.ui.tb.bmp.close ? o.ui.tb.bmp.close : g.bmpDef.cls));

}
