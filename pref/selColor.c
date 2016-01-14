/* --------------------------------------------------------------------------
 selColor.c : color selection dialog.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------
typedef struct {
   ULONG state;
   HWND hNotify;
   CLR lColor;
   CLR lColorUndo;
} CLRSELDLG, * PCLRSELDLG;

#define IGNORESPINEVENTS      0x0001

// prototypes ---------------------------------------------------------------
VOID setRGBValues(HWND hwnd, PCLRSELDLG p);
VOID onRGBValueChange(HWND hDlg, HWND hSpin, ULONG ulEvent,
                      PCLRSELDLG p, PUCHAR pClrComp);
MRESULT EXPENTRY selClrDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);


/* --------------------------------------------------------------------------
 Display the color selection dialog.
- Parameters -------------------------------------------------------------
 HWND hOwner : owner window handle.
 LONG lColor : initial color.
- Return value -----------------------------------------------------------
 LONG : selected color or CLR_ERROR in case of error.
-------------------------------------------------------------------------- */
LONG editColorDlg(HWND hOwner, LONG lColor) {
   CLRSELDLG csd;
   HLIB hlib;
   HAB hab;
   csd.state = 0;
   csd.hNotify = hOwner;
   csd.lColor.l = csd.lColorUndo.l = lColor;
   // register the color wheel control and open the dialog
   hab = WinQueryAnchorBlock(hOwner);
   if (NULLHANDLE == (hlib = WinLoadLibrary(hab, "WPCONFIG.DLL")))
      return CLR_ERROR;
   lColor = (LONG)WinDlgBox(HWND_DESKTOP, hOwner, selClrDlgProc, g.hResLib,
                            DLG_CWHEEL, &csd);
   WinDeleteLibrary(hab, hlib);
   return lColor;
}


/* --------------------------------------------------------------------------
 Update the spin buttons showing the RGB values.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
 PCLR pClr : color value.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID setRGBValues(HWND hwnd, PCLRSELDLG p) {
   p->state |= IGNORESPINEVENTS;
   dSpinBtnSetValue(hwnd, SPN_CLRWRED, p->lColor.rgb .red);
   dSpinBtnSetValue(hwnd, SPN_CLRWGREEN, p->lColor.rgb .grn);
   dSpinBtnSetValue(hwnd, SPN_CLRWBLUE, p->lColor.rgb .blu);
   p->state &= ~IGNORESPINEVENTS;
}


/* --------------------------------------------------------------------------
 Handle RGB value changes performed via the spin buttons.
- Parameters -------------------------------------------------------------
 HWND hDlg       : dialog window handle.
 HWND hSpin      : spinbutton window handle.
 ULONG ulEvent   : spinbutton event.
 PCLRSELDLG p    : color selection dialog data.
 PUCHAR pClrComp : red/green/blue value being updated.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID onRGBValueChange(HWND hDlg, HWND hSpin, ULONG ulEvent,
                      PCLRSELDLG p, PUCHAR pClrComp) {
   LONG lValue = (LONG)*pClrComp;
   if (!(p->state & IGNORESPINEVENTS)
       &&
       handleSpinbtnEvent(hSpin, ulEvent, &lValue, (LONG)*pClrComp)) {
      *pClrComp = (UCHAR)lValue;
      WinSendDlgItemMsg(hDlg, CWHEEL, CWM_COLORWHEELSETVAL,
                        (MPARAM)p->lColor.l, MPVOID);
      // notify the new color to the owner (the dialog owner is a
      // WC_CLRBUTTON window - see /headers/clrButton.h)
      wClrBtnColorSet(p->hNotify, p->lColor.l);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Color selection dialog procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY selClrDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PCLRSELDLG p;
   switch (msg) {
   // dialog initialization ------------------------------------------------
      case WM_INITDLG:
         WinSetWindowPtr(hwnd, 0L, mp2);
         p = (PCLRSELDLG)mp2;
         setRGBValues(hwnd, p);
         // initialize the color wheel control
         WinSendDlgItemMsg(hwnd, CWHEEL, CWM_COLORWHEELSETVAL,
                           (MPARAM)p->lColor.l, MPVOID);
         break;
   // a new color has been set through the color wheel ---------------------
      case WM_COLORWHEELCHANGED:
         if (NULL != (p = WinQueryWindowPtr(hwnd, 0L))) {
            p->lColor.l = (LONG)mp1;
            setRGBValues(hwnd, p);
            // notify the new color to the owner (the dialog owner is a
            // WC_CLRBUTTON window - see /headers/clrButton.h)
            wClrBtnColorSet(p->hNotify, p->lColor.l);
         } /* endif */
         break;
   // spinbutton value has changed -----------------------------------------
      case WM_CONTROL:
         if (NULL != (p = WinQueryWindowPtr(hwnd, 0L))) {
            switch (SHORT1FROMMP(mp1)) {
               case SPN_CLRWRED:
                  onRGBValueChange(hwnd, (HWND)mp2, SHORT2FROMMP(mp1),
                                   p, &p->lColor.rgb .red);
                  break;
               case SPN_CLRWGREEN:
                  onRGBValueChange(hwnd, (HWND)mp2, SHORT2FROMMP(mp1),
                                   p, &p->lColor.rgb .grn);
                  break;
               case SPN_CLRWBLUE:
                  onRGBValueChange(hwnd, (HWND)mp2, SHORT2FROMMP(mp1),
                                   p, &p->lColor.rgb .blu);
                  break;
            } /* endswitch */
         } /* endif */
         break;
   // user clicked on a PUSHBUTTON -----------------------------------------
      case WM_COMMAND:
         if (NULL != (p = WinQueryWindowPtr(hwnd, 0L))) {
            switch ((USHORT)mp1) {
               case BTN_CLRWOK:            // OK: return current color
                  WinDismissDlg(hwnd, p->lColor.l);
                  break;
               case BTN_CLRWUNDO:          // UNDO: restore start color
                  p->lColor.l = p->lColorUndo.l;
                  setRGBValues(hwnd, p);
                  WinSendDlgItemMsg(hwnd, CWHEEL, CWM_COLORWHEELSETVAL,
                                    (MPARAM)p->lColor.l, MPVOID);
                  break;
               case BTN_CLRWCANCEL:        // CANCEL: return start color
                  WinDismissDlg(hwnd, p->lColorUndo.l);
                  break;
            } /* endswitch */
         } /* endif */
         break;
   // user closed the "edit color" dialog window ---------------------------
      case WM_CLOSE:
         selClrDlgProc(hwnd, WM_COMMAND, (MPARAM)BTN_CLRWCANCEL, MPVOID);
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}
