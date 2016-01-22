// Stub wrapper for replacing APIEX000.DLL.
//
// Some of the replaced functions are reimplemented in apixwrap.c;
// others are redirected to routines in AFCUTL05.DLL.
//

#ifndef _APIXWRAP_H_
  #define _APIXWRAP_H_

#ifndef _APIEX_PM_H_


//========================================================================\
// statusbar, groupbox, and horizontal/vertical bar (line) controls       |
//========================================================================/

#define WC_STATUSBAR    "000afcccDlgStatusBar"
#define WC_3DGROUPBOX   "000afcccGroupBox"
#define WC_BAR          "000afcccBar"

BOOL APIENTRY EDBarReg(HAB hab);
BOOL APIENTRY EDGrpReg(HAB hab);
BOOL APIENTRY EDStbReg(HAB hab);

// styles ocmmon to all 3 controls:

// 3D look style:----------------------------------------------------------
#define CCTL_INDENTED          0x00   // DEFAULT
#define CCTL_RAISED            0x40
// frame height/depth: (common flags) -------------------------------------
#define CCTL_NOBORDER           0x0   // flat (DEFAULT)
#define CCTL_BORDER1            0x1   // 1 pixel high/deep
#define CCTL_BORDER2            0x2   // 2 pixels high/deep
#define CCTL_BORDER3            0x3   // 3 pixels high/deep
#define CCTL_BORDER4            0x4   // 4 pixels high/deep
#define CCTL_BORDER5            0x5   // 5 pixels high/deep
#define CCTL_BORDER6            0x6   // 6 pixels high/deep
#define CCTL_BORDER7            0x7   // 7 pixels high/deep

// styles common to groupbox and statusbar

// bordered frame:---------------------------------------------------------
#define CCTL_NOFRAME           0x00   // no frame (DEFAULT)
#define CCTL_FRAME0            0x08   // no border
#define CCTL_FRAME1            0x10   // border width = border thickness
#define CCTL_FRAME2            0x18   // border width = 2 * border thickness
#define CCTL_FRAME3            0x20   // border width = 3 * border thickness
#define CCTL_FRAME4            0x28   // border width = 4 * border thickness
#define CCTL_FRAME5            0x30   // border width = 5 * border thickness
#define CCTL_FRAME6            0x38   // border width = 6 * border thickness

// text styles defined in PMWIN.H:-----------------------------------------
//   DT_LEFT                    0x00000000
//   DT_CENTER                  0x00000100
//   DT_RIGHT                   0x00000200
//   DT_TOP                     0x00000000
//   DT_VCENTER                 0x00000400  // invalid with STBAR_GROUP
//   DT_BOTTOM                  0x00000800  // invalid with STBAR_GROUP
//   DT_MNEMONIC                0x00002000
//   DT_WORDBREAK               0x00004000  // invalid with STBAR_GROUP

// sfondo opaco/trasparente
//#define CCTL_OPAQUEBKGND       0x00000000   // deafult: sfondo col background
//#define CCTL_TRANSPARENT       0x00008000   // sfondo trasparente


// Create shortcut macros for various common PM functions and messages
#define DlgItemIsEnabled(hwnd, id)  WinIsWindowEnabled(WinWindowFromID((hwnd),(id)))
#define DlgItemEnable(hwnd, id, f)  WinEnableControl((hwnd), (id), (f))
#define DlgItemHwnd(hwnd, id)       WinWindowFromID((hwnd), (id))
#define DlgItemShow(hwnd, id, f)    WinShowWindow(WinWindowFromID((hwnd), (id)), (f))
#define DlgItemSetPos(hwnd, id, hib, x, y, cx, cy, f) \
            WinSetWindowPos( WinWindowFromID((hwnd), (id)), (hib), (x), (y), (cx), (cy), (fl))
#define WinID(hwnd)                 WinQueryWindowUShort((hwnd), QWS_ID)
#define WinOwner(hwnd)              WinQueryWindow((hwnd), QW_OWNER)
#define WinParent(hwnd)             WinQueryWindow((hwnd), QW_PARENT)
#define WinStyle(hwnd)              WinQueryWindowULong((hwnd), QWL_STYLE)
#define WinSysVal(val)              WinQuerySysValue(HWND_DESKTOP, (val))


#define dBtnCheckSet(hwnd, id, state) \
            ((USHORT)WinSendDlgItemMsg((hwnd), (id), BM_SETCHECK, MPFROMSHORT((state)), MPVOID))
#define wBtnCheckSet(hwnd, state) \
            ((USHORT)WinSendMsg((hwnd), BM_SETCHECK, MPFROMSHORT((state)), MPVOID))

#define wBtnCheckState(hwnd) \
            ((USHORT)WinSendMsg((hwnd), BM_QUERYCHECK, MPVOID, MPVOID))

#define dLbxItemCount(hwnd, id) \
            ((SHORT)(WinSendDlgItemMsg((hwnd), (id), LM_QUERYITEMCOUNT, MPVOID, MPVOID)))
#define wLbxItemCount(hwnd) \
            ((SHORT)WinSendMsg((hwnd), LM_QUERYITEMCOUNT, MPVOID, MPVOID))

#define wLbxItemHnd(hwnd, idx) \
            ((ULONG)WinSendMsg((hwnd), LM_QUERYITEMHANDLE, MPFROMSHORT(idx), MPVOID))

#define wLbxItemHndSet(hwnd, idx, hnd) \
            ((BOOL)WinSendMsg((hwnd), LM_SETITEMHANDLE, MPFROMSHORT(idx), MPFROMLONG(hnd)))

#define dLbxItemSelect(hwnd, id, idx) \
            ((BOOL)WinSendDlgItemMsg((hwnd), (id), LM_SELECTITEM, MPFROMSHORT(idx), MPFROMSHORT(TRUE)))
#define wLbxItemSelect(hwnd, idx ) \
            ((BOOL)WinSendMsg((hwnd), LM_SELECTITEM, MPFROMSHORT(idx), MPFROMSHORT(TRUE)))

#define dLbxItemSelected(hwnd, id) \
            ((SHORT)WinSendDlgItemMsg((hwnd), (id), LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), MPVOID))
#define wLbxItemSelected(hwnd) \
            ((SHORT)WinSendMsg((hwnd), LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), MPVOID))

#define wLbxItemAndHndIns(hwnd, idx, psz, hnd) \
            ((BOOL)WinSendMsg((hwnd), LM_SETITEMHANDLE, \
                              MPFROMSHORT(WinSendMsg((hwnd), LM_INSERTITEM, MPFROMSHORT(idx), MPFROMP(psz))), \
                              MPFROMP(hnd)))

#define dLbxItemTextSearch(hwnd, id, fl, idx, psz) \
            ((SHORT)WinSendDlgItemMsg((hwnd), (id), LM_SEARCHSTRING, MPFROM2SHORT((fl), (idx)), MPFROMP(psz)))
#define wLbxItemTextSearch(hwnd, fl, idx, psz) \
            ((SHORT)WinSendMsg((hwnd), LM_SEARCHSTRING, MPFROM2SHORT((fl), (idx)), MPFROMP(psz)))

#define wLbxItemText(hwnd, idx, cb, psz) \
            ((SHORT)WinSendMsg((hwnd), LM_QUERYITEMTEXT, MPFROM2SHORT((idx), (cb)), MPFROMP(psz)))

#define dLbxItemIns(hwnd, id, idx, psz) \
            ((SHORT)WinSendDlgItemMsg((hwnd), (id), LM_INSERTITEM, MPFROMSHORT(idx), MPFROMP(psz)))
#define wLbxItemIns(hwnd, idx, psz) \
            ((SHORT)WinSendMsg((hwnd), LM_INSERTITEM, MPFROMSHORT(idx), MPFROMP(psz)))

#define wLbxEmpty(hwnd) \
            ((BOOL)WinSendMsg((hwnd), LM_DELETEALL, MPVOID, MPVOID))

#define dSpinBtnSetValue(hwnd, id, val) \
            WinSendDlgItemMsg((hwnd), (id), SPBM_SETCURRENTVALUE, MPFROMLONG((val)), MPVOID)

#define RectInflate(prect, dx, dy)    (((PRECTL)(prect))->xLeft -= (dx),   \
                                       ((PRECTL)(prect))->yBottom -= (dy), \
                                       ((PRECTL)(prect))->xRight += (dx),  \
                                       ((PRECTL)(prect))->yTop += (dy))
#define RectShift(prect, dx, dy)      (((PRECTL)(prect))->xLeft += (dx),   \
                                       ((PRECTL)(prect))->yBottom += (dy), \
                                       ((PRECTL)(prect))->xRight += (dx),  \
                                       ((PRECTL)(prect))->yTop += (dy))
#define RectSet(prect, x0, y0, x1, y1) (((PRECTL)(prect))->xLeft = (x0),   \
                                        ((PRECTL)(prect))->yBottom = (y0), \
                                        ((PRECTL)(prect))->xRight = (x1),  \
                                        ((PRECTL)(prect))->yTop = (y1))
#define PointSet(ppt, px, py)         (((PPOINTL)(ppt))->x = (px),         \
                                       ((PPOINTL)(ppt))->y = (py))

#define MRFALSE       ((MRESULT)0)
#define MRTRUE        ((MRESULT)1)

#define MOUSEX(mp)    ((SHORT)(ULONG)(mp))
#define MOUSEY(mp)    ((SHORT)((ULONG)mp >> 16))

#endif  // #ifndef _APIEX_PM_H_


#ifndef APIEX_GENERIC_H_

#define ANYFILE       (FILE_ARCHIVED|FILE_SYSTEM|FILE_HIDDEN|FILE_READONLY)

#define RNDUP(n, m)   (((ULONG)(n) + (m) - 1) & ~((m) - 1))
#define RNDDWN(n, m)  ((ULONG)(n) & ~((m) - 1))

#endif  // #ifndef APIEX_GENERIC_H_


#ifndef _CTRUTIL_H_

#define CtrlUpdate(hwnd, f) \
    (WinIsWindowShowing(hwnd) ? WinInvalidateRect((hwnd), NULL, (f)) : 0)

typedef struct {
    LONG x, y, cx, cy;
} WPOSSIZE, *PWPOSSIZE;

#endif  // #ifndef _CTRUTIL_H_


// Flag values used by gpStartApp
#define GPSTART_DEFAULT     0
#define GPSTART_FULLSCREEN  1
#define GPSTART_HIDDEN      2
#define GPSTART_MAX         3

// Functions in apixwrap.c
PSZ   EXPENTRY gpReadFile(PSZ pszFile);
VOID  EXPENTRY gpFreeReadFile(PSZ pszFileContent);
HAPP  EXPENTRY gpStartApp(HWND hwnd, PSZ pszAppl, PSZ pszParm, PSZ pszPath, PSZ pszEnv, ULONG fl );

// Functions exported by afcutl05.dll
ULONG APIENTRY lockxchng(PULONG pul, ULONG val);
PVOID APIENTRY tsmalloc(ULONG cb);
VOID  APIENTRY tsfree(PVOID pv);
INT   APIENTRY tsheapmin(VOID);

extern ULONG ulsem;

#endif  // _APIXWRAP_H_
