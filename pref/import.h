/* --------------------------------------------------------------------------
 import.h : declaration of procedures dynamically imported from ESTLRL11.DLL.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */



#ifndef _STLR_PREF_IMPORT_H_
   #define _STLR_PREF_IMPORT_H_

// get the current styler version: PSTLRVER stlrGetVer(VOID);
typedef PSTLRVER (STLR_GETVER_FN)(VOID);
typedef STLR_GETVER_FN * PSTLR_GETVER_FN;
#define IPROC_GETVER         2
extern PSTLR_GETVER_FN stlrGetVer;

// open eStyler.ini:  HINI stlrOpenProfile(VOID)
typedef HINI (STLR_OPENPRF_FN)(VOID);
typedef STLR_OPENPRF_FN * PSTLR_OPENPRF_FN;
#define IPROC_OPENPRF        3
extern PSTLR_OPENPRF_FN stlrOpenProfile;

// get the address of the structure holding the current settings:
// POPTIONS stlrGetOptionsDataAddress(VOID);
typedef POPTIONS (STLR_GETOPTS_FN)(VOID);
typedef STLR_GETOPTS_FN * PSTLR_GETOPTS_FN;
#define IPROC_GETOPTS        4
extern PSTLR_GETOPTS_FN stlrGetOptionsDataAddress;

// get the handle of the worker thread object window of Styler/2:
// HWND stlrGetWorkerThreadHandle(VOID);
typedef HWND (STLR_GETWORKERHWND_FN)(VOID);
typedef STLR_GETWORKERHWND_FN * PSTLR_GETWORKERHWND_FN;
#define IPROC_GETWORKERHWND  5
// this is now a local variable . See initend.c - importProcedures().
// extern PSTLR_GETWORKERHWND_FN stlrGetWorkerThreadHandle;

// get the current screen resolution and color depth:
// PSCRRES stlrQryScreenRes(VOID);
typedef PSCRRES (STLR_GETSCRRES_FN)(VOID);
typedef STLR_GETSCRRES_FN * PSTLR_GETSCRRES_FN;
#define IPROC_GETSCRRES      6
extern PSTLR_GETSCRRES_FN stlrQryScreenRes;

// set the default values of the general titlebar options
// VOID stlrSetTitlebarGeneralDefaults(PTITLEBAR ptbo);
typedef VOID (SETTBGENDEF_FN)(PTITLEBAR);
typedef SETTBGENDEF_FN * PSETTBGENDEF_FN;
#define IPROC_SETTBGENDEF            7
extern PSETTBGENDEF_FN stlrSetTitlebarGeneralDefaults;

// set the default values of the active/inactive titlebar options
// VOID stlrSetTitlebarHiliteDefaults(PTBARHILITE ptbh, BOOL bActive);
typedef VOID (SETTBHILITEDEF_FN)(PTBARHILITE, BOOL);
typedef SETTBHILITEDEF_FN * PSETTBHILITEDEF_FN;
#define IPROC_SETTBHILITEDEF         8
extern PSETTBHILITEDEF_FN stlrSetTitlebarHiliteDefaults;

// set the default values of the button options
// VOID stlrSetButtonDefaults(PBTNOPT pbo);
typedef VOID (SETBTNDEF_FN)(PBTNOPT);
typedef SETBTNDEF_FN * PSETBTNDEF_FN;
#define IPROC_SETBTNDEF              9
extern PSETBTNDEF_FN stlrSetButtonDefaults;

// set the default values of the dialog options
//VOID stlrSetDlgDefaults(PDLGOPTIONS pdo);
typedef VOID (SETDLGDEF_FN)(PDLGOPTIONS);
typedef SETDLGDEF_FN * PSETDLGDEF_FN;
#define IPROC_SETDLGDEF             10
extern PSETDLGDEF_FN stlrSetDlgDefaults;

// set the default values of the general shutdown options
// VOID stlrSetShutdownDefaults(HINI hini, PSDGENERAL psd);
typedef VOID (SETSHUTDOWNDEF_FN)(HINI, PSDGENERAL);
typedef SETSHUTDOWNDEF_FN * PSETSHUTDOWNDEF_FN;
#define IPROC_SETSHUTDOWNDEF        11
extern PSETSHUTDOWNDEF_FN stlrSetShutdownDefaults;

// set the default values of the shutdown timing options
// VOID stlrSetShutdownTimingDefaults(PSDTIMING psdt);
typedef VOID (SETSDTIMINGDEF_FN)(PSDTIMING);
typedef SETSDTIMINGDEF_FN * PSETSDTIMINGDEF_FN;
#define IPROC_SETSDTIMINGDEF        12
extern PSETSDTIMINGDEF_FN stlrSetShutdownTimingDefaults;

// build a bitmap from gradient parameters
// HBITMAP stlrShadeHBmp(HPS hps, PSHDCREATE psc, ULONG fl,
//                       LONG clrLeftTop, LONG clrRightBottom);
typedef HBITMAP (HBMPSHADE_FN)(HPS, PSHDCREATE, ULONG, LONG, LONG);
typedef HBMPSHADE_FN * PHBMPSHADE_FN;
#define IPROC_HBMPSHADE             13
extern PHBMPSHADE_FN stlrShadeHBmp;

// Get a bitmap handle from the content of a bitmap file optionally
// returning the bitmap size.
// HBITMAP stlrHBmpFromBmpData(HPS hps, PBYTE pbmp, PBMPPAINT pbp, PBMPDAT pbd);
typedef HBITMAP (HBMPFROMBMPDATA_FN)(HPS, PBYTE, PBMPPAINT, PBMPDAT);
typedef HBMPFROMBMPDATA_FN * PHBMPFROMBMPDATA_FN;
#define IPROC_HBMPFROMBMPDATA       14
extern PHBMPFROMBMPDATA_FN stlrHBmpFromBmpData;

// Get a bitmap handle and optionally the bitmap size from a bitmap
// contained in a resources DLL.
// HBITMAP stlrHBmpFromModule(HPS hps, HMODULE hmod, ULONG id,
//                            PBMPPAINT pbp, PBMPDAT pbd);
typedef HBITMAP (HBMPFROMMODULE_FN)(HPS, HMODULE, ULONG, PBMPPAINT, PBMPDAT);
typedef HBMPFROMMODULE_FN * PHBMPFROMMODULE_FN;
#define IPROC_HBMPFROMMODULE        15
extern PHBMPFROMMODULE_FN stlrHBmpFromModule;

// Make a bitmap handle global.
// BOOL stlrMakeGlobalBitmap(HPS hps, HBITMAP hbmp);
typedef BOOL (MAKEGLOBALBMP_FN)(HPS, HBITMAP);
typedef MAKEGLOBALBMP_FN * PMAKEGLOBALBMP_FN;
#define IPROC_MAKEGLOBALBMP         16
extern PMAKEGLOBALBMP_FN stlrMakeGlobalBitmap;

// Free a global bitmap.
// VOID stlrFreeGlobalBitmap(HPS hps, HBITMAP phbmp, PID pid);
typedef VOID (FREEGLOBALBMP_FN)(HPS, HBITMAP, PID);
typedef FREEGLOBALBMP_FN * PFREEGLOBALBMP_FN;
#define IPROC_FREEGLOBALBMP         17
extern PFREEGLOBALBMP_FN stlrFreeGlobalBitmap;
//
//
//typedef  ()();
//typedef  * P;
//#define IPROC_
//extern  ;




#endif // _STLR_PREF_IMPORT_H_
