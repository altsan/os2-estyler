/* --------------------------------------------------------------------------
 test.h : routines used to test the preferences application without the
          runtime DLL.
 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */



#ifndef _STLR_PREF_TEST_H_
   #define _STLR_PREF_TEST_H_
   #include "stlrVersion.h"
   #include "stlrMacros.h"

PSTLRVER _stlrGetVer(VOID);
POPTIONS _stlrGetOptionsDataAddress(VOID);
HINI _stlrOpenProfile(VOID);
PSCRRES _stlrQryScreenRes(VOID);
VOID _stlrSetTitlebarGeneralDefaults(PTITLEBAR ptbo);
VOID _stlrSetTitlebarHiliteDefaults(PTBARHILITE ptbh, BOOL bActive);
VOID _stlrSetButtonDefaults(PBTNOPT pbo);
VOID _stlrSetDlgDefaults(PDLGOPTIONS pdo);
VOID _stlrSetShutdownDefaults(HINI hini, PSDGENERAL psd);
VOID _stlrSetShutdownTimingDefaults(PSDTIMING psdt);
HBITMAP _stlrShadeHBmp(HPS hps, PSHDCREATE psc, ULONG fl,
                       LONG clrLeftTop, LONG clrRightBottom);
HBITMAP _stlrHBmpFromBmpData(HPS hps, PBYTE pbmp, PBMPPAINT pbp, PBMPDAT pbd);
HBITMAP _stlrHBmpFromModule(HPS hps, HMODULE hmod, ULONG id,
                            PBMPPAINT pbp, PBMPDAT pbd);
BOOL _stlrMakeGlobalBitmap(HPS hps, HBITMAP hbmp);
VOID _stlrFreeGlobalBitmap(HPS hps, PHBITMAP phbmp, PID pid);






#endif // #ifndef _STLR_PREF_TEST_H_
