/* --------------------------------------------------------------------------
 initend.c : initialization and termination routines

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"
#include "notebook.h"

// definitions --------------------------------------------------------------


// prototypes ---------------------------------------------------------------
static BOOL importProcedures(VOID);
static BOOL getCurrentSettings(VOID);
static BOOL getBmpFileNamesAndData(VOID);
static BOOL getStlrBmpDetails(HINI hini, PBMPFILEDATA p, PSZ pszNameKey, PSZ pszDataKey,
                       ULONG idRes, ULONG flPreview, ULONG updateMsg);
static BOOL getPrfData(HINI hini, PSZ pszApp, PSZ pszKey, PULONG pCb, PBYTE* ppData);
static BOOL getShutdownData(VOID);
static BOOL getWinPos(VOID);
static BOOL initBitmapHandles(VOID);
static VOID initFileSelDlgBuffer(VOID);
static VOID resetBmpHandles(PUIOPTIONS pOpts);
static VOID endHelp(VOID);

// global variables ---------------------------------------------------------

// get the current styler version: PSTLRVER stlrGetVer(VOID);
PSTLR_GETVER_FN stlrGetVer;

// open eStyler.ini:  HINI stlrOpenProfile(VOID)
PSTLR_OPENPRF_FN stlrOpenProfile;

// get the address of the structure holding the current settings:
// POPTIONS stlrGetOptionsDataAddress(VOID);
PSTLR_GETOPTS_FN stlrGetOptionsDataAddress;

// get the current screen resolution and color depth:
// PSCRRES stlrQryScreenRes(VOID);
PSTLR_GETSCRRES_FN stlrQryScreenRes;

// set the default values of the general titlebar options
// VOID stlrSetTitlebarGeneralDefaults(PTITLEBAR ptbo);
PSETTBGENDEF_FN stlrSetTitlebarGeneralDefaults;

// set the default values of the active/inactive titlebar options
// VOID stlrSetTitlebarHiliteDefaults(PTBARHILITE ptbh, BOOL bActive);
PSETTBHILITEDEF_FN stlrSetTitlebarHiliteDefaults;

// set the default values of the button options
// VOID stlrSetButtonDefaults(PBTNOPT pbo);
PSETBTNDEF_FN stlrSetButtonDefaults;

// set the default values of the dialog options
//VOID stlrSetDlgDefaults(PDLGOPTIONS pdo);
PSETDLGDEF_FN stlrSetDlgDefaults;

// set the default values of the general button options
// VOID stlrSetShutdownDefaults(HINI hini, PSDGENERAL psd);
PSETSHUTDOWNDEF_FN stlrSetShutdownDefaults;

// set the default values of the shutdown timing options
// VOID stlrSetShutdownTimingDefaults(PSDTIMING psdt);
PSETSDTIMINGDEF_FN stlrSetShutdownTimingDefaults;

// build a bitmap from gradient parameters
// HBITMAP stlrShadeHBmp(HPS hps, PSHDCREATE psc, ULONG fl,
//                       LONG clrLeftTop, LONG clrRightBottom);
PHBMPSHADE_FN stlrShadeHBmp;

// Get a bitmap handle and optionally the bitmap size from the data read
// from a bitmap file.
// HBITMAP stlrHBmpFromBmpData(HPS hps, PBYTE pbmp, PBMPPAINT pbp, PBMPDAT pbd);
PHBMPFROMBMPDATA_FN stlrHBmpFromBmpData;

// Get a bitmap handle and optionally the bitmap size from a bitmap
// contained in a resources DLL.
// HBITMAP stlrHBmpFromModule(HPS hps, HMODULE hmod, ULONG id,
//                            PBMPPAINT pbp, PBMPDAT pbd);
PHBMPFROMMODULE_FN stlrHBmpFromModule;

// Make a bitmap handle global.
// BOOL stlrMakeGlobalBitmap(HPS hps, HBITMAP hbmp);
PMAKEGLOBALBMP_FN stlrMakeGlobalBitmap;

// Free a global bitmap.
// VOID stlrFreeGlobalBitmap(HPS hps, HBITMAP phbmp, PID pid);
PFREEGLOBALBMP_FN stlrFreeGlobalBitmap;


/* --------------------------------------------------------------------------
 According to the startup parameters ESTYLER.EXE display the settings
 notebook of the user interface settings, the shutdown settings or the
 Windows keys settings.
 Only the first parameter is taken into account: if it starts with 's' or
 'S' the shutodwn settings are displayed, if it starts with 'w' or 'W' the
 Windows keys settings are displayed, if there is no parameter or if the
 parameter is not valid the useri interface settings are displayed.
- Parameters -------------------------------------------------------------
 Standard executable startup parameters.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID parseArgs(INT argc, PSZ* argv) {
   if (argc > 1) {
      switch (argv[1][0]) {
         case 's':
         case 'S':
            g.mode = IDX_SHUTDOWNPREF;
            break;
         case 'w':
         case 'W':
            g.mode = IDX_WINDOWSKEYSPREF;
            break;
      } /* endswitch */
   } /* endif */
}


/* --------------------------------------------------------------------------
 Application initialization:
 - when in user-interface-settings mode start the worker thread,
 - load the runtime DLL and import the needed procedures,
 - initialize the PM environment,
 - register the custom controls,
 - load the resources library.
- Parameters -------------------------------------------------------------
 VOID.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
BOOL initApplication(VOID) {
   if (NULLHANDLE == (g.hResLib = WinLoadLibrary(g.appl.hab, SZ_RESMODULE)))
      return handleError((ULONG)SZERR_LOADRESMODULE, FALSE);

   // always create a worker thread
   if (!startWorkerThread())
      return FALSE;
   if (!importProcedures())
      return FALSE;
   if (!getCurrentSettings())
      return FALSE;
   getWinPos();
   g.appl.hmq = WinCreateMsgQueue(g.appl.hab = WinInitialize(0), 0);
   if (!g.appl.hmq)
      return handleError(ISERR_INITPM, FALSE);
   // register the classes used by the application
//   if (!STFVRegister(g.appl.hab))
//      return handleError(ISERR_REGSTFVCLASS, FALSE);
#ifdef USE_APIEX
   if (!BoxClassRegister(g.appl.hab))
#else
    if (!EDBarReg(g.appl.hab))
#endif
      return handleError(ISERR_REGBOXCLASS, FALSE);
   if (!xComboBoxRegister(g.appl.hab))
      return handleError(ISERR_REGXCOMBOCLASS, FALSE);
   if (!xClrButtonRegister(g.appl.hab))
      return handleError(ISERR_REGCLRBTNCLASS, FALSE);
   // load the module library
//   if (NULLHANDLE == (g.hResLib = WinLoadLibrary(g.appl.hab, SZ_RESMODULE)))
//      return handleError(ISERR_LOADRESMODULE, FALSE);
   // duplicate the current bitmap handles
   if ((g.mode == IDX_USERINTERFACEPREF) && !initBitmapHandles())
      return handleError(ISERR_GETBMPHANDLES, FALSE);
   // initialize the path of the file selection dialog buffer
   initFileSelDlgBuffer();
   return TRUE;
}


/* --------------------------------------------------------------------------
 Terminate the PM environment.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID endApplication(VOID) {
   PSZ aPszKey[] = {SZPRO_UINTERFACE, SZPRO_SHUTDWON, SZPRO_WINKEYS};
   PVOID aOpts[] = {&g.pCurOpts->ui, &g.pCurOpts->sd, &g.pCurOpts->wk};
   ULONG aCbOpts[] = {sizeof(g.pCurOpts->ui), sizeof(g.pCurOpts->sd),
                      sizeof(g.pCurOpts->wk)};
   SWP swp;
   HINI hini;
   if (NULLHANDLE != (hini = stlrOpenProfile())) {
      WinQueryWindowPos(g.appl.hwnd, &swp);
      if (swp.fl & SWP_MINIMIZE) {
         swp.x = WinQueryWindowUShort(g.appl.hwnd, QWS_XRESTORE);
         swp.y = WinQueryWindowUShort(g.appl.hwnd, QWS_YRESTORE);
      } /* endif */
      g.winPos.x = swp.x, g.winPos.y = swp.y;
      switch (g.mode) {
         case IDX_USERINTERFACEPREF:
            if (g.pUiData->preview.hwnd) {
               WinQueryWindowPos(g.pUiData->preview.hwnd, &swp);
               g.pUiData->preview.x = swp.x;
               g.pUiData->preview.y = swp.y;
               g.pUiData->preview.cx = swp.cx;
               g.pUiData->preview.cy = swp.cy;
               g.pUiData->preview.hidden = !(g.state & STLRIS_PREVIEWVISIBLE);
               PrfWriteProfileData(hini, SZPRO_SIZEPOS, SZPRO_PREVIEW,
                                   &g.pUiData->preview,
                                   sizeof(PREVIEWWND) - sizeof(HWND));
               WinDestroyWindow(g.pUiData->preview.hwnd);
            } /* endif */
            if (g.pUiData) {
               // if the active/inactive titlebar pages are hidden destroy them
               if (!g.aNBpageIDs[NBP_UITITLEBARACTIVE]) {
                  if (g.pUiData->hwndActiveTbar)
                     WinDestroyWindow(g.pUiData->hwndActiveTbar);
                  if (g.pUiData->hwndInactiveTbar)
                     WinDestroyWindow(g.pUiData->hwndInactiveTbar);
               } /* endif */
               GpiDeleteBitmap(g.pUiData->pOpts->tb.a.bmpp.hbmp);
               GpiDeleteBitmap(g.pUiData->pOpts->tb.i.bmpp.hbmp);
               GpiDeleteBitmap(g.pUiData->pOpts->btn.bmpp.hbmp );
               free(g.pUiData->tba.pDataUndo);
               free(g.pUiData->tbi.pDataUndo);
               free(g.pUiData->btn.pDataUndo);
               free(g.pUiData->pOpts);
               free(g.pUiData);
            } /* endif */
            break;
         case IDX_SHUTDOWNPREF:
            if (g.pShdwnData) {
               // if the reboot page is hidden destroy it
               if (!g.aNBpageIDs[NBP_SDREBOOT] && g.pShdwnData->hwndReboot)
                  WinDestroyWindow(g.pShdwnData->hwndReboot);
               free(g.pShdwnData->pBootVols);
               free(g.pShdwnData->pAppliedBootVols);
               free(g.pShdwnData->pRunProgs);
               free(g.pShdwnData->pAppliedRunProgs);
               free(g.pShdwnData->pKillProgs);
               free(g.pShdwnData->pAppliedKillProgs);
               free(g.pShdwnData);
            } /* endif */
            break;
         case IDX_WINDOWSKEYSPREF:
            break;
      } /* endswitch */
      PrfWriteProfileData(hini, SZPRO_SIZEPOS, aPszKey[g.mode],
                          &g.winPos, sizeof(g.winPos));
      PrfWriteProfileData(hini, SZPRO_OPTIONS, aPszKey[g.mode],
                          aOpts[g.mode], aCbOpts[g.mode]);
      PrfCloseProfile(hini);
   } else {
      errorBox(ISERR_OPENPRF);
   } /* endif */
#ifdef _STLR_TEST_MODE_
   free(g.pCurOpts);
#endif
   free(g.pUndoUiOpts);
   endHelp();
   if (g.worker.hev) DosCloseEventSem(g.worker.hev);
   if (g.hRunLib) DosFreeModule(g.hRunLib);
   if (g.hResLib) WinDeleteLibrary(g.appl.hab, g.hResLib);
   if (g.appl.hwnd) WinDestroyWindow(g.appl.hwnd);
   if (g.appl.hmq) WinDestroyMsgQueue(g.appl.hmq);
   if (g.appl.hab) WinTerminate(g.appl.hab);
}


/* --------------------------------------------------------------------------
 Load the runtime DLL and import the needed procedures.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL importProcedures(VOID) {
   // get the handle of the worker thread object window of Styler/2:
   // HWND stlrGetTWorkerThreadHandle(VOID);
   PSTLR_GETWORKERHWND_FN stlrGetWorkerThreadHandle;

#ifndef _STLR_TEST_MODE_
   if (DosLoadModule(NULL, 0, SZ_MODULENAME, &g.hRunLib))
      return handleError(ISERR_LOADRUNTIMEMODULE, FALSE);
   if (
       DosQueryProcAddr(g.hRunLib, IPROC_GETVER, NULL,
                        (PFN*)&stlrGetVer)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_OPENPRF, NULL,
                        (PFN*)&stlrOpenProfile)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_GETOPTS, NULL,
                        (PFN*)&stlrGetOptionsDataAddress)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_GETWORKERHWND, NULL,
                        (PFN*)&stlrGetWorkerThreadHandle)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_GETSCRRES, NULL,
                        (PFN*)&stlrQryScreenRes)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_SETTBGENDEF, NULL,
                        (PFN*)&stlrSetTitlebarGeneralDefaults)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_SETTBHILITEDEF, NULL,
                        (PFN*)&stlrSetTitlebarHiliteDefaults)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_SETBTNDEF, NULL,
                        (PFN*)&stlrSetButtonDefaults)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_SETDLGDEF, NULL,
                        (PFN*)&stlrSetDlgDefaults)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_SETSHUTDOWNDEF, NULL,
                        (PFN*)&stlrSetShutdownDefaults)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_SETSDTIMINGDEF, NULL,
                        (PFN*)&stlrSetShutdownTimingDefaults)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_HBMPSHADE, NULL,
                        (PFN*)&stlrShadeHBmp)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_HBMPFROMBMPDATA, NULL,
                        (PFN*)&stlrHBmpFromBmpData)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_HBMPFROMMODULE, NULL,
                        (PFN*)&stlrHBmpFromModule)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_MAKEGLOBALBMP, NULL,
                        (PFN*)&stlrMakeGlobalBitmap)
       ||
       DosQueryProcAddr(g.hRunLib, IPROC_FREEGLOBALBMP, NULL,
                        (PFN*)&stlrFreeGlobalBitmap)
//       ||
//       DosQueryProcAddr(g.hRunLib, IPROC_, NULL,
//                        (PFN*)&))
       ) {
      return handleError(ISERR_QUERYPROCADDR, FALSE);
   } else {
      // get the handle of the worker thread object window of Styler/2:
      g.hwndRunTimeWorker = stlrGetWorkerThreadHandle();
   } /* endif */
#else
    stlrGetVer = (PSTLR_GETVER_FN)_stlrGetVer;
    stlrGetOptionsDataAddress = (PSTLR_GETOPTS_FN)_stlrGetOptionsDataAddress;
    stlrOpenProfile = (PSTLR_OPENPRF_FN)_stlrOpenProfile;
    stlrQryScreenRes = (PSTLR_GETSCRRES_FN) _stlrQryScreenRes;
    stlrSetTitlebarGeneralDefaults = (PSETTBGENDEF_FN) _stlrSetTitlebarGeneralDefaults;
    stlrSetTitlebarHiliteDefaults = (PSETTBHILITEDEF_FN) _stlrSetTitlebarHiliteDefaults;
    stlrSetButtonDefaults = (PSETBTNDEF_FN) _stlrSetButtonDefaults;
    stlrSetDlgDefaults = (PSETDLGDEF_FN)_stlrSetDlgDefaults;
    stlrSetShutdownDefaults = (PSETSHUTDOWNDEF_FN) _stlrSetShutdownDefaults;
    stlrSetShutdownTimingDefaults = (PSETSDTIMINGDEF_FN) _stlrSetShutdownTimingDefaults;
    stlrShadeHBmp = (PHBMPSHADE_FN)_stlrShadeHBmp;
    stlrHBmpFromBmpData = (PHBMPFROMBMPDATA_FN)_stlrHBmpFromBmpData;
    stlrHBmpFromModule = (PHBMPFROMMODULE_FN)_stlrHBmpFromModule;
    stlrMakeGlobalBitmap = (PMAKEGLOBALBMP_FN)_stlrMakeGlobalBitmap;
    stlrFreeGlobalBitmap = (PFREEGLOBALBMP_FN)_stlrFreeGlobalBitmap;
//     = ()_;

#endif
   return TRUE;
}


/* --------------------------------------------------------------------------
 Get the current user interface, shutdown or windows keys settings.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL getCurrentSettings(VOID) {
   // get the current screen resolution
   g.scr = *(stlrQryScreenRes());
   // get the current options
   if (NULL == (g.pCurOpts = stlrGetOptionsDataAddress()))
      return handleError(ISERR_GETCUROPTIONS, FALSE);
   // allocate the structures for the apply and undo features
   switch (g.mode) {
      case IDX_USERINTERFACEPREF:
         if (NULL == (g.pUiData = malloc(sizeof(UIDATA))))
            return handleError(ISERR_ALLOCATION, FALSE);
         memset(g.pUiData, 0, sizeof(UIDATA));
         if ((NULL == (g.pUiData->pOpts = malloc(sizeof(UIOPTIONS))))
             ||
             (NULL == (g.pUndoUiOpts = malloc(sizeof(UIOPTIONS)))))
            return handleError(ISERR_ALLOCATION, FALSE);
         memcpy(g.pUiData->pOpts, &g.pCurOpts->ui, sizeof(UIOPTIONS));
         resetBmpHandles(g.pUiData->pOpts);
         memcpy(g.pUndoUiOpts, &g.pCurOpts->ui, sizeof(UIOPTIONS));
         resetBmpHandles(g.pUndoUiOpts);
         memcpy(g.aNBpageFlags, UIPAGEFLAGS, NBP_UICOUNT);
         if (!getBmpFileNamesAndData()) return FALSE;
         break;
      case IDX_SHUTDOWNPREF:
         if (NULL == (g.pShdwnData = malloc(sizeof(SHUTDOWNDATA))))
            return handleError(ISERR_ALLOCATION ,FALSE);
         memset(g.pShdwnData, 0, sizeof(SHUTDOWNDATA));
         if (NULL == (g.pUndoSdOpts = malloc(sizeof(SHUTDOWN))))
            return handleError(ISERR_ALLOCATION, FALSE);
         memcpy(g.pUndoSdOpts, &g.pCurOpts->sd, sizeof(SHUTDOWN));
         memcpy(g.aNBpageFlags, SHDOWNPAGEFLAGS, NBP_SDCOUNT);
         if (!getShutdownData()) return FALSE;
         break;
      case IDX_WINDOWSKEYSPREF:
         if (NULL == (g.pUndoWkOpts = malloc(sizeof(WINKEY))))
            return handleError(ISERR_ALLOCATION, FALSE);
         memcpy(g.pUndoWkOpts, &g.pCurOpts->wk, sizeof(WINKEY));
         memcpy(g.aNBpageFlags, WKPAGEFLAGS, NBP_WKCOUNT);
         break;
   } /* endswitch */
   return TRUE;
}


/* --------------------------------------------------------------------------
 - Get the names of the selected bitmap files.
 Note: in case of error the program is terminated and the allocated resources
 are freed by endApplication().
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL getBmpFileNamesAndData(VOID) {
   HINI hini = NULLHANDLE;
   BOOL rc;
   if (NULLHANDLE == (hini = stlrOpenProfile()))
      return handleError(ISERR_OPENPRF, FALSE);
   // active titlebar bitmap name and data
   rc =
        getStlrBmpDetails(hini, &g.pUiData->tba,
                          SZPRO_BMPATBAR, SZPRO_BMPATBARDATA,
                          ((g.scr.cclr > 256) ? IDBMP_TBA: IDBMP_TBA8),
                          PVUPD_TITLEACTIVE, MODIF_ACTWIN)
        &&
        getStlrBmpDetails(hini, &g.pUiData->tbi,
                          SZPRO_BMPITBAR, SZPRO_BMPITBARDATA,
                          ((g.scr.cclr > 256) ? IDBMP_TBI: IDBMP_TBI8),
                          PVUPD_TITLEINACTIVE, MODIF_INACTWIN)
        &&
        getStlrBmpDetails(hini, &g.pUiData->btn,
                          SZPRO_BMPBUTTON, SZPRO_BMPBTNDATA,
                          ((g.scr.cclr > 256) ? IDBMP_BTN: IDBMP_BTN8),
                          PVUPD_BTNALL, MODIF_PUSHBTN)
        ;
   PrfCloseProfile(hini);
   return rc;
}


/* --------------------------------------------------------------------------
 Get the name and the data of the currently selected bitmaps used by Styler
 for painting the titlebars and the buttons.
- Parameters -------------------------------------------------------------
 HINI hini       : eStyler.ini handle.
 PBMPFILEDATA p  : address where to store the bitmap details.
 PSZ pszNameKey  : ini file key where the bitmap file name is stored.
 PSZ pszDataKey  : ini file key where the bitmap file data is stored.
 ULONG idRes     : resource ID of the default bitmap.
 ULONG flPreview : update preview window flag.
 ULONG updateMsg : apply settings to update user interface message.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static
BOOL getStlrBmpDetails(HINI hini, PBMPFILEDATA p, PSZ pszNameKey, PSZ pszDataKey,
                       ULONG idRes, ULONG flPreview, ULONG updateMsg) {
   // active titlebar bitmap name and data
   if (PrfQueryProfileString(hini, SZPRO_OPTIONS, pszNameKey, NULL,
                             p->pszCur, CCHMAXPATHCOMP)) {
      strcpy(p->pszUndo, p->pszCur);
      strcpy(p->pszApplied, p->pszCur);
   } /* endif */
   if (!getPrfData(hini, SZPRO_OPTIONS, pszDataKey, &p->cbUndo, &p->pDataUndo))
      return FALSE;
   p->pszNameKey = pszNameKey;
   p->pszDataKey = pszDataKey;
   p->idRes = idRes;
   p->flPreview = flPreview;
   p->updateMsg = updateMsg;
   return TRUE;
}


/* --------------------------------------------------------------------------
 Query the size of styler data stored in estyler.ini or OS2.INI.
 Allocate the needed storage and read the data.
- Parameters -------------------------------------------------------------
 HINI hini     : estyler.ini profile handle.
 PSZ pszApp    : name of the ini application
 PSZ pszKey    : name of the ini key holding the bitmap data.
 PULONG pCb    : data size.
 PBYTE* ppData : bitmap data.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static
BOOL getPrfData(HINI hini, PSZ pszApp, PSZ pszKey, PULONG pCb, PBYTE* ppData) {
   if (PrfQueryProfileSize(hini, pszApp, pszKey, pCb)) {
      if (NULL == (*ppData = malloc(*pCb)))
         return handleError(ISERR_ALLOCATION, FALSE);
      if (!PrfQueryProfileData(hini, pszApp, pszKey, *ppData, pCb))
         return handleError(ISERR_READPRFDATA, FALSE);
    } /* endif */
    return TRUE;
}


/* --------------------------------------------------------------------------
 Read from Styler.ini and os2.ini the list of the bootable volumes, the
 list of the programs to be executed on shutdown and the list of the
 programs to be killed on shutdown.
 Note: in case of error the program is terminated and the allocated resources
 are freed by endApplication().
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL getShutdownData(VOID) {
   HINI hini;
   ULONG rc = FALSE;
   if (NULLHANDLE == (hini = stlrOpenProfile()))
      return handleError(ISERR_OPENPRF, FALSE);
   // get the re-bootable volumes list
   if (!getPrfData(hini, SZPRO_SHUTDWON, SZPRO_SDWNLIST,
                   &g.pShdwnData->cbBootVols, &g.pShdwnData->pBootVols))
         goto exit_0;
   if (g.pShdwnData->cbBootVols) {
      g.pShdwnData->pAppliedBootVols = dataDup(g.pShdwnData->pBootVols,
                                               g.pShdwnData->cbBootVols,
                                               &g.pShdwnData->cbAppliedBootVols);
      if (!g.pShdwnData->pAppliedBootVols)
         goto exit_0;
   } /* endif */
   // get the list of the programs to be executed on shutdown
   if (!getPrfData(hini, SZPRO_SHUTDWON, SZPRO_SDWNPRGLIST,
                   &g.pShdwnData->cbRunProgs, &g.pShdwnData->pRunProgs))
      goto exit_0;
   if (g.pShdwnData->cbRunProgs) {
      g.pShdwnData->pAppliedRunProgs = dataDup(g.pShdwnData->pRunProgs,
                                               g.pShdwnData->cbRunProgs,
                                               &g.pShdwnData->cbAppliedRunProgs);
      if (!g.pShdwnData->pAppliedRunProgs)
         goto exit_0;
   } /* endif */
   // get the list of the programs to be killed on shutdown
   if (!getPrfData(hini, SZPRO_SHUTDWON, SZPRO_KILLPROGS,
                   &g.pShdwnData->cbKillProgs, &g.pShdwnData->pKillProgs))
      goto exit_0;
   if (g.pShdwnData->cbRunProgs) {
      g.pShdwnData->pAppliedKillProgs = dataDup(g.pShdwnData->pKillProgs,
                                              g.pShdwnData->cbKillProgs,
                                              &g.pShdwnData->cbAppliedKillProgs);
      if (!g.pShdwnData->pAppliedKillProgs)
         goto exit_0;
   } /* endif */
   rc = TRUE;
exit_0:
   PrfCloseProfile(hini);
   return rc;
}



/* --------------------------------------------------------------------------
 Get the preferences window position (and the preview window position and size).
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL getWinPos(VOID) {
   HINI hini;
   ULONG ul;
   PSZ aPosKey[] = {SZPRO_UINTERFACE, SZPRO_SHUTDWON, SZPRO_WINKEYS};
   hini = stlrOpenProfile();
   ul = sizeof(POINTL);
   if (hini) {
      PrfQueryProfileData(hini, SZPRO_SIZEPOS, aPosKey[g.mode], &g.winPos, &ul);
      if (g.mode == IDX_USERINTERFACEPREF) {
         ul = sizeof(PREVIEWWND) - sizeof(HWND);
         PrfQueryProfileData(hini, SZPRO_SIZEPOS, SZPRO_PREVIEW,
                             &g.pUiData->preview, &ul);
         if (!g.pUiData->preview.hidden) g.state |= STLRIS_PREVIEWVISIBLE;
      } /* endif */
      PrfCloseProfile(hini);
      return TRUE;
   } /* endif */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Duplicate the current bitmap handles for the preview window.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL initBitmapHandles(VOID) {
   HPS hps;
   BOOL rc = FALSE;
   PSHDCREATE pshc;
   // allocate the needed resources --------------------------------------
   if (NULL == (pshc = malloc(CB_SHADEALLOC))) return FALSE;
   if (NULLHANDLE == (hps = WinGetScreenPS(HWND_DESKTOP))) goto exit_0;
   // active title bar bitmap --------------------------------------------
   if (!getTitlebarHbmp(hps, g.pUiData->tba.pDataUndo,
                        &g.pUiData->pOpts->tb.a, pshc,
                        ((g.scr.cclr > 256) ? IDBMP_TBA: IDBMP_TBA8)))
      goto exit_1;
   // active title bar bitmap --------------------------------------------
   if (!getTitlebarHbmp(hps, g.pUiData->tbi.pDataUndo,
                        &g.pUiData->pOpts->tb.i, pshc,
                        ((g.scr.cclr > 256) ? IDBMP_TBI: IDBMP_TBI8)))
      goto exit_1;
   // pushbutton bitmap --------------------------------------------------
   if (!g.pUiData->pOpts->btn.solid) {
      if (!getBtnHbmp(hps, g.pUiData->btn.pDataUndo, &g.pUiData->pOpts->btn))
         goto exit_1;
   } /* endif */
   rc = TRUE;
exit_1:
   WinReleasePS(hps);
exit_0:
   free(pshc);
   return rc;
}


/* --------------------------------------------------------------------------
 If the worker thread is running terminate after having deleted any pending
 job and then post a WM_QUIT to the main thread PM queue, otherwise directly
 post the WM_QUIT message.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID quit(VOID) {
   INT i;
   QMSG qmsg;
   // apply the unapplied options
   for (i = 0; i < NBP_MAXCOUNT; ++i) {
      if (g.aNBpageIDs[i] && (g.aNBpageFlags[i] & PGFL_APPLY_ENABLED)) {
         g.pageIdx = i;
         WinSendMsg(wnbkQueryPageHwnd(g.hwndNoteBook, g.aNBpageIDs[i]),
                    WM_COMMAND, (MPARAM)BTN_APPLY, MPVOID);
      } /* endif */
   } /* endfor */
   // if the worker thread has been started delete all the pending jobs
   // and let it terminate the main thread when the current job ends
   if (g.worker.tid) {
      // if the worker thread is executing a job delete the pending jobs
      while (g.worker.pCurJob) {
         DosSleep(1);
         while (WinPeekMsg(g.appl.hab, &qmsg, NULLHANDLE, 0, 0, PM_REMOVE))
            WinDispatchMsg(g.appl.hab, &qmsg);
      } /* endif */
      g.worker.bTerminate = TRUE;
      DosPostEventSem(g.worker.hev);
   // otherwise just post WM_QUIT to the main thread PM queue
   } else {
      WinPostMsg(g.appl.hwnd, WM_QUIT, MPVOID, MPVOID);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Registered exit function.
 Reset the unique instance handle.
- Parameters -------------------------------------------------------------
 ULONG ul : termination code. A value of 0xffff means that the procedure
            has been called directly (not by the operating system).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID APIENTRY onAppExit(ULONG ul) {
#ifndef _STLR_TEST_MODE_
   mNotifyRunTime(STLRM_CHECKINSTANCE, g.appl.hwnd, g.mode);
#endif
   if (ul != 0xffff) DosExitList(EXLST_EXIT, (PFNEXITLIST)NULL);
}


/* --------------------------------------------------------------------------
 Initialize the path of the buffer holding the name of the last file
 selected via the file dialog.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID initFileSelDlgBuffer(VOID) {
   ULONG ul;
   DosQuerySysInfo(QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, (PVOID)&ul, sizeof(ULONG));
   memcpy(g.achFileSel, "X:\\", 4);
   g.achFileSel[0] = 'A' + ul  - 1;
}


/* --------------------------------------------------------------------------
 When the setting are copied to the UNDO and CURRENT structures from the
 APPLIED data, resets the bitmap handles and information to 0.
- Parameters -------------------------------------------------------------
 PUIOPTIONS pOpts : UNDO or CURRENT user interface options.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID resetBmpHandles(PUIOPTIONS pOpts) {
   memset(&pOpts->tb.a.bmpp, 0, sizeof(BMPPAINT));
   memset(&pOpts->tb.i.bmpp, 0, sizeof(BMPPAINT));
   memset(&pOpts->btn.bmpp, 0, sizeof(BMPPAINT));
}


/* --------------------------------------------------------------------------
 Initialize the program help. In case of error just display an error message.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID initHelp(VOID) {
   HELPINIT hi;
   CHAR achTitle[256];
   memset(&hi, 0, sizeof(HELPINIT));
   hi.cb = sizeof(HELPINIT);
   hi.phtHelpTable = ((PHELPTABLE)((HLP_MAIN) | 0xffff0000));
   hi.hmodHelpTableModule = g.hResLib;
   if (loadString(1 + g.mode, achTitle))
      hi.pszHelpWindowTitle = achTitle;
   hi.fShowPanelId = CMIC_HIDE_PANEL_ID;
   hi.pszHelpLibraryName = SZ_HLPFILE;
   if (!(g.hwndHelp = WinCreateHelpInstance(g.appl.hab, &hi))) return;
   if (hi.ulReturnCode) {
      WinDestroyHelpInstance(g.hwndHelp);
      g.hwndHelp = NULLHANDLE;
      errorBox(ISERR_INITHELP);
   } else {
      WinAssociateHelpInstance(g.hwndHelp, g.appl.hwnd);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Terminate the program help.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID endHelp(VOID) {
   if (g.hwndHelp) {
      WinAssociateHelpInstance(NULLHANDLE, g.appl.hwnd);
      WinDestroyHelpInstance(g.hwndHelp);
   } /* endif */
}

