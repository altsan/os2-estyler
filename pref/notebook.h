/* --------------------------------------------------------------------------
 notebook.h : notebook control - utility macros.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */



#ifndef _STLRNOTEBOOK_H_
   #define _STLRNOTEBOOK_H_
#define BKA_COMMON         BKA_AUTOPAGESIZE | BKS_TABTEXTCENTER | \
                           BKA_STATUSTEXTON | BKA_MINOR

#define wnbkSetPageHWND(_hwnd_, _id_, _hwndPage_) \
   ((BOOL)WinSendMsg(_hwnd_, BKM_SETPAGEWINDOWHWND, \
                     (MPARAM)(_id_), (MPARAM)(_hwndPage_)))

#define wnbkSetTabText(_hwnd_, _id_, _text_) \
   ((BOOL)WinSendMsg(_hwnd_, BKM_SETTABTEXT, \
                     (MPARAM)(_id_), (MPARAM)(_text_)))

#define wnbkSetStatuslineText(_hwnd_, _id_, _text_) \
   ((BOOL)WinSendMsg(_hwnd_, BKM_SETSTATUSLINETEXT, \
                     (MPARAM)(_id_), (MPARAM)(_text_)))

#define wnbkDeletePage(_hwnd_, _id_) \
   ((BOOL)WinSendMsg(_hwnd_, BKM_DELETEPAGE, (MPARAM)(_id_), (MPARAM)BKA_SINGLE))

#define wnbkDeleteTab(_hwnd_, _id_) \
   ((BOOL)WinSendMsg(_hwnd_, BKM_DELETEPAGE, (MPARAM)(_id_), (MPARAM)BKA_TAB))

#define wnbkDeleteAllPages(_hwnd_) \
   ((BOOL)WinSendMsg(_hwnd_, BKM_DELETEPAGE, MPVOID, (MPARAM)BKA_ALL))

// insert a new page at position '_order_' relative to page id '_refId_'
#define wnbkInsertPage(_hwnd_, _pageStyle_, _refId_, _order_)           \
   ((ULONG)WinSendMsg(_hwnd_, BKM_INSERTPAGE, (MPARAM)(_refId_),        \
                      MPFROM2SHORT(BKA_COMMON | (_pageStyle_), (_order_))))

#define wnbkAppendPage(_hwnd_, _pageStyle_)           \
   ((ULONG)WinSendMsg(_hwnd_, BKM_INSERTPAGE, MPVOID, \
                      MPFROM2SHORT(BKA_COMMON | (_pageStyle_), BKA_LAST)))

#define wnbkSetPageInfo(_hwnd_, _idPage_, _pPageInfo_) \
   ((BOOL)WinSendMsg(_hwnd_, BKM_SETPAGEINFO,          \
                     (MPARAM)(_idPage_), (MPARAM)(_pPageInfo_)))

#define wnbkQueryPageHwnd(_hwndNotebook_, _idPage_)          \
   ((HWND)WinSendMsg(_hwndNotebook_, BKM_QUERYPAGEWINDOWHWND,\
                     (MPARAM)(_idPage_), MPVOID))

#define wnbkQueryTopPageID(_hwndNotebook_)             \
   ((ULONG)WinSendMsg(_hwndNotebook_, BKM_QUERYPAGEID, \
                      MPVOID, MPFROM2SHORT(BKA_TOP, 0)))

#define wnbkQueryTopPageHwnd(_hwndNotebook_) \
   ((HWND)wnbkQueryPageHwnd(_hwndNotebook_,  \
                            wnbkQueryTopPageID(_hwndNotebook_)))


#define wnbkSetTabColor(_hwndNotebook_, _idPage_, _color_)  \
   ((BOOL)WinSendMsg(_hwndNotebook_, BKM_SETTABCOLOR,       \
                     (MPARAM)(_idPage_), (MPARAM)(_color_)))

#define wnbkSetTabTextColor(_hwndNotebook_, _color_)        \
   ((BOOL)WinSendMsg(_hwndNotebook_, BKM_SETNOTEBOOKCOLORS, \
                     (MPARAM)(_color_), (MPARAM)BKA_FOREGROUNDMAJORCOLORINDEX))

#define wnbkSetPageHelp(_hwndHelp_, _hwndMain_, _hwndNotebook_, _hwndPage_) \
   (WinSendMsg((_hwndHelp_), HM_SET_ACTIVE_WINDOW,                          \
               (MPARAM)(_hwndPage_),                                        \
               (MPARAM)(_hwndMain_)))

#endif // #ifndef _STLRNOTEBOOK_H_
