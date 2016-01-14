//===========================================================================
// macros.h : eStylerLite macros
//
// --2004  - Alessandro Felice Cantatore
//===========================================================================


#ifndef _ESTYLER_MACROS_H_
   #define _ESTYLER_MACROS_H_

   #ifndef RC_INVOKED
   #endif

// offset of structure member
#ifndef offsetof
   #if __IBMC__ || __IBMCPP__
      #define offsetof( x, y ) __offsetof( x, y )
   #else
      #define offsetof(s_name, s_member) (size_t)&(((s_name *)0)->s_member)
   #endif
#endif


// never ending loop
#define forever for(;;)

#define lengthOfArray(_array_) \
   (sizeof(_array_) / sizeof((_array_)[0]))

// initialize the version number
#define m_setVersion(_p_) \
   (_p_)->major = VERMAJ, (_p_)->minor = VERMIN, (_p_)->build = VERBUILD

// write an error ID in the log file
#define m_logError(_errorID_, _returnCode_) \
   (stlrlog(_errorID_), (_returnCode_))

// return TRUE if the operating system version is greater or equal VERSUPPORTED
#define m_validVersion(_version_) \
   ((_version_) >= VERSUPPORTED)

// set the default screen resolution and color depth
#define m_setDefScreeCaps(_pScreenData_) \
   (_pScreenData_)->cx = 640,            \
   (_pScreenData_)->cy = 480,            \
   (_pScreenData_)->cclr = 256

// allocate/free memory via DosAllocMem/DosFreeMem
#define STDALLOCFLAGS    (PAG_READ | PAG_WRITE | PAG_COMMIT)
#define m_sysMemAlloc(_pBuf_, _cbAlloc_) \
   (!DosAllocMem((PPVOID)&(_pBuf_), (_cbAlloc_), STDALLOCFLAGS))

#define m_sysMemFree(_pBuf_) \
   (DosFreeMem(_pBuf_), NULL)

// delete a global bitmap
#define m_freeGlobalBitmap(_hps_, _hbmp_, _pid_)         \
   GpiSetBitmapOwner((_hps_), (_hbmp_), (_pid_)),        \
   GpiDeleteBitmap(_hbmp_),                              \
   (_hbmp_) = NULLHANDLE

// delete a bitmap handle
#define m_freeBitmap(_hbmp_) \
   if (_hbmp_) GpiDeletebitmap(_hbmp_), (_hbmp_) = 0

// create an object window
#define m_createObjectWindow(_wclass_, _hwndOwner_, _id_) \
   WinCreateWindow(HWND_OBJECT, (_wclass_), NULL, 0, 0, 0, 0, 0, \
                   (_hwndOwner_), HWND_BOTTOM, (_id_), NULL, NULL)

// ensure that the value '_val_' is >= _min_ and <= _max_
#define m_limitVal(_val_, _min_, _max_) \
   ((_val_) < (_min_)? (_min_): ((_val_) > (_max_)? (_max_): (_val_)))

// get a bitmap handle from bitmap header and bitmap data
#define m_createBmp(hps, pbh, pb) \
 ((HBITMAP)GpiCreateBitmap((hps), (PBITMAPINFOHEADER2)(pbh), CBM_INIT, \
                           (pb), (PBITMAPINFO2)(pbh)))

// set WPS object data
#define m_setWpsObject(_pszObject_, _pszSetup_) \
   (WinSetObjectData(WinQueryObject(_pszObject_), (_pszSetup_)))

// post a message to the worker thread
#define m_postWorkerMsg(_msg_, _mp1_, _mp2_) \
   WinPostMsg(g.worker.hwnd, (ULONG)(_msg_), (MPARAM)(_mp1_), (MPARAM)(_mp2_))

// check if the process is the WORKPLACE process
#define m_IsWorkPlaceProcess(_ppd_) \
   (g.pid.pmshell == (_ppd_)->pib_ulpid)

// change the PP_FONTNAMESIZE presentation parameter
#define m_setFont(_hwnd_, _font_) \
   WinSetPresParam((_hwnd_), PP_FONTNAMESIZE, strlen(_font_) + 1, (_font_))

// according to the color depth set the default titlebar background settings
// profondit… di colore clr
#define m_titleBarDefBkgnd(_clr_)      \
   (((_clr_) < 256)? TBARBKGNDDEF:              \
      (((_clr_) == 256)? TBARBKGNDBMP: TBARBKGNDSHADE))

// set the default titlebar border settings according to the background type
#define m_titleBarDefBorder(_background_)   \
      (((_background_) == TBARBKGNDSHADE) ? 0: TBO_BORDER)

// get the name of the profile application where to store the current settings
// the name depends on the color depth _cclrs_
#define m_iniAppNameFromColorDepth(_cclrs_) \
   ((_cclrs_) < 256) ? SZPRO_OPTIONS4 : \
      (((_cclrs_) > 256) ? SZPRO_OPTIONST : SZPRO_OPTIONS8)

// cast message parameter 2 to a CREATESTRUCT
#define m_creatParm(_mp_)    ((PCREATESTRUCT)(_mp_))

// cast message a parameter to a WNDPARAMS structure
#define m_wndParm(_mp_)      ((PWNDPARAMS)(_mp_))

// macro to post the special Styler/2 PM message
#define m_stlrMsgPost(_hwnd_, _mp1_, _mp2_) \
   (WinPostMsg((_hwnd_), WM_QUERYFRAMEINFO, (MPARAM)(_mp1_), (MPARAM)(_mp2_)))

// macro to send the special Styler/2 PM message
#define _stlrMsgSend(_hwnd_, _mp1_, _mp2_) \
   (WinSendMsg((_hwnd_), WM_QUERYFRAMEINFO, (MPARAM)(_mp1_), (MPARAM)(_mp2_)))

// macro to broadcast the special Styler/2 PM message
#define m_stlrMsgBroadcast(_mp1_, _mp2_) \
   (WinBroadcastMsg(HWND_DESKTOP, WM_QUERYFRAMEINFO, \
                    (MPARAM)(_mp1_), (MPARAM)(_mp2_),  \
                    BMSG_POST | BMSG_FRAMEONLY | BMSG_DESCENDANTS))


#endif // #ifndef _ESTYLER_MACROS_H_
