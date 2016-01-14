/* --------------------------------------------------------------------------
 frame.h : data structures, definitions and macros used by the enhanced
           WC_FRAME window procedure.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */



#ifndef _STLR_WC_FRAME_H_
   #define _STLR_WC_FRAME_H_

// identify one of the message boxes displayed by system on shutdown
#define mIsSysShutdownBox(_pc_)                     \
   (                                                \
      g.is.replShutdownBox                          \
      ||                                            \
      (                                             \
         g.is.hookShutdown                          \
         &&                                         \
         (g.pid.protshell == pid)                   \
         &&                                         \
         !m_creatParm(_pc_)->hwndOwner              \
         &&                                         \
         (m_creatParm(_pc_)->id == 0x3e8)           \
         &&                                         \
         (m_creatParm(_pc_)->flStyle == 0x16000090) \
      )                                             \
   )

// identify the frame of the desktop folder
// NOTE: it is necessary to check that pszClass is a real string and not
// a class name defined as WC_*    ((PSZ)0xffff****L)
#define mIsDesktopFolderFrame(_pc_)                              \
   (                                                             \
      (m_creatParm(_pc_)->id == 1)                               \
      &&                                                         \
      !g.hwnd.desktopFolder                                      \
      &&                                                         \
      (m_creatParm(_pc_)->pszClass < WC_FRAME)                   \
      &&                                                         \
      !memcmp(m_creatParm(_pc_)->pszClass, "wpFolder window", 16)\
   )

// identify the frame of the launchpad
#define mIsLaunchPadFrame(_hwnd_, _pc_)                           \
   (                                                              \
      (m_creatParm(_pc_)->id == 0x807)                            \
      &&                                                          \
      (WinQueryWindowUShort((_hwnd_), QWS_FLAGS) && FF_DIALOGBOX) \
   )

// identify the (hidden) frame of the system lockup window
#define mIsSysLockupFrame(_hwnd_, _pc_)                           \
   (                                                              \
      (m_creatParm(_pc_)->id == 0x400)                            \
      &&                                                          \
      !m_creatParm(_pc_)->hwndOwner                               \
      &&                                                          \
      (m_creatParm(_pc_)->hwndParent == g.hwnd.desktop)           \
      &&                                                          \
      (WinQueryWindowUShort((_hwnd_), QWS_FLAGS) && FF_DIALOGBOX) \
   )


// extended frame flags:

#define FRM_HIDEALLMAX        0x0001    // a maximized window is minimized
                                        // by the hide-all-windows command
#define FRM_HIDEALLREST       0x0002    // a non-maximized window is minimized
                                        // by the hide-all-windows command
#define FRM_LOCKUP            0x0004    // identify the system-lockup frame
#define FRM_HIDEALL           (FRM_HIDEALLMAX | FRM_HIDEALLREST)
#define FRM_INITFONT          0x0008    // set the dialog font on the next
                                        // WM_ADJUSTWINDOWPOS-SWP_SHOW
#define FRM_STLRFONTSET       0x0010    // a dialog font has been set
#define FRM_HASOWNFONT        0x0020    // the dialog use font pres parm
#define FRM_INITFONTMASK      (FRM_INITFONT | FRM_STLRFONTSET | FRM_HASOWNFONT)

#define mSetExFrameFlags(_hwnd_, _flag_) \
   (WinSetWindowULong((_hwnd_), g.cd.frame.cbData, (_flag_)))

#define mResetExFrameFlags(_hwnd_, _flag_) \
   (WinSetWindowBits((_hwnd_), g.cd.frame.cbData, 0, (_flag_)))

#define mSetExFrameFlagMask(_hwnd_, _val_, _mask_) \
   (WinSetWindowBits((_hwnd_), g.cd.frame.cbData, (_val_), (_mask_)))

#define mIsExFlag(_hwnd_, _flag_) \
   (WinQueryWindowULong((_hwnd_), g.cd.frame.cbData) & (_flag_))

#define mExFrameFlag(_hwnd_) \
   (WinQueryWindowULong((_hwnd_), g.cd.frame.cbData))

#define mIsDeskFolderShutdownCmd(_msg_, _mp1_) \
   (((_msg_) == WM_COMMAND) && (((ULONG)(_mp1_)) == 0x2c0))

#define mIsLaunchpadShutdownCmd(_msg_, _mp1_) \
   (((_msg_) == WM_COMMAND) && (((ULONG)(_mp1_)) == 0x64))

#define mHasFont(_hwnd_, _buf_) \
   (WinQueryPresParam((_hwnd_), PP_FONTNAMESIZE, 0, NULL, \
                      sizeof(ULONG), (_buf_), QPF_NOINHERIT))


#endif // #ifndef _STLR_WC_FRAME_H_
