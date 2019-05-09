/* --------------------------------------------------------------------------
 shutdown.c : command line interface shutdown for eStyler

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "shutdown.h"

// definitions --------------------------------------------------------------
#define HF_STDOUT     ((HFILE)1)
#define HF_STDERR     ((HFILE)2)

// prototypes ---------------------------------------------------------------
HWND getWorkerWindowHandle(VOID);
BOOL parseArgs(INT argc, PSZ* argv, PULONG pMode);
BOOL parseCommand(PSZ pszArg, PULONG pMode);
ULONG getRebootFlags(PSZ pszArg);
PREBOOTLIST getRebootList(VOID);
HINI stlrOpenProfile(VOID);
BOOL parseOption(PSZ pszArg, PULONG pMode);
BOOL requestShudown(HWND hwndWorker, ULONG shutdownMode);
VOID printMsg(HFILE hf, PSZ psz);
VOID showHelp(VOID);
VOID printHelp(VOID);

// global variables ---------------------------------------------------------


// entry point --------------------------------------------------------------

INT main(INT argc, PSZ* argv) {
   HWND hwndWorker;
   ULONG rc;
   ULONG shutdownMode;
   if (NULLHANDLE == (hwndWorker = getWorkerWindowHandle()))
      return RC_CLISD_GETOBJWINHANDLE;
   if (!parseArgs(argc, argv, &shutdownMode))
      return RC_CLISD_INVALIDPARMS;
   if (!(shutdownMode & SDFL_HELPREQUEST)) {
      if (!requestShudown(hwndWorker, shutdownMode))
         return RC_CLISD_ERRPOSTMSG;
   } /* endif */
   return RC_CLISD_SUCCESS;
}


/* --------------------------------------------------------------------------
 Get the handle of the object window running in the eStyler worker thread.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 HWND : handle of the the eStyler worker thread object window.
        NULLHANDLE in case of error.
-------------------------------------------------------------------------- */
static HWND getWorkerWindowHandle(VOID) {
   HMODULE hmod;
   HWND hwndWorker = NULLHANDLE;
   PSTLR_GETWORKERHWND_FN pfnGetWorkerHandle;
   // load estlrlXX.DLL
   if (!DosLoadModule(NULL, 0, SZ_MODULENAME, &hmod)) {
      if (!DosQueryProcAddr(hmod, IPROC_GETWORKERHWND,
                            NULL, (PFN *)&pfnGetWorkerHandle))
         hwndWorker = pfnGetWorkerHandle();
      DosFreeModule(hmod);
   } /* endif */
   if (!hwndWorker)
      printMsg(HF_STDERR, SZERR_WORKERHANDLE);
   return hwndWorker;
}


/* --------------------------------------------------------------------------
 Parse the input arguments.
- Parameters -------------------------------------------------------------
 INT argc     : count of input arguments.
 PSZ* argv    : arguments pointer.
 PULONG pMode : (output) argument parsing result as shutdown mode flags.
- Return value ----- ------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL parseArgs(INT argc, PSZ* argv, PULONG pMode) {
   // no parameter : ordinary shutdown as the original SHUTDOWN.EXE
   if (argc == 1) {
      *pMode = SDFL_SHUTDOWN;
   // parse the first argument and if present the second one
   } else if (!parseCommand(argv[1], pMode)
              ||
              ((argc > 2) && !parseOption(argv[2], pMode))) {
      printMsg(HF_STDERR, SZERR_INVALIDPARMS);
      return FALSE;
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 Parse the first command line argument (shutdown command).
- Parameters -------------------------------------------------------------
 PSZ pszArg   : shutdown option.
 PULONG pMode : (output) argument parsing result as shutdown mode flags.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL parseCommand(PSZ pszArg, PULONG pMode) {
   if ((*pszArg == '/') || (*pszArg == '-')) ++pszArg;
   switch (*pszArg) {
      // help request
      case '?':
      case 'H':
      case 'h':
         *pMode = SDFL_HELPREQUEST;
         showHelp();
         break;
      // interactive shutdown - show the shutdown selection dialog
      case 'd':
      case 'D':
         *pMode = SDFL_DLG;
         break;
      // lockup and suspend the system
      case 'l':
      case 'L':
         *pMode = SDFL_LOCKSUSP;
         break;
      // suspend the system
      case 's':
      case 'S':
         *pMode = SDFL_SUSP;
         break;
      // ordinary shutdonw == no command line argument
      case 'c':
      case 'C':
         *pMode = SDFL_SHUTDOWN;
         break;
      // power off the system
      case 'o':
      case 'O':
         *pMode = SDFL_OFF;
         break;
      // reboot the system
      case 'r':
      case 'R':
         *pMode = getRebootFlags(++pszArg);
         break;
      default:
         return FALSE;
   } /* endswitch */
   return TRUE;
}


/* --------------------------------------------------------------------------
 Parse REBOOT subcommands.
 If no volume to reboot to was specified or the specified volume is not
 valid the system will be rebooted to the boot manager.
- Parameters -------------------------------------------------------------
 PSZ pszArg   : shutdown option.
 PULONG pMode : (output) argument parsing result as shutdown mode flags.
- Return value -----------------------------------------------------------
 ULONG shutdown mode flags.
-------------------------------------------------------------------------- */
static ULONG getRebootFlags(PSZ pszArg) {
   PREBOOTLIST pRebootList;
   ULONG mode, iVol;
   mode = SDFL_REBOOT;
   pRebootList = getRebootList();
   // if a list of bootable volumes had already been defined
   if (pRebootList) {
      // volume index specification
      if (*pszArg == ':') {
         ++pszArg;
         // get the volume index and check if it is valid
         if ((*pszArg >= '0') && (*pszArg <= '9')) {
            iVol = strtoul(pszArg, NULL, 10);
            // if the volume index is valid update the pMode flag
            if (iVol < pRebootList->ci)
               mode |= (iVol << 16) | SDFL_REBOOTIDX;
         } /* endif */
      // volume name specification
      } else if (*pszArg) {
         // if a matching volume name is found update the pMode flag
         for (iVol = 0; iVol < pRebootList->ci; ++iVol) {
            if (!strcmp(pRebootList->aSetBootParm[iVol], pszArg)) {
               mode |= (iVol << 16) | SDFL_REBOOTIDX;
               break;
            } /* endif */
         } /* endfor */
      } /* endif */
   } /* endif */
   return mode;
}


/* --------------------------------------------------------------------------
 Get the list of the bootable volumes stored in eStylerIni.
 The format of each list item is:
 <volumeName>\t<volumeDescription>\x00
 Each item include a volume name followed by a tab character and the
 volume description which is NUL terminated.
 A void item (i.e. an item with no volume name and description, just a NUL
 character) marks the end of the list.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 PREBOOTLIST : reboot list data or NULL in case of error.
-------------------------------------------------------------------------- */
PREBOOTLIST getRebootList(VOID) {
   HINI hini;
   ULONG ul;
   PSZ psz;
   PREBOOTLIST pRebootList = NULL;
   // open estyler.ini and allocate storage to read the bootable volumes list
   if (!(hini = stlrOpenProfile())) return NULL;
   if (!PrfQueryProfileSize(hini, SZPRO_SHUTDWON, SZPRO_SDWNLIST, &ul))
      goto exit_0;
   if (NULL == (pRebootList = malloc(ul + sizeof(REBOOTLIST) - 4)))
      goto exit_0;
   // read data from estyler.ini
   if (!PrfQueryProfileData(hini, SZPRO_SHUTDWON, SZPRO_SDWNLIST,
                            pRebootList->ablist, &ul))
      goto exit_1;
   for (ul = 0, psz = pRebootList->ablist; *psz; ul++) {
      // store the SETBOOT parameter
      pRebootList->aSetBootParm[ul] = psz;
      if (!(psz = strchr(psz, '\t'))) goto exit_1;
      *psz = 0;
      // store the volume long name (description)
      pRebootList->aVolDescr[ul] = ++psz;
      // get to the next entry
      psz += strlen(psz) + 1;
   } /* endfor */
   pRebootList->ci = ul;
   goto exit_0;
exit_1:
   free(pRebootList);
   pRebootList = NULL;
exit_0:
   PrfCloseProfile(hini);
   return pRebootList;
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
 Parse the second command line argument (shutdown option).
- Parameters -------------------------------------------------------------
 PSZ pszArg   : shutdown option.
 PULONG pMode : (output) argument parsing result as shutdown mode flags.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL parseOption(PSZ pszArg, PULONG pMode) {
   if ((*pszArg == '/') || (*pszArg == '-')) ++pszArg;
   switch (*pszArg) {
      case 'e':
      case 'E':
         *pMode |= SDFL_EXECUTE;
         break;
      case 'n':
      case 'N':
         *pMode |= SDFL_NOEXECUTE;
         break;
      default:
         return FALSE;
   } /* endswitch */
   return TRUE;
}


/* --------------------------------------------------------------------------
 Post the shutdown request as a PM message to the worker tread object window.
- Parameters -------------------------------------------------------------
 HWND hwnd         : worker thread object window handle.
 ULONG shutdowMode : shutdown mode flag.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL requestShudown(HWND hwndWorker, ULONG shutdownMode) {

//   printf("shutdown flags: %08x\r\n", shutdownMode);
   if (!WinPostMsg(hwndWorker,
                   STLRM_SHUTDOWNREQ,
                   (MPARAM)hwndWorker,
                   (MPARAM)shutdownMode)) {
      printMsg(HF_STDERR, SZERR_POSTMSG);
      return FALSE;
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 Display an error message on the screen.
- Parameters -------------------------------------------------------------
 HFILE hfile : stderr or stdout.
 PSZ psz     : error message.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID printMsg(HFILE hf, PSZ psz) {
   DosPutMessage(hf, strlen(psz), psz);
}


/* --------------------------------------------------------------------------
 Open the styler.hlp page on SHUTDOWN.EXE usage.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID showHelp(VOID) {
   CHAR achHelpFile[CCHMAXPATH + 256];
   PROGDETAILS pd;
   // find estyler.hlp
   if (!DosSearchPath(SEARCH_IGNORENETERRS | SEARCH_ENVIRONMENT,
                      "HELP",
                      SZ_HLPFILE,
                      achHelpFile,
                      CCHMAXPATH)) {
      strcat(achHelpFile, " "SZ_HELPPANEL);
      memset(&pd, 0, sizeof(PROGDETAILS));
      pd.Length = sizeof(PROGDETAILS);
      pd.progt.progc = PROG_PM;
      pd.progt.fbVisible = SHE_VISIBLE;
      pd.pszExecutable = "VIEW.EXE";
      pd.pszParameters = achHelpFile;
      pd.swpInitial.hwndInsertBehind = HWND_TOP;
      pd.swpInitial.fl = SWP_SHOW | SWP_ZORDER;
      if (WinStartApp(NULLHANDLE, &pd, NULL, NULL, SAF_INSTALLEDCMDLINE))
         return;
   } /* endif */
   // Print text help if help file could not be opened
   printHelp();
}

/* --------------------------------------------------------------------------
 Print summary help information to the console.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID printHelp(VOID) {
    printf("SHUTDOWN - Command line enhanced shutdown (Styler v.1.1)\n");
    printf("(c) 2004 Alessandro Cantatore\n\n");
    printf("Syntax:\nSHUTDOWN [command] [option]\n\n");
    printf("Valid commands are:  [ ? | D | L | S | O | R[:n] ]\n");
    printf("   : (no command - default) works as the default system shutdown\n");
    printf(" ? : shows this help screen\n");
    printf(" D : shows the shutdown dialog\n");
    printf(" L : locks the desktop and puts the system in suspend mode\n");
    printf(" S : puts the system in suspend mode\n");
    printf(" O : powers the system off\n");
    printf(" R : Reboots the current volume, or the nth volume as defined in the shutdown\n");
    printf("     settings notebook\n");
    printf("\nOptions are valid only with O, R, or no command.\n");
    printf(" /E : execute programs defined through the shutdown settings notebook\n");
    printf(" /N : don't execute any defined programs\n");
    printf("If no option is specified, the currently configured shutdown settings are used.\n");
    printf("\nNote: commands and options are case insensitive.\n");
}
