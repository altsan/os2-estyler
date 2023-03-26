/* --------------------------------------------------------------------------
 textres.h : text definitions used by dialog templates and string tables
 2004-05-29 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------
 COMMON            text strings shared among multiple dialogs
 MAINWINDOW        text displayed by the settings notebook parent
 TITLEBARGEN       general titlebar options
 TITLEBAR          titlebar background and text options
 PUSHBUTTON        pushbutton options
 DIALOG            dialog font options
 PRODINFO          product information dialog
 SHUTDOWNGEN       shutdown - general options dialog
 DLG_SDTIMING      shutdown - general options dialog
 REBOOTLIST        shutdown - reboot list options dialog
 WAITGETBOOTLIST   wait-while-searching-bootable-volumes dialog
 SHUTDOWNPROGRAMS  shutdown - shutdown programs list dialog
 SHUTDOWNKILLLIST  shutdown - list of programs to be killed on shutdown dialog
 SHUTDOWNSELECTION shutdown selection dialog
 SHUTDOWNPROGRESS  shutdown progress dialog
 WINKEY            windows keys options dialog
 PREVIEW           preview window
 STRINGRES         string resources
 ERRORMSGS         error messages string resources
-------------------------------------------------------------------------- */

#ifndef _TEXT_RES_H_
   #define _TEXT_RES_H_


/* --------------------------------------------------------------------------
 COMMON            text strings shared among multiple dialogs
*/
#define SZ__ADD               "~Add"
#define SZ__REM               "~Remove"
#define SZ__EDIT              "~Edit"
#define SZ__OK                "~OK"
#define SZ__CANCEL            "~Cancel"
#define SZ__OPEN              "~Open..."
#define SZ__SAVE              "~Save..."
#define SZ__SAVEAS            "Save ~as..."
#define SZ__EXIT              "E~xit"
#define SZ__FIND              "~Find..."
#define SZ_ADDTOTHELIST       "Add to the list"
#define SZ_FONT_              "~Font:   %s   . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."


/* --------------------------------------------------------------------------
 MAINWINDOW        text displayed by the settings notebook parent
*/
// help title
#define SZ_APPNAME               "Styler"
#define SZ_APPLY                 "~Apply"
#define SZ_UNDO                  "~Undo"
#define SZ__DEFAULT              "~Default"
#define SZ_PREVIEW               "~Preview"
#define SZ_HELP                  "Help"
// settings notebook titles
#define SZ_UITITLE               "User interface enhancements"
#define SZ_SHUTDOWNTITLE         "System shutdown"
#define SZ_WINKEYTITLE           "Windows keys"

/* --------------------------------------------------------------------------
 UIGEN             general User interface options
*/
#define SZ_UIENABLE              "~Enable user interface enhancements"
#define SZ_EXCEPTIONS            "~Do not apply enhancements to:"

/* --------------------------------------------------------------------------
 TITLEBARGEN       general titlebar options
*/
#define SZ_TBENABLE              "~Enable titlebar enhancements"
#define SZ_TBOVERRIDEPP          "~Override presentation parameters"
#define SZ__FONT                 "~Font:"
#define SZ_TEXTATLIGN            "Text ~alignment:"
#define SZ_LEFTCENTER            "Left\tCenter\t"

/* --------------------------------------------------------------------------
 TITLEBAR          titlebar background and text options
*/
#define SZ_TYPEOFBKGND           "~Type of background:"
#define SZ_COMBOBKGND            "Solid color\tGradient\tBitmap\t"
// solid color mode controls
#define SZ_BKGNDCOLOR            "~Background color"
// gradient mode controls
#define SZ_SHADEDIRECTION        "~Gradient direction:"
#define SZ_COMBOSHADE            "Horizontal\tVertical\t" \
                                 "Horizontal - double\tVertical - double\t"
#define SZ_SWAPCOLORS            "~Swap colors =>"
#define SZ_OUTERCOLOR            "~Outer color"
#define SZ_INNERCOLOR            "~Inner color"
#define SZ_LEFTCOLOR             "~Left color"
#define SZ_RIGHTCOLOR            "~Right color"
#define SZ_TOPCOLOR              "~Top color"
#define SZ_BOTTOMCOLOR           "~Bottom color"
// image mode controls
#define SZ_BKGNDIMAGE            "~Background image:"

// !!! NOTE !!! when translating the following string do not remove the
// 3 leading blank spaces !!!!!!!!
#define SZ_DEFIMAGE              "   Default image"
#define SZ_SCALEIMAGE            "~Scale the image"
//#define SZ_IMAGEOPTIONS          "~Image options..."
#define SZ_ADDIMAGE              "~Add a new image..."
// border controls
#define SZ_SHOWBORDER            "~Show the titlebar border"
#define SZ_LEFTTOPCLR            "~Left/top color"
#define SZ_RIGHTBTMCLR           "~Right/bottom color"
// text options controls
#define SZ_TEXTSHADOW            "Text ~shadow"
#define SZ_TEXTCOLOR             "~Text color"
#define SZ_TEXTSHADOWCLR         "Text s~hadow color"

/* --------------------------------------------------------------------------
 PUSHBUTTON        pushbutton options
*/
#define SZ_BTNON                 "~Pushbuttons enhancements"
#define SZ_BTNTHICK              "~Thick border"
#define SZ_BTNFLAT               "~Flat border"
#define SZ_BTNOVRCLR             "~Override non-default colors"
#define SZ_BTN3DDEF              "~3D style default buttons"
#define SZ_BTN3DDIS              "3~D style disabled buttons"
#define SZ_BTNSOLID              "~Solid color background"
//#define SZ_BKGNDIMAGE            "~Background image:"
//#define SZ_BKGNDCOLOR            "~Background color"

/* --------------------------------------------------------------------------
 DIALOG            dialog font options
*/
#define SZ_DLGOVERPP             "~Override non-default fonts"
/* see string resources below for SZ_DLGFONTON */

/* --------------------------------------------------------------------------
 PRODINFO          product information dialog
*/
#define SZ_PRODUCT               "Styler for OS/2"
#define SZ_PIVERSION             "Version %d.%d - build no. %d"
#define SZ_PICPYRIGHT            "(c) 1998-2004 Alessandro Felice Cantatore"
#define SZ_PICPYRIGHT2           "Portions (c) 2008-2022 Alexander Taylor"
#define SZ_PIURL                 "https://github.com/altsan/os2-estyler"
#define SZ_PILICENSE             "Licensed under the GNU General Public License version 3"

/* --------------------------------------------------------------------------
 SHUTDOWNGEN       shutdown - general options dialog
*/
#define SZ_SDWNON                "~Enable extended shutdown"
#define SZ_SDWNINCLUDE           "Include the following shutdown options: "
#define SZ_SDWNLKSUSP            "~Lockup and suspend"
#define SZ_SDWNSUSP              "~Suspend"
#define SZ_SDWNOFF               "~Power off"
#define SZ_SDWNORD               "Shut ~down"
#define SZ_SDWNREBOOT            "~Reboot"
#define SZ_SDWNANIMATE           "~Animate the shutdown dialog"
#define SZ_SDWNSTEPS             "Animation s~teps:"
#define SZ_SDWNADVOPTS           "Advanced ~options..."

/* --------------------------------------------------------------------------
 DLG_SDTIMING      shutdown - general options dialog
*/
#define SZ_SDTIMTITLE            "Shutdown - timing options"
#define SZ_SDTIMWARNING          "Don't change the following values unless "\
                                 "you are fully aware of the consequences."
#define SZ_SDTIMDELAY            "Delay settings (tenth of seconds) :"
#define SZ_SDTIMLKUPSUSP         "~Lockup and suspend :"
#define SZ_SDTIMSUSPEND          "~Suspend :"
#define SZ_SDTIMWARPCENTER       "Save ~WarpCenter settings :"
#define SZ_SDTIMUNROLLWIN        "~Unroll windows :"
#define SZ_SDTIMSYSMSGBOX        "System shutdown message ~boxes :"
#define SZ_SDTIMWINSD            "WinS~hutdownSystem :"
#define SZ_SDTIMDOSSD            "DosShu~tdown :"
#define SZ_SDTIMOFF              "~Power off :"
#define SZ_SDTIMKILL             "Dos~KillProcess :"
#define SZ_SDTIMOK               SZ__OK
#define SZ_SDTIMCANCEL           SZ__CANCEL
#define SZ_SDTIMDEFAULT          SZ__DEFAULT
#define SZ_SDTIMHELP             SZ_HELP

/* --------------------------------------------------------------------------
 REBOOTLIST        shutdown - reboot list options dialog
*/
#define SZ_SDWNVOLLIST           "~Allow to reboot the following volumes:"
#define SZ_SDWNVOLNAME           "~Volume name:"
#define SZ_SDWNVOLDDESCR         "Volume ~description:"
#define SZ_SDWNREBOOTADD         SZ__ADD
#define SZ_SDWNREBOOTREM         SZ__REM
#define SZ_SDWNREBOOTEDIT        SZ__EDIT
#define SZ_SDWNREBOOTFIND        "Find"

/* --------------------------------------------------------------------------
 WAITGETBOOTLIST   wait-while-searching-bootable-volumes dialog
*/
#define SZ_SDGETBOOTLIST        "Looking for bootable volumes.\x0a\x0a" \
                                "Please wait ..."

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRAMS  shutdown - shutdown programs list dialog
*/
#define SZ_SDWNRUNPROG           "~Run the following programs on shutdown:"
#define SZ_SDWNPROG              "~Program:"
#define SZ_SDWNPARMS             "Para~meters:"
#define SZ_SDWNWIN               "~Window:"
#define SZ_SDWNCOMBOWINS         "Normal\tMinimized\tMaximized\t"
#define SZ_SDWNPROGADD           SZ__ADD
#define SZ_SDWNPROGREM           SZ__REM
#define SZ_SDWNPROGEDIT          SZ__EDIT

/* --------------------------------------------------------------------------
 SHUTDOWNKILLLIST  shutdown - list of programs to be killed on shutdown dialog
*/
#define SZ_SDWNKILLPROG          "~Terminate the following programs on shutdown:"

/* --------------------------------------------------------------------------
 SHUTDOWNSELECTION shutdown selection dialog
*/
// the strings have been already defined above :
// #define SZ_SHUTDOWNTITLE         "System shutdown"
// #define SZ_SDWNLKSUSP            "~Lockup and suspend"
// #define SZ_SDWNSUSP              "~Suspend"
// #define SZ_SDWNOFF               "~Power off"
// #define SZ_SDWNORD               "S~hut down"
// #define SZ_SDWNREBOOT            "~Reboot"
#define SZ_SDWNRUNPRGRS          "~Execute shutdown programs"

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRESS  shutdown progress dialog
*/
#define SZ_SSDWNSDWNWAIT    "Please wait while the system shuts down . . ."
#define SZ_SSDWNCOMPLETE    "終了が完了しました。"
#define SZ_SSDWNREBOOTING   "The system will reboot once shutdown is complete."
#define SZ_SSDWNPWRINGOFF   "The system will power off once shutdown is "\
                            "complete."
#define SZ_SSDWNSWTCHOFF    "コンピューターの電源を切ることができます。"

/* --------------------------------------------------------------------------
 WINKEY            windows keys options dialog
*/
#define SZ_WINKEYON         "~Enable the Windows keys"
#define SZ_WKEYLEFT         "~Left Windows key:"
#define SZ_WKEYRIGHT        "~Right Windows key:"
#define SZ_WKEYMENU         "Windows ~menu key:"

/* --------------------------------------------------------------------------
 PREVIEW           preview window
*/
#define SZ_PREVIEWTITLE     "Styler - preview window"
#define SZ_STATICSAMPLE     "Static text sample"
#define SZ_BTNSAMPLES       "Push button samples: "
#define SZ__NORMAL          "~Normal"
#define SZ__DISABLED        "~Disabled"


/* --------------------------------------------------------------------------
 COLORWHEEL        Color selection dialog
*/
#define SZ_CLRWTITLE        "Color selection"
#define SZ__RED             "~Red"
#define SZ__GREEN           "~Green"
#define SZ__BLUE            "~Blue"


/* --------------------------------------------------------------------------
 STLRBMPED         Styler bitmap editor - implementation suspended
*/
// #define SZ_DLGSBMPED        "Styler bitmap editor"
// #define SZ_SBMPSCALING      "Image scaling options"
// #define SZ_SBMPTILE         "~Tile the image (no scaling)"
// #define SZ_SBMPWHOLE        "~Scale the whole image"
// #define SZ_SBMPLRBORDER     "~Left/right borders + middle part"
// #define SZ_SBMPBTBORDER     "~Bottom/top borders + middle part"
// #define SZ_SBMP9PARTS       "~Corners + borders + middle part"
// #define SZ_SBMPBCWIDTH      "Border/corner ~width:"
// #define SZ_SBMPBCHEIGHT     "Border/corner ~height:"
// #define SZ_SBMPPREVIEW      "Image preview"
// #define SZ_SBMPSAMPLEHEIGHT "Image preview height (~pixels) :"


/* --------------------------------------------------------------------------
 STRINGRES         string resources.
*/
#define SZ_PAGE1OF3         "Page 1 of 3"
#define SZ_PAGE2OF3         "Page 2 of 3"
#define SZ_PAGE3OF3         "Page 3 of 3"
#define SZ_GENOPTIONS       "General options"
#define SZ_TITLEBAR         "Titlebar"
#define SZ_ACTIVEWIN        "Active window"
#define SZ_INACTIVEWIN      "Inactive window"
#define SZ_PUSHBUTTON       "Button"
#define SZ_DIALOGOPTS       "Dialogs"
#define SZ_PRODINFO         "Product information"
#define SZ_SHUTDOWN         "Shutdown"
#define SZ_REBOOT           "Reboot"
#define SZ_SHUTDOWNPROGS    "Programs"
#define SZ_KILLLIST         "Termination list"
#define SZ_VERSION          "Version: %d.%d.%d"
#define SZ_WKEYITEM01       "Do not perform any action"
#define SZ_WKEYITEM02       "Hide/show all windows"
#define SZ_WKEYITEM03       "Show window list"
#define SZ_WKEYITEM04       "Show context menu"
#define SZ_WKEYITEM05       "Lock up the desktop"
#define SZ_WKEYITEM06       "Open the Drives folder"
#define SZ_WKEYITEM07       "Open the System Setup folder"
#define SZ_WKEYITEM08       "Open a windowed command prompt"
#define SZ_WKEYITEM09       "Show WarpCenter desktop menu"
#define SZ_WKEYITEM10       "Show WarpCenter window list"
#define SZ_WKEYITEM11       "Show WarpCenter process list"
#define SZ_WKEYITEM12       "Show WarpCenter tray list"
#define SZ_WKEYITEM13       "Switch to the next WarpCenter tray"
#define SZ_WKEYITEM14       "Show WarpCenter information menu"
#define SZ_WKEYITEM15       "Show the XButton menu (top XCenter)"
#define SZ_WKEYITEM16       "Show the XButton menu (bottom XCenter)"
#define SZ_WKEYITEM17       "Show the XCenter tray list"
#define SZ_WKEYITEM18       "Switch to the next XCenter tray"
#define SZ_WKEYITEM19       "Show XCenter tray 1"
#define SZ_WKEYITEM20       "Show XCenter tray 2"
#define SZ_WKEYITEM21       "Show XCenter tray 3"
#define SZ_WKEYITEM22       "Show XCenter tray 4"
#define SZ_WKEYITEM23       "Show XCenter tray 5"
#define SZ_WKEYITEM24       "Show XCenter tray 6"
#define SZ_WKEYITEM25       "Show XCenter tray 7"
#define SZ_WKEYITEM26       "Show XCenter tray 8"
#define SZ_WKEYITEM27       "Show XCenter tray 9"
#define SZ_WKEYITEM28       "Show XCenter tray 10"
#define SZ_WKEYITEM29       "Move the XCenter behind/on-top-of the other windows"
#define SZ_TITLEBARFONT     "Titlebar font"
#define SZ_FONTDLGSAMPLE    "Styler - sample text"
#define SZ_GRADIENTCOLOR    "%s gradient color"
#define SZ_CLRSHADELEFT     "~Left"
#define SZ_CLRSHADTOP       "~Top"
#define SZ_CLRSHADOUTER     "~Outer"
#define SZ_CLRSHADRIGHT     "~Right"
#define SZ_CLRSHADBOTTOM    "~Bottom"
#define SZ_CLRSHADINNER     "~Inner"
#define SZ_DIALOGFONT       "Dialog font"
#define SZ_DLGFONTON        "~Use %s for dialog windows"

/* --------------------------------------------------------------------------
 ERRORMSGS         error messages string resources
*/

#define SZERR_WORKERSEMAPHORE \
   "Failed to create the thread synchronization semaphore."

#define SZERR_WORKERTHREAD \
   "Failed to create the worker thread."

#define SZERR_LOADRUNTIMEMODULE \
   "Failed to load the runtime module ("SZ_MODULENAME".DLL)."

#define SZERR_QUERYPROCADDR \
   "Failed to import the runtime module procedures."

#define SZERR_GETCUROPTIONS \
   "Failed to get the current active Styler options."

#define SZERR_ALLOCATION \
   "Failed to allocate memory."

#define SZERR_OPENPRF \
   "Failed to open STYLER.INI."

#define SZERR_READPRFDATA \
   "Failed to read INI file data."

#define SZERR_INITPM \
   "Failed to initialize the Presentation Manager."

#define SZERR_REGBOXCLASS \
   "Failed to register the box/bar class control."

#define SZERR_REGXCOMBOCLASS \
   "Failed to register the enhanced drop down box control."

#define SZERR_REGCLRBTNCLASS \
   "Failed to register the color button class control."

#define SZERR_GETBMPHANDLES \
   "Failed to get the bitmap handles for the preview window."

#define SZERR_SETNOTEBOOKPAGES \
   "Failed to insert the dialog pages in the settings notebook."

#define SZERR_REGEXITLIST \
   "Failed to register the exit routine to be run on application end."

#define SZERR_TOOMANYENTRIES \
   "There are too many entries in the list. The list data cannot be saved "\
   "in the INI file. Remove some items and try again."

#define SZERR_SAVEEXCPLIST \
   "Failed to save the list of the programs which should be excluded from "\
   "the user interface enhancements."

#define SZERR_READFILE \
   "Failed to read the file: %s."

#define SZERR_INVALIDFILE \
   "The format of the file: %s is not valid."

#define SZERR_BMPTOOLARGE \
   "The bitmap : %s exceeds the 64 KBs limit."

#define SZERR_SAVEDATA \
   "Failed to save data to STYLER.INI."

#define SZERR_APPLYBMP \
   "Failed to set the selected bitmap image."

#define SZERR_GETHPSFORHBMP \
   "Get bitmap data error: failed to get the presentation space handle."

#define SZERR_GETBMPHANDLE \
   "Get bitmap data error: failed to get the bitmap handle."

#define SZERR_COPYFILE \
   "Failed to copy the file %s to %s."

#define SZERR_SOURCEEQDEST \
   "The source path is equal to the destination path."

#define SZERR_PROGLISTCOUNT \
   "The list of programs contains too many items. The last items will not "\
   "be stored."

#define SZERR_INVALIDEXE \
   "The file %s has not a valid executable extension ("".CMD"", "\
   """.COM"", "".EXE"")."

#define SZERR_INSPAGE \
   "Failed to insert a page in the settings notebook."

#define SZERR_DELPAGE \
   "Failed to delete a page in the settings notebook."

#define SZERR_GETBOTTLIST \
   "Get bootable volumes list: failed to start the worker thread job."

#define SZERR_HELPSYS \
   "\x0a" "Type HELPSYS%04u in a command line window for more informations."

#define SZERR_GBLDISKHANDLE \
   "Failed to get the disk handle."SZERR_HELPSYS

#define SZERR_GBLREADSECT1  \
   "Failed to read the first hard disk sector."SZERR_HELPSYS

#define SZERR_GBLAIRBOOTVER \
   "Invalid AiRBOOT version. Please install a newer version."

#define SZERR_GBLREADSECT2  \
   "Failed to read the disk sector containing AiRBOOT configuration data." \
   SZERR_HELPSYS

#define SZERR_GBLINVALIDDATA \
   "Invalid signature in AiRBOOT configuration data."

#define SZERR_GBLREADSECT3  \
   "Failed to read the disk sectors containing the list of bootable " \
   "AiRBOOT volumes."SZERR_HELPSYS

#define SZERR_LVMCODE \
   " (LVM error code: %d)."

#define SZERR_GBLOPENLVM    \
   "Failed to open the LVM engine"SZERR_LVMCODE

#define SZERR_GBLLVMDRVLIST \
   "Failed to get the disk list"SZERR_LVMCODE

#define SZERR_GBLLVMPARTLIST \
   "Failed to get the partition list"SZERR_LVMCODE

#define SZERR_GBLLVMVOLINFO \
   "Failed to get LVM volume data"SZERR_LVMCODE

#define SZERR_INITHELP \
   "Failed to load the application help file STYLER.HLP."

#endif // #ifndef _TEXT_RES_H_
