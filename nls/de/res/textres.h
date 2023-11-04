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
#define SZ__ADD               "~HinzufÅgen"
#define SZ__REM               "Entfe~rnen"
#define SZ__EDIT              "B~earbeiten"
#define SZ__OK                "~OK"
#define SZ__CANCEL            "Abbre~chen"
#define SZ__OPEN              "ô~ffnen..."
#define SZ__SAVE              "~Speichern..."
#define SZ__SAVEAS            "Speichern ~als..."
#define SZ__EXIT              "~Beenden"
#define SZ__FIND              "F~inden..."
#define SZ_ADDTOTHELIST       "Zur Liste hinzufÅgen"
#define SZ_FONT_              "Schrif~t:   %s   . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."


/* --------------------------------------------------------------------------
 MAINWINDOW        text displayed by the settings notebook parent
*/
// help title
#define SZ_APPNAME               "Styler"
#define SZ_APPLY                 "~Anwenden"
#define SZ_UNDO                  "~RÅckgÑngig"
#define SZ__DEFAULT              "~Standard"
#define SZ_PREVIEW               "~Vorschau"
#define SZ_HELP                  "Hilfe"
// settings notebook titles
#define SZ_UITITLE               "Verbesserungen der BenutzeroberflÑche"
#define SZ_SHUTDOWNTITLE         "Systemabschlu·"
#define SZ_WINKEYTITLE           "Windowstasten"

/* --------------------------------------------------------------------------
 UIGEN             general User interface options
*/
#define SZ_UIENABLE              "~Aktivieren der Verbesserungen"
#define SZ_EXCEPTIONS            "~Nicht anwenden auf:"

/* --------------------------------------------------------------------------
 TITLEBARGEN       general titlebar options
*/
#define SZ_TBENABLE              "Titelleistenverbesserung ~aktivieren"
#define SZ_TBOVERRIDEPP          "PrÑsentationsparameter Å~bergehen"
#define SZ__FONT                 "~Schrift:"
#define SZ_TEXTATLIGN            "~Textanordnung:"
#define SZ_LEFTCENTER            "Links\tZentriert\t"

/* --------------------------------------------------------------------------
 TITLEBAR          titlebar background and text options
*/
#define SZ_TYPEOFBKGND           "Ar~t des Hintergrunds:"
#define SZ_COMBOBKGND            "Vollfarbe\tVerlauf\tBitmap\t"
// solid color mode controls
#define SZ_BKGNDCOLOR            "~Hintergrundfarbe"
// gradient mode controls
#define SZ_SHADEDIRECTION        "~Verlaufsrichtung:"
#define SZ_COMBOSHADE            "Horizontal\tVertikal\t" \
                                 "Horizontal - doppelt\tVertikal - doppelt\t"
#define SZ_SWAPCOLORS            "~Farben tauschen =>"
#define SZ_OUTERCOLOR            "~Au·enfarbe"
#define SZ_INNERCOLOR            "~Innenfarbe"
#define SZ_LEFTCOLOR             "~Linke Farbe"
#define SZ_RIGHTCOLOR            "~Rechte Farbe"
#define SZ_TOPCOLOR              "~Obere Farbe"
#define SZ_BOTTOMCOLOR           "~Untere Farbe"
// image mode controls
#define SZ_BKGNDIMAGE            "Hintergrund-~Bild:"

// !!! NOTE !!! when translating the following string do not remove the
// 3 leading blank spaces !!!!!!!!
#define SZ_DEFIMAGE              "   Standardbild"
#define SZ_SCALEIMAGE            "Bild ~Skalieren"
//#define SZ_IMAGEOPTIONS          "~Image options..."
#define SZ_ADDIMAGE              "~Neues Bild hinzufÅgen..."
// border controls
#define SZ_SHOWBORDER            "Titelleistenrand ~zeigen"
#define SZ_LEFTTOPCLR            "Farbe ~Links/oben"
#define SZ_RIGHTBTMCLR           "Farbe ~Rechts/unten"
// text options controls
#define SZ_TEXTSHADOW            "Te~xtschatten"
#define SZ_TEXTCOLOR             "Text~farbe"
#define SZ_TEXTSHADOWCLR         "T~extschattenfarbe"

/* --------------------------------------------------------------------------
 PUSHBUTTON        pushbutton options
*/
#define SZ_BTNON                 "~SchaltflÑchenerweiterungen"
#define SZ_BTNTHICK              "D~icker Rand"
#define SZ_BTNFLAT               "~Flacher Rand"
#define SZ_BTNOVRCLR             "~Umgehen von Standardfarben"
#define SZ_BTN3DDEF              "~3D-Stil fÅr StandardschaltflÑchen"
#define SZ_BTN3DDIS              "3~D-Stil fÅr inaktive SchaltflÑchen"
#define SZ_BTNSOLID              "~Vollfarbenhintergrund"
//#define SZ_BKGNDIMAGE            "~Hintergrundbild:"
//#define SZ_BKGNDCOLOR            "Hi~ntergrundfarbe"

/* --------------------------------------------------------------------------
 DIALOG            dialog font options
*/
#define SZ_DLGOVERPP             "N~icht-Standard-Schriften umgehen"
/* see string resources below for SZ_DLGFONTON */

/* --------------------------------------------------------------------------
 PRODINFO          product information dialog
*/
#define SZ_PRODUCT               "Styler fÅr OS/2"
#define SZ_PIVERSION             "Version %d.%d - Build Nr. %d"
#define SZ_PICPYRIGHT            "(c) 1998-2004 Alessandro Felice Cantatore"
#define SZ_PICPYRIGHT2           "Teile (c) 2008-2023 Alexander Taylor"
#define SZ_PIURL                 "https://github.com/altsan/os2-estyler"
#define SZ_PILICENSE             "Lizenziert unter GNU General Public License V3"

/* --------------------------------------------------------------------------
 SHUTDOWNGEN       shutdown - general options dialog
*/
#define SZ_SDWNON                "~Erweiterten Systemabschlu· aktivieren"
#define SZ_SDWNINCLUDE           "Einschlie·en dieser Abschaltoptionen: "
#define SZ_SDWNLKSUSP            "~Sperren und Ruhezustand"
#define SZ_SDWNSUSP              "~Ruhezustand"
#define SZ_SDWNOFF               "~Abschalten"
#define SZ_SDWNORD               "S~ystemabschlu·"
#define SZ_SDWNREBOOT            "Neusta~rt"
#define SZ_SDWNANIMATE           "Systemabschlu·-Dialog a~nimieren"
#define SZ_SDWNSTEPS             "Animationsschri~tte:"
#define SZ_SDWNADVOPTS           "Erweiterte ~Optionen..."
#define SZ_SDWNMOUSE             "~Capture mouse pointer"           // TRANSLATE

/* --------------------------------------------------------------------------
 DLG_SDTIMING      shutdown - general options dialog
*/
#define SZ_SDTIMTITLE            "Systemabschlu· - Zeitsteuerungsoptionen"
#define SZ_SDTIMWARNING          "éndern Sie die Werte nicht, ohne voll- "\
                                 "stÑndig zu verstehen, was sie bewirken."
#define SZ_SDTIMDELAY            "Verzîgerung (Zehntelsekunden) :"
#define SZ_SDTIMLKUPSUSP         "~Sperren und Ruhezustand :"
#define SZ_SDTIMSUSPEND          "~Ruhezustand :"
#define SZ_SDTIMWARPCENTER       "~WarpCenter-Einstellungen speichern :"
#define SZ_SDTIMUNROLLWIN        "Fenster ~entrollen :"
#define SZ_SDTIMSYSMSGBOX        "Nachrichten ~beim Systemabschlu· :"
#define SZ_SDTIMWINSD            "WinS~hutdownSystem :"
#define SZ_SDTIMDOSSD            "DosShu~tdown :"
#define SZ_SDTIMOFF              "S~ystemabschlu· :"
#define SZ_SDTIMKILL             "Dos~KillProcess :"
#define SZ_SDTIMOK               SZ__OK
#define SZ_SDTIMCANCEL           SZ__CANCEL
#define SZ_SDTIMDEFAULT          SZ__DEFAULT
#define SZ_SDTIMHELP             SZ_HELP

/* --------------------------------------------------------------------------
 REBOOTLIST        shutdown - reboot list options dialog
*/
#define SZ_SDWNVOLLIST           "~Diese Volumes fÅr den Neustart erlauben:"
#define SZ_SDWNVOLNAME           "D~atentrÑgername:"
#define SZ_SDWNVOLDDESCR         "DatentrÑger~beschreibung:"
#define SZ_SDWNREBOOTADD         SZ__ADD
#define SZ_SDWNREBOOTREM         SZ__REM
#define SZ_SDWNREBOOTEDIT        SZ__EDIT
#define SZ_SDWNREBOOTFIND        "Suchen"

/* --------------------------------------------------------------------------
 WAITGETBOOTLIST   wait-while-searching-bootable-volumes dialog
*/
#define SZ_SDGETBOOTLIST        "Suche nach startbaren Volumes.\x0a\x0a" \
                                "Bitte warten ..."

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRAMS  shutdown - shutdown programs list dialog
*/
#define SZ_SDWNRUNPROG           "~Run the following programs on shutdown:"     // TRANSLATE
#define SZ_SDWNPROG              "~Programm:"
#define SZ_SDWNPARMS             "Para~meter:"
#define SZ_SDWNWIN               "~Fenster:"
#define SZ_SDWNCOMBOWINS         "Normal\tMinimiert\tMaximiert\t"
#define SZ_SDWNPROGADD           SZ__ADD
#define SZ_SDWNPROGREM           SZ__REM
#define SZ_SDWNPROGEDIT          SZ__EDIT

/* --------------------------------------------------------------------------
 SHUTDOWNKILLLIST  shutdown - list of programs to be killed on shutdown dialog
*/
#define SZ_SDWNKILLPROG          "Diese Programme bei Systemabschlu· ~terminieren:"

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
#define SZ_SDWNRUNPRGRS          "~Abschlu·programme ausfÅhren"

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRESS  shutdown progress dialog
*/
#define SZ_SSDWNSDWNWAIT    "Systemabschlu· wird ausgefÅhrt. Bitte warten."
#define SZ_SSDWNCOMPLETE    "Systemabschlu· vollstÑndig."
#define SZ_SSDWNREBOOTING   "Das System startet in KÅrze neu."
#define SZ_SSDWNPWRINGOFF   "Das System schaltet nach dem Systemabschlu· ab."
#define SZ_SSDWNSWTCHOFF    "Es ist sicher jetzt abzuschalten."

/* --------------------------------------------------------------------------
 WINKEY            windows keys options dialog
*/
#define SZ_WINKEYON         "~Einschalten der Windows-Tasten"
#define SZ_WKEYLEFT         "~Linke Windows-Taste:"
#define SZ_WKEYRIGHT        "~Rechte Windows-Taste:"
#define SZ_WKEYMENU         "Windows-~Kontext-Taste:"

/* --------------------------------------------------------------------------
 PREVIEW           preview window
*/
#define SZ_PREVIEWTITLE     "Styler - Vorschaufenster"
#define SZ_STATICSAMPLE     "Beispiel fÅr statischen Text"
#define SZ_BTNSAMPLES       "Beispiel fÅr SchaltflÑchen"
#define SZ__NORMAL          "~Normal"
#define SZ__DISABLED        "~Inaktiv"


/* --------------------------------------------------------------------------
 COLORWHEEL        Color selection dialog
*/
#define SZ_CLRWTITLE        "Farbauswahl"
#define SZ__RED             "~Rot"
#define SZ__GREEN           "~GrÅn"
#define SZ__BLUE            "~Blau"


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
#define SZ_PAGE1OF3         "Seite 1 von 3"
#define SZ_PAGE2OF3         "Seite 2 von 3"
#define SZ_PAGE3OF3         "Seite 3 von 3"
#define SZ_GENOPTIONS       "Allgemeine Optionen"
#define SZ_TITLEBAR         "Titelleiste"
#define SZ_ACTIVEWIN        "Aktives Fenster"
#define SZ_INACTIVEWIN      "Inaktives Fenster"
#define SZ_PUSHBUTTON       "SchaltflÑche"
#define SZ_DIALOGOPTS       "Dialogfenster"
#define SZ_PRODINFO         "Produktinformationen"
#define SZ_SHUTDOWN         "Systemabschlu·"
#define SZ_REBOOT           "Neustart"
#define SZ_SHUTDOWNPROGS    "Programme"
#define SZ_KILLLIST         "Terminierungsliste"
#define SZ_VERSION          "Version: %d.%d.%d"
#define SZ_WKEYITEM01       "Nichts unternehmen"
#define SZ_WKEYITEM02       "Alle Fenster verstecken/zeigen"
#define SZ_WKEYITEM03       "Fensterliste anzeigen"
#define SZ_WKEYITEM04       "KontextmenÅ anzeigen"
#define SZ_WKEYITEM05       "Desktop sperren"
#define SZ_WKEYITEM06       "Laufwerke anzeigen"
#define SZ_WKEYITEM07       "Systemeinstellungen îffnen"
#define SZ_WKEYITEM08       "Eingabezeile (Fenster) îffnen"
#define SZ_WKEYITEM09       "WarpCenter-MenÅ îffnen"
#define SZ_WKEYITEM10       "WarpCenter-Fensterliste îffnen"
#define SZ_WKEYITEM11       "WarpCenter-Proze·liste îffnen"
#define SZ_WKEYITEM12       "WarpCenter-Fach-Liste îffnen"
#define SZ_WKEYITEM13       "Zum nÑchsten WarpCenter-Fach schalten"
#define SZ_WKEYITEM14       "WarpCenter-InformationsmenÅ îffnen"
#define SZ_WKEYITEM15       "XButton-MenÅ îffnen (wenn Leiste oben)"
#define SZ_WKEYITEM16       "XButton-MenÅ îffnen (wenn Leiste unten)"
#define SZ_WKEYITEM17       "XCenter-Fachliste îffnen"
#define SZ_WKEYITEM18       "Zum nÑchsten XCenter-Fach wechseln"
#define SZ_WKEYITEM19       "XCenter-Fach 1 zeigen"
#define SZ_WKEYITEM20       "XCenter-Fach 2 zeigen"
#define SZ_WKEYITEM21       "XCenter-Fach 3 zeigen"
#define SZ_WKEYITEM22       "XCenter-Fach 4 zeigen"
#define SZ_WKEYITEM23       "XCenter-Fach 5 zeigen"
#define SZ_WKEYITEM24       "XCenter-Fach 6 zeigen"
#define SZ_WKEYITEM25       "XCenter-Fach 7 zeigen"
#define SZ_WKEYITEM26       "XCenter-Fach 8 zeigen"
#define SZ_WKEYITEM27       "XCenter-Fach 9 zeigen"
#define SZ_WKEYITEM28       "XCenter-Fach 10 zeigen"
#define SZ_WKEYITEM29       "XCenter hinter/Åber den anderen Fenstern anzeigen"
#define SZ_TITLEBARFONT     "Schriftart Titelleiste"
#define SZ_FONTDLGSAMPLE    "Styler - Beispieltext"
#define SZ_GRADIENTCOLOR    "%s Verlaufsfarbe"
#define SZ_CLRSHADELEFT     "~Links"
#define SZ_CLRSHADTOP       "~Oben"
#define SZ_CLRSHADOUTER     "~Au·en"
#define SZ_CLRSHADRIGHT     "~Rechts"
#define SZ_CLRSHADBOTTOM    "~Unten"
#define SZ_CLRSHADINNER     "~Innen"
#define SZ_DIALOGFONT       "Dialogschriftart"
#define SZ_DLGFONTON        "~Use %s for dialog windows"        // TRANSLATE

/* --------------------------------------------------------------------------
 ERRORMSGS         error messages string resources
*/

#define SZERR_WORKERSEMAPHORE \
   "Die Thread-Synchronisationssemaphore konnte nicht erstellt werden."

#define SZERR_WORKERTHREAD \
   "Das Erstellen des Worker-Threads ist fehlgeschlagen."

#define SZERR_LOADRUNTIMEMODULE \
   "Laden des Laufzeitmoduls fehlgeschlagen ("SZ_MODULENAME".DLL)."

#define SZERR_QUERYPROCADDR \
   "Der Import der Prozeduren des Laufzeitmoduls ist fehlgeschlagen."

#define SZERR_GETCUROPTIONS \
   "Die derzeit aktiven Styler-Optionen konnten nicht abgerufen werden."

#define SZERR_ALLOCATION \
   "Die Speicherzuweisung ist fehlgeschlagen."

#define SZERR_OPENPRF \
   "STYLER.INI konnte nicht geîffnet werden."

#define SZERR_READPRFDATA \
   "Das Lesen von INI-Dateidaten ist fehlgeschlagen."

#define SZERR_INITPM \
   "Der PrÑsentationsmanager konnte nicht initialisiert werden."

#define SZERR_REGBOXCLASS \
   "Das Box-/Balkenklassen-Steuerelement konnte nicht registriert werden."

#define SZERR_REGXCOMBOCLASS \
   "Die erweiterte Dropdown-Box-Steuerung konnte nicht registriert werden."

#define SZERR_REGCLRBTNCLASS \
   "Farb-SchaltflÑchen-Klassensteuerelement konnte nicht registriert werden."

#define SZERR_GETBMPHANDLES \
   "Bitmap-Handles fÅr das Vorschaufenster konnten nicht abgerufen werden."

#define SZERR_SETNOTEBOOKPAGES \
   "EinfÅgen der Dialogseiten in den Einstellungsnotizblock fehlgeschlagen."

#define SZERR_REGEXITLIST \
   "Exit-Routine, ausgefÅhrt bei Anwendungsende, nicht registrierbar!"

#define SZERR_TOOMANYENTRIES \
   "Zuviele EintrÑge in der Liste; Der Listeninhalt konnte nicht in die "\
   "INI-Datei abgelegt werden. Bitte Liste reduzieren und erneut versuchen."

#define SZERR_SAVEEXCPLIST \
   "Die Exklusionsliste zu den Verbesserungen der Benutzerschnittstelle "\
   "konnte nicht gespeichert werden."

#define SZERR_READFILE \
   "Datei: %s konnte nicht gelesen werden."

#define SZERR_INVALIDFILE \
   "Das Format der Datei: %s ist fehlerhaft."

#define SZERR_BMPTOOLARGE \
   "Das Bitmap: %s Åberschreitet das 64 KBs-Limit."

#define SZERR_SAVEDATA \
   "Fehler beim Speichern in die STYLER.INI."

#define SZERR_APPLYBMP \
   "Das ausgewÑhlte Bitmap konnte nicht gesetzt werden."

#define SZERR_GETHPSFORHBMP \
   "Bitmap-Datenfehler: Zugriff auf den PrÑsentationsbereich fehlgeschlagen."

#define SZERR_GETBMPHANDLE \
    "Bitmap-Datenfehler: Zugriff auf das Bitmap-Handle fehlgeschlagen."

#define SZERR_COPYFILE \
   "Fehler beim Kopieren der Datei von %s nach %s."

#define SZERR_SOURCEEQDEST \
   "Quell- und Zielpfad sind gleich."

#define SZERR_PROGLISTCOUNT \
   "Die Programmliste enthÑlt zuviele Elemente. Die letzten paar EintrÑge "\
   "werden nicht gespeichert werden."

#define SZERR_INVALIDEXE \
   "Die Datei %s hat keine verwendbare ausfÅhrbare Extension ("".CMD"", "\
   """.COM"", "".EXE"")."

#define SZERR_INSPAGE \
   "Fehler beim EinfÅgen einer Seite in den Einstellungsnotizblock."

#define SZERR_DELPAGE \
   "Fehler beim Lîschen einer Seite im Einstellungsnotizblock."

#define SZERR_GETBOTTLIST \
    "Arbeitsproze·: Fehlschlag beim Ermitteln der startfÑhigen Volumes."

#define SZERR_HELPSYS \
   "\x0a" "Schreiben Sie HELPSYS%04u in der Eingabezeile fÅr Informationen."

#define SZERR_GBLDISKHANDLE \
   "Disk-Handle konnte nicht ermittelt werden."SZERR_HELPSYS

#define SZERR_GBLREADSECT1  \
   "Fehler beim Lesen des ersten Festplattensektors."SZERR_HELPSYS

#define SZERR_GBLAIRBOOTVER \
   "Falsche AiRBOOT-Version. Bitte neuere Version installieren."

#define SZERR_GBLREADSECT2  \
   "Fehler beim Lesen des Disksektors mit der AiRBOOT-Konfiguration! " \
   SZERR_HELPSYS

#define SZERR_GBLINVALIDDATA \
   "Falsche Signatur in der AiRBOOT-Konfiguration!"

#define SZERR_GBLREADSECT3  \
   "Fehler beim Lesen des Disk-Sektors, der die Liste der startbaren" \
   " AiRBOOT-Volumes enthÑlt."SZERR_HELPSYS

#define SZERR_LVMCODE \
   " (LVM-Fehlercode: %d)."

#define SZERR_GBLOPENLVM    \
   "Fehler beim Verbinden zu LVM"SZERR_LVMCODE

#define SZERR_GBLLVMDRVLIST \
   "Fehler beim Ermitteln der Disk-Liste"SZERR_LVMCODE

#define SZERR_GBLLVMPARTLIST \
   "Fehler beim Ermitteln der Partitionsliste"SZERR_LVMCODE

#define SZERR_GBLLVMVOLINFO \
   "Fehler beim Ermitteln der LVM-Volume-Daten"SZERR_LVMCODE

#define SZERR_INITHELP \
   "Fehler beim Laden der Hilfe-Datei STYLER.HLP!"

#endif // #ifndef _TEXT_RES_H_
