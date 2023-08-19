/* --------------------------------------------------------------------------
 pageProdInfo.c : product information dialog procedure.

 2004-6-4 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------



// prototypes ---------------------------------------------------------------
static BOOL onDlgInit(HWND hwnd);
static VOID onCmdMsg(HWND hwnd, ULONG id);

// global variables ---------------------------------------------------------

/* --------------------------------------------------------------------------
 Product information dialog procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY prodInfoPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         onDlgInit(hwnd);
         break;
      case WM_COMMAND:
         onCmdMsg(hwnd, (ULONG)mp1);
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}

/* --------------------------------------------------------------------------
 initialization.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL onDlgInit(HWND hwnd) {
   STLRVER version;
   INT i;
   initPage(hwnd);
   mResetStaticIcon(hwnd, ICO_PRODINFO, g.mode + 2);
   version = *(stlrGetVer());
   setCtrlTextParm(hwnd, TXT_PIVERSION, IDS_VERSION,
                   version.major, version.minor, version.build);
   for (i = BTN_APPLY; i <= BTN_PREVIEW; ++i)
      WinEnableControl(g.appl.hwnd, i, FALSE);

   if (g.pCurOpts->ui.dlg.on) {
      emboldenCtrlText(hwnd, TXT_PIPRODUCT, g.pCurOpts->ui.dlg.achFont);
      underlineCtrlText(hwnd, BTN_PIURL, g.pCurOpts->ui.dlg.achFont);
      WinSendMsg(WinWindowFromID(hwnd, BTN_PIURL), BM_AUTOSIZE, 0, 0);
   }
   return TRUE;
}


/* --------------------------------------------------------------------------
 Process command events.
- Parameters -------------------------------------------------------------
 HWND hwnd : window handle
 ULONG id  : button ID.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID onCmdMsg(HWND hwnd, ULONG id) {
   CHAR achExe[CCHMAXPATH];
   CHAR achPath[CCHMAXPATHCOMP];
   CHAR achUrl[256];
   if ((id == BTN_PIURL)
       &&
       WinQueryDlgItemText(hwnd, BTN_PIURL, sizeof(achUrl), achUrl)
       &&
       PrfQueryProfileString(HINI_USER, "WPURLDEFAULTSETTINGS",
                             "DefaultBrowserExe", "", achExe, sizeof(achExe))
       &&
       PrfQueryProfileString(HINI_USER, "WPURLDEFAULTSETTINGS",
                             "DefaultWorkingDir", "", achPath, sizeof(achPath)))
      gpStartApp(hwnd, achExe, achUrl, achPath, NULL, GPSTART_DEFAULT);
}
