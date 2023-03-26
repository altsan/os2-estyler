/* --------------------------------------------------------------------------
 pmctrls.c : superclassed PM controls.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"
#include "xComboBox.h"

// definitions --------------------------------------------------------------
#define mSetColor(_hwnd_, _color_) \
   ((BOOL)WinSetWindowULong(_hwnd_, cbBtnData, (ULONG)(_color_)))

#define mQueryColor(_hwnd_) \
   ((LONG)WinQueryWindowULong(_hwnd_, cbBtnData))

// prototypes ---------------------------------------------------------------
MRESULT EXPENTRY xcombo2Proc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY xClrBtnProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID btnPaint(HWND hwnd);

// global variables ---------------------------------------------------------
PFNWP pfnComboBox;
PFNWP pfnBtn;
ULONG cbBtnData;

// --------------------------------------------------------------------------

/* --------------------------------------------------------------------------
 Register the WC_XCOMBOBOX2 class.
- Parameters -------------------------------------------------------------
 HAB hab : anchor block handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
BOOL xComboBoxRegister(HAB hab) {
   CLASSINFO ci;
   if (WinQueryClassInfo(NULLHANDLE, WC_COMBOBOX, &ci)) {
      pfnComboBox = ci.pfnWindowProc;
      return WinRegisterClass(hab, WC_XCOMBOBOX2, xcombo2Proc,
                              ci.flClassStyle & ~CS_PUBLIC, ci.cbWindowData);
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Register the WC_CLRBUTTON class.
- Parameters -------------------------------------------------------------
 HAB hab : anchor block handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
BOOL xClrButtonRegister(HAB hab) {
   CLASSINFO ci;
   if (WinQueryClassInfo(NULLHANDLE, WC_BUTTON, &ci)) {
      pfnBtn = ci.pfnWindowProc;
      cbBtnData = ci.cbWindowData;
      return WinRegisterClass(hab, WC_CLRBUTTON, xClrBtnProc,
                              ci.flClassStyle & ~CS_PUBLIC, cbBtnData + 4);
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Enhanced combination box procedure
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY xcombo2Proc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   PSZ     pStart, pEnd, pText;
   MRESULT mr;

   if (msg == WM_CHAR) {
      if (!WinIsWindowEnabled(hwnd))
         return 0;
   }

   // send all msgs to the default wndproc,
   // then exit if the current msg is not WM_CREATE 
   mr = pfnComboBox(hwnd, msg, mp1, mp2);
   if (msg != WM_CREATE || mr)
      return mr;

   // if any text was defined in the CREATESTRUCT use it to fill the listbox
   pText = ((PCREATESTRUCT)mp2)->pszText;
   if (pText && *pText) {

      // this is due to the implementation of strchr in IBM visual AGE
      for (pStart = pText, pEnd = pText; *pStart; ++pEnd)
      {
         if (!*pEnd) {
            pfnComboBox(hwnd, LM_INSERTITEM, (MPARAM)LIT_END, (MPARAM)pStart);
            break;
         }

         if (*pEnd == '\t') {
            *pEnd = 0;
            pfnComboBox(hwnd, LM_INSERTITEM, (MPARAM)LIT_END, (MPARAM)pStart);
            *pEnd = '\t';
            pStart = ++pEnd;
         }
      }
   }

   return 0;
}

/* --------------------------------------------------------------------------
 Color button procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY xClrBtnProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_SETFOCUS: {
         MRESULT mr = pfnBtn(hwnd, msg, mp1, mp2);
         if (mp2 && !(WinStyle(hwnd) & BS_NOPOINTERFOCUS)) {
            RECTL r;
            WinQueryWindowRect(hwnd, &r);
            RectInflate(&r, -4, -4);
            WinShowCursor(hwnd, FALSE);
            WinCreateCursor(hwnd, r.xLeft, r.yBottom, r.xRight - 4, r.yTop - 4,
                            CURSOR_FRAME | CURSOR_HALFTONE, NULL);
            WinShowCursor(hwnd, TRUE);
         } /* endif */
         return mr;
      }
      case WM_PAINT:
         pfnBtn(hwnd, msg, mp1, mp2);
         btnPaint(hwnd);
         break;
      case BM_SETHILITE:
      case BM_SETDEFAULT: {
         MRESULT mr = pfnBtn(hwnd, msg, mp1, mp2);
         btnPaint(hwnd);
         return mr;
      }
      case WM_PRESPARAMCHANGED:
         if (((ULONG)mp1 == PP_BACKGROUNDCOLOR)
             ||
             ((ULONG)mp1 == PP_BACKGROUNDCOLORINDEX)) {
            LONG lColor;
            if (WinQueryPresParam(hwnd,
                        PP_BACKGROUNDCOLOR, PP_BACKGROUNDCOLORINDEX,
                        NULL, sizeof(LONG), &lColor,
                        QPF_NOINHERIT | QPF_PURERGBCOLOR |QPF_ID2COLORINDEX)) {
               mSetColor(hwnd, lColor);
               btnPaint(hwnd);
               WinSendMsg(WinOwner(hwnd), WM_CONTROL,
                          MPFROM2SHORT(WinID(hwnd), BN_COLORCHANGED),
                          (MPARAM)lColor);
            } /* endif */
         } else {
            pfnBtn(hwnd, msg, mp1, mp2);
         } /* endif */
         break;
      case BM_SETCOLOR:
         msg = (ULONG)mSetColor(hwnd, (LONG)mp1);
         btnPaint(hwnd);
         WinSendMsg(WinOwner(hwnd), WM_CONTROL,
                    MPFROM2SHORT(WinID(hwnd), BN_COLORCHANGED), mp1);
         return (MRESULT)msg;
      case BM_QUERYCOLOR:
         return (MRESULT)mQueryColor(hwnd);
      case WM_SETWINDOWPARAMS:
         return MRTRUE;
      case WM_CREATE:
         // ignore text
         ((PCREATESTRUCT)mp2)->pszText = NULL;
         // ignore BS_PRIMARYSTYLES
         WinSetWindowBits(hwnd, QWL_STYLE, 0, ~BCLRS_VALIDSTYLES);
         return pfnBtn(hwnd, msg, mp1, mp2);
      default :
         return pfnBtn(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}


/* --------------------------------------------------------------------------
 Paint the text "..." and a color rectangle.
- Parameters -------------------------------------------------------------
 HWND hwnd : color button window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID btnPaint(HWND hwnd) {
   HPS hps;
   RECTL r;
   BOOL bFocus = (hwnd == WinQueryFocus(HWND_DESKTOP))
                 &&
                 !(WinStyle(hwnd) & BS_NOPOINTERFOCUS);
   if (bFocus) WinShowCursor(hwnd, FALSE);
   if (WinIsWindowEnabled(hwnd) && (NULLHANDLE != (hps = WinGetPS(hwnd)))) {
      GpiCreateLogColorTable(hps, 0, LCOLF_RGB, 0, 0, NULL);
      WinQueryWindowRect(hwnd, &r);
      RectInflate(&r, -4, -4);
      if (pfnBtn(hwnd, BM_QUERYHILITE, MPVOID, MPVOID))
         RectShift(&r, 1, -1);
      WinDrawBorder(hps, &r, 1, 1, 0, 0, 0);
      RectInflate(&r, -1, -1);
      WinFillRect(hps, &r, mQueryColor(hwnd));
      WinReleasePS(hps);
   } /* endif */
   if (bFocus) WinShowCursor(hwnd, TRUE);
}

