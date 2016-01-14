/* --------------------------------------------------------------------------
 macros.h : styler/2 preferences program macros.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */



#ifndef _STLRPREF_MACROS_H_
   #define _STLRPREF_MACROS_H_

// reset the correct owner of a notebook page dialog
#define mResetPageOwner(_hwndPage_) \
   WinSetOwner(_hwndPage_, g.hwndNoteBook)

// change the icon of a static control after having destroyed the previous one
#define mResetStaticIcon(_hwnd_, _idStatic_, _idIcon_) \
(WinDestroyPointer((HPOINTER)WinSendDlgItemMsg(_hwnd_, (_idStatic_),      \
                                               SM_QUERYHANDLE,            \
                                               MPVOID, MPVOID)),          \
 WinSendDlgItemMsg(_hwnd_, (_idStatic_), SM_SETHANDLE,                    \
                  (MPARAM)WinLoadPointer(HWND_DESKTOP, g.hResLib, (_idIcon_)),\
                  MPVOID))

// post a message to the worker thread of the runtime DLL
#define mNotifyRunTime(_msg_, _mp1_, _mp2_) \
   WinPostMsg(g.hwndRunTimeWorker, (ULONG)(_msg_), \
              (MPARAM)(_mp1_), (MPARAM)(_mp2_))

// TRUE if the visibility state of the "Preview" button changes when a
// notebook page is turned
#define mPreviewVisibilityChange(_iNew_, _iPrev_)       \
 ((g.aNBpageFlags[_iNew_] ^ g.aNBpageFlags[_iPrev_]) & PGFL_PREVIEW_VISIBLE)


// compare 2 strings case insensitively
#define mStringMatch(_s1_, _s2_) \
   (WCS_EQ == WinCompareStrings(g.appl.hab, 0, 0, (_s1_), (_s2_), 0))

// typically called as : <return handleError(errID, retCode)>
#define handleError(_msg_, _rc_) \
   errorBox(_msg_), (_rc_)

#define mSysColor(_iClr_) \
   (WinQuerySysColor(HWND_DESKTOP, (_iClr_), 0))

#endif // _STLRPREF_MACROS_H_
