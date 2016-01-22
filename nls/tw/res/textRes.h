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
#define SZ__ADD               "增加(~A)"
#define SZ__REM               "移除(~R)"
#define SZ__EDIT              "編輯(~E)"
#define SZ__OK                "~OK"
#define SZ__CANCEL            "取消(~C)"
#define SZ__OPEN              "~Open..."
#define SZ__SAVE              "~Save..."
#define SZ__SAVEAS            "Save ~as..."
#define SZ__EXIT              "E~xit"
#define SZ__FIND              "尋找(~F)..."
#define SZ_ADDTOTHELIST       "Add to the list"
#define SZ_FONT_              "字型(~F)：  %s   . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."


/* --------------------------------------------------------------------------
 MAINWINDOW        text displayed by the settings notebook parent
*/
// help title
#define SZ_APPNAME               "eStyler for eComStation"
#define SZ_APPLY                 "套用(~A)"
#define SZ_UNDO                  "回復(~U)"
#define SZ__DEFAULT              "預設(~D)"
#define SZ_PREVIEW               "~Preview"
#define SZ_HELP                  "解說"
// settings notebook titles
#define SZ_UITITLE               "User interface enhancements"
#define SZ_SHUTDOWNTITLE         "系統關機"
#define SZ_WINKEYTITLE           "視窗按鍵"

/* --------------------------------------------------------------------------
 UIGEN             general User interface options
*/
#define SZ_UIENABLE              "~Enable user interface enhancements"
#define SZ_EXCEPTIONS            "~Do not apply enhancements to:"

/* --------------------------------------------------------------------------
 TITLEBARGEN       general titlebar options
*/
#define SZ_TBENABLE              "~Enable titlebar enhancements"
#define SZ_TBOVERRIDEPP          "覆寫展示的參數(~O)"
#define SZ__FONT                 "字型(~F)："
#define SZ_TEXTATLIGN            "Text ~alignment:"
#define SZ_LEFTCENTER            "左\t置中\t"

/* --------------------------------------------------------------------------
 TITLEBAR          titlebar background and text options
*/
#define SZ_TYPEOFBKGND           "~Type of background:"
#define SZ_COMBOBKGND            "純色\t漸層\t點陣圖\t"
// solid color mode controls
#define SZ_BKGNDCOLOR            "~Background color"
// gradient mode controls
#define SZ_SHADEDIRECTION        "~Gradient direction:"
#define SZ_COMBOSHADE            "Horizontal\tVertical\t" \
                                 "Horizontal - double\tVertical - double\t"
#define SZ_SWAPCOLORS            "交換顏色(~S) =>"
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
#define SZ_BTNON                 "加強型的按鈕(~P)"
#define SZ_BTNTHICK              "厚邊框(~T)"
#define SZ_BTNFLAT               "浮動邊框(~F)"
#define SZ_BTNOVRCLR             "~Override non-default colors"
#define SZ_BTN3DDEF              "3D 樣式的預設按鈕(~3)"
#define SZ_BTN3DDIS              "3D 樣式的停用按鈕(~D)"
#define SZ_BTNSOLID              "純色背景(~S)"
//#define SZ_BKGNDIMAGE            "~Background image:"
//#define SZ_BKGNDCOLOR            "~Background color"

/* --------------------------------------------------------------------------
 DIALOG            dialog font options
*/
#define SZ_DLGFONTON             "~Set a default font for the dialog windows"
#define SZ_DLGOVERPP             "~Override non-default fonts"

/* --------------------------------------------------------------------------
 PRODINFO          product information dialog
*/
#define SZ_PRODUCT               "eStyler for eComStation"
#define SZ_PIVERSION             "Version %d.%d - build no. %d"
#define SZ_PICPYRIGHT            "(C) 1998-2004 Alessandro Felice Cantatore"
#define SZ_PIURL                 "http://xoomer.virgilio.it/acantato"

/* --------------------------------------------------------------------------
 SHUTDOWNGEN       shutdown - general options dialog
*/
#define SZ_SDWNON                "~Enable extended shutdown"
#define SZ_SDWNINCLUDE           "Include the following shutdown options: "
#define SZ_SDWNLKSUSP            "鎖住及休眠(~L)"
#define SZ_SDWNSUSP              "暫停(~S)"
#define SZ_SDWNOFF               "關閉電源(~P)"
#define SZ_SDWNORD               "關機(~D)"
#define SZ_SDWNREBOOT            "重新開機(~R)"
#define SZ_SDWNANIMATE           "關機對話框以動畫出現(~A)"
#define SZ_SDWNSTEPS             "Animation s~teps:"
#define SZ_SDWNADVOPTS           "Advanced ~options..."

/* --------------------------------------------------------------------------
 DLG_SDTIMING      shutdown - general options dialog
*/
#define SZ_SDTIMTITLE            "Shutdown - timing options"
#define SZ_SDTIMWARNING          "Don't change the following values unless "\
                                 "you are fully aware of the consequences."
#define SZ_SDTIMDELAY            "Delay settings (tenth of seconds) :"
#define SZ_SDTIMLKUPSUSP         "鎖住及休眠(~L)："
#define SZ_SDTIMSUSPEND          "暫停(~S)："
#define SZ_SDTIMWARPCENTER       "Save ~WarpCenter settings :"
#define SZ_SDTIMUNROLLWIN        "~Unroll windows :"
#define SZ_SDTIMSYSMSGBOX        "System shutdown message ~boxes :"
#define SZ_SDTIMWINSD            "WinS~hutdownSystem："
#define SZ_SDTIMDOSSD            "DosShu~tdown："
#define SZ_SDTIMOFF              "關閉電源(~P)："
#define SZ_SDTIMKILL             "Dos~KillProcess："
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
#define SZ_SDWNREBOOTFIND        "尋找"

/* --------------------------------------------------------------------------
 WAITGETBOOTLIST   wait-while-searching-bootable-volumes dialog
*/
#define SZ_SDGETBOOTLIST        "Looking for bootable volumes.\x0a\x0a" \
                                "Please wait ..."

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRAMS  shutdown - shutdown programs list dialog
*/
#define SZ_SDWNRUNPROG           "關機時執行以下的程式(~R)："
#define SZ_SDWNPROG              "程式(~P)："
#define SZ_SDWNPARMS             "參數(~M)："
#define SZ_SDWNWIN               "視窗(~W)："
#define SZ_SDWNCOMBOWINS         "正常\t最小化\t最大化\t"
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
#define SZ_SDWNRUNPRGRS          "執行關機程式(~E)"

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRESS  shutdown progress dialog
*/
#define SZ_SSDWNSDWNWAIT    "系統關機中，請稍後… . . ."
#define SZ_SSDWNCOMPLETE    "完成關機。"
#define SZ_SSDWNREBOOTING   "關機完成後，系統將會重新啟動。"
#define SZ_SSDWNPWRINGOFF   "關機完成後，系統將會關閉"\
                            "電源。"
#define SZ_SSDWNSWTCHOFF    "你可以安全地關閉電源了。"

/* --------------------------------------------------------------------------
 WINKEY            windows keys options dialog
*/
#define SZ_WINKEYON         "~Enable the Windows keys"
#define SZ_WKEYLEFT         "左視窗按鍵(~L)："
#define SZ_WKEYRIGHT        "右視窗按鍵(~R)："
#define SZ_WKEYMENU         "視窗選單按鍵(~M)："

/* --------------------------------------------------------------------------
 PREVIEW           preview window
*/
#define SZ_PREVIEWTITLE     "eStyler - preview window"
#define SZ_STATICSAMPLE     "Static text sample"
#define SZ_BTNSAMPLES       "Push button samples: "
#define SZ__NORMAL          "正常(~N)"
#define SZ__DISABLED        "停用(~D)"


/* --------------------------------------------------------------------------
 COLORWHEEL        Color selection dialog
*/
#define SZ_CLRWTITLE        "選擇顏色"
#define SZ__RED             "紅(~R)"
#define SZ__GREEN           "綠(~G)"
#define SZ__BLUE            "藍(~B)"


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
#define SZ_PAGE1OF3         "第 1／3 頁"
#define SZ_PAGE2OF3         "第 2／3 頁"
#define SZ_PAGE3OF3         "第 3／3 頁"
#define SZ_GENOPTIONS       "一般選項"
#define SZ_TITLEBAR         "標題欄"
#define SZ_ACTIVEWIN        "作用中視窗"
#define SZ_INACTIVEWIN      "非作用中視窗"
#define SZ_PUSHBUTTON       "Button"
#define SZ_DIALOGOPTS       "Dialogs"
#define SZ_PRODINFO         "產品資訊"
#define SZ_SHUTDOWN         "關機"
#define SZ_REBOOT           "重新開機"
#define SZ_SHUTDOWNPROGS    "Programs"
#define SZ_KILLLIST         "Termination list"
#define SZ_VERSION          "Version: %d.%d.%d"
#define SZ_WKEYITEM01       "不執行任何動作"
#define SZ_WKEYITEM02       "隱藏／顯示所有視窗"
#define SZ_WKEYITEM03       "顯示視窗清單"
#define SZ_WKEYITEM04       "顯示蹦現選單"
#define SZ_WKEYITEM05       "鎖住桌面"
#define SZ_WKEYITEM06       "開啟磁碟機資料夾"
#define SZ_WKEYITEM07       "開啟系統設定資料夾"
#define SZ_WKEYITEM08       "開啟一個命令列視窗"
#define SZ_WKEYITEM09       "顯示 eComCenter 桌面選單"
#define SZ_WKEYITEM10       "顯示 eComCenter 視窗清單"
#define SZ_WKEYITEM11       "顯示 eComCenter 進程清單"
#define SZ_WKEYITEM12       "顯示 eComCenter 置物匣清單"
#define SZ_WKEYITEM13       "切換到下一個 eComCenter 置物匣"
#define SZ_WKEYITEM14       "顯示 eComCenter 資訊選單"
#define SZ_WKEYITEM15       "Show the eCS menu (top eCenter)"
#define SZ_WKEYITEM16       "Show the eCS menu (bottom eCenter)"
#define SZ_WKEYITEM17       "Show the eCenter tray list"
#define SZ_WKEYITEM18       "Switch to the next eCenter tray"
#define SZ_WKEYITEM19       "Show eCenter tray 1"
#define SZ_WKEYITEM20       "Show eCenter tray 2"
#define SZ_WKEYITEM21       "Show eCenter tray 3"
#define SZ_WKEYITEM22       "Show eCenter tray 4"
#define SZ_WKEYITEM23       "Show eCenter tray 5"
#define SZ_WKEYITEM24       "Show eCenter tray 6"
#define SZ_WKEYITEM25       "Show eCenter tray 7"
#define SZ_WKEYITEM26       "Show eCenter tray 8"
#define SZ_WKEYITEM27       "Show eCenter tray 9"
#define SZ_WKEYITEM28       "Show eCenter tray 10"
#define SZ_WKEYITEM29       "Move the eCenter behind/on-top-of the other windows"
#define SZ_TITLEBARFONT     "標題欄字型"
#define SZ_FONTDLGSAMPLE    "eStyler - sample text"
#define SZ_GRADIENTCOLOR    "%s gradient color"
#define SZ_CLRSHADELEFT     "左(~L)"
#define SZ_CLRSHADTOP       "頂(~T)"
#define SZ_CLRSHADOUTER     "外層(~O)"
#define SZ_CLRSHADRIGHT     "右(~R)"
#define SZ_CLRSHADBOTTOM    "底(~B)"
#define SZ_CLRSHADINNER     "內層(~I)"
#define SZ_DIALOGFONT       "Dialog font"

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
   "Failed to get the current active eStyler options."

#define SZERR_ALLOCATION \
   "Failed to allocate memory."

#define SZERR_OPENPRF \
   "Failed to open ESTYLER.INI."

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
   "Failed to save data to ESTYLER.INI."

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
   "Failed to read the disk sector contating AiRBOOT configuration data." \
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
   "Failed to load the application help file eStyler.hlp."

#endif // #ifndef _TEXT_RES_H_
