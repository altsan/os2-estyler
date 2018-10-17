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
#define SZ__ADD               "~Toevoegen"
#define SZ__REM               "Ve~rwijderen"
#define SZ__EDIT              "B~ewerken"
#define SZ__OK                "~OK"
#define SZ__CANCEL            "~Annuleren"
#define SZ__OPEN              "~Openen..."
#define SZ__SAVE              "Op~slaan..."
#define SZ__SAVEAS            "Opslaan ~als..."
#define SZ__EXIT              "E~inde"
#define SZ__FIND              "~Zoeken..."
#define SZ_ADDTOTHELIST       "Toevoegen aan de lijst"
#define SZ_FONT_              "~Lettertype:   %s   . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."


/* --------------------------------------------------------------------------
 MAINWINDOW        text displayed by the settings notebook parent
*/
// help title
#define SZ_APPNAME               "Styler"
#define SZ_APPLY                 "~Toepassen"
#define SZ_UNDO                  "~Ongedaan maken"
#define SZ__DEFAULT              "Stan~daard"
#define SZ_PREVIEW               "~Voorbeeld"
#define SZ_HELP                  "Help"
// settings notebook titles
#define SZ_UITITLE               "Gebruikersinterface verbeteringen"
#define SZ_SHUTDOWNTITLE         "Systeem afsluiten"
#define SZ_WINKEYTITLE           "Windowstoetsen"

/* --------------------------------------------------------------------------
 UIGEN             general User interface options
*/
#define SZ_UIENABLE              "Activ~eer gebruikersinterface verbeteringen"
#define SZ_EXCEPTIONS            "~Pas verbeteringen niet toe op:"

/* --------------------------------------------------------------------------
 TITLEBARGEN       general titlebar options
*/
#define SZ_TBENABLE              "Activ~eer titelbalk verbeteringen"
#define SZ_TBOVERRIDEPP          "~Vervang presentatie-parameters"
#define SZ__FONT                 "~Lettertype:"
#define SZ_TEXTATLIGN            "Tekst ~uitlijning:"
#define SZ_LEFTCENTER            "Links\tCentreren\t"

/* --------------------------------------------------------------------------
 TITLEBAR          titlebar background and text options
*/
#define SZ_TYPEOFBKGND           "Achtergrond~type:"
#define SZ_COMBOBKGND            "Vaste kleur\tVerlopend\tRaster afbeelding\t"
// solid color mode controls
#define SZ_BKGNDCOLOR            "~Achtergrondkleur"
// gradient mode controls
#define SZ_SHADEDIRECTION        "~Verlooprichting:"
#define SZ_COMBOSHADE            "Horizontaal\tVerticaal\t" \
                                 "Horizontaal - dubbel\tVerticaal - dubbel\t"
#define SZ_SWAPCOLORS            "Kleur wi~sselen =>"
#define SZ_OUTERCOLOR            "~Kleur buitenkant"
#define SZ_INNERCOLOR            "K~leur binnenkant"
#define SZ_LEFTCOLOR             "Kl~eur links"
#define SZ_RIGHTCOLOR            "Kle~ur rechts"
#define SZ_TOPCOLOR              "Kleu~r bovenzijde"
#define SZ_BOTTOMCOLOR           "Kleur ~onderzijde"
// image mode controls
#define SZ_BKGNDIMAGE            "Achter~grond afbeelding:"

// !!! NOTE !!! when translating the following string do not remove the
// 3 leading blank spaces !!!!!!!!
#define SZ_DEFIMAGE              "   Standaard afbeelding"
#define SZ_SCALEIMAGE            "~Schaal de afbeelding"
//#define SZ_IMAGEOPTIONS          "Afbeeld~ing opties..."
#define SZ_ADDIMAGE              "Een nieuwe afbeelding ~toevoegen..."
// border controls
#define SZ_SHOWBORDER            "~Toon de titelbalk-rand"
#define SZ_LEFTTOPCLR            "~Kleur links + bovenzijde"
#define SZ_RIGHTBTMCLR           "K~leur rechts + onderzijde"
// text options controls
#define SZ_TEXTSHADOW            "Tekst~schaduw"
#define SZ_TEXTCOLOR             "~Tekstkleur"
#define SZ_TEXTSHADOWCLR         "Tekstsc~haduw kleur"

/* --------------------------------------------------------------------------
 PUSHBUTTON        pushbutton options
*/
#define SZ_BTNON                 "Drukkno~p verbeteringen"
#define SZ_BTNTHICK              "~Dikke rand"
#define SZ_BTNFLAT               "~Platte rand"
#define SZ_BTNOVRCLR             "~Vervangen niet standaard kleuren"
#define SZ_BTN3DDEF              "~3D stijl standaard toetsen"
#define SZ_BTN3DDIS              "3~D stijl uitgeschakelde toetsen"
#define SZ_BTNSOLID              "Va~ste achtergrondkleur"
//#define SZ_BKGNDIMAGE            "~Achtergrond afbeelding:"
//#define SZ_BKGNDCOLOR            "~Achtergrondkleur"

/* --------------------------------------------------------------------------
 DIALOG            dialog font options
*/
#define SZ_DLGFONTON             "~Stel een standaard lettertype voor de dialoogvensters in"
#define SZ_DLGOVERPP             "~Vervang niet standaard lettertypes"

/* --------------------------------------------------------------------------
 PRODINFO          product information dialog
*/
#define SZ_PRODUCT               "Styler voor OS/2"
#define SZ_PIVERSION             "Versie %d.%d - build nr. %d"
#define SZ_PICPYRIGHT            "¸ 1998-2004 Alessandro Felice Cantatore"
#define SZ_PICPYRIGHT2           "Portions (c) 2008-2017 Alexander Taylor"
#define SZ_PIURL                 "https://github.com/altsan/os2-estyler"
#define SZ_PILICENSE             "Licensed under the GNU General Public License version 3"

/* --------------------------------------------------------------------------
 SHUTDOWNGEN       shutdown - general options dialog
*/
#define SZ_SDWNON                "Activ~eer uitgebreid afsluiten"
#define SZ_SDWNINCLUDE           "Neem de volgende afsluitopties op: "
#define SZ_SDWNLKSUSP            "Vergrende~len en slaapstand"
#define SZ_SDWNSUSP              "~Slaapstand"
#define SZ_SDWNOFF               "~Uitzetten"
#define SZ_SDWNOFF_USEACPI       "ACPI gebruiken"
#define SZ_SDWNORD               "Af~sluiten"
#define SZ_SDWNREBOOT            "Opnieuw sta~rten"
#define SZ_SDWNANIMATE           "Afsluitdialoog ~animatie"
#define SZ_SDWNSTEPS             "Animatie s~toppen:"
#define SZ_SDWNADVOPTS           "Uitgebreide ~opties..."

/* --------------------------------------------------------------------------
 DLG_SDTIMING      shutdown - general options dialog
*/
#define SZ_SDTIMTITLE            "Afsluit - tijdsopties"
#define SZ_SDTIMWARNING          "Verander de volgende waarden niet tenzij "\
                                 "u volledig bewust bent van de consequenties."
#define SZ_SDTIMDELAY            "Vertragingsinstellingen (tienden van seconden) :"
#define SZ_SDTIMLKUPSUSP         "Vergrende~len en slaapstand :"
#define SZ_SDTIMSUSPEND          "~Slaapstand :"
#define SZ_SDTIMWARPCENTER       "Sla ~WarpCenter instellingen op :"
#define SZ_SDTIMUNROLLWIN        "Vensters uitvo~uwen :"
#define SZ_SDTIMSYSMSGBOX        "Systeem afsluiten ~boodschappenvensters :"
#define SZ_SDTIMWINSD            "WinS~hutdownSystem :"
#define SZ_SDTIMDOSSD            "DosShu~tdown :"
#define SZ_SDTIMOFF              "~Uitzetten :"
#define SZ_SDTIMKILL             "Dos~KillProcess :"
#define SZ_SDTIMOK               SZ__OK
#define SZ_SDTIMCANCEL           SZ__CANCEL
#define SZ_SDTIMDEFAULT          SZ__DEFAULT
#define SZ_SDTIMHELP             SZ_HELP

/* --------------------------------------------------------------------------
 REBOOTLIST        shutdown - reboot list options dialog
*/
#define SZ_SDWNVOLLIST           "M~aak opstarten vanaf de volgende volumes mogelijk:"
#define SZ_SDWNVOLNAME           "~Volume naam:"
#define SZ_SDWNVOLDDESCR         "Volume ~omschrijving:"
#define SZ_SDWNREBOOTADD         SZ__ADD
#define SZ_SDWNREBOOTREM         SZ__REM
#define SZ_SDWNREBOOTEDIT        SZ__EDIT
#define SZ_SDWNREBOOTFIND        "Zoeken"

/* --------------------------------------------------------------------------
 WAITGETBOOTLIST   wait-while-searching-bootable-volumes dialog
*/
#define SZ_SDGETBOOTLIST        "Aan het zoeken naar opstartbare volumes.\x0a\x0a" \
                                "Moment geduld a.u.b. ..."

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRAMS  shutdown - shutdown programs list dialog
*/
#define SZ_SDWNRUNPROG           "Voe~r de volgende programma's uit bij het afsluiten:"
#define SZ_SDWNPROG              "~Programma:"
#define SZ_SDWNPARMS             "Para~meters:"
#define SZ_SDWNWIN               "~Venster:"
#define SZ_SDWNCOMBOWINS         "Normaal\tPictogrammen\tMaximum venster\t"
#define SZ_SDWNPROGADD           SZ__ADD
#define SZ_SDWNPROGREM           SZ__REM
#define SZ_SDWNPROGEDIT          SZ__EDIT

/* --------------------------------------------------------------------------
 SHUTDOWNKILLLIST  shutdown - list of programs to be killed on shutdown dialog
*/
#define SZ_SDWNKILLPROG          "S~top de volgende programma's bij het afsluiten:"

/* --------------------------------------------------------------------------
 SHUTDOWNSELECTION shutdown selection dialog
*/
// the strings have been already defined above :
// #define SZ_SHUTDOWNTITLE         "Systeem afsluiten"
// #define SZ_SDWNLKSUSP            "Vergrende~len en slaapstand"
// #define SZ_SDWNSUSP              "~Slaapstand"
// #define SZ_SDWNOFF               "~Uitzetten"
// #define SZ_SDWNORD               "Af~sluiten"
// #define SZ_SDWNREBOOT            "Opnieuw opsta~rten"
#define SZ_SDWN_INFO               "xxXXxx"
#define SZ_SDWNRUNPRGRS          "Vo~er afsluitprogramma's uit"

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRESS  shutdown progress dialog
*/
#define SZ_SSDWNSDWNWAIT    "Even geduld a.u.b. tijdens het afsluiten van het systeem . . ."
#define SZ_SSDWNCOMPLETE    "Afsluiten is klaar."
#define SZ_SSDWNREBOOTING   "Het systeem zal opnieuw opstarten als het afsluiten klaar is."
#define SZ_SSDWNPWRINGOFF   "Het systeem zal zichzelf uitzetten als het afsluiten "\
                            "klaar is."
#define SZ_SSDWNSWTCHOFF    "U kunt de computer veilig uitzetten."

/* --------------------------------------------------------------------------
 WINKEY            windows keys options dialog
*/
#define SZ_WINKEYON         "Activ~eer de Windowstoetsen"
#define SZ_WKEYLEFT         "~Linker Windowstoets:"
#define SZ_WKEYRIGHT        "~Rechter Windowstoets:"
#define SZ_WKEYMENU         "Windows ~menutoets:"

/* --------------------------------------------------------------------------
 PREVIEW           preview window
*/
#define SZ_PREVIEWTITLE     "Styler - voorbeeldvenster"
#define SZ_STATICSAMPLE     "Statisch tekstvoorbeeld"
#define SZ_BTNSAMPLES       "Toets voorbeelden: "
#define SZ__NORMAL          "~Normaal"
#define SZ__DISABLED        "Uitgeschakel~d"


/* --------------------------------------------------------------------------
 COLORWHEEL        Color selection dialog
*/
#define SZ_CLRWTITLE        "Kleurselectie"
#define SZ__RED             "~Rood"
#define SZ__GREEN           "~Groen"
#define SZ__BLUE            "~Blauw"


/* --------------------------------------------------------------------------
 STLRBMPED         Styler bitmap editor - implementation suspended
*/
// #define SZ_DLGSBMPED        "Styler raster afbeelding bewerker"
// #define SZ_SBMPSCALING      "Afbeeldingsschalings opties"
// #define SZ_SBMPTILE         "Afbeelding naas~t elkaar afbeelden (geen schaling)"
// #define SZ_SBMPWHOLE        "~Schaal de gehele afbeelding"
// #define SZ_SBMPLRBORDER     "~Linker/rechter randen + middendeel"
// #define SZ_SBMPBTBORDER     "~Onderkant/bovenkant randen + middendeel"
// #define SZ_SBMP9PARTS       "~Hoeken + randen + middendeel"
// #define SZ_SBMPBCWIDTH      "Rand/hoek ~breedte:"
// #define SZ_SBMPBCHEIGHT     "Rand/hoek ~hoogte:"
// #define SZ_SBMPPREVIEW      "Afbeeldingsvoorbeeld"
// #define SZ_SBMPSAMPLEHEIGHT "Afbeeldingsvoorbeeld hoogte (~pixels) :"


/* --------------------------------------------------------------------------
 STRINGRES         string resources.
*/
#define SZ_PAGE1OF3         "Pagina 1 van 3"
#define SZ_PAGE2OF3         "Pagina 2 van 3"
#define SZ_PAGE3OF3         "Pagina 3 van 3"
#define SZ_GENOPTIONS       "Algemene opties"
#define SZ_TITLEBAR         "Titelbalk"
#define SZ_ACTIVEWIN        "Actieve venster"
#define SZ_INACTIVEWIN      "Inactieve venster"
#define SZ_PUSHBUTTON       "Toetsen"
#define SZ_DIALOGOPTS       "Dialogen"
#define SZ_PRODINFO         "Produkt informatie"
#define SZ_SHUTDOWN         "Afsluiten"
#define SZ_REBOOT           "Opnieuw opstarten"
#define SZ_SHUTDOWNPROGS    "Programma's"
#define SZ_KILLLIST         "Stoppen lijst"
#define SZ_VERSION          "Versie: %d.%d.%d"
#define SZ_WKEYITEM01       "Voer geen enkele actie uit"
#define SZ_WKEYITEM02       "Verberg/toon alle vensters"
#define SZ_WKEYITEM03       "Toon vensterlijst"
#define SZ_WKEYITEM04       "Toon contextmenu"
#define SZ_WKEYITEM05       "Vergrendel de Werkplek"
#define SZ_WKEYITEM06       "Open de Stationsmap"
#define SZ_WKEYITEM07       "Open de Instellingen map"
#define SZ_WKEYITEM08       "Open een opdrachtregel venster"
#define SZ_WKEYITEM09       "Toon het WarpCenter Werkplekmenu"
#define SZ_WKEYITEM10       "Toon de WarpCenter vensterlijst"
#define SZ_WKEYITEM11       "Toon de WarpCenter proceslijst"
#define SZ_WKEYITEM12       "Toon de WarpCenter ladenlijst"
#define SZ_WKEYITEM13       "Schakel over naar de volgende WarpCenter lade"
#define SZ_WKEYITEM14       "Toon het WarpCenter informatiemenu"
#define SZ_WKEYITEM15       "Toon het XButton menu (boven XCenter)"
#define SZ_WKEYITEM16       "Toon het XButton menu (onder XCenter)"
#define SZ_WKEYITEM17       "Toon de XCenter ladenlijst"
#define SZ_WKEYITEM18       "Schakel over naar de volgende XCenter lade"
#define SZ_WKEYITEM19       "Toon XCenter lade 1"
#define SZ_WKEYITEM20       "Toon XCenter lade 2"
#define SZ_WKEYITEM21       "Toon XCenter lade 3"
#define SZ_WKEYITEM22       "Toon XCenter lade 4"
#define SZ_WKEYITEM23       "Toon XCenter lade 5"
#define SZ_WKEYITEM24       "Toon XCenter lade 6"
#define SZ_WKEYITEM25       "Toon XCenter lade 7"
#define SZ_WKEYITEM26       "Toon XCenter lade 8"
#define SZ_WKEYITEM27       "Toon XCenter lade 9"
#define SZ_WKEYITEM28       "Toon XCenter lade 10"
#define SZ_WKEYITEM29       "Verplaats het XCenter achter/bovenop de andere vensters"
#define SZ_TITLEBARFONT     "Titelbalk lettertype"
#define SZ_FONTDLGSAMPLE    "Styler - voorbeeldtekst"
#define SZ_GRADIENTCOLOR    "%s verloopkleur"
#define SZ_CLRSHADELEFT     "~Links"
#define SZ_CLRSHADTOP       "~Bovenkant"
#define SZ_CLRSHADOUTER     "B~uitenkant"
#define SZ_CLRSHADRIGHT     "~Rechts"
#define SZ_CLRSHADBOTTOM    "~Onderkant"
#define SZ_CLRSHADINNER     "B~innenkant"
#define SZ_DIALOGFONT       "Dialoogvensters lettertype"

/* --------------------------------------------------------------------------
 ERRORMSGS         error messages string resources
*/

#define SZERR_WORKERSEMAPHORE \
   "Het is niet gelukt om de thread synchronisatie semaphore te maken."

#define SZERR_WORKERTHREAD \
   "Het is niet gelukt om de arbeider thread te maken."

#define SZERR_LOADRUNTIMEMODULE \
   "Het is niet gelukt om de runtime module te laden ("SZ_MODULENAME".DLL)."

#define SZERR_QUERYPROCADDR \
   "Het is niet gelukt om de runtime module procedures te importeren."

#define SZERR_GETCUROPTIONS \
   "Het is niet gelukt om de huidige actieve Styler opties te achterhalen."

#define SZERR_ALLOCATION \
   "Het is niet gelukt om geheugen te alloceren."

#define SZERR_OPENPRF \
   "Het is niet gelukt om STYLER.INI te openen."

#define SZERR_READPRFDATA \
   "Het is niet gelukt om de INI-bestand gegevens te laden."

#define SZERR_INITPM \
   "Het is niet gelukt om de Presentation Manager te initialiseren."

#define SZERR_REGBOXCLASS \
   "Het is niet gelukt om de doos/balk optie class  te registreren."

#define SZERR_REGXCOMBOCLASS \
   "Het is niet gelukt om de uitgebreide selectielijst optie te registreren."

#define SZERR_REGCLRBTNCLASS \
   "Het is niet gelukt om de gekleurde toets optie class te registreren."

#define SZERR_GETBMPHANDLES \
   "Het is niet gelukt om de raster afbeelding handvaten voor het voorbeeldvenster te achterhalen."

#define SZERR_SETNOTEBOOKPAGES \
   "Het is niet gelukt om de dialoogpagina's in het instellingennotitieblok in te voegen."

#define SZERR_REGEXITLIST \
   "Het is niet gelukt om de afsluitroutine te registreren die uitgevoerd moet worden bij het be‰indigen van een applicatie."

#define SZERR_TOOMANYENTRIES \
   "Er zijn te veel ingangen in de lijst. De lijst kan niet opgeslagen worden "\
   "in het INI bestand. Verwijder enkele ingangen en probeer opnieuw."

#define SZERR_SAVEEXCPLIST \
   "Het is niet gelukt om de lijst van programma's die uitgesloten zouden moeten worden voor "\
   "de gebruikersinterface verbeteringen op te slaan."

#define SZERR_READFILE \
   "Het is niet gelukt om bestand %s te lezen."

#define SZERR_INVALIDFILE \
   "De indeling van het %s bestand is niet goed."

#define SZERR_BMPTOOLARGE \
   "De raster afbeelding %s is groter dan de 64 KB grens."

#define SZERR_SAVEDATA \
   "Het is niet gelukt gegevens op te slaan in STYLER.INI."

#define SZERR_APPLYBMP \
   "Het is niet gelukt om de geselecteerde raster afbeelding in te stellen."

#define SZERR_GETHPSFORHBMP \
   "Krijg een raster afbeelding gegevensfout: het is niet gelukt het presentation space handvat te achterhalen."

#define SZERR_GETBMPHANDLE \
   "Krijg een raster afbeeldin gegevensfout: het is niet gelukt het raster afbeelding handvat te achterhalen."

#define SZERR_COPYFILE \
   "Het is niet gelukt het bestand %s te kopi‰ren naar %s."

#define SZERR_SOURCEEQDEST \
   "Het bronpad is gelijk aan het doelpad."

#define SZERR_PROGLISTCOUNT \
   "De programmalijst bevat te veel onderwerpen. De laatste onderwerpen zullen niet "\
   "opgeslagen worden."

#define SZERR_INVALIDEXE \
   "Het bestand %s heeft geen correct uitvoerbare extensie ("".CMD"", "\
   """.COM"", "".EXE"")."

#define SZERR_INSPAGE \
   "Het is niet gelukt om een pagina in het instellingennotitieblok in te voegen."

#define SZERR_DELPAGE \
   "Het is niet gelukt om een pagina uit het instellingennotitieblok te verwijderen."

#define SZERR_GETBOTTLIST \
   "Ophalen lijst met opstartbare volumes: het is niet gelukt de werk thread taak te starten."

#define SZERR_HELPSYS \
   "\x0a" "Type HELPSYS%04u in een opdrachtregel venster voor meer informatie."

#define SZERR_GBLDISKHANDLE \
   "Het is niet gelukt het schijf handvat te achterhalen."SZERR_HELPSYS

#define SZERR_GBLREADSECT1  \
   "Het is niet gelukt de eerste sector van de harde schijf te lezen."SZERR_HELPSYS

#define SZERR_GBLAIRBOOTVER \
   "Verkeerde AiRBOOT versie. Installeer a.u.b. een nieuwere versie."

#define SZERR_GBLREADSECT2  \
   "Het is niet gelukt om de schijfsector te lezen die de AiRBOOT configuratie gegevens bevat." \
   SZERR_HELPSYS

#define SZERR_GBLINVALIDDATA \
   "Verkeerde handtekening in AiRBOOT configuratie gegevens."

#define SZERR_GBLREADSECT3  \
   "Het is niet gelukt om de schijfsectoren te lezen die de lijst van " \
   "opstartbare AiRBOOT volumes bevatten."SZERR_HELPSYS

#define SZERR_LVMCODE \
   " (LVM foutcode: %d)."

#define SZERR_GBLOPENLVM    \
   "Het is niet gelukt om de LVM motor te starten"SZERR_LVMCODE

#define SZERR_GBLLVMDRVLIST \
   "Het is niet gelukt de schijvenlijst te achterhalen"SZERR_LVMCODE

#define SZERR_GBLLVMPARTLIST \
   "Het is niet gelukt de partitielijst te achterhalen"SZERR_LVMCODE

#define SZERR_GBLLVMVOLINFO \
   "Het is niet gelukt om LVM volume gegevens te achterhalen"SZERR_LVMCODE

#define SZERR_INITHELP \
   "Het is niet gelukt om het applicatie helpbestand STYLER.HLP te laden."

#endif // #ifndef _TEXT_RES_H_
