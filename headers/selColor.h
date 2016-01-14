//=========================================================================\
// edit color dialog based on the undocumented color wheel OS/2 control    |
// from an idea of Paul Ratcliffe (E-mail: paulr@orac.clara.co.uk or       |
// paul.ratcliffe@bbc.co.uk)                                               |
// coded by Alessandro Cantatore (alexcant@tin.it                          |
// version 1.1                                                             |
//=========================================================================/

// color wheel class ------------------------------------------------------
#ifndef WC_COLORWHEEL
   #define WC_COLORWHEEL               "ColorSelectClass"


//=========================================================================\
// messages:                                                               |
// WM_COLORWHEELCHANGED : mp1 = LONG RGB color                             |
//                        mp2 = MPVOID                                     |
//                        is sent by the color wheel control to its owner  |
//                        when the color wheel color changes               |
//                        Is is also sent from the "edit color" dialog to  |
//                        its owner window                                 |
// CWM_COLORWHEELSETVAL : mp1 = LONG RGB color                             |
//                        mp2 = MPVOID                                     |
//                        is sent to the color wheel to set the current    |
//                        color                                            |
//=========================================================================/


// warp 4 messages
#define WM_COLORWHEELCHANGED        0x0601
#define CWM_COLORWHEELSETVAL        0x0602


// various constants ------------------------------------------------------

#define CWDS_CENTER        0x1   // force the dialog to be centered inside
                                 // its parent window ignoring the ptl
                                 // CLRWHEEL structure member


// dlg controls -----------------------------------------------------------
#define DLG_CWHEEL                0x7100
#define CWHEEL                       100
#define TXT_CLRWRED                  101
#define SPN_CLRWRED                  102
#define TXT_CLRWGREEN                103
#define SPN_CLRWGREEN                104
#define TXT_CLRWBLUE                 105
#define SPN_CLRWBLUE                 106
#define BTN_CLRWOK                   107
#define BTN_CLRWUNDO                 108
#define BTN_CLRWCANCEL               109

#endif
