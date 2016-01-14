/* --------------------------------------------------------------------------
 hooks.c : input hook procedure to intercept the Windows key events

 2004-05-12 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */


#pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------
// first message parameter of valid windows keys events
#define MP1_LEFTWINKEY      0x7e010144
#define MP1_RIGHTWINKEY     0x7f010144
#define MP1_MENUKEY         0x7c010144

// prototypes ---------------------------------------------------------------
VOID processWkey(ULONG fl, HWND hwnd);
VOID eCenterECSmenu(BOOL bottomCenter);
VOID eCenterTrayList(VOID);
VOID eCenterTrayNext(VOID);
VOID eCenterSwitchToTray(ULONG iTray);
VOID eCenterZorder(VOID);
BOOL eCenterHandle(ULONG fl, PHWND phwnd);
HWND eCenterTrayHandle(VOID);

/* --------------------------------------------------------------------------
 Input hook procedure: intercept windows keys events.
- Parameters -------------------------------------------------------------
 Ordinary input hook parameters.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (don't pass/ pass the message to the next hook).
-------------------------------------------------------------------------- */
BOOL EXPENTRY postMsgHook(HAB hab, PQMSG pqmsg, USHORT fs) {
   if (g.is.locked || !o.wk.on) return FALSE;
   if (pqmsg->msg == WM_CHAR) {
      switch ((ULONG)pqmsg->mp1 & ~KC_TOGGLE) {
         case MP1_LEFTWINKEY:
            if (o.wk.left) processWkey(o.wk.left, pqmsg->hwnd);
            break;
         case MP1_RIGHTWINKEY:
            if (o.wk.right) processWkey(o.wk.right, pqmsg->hwnd);
            break;
         case MP1_MENUKEY:
            if (o.wk.menu) processWkey(o.wk.menu, pqmsg->hwnd);
            break;
      } /* endswitch */
   } /* endif */
   return FALSE;
}

/* --------------------------------------------------------------------------
 Execute the function associated with the current windows key.
- Parameters -------------------------------------------------------------
 ULONG flag : action to be performed (see ..\headers\definitions.h").
 HWND hwnd  : handle of the window which intercepted the windows key event.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID processWkey(ULONG fl, HWND hwnd) {
   g.hwnd.wkey = hwnd;
   switch (fl) {
      case IWKEY_HIDEWIN:                       // hide all windows
         g.is.hideall = !g.is.hideall;
         m_stlrMsgBroadcast(STLRM_HIDEALL, 0);
         break;
      case IWKEY_WINLIST:                       // show/hide the window list
         if (WinQueryWindowULong(g.hwnd.winList, QWL_STYLE) & WS_VISIBLE) {
            WinShowWindow(g.hwnd.winList, FALSE);
         } else {
            WPOSSIZE wrcl;
            WinQueryWindowRect(g.hwnd.winList, (PRECTL)&wrcl);
            WinQueryPointerPos(g.hwnd.desktop, (PPOINTL)&wrcl);
            wrcl.x -= wrcl.cx / 2;
            wrcl.y -= wrcl.cy / 2;
            if (wrcl.x < 0)
               wrcl.x = 0;
            else if ((wrcl.x + wrcl.cx) > g.scr.cx)
               wrcl.x = g.scr.cx - wrcl.cx;
            if (wrcl.y < 0)
               wrcl.y = 0;
            else if ((wrcl.y + wrcl.cy) > g.scr.cy)
               wrcl.y = g.scr.cy - wrcl.cy;
            WinSetWindowPos(g.hwnd.winList, 0, wrcl.x, wrcl.y, 0, 0, SWP_MOVE);
            WinSetWindowPos(g.hwnd.winList, HWND_TOP, 0, 0, 0, 0,
                            SWP_SHOW | SWP_ACTIVATE | SWP_ZORDER);
         } /* endif */
         break;
      case IWKEY_CTXTMENU:
         WinPostMsg(hwnd, WM_CONTEXTMENU, MPVOID, (MPARAM)0x00010000);
         break;
      case IWKEY_LOCKUP:
         m_postWorkerMsg(STLRM_LOCKUPSYS, g.worker.hwnd, 0);
         break;
      case IWKEY_FLDDRIVE:
         m_setWpsObject(OBJID_DRIVES, OBJ_OPENSINGLE);
         break;
      case IWKEY_FLDSYSSETUP:
         m_setWpsObject(OBJID_SYSSETUP, OBJ_OPENSINGLE);
         break;
      case IWKEY_OS2CMD:
         m_setWpsObject(OBJID_OS2COMMAND, OBJ_OPENDEF);
         break;
      case IWKEY_ECTOPECSMENU:
      case IWKEY_ECBOTTOMECSMENU:
         eCenterECSmenu(fl - IWKEY_ECTOPECSMENU);
         break;
      case IWKEY_ECTRAYLIST:
         eCenterTrayList();
         break;
      case IWKEY_ECNEXTTRAY:
         eCenterTrayNext();
         break;
      case IWKEY_ECTRAY1:
      case IWKEY_ECTRAY2:
      case IWKEY_ECTRAY3:
      case IWKEY_ECTRAY4:
      case IWKEY_ECTRAY5:
      case IWKEY_ECTRAY6:
      case IWKEY_ECTRAY7:
      case IWKEY_ECTRAY8:
      case IWKEY_ECTRAY9:
      case IWKEY_ECTRAY10:
         eCenterSwitchToTray(fl - IWKEY_ECTRAY1);
         break;
      case IWKEY_ECZORDER:
         eCenterZorder();
         break;
      default:
         m_stlrMsgPost(g.hwnd.warpCenter, STLRM_WCCMD, fl);
         break;
   } /* endswitch */
}


/* --------------------------------------------------------------------------
 Show the menu of the eCenter special eCS widget (the leftmost widget):
 - get the handle of the bottom or top eCenter,
 - get the handle of the leftmost widget,
 - emulate a mouse button 1 click on the leftmost widget.
- Parameters -------------------------------------------------------------
 BOOL : TRUE/FALSE (bottom/top eCenter).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID eCenterECSmenu(BOOL bottomCenter) {
   HWND hwndCenter, hwndWidget;
   RECTL r;
   if (eCenterHandle(bottomCenter, &hwndCenter)
       &&
       (NULLHANDLE != (hwndCenter = WinWindowFromID(hwndCenter, FID_CLIENT)))) {
      WinQueryWindowRect(hwndCenter, &r);
      r.xLeft = r.yBottom = r.yTop / 2;
      hwndWidget = WinWindowFromPoint(hwndCenter, (PPOINTL)&r, FALSE);
      if (hwndWidget) {
         WinPostMsg(hwndWidget, WM_BUTTON1DOWN, MPVOID, MPVOID);
         WinPostMsg(hwndWidget, WM_BUTTON1UP, MPVOID, MPVOID);
      } /* endif */
   } /* endif */
}


/* --------------------------------------------------------------------------
 Show the eCenter tray list.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID eCenterTrayList(VOID) {
   HWND hwndTray;
   if (NULLHANDLE != (hwndTray = eCenterTrayHandle())) {
      WinPostMsg(hwndTray, WM_BUTTON1DOWN, MPVOID, MPVOID);
      WinPostMsg(hwndTray, WM_BUTTON1UP, MPVOID, MPVOID);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Switch to the next eCenter tray.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID eCenterTrayNext(VOID) {
   HWND hwndTray;
   RECTL r;
   if (NULLHANDLE != (hwndTray = eCenterTrayHandle())) {
      WinQueryWindowRect(hwndTray, &r);
      WinPostMsg(hwndTray, WM_BUTTON1CLICK, MPFROM2SHORT(r.xRight, 0), MPVOID);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Switch to the tray index iTray.
- Parameters -------------------------------------------------------------
 ULONG iTray : tray index (zero based).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID eCenterSwitchToTray(ULONG iTray) {
   HWND hwndTray;
   if (NULLHANDLE != (hwndTray = eCenterTrayHandle()))
      WinPostMsg(hwndTray, WM_COMMAND, (MPARAM)(0x64 + iTray), MPVOID);
}


/* --------------------------------------------------------------------------
 Toggle the eCenter Z-order to the bottom/top.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID eCenterZorder(VOID) {
   HWND aHwndCenter[2];
   HWND hwndZorder;
   hwndZorder = g.is.eCenterTop ? HWND_TOP : HWND_BOTTOM;
   g.is.eCenterTop = !g.is.eCenterTop;
   if (eCenterHandle(2, aHwndCenter)) {
      WinSetWindowPos(aHwndCenter[0], hwndZorder, 0, 0, 0, 0, SWP_ZORDER);
      if (aHwndCenter[1])
         WinSetWindowPos(aHwndCenter[1], hwndZorder, 0, 0, 0, 0, SWP_ZORDER);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Get the handle(s) of the top eCenter, the bottom eCenter or both.
- Parameters -------------------------------------------------------------
 ULONG fl    : 0 (top eCenter), 1 (bottom eCenter) 2 (both eCenters).
 PHWND phwnd : (output) handles of the eCenter(s).
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL eCenterHandle(ULONG fl, PHWND phwnd) {
   HENUM henum;
   HWND hwndNext;
   SWP swp;
   ULONG cbClassName;
   CHAR achClass[16];
   ULONG iHwnd;
   phwnd[0] = NULLHANDLE;
   if (fl == 2) phwnd[1] = NULLHANDLE;
   iHwnd = 0;
   henum = WinBeginEnumWindows(HWND_DESKTOP);
   while (NULLHANDLE != (hwndNext = WinGetNextWindow(henum))) {
      if ((14 == WinQueryClassName(hwndNext, sizeof(achClass), achClass))
          &&
          !memcmp(achClass, "XWPCenterFrame", 15)
          &&
          WinQueryWindowPos(hwndNext, &swp)) {
         if (!fl) {                              // top eCenter
            if (swp.y) *phwnd = hwndNext;
            break;
         } else if (fl == 1) {                   // bottom eCenter
            if (!swp.y) *phwnd = hwndNext;
            break;
         } else {                                // both eCenters
            phwnd[iHwnd++] = hwndNext;
            if (iHwnd == 2) break;
         } /* endif */
      } /* endif */
   } /* endwhile */
   WinEndEnumWindows(henum);
   return (BOOL)*phwnd;
}


/* --------------------------------------------------------------------------
 Get the handle of the eCenter Tray widget.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 HWND : handle of the eCenter tray widget or NULLHANDLE in case of error.
-------------------------------------------------------------------------- */
static HWND eCenterTrayHandle(VOID) {
   CHAR achClass[20];
   HWND aHwndCenter[2], hwndNext;
   HENUM henum;
   INT i;
   eCenterHandle(2, aHwndCenter);
   for (i = 0; aHwndCenter[i]; ++i) {
      aHwndCenter[i] = WinWindowFromID(aHwndCenter[i], FID_CLIENT);
      if (aHwndCenter[i]) {
         henum = WinBeginEnumWindows(aHwndCenter[i]);
         while (NULLHANDLE != (hwndNext = WinGetNextWindow(henum))) {
            if ((19 == WinQueryClassName(hwndNext, sizeof(achClass), achClass))
                &&
                !memcmp(achClass, "XWPCenterTrayWidget", 20))
               break;
         } /* endwhile */
         WinEndEnumWindows(henum);
         if (hwndNext) return hwndNext;
      } /* endif */
   } /* endfor */
   return NULLHANDLE;
}
