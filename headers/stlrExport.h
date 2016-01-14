// export.c
PSCRRES stlrQryScreenRes(VOID);
PSTLRVER stlrGetVer(void);
POPTIONS stlrGetOptionsDataAddress(VOID);
HWND stlrGetWorkerThreadHandle(VOID);
// main.c
HINI stlrOpenProfile(VOID);
VOID stlrSetTitlebarGeneralDefaults(PTITLEBAR ptbo);
VOID stlrSetTitlebarHiliteDefaults(PTBARHILITE ptbh, BOOL bActive);
VOID stlrSetButtonDefaults(PBTNOPT pbo);
VOID stlrSetDlgDefaults(PDLGOPTIONS pdo);
VOID stlrSetShutdownDefaults(HINI hini, PSDGENERAL psd);
VOID stlrSetShutdownTimingDefaults(PSDTIMING psdt);
