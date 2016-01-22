/* --------------------------------------------------------------------------
 estlrStrings.h : eStylerLite - not-to-be-translated text strings
 2004/05/08 - Alessandro Cantatore - Italy

 Contents: ---------------------------------------------------------------
 FILENAMES         module and file names
 OS2INI            application / key names stored in OS2.INI
 ESTYLER_INI       application / key names stored in ESTYLER.INI
 KILL_LIST         list of programs to be killed on shutdown
 SCREEN_RES        application/key names to get the screen res from OS2.INI
 OBJECT_SETUP      WPS ojbect setup strings
 OBJECT_IDS        WPS object IDs
 EXCEPTIONS        alias used in the exception names
 BMPPATHS          titlebar and button bitmap files relative paths.
 ERRORMSGS         hardcoded error messages (i.e. cannot load the NLS res dll)
-------------------------------------------------------------------------- */


#ifndef _ESTYLER_TEXTSTRINGS_H_
   #define _ESTYLER_TEXTSTRINGS_H_


/* --------------------------------------------------------------------------
 FILENAMES         module and file names
*/
#define SZ_MODULENAME    "ESTLRL11"         // ESTLRL11 (runtime module)
#define SZ_RESMODULE     "ESTLRLRS"         // ESTLRLRS (resources module)
#define SZ_LOGFILE       ":\\eStylerL.log"  // errors log file
#define SZPRO_PROFNAME   "ESTYLER.INI"      // profile name
#define SZ_HLPFILE       "ESTYLER.HLP"

/* --------------------------------------------------------------------------
 OS2INI            application / key names stored in OS2.INI
*/
#define SZ_ENVINI        "USER_INI"         // environment name of OS2.INI
#define SZPRO_APP        "eStylerLite"      // application name
#define SZEXC_GENERAL    "Exceptions"       // exceptions
#define SZPRO_ILAST      "LastShutdown"     // last shutdown selection
#define SZPRO_SYSFONT    "PM_SystemFonts"   // PM font application
#define SZPRO_WINTITLES  "WindowTitles"     // titlebar key
#define SZPRO_WSBOLD     "9.WarpSans Bold"  // default titlebar font

// alias used in the exception names to exclude all command line windows
#define SZXCPT_PROTSHELL          "<PROTSHELL>"

// ESTYLER.INI application / key names

/* --------------------------------------------------------------------------
 ESTYLER_INI       application / key names stored in ESTYLER.INI
*/
#define SZPRO_CLRDEPTH     "ColorDepth"     // appl: color depth
#define SZPRO_CURRENT      "Current"        // key : current color depth

#define SZPRO_OPTIONST     "OptionsT"       // appl: true color options
#define SZPRO_OPTIONS8     "Options8"       // appl: 256 colors options
#define SZPRO_OPTIONS4     "Options4"       // appl: 16 colors options

#define SZPRO_OPTIONS      "Options"        // appl: default options
//#define SZPRO_GENERAL      "General"        // key : general options
#define SZPRO_VERSION      "Version"        // key : version of options format
#define SZPRO_UINTERFACE   "UserInterface"  // key : titlebar/button options
#define SZPRO_BMPATBAR     "BmpNameATBar"   // key : active titlebar image name
#define SZPRO_BMPITBAR     "BmpNameITBar"   // key : inactive titlebar image name
#define SZPRO_BMPBUTTON    "BmpNamePushBtn" // key : pushbutton image name
#define SZPRO_BMPATBARDATA "BmpDataATBar"   // key : active titlebar image data
#define SZPRO_BMPITBARDATA "BmpDataITBar"   // key : inactive titlebar image data
#define SZPRO_BMPBTNDATA   "BmpDataPushBtn" // key : pushbutton image data

#define SZPRO_SIZEPOS      "Sizepos"        // appl: size and position
#define SZPRO_WINKEYS      "WinKeys"        // key : win-keys options dialog
#define SZPRO_SHUTDWON     "Shutdown"       // appl: (key too) shutdown options
                                            // key : size and position of
                                            //       shutdown options window
#define SZPRO_PREVIEW      "Preview"        // key : size and position of the
                                            // preview window
#define SZPRO_SDWNLIST     "BootList"       // key : reboot selection
#define SZPRO_SDWNPRGLIST  "ProgramList"    // key : list of programs to be run
#define SZPRO_KILLPROGS    "KillList"       // key : list of programs to be killed
// obsolete keys
#define SZPRO_PREF         "Preferences"    // key : preferences application
#define SZPRO_GLOBAL       "Global"         // key : options data structure
#define SZPRO_OLDBMPATBAR  "BMPATBAR"       // key : active titlebar image name
#define SZPRO_OLDBMPITBAR  "BMPITBAR"       // key : inactive titlebar image name
#define SZPRO_OLDBMPBUTTON "BMPBUTTON"      // key : pushbutton image name

/* --------------------------------------------------------------------------
 KILL_LIST
 list here the programs which must be added by default to the kill-list
 put '\x00' after each program name and modify CB_DEFKILLPRG!
*/
#define SZPRO_DEFKILLPRG \
"CHAT.EXE\x00" "CLKBASIC.EXE\x00" "CAD_POP.EXE\x00" "WPA_SUPPLICANT.EXE\x00"
#define CB_DEFKILLPRG  54        // length of previous data + termination

/* --------------------------------------------------------------------------
 SCREEN_RES        application/key names to get the screen res from OS2.INI
*/
#define PRF_SCRAPP       "PM_DISPLAYDRIVERS"
#define PRF_SCRKEY       "DEFAULTSYSTEMRESOLUTION"

/* --------------------------------------------------------------------------
 OBJECT_SETUP      WPS ojbect setup strings
*/

#define OBJ_OPENSINGLE       "CCVIEW=NO;MENUITEMSELECTED=1"
#define OBJ_OPENDEF          "MENUITEMSELECTED=1"
#define OBJ_OPENPROPERTY     "MENUITEMSELECTED=112"
#define OBJ_SAVEWC           "SAVENOW=YES"

/* --------------------------------------------------------------------------
 OBJECT_IDS        WPS object IDs
*/
#define OBJID_ECS10CLOCK     "<ECSCLOCK_CLOCK>"
#define OBJID_ECS11CLOCK     "<ECLOCK_CLKSET>"
#define OBJID_OS2CLOCK       "<WP_CLOCK>"
#define OBJID_WARPCENTER     "<WP_WARPCENTER>"
#define OBJID_DRIVES         "<WP_DRIVES>"
#define OBJID_SYSSETUP       "<WP_CONFIG>"
#define OBJID_OS2COMMAND     "<WP_OS2WIN>"

/* --------------------------------------------------------------------------
 EXCEPTIONS        alias used in the exception names
*/
#define SZXCPT_PROTSHELL          "<PROTSHELL>" // exclude command line windows

/* --------------------------------------------------------------------------
 BMPPATHS          titlebar and button bitmap files relative paths.
*/
#define SZ_BMPPATHTBAR            "BITMAP\\TBKGNDS\\"
#define SZ_BMPPATHBTN             "BITMAP\\BUTTON\\"

/* --------------------------------------------------------------------------
 DEFDLGFONT        default dialog font
*/
#define SZ_DEFDLGFONT             "9.WarpSans"

/* --------------------------------------------------------------------------
 ERRORMSGS         hardcoded error messages (i.e. cannot load the NLS res dll)
*/

#define SZERR_LOADRESMODULE \
   "Failed to load the resources module ("SZ_RESMODULE".DLL)."

#endif // #ifndef _ESTYLER_TEXTSTRINGS_H_
