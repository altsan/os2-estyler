/* --------------------------------------------------------------------------
 clrButton.h : color button control.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */



#ifndef _CLR_BUTTON_H_
   #define _CLR_BUTTON_H_
   // color button class
   #define WC_CLRBUTTON       "stlrClrButton"

   // color button class registration procedure
   BOOL xClrButtonRegister(HAB hab);

   // valid button styles
   #define BCLRS_VALIDSTYLES  \
   (0xffff0000 | BS_SYSCOMMAND | BS_DEFAULT | BS_NOPOINTERFOCUS)

   // set/query color messages
   #define BM_SETCOLOR       (WM_USER)
   #define BM_QUERYCOLOR     (WM_USER + 1)

   // WM_CONTROL notification code:
   // note : message parameter 2 is the new color value
   #define BN_COLORCHANGED   100

   // set/query color macros
   #define dClrBtnColorSet(_hwnd_, _id_, _color_) \
      ((BOOL)WinSendDlgItemMsg(_hwnd_, (_id_), BM_SETCOLOR, \
                               (MPARAM)(_color_), MPVOID))
   #define wClrBtnColorSet(_hwnd_, _color_) \
      ((BOOL)WinSendMsg(_hwnd_, BM_SETCOLOR, (MPARAM)(_color_), MPVOID))

   #define dClrBtnColor(_hwnd_, _id_, _color_) \
      ((LONG)WinSendDlgItemMsg(_hwnd_, (_id_), BM_QUERYCOLOR, MPVOID, MPVOID))
   #define wClrBtnColor(_hwnd_) \
      ((LONG)WinSendMsg(_hwnd_, BM_QUERYCOLOR, MPVOID, MPVOID))

#endif // #ifndef _CLR_BUTTON_H_
