/* --------------------------------------------------------------------------
 types.h : definitions of data structures used only by the eStyleLite DLL.

 2004-05-12 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------
 WPSCLASSLIST      Structure used to check the registered WPS classes.
 SHELL_PIDS        PROTSHELL and RUNWORKPLACE process IDs
 SUPERCLASSDATA    structure storing all the data of the superclassed windows
 SYSSTATE          structure used to monitor the system state
 SYSHWND           relevant window handles
 SYSPFNWP          relevant window procedures (to be subclassed)
 ACPIPFN           registered functions from ACPI32.DLL (when ACPI_ENABLED=1)
 WORKERTHREAD      worker thread data
 GLOBAL            overall global data structure
-------------------------------------------------------------------------- */


#ifndef _ESTYLER_DLLTYPES_H_
   #define _ESTYLER_DLLTYPES_H_

/* --------------------------------------------------------------------------
 WPSCLASSLIST      Structure used to check the registered WPS classes.
                   These structures are used to check if the WarpCenter is
                   a registered class by reading the OS2SYS.INI class list.
*/

typedef struct {
   LONG offNext;          // offset to the next entry
   LONG reserved;         // flag ???
   CHAR achClass[1];      // class name and DLL
} WPSCLASSENTRY, * PWPSCLASSENTRY;

#pragma pack(2)
typedef struct {
   USHORT usRes;          // always 0x0065
   USHORT cbData;         // total data size
   ULONG ulRes;           // always 0x00080000
   WPSCLASSENTRY aClass[1];
} WPSCLASSLIST, * PWPSCLASSLIST;
#pragma pack()

#define PRF_WPSCLASSESAPP     "PM_Objects"
#define PRF_WPSCLASSESKEY     "ClassTable"
#define SZ_WCENTERCLASS       "SmartCenter"
#define CB_WCENTERCLASS       12

#define PNEXTCLASS(_pclass_) \
   ((PWPSCLASSENTRY)((PBYTE)(_pclass_) + (_pclass_)->offNext))



/* --------------------------------------------------------------------------
 PROTSHELL and RUNWORKPLACE process IDs
*/
typedef struct {
   PID protshell;     // PROTSHELL process ID
   PID pmshell;       // RUNWORKPLACE process ID
} SHELL_PIDS, * PSHELL_PIDS;

/* --------------------------------------------------------------------------
 FRAMECTLBMPS default frame controls bitmap handles
*/
typedef struct {
   HBITMAP sys;        // system menu bitmap handle
   HBITMAP cls;        // close window bitmap handle
   HBITMAP min;        // minimize window bitmap handle
   HBITMAP hide;       // hide window bitmap handle
   HBITMAP rest;       // restore window bitmap handle
   HBITMAP max;        // maximize window bitmap handle
} FRAMECTLBMPS, * PFRAMECTLBMPS;

/* --------------------------------------------------------------------------
 SUPERCLASSDATA    structure storing all the data of the superclassed windows
*/
typedef struct {
   CLASSDATA frame;   // WC_FRAME data
   CLASSDATA tbar;    // WC_TITLEBAR data
   CLASSDATA btn;     // WC_BUTTON data
   CLASSDATA menu;    // WC_MENU data
} SUPERCLASSDATA, * PSUPERCLASSDATA;

/* --------------------------------------------------------------------------
 SYSSTATE          structure used to monitor the system state
*/
typedef struct {
   UINT version       : 16; // system version (minor)
   UINT bootdrv       : 8;  // boot drive letter
   UINT cywc          : 8;  // warp center height (also means that
                            // WM_BUTTON1DOWN occurred on the warpcenter)
   UINT wcRegistered  : 1;  // TRUE if
//   UINT resetWPS      : 1;  // indica che la WPS Š stata resettata
   UINT hideall       : 1;  // windows have been hidden by the Hide-All command
//   UINT wcbtn         : 1;  // flag che indica di monitorare i msg dei
                            // bottoni per riposizionare pseudomenu warpcenter
                            // sotto il mouse
//   UINT winkfldr      : 1;  // a folder was opened via the Windows keys so
//                            // intercept WM_ADJUSTWINDOWPOST and add to the
//                            // flags SWP_ACTIVATE | SWP_ZORDER
//   UINT winkshld      : 1;  // a command window was opened - proceed as above
   UINT locked        : 1;  // the system was locked up
   UINT startShutdown    : 1;  // shutdown thread started
   UINT hookShutdown     : 1;  // hook WM_DESTROY to execute the system shutdown
   UINT replShutdownBox  : 1;  // used to intercept the WM_CREATE message in
                            // WC_FRAME windows in order to replace the
                            // system shutdown dialog boxes with the eStylerLite
                            // ones.
   UINT styler2v1        : 1;  // styler2 version 1.x is installed
   UINT eCenterTop       : 1;  // move eCenter to the top or bottom
} SYSSTATE, * PSYSSTATE;

/* --------------------------------------------------------------------------
 SYSHWND           relevant window handles
*/
typedef struct {
   HWND desktop;                // HWND_DESKTOP window handle
   HWND winList;                // window list - frame handle
   volatile HWND desktopFolder; // desktop folder - frame handle
   volatile HWND warpCenter;    // warp center handle
   // do not change the order of these members !!!
   volatile HWND interfacePref; // titlebar/pushbutton options - window handle
   volatile HWND shutdownPref;  // shutdown options - window handle
   volatile HWND winkeyPref;    // windows keys options - window handle
   // end of "do not change the order of these members !!!"
   volatile HWND wkey;          // window which spotted a windows key event
   volatile HWND shutdown;      // window showed on system shutdown

} SYSHWND, * PSYSHWND;

/* --------------------------------------------------------------------------
 SYSPFNWP          relevant window procedures (to be subclassed)
*/
typedef struct {
   PFNWP desktopFolder;        // desktop folder - default system procedure
   PFNWP launchpad;            // launchpad - default system procedure
   PFNWP warpCenter;           // warp center - default system procedure
} SYSPFNWP, * PSYSPFNWP;

#ifdef ACPI_ENABLED
/* --------------------------------------------------------------------------
 ACPIPFN           registered functions from ACPI32.DLL
*/
typedef struct {
   PFN pfnAcpiStartApi;         // pointer to AcpiStartApi()
   PFN pfnAcpiPrepareToSleep;   // pointer to AcpiGoToSleep()
   PFN pfnAcpiGoToSleep;        // pointer to AcpiGoToSleep()
   PFN pfnAcpiEndApi;           // pointer to AcpiEndApi()
} ACPIPFN, * PACPIPFN;
#endif

/* --------------------------------------------------------------------------
 WORKERTHREAD      worker thread data
*/
typedef struct {
   HAB hab;            // object window anchor block handle
   HMQ hmq;            // object window message queue handle
   HWND hwnd;          // object window handle
} WORKERTHREAD, * PWORKERTHREAD;

/* --------------------------------------------------------------------------
 GLOBAL            overall global data structure
*/
typedef struct {
   SUPERCLASSDATA cd;  // superclassed window classes data
   STLRVER version;    // current version
   SHELL_PIDS pid;     // PROTOSHELL/RUNWORKPLACE process IDs
   SYSHWND hwnd;       // relevant window handles
   SYSPFNWP pfn;       // relevant window procedures
   WORKERTHREAD worker;// worker thread data
   volatile SYSSTATE is;   // global system state (only relevant data)
   CHAR achsd[24];     // previous shutdown data
#ifdef ACPI_ENABLED
   ACPIPFN acpifn;     // functions from ACPI32.DLL
   HMODULE hmodAcpi;   // handle to ACPI32.DLL
#endif
   ULONG cyIco;        // height of the titlebar
   SCRRES scr;         // screen resolution and color depth
   FRAMECTLBMPS bmpDef;// default bitmaps used by the titlebar buttons
} GLOBAL, * PGLOBAL;

/* --------------------------------------------------------------------------
 PROCDATA              process data (previously a structure now just a typedef)
*/

typedef PPIB PROCDATA;

#endif // #ifndef _ESTYLER_DLLTYPES_H_
