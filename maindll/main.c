/* --------------------------------------------------------------------------
 main.c : dll entry point, initialization, termination procedures.
 2004/05/08 - Alessandro Cantatore - Italy

-------------------------------------------------------------------------- */

#pragma strings(readonly)

// includes -----------------------------------------------------------------

#include "main.h"
#include "stlrExport.h"
#include "error.h"

// prototypes ---------------------------------------------------------------
static BOOL initialize(ULONG hmod);
static BOOL terminate(VOID);
static BOOL keyPressedToAbortLoading(VOID);
static VOID readProfile(VOID);
static VOID setUserInterfaceDefaults(VOID);
static BOOL cacheSysVals(VOID);
static BOOL stlrQueryScreenCaps(PSCRRES pScreenData);
static VOID handleColorDepthChange(HINI hini, ULONG cclrs);
static VOID migrateImageName(HINI hini, PSZ pOldApp, PSZ pNewApp, PSZ pKey, PSZ pBuf);
static VOID migrateImageData(HINI hini, PSZ pOldApp, PSZ pNewApp, PSZ pKey, PSZ pBuf);
static BOOL getDefFrameCtlsBmpHnds(VOID);
static BOOL superclassMainPMclasses(VOID);
static BOOL stlrSuperclass(PSZ pszClass, PCLASSDATA pcd, PFNWP pfnwpNew,
                    LONG cbWin, ULONG id1, ULONG id2);
static TID initWorkerThreads(VOID);
static BOOL initHooks(HMODULE hmod);
static VOID checkWarpCenterClass(VOID);

// global variables ---------------------------------------------------------
GLOBAL g;
OPTIONS o;

#ifdef DEBUGGING    // when DEBUGGING is defined debug information is written
   HFILE hf;        // to a log file
#endif

// entry point --------------------------------------------------------------

#pragma entry(eStylerInitTerm)

#ifdef __KLIBC__
#define eStylerInitTerm _DLL_InitTerm
#endif

/* --------------------------------------------------------------------------
 DLL initialization/termination routine
- Parameters -------------------------------------------------------------
 ULONG hmod : DLL module handle.
 ULONG flag : 0/1 (initialization/termination)
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
ULONG _System eStylerInitTerm(ULONG hmod, ULONG flag) {
   switch (flag) {
      case 0:
         return (ULONG)initialize(hmod);
      case 1:
         return (ULONG)terminate();
      default:
         return 0L;
   } /* endswitch */
   return 1L;
}


/* --------------------------------------------------------------------------
 DLL initialization routine.
- Parameters -------------------------------------------------------------
 ULONG hmod : current DLL module handle
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL initialize(ULONG hmod) {
   TID tid;
#ifdef __KLIBC__
   if (_CRT_init() != 0)
       return;
   __ctordtorInit();
#endif
   m_setVersion(&g.version);     // set version number
   dbgInit();                    // debug initialization
   if (g.pid.protshell) return FALSE; // abort if DLL is already loaded
   // cache some system values (this must come before readProfile() !)
   // abort if the operating system version is less than VERSUPPORTED
   if (!cacheSysVals())                           // (see estlrVer.h)
      return m_logError(IDERR_NOTVALIDVER, FALSE);
dbgPrintf1("system values cached\n");
   // abort if one of the Ctrl key was pressed
   if (keyPressedToAbortLoading()) return FALSE;
   // read the initialization file
   readProfile();
dbgPrintf1("profile read\n");
   // get the default frame controls bitmap handles
   if (!getDefFrameCtlsBmpHnds()) goto error0;
   // superclasses various PM window classes
   if (!superclassMainPMclasses()) goto error0;
dbgPrintf1("mainPMclass-succesfully superclassed\n");
   // create the worker threads (currently just 1 thread)
   if (0 == (tid = initWorkerThreads())) goto error1;
dbgPrintf1("worker thread initialized\n");
   // set the eStyler hooks (currently just an input hook)
   if (!initHooks(hmod)) goto error2;
dbgPrintf1("input hook started\n");

   return TRUE;
   // error handling
error2:
   // stop the worker thread
   stopWorker();
error1:
   // restore the system window procedures
   stlrSuperclass(WC_BUTTON, NULL, g.cd.btn.pfnwp, -4, 0, 0);
   stlrSuperclass(WC_TITLEBAR, NULL, g.cd.tbar.pfnwp, -4, 0, 0);
   stlrSuperclass(WC_FRAME, NULL, g.cd.frame.pfnwp, -4, 0, 0);
#if 1
   // only for owner drawing...
   stlrSuperclass(WC_MENU, NULL, g.cd.menu.pfnwp, -4, 0, 0);
#endif
error0:
   // free the global bitmaps
   stlrFreeGlobalBitmaps(0, 0, 0, MODIF_ALLBMPS);
   return FALSE;
}


/* --------------------------------------------------------------------------
 DLL termination routine.
  Do nothing as the system is shutdown before getting here.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL terminate(VOID) {
#ifdef __KLIBC__
    __ctordtorTerm();
    _CRT_term();
#endif
   return TRUE;
}


/* --------------------------------------------------------------------------
 Abort loading the DLL if one of the 2 Ctrl keys is pressed.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (abort/go on loading the DLL)
-------------------------------------------------------------------------- */
static BOOL keyPressedToAbortLoading(VOID) {
   if ((WinGetPhysKeyState(HWND_DESKTOP, 0x1d) & 0x8000) ||
       (WinGetPhysKeyState(HWND_DESKTOP, 0x5b) & 0x8000)) {
      DosBeep(440, 200);
      return TRUE;
   } /* endif */
   return FALSE;
}

/* --------------------------------------------------------------------------
 Read the current settings from OS2.INI and ESTYLER.INI
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID readProfile(VOID) {
   HINI hini;
   ULONG cclrs;
   ULONG ul;
   ULONG ordAcpiStart,
         ordAcpiEnd,
         ordAcpiSleep;
   BOOL rc;
   // get previous shutdown data
   PrfQueryProfileString(HINI_USER, SZPRO_APP, SZPRO_ILAST, "0", g.achsd, 24);
   // open ESTYLER.INI
   if (!(hini = stlrOpenProfile())) stlrlog(IDERR_INIOPEN);

   if (hini) {
       char version[16];
       PrfQueryProfileString( hini, SZPRO_OPTIONS, SZPRO_VERSION, "",
                              version, &ul);
       dbgPrintf3("readProfile: current '%s', ini '%s'.\n",
                  INI_VERSION_CURRENT, version);
       // check if version is matching
       if (strcmp( version, INI_VERSION_CURRENT)) {
           // migration is currently not supported, close ini and load
           // defaults
           PrfCloseProfile(hini);
           hini = NULL;
           dbgPrintf1("readProfile: version mismatch, loading defaults.\n");
       }
   }

   // if cannot get the user interface options set the default values
   ul = sizeof(UIOPTIONS);
   if (!hini
       ||
       !PrfQueryProfileData(hini, SZPRO_OPTIONS, SZPRO_UINTERFACE, &o.ui, &ul))
      setUserInterfaceDefaults();
   // if cannot get the shutdown options set the default values
   ul = sizeof(SHUTDOWN);
   if (!hini
       ||
       !PrfQueryProfileData(hini, SZPRO_OPTIONS, SZPRO_SHUTDWON, &o.sd, &ul)) {
      stlrSetShutdownDefaults(hini, &o.sd.gen);
      stlrSetShutdownTimingDefaults(&o.sd.wait);
   } /* endif */
   // get the Windows keys options (default values are 0)
   ul = sizeof(WINKEY);
   if (hini)
      PrfQueryProfileData(hini, SZPRO_OPTIONS, SZPRO_WINKEYS, &o.wk, &ul);
   // check if the color depth changed
   if (hini && g.scr.cclr) {
      ul = sizeof(ULONG);
      // read the previous color depth (if present) if that is different
      // from the current one save the current data
      if (PrfQueryProfileData(hini, SZPRO_CLRDEPTH, SZPRO_CURRENT, &cclrs, &ul)
          &&
          !((g.scr.cclr == cclrs) || ((g.scr.cclr > 256) && (cclrs > 256))))
         handleColorDepthChange(hini, cclrs);
   } // end if

   // always load bitmaps
   setGlobalBitmaps(hini);

   if (hini) PrfCloseProfile(hini);
   // check if there are any user defined exceptions
   rc = PrfQueryProfileSize(HINI_USER, SZPRO_APP, SZEXC_GENERAL, &ul);
   o.ui.exception = rc && ul > 0;
   // check if the WarpCenter is registered
   checkWarpCenterClass();

#ifdef ACPI_ENABLED
   // try and register the ACPI functions, if available
   g.acpifn.pfnAcpiStartApi       = NULL;
   g.acpifn.pfnAcpiPrepareToSleep = NULL;
   g.acpifn.pfnAcpiGoToSleep      = NULL;
   g.acpifn.pfnAcpiEndApi         = NULL;

   ordAcpiStart = _ACPI_START_API;
   ordAcpiEnd   = _ACPI_END_API;
   ordAcpiSleep = _ACPI_GO_TO_SLEEP;
   if (DosLoadModule(NULL, 0, "ACPI32.DLL", &(g.hmodAcpi)) == NO_ERROR) {
      if ( DosQueryProcAddr(g.hmodAcpi, _ACPI_TK_PREPARE_TO_SLEEP, NULL, &(g.acpifn.pfnAcpiPrepareToSleep)) == NO_ERROR )
      {
         ordAcpiStart = _ACPI_TK_START_API;
         ordAcpiEnd   = _ACPI_TK_END_API;
         ordAcpiSleep = _ACPI_TK_GO_TO_SLEEP;
      }
      if ((DosQueryProcAddr(g.hmodAcpi, ordAcpiStart, NULL, &(g.acpifn.pfnAcpiStartApi))  == NO_ERROR) &&
          (DosQueryProcAddr(g.hmodAcpi, ordAcpiSleep, NULL, &(g.acpifn.pfnAcpiGoToSleep)) == NO_ERROR) &&
          (DosQueryProcAddr(g.hmodAcpi, ordAcpiEnd,   NULL, &(g.acpifn.pfnAcpiEndApi))    == NO_ERROR))
      {
         dbgPrintf1("ACPI32 functions were registered successfully.\n");
      }
      else dbgPrintf1("ACPI32 functions could not be registered.\n");
   }
   else dbgPrintf1("ACPI32 not loaded.\n");
#endif
}


/* --------------------------------------------------------------------------
 Set the default user interface options.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setUserInterfaceDefaults(VOID) {
   stlrSetTitlebarGeneralDefaults(&o.ui.tb);
   stlrSetTitlebarHiliteDefaults(&o.ui.tb.a, TRUE);
   stlrSetTitlebarHiliteDefaults(&o.ui.tb.i, FALSE);
   stlrSetButtonDefaults(&o.ui.btn);
   stlrSetDlgDefaults(&o.ui.dlg);
}


/* --------------------------------------------------------------------------
 Set the default values of the general titlebar options
- Parameters -------------------------------------------------------------
 PTITLEBAR ptbo : general titlebar options.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID stlrSetTitlebarGeneralDefaults(PTITLEBAR ptbo) {
   strcpy(ptbo->achFont, "9.WarpSans Bold");
   ptbo->on = 1;                    // enable enhancements
   ptbo->overridePP = 1;            // override presentation parameters
   ptbo->center = 1;                // center the text
}


/* --------------------------------------------------------------------------
 Set the active/inactive titlebar default values.
- Parameters -------------------------------------------------------------
 PTBARHILITE ptbh : active/inactive titlebar options.
 BOOL bActive     : TRUE/FALSE (active/inactive titlebar).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID stlrSetTitlebarHiliteDefaults(PTBARHILITE ptbh, BOOL bActive) {
   ptbh->bkgnd = m_titleBarDefBkgnd(g.scr.cclr);
   ptbh->fl = TBO_FLDEFAULTS | m_titleBarDefBorder(ptbh->bkgnd);
   if (bActive) {
      if (ptbh->bkgnd) {
         ptbh->clrLeftTop = 0xcccccc;
         ptbh->clrRghtBtm = 0x004080;
      } else {
         ptbh->clrLeftTop = 0x80;
         ptbh->clrRghtBtm = 0xcccccc;
      } /* endif */
      ptbh->clrTopLeftBorder = 0;
      ptbh->clrRightBottomBorder = 0xffffff;
      ptbh->clrFgTxt = 0xffffff;
      ptbh->clrBgTxt = 0;
   } else {
      ptbh->clrLeftTop = 0xcccccc;
      ptbh->clrRghtBtm = 0xa0a0a0;
      ptbh->clrTopLeftBorder = 0;
      ptbh->clrRightBottomBorder = 0xffffff;
      ptbh->clrFgTxt = 0x606060;
      ptbh->clrBgTxt = 0xffffff;
   } /* endif */
}


/* --------------------------------------------------------------------------
 Set the default values of the button options.
- Parameters -------------------------------------------------------------
 PBTNOPT pbo : pushbutton options.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID stlrSetButtonDefaults(PBTNOPT pbo) {
   // pushbutton options
   pbo->on = 1;                     // enable enhancements
   pbo->border = (g.scr.cx > 1280); // thick border if horz res >= 1280
   pbo->flat = 0;
   pbo->overPP = 0;
   pbo->def3D = 0;
   pbo->dis3D = 1;
   pbo->solid = 1;                  // solid color always
   pbo->color = 0xcccccc;
}


/* --------------------------------------------------------------------------
 Set the default values of the dialog options.
- Parameters -------------------------------------------------------------
 PDLGOPTIONS pdo: dialog options.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID stlrSetDlgDefaults(PDLGOPTIONS pdo) {
   strcpy(pdo->achFont, SZ_DEFDLGFONT);
   pdo->on = 1;                    // enable enhancements
   pdo->overridePP = 0;            // don't override presentation parameters
}


/* --------------------------------------------------------------------------
 Set the default shutdown values.
- Parameters -------------------------------------------------------------
 HINI hini      : ESTYLER.INI handle.
 PSDGENERAL psd : shutdown general options.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID stlrSetShutdownDefaults(HINI hini, PSDGENERAL psd) {
   if (hini)
      PrfWriteProfileData(hini, SZPRO_SHUTDWON, SZPRO_KILLPROGS,
                          SZPRO_DEFKILLPRG, CB_DEFKILLPRG);
   psd->steps = 100;
   psd->on = 0;
   psd->lksusp = 0;
   psd->susp = 0;
   psd->off = 0;
   psd->reboot = 1;
   psd->ord = 1;
   psd->anim = 0;
}


/* --------------------------------------------------------------------------
 Set the default values for the shutdown timings.
- Parameters -------------------------------------------------------------
 PSDTIMING psdt : shutdown timing data structure.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID stlrSetShutdownTimingDefaults(PSDTIMING psdt) {
   psdt->lkupSusp = 8000;
   psdt->suspend = 0;
   psdt->warpCenter = 1000;
   psdt->unrollWindows = 1000;
   psdt->closeBox = 6000;
   psdt->sysShutdown = 3000;
   psdt->switchOffMsg = 2000;
   psdt->onPowerOff = 3000;
   psdt->killProcs = 3500;
}


/* --------------------------------------------------------------------------
 Get and cache some system values.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (valid/invalid operating system version).
-------------------------------------------------------------------------- */
static BOOL cacheSysVals(VOID) {
   HDC hdc = NULLHANDLE;
   HPS hps = NULLHANDLE;
   HMODULE hmod;
   CHAR achDLLs[512];
   g.is.bootdrv = 'A' + sysInfo(QSV_BOOT_DRIVE) - 1; // boot drive letter
   if ((g.is.version = sysInfo(QSV_VERSION_MINOR)) < VERSUPPORTED)
      return FALSE;
   g.cyIco = WinSysVal(SV_CYICON);                    // icon height
   // get screen resolution and color depth
   if (!stlrQueryScreenCaps(&g.scr)) m_setDefScreeCaps(&g.scr);
   if (PrfQueryProfileString(HINI_USER, "SYS_DLLS", "LoadPerProcess",
                             NULL, achDLLs, sizeof(achDLLs))
       &&
       strstr(achDLLs, "SMARTWIN")) {
      g.is.styler2v1 = 1;
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 Get the screen resolution and color depth. In case of error set the data
 to 640x480x256 colors.
- Parameters -------------------------------------------------------------
 PSCRRES pScreenData : structure to store the screen data.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL stlrQueryScreenCaps(PSCRRES pScreenData) {
   HDC hdc = NULLHANDLE;
   HPS hps = NULLHANDLE;
   BOOL rc = TRUE;
   if (NULLHANDLE == (hps = WinGetScreenPS(HWND_DESKTOP)))
      return FALSE;
   if ((NULLHANDLE == (hdc = GpiQueryDevice(hps)))
       ||
       !DevQueryCaps(hdc, CAPS_WIDTH, 2, &pScreenData->cx)
       ||
       !DevQueryCaps(hdc, CAPS_COLORS, 1, &pScreenData->cclr)
       ||
       !pScreenData->cx)
      rc = FALSE;
   WinReleasePS(hps);
   return rc;
}


/* --------------------------------------------------------------------------
 Open ESTYLER.INI in the USER_INI path or in the root if the USER_INI
 environment variable has not been set.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 HINI : ini file handle or NULLHANDLE in case of error.
-------------------------------------------------------------------------- */
HINI stlrOpenProfile(VOID) {
   CHAR achIniPath[CCHMAXPATH];         // path file inizializzazione
   PSZ pval;
   if (DosScanEnv(SZ_ENVINI, &pval)) return NULLHANDLE;
   strcpy(achIniPath, pval);
   pval = (NULL == (pval = strrchr(achIniPath, '\\'))) ? achIniPath : pval + 1;
   strcpy(pval, SZPRO_PROFNAME);
   return PrfOpenProfile(NULLHANDLE, achIniPath);
}


/* --------------------------------------------------------------------------
 As the color depth changes the settings relative to the previous color
 depth are saved in ESTYLER.INI.
 If previous settings relative to the new color depth are found they are
 restored.
- Parameters -------------------------------------------------------------
 HINI hini   : profile handle
 ULONG cclrs : previous color depth
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID handleColorDepthChange(HINI hini, ULONG cclrs) {
   INT i;
   PSZ pIniAppl;
   ULONG cbData;
   PSZ pBuf;
   PSZ aPszKeys[] = { SZPRO_BMPATBAR, SZPRO_BMPATBARDATA, SZPRO_BMPITBAR,
                      SZPRO_BMPITBARDATA, SZPRO_BMPBUTTON, SZPRO_BMPBTNDATA
                    };
   // allocate the buffer to transfer the data (for storing and restoring)
   if (!m_sysMemAlloc(pBuf, 0x10000)) return;
   // get the name of the INI application where to store the previous settings
   pIniAppl = m_iniAppNameFromColorDepth(cclrs);
   cbData = sizeof(UIOPTIONS);
   // save the settings corresponding to the previous color depth
   PrfWriteProfileData(hini, pIniAppl, SZPRO_UINTERFACE, &o.ui, cbData);
   for (i = 0; i < lengthOfArray(aPszKeys);) {    // bitmap data
      migrateImageName(hini, SZPRO_OPTIONS, pIniAppl, aPszKeys[i++], pBuf);
      migrateImageData(hini, SZPRO_OPTIONS, pIniAppl, aPszKeys[i++], pBuf);
   } /* endfor */
   // if there are previous settings for the new color depth restore them
   pIniAppl = m_iniAppNameFromColorDepth(g.scr.cclr);
   // in case of error set the default values
   if (!PrfQueryProfileData(hini, pIniAppl, SZPRO_UINTERFACE, &o.ui, &cbData))
      setUserInterfaceDefaults();
   for (i = 0; i < lengthOfArray(aPszKeys);) {    // bitmap data
      migrateImageName(hini, pIniAppl, SZPRO_OPTIONS, aPszKeys[i++], pBuf);
      migrateImageData(hini, pIniAppl, SZPRO_OPTIONS, aPszKeys[i++], pBuf);
   } /* endfor */
   m_sysMemFree(pBuf);
}


/* --------------------------------------------------------------------------
 Store or restore the name of the image relative to a given color depth.
- Parameters -------------------------------------------------------------
 HINI hini   : ESTYLER.INI handle.
 PSZ pOldApp : source of image data (INI file application name).
 PSZ pNewApp : target of image data (INI file application name).
 PSZ pKey    : INI key where to store the image name.
 PSZ pBuf    : buffer used to transfer the data
- Return value -----------------------------------------------------------
 VOID (the default values are loaded in case of error).
-------------------------------------------------------------------------- */
static VOID migrateImageName(HINI hini,
                             PSZ pOldApp, PSZ pNewApp, PSZ pKey, PSZ pBuf) {
   ULONG cbData = 0x10000;
   // if oldApp data is present copy it to newApp
   if (PrfQueryProfileString(hini, pOldApp, pKey, NULL, pBuf, CCHMAXPATH))
      PrfWriteProfileString(hini, pNewApp, pKey, pBuf);
   // otherwise if newApp data is present remove it
   else if (PrfQueryProfileSize(hini, pNewApp, pKey, &cbData))
      PrfWriteProfileString(hini, pNewApp, pKey, NULL);
}


/* --------------------------------------------------------------------------
 Store or restore the data of the data relative to a given color depth.
- Parameters -------------------------------------------------------------
 HINI hini   : ESTYLER.INI handle.
 PSZ pOldApp : source of image data (INI file application name).
 PSZ pNewApp : target of image data (INI file application name).
 PSZ pKey    : INI key where to store the image data.
 PSZ pBuf    : buffer used to transfer the data
- Return value -----------------------------------------------------------
 VOID (the default values are loaded in case of error).
-------------------------------------------------------------------------- */
static VOID migrateImageData(HINI hini,
                             PSZ pOldApp, PSZ pNewApp, PSZ pKey, PSZ pBuf) {
   ULONG cbData = 0x10000;
   // if oldApp data is present copy it to newApp
   if (PrfQueryProfileData(hini, pOldApp, pKey, pBuf, &cbData))
      PrfWriteProfileData(hini, pNewApp, pKey, pBuf, cbData);
   // otherwise if newApp data is present remove it
   else if (PrfQueryProfileSize(hini, pNewApp, pKey, &cbData))
      PrfWriteProfileData(hini, pNewApp, pKey, NULL, 0);
}


//===========================================================================
// Get the handles used for the default bitmaps used to paint the frame
// controls.
// Parameters --------------------------------------------------------------
// VOID
// Return value ------------------------------------------------------------
// BOOL : TRUE/FALSE (success/error)
//===========================================================================
static
BOOL getDefFrameCtlsBmpHnds(VOID) {
    HMQ hmq;
    HAB hab;
    BOOL rc = FALSE;
    CLASSINFO ci;
    HWND hwnd, hmn;
    FRAMECDATA fcd = {0};
    MENUITEM mi;

    dbgPrintf1("getDefFrameCtlsBmpHnds\n");

    // crea finestra fittizia x ricavare handle bmp controlli default finestra
    hmq = WinCreateMsgQueue((hab = WinInitialize(0)), 0);

    // dati classe menu
    if (!WinQueryClassInfo(NULLHANDLE, WC_MENU, &ci)) {
        WinDestroyMsgQueue(hmq);
        WinTerminate(hab);
        if (!rc) stlrlog(IDERR_BMPMINMAX);
        return rc;
    }

    g.cd.menu.pfnwp = ci.pfnWindowProc;
    g.cd.menu.cbData = ci.cbWindowData;
    // crea finestra fittizia
    fcd.cb = sizeof(FRAMECDATA);
    fcd.flCreateFlags = FCF_SYSMENU | FCF_MINMAX | FCF_HIDEBUTTON;
    if ((hwnd = WinCreateWindow(HWND_DESKTOP, WC_FRAME, "", 0, 0, 0, 0, 0, 0,
                                HWND_BOTTOM, 0, &fcd, NULL)) != NULLHANDLE) {
        // ricava handle bitmap titlebar button default
        hmn = WinWindowFromID(hwnd, FID_SYSMENU);
        g.cd.menu.pfnwp(hmn, MM_QUERYITEM, (MPARAM)SC_SYSMENU, (MPARAM)&mi);
        g.bmpDef.sys = mi.hItem;
        hmn = WinWindowFromID(hwnd, FID_MINMAX);
        g.cd.menu.pfnwp(hmn, MM_QUERYITEM, (MPARAM)SC_CLOSE, (MPARAM)&mi);
        g.bmpDef.cls = mi.hItem;
        g.cd.menu.pfnwp(hmn, MM_QUERYITEM, (MPARAM)SC_MINIMIZE, (MPARAM)&mi);
        g.bmpDef.min = mi.hItem;
        g.cd.menu.pfnwp(hmn, MM_QUERYITEM, (MPARAM)SC_HIDE, (MPARAM)&mi);
        g.bmpDef.hide = mi.hItem;
        g.cd.menu.pfnwp(hmn, MM_QUERYITEM, (MPARAM)SC_MAXIMIZE, (MPARAM)&mi);
        g.bmpDef.max = mi.hItem;
        WinSetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_MAXIMIZE);
        g.cd.menu.pfnwp(hmn, MM_QUERYITEM, (MPARAM)SC_RESTORE, (MPARAM)&mi);
        g.bmpDef.rest = mi.hItem;
        WinDestroyWindow(hwnd);
        rc = TRUE;
    }

    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);
    if (!rc) stlrlog(IDERR_BMPMINMAX);
    return rc;
}


/* --------------------------------------------------------------------------
 Superclass WC_FRAME, WC_TITLEBAR and WC_BUTTON.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL superclassMainPMclasses(VOID) {

   dbgPrintf1("getDefFrameCtlsBmpHnds\n");

   if (!stlrSuperclass(WC_FRAME, &g.cd.frame, stlrFrameProc, 4,
                          IDERR_FRAMECLASS, IDERR_NEWFRAME))
      return FALSE;
   if (!stlrSuperclass(WC_TITLEBAR, &g.cd.tbar, stlrTitleBarProc, 4,
                          IDERR_TITLEBARCLASS, IDERR_NEWTITLEBAR))
      goto restoreWC_FRAME;
   if (!stlrSuperclass(WC_BUTTON, &g.cd.btn, stlrButtonProc, 4,
                          IDERR_BTNCLASS, IDERR_NEWBTN))
      goto restoreWC_TITLEBAR;
#if 1
   // MENU subclassing is not required for replacing bitmap buttons,
   // but it will be necessary for control ownerdrawing
   if (!stlrSuperclass(WC_MENU, &g.cd.menu, stlrMenuProc, 4,
                          IDERR_MENUCLASS, IDERR_NEWMENU))
      goto restoreWC_BUTTON;
#endif

   return TRUE;

   // in case of error restore the original window classes procedures
restoreWC_BUTTON:
   stlrSuperclass(WC_BUTTON, NULL, g.cd.btn.pfnwp, -4, 0, 0);
restoreWC_TITLEBAR:
   stlrSuperclass(WC_TITLEBAR, NULL, g.cd.tbar.pfnwp, -4, 0, 0);
restoreWC_FRAME :
   stlrSuperclass(WC_FRAME, NULL, g.cd.frame.pfnwp, -4, 0, 0);
   return FALSE;
}


/* --------------------------------------------------------------------------
 Superclass a PM window class optionally storing the data of the original
 class in the 'pcd' parameter.
- Parameters -------------------------------------------------------------
 PSZ pszClass   : PM window class which must be superclassed
 PCLASSDATA pcd : storage for the default class data
 PFNWP pfnwpNew : new window class procedure
 LONG cbWin     : needed window words size increment
 ULONG id1      : error code 1
 ULONG id2      : error code 2
- Return value -----------------------------------------------------------
 BOOL TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL stlrSuperclass(PSZ pszClass, PCLASSDATA pcd, PFNWP pfnwpNew,
                           LONG cbWin, ULONG id1, ULONG id2) {
   CLASSINFO ci;
   if (!WinQueryClassInfo(NULLHANDLE, pszClass, &ci))
      return m_logError(id1, FALSE);
   if (pcd) {
      pcd->pfnwp = ci.pfnWindowProc;
      pcd->cbData = ci.cbWindowData;
   } /* endif */
   if (!WinRegisterClass(NULLHANDLE, pszClass, pfnwpNew,
                         ci.flClassStyle, ci.cbWindowData + cbWin))
      return m_logError(id2, FALSE);
   return TRUE;
}


/* --------------------------------------------------------------------------
 Start the worker threads.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 TID : 0 in case of error or thread ID of the worker thread.
-------------------------------------------------------------------------- */
static TID initWorkerThreads(VOID) {
   TID tidWorker = 0;
   // crea thread timers
   if (DosCreateThread(&tidWorker, (PFNTHREAD)threadWorker,0, 0, 8192))
      return m_logError(IDERR_CREATETIMERMGR, (TID)0);
   return tidWorker;
}


/* --------------------------------------------------------------------------
 Initialize the event and the send message hooks.
- Parameters -------------------------------------------------------------
 HMODULE hmod : the handle of this module
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL initHooks(HMODULE hmod) {
   // input hook initialization
   if (!WinSetHook(NULLHANDLE, NULLHANDLE, HK_INPUT, (PFN)postMsgHook, hmod))
      return m_logError(IDERR_CREATEINPUTHOOK, FALSE);
   return TRUE;
}


/* --------------------------------------------------------------------------
 Check if the WarpCenter class is registered. If it is not registered
 skip all WarpCenter enhancements.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID checkWarpCenterClass(VOID) {
   PWPSCLASSLIST pClassList;
   ULONG cb;
   PWPSCLASSENTRY pClass;
   PBYTE pEnd;
dbgPrintf1("*** check warp center registration\n");

   cb = 0x10000;
   if (m_sysMemAlloc(pClassList, cb)) {
      if (PrfQueryProfileData(HINI_SYSTEM, PRF_WPSCLASSESAPP,
                              PRF_WPSCLASSESKEY, pClassList, &cb)) {
         for (pClass = &pClassList->aClass[0], pEnd = (PBYTE)pClassList + cb;
              (PBYTE)pClass < pEnd;
              pClass = PNEXTCLASS(pClass)) {
            if (!memcmp(SZ_WCENTERCLASS, pClass->achClass, CB_WCENTERCLASS)) {
               g.is.wcRegistered = 1;
               break;
            } /* endif */
         } /* endfor */
      m_sysMemFree(pClassList);
      } /* endif */
   } /* endif */
dbgPrintf2("*** warp center is %sregistered\n", (g.is.wcRegistered? "": "not "));

}


