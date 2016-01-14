/* --------------------------------------------------------------------------
 main.h : main header used for the eStylerLite preferences application.
 2004-05-29 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------
 PMTHREAD          data needed to initialize a PM thread.
 PAGEIDXS          indexes of the notebook pages.
 PAGEFLAGS         page flags (common button visibile/enabled states)
 STATEFLAGS        state flags.
 GLOBAL            global data structure.
 PROTOTYPES        function prototypes.
-------------------------------------------------------------------------- */


#ifndef _STLRPREF_H_
   #define _STLRPREF_H_
   #define INCL_WIN
   #define INCL_GPI
   #define INCL_DOS
   #define INCL_DOSDEVICES
   #define INCL_DOSDEVIOCTL
   #define INCL_DOSMISC
   #define INCL_DOSMODULEMGR
   #define INCL_DOSFILEMGR
   #define INCL_DOSPROCESS
   #define INCL_DOSNLS
   #define INCL_DOSERRORS

   #include <os2.h>
   #include <stdlib.h>
   #include <stdio.h>
   #include <string.h>

   #include "ApiEx.h"
   #include "APIexPM.h"
// ../headers directory
   #include "stlrDefs.h"          // common usage data types and constants
   #include "stlrGraphics.h"
   #include "stlrOptions.h"
   #include "resID.h"
   #include "stlrStrings.h"
   #include "stlrMacros.h"
   #include "xComboBox.h"
   #include "clrButton.h"
   #include "selColor.h"
// current directory
   #include "import.h"
   #include "macros.h"
   #include "uiData.h"
   #include "shutdownData.h"
   #include "winKeyData.h"
   #include "worker.h"
// eCS installer - headers directory
   #include "boxclass.h"
   #include "stfv.h"
   #include "ctrlutil.h"
   #include "gpParser.h"
   #include "GuidProc.h"
   #include "gpExport.h"
   #include "ClibWrap.h"

   #ifndef RC_INVOKED
   #endif

//#define _STLR_TEST_MODE_
#ifdef _STLR_TEST_MODE_
   #include "test.h"
#endif

/* --------------------------------------------------------------------------
 PMTHREAD          data needed to initialize a PM thread.
*/
typedef struct {
   HMQ hmq;        // message quueue handle
   HAB hab;        // anchor block handle
   HWND hwnd;      // thread window handle
} PMTHREAD, * PPMTHREAD;

/* --------------------------------------------------------------------------
 PAGEIDXS          indexes of the notebook pages.
*/
enum {             // User-interface settings notebook
   NBP_UIGEN,
   NBP_UITITLEBARGEN,
   NBP_UITITLEBARACTIVE,
   NBP_UITITLEBARINACTIVE,
   NBP_UIPUSHBUTTON,
   NBP_UIDIALOGS,
   NBP_UIPRODINFO,
   NBP_UICOUNT
} ;

enum {             // Shutdown settings notebook
   NBP_SDGEN,
   NBP_SDREBOOT,
   NBP_SDPROGRAMS,
   NBP_SDKILLLIST,
   NBP_SDPRODINFO,
   NBP_SDCOUNT
} ;

enum {             // Windows keys settings notebook
   NBP_WKGEN,
   NBP_WKPRODINFO,
   NBP_WKCOUNT
} ;

#define NBP_MAXCOUNT       NBP_UICOUNT

/* --------------------------------------------------------------------------
 PAGEFLAGS         page flags (common button visibile/enabled states)
*/
#define PGFL_APPLY_VISIBLE         0x01
#define PGFL_UNDO_VISIBLE          0x02
#define PGFL_DEFAULT_VISIBLE       0x04
#define PGFL_PREVIEW_VISIBLE       0x08
#define PGFL_APPLY_ENABLED         0x10
#define PGFL_UNDO_ENABLED          0x20
#define PGFL_DEFAULT_ENABLED       0x40
#define PGFL_PREVIEW_ENABLED       0x80
#define PGFL_MASK_VISIBLE          0x0f
#define PGFL_MASK_ENABLED          0xf0
// special flag used by resetCommonButton() - see util.c
#define PGFL_MASK_IGNORE           0x8000

// user interface pages initial state
#define UIPAGEFLAGS \
   "\x01" "\x8f" "\x8f" "\x8f" "\x8f" "\x8f" "\x00"
// shutdown pages initial state
#define SHDOWNPAGEFLAGS \
   "\x06" "\x03" "\x03" "\x03" "\x00"
// windows keys initial state
#define WKPAGEFLAGS \
   "\x06" "\x00"

/* --------------------------------------------------------------------------
 STATEFLAGS        state flags.
*/
#define STLRIS_SEMAPHOREOK        0x00000001   // envent semaphore created
#define STLRIS_FEATUREDISABLED    0x00000002   // features disabled
#define STLRIS_PREVIEWVISIBLE     0x00000004   // preview window visible
#define STLRIS_COLORSELDLG        0x00000008   // the color selection dialog
                                               // is displayed
#define STLRIS_COLORSWAPPED       0x00000010   // a pair of colors have been
                                               // swapped use the flag to
                                               // avoid updating the preview
                                               // window twice
#define STLRIS_SKIPNOTIFICATION   0x00000020   // ignore notification messages
#define STLRIS_INSERTINGPAGES     0x00000040   // initial page insertion

/* --------------------------------------------------------------------------
 GLOBAL            global data structure.
*/
#pragma pack(1)
typedef struct {
   PMTHREAD appl;         // main thread data
   HLIB hResLib;          // resource library handle
   HMODULE hRunLib;       // eStlrl11.dll module
   HWND hwndNoteBook;     // notebook control window handle
   HWND hwndRunTimeWorker;// handle of the worker thread
   HWND hwndHelp;         // help instance handle
   ULONG aNBpageIDs[NBP_MAXCOUNT]; // notebook page IDs
   UCHAR aNBpageFlags[NBP_MAXCOUNT]; // page flags (buttons state)
   SCRRES scr;            // current screen resolution and color depth
   POPTIONS pCurOpts;     // current options
   union {
      PUIOPTIONS pUndoUiOpts;// undo user interface options
      PSHUTDOWN pUndoSdOpts; // undo shutdown options
      PWINKEY pUndoWkOpts;   // undo windows key options
   } ;
   POINTL winPos;         // window position
   ULONG state;           // state flags
   UCHAR mode;            // operating mode (user-interface/shutdonw/winkeys)
                          // test mode used for developing and debugging
   UCHAR pageIdx;         // page index
   WORKERTHREAD worker;   // worker thread data
   union {
      PUIDATA pUiData;          // user interface settings mode - specific data
      PSHUTDOWNDATA pShdwnData; // shutdown settings mode - specific data
   } ;
   CHAR achFileSel[CCHMAXPATH]; // buffer holding the name of the last file
                                // selected via the file dialog
   #ifdef _STLR_TEST_MODE_
      SCRRES screenData;
      STLRVER version;
   #endif
} GLOBAL;
#pragma pack()

extern GLOBAL g;

/* --------------------------------------------------------------------------
 PROTOTYPES        function prototypes.
*/
// bitmaps.c
ULONG getBitmapDataJob(PGETBMPDATA pgb);
VOID freeGetBitmapDataRes(PGETBMPDATA pgb);
VOID handleGetBitmapJobError(ULONG rc, PGETBMPDATA pgb);
BOOL applyBitmap(HWND hwnd, PSZ pszPath, PSZ pszFile, PAPPLYBMP pa);
ULONG applyBitmapJob(PAPPLYBMP pa);
VOID freeApplyBitmapRes(PAPPLYBMP pa);
VOID handleApplyBitmapJobError(ULONG rc, PAPPLYBMP pa);
VOID listBitmaps(HWND hwnd, PSZ pszFileSpec,
                 PSZ pszCurBmpFile, PBMPLIMITS pBmpLimit);
ULONG listBitmapsJob(PLISTBMPFILES plbf);
VOID freeBitmapsList(PLISTBMPFILES plbf);
VOID fillBitmapList(HWND hLbox, PLISTBMPFILES plbf, BOOL bEnable);
HBITMAP stlrHBmp(HPS hps, PBYTE pData, PBMPPAINT pbp, ULONG id, PBMPDAT pbd);
VOID freeGlobalBitmap(HWND hwnd, HBITMAP hbmp);
BOOL getTitlebarHbmp(HPS hps, PBYTE pData,
                     PTBARHILITE ptbh, PSHDCREATE pshc, ULONG idRes);
BOOL getBtnHbmp(HPS hps, PBYTE pData, PBTNOPT pbtno);
BOOL addImage(HWND hwnd, PSZ pszPath, PBMPLIMITS pBmpLimit);
VOID handleAddImageJobError(ULONG rc, PADDBMPFILE pabf);
VOID addImageJobEnd(HWND hwnd, ULONG rc, PADDBMPFILE pabf);

// editcol.c
LONG editColorDlg(HWND hOwner, LONG lColor);

// dlgSDtiming.c
MRESULT EXPENTRY sdTimeOptsProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// initEnd.c
VOID parseArgs(INT argc, PSZ* argv);
BOOL initApplication(VOID);
VOID endApplication(VOID);
VOID quit(VOID);
VOID APIENTRY onAppExit(ULONG ul);
VOID initHelp(VOID);

// fontdlg.c
BOOL fontDlg(HWND hwnd, ULONG idsTitle, ULONG idsSample, PSZ pszFont);

// util.c
VOID initPage(HWND hwnd);
BOOL setCtrlText(HWND hwnd, ULONG idCtrl, ULONG idText);
BOOL setCtrlTextParm(HWND hwnd, ULONG idCtrl, ULONG idText, ...);
VOID errorBox(ULONG idMsg, ...);
ULONG loadString(ULONG id, PSZ pszBuf);
VOID resetCommonButton(ULONG idBtn, ULONG mask, ULONG reset);
VOID setCommonButtons(ULONG fl);
VOID enableNotebookTabs(HWND hwndNoteBook, BOOL bEnable);
PSZ makeFullPathName(PSZ pszBuf, PSZ pszFile);
BOOL handleSpinbtnEvent(HWND hwnd, ULONG ulEvent, PLONG pVal, LONG lCurVal);
VOID selectColor(HWND hwnd, ULONG id, LONG lColor);
VOID draw3Dborder(HPS hps, PRECTL pr, LONG clrul, LONG clrbr, ULONG cpBorder);
BOOL addFileDlg(HWND hwnd, PSZ pszFileMask);
VOID dlgItemMultiEnable(HWND hwnd, ULONG id1st, ULONG idLast, BOOL fl);
PBYTE dataDup(PBYTE pData, ULONG cbData, PULONG pCbDup);
VOID moveItem(HWND hwnd, ULONG id, INT offset);
VOID handleItemStateChange(HWND hwnd, HWND hCtrl, PSZ pszItem, ULONG handle,
                           ULONG idRemBtn, ULONG idUpBtn);
BOOL addUniqueFileName(HWND hwnd, ULONG lboxId, PSZ pszItem, INT offset);
VOID selectListItem(HWND hlbox, PSZ pszItem);
BOOL setProfileString(HINI hini, PSZ pszApp, PSZ pszKey, PSZ pData);
BOOL setProfileData(HINI hini, PSZ pszApp, PSZ pszKey, PVOID pData, ULONG cb);

// preview.c
VOID togglePreviewWindow(BOOL pageTurned);
VOID updatePreviewWindow(ULONG updateFlag);

// worker.c
BOOL startWorkerThread(VOID);
BOOL workerJobAdd(HWND hwndNotify, ULONG jobID,
                  PTHREADJOBFN pFunc, PDELJOBFN pDelFunc, PVOID pData);
VOID workerJobUnlink(VOID);

// page modules
MRESULT EXPENTRY uiGenPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY uiTbarGenPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY uiTbarPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY uiBtnPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY uiDlgPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY prodInfoPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY sdGenPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY sdRebootPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY sdProgsPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY sdKillPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY wkPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);


#endif // #ifndef _STLRPREF_H_
