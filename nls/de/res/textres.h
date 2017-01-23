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
#define SZ__EDIT              "~Bearbeiten"
#define SZ__OK                "OK"
#define SZ__CANCEL            "Abbruch"
#define SZ__OPEN              "~ôffnen..."
#define SZ__SAVE              "S~ichern..."
#define SZ__SAVEAS            "Sichern ~unter..."
#define SZ__EXIT              "Be~enden"
#define SZ__FIND              "Su~chen..."
#define SZ_ADDTOTHELIST       "Zur Liste hinzufÅgen"
#define SZ_FONT_              "Schri~ftart:   %s   . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."


/* --------------------------------------------------------------------------
 MAINWINDOW        text displayed by the settings notebook parent
*/
// help title
#define SZ_APPNAME               "eStyler fÅr eComStation"
#define SZ_APPLY                 "Anwenden"
#define SZ_UNDO                  "~Widerrufen"
#define SZ__DEFAULT              "~Standard"
#define SZ_PREVIEW               "~Vorschau"
#define SZ_HELP                  "Hilfe"
// settings notebook titles
#define SZ_UITITLE               "Erweiterungen fÅr BenutzeroberflÑche"
#define SZ_SHUTDOWNTITLE         "Systemabschlu·"
#define SZ_WINKEYTITLE           "Windows-Tasten"

/* --------------------------------------------------------------------------
 UIGEN             general User interface options
*/
#define SZ_UIENABLE              "~Erweiterungen fÅr BenutzeroberflÑche aktivieren"
#define SZ_EXCEPTIONS            "Erweiterungen ~nicht aktivieren bei:"

/* --------------------------------------------------------------------------
 TITLEBARGEN       general titlebar options
*/
#define SZ_TBENABLE              "~Erweiterungen fÅr Titelleisten aktivieren"
#define SZ_TBOVERRIDEPP          "~Darstellungseinstellungen Åbersteuern"
#define SZ__FONT                 "Schri~ftart:"
#define SZ_TEXTATLIGN            "~Textausrichtung:"
#define SZ_LEFTCENTER            "Links\tZentriert\t"
#define SZ_BUTTONTHEME           "3d\t3dcrack\tblack\tblackhi\tblue\tbtn0\tbtn1\tbtn2\tbtn3\tcrack\tdon\tlinux\tlite\tlitehi\tmath\tsfera\twarp4\twin9x"

/* --------------------------------------------------------------------------
 TITLEBAR          titlebar background and text options
*/
#define SZ_TYPEOFBKGND           "~Hintergrundart:"
#define SZ_COMBOBKGND            "Nur Farbe\tFarbverlauf\tBitmap\t"
// solid color mode controls
#define SZ_BKGNDCOLOR            "Hintergrund~farbe"
// gradient mode controls
#define SZ_SHADEDIRECTION        "Richtun~g des Farbverlaufs:"
#define SZ_COMBOSHADE            "Horizontal\tVertikal\t" \
                                 "Horizontal - doppelt\tVertikal - doppelt\t"
#define SZ_SWAPCOLORS            "Farben tauschen =>"
#define SZ_OUTERCOLOR            "~Au·enfarbe"
#define SZ_INNERCOLOR            "~Innenfarbe"
#define SZ_LEFTCOLOR             "~Linke Farbe"
#define SZ_RIGHTCOLOR            "~Rechte Farbe"
#define SZ_TOPCOLOR              "~Obere Farbe"
#define SZ_BOTTOMCOLOR           "~Untere Farbe"
// image mode controls
#define SZ_BKGNDIMAGE            "Hintergrund~bild:"

// !!! NOTE !!! when translating the following string do not remove the
// 3 leading blank spaces !!!!!!!!
#define SZ_DEFIMAGE              "   Standardbild"
#define SZ_SCALEIMAGE            "Bil~d skalieren"
//#define SZ_IMAGEOPTIONS          "~Bildoptionen..."
#define SZ_ADDIMAGE              "Neues Bild hinzuf~Ågen..."
// border controls
#define SZ_SHOWBORDER            "Titelleistenumrandung an~zeigen"
#define SZ_LEFTTOPCLR            "Li~nke/obere Farbe"
#define SZ_RIGHTBTMCLR           "R~echte/untere Farbe"
// text options controls
#define SZ_TEXTSHADOW            "~Textschatten"
#define SZ_TEXTCOLOR             "Farbe Te~xt"
#define SZ_TEXTSHADOWCLR         "Farbe Texts~chatten"

/* --------------------------------------------------------------------------
 PUSHBUTTON        pushbutton options
*/
#define SZ_BTNON                 "Erweiterungen fÅr Druck~knîpfe aktivieren"
#define SZ_BTNTHICK              "Bre~iter Rand"
#define SZ_BTNFLAT               "~Flacher Rand"
#define SZ_BTNOVRCLR             "Farben ~Åbersteuern"
#define SZ_BTN3DDEF              "~3D-Stil fÅr Standarddruckknopf"
#define SZ_BTN3DDIS              "3~D-Stil fÅr deaktivierte Druckknîpfe"
#define SZ_BTNSOLID              "Farbe als ~Hintergrund"
//#define SZ_BKGNDIMAGE            "Hintergrund~bild:"
//#define SZ_BKGNDCOLOR            "Hintergrundf~arbe:"

/* --------------------------------------------------------------------------
 DIALOG            dialog font options
*/
#define SZ_DLGFONTON             "Standardschri~ftart fÅr Dialoge festlegen"
#define SZ_DLGOVERPP             "Abweichende Schriftarten ~Åbersteuern"

/* --------------------------------------------------------------------------
 PRODINFO          product information dialog
*/
#define SZ_PRODUCT               "eStyler fÅr eComStation"
#define SZ_PIVERSION             "Version %d.%d - Build-Nr. %d"
#define SZ_PICPYRIGHT            "∏ 1998-2004 Alessandro Felice Cantatore"
#define SZ_PICPYRIGHT2           "Portions (c) 2008-2016 Alexander Taylor"
#define SZ_PIURL                 "https://github.com/altsan/os2-estyler"
#define SZ_PILICENSE             "Licensed under the GNU General Public License version 3"

/* --------------------------------------------------------------------------
 SHUTDOWNGEN       shutdown - general options dialog
*/
#define SZ_SDWNON                "~Erweiterten Systemabschlu· aktivieren"
#define SZ_SDWNINCLUDE           "Mit folgenden Optionen: "
#define SZ_SDWNLKSUSP            "S~perren und Wartemodus"
#define SZ_SDWNSUSP              "Warte~modus"
#define SZ_SDWNOFF               "A~usschalten"
#define SZ_SDWNORD               "S~ystemabschlu·"
#define SZ_SDWNREBOOT            "System~neustart"
#define SZ_SDWNANIMATE           "Systemabschlu·dialog ~animieren"
#define SZ_SDWNSTEPS             "Animationsschri~tte:"
#define SZ_SDWNADVOPTS           "Erweiterte ~Optionen..."

/* --------------------------------------------------------------------------
 DLG_SDTIMING      shutdown - general options dialog
*/
#define SZ_SDTIMTITLE            "Systemabschlu· - Zeitoptionen"
#define SZ_SDTIMWARNING          "éndern Sie folgende Werte nur, wenn Sie sich "\
                                 "Åber die Folgen im klaren sind."
#define SZ_SDTIMDELAY            "Verzîgerung (Zehntelsekunden):"
#define SZ_SDTIMLKUPSUSP         "S~perren und Wartemodus:"
#define SZ_SDTIMSUSPEND          "Warte~modus:"


#define SZ_SDTIMWARPCENTER       "eCom~Center-Einstellungen sichern:"
#define SZ_SDTIMUNROLLWIN        "Fenster a~usklappen:"
#define SZ_SDTIMSYSMSGBOX        "S~ystemabschlu·meldungen:"
#define SZ_SDTIMWINSD            "WinS~hutdownSystem:"
#define SZ_SDTIMDOSSD            "DosShu~tdown:"
#define SZ_SDTIMOFF              "Ausscha~lten:"
#define SZ_SDTIMKILL             "Dos~KillProcess:"
#define SZ_SDTIMOK               SZ__OK
#define SZ_SDTIMCANCEL           SZ__CANCEL
#define SZ_SDTIMDEFAULT          SZ__DEFAULT
#define SZ_SDTIMHELP             SZ_HELP

/* --------------------------------------------------------------------------
 REBOOTLIST        shutdown - reboot list options dialog
*/
#define SZ_SDWNVOLLIST           "~Neustart von folgenden Volumen erlauben:"
#define SZ_SDWNVOLNAME           "Volumen~name:"
#define SZ_SDWNVOLDDESCR         "Volumen~bezeichnung:"
#define SZ_SDWNREBOOTADD         SZ__ADD
#define SZ_SDWNREBOOTREM         SZ__REM
#define SZ_SDWNREBOOTEDIT        SZ__EDIT
#define SZ_SDWNREBOOTFIND        "Erkennung"

/* --------------------------------------------------------------------------
 WAITGETBOOTLIST   wait-while-searching-bootable-volumes dialog
*/
#define SZ_SDGETBOOTLIST        "Erkennung startfÑhiger Volumen.\x0a\x0a" \
                                "Bitte warten..."

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRAMS  shutdown - shutdown programs list dialog
*/
#define SZ_SDWNRUNPROG           "Bei Systemabschlu· ~auszufÅhrende Programme:"
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
#define SZ_SDWNKILLPROG          "Bei Systemabschlu· zu ~beendende Programme:"

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
#define SZ_SDWNRUNPRGRS          "Systemabschlu·programme ~ausfÅhren"

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRESS  shutdown progress dialog
*/
#define SZ_SSDWNSDWNWAIT    "Der Systemabschlu· wird durchgefÅhrt. Bitte warten..."
#define SZ_SSDWNCOMPLETE    "Systemabschlu· beendet."
#define SZ_SSDWNREBOOTING   "Das System wird nach DurchfÅhren des Systemabschlusses neu gestartet."
#define SZ_SSDWNPWRINGOFF   "Das System wird nach DurchfÅhren des Systemabschlusses ausgeschaltet. "
#define SZ_SSDWNSWTCHOFF    "Der Rechner kann nun ausgeschaltet werden."

/* --------------------------------------------------------------------------
 WINKEY            windows keys options dialog
*/
#define SZ_WINKEYON         "Windows-Tasten ~aktivieren"
#define SZ_WKEYLEFT         "~Linke Windows-Taste:"
#define SZ_WKEYRIGHT        "~Rechte Windows-Taste:"
#define SZ_WKEYMENU         "Windows-~MenÅ-Taste:"

/* --------------------------------------------------------------------------
 PREVIEW           preview window
*/
#define SZ_PREVIEWTITLE     "eStyler - Vorschaufenster"
#define SZ_STATICSAMPLE     "Beispiel fÅr statischen Text"
#define SZ_BTNSAMPLES       "Beispiele fÅr Druckknîpfe: "
#define SZ__NORMAL          "~Normal"
#define SZ__DISABLED        "~Deaktiviert"


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
#define SZ_PUSHBUTTON       "Druckknopf"
#define SZ_DIALOGOPTS       "Dialoge"
#define SZ_PRODINFO         "Produktinformation"
#define SZ_SHUTDOWN         "Systemabschlu·"
#define SZ_REBOOT           "Systemneustart"
#define SZ_SHUTDOWNPROGS    "Programme"
#define SZ_KILLLIST         "Liste fÅr Beenden"
#define SZ_VERSION          "Version: %d.%d.%d"
#define SZ_WKEYITEM01       "Keine Aktion durchfÅhren"
#define SZ_WKEYITEM02       "Alle Fenster anzeigen/verbergen"
#define SZ_WKEYITEM03       "Fensterliste anzeigen"
#define SZ_WKEYITEM04       "KontextmenÅ anzeigen"
#define SZ_WKEYITEM05       "ArbeitsoberflÑche sperren"
#define SZ_WKEYITEM06       "Ordner Laufwerke îffnen"
#define SZ_WKEYITEM07       "Ordner Systemkonfiguration îffnen"
#define SZ_WKEYITEM08       "Befehlszeilenfenster îffnen"
#define SZ_WKEYITEM09       "eComCenter-ArbeitsoberflÑchenmenÅ anzeigen"
#define SZ_WKEYITEM10       "eComCenter-Fensterliste anzeigen"
#define SZ_WKEYITEM11       "eComCenter-Proze·liste anzeigen"
#define SZ_WKEYITEM12       "eComCenter-FÑcherliste anzeigen"
#define SZ_WKEYITEM13       "Zum nÑchsten eComCenter-Fach umschalten"
#define SZ_WKEYITEM14       "eComCenter-InformationsmenÅ anzeigen"
#define SZ_WKEYITEM15       "eCS-MenÅ anzeigen (oberes eCenter)"
#define SZ_WKEYITEM16       "eCS-MenÅ anzeigen (unteres eCenter)"
#define SZ_WKEYITEM17       "eCenter-FÑcherliste anzeigen"
#define SZ_WKEYITEM18       "Zum nÑchsten eCenter-Fach umschalten"
#define SZ_WKEYITEM19       "eCenter-Fach 1 anzeigen"
#define SZ_WKEYITEM20       "eCenter-Fach 2 anzeigen"
#define SZ_WKEYITEM21       "eCenter-Fach 3 anzeigen"
#define SZ_WKEYITEM22       "eCenter-Fach 4 anzeigen"
#define SZ_WKEYITEM23       "eCenter-Fach 5 anzeigen"
#define SZ_WKEYITEM24       "eCenter-Fach 6 anzeigen"
#define SZ_WKEYITEM25       "eCenter-Fach 7 anzeigen"
#define SZ_WKEYITEM26       "eCenter-Fach 8 anzeigen"
#define SZ_WKEYITEM27       "eCenter-Fach 9 anzeigen"
#define SZ_WKEYITEM28       "eCenter-Fach 10 anzeigen"
#define SZ_WKEYITEM29       "eCenter hinter/vor andere Fenster bewegen"
#define SZ_TITLEBARFONT     "Schriftart Titelleiste"
#define SZ_FONTDLGSAMPLE    "eStyler - Beispieltext"
#define SZ_GRADIENTCOLOR    "%s Verlaufsfarbe"
#define SZ_CLRSHADELEFT     "~Linke"
#define SZ_CLRSHADTOP       "~Obere"
#define SZ_CLRSHADOUTER     "~éu·ere"
#define SZ_CLRSHADRIGHT     "~Rechte"
#define SZ_CLRSHADBOTTOM    "~Untere"
#define SZ_CLRSHADINNER     "~Innere"
#define SZ_DIALOGFONT       "Schriftart Dialoge"

/* --------------------------------------------------------------------------
 ERRORMSGS         error messages string resources
*/

#define SZERR_WORKERSEMAPHORE \
   "Erstellen der Thread-Synchronisations-Semaphore fehlgeschlagen."

#define SZERR_WORKERTHREAD \
   "Erstellen des Worker-Thread fehlgeschlagen."

#define SZERR_LOADRUNTIMEMODULE \
   "Laden des Laufzeitmoduls fehlgeschlagen ("SZ_MODULENAME".DLL)."

#define SZERR_QUERYPROCADDR \
   "Importieren der Prozeduren des Laufzeitmodus fehlgeschlagen."

#define SZERR_GETCUROPTIONS \
   "Abfragen der aktuellen eStyler-Optionen fehlgeschlagen."

#define SZERR_ALLOCATION \
   "Zuordnen von Speicher fehlgeschlagen."

#define SZERR_OPENPRF \
   "ôffnen der Datei ESTYLER.INI fehlgeschlagen."

#define SZERR_READPRFDATA \
   "Lesen der Daten der INI-Datei fehlgeschlagen."

#define SZERR_INITPM \
   "Initialisieren des Presentation Manager fehlgeschlagen."

#define SZERR_REGBOXCLASS \
   "Registrieren des Bedienungselements fÅr Kasten/Balken fehlgeschlagen."

#define SZERR_REGXCOMBOCLASS \
   "Registrieren des Bedienungselements fÅr das erweiterte Listenfenster fehlgeschlagen."

#define SZERR_REGCLRBTNCLASS \
   "Registrieren des Bedienungselements fÅr farbige Druckknîpfe fehlgeschlagen."

#define SZERR_GETBMPHANDLES \
   "Abfragen des Bitmap-Handles fÅr das Vorschaufenster fehlgeschlagen."

#define SZERR_SETNOTEBOOKPAGES \
   "HinzufÅgen der Dialogseiten zum Einstellungsnotizbuch fehlgeschlagen."

#define SZERR_REGEXITLIST \
   "Registrieren der bei Anwendungsende auszufÅhrenden Exit-Routine fehlgeschlagen."

#define SZERR_TOOMANYENTRIES \
   "Zu viele EintrÑge in der Liste. Listendaten kînnen nicht in der INI-Datei "\
   "gespeichert werden. Einige EintrÑge entfernen und erneut versuchen."

#define SZERR_SAVEEXCPLIST \
   "Sichern der Liste der Programme, die von der Anwendung der Erweiterungen "\
   "fÅr die BenutzeroberflÑche auszunehmen sind, fehlgeschlagen."

#define SZERR_READFILE \
   "Lesen der Datei fehlgeschlagen: %s."

#define SZERR_INVALIDFILE \
   "Format der Datei: %s ungÅltig."

#define SZERR_BMPTOOLARGE \
   "Die Bitmap: %s Åberschreitet die Grenze von 64 KB."

#define SZERR_SAVEDATA \
   "Sichern der Daten in ESTYLER.INI fehlgeschlagen."

#define SZERR_APPLYBMP \
   "Festlegen der ausgewÑhlten Bitmap fehlgeschlagen."

#define SZERR_GETHPSFORHBMP \
   "Fehler bei Abfrage von Bitmap-Daten: Abfragen des Presentation-Space-Handle fehlgeschlagen."

#define SZERR_GETBMPHANDLE \
   "Fehler bei Abfrage von Bitmap-Daten: Abfragen des Bitmap-Handle fehlgeschlagen."

#define SZERR_COPYFILE \
   "Kopieren der Datei %s nach %s fehlgeschlagen."

#define SZERR_SOURCEEQDEST \
   "Quell- und Zielpfad stimmen Åberein."

#define SZERR_PROGLISTCOUNT \
   "Die Liste der Programme enthÑlt zu viele EintrÑge. Die letzten EintrÑge werden "\
   "nicht gesichert."

#define SZERR_INVALIDEXE \
   "Die Datei %s besitzt keine fÅr ausfÅhrbare Dateien gÅltige Erweiterung ("".CMD"", "\
   """.COM"", "".EXE"")."

#define SZERR_INSPAGE \
   "HinzufÅgen einer Seite zum Einstellungsnotizbuch fehlgeschlagen."

#define SZERR_DELPAGE \
   "Entfernen einer Seite aus dem Einstellungsnotizbuch fehlgeschlagen."

#define SZERR_GETBOTTLIST \
   "Abfragen der Liste startfÑhiger Volumen: Starten des Worker-Thread-Jobs fehlgeschlagen."

#define SZERR_HELPSYS \
   "\x0a" "Weitere Informationen durch Eingeben von HELPSYS%04u in einer Befehlszeile."

#define SZERR_GBLDISKHANDLE \
   "Abfragen des Laufwerks-Handles fehlgeschlagen."SZERR_HELPSYS

#define SZERR_GBLREADSECT1  \
   "Lesen des ersten Festplattensektors fehlgeschlagen."SZERR_HELPSYS

#define SZERR_GBLAIRBOOTVER \
   "UngÅltige AiRBOOT-Version. Bitte eine neuere Version installieren."

#define SZERR_GBLREADSECT2  \
   "Lesen des Festplattensektors mit AiRBOOT-Konfigurationsdaten fehlgeschlagen." \
   SZERR_HELPSYS

#define SZERR_GBLINVALIDDATA \
   "UngÅltige Signatur in AiRBOOT-Konfigurationsdaten."

#define SZERR_GBLREADSECT3  \
   "Lesen des Festplattensektors mit der Liste startfÑhiger " \
   "AiRBOOT-Volumen fehlgeschlagen."SZERR_HELPSYS

#define SZERR_LVMCODE \
   " (LVM-Fehler-Code: %d)."

#define SZERR_GBLOPENLVM    \
   "ôffnen der LVM-Schnittstelle fehlgeschlagen"SZERR_LVMCODE

#define SZERR_GBLLVMDRVLIST \
   "Abfrage der Laufwerksliste fehlgeschlagen"SZERR_LVMCODE

#define SZERR_GBLLVMPARTLIST \
   "Abfrage der Partitionsliste fehlgeschlagen"SZERR_LVMCODE

#define SZERR_GBLLVMVOLINFO \
   "Abfrage der LVM-Volumendaten fehlgeschlagen"SZERR_LVMCODE

#define SZERR_INITHELP \
   "Laden der Anwendungshilfedatei eStyler.hlp fehlgeschlagen."

#endif // #ifndef _TEXT_RES_H_
