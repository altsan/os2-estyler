/* --------------------------------------------------------------------------
 fontdlg.c : show the font dialog.

 2004-6-4 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------



// prototypes ---------------------------------------------------------------
MRESULT EXPENTRY newFontDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// global variables ---------------------------------------------------------

#include "main.h"


/* --------------------------------------------------------------------------

- Parameters -------------------------------------------------------------
 HWND hwnd      : handle of the window whose font must be changed.
 ULONG idsTitle : string ID of the font dialog title.
 ULONG idsSample: string ID of the text to be showed as font sample.
 PSZ pszFont    : in (current font) / out (new selected font).
- Return value -----------------------------------------------------------
 BOOL : TRUE (new font selected) or FALSE (error or no new font selected).
-------------------------------------------------------------------------- */
BOOL fontDlg(HWND hwnd, ULONG idsTitle, ULONG idsSample, PSZ pszFont) {
   FONTDLG fd = {0};
   FONTMETRICS fm = {0};
   CHAR achTitle[128];
   CHAR achSample[128];
   PSZ p;
   BOOL rc = FALSE;
   CHAR buf[140];
   PPRESPARAMS ppp;
   HWND hwndCurFont = NULLHANDLE;
   ULONG pointSize;

   if (!pszFont) return FALSE; // pszFont must be a valid address
   loadString(idsTitle, achTitle);
   loadString(idsSample, achSample);
   fd.cbSize = sizeof(FONTDLG);
   fd.pszFamilyname = pszFont;
   fd.usFamilyBufLen = CCH_FONTDATA;
   fd.pszTitle = achTitle;
   fd.pszPreview = achSample;
   fd.clrFore = SYSCLR_WINDOWTEXT;
   fd.clrBack = SYSCLR_WINDOW;
   // create a dummy window setting the current font as presparm
   if (*pszFont) {
      ppp = (PPRESPARAMS)buf;
      // initialize a PRESPARM structure with the data of the current font
      ppp->aparam[0].id = PP_FONTNAMESIZE;
      ppp->aparam[0].cb = strlen(pszFont) + 1;
      memcpy(ppp->aparam[0].ab, pszFont, ppp->aparam[0].cb);
      ppp->cb = ppp->aparam[0].cb + 8;
      // create a 0-0 size static window and
      hwndCurFont = WinCreateWindow(hwnd, WC_STATIC, NULL, SS_TEXT,
                                 0, 0, 0, 0, hwnd, HWND_BOTTOM, 0, NULL, ppp);
      if (!hwndCurFont) return FALSE;
      fd.hpsScreen = WinGetPS(hwndCurFont);
   } else {
      fd.hpsScreen = WinGetPS(hwnd);
   } /* endif */
   if (!fd.hpsScreen) goto exit_0;
   // subclass the dialog procedure to hide some unneeded controls
   fd.pfnDlgProc = newFontDlgProc;
   if (!GpiQueryFontMetrics(fd.hpsScreen, sizeof(FONTMETRICS), &fm))
      goto exit_1;

   // initialize the FATTRS sub-structure
   fd.fAttrs.usRecordLength = sizeof(FATTRS);
   // initialize the font attributes
   pointSize = strtoul(pszFont, &p, 10);
   fd.fxPointSize = MAKEFIXED(pointSize, 0);
   while (*p) {
      if (!memcmp(p, ".Italic", 7)) {
         fd.fAttrs.fsSelection |= FATTR_SEL_ITALIC; p += 7;
      } else if (!memcmp(p, ".Bold", 5)) {
         fd.fAttrs.fsSelection |= FATTR_SEL_BOLD; p += 5;
      } else if (!memcmp(p, ".Underscore", 11)) {
         fd.fAttrs.fsSelection |= FATTR_SEL_UNDERSCORE; p += 11;
      } else if (!memcmp(p, ".Outline", 8)) {
         fd.fAttrs.fsSelection |= FATTR_SEL_OUTLINE; p += 8;
      } else if (!memcmp(p, ".Strikeout", 10)) {
         fd.fAttrs.fsSelection |= FATTR_SEL_STRIKEOUT; p += 10;
      } else {
         p++;
      } /* endif */
   } /* endwhile */

   fd.fAttrs.lMatch = fm.lMatch;
   fd.fAttrs.idRegistry = fm.idRegistry;
   fd.fAttrs.usCodePage = fm.usCodePage;

   if (!(fm.fsDefn & FM_DEFN_OUTLINE)) {
      fd.fAttrs.lMaxBaselineExt = fm.lMaxBaselineExt;
      fd.fAttrs.lAveCharWidth = fm.lAveCharWidth;
   } // end if
   // font type
   fd.fAttrs.fsType |= fm.fsType & FM_TYPE_KERNING? FATTR_TYPE_KERNING: 0;
   fd.fAttrs.fsType |= fm.fsType & FM_TYPE_DBCS? FATTR_TYPE_DBCS: 0;
   fd.fAttrs.fsType |= fm.fsType & FM_TYPE_MBCS? FATTR_TYPE_MBCS: 0;
   strcpy(fd.fAttrs.szFacename, fm.szFacename);

   // the fontdlg style flags
   fd.fl = FNTS_CENTER | FNTS_INITFROMFATTRS | FNTS_RESETBUTTON;
   fd.usWeight = fm.usWeightClass;
   fd.usWidth = fm.usWidthClass;

   if (!WinFontDlg(HWND_DESKTOP, hwnd, &fd)) goto exit_1;
   if (fd.lReturn == DID_OK) {
      p = pszFont + sprintf(pszFont, "%d.%s", FIXEDINT(fd.fxPointSize),
                            fd.fAttrs.szFacename);
      if (fd.fAttrs.fsSelection & FATTR_SEL_ITALIC)
         memcpy(p, ".Italic", 8), p += 7;
      if (fd.fAttrs.fsSelection & FATTR_SEL_UNDERSCORE)
         memcpy(p, ".Underscore", 12), p += 11;
      if (fd.fAttrs.fsSelection & FATTR_SEL_STRIKEOUT)
         memcpy(p, ".Strikeout", 11), p += 10;
      if (fd.fAttrs.fsSelection&FATTR_SEL_BOLD)
         memcpy(p, ".Bold", 6), p += 6;
      if (fd.fAttrs.fsSelection&FATTR_SEL_OUTLINE)
         memcpy(p, ".Outline", 9), p += 8;
      rc = TRUE;
   } /* endif */
exit_1:
   if (fd.hpsScreen) WinReleasePS(fd.hpsScreen);
exit_0:
   if (hwndCurFont) WinDestroyWindow(hwndCurFont);
   return rc;
}


/* --------------------------------------------------------------------------
 Custom font dialog procedure. This just hides the DID_DISPLAY_FILTER and
 DID_PRINTER_FILTER controls.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY newFontDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if (msg == WM_INITDLG) {
      DlgItemShow(hwnd, DID_DISPLAY_FILTER, FALSE);
      DlgItemShow(hwnd, DID_PRINTER_FILTER, FALSE);
   } // end if
   return WinDefFontDlgProc(hwnd, msg, mp1, mp2);
}
