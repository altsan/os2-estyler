/* --------------------------------------------------------------------------
 uidata.h : user interface settings specific data structures and definitions
 2004-05-29 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------
 PREVIEWUPD        preview window - update flags.
 PREVIEWWND        preview window data: position/size/visibility/handle.
 BMPFILEDATA       structure used to store details of the titlebar button bmps.
 TBARHILITESETS    titlebar hiliting (i.e. active/inactive) settings.
 UIDATA            user interface mode data structure
-------------------------------------------------------------------------- */

#ifndef _UIDATA_H_
   #define _UIDATA_H_

/* --------------------------------------------------------------------------
 PREVIEWUPD        preview window - update flags.
*/
#define PVUPD_TITLEACTIVE     0x0001    // update the active title sample
#define PVUPD_TITLEINACTIVE   0x0002    // update the inactive title sample
#define PVUPD_TITLEBARS       (PVUPD_TITLEACTIVE | PVUPD_TITLEINACTIVE)
#define PVUPD_TITLEFONT       0x0004    // update the titlebar font
#define PVUPD_TITLEALL        0x0007    // update all titlebar samples
#define PVUPD_BTNNORMAL       0x0010    // update the normal button sample
#define PVUPD_BTNDEFAULT      0x0020    // update the default button sample
#define PVUPD_BTNDISABLED     0x0040    // update the disabled button sample
#define PVUPD_BTNALL          0x0070    // update all button samples
#define PVUPD_DLGFONT         0x0100    // update the dialog font
#define PVUPD_ALL             0x0177    // update everything


/* --------------------------------------------------------------------------
 PREVIEWWND        preview window data: position/size/visibility/handle.
*/
typedef struct {
   LONG x, y, cx, cy;         // window position and size
   BOOL hidden;               // true if the window is not visible
   HWND hwnd;                 // preview window handle
} PREVIEWWND, * PPREVIEWWND;


/* --------------------------------------------------------------------------
 BMPFILEDATA       structure used to store the names of the bitmap files
                   used for the titlebar and push button options.
                   This structure also stores the data of the previous
                   bitmaps for the undo feature.
                   Note:
                   when the current, applied or undo bitmap is the default
                   one, the file name is set to "", the file size and crc
                   values are set to 0 and the undo data pointer is set to
                   NULL.
*/

#pragma pack(2)
typedef struct {
   CHAR pszCur[CCHMAXPATHCOMP];  // currently selected bitmap file
   CHAR pszApplied[CCHMAXPATHCOMP]; // last applied bitmap file
   CHAR pszUndo[CCHMAXPATHCOMP];    // name of previous bitmap file (undo)
   ULONG cbCur;               // sizeof the current bitmap file
   ULONG crcCur;              // crc of the current bitmap file
   ULONG cbApplied;           // sizeof the applied bitmap file
   ULONG crcApplied;          // crc of the applied bitmap file
   ULONG cbUndo;              // size of the bitmap file data
   ULONG crcUndo;             // crc of the Undo bitmap file
   PBYTE pDataUndo;           // bitmap file
   PSZ pszNameKey;            // estyler.ini key (to store the bmp file name)
   PSZ pszDataKey;            // estyler.ini key (to store the bmp file data)
   ULONG idRes;               // resource ID of the default bitmap
   USHORT flPreview;          // update preview window flag
   USHORT updateMsg;          // update message to be sent to the runtime DLL
   HBITMAP hbitmap;
} BMPFILEDATA, *PBMPFILEDATA;
#pragma pack()


/* --------------------------------------------------------------------------
 TBARHILITESETS    titlebar hiliting (i.e. active/inactive) settings.
                   Since the active and inactive titlebar page use
                   the same dialog procedure this structure is used to
                   tell which settings each dialog refers to.
                   During dialog initialization storage is allocate to
                   store these data and the address is set in the window
                   words of the dialog windows.
*/

typedef struct {
   PTBARHILITE pSetsCur;      // current options
   PTBARHILITE pSetsApplied;  // last applied options
   PTBARHILITE pSetsUndo;     // undo options
   PBMPFILEDATA pBmp;         // bitmap file data
} TBARHILITESETS, * PTBARHILITESETS;


/* --------------------------------------------------------------------------
 UIDATA            user interface mode data structure
*/
typedef struct {
   HWND hwndActiveTbar;       // handle of the Active-titlebar page dialog
   HWND hwndInactiveTbar;     // handle of the Inactive-titlebar page dialog
   BMPFILEDATA tba;           // active titlebar bitmap data
   BMPFILEDATA tbi;           // inactive titlebar bitmap data
   BMPFILEDATA btn;           // button bitmap data
   BMPFILEDATA close;         // close button bitmap data
   BMPFILEDATA hide;          // hide button bitmap data
   BMPFILEDATA rest;          // restore button bitmap data
   BMPFILEDATA min;           // min button bitmap data
   BMPFILEDATA max;           // max button bitmap data
   PREVIEWWND preview;        // preview window data
   PUIOPTIONS pOpts;          // temp user interface options
} UIDATA, *PUIDATA;


#endif // #ifndef _UIDATA_H_
