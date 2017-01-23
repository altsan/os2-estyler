//===========================================================================
// menu.c : enhanced WC_MENU procedure
//===========================================================================


#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "main.h"
#include "frame.h"


// globals ------------------------------------------------------------------

extern PROCDATA pd;


//===========================================================================
// Enhaced WC_MENU window procedure.
// Parameters --------------------------------------------------------------
// Ordinary window procedure parameters.
// Return value ------------------------------------------------------------
// MRESULT.
//===========================================================================

MRESULT EXPENTRY stlrMenuProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {

    switch (msg) {

    // note: interception of WM_CREATE is not useful as all menus are created
    //       with the same style and the FID_MENU id.
    //       The real menu id, for the frame controls, is set just before the
    //       first MM_INSERTITEM message.

    //case WM_CREATE:
    //   /*-*/ dbgPrintf6("->menu creation : %08x "
    //                    "(parent: %08x, style: %08x, id: %08x, data: %08x)\n",
    //                    hwnd,
    //                    ((PCREATESTRUCT)mp2)->hwndParent,
    //                    ((PCREATESTRUCT)mp2)->flStyle,
    //                    ((PCREATESTRUCT)mp2)->id,
    //                    ((PCREATESTRUCT)mp2)->pCtlData);
    //   break;

    //case MM_DELETEITEM:
    //   /*-*/ dbgPrintf3("MM_DELETEITEM %08x id: %04x\n", hwnd, SHORT1FROMMP(mp1));
    //   break;
    //===========================================================================
    // menu item insertion
    // if hwnd is the handle of the FID_MINMAX substitutes the bitmap handle

    case MM_INSERTITEM:
        //   /*-*/ dbgPrintf4("MM_INSERTITEM %08x pos %d id %04x\n",
        //                    hwnd, ((PMENUITEM)mp1)->iPosition, ((PMENUITEM)mp1)->id);
        if (!o.ui.disabled
                //&& pd.ppib
                //&& !pd.we.tbar
                && o.ui.tb.on
                && (FID_MINMAX == WinID(hwnd))) {
            switch (((PMENUITEM)mp1)->id) {
            case SC_RESTORE:
                if (o.ui.tb.bmp.rest)
                    ((PMENUITEM)mp1)->hItem = o.ui.tb.bmp.rest;
                break;
            case SC_MINIMIZE:
                if (o.ui.tb.bmp.min)
                    ((PMENUITEM)mp1)->hItem = o.ui.tb.bmp.min;
                break;
            case SC_MAXIMIZE:
                if (o.ui.tb.bmp.max)
                    ((PMENUITEM)mp1)->hItem = o.ui.tb.bmp.max;
                break;
            case SC_HIDE:
                if (o.ui.tb.bmp.hide)
                    ((PMENUITEM)mp1)->hItem = o.ui.tb.bmp.hide;
                break;
            case SC_CLOSE:
                dbgPrintf3("MM_INSERTITEM %08x CLOSE %x\n",
                           hwnd, o.ui.tb.bmp.close);
                if (o.ui.tb.bmp.close)
                    ((PMENUITEM)mp1)->hItem = o.ui.tb.bmp.close;
                // move the close button in win32 windows
                //if (o.ui.tb.close == 3)
                //    ((PMENUITEM)mp1)->iPosition = MIT_END;
                break;
            } /* endswitch */
        } /* endif */
        break;


        //===========================================================================
        // con warp 4 impedisce che il bottone di chiusura della finestra venga
        // reinserito quando Š disabilitato o spostato a sinistra
#if 0
    case MM_ISITEMVALID:
        if ((SHORT1FROMMP(mp1) == SC_CLOSE)
                //&& !pd.we.tbar
                && !o.ui.disabled
                && o.ui.tb.on
                && (WinID(hwnd) == FID_MINMAX)
                && _getFrameData(hwnd, p)
                //&& ((o.ui.tb.close < 2) || p->is.skipuf)
                )
            return (MRESULT)TRUE;
        break;
#endif

        //===========================================================================
        // memorizza coordinata y barra menu

        //case WM_ADJUSTWINDOWPOS:
        //   // memorizza coordinata y FID_MENU
        //   if ((((PSWP)mp1)->fl & SWP_MOVE)
        //       && !pd.we.tbarhide
        //       && _getFrameData(hwnd, p)
        //       && (p->hMenu == hwnd)) {
        //      // a trick to avoid allocating another variable on the stack
        //      msg = (ULONG)g.cd.menu.pfnwp(hwnd, WM_ADJUSTWINDOWPOS, mp1, mp2);
        //      p->yMenu = ((PSWP)mp1)->y;
        //      return (MRESULT)msg;
        //   } /* endif */
        //   break;


        //===========================================================================

#if 0
    case WM_DESTROY:
        /*-*/ dbgPrintf2("menu destruction -> %08x\n", hwnd);
        if (_getFrameData(hwnd, p)) {
            if (hwnd == p->hSMenu) {
                p->hSMenu = 0;
                p->ffl &= ~FCF_SYSMENU;
                //if (p->is.minMaxAdd)
                //    WinDestroyWindow(p->hMinMax), p->hMinMax = 0;
            } else if (hwnd == p->hMinMax) {
                p->hMinMax = 0;
                p->ffl &= ~(FCF_MINMAX | FCF_HIDEBUTTON | FCF_CLOSEBUTTON);
                //p->is.minMaxAdd = 0;
            } else if (hwnd == p->hMenu) {
                p->ffl &= ~FCF_MENU;
                p->hMenu = 0;
                p->yMenu = 0;
            } /* endif */
        } /* endif */
        break;
#endif

        //===========================================================================
        // setta flag mouse sopra menu, se non Š actionbar setta flag per impedire
        // perdita focus all'owner del menu

#if 0
    case WM_MOUSEMOVE:
        if (g.is.onctrl != OCTLMENU) {
            g.is.onctrl = OCTLMENU;
            _resetHideMouTimer();
        } /* endif */
        break;
#endif

        //===========================================================================
        // system default WC_MENU procedure :

    } /* endswitch */

    return (MRESULT)g.cd.menu.pfnwp(hwnd, msg, mp1, mp2);
}
