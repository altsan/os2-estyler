/* --------------------------------------------------------------------------
 resID.h : resources IDs.
 2004-05-29 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------
 PAGESIZE          standard notebook page size
 MAIN_WINDOW       id of the controls of the main dialog window
 UIGEN             user interface enhancements - general options controls ID
 TITLEBARGEN       general titlebar options page - ID of the controls
 TITLEBAR          active/inactive titlebar options page - ID of the controls
 PUSHBUTTON        pushbutton options page - ID of the controls
 DIALOG            dialog font options page - ID of the controls
 PRODINFO          product information dialog
 SHUTDOWNGEN       shutdown - general options dialog
 SHUTDOWNTIMING    shutdown - general options dialog
 REBOOTLIST        shutdown - reboot list options dialog
 WAITGETBOOTLIST   wait-while-searching-bootable-volumes dialog
 SHUTDOWNPROGRAMS  shutdown - shutdown programs list dialog
 SHUTDOWNKILLLIST  shutdown - list of programs to be killed on shutdown dialog
 SHUTDOWNSELECTION shutdown selection dialog
 SHUTDOWNPROGRESS  shutdown progress dialog
 WINKEY            windows keys options dialog
 PREVIEW           preview window IDs
 STLRBMPED         Styler bitmap editor
 STRINGIDS         string IDs
 ERRMSGSSTRIDS     error messages string IDs
 HELP_IDS          help table/item/subtable IDs
-------------------------------------------------------------------------- */

#ifndef _STLR_RESOURCE_IDS_H_
   #define _STLR_RESOURCE_IDS_H_


/* --------------------------------------------------------------------------
 PAGESIZE          standard notebook page size
*/
#define CX_PAGEDLG               250
#define CY_PAGEDLG               140

/* --------------------------------------------------------------------------
 MAIN_WINDOW       id of the controls of the main dialog window
*/
#define DLG_MAIN                   1
#define PREF_NOTEBOOK          10000
#define BTN_APPLY              10001
#define BTN_UNDO               10002
#define BTN_DEFAULT            10003
#define BTN_PREVIEW            10004
#define BTN_HELP               10005

/* --------------------------------------------------------------------------
 UIGEN             user interface enhancements - general options controls ID
*/
#define DLG_UIGENERAL            100
#define CHK_UIENABLE             100
#define GRP_UIEXCP               101
#define LBOX_UIEXCP              102
#define EF_UIEXCP                103
#define BTN_UIEXCPADD            104
#define BTN_UIEXCPDEL            105
#define BTN_UIEXCPMORE           106

/* --------------------------------------------------------------------------
 TITLEBARGEN       general titlebar options page - ID of the controls
*/
#define DLG_TITLEBARGEN          101
#define CHK_TBENABLE             100
#define CHK_OVERRIDEPP           101
#define TXT_TBFONT               102
#define BTN_TBFONT               103
#define TXT_TBALIGN              104
#define COMBO_TBALIGN            105
#define TXT_THEME                106
#define COMBO_THEME              107

/* --------------------------------------------------------------------------
 TITLEBAR          active/inactive titlebar options page - ID of the controls
*/
#define DLG_TITLEBAR             102     // dialog id
#define TXT_TBBKGNDTYPE          100     // background selection controls
#define COMBO_TBBKGND            101
#define TXT_TBBKGNDCLR           102     // solid color mode controls
#define BTN_TBSOLID              103
#define TXT_TBDIRECTION          104     // gradient mode controls
#define COMBO_TBSHADE            105
#define BTN_TBSWAPSHADE          106
#define TXT_TBSHADECLR1          107
#define BTN_TBSHADECLR1          108
#define TXT_TBSHADECLR2          109
#define BTN_TBSHADECLR2          110
#define TXT_TBIMAGE              111     // image mode controls
#define COMBO_TBIMAGE            112
#define CHK_TBSCALE              113
//#define BTN_TBIMAGE              113
#define BTN_TBADDIMAGE           114
#define BAR_TB1                  115
#define CHK_TBBORDER             116     // titlebar border controls
#define BTN_TBSWAPBORDER         117
#define TXT_TBLEFTTOP            118
#define BTN_TBLEFTTOP            119
#define TXT_TBRIGHTBTM           120
#define BTN_TBRIGHTBTM           121
#define BAR_TB2                  122
#define CHK_TBSHADOW             123     // titlebar text controls
#define BTN_TBSWAPTXT            124
#define TXT_TBTXTCLR             125
#define BTN_TBTXTCLR             126
#define TXT_TBSHADOWCLR          127
#define BTN_TBSHADOWCLR          128

/* --------------------------------------------------------------------------
 PUSHBUTTON        pushbutton options page - ID of the controls
*/
#define DLG_BTNOPT               110
#define CHK_BTNON                100
#define CHK_BTNTHICK             101
#define CHK_BTNFLAT              102
#define CHK_BTNOVRCLR            103
#define CHK_BTN3DDEF             104
#define CHK_BTN3DDIS             105
#define CHK_BTNSOLID             106
#define TXT_BTNIMAGE             107
#define COMBO_BTNIMAGE           108
#define BTN_BTNADDIMAGE          109
#define TXT_BTNBKGNDCLR          110
#define BTN_BTNBKGNDCLR          111

/* --------------------------------------------------------------------------
 DIALOG            dialog font options page - ID of the controls
*/
#define DLG_DIALOG               120
#define CHK_DLGFONTON            100
#define CHK_DLGOVERPP            101
#define TXT_DLGFONT              102
#define BTN_DLGFONT              103

/* --------------------------------------------------------------------------
 PRODINFO          product information dialog
*/
#define DLG_PRODINFO             200
#define ICO_PRODINFO             100
#define TXT_PIPRODUCT            101
#define TXT_PIVERSION            102
#define BAR_PRODINFO             103
#define TXT_PICPYRIGHT           104
#define BTN_PIURL                105
#define TXT_PICPYRIGHT2          106
#define TXT_LICENSE              107

/* --------------------------------------------------------------------------
 SHUTDOWNGEN       shutdown - general options dialog
*/
#define DLG_SDWNGEN              300
#define CHK_SDWNON               100
#define GRP_SDWNINCLUDE          101
#define CHK_SDWNLKSUSP           102
#define CHK_SDWNSUSP             103
#define CHK_SDWNOFF              104
#define CHK_SDWNORD              105
#define CHK_SDWNREBOOT           106
#define CHK_SDWNANIMATE          107
#define TXT_SDNWSTEPS            108
#define SPIN_SDWNSTEPS           109
#define BTN_SDWNADVANCDED        110

/* --------------------------------------------------------------------------
 SHUTDOWNTIMING    shutdown - general options dialog
*/
#define DLG_SDTIMING             310
#define TXT_SDTIMWARNING         100
#define TXT_SDTIMLKUPSUSP        101
#define SPIN_SDTIMLKUPSUSP       102
#define TXT_SDTIMSUSPEND         103
#define SPIN_SDTIMSUSPEND        104
#define TXT_SDTIMWARPCENTER      105
#define SPIN_SDTIMWARPCENTER     106
#define TXT_SDTIMUNROLLWIN       107
#define SPIN_SDTIMUNROLLWIN      108
#define TXT_SDTIMSYSMSGBOX       109
#define SPIN_SDTIMSYSMSGBOX      110
#define TXT_SDTIMWINSD           111
#define SPIN_SDTIMWINSD          112
#define TXT_SDTIMDOSSD           113
#define SPIN_SDTIMDOSSD          114
#define TXT_SDTIMOFF             115
#define SPIN_SDTIMOFF            116
#define BTN_SDTIMOK              DID_OK
#define BTN_SDTIMCANCEL          DID_CANCEL
#define BTN_SDTIMDEFAULT         117
#define BTN_SDTIMHELP            118
#define TXT_SDTIMKILL            119
#define SPIN_SDTIMKILL           120
#define GRP_SDTI                 0xffff

/* --------------------------------------------------------------------------
 REBOOTLIST        shutdown - reboot list options dialog
*/
#define DLG_SDWNBOOTLIST         301
#define TXT_VOLLIST              100
#define BTN_SDWNREBOOTUP         101
#define BTN_SDWNREBOOTDOWN       102
#define LBX_SDWNREBOOTLIST       103
#define TXT_SDWNVOLNAME          104
#define EF_SDWNVOLNAME           105
#define TXT_SDWNVOLDESCR         106
#define EF_SDWNVOLDESCR          107
#define BTN_SDWNREBOOTADD        108
#define BTN_SDWNREBOOTREM        109
#define BTN_SDWNREBOOTEDIT       110
#define BTN_SDWNREBOOTFIND       111

/* --------------------------------------------------------------------------
 WAITGETBOOTLIST   wait-while-searching-bootable-volumes dialog
*/
#define DLG_SDGETBOOLLIST        311

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRAMS  shutdown - shutdown programs list dialog
*/
#define DLG_SDWNPROGS            302
#define CHK_SDWNRUNPROG          100
#define BTN_SDWNPROGUP           101
#define BTN_SDWNPROGDOWN         102
#define LBX_SDWNPROG             103
#define TXT_SDWN2PROG            104
#define EF_SDWNPROGNAME          105
#define TXT_SDWNPROGPARMS        106
#define EF_SDWNPROGPARMS         107
#define TXT_SDWNPROGWIN          108
#define COMBO_SDWNPROG           109
#define BTN_SDWNPROGADD          110
#define BTN_SDWNPROGREM          111
#define BTN_SDWNPROGEDIT         112
#define BTN_SDWNPROGFIND         113

/* --------------------------------------------------------------------------
 SHUTDOWNKILLLIST  shutdown - list of programs to be killed on shutdown dialog
*/
#define DLG_SDWNKILLPROG         303
#define CHK_SDWNKILLPROG         100
#define LBX_SDWNKILLPROG         101
#define EF_SDWNKILLPROG          102
#define BTN_SDWNKILLADD          103
#define BTN_SDWNKILLDEL          104
#define BTN_SDWNKILLEDIT         105
#define BTN_SDWNKILLMORE         106

/* --------------------------------------------------------------------------
 SHUTDOWNSELECTION shutdown selection dialog
*/
#define DLG_SHUTDOWNREQ           10
#define RDSSDWN_LOCKSUSP         100
#define RDSSDWN_SUSP             101
#define RDSSDWN_OFF              102
#define RDSSDWN_ORD              103
#define RDSSDWN_REBOOT           104
#define COMBOSSDWN               105
#define CHK_RUNPROGS             106
#define BTNSSDWN_OK              107
#define BTNSSDWN_CANC            108
#define GRP_SDWN                 109

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRESS  shutdown progress dialog
*/
#define DLG_LAST              0x1111
#define ICO_LAST              0x2000
#define TXT_LASTSHDWN         0x2001
#define TXT_LASTREBOOT        0x2002
#define TXT_LASTPWROFF        0x2003
#define TXT_LASTSWTCHOFF      0x2004
#define TXT_LASTSDCOMPLETE    0x2005

#define TXT_LASTSDHIDE        0x2009

/* --------------------------------------------------------------------------
 WINKEY            windows keys options dialog
*/
#define DLG_WINKEY               400
#define CHK_WINKEYON             100
#define TXT_WKEYLEFT             101
#define COMBO_WKLEFT             102
#define TXT_WKEYRIGHT            103
#define COMBO_WKRIGHT            104
#define TXT_WKEYMENU             105
#define COMBO_WKMENU             106

/* --------------------------------------------------------------------------
 PREVIEW           preview window IDs
*/
#define ID_PREVIEWMAIN             1      // main frame
#define ID_PREVIEWACTIVE           2      // active sample frame
#define ID_PREVIEWINACTIVE         3      // inactive sample frame

#define ID_PVTEXT                100      // static text sample
#define ID_PVGRP                 101      // groupbox sample
#define ID_PVBTNNORMAL           102      // normal button
#define ID_PVBTNDEFAULT          103      // default button
#define ID_PVBTNDISABLED         104      // disabled button

/* --------------------------------------------------------------------------
 STLRBMPED         Styler bitmap editor
*/
#define DLG_SBMPED               900
#define GRP_SBMPSCALING          100
#define RDBTN_SBMPTILE           101
#define RDBTN_SBMPWHOLE          102
#define RDBTN_SBMPLRBORDER       103
#define RDBTN_SBMPBTBORDER       104
#define RDBTN_SBMP9PARTS         105
#define TXT_SBMPBCWIDTH          106
#define SPIN_SBMPBCWIDTH         107
#define TXT_SBMPBCHEIGHT         108
#define SPIN_SBMPBCHEIGHT        109
#define GRP_SBMPPREVIEW          110
#define SBMP_SBMPSAMPLE          111
#define TXT_SBMPSAMPLEHEIGHT     112
#define SPIN_SBMPSAMPLEHEIGHT    113
#define BTN_SBMPSAVE             114
#define BTN_SBMPSAVEAS           115
#define BTN_SBMPCANCEL           116
#define BTN_SBMPHELP             117

/* --------------------------------------------------------------------------
 STRINGIDS         string IDs
*/
#define IDS_UITITLE                1    // "User interface enhancements"
#define IDS_SHUTDOWNTITLE          2    // "System shutdown"
#define IDS_WINKEYS                3    // "Windows keys"
#define IDS_PAGE1OF3               4    // "Page 1 of 3"
#define IDS_PAGE2OF3               5    // "Page 2 of 3"
#define IDS_PAGE3OF3               6    // "Page 3 of 3"
#define IDS_GENOPTIONS             7    // "General options"
#define IDS_TITLEBAR               8    // "Titlebar"
#define IDS_ACTIVEWIN              9    // "Active window"
#define IDS_INACTIVEWIN           10    // "Inactive window"
#define IDS_PUSHBUTTON            11    // "Pushbutton"
#define IDS_DIALOGOPT             12    // "Dialogs"
#define IDS_PRODINFO              13    // "Product information"
#define IDS_SHUTDOWN              14    // "Shutdown"
#define IDS_REBOOT                15    // "Reboot"
#define IDS_SHUTDOWNPROGS         16    // "Programs"
#define IDS_KILLLIST              17    // "Kill list"
#define IDS_VERSION               18    // "Version: %d.%d build: %d"
#define IDS_WKEYITEM01            19    // "Do not perform any action"
#define IDS_WKEYITEM02            20    // "Hide/show all windows"
#define IDS_WKEYITEM03            21    // "Show window list"
#define IDS_WKEYITEM04            22    // "Show context menu"
#define IDS_WKEYITEM05            23    // "Lock up the desktop"
#define IDS_WKEYITEM06            24    // "Open the Drives folder"
#define IDS_WKEYITEM07            25    // "Open the System Setup folder"
#define IDS_WKEYITEM08            26    // "Open a windowed command prompt"
#define IDS_WKEYITEM09            27    // "Show eComCenter desktop menu"
#define IDS_WKEYITEM10            28    // "Show eComCenter window list"
#define IDS_WKEYITEM11            29    // "Show eComCenter process list"
#define IDS_WKEYITEM12            30    // "Show eComCenter tray list"
#define IDS_WKEYITEM13            31    // "Switch to the next eComCenter tray"
#define IDS_WKEYITEM14            32    // "Show eComCenter information menu"
#define IDS_WKEYITEM15            33    // "Show the eCS menu (top eCenter)"
#define IDS_WKEYITEM16            34    // "Show the eCS menu (bottom eCenter)"
#define IDS_WKEYITEM17            35    // "Show the eCenter tray list"
#define IDS_WKEYITEM18            36    // "Switch to the next eCenter tray"
#define IDS_WKEYITEM19            37    // "Show eCenter tray 1"
#define IDS_WKEYITEM20            38    // "Show eCenter tray 2"
#define IDS_WKEYITEM21            39    // "Show eCenter tray 3"
#define IDS_WKEYITEM22            40    // "Show eCenter tray 4"
#define IDS_WKEYITEM23            41    // "Show eCenter tray 5"
#define IDS_WKEYITEM24            42    // "Show eCenter tray 6"
#define IDS_WKEYITEM25            43    // "Show eCenter tray 7"
#define IDS_WKEYITEM26            44    // "Show eCenter tray 8"
#define IDS_WKEYITEM27            45    // "Show eCenter tray 9"
#define IDS_WKEYITEM28            46    // "Show eCenter tray 10"
#define IDS_WKEYITEM29            47    // "Move eCenter behind/on-top"
#define IDS_ADDTOTHELIST          48    // "Add to the list"
#define IDS__ADD                  49    // "Add"
#define IDS_FONT_                 50    // "Font: . . . . . . . "
#define IDS_TITLEBARFONT          51    // "Titlebar font"
#define IDS_FONTDLGSAMPLE         52    // "eStyler - sample text"
#define IDS_PREVIEWTITLE          53    // "eStyler - preview window"
#define IDS_NORMAL                54    // "~Normal"
#define IDS_DEFAULT               55    // "~Default"
#define IDS_DISABLED              56    // "~Disabled"
#define IDS_GRADIENTCOLOR         57    // "%s gradient color"
#define IDS_CLRSHADELEFT          58    // "~Left"
#define IDS_CLRSHADTOP            59    // "~Top"
#define IDS_CLRSHADOUTER          60    // "~Outer"
#define IDS_CLRSHADRIGHT          61    // "~Right"
#define IDS_CLRSHADBOTTOM         62    // "~Bottom"
#define IDS_CLRSHADINNER          63    // "~Inner"
#define IDS_DEFIMAGE              64    // "Default image"
#define IDS_DIALOGFONT            65    // "Dialog font"

// change this when adding further string IDs
#define IDS_LAST                  IDS_DIALOGFONT

/* --------------------------------------------------------------------------
 ERRMSGSSTRIDS     error messages string IDs
*/

#define ISERR_WORKERSEMAPHORE      (IDS_LAST +  1)
#define ISERR_WORKERTHREAD         (IDS_LAST +  2)
#define ISERR_LOADRUNTIMEMODULE    (IDS_LAST +  3)
#define ISERR_QUERYPROCADDR        (IDS_LAST +  4)
#define ISERR_GETCUROPTIONS        (IDS_LAST +  5)
#define ISERR_ALLOCATION           (IDS_LAST +  6)
#define ISERR_OPENPRF              (IDS_LAST +  7)
#define ISERR_READPRFDATA          (IDS_LAST +  8)
#define ISERR_INITPM               (IDS_LAST +  9)
#define ISERR_REGBOXCLASS          (IDS_LAST + 10)
#define ISERR_REGXCOMBOCLASS       (IDS_LAST + 11)
#define ISERR_REGCLRBTNCLASS       (IDS_LAST + 12)
#define ISERR_GETBMPHANDLES        (IDS_LAST + 13)
#define ISERR_SETNOTEBOOKPAGES     (IDS_LAST + 14)
#define ISERR_REGEXITLIST          (IDS_LAST + 15)
#define ISERR_TOOMANYENTRIES       (IDS_LAST + 16)
#define ISERR_SAVEEXCPLIST         (IDS_LAST + 17)
#define ISERR_READFILE             (IDS_LAST + 18)
#define ISERR_INVALIDFILE          (IDS_LAST + 19)
#define ISERR_BMPTOOLARGE          (IDS_LAST + 20)
#define ISERR_SAVEDATA             (IDS_LAST + 21)
#define ISERR_APPLYBMP             (IDS_LAST + 22)
#define ISERR_GETHPSFORHBMP        (IDS_LAST + 23)
#define ISERR_GETBMPHANDLE         (IDS_LAST + 24)
#define ISERR_COPYFILE             (IDS_LAST + 25)
#define ISERR_SOURCEEQDEST         (IDS_LAST + 26)
#define ISERR_PROGLISTCOUNT        (IDS_LAST + 27)
#define ISERR_INVALIDEXE           (IDS_LAST + 28)
#define ISERR_INSPAGE              (IDS_LAST + 29)
#define ISERR_DELPAGE              (IDS_LAST + 30)
#define ISERR_GETBOTTLIST          (IDS_LAST + 31)
#define ISERR_GBLDISKHANDLE        (IDS_LAST + 32)
#define ISERR_GBLREADSECT1         (IDS_LAST + 33)
#define ISERR_GBLAIRBOOTVER        (IDS_LAST + 34)
#define ISERR_GBLREADSECT2         (IDS_LAST + 35)
#define ISERR_GBLINVALIDDATA       (IDS_LAST + 36)
#define ISERR_GBLREADSECT3         (IDS_LAST + 37)
#define ISERR_GBLOPENLVM           (IDS_LAST + 38)
#define ISERR_GBLLVMDRVLIST        (IDS_LAST + 39)
#define ISERR_GBLLVMPARTLIST       (IDS_LAST + 40)
#define ISERR_GBLLVMVOLINFO        (IDS_LAST + 41)
#define ISERR_INITHELP             (IDS_LAST + 42)


/* --------------------------------------------------------------------------
 HELP_IDS          help table/item/subtable IDs
*/
#define HLP_MAIN                   100
#define HLPS_MAIN                    1
#define HLPS_UIGEN                   2
#define HLPS_TBARGEN                 3
#define HLPS_TBARHILITE              4
#define HLPS_BTN                     5
#define HLPS_DLG                     6
#define HLPS_PRODINFO                7
#define HLPS_SDWNGEN                 8
#define HLPS_SDTIMING                9
#define HLPS_SDWNREBOOT             10
#define HLPS_SDWNPROGS              11
#define HLPS_SDWNKILL               12
#define HLPS_SDWINKEY               13

/* --------------------------------------------------------------------------
*/

#endif // ifndef _STLR_RESOURCE_IDS_H_
