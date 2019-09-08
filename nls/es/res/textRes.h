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
#define SZ__ADD               "A~¤adir"
#define SZ__REM               "~Quitar"
#define SZ__EDIT              "~Editar"
#define SZ__OK                "~Bien"
#define SZ__CANCEL            "~Cancelar"
#define SZ__OPEN              "~Abrir..."
#define SZ__SAVE              "~Guardar..."
#define SZ__SAVEAS            "Guardar ~como..."
#define SZ__EXIT              "~Salir"
#define SZ__FIND              "~Buscar..."
#define SZ_ADDTOTHELIST       "A¤adir a la lista"
#define SZ_FONT_              "~Fuente: %s  . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."


/* --------------------------------------------------------------------------
 MAINWINDOW        text displayed by the settings notebook parent
*/
// help title
#define SZ_APPNAME               "Styler"
#define SZ_APPLY                 "~Aplicar"
#define SZ_UNDO                  "~Deshacer"
#define SZ__DEFAULT              "~Por omisi¢n"
#define SZ_PREVIEW               "~Vista previa"
#define SZ_HELP                  "Ayuda"
// settings notebook titles
#define SZ_UITITLE               "Mejoras a la interfaz de usuario"
#define SZ_SHUTDOWNTITLE         "Concluir el sistema"
#define SZ_WINKEYTITLE           "Teclas de Windows"

/* --------------------------------------------------------------------------
 UIGEN             general User interface options
*/
#define SZ_UIENABLE              "~Habilitar mejoras a la interfaz de usuario"
#define SZ_EXCEPTIONS            "~No aplicar mejoras a:"

/* --------------------------------------------------------------------------
 TITLEBARGEN       general titlebar options
*/
#define SZ_TBENABLE              "~Habilitar mejoras a las barras de t¡tulo"
#define SZ_TBOVERRIDEPP          "~Ignorar par metros de presentaci¢n"
#define SZ__FONT                 "~Fuente:"
#define SZ_TEXTATLIGN            "Alineaci¢n del ~texto:"
#define SZ_LEFTCENTER            "A la izquierda\tCentrado\t"

/* --------------------------------------------------------------------------
 TITLEBAR          titlebar background and text options
*/
#define SZ_TYPEOFBKGND           "Tipo de ~fondo:"
#define SZ_COMBOBKGND            "Color s¢lido\tGradiente de color\tImagen\t"
// solid color mode controls
#define SZ_BKGNDCOLOR            "~Color de fondo"
// gradient mode controls
#define SZ_SHADEDIRECTION        "Direcci¢n del ~gradiente:"
#define SZ_COMBOSHADE            "Horizontal\tVertical\t" \
                                 "Horizontal - doble\tVertical - doble\t"
#define SZ_SWAPCOLORS            "~Intercambiar colores =>"
#define SZ_OUTERCOLOR            "Color ~exterior"
#define SZ_INNERCOLOR            "Color i~nterior"
#define SZ_LEFTCOLOR             "Color iz~quierdo"
#define SZ_RIGHTCOLOR            "Color de~recho"
#define SZ_TOPCOLOR              "Color a~lto"
#define SZ_BOTTOMCOLOR           "Color ~bajo"
// image mode controls
#define SZ_BKGNDIMAGE            "~Imagen de fondo:"

// !!! NOTE !!! when translating the following string do not remove the
// 3 leading blank spaces !!!!!!!!
#define SZ_DEFIMAGE              "   Imagen por omisi¢n"
#define SZ_SCALEIMAGE            "~Redimensionar la imagen"
//#define SZ_IMAGEOPTIONS          "~Image options..."
#define SZ_ADDIMAGE              "A¤adir ~nueva imagen..."
// border controls
#define SZ_SHOWBORDER            "~Borde para la barra de t¡tulo"
#define SZ_LEFTTOPCLR            "Color i~zquierda/arriba"
#define SZ_RIGHTBTMCLR           "Color de~recha/abajo"
// text options controls
#define SZ_TEXTSHADOW            "~Sombra para el texto"
#define SZ_TEXTCOLOR             "Color del ~texto"
#define SZ_TEXTSHADOWCLR         "Color de la s~ombra"

/* --------------------------------------------------------------------------
 PUSHBUTTON        pushbutton options
*/
#define SZ_BTNON                 "~Mejoras a los botones de los di logos"
#define SZ_BTNTHICK              "Borde ~grueso"
#define SZ_BTNFLAT               "~Borde plano"
#define SZ_BTNOVRCLR             "~Ignorar colores establecidos expl¡citamente"
#define SZ_BTN3DDEF              "Botones por ~omisi¢n 3D"
#define SZ_BTN3DDIS              "Botones inactivos ~3D"
#define SZ_BTNSOLID              "~S¢lo color de fondo"
//#define SZ_BKGNDIMAGE            "~Imagen de fondo:"
//#define SZ_BKGNDCOLOR            "~Color de fondo"

/* --------------------------------------------------------------------------
 DIALOG            dialog font options
*/
#define SZ_DLGFONTON             "~Establecer una fuente por omisi¢n para los di logos"
#define SZ_DLGOVERPP             "~Ignorar fuentes establecidas expl¡citamente"

/* --------------------------------------------------------------------------
 PRODINFO          product information dialog
*/
#define SZ_PRODUCT               "Styler para OS/2"
#define SZ_PIVERSION             "Versi¢n %d.%d - compilaci¢n n§. %d"
#define SZ_PICPYRIGHT            "¸ 1998-2004 Alessandro Felice Cantatore"
#define SZ_PICPYRIGHT2           "Partes ¸ 2008-2017 Alexander Taylor"
#define SZ_PIURL                 "https://github.com/altsan/os2-estyler"
#define SZ_PILICENSE             "Publicado bajo Licencia P£blica General de GNU versi¢n 3"

/* --------------------------------------------------------------------------
 SHUTDOWNGEN       shutdown - general options dialog
*/
#define SZ_SDWNON                "~Habilitar conclusi¢n extendida"
#define SZ_SDWNINCLUDE           "Incluir las siguientes opciones de conclusi¢n: "
#define SZ_SDWNLKSUSP            "~Bloquear y suspender"
#define SZ_SDWNSUSP              "~Suspender"
#define SZ_SDWNOFF               "~Apagar"
#define SZ_SDWNORD               "~Concluir"
#define SZ_SDWNREBOOT            "~Reiniciar"
#define SZ_SDWNANIMATE           "A~nimar el di logo de concluir"
#define SZ_SDWNSTEPS             "Pasos de la ani~maci¢n:"
#define SZ_SDWNADVOPTS           "~Opciones avanzadas..."

/* --------------------------------------------------------------------------
 DLG_SDTIMING      shutdown - general options dialog
*/
#define SZ_SDTIMTITLE            "Concluir - opciones de temporizaci¢n"
#define SZ_SDTIMWARNING          "No cambie los siguientes valores si no es "\
                                 "plenamente consciente de las consecuencias."
#define SZ_SDTIMDELAY            "Opciones de espera (d‚cimas de segundo):"
#define SZ_SDTIMLKUPSUSP         "~Bloquear y suspender :"
#define SZ_SDTIMSUSPEND          "~Suspender :"
#define SZ_SDTIMWARPCENTER       "Guardar opciones del ~WarpCenter :"
#define SZ_SDTIMUNROLLWIN        "~Desenrollar ventanas :"
#define SZ_SDTIMSYSMSGBOX        "Cuadros de ~mensajes de concluir el sistema :"
#define SZ_SDTIMWINSD            "WinS~hutdownSystem :"
#define SZ_SDTIMDOSSD            "DosShu~tdown :"
#define SZ_SDTIMOFF              "~Apagar :"
#define SZ_SDTIMKILL             "Dos~KillProcess :"
#define SZ_SDTIMOK               SZ__OK
#define SZ_SDTIMCANCEL           SZ__CANCEL
#define SZ_SDTIMDEFAULT          SZ__DEFAULT
#define SZ_SDTIMHELP             SZ_HELP

/* --------------------------------------------------------------------------
 REBOOTLIST        shutdown - reboot list options dialog
*/
#define SZ_SDWNVOLLIST           "~Permitir arrancar desde estos vol£menes tras reiniciar:"
#define SZ_SDWNVOLNAME           "Nombre del ~volumen:"
#define SZ_SDWNVOLDDESCR         "Descripci¢n del vo~lumen:"
#define SZ_SDWNREBOOTADD         SZ__ADD
#define SZ_SDWNREBOOTREM         SZ__REM
#define SZ_SDWNREBOOTEDIT        SZ__EDIT
#define SZ_SDWNREBOOTFIND        "~Buscar"

/* --------------------------------------------------------------------------
 WAITGETBOOTLIST   wait-while-searching-bootable-volumes dialog
*/
#define SZ_SDGETBOOTLIST        "Buscando vol£menes arrancables.\x0a\x0a" \
                                "Por favor, espere ..."

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRAMS  shutdown - shutdown programs list dialog
*/
#define SZ_SDWNRUNPROG           "Ejecutar los siguientes ~programas al concluir:"
#define SZ_SDWNPROG              "P~rograma:"
#define SZ_SDWNPARMS             "Par ~metros:"
#define SZ_SDWNWIN               "~Ventana:"
#define SZ_SDWNCOMBOWINS         "Normal\tMinimizada\tMaximizada\t"
#define SZ_SDWNPROGADD           SZ__ADD
#define SZ_SDWNPROGREM           SZ__REM
#define SZ_SDWNPROGEDIT          SZ__EDIT

/* --------------------------------------------------------------------------
 SHUTDOWNKILLLIST  shutdown - list of programs to be killed on shutdown dialog
*/
#define SZ_SDWNKILLPROG          "~Cerrar los siguientes programas al concluir:"

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
#define SZ_SDWNRUNPRGRS          "~Ejecutar programas de concluir"

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRESS  shutdown progress dialog
*/
#define SZ_SSDWNSDWNWAIT    "Por favor, espere mientras el sistema se cierra..."
#define SZ_SSDWNCOMPLETE    "Concluir ha terminado."
#define SZ_SSDWNREBOOTING   "El equipo se reiniciar  una vez concluido el sistema."
#define SZ_SSDWNPWRINGOFF   "El equipo se apagar  cuando concluir termine."
#define SZ_SSDWNSWTCHOFF    "Ya es seguro apagar el equipo."

/* --------------------------------------------------------------------------
 WINKEY            windows keys options dialog
*/
#define SZ_WINKEYON         "~Habilitar las teclas de Windows"
#define SZ_WKEYLEFT         "Tecla de Windows ~izquierda:"
#define SZ_WKEYRIGHT        "Tecla de Windows ~derecha:"
#define SZ_WKEYMENU         "Tecla de ~men£ de Windows:"

/* --------------------------------------------------------------------------
 PREVIEW           preview window
*/
#define SZ_PREVIEWTITLE     "Styler - vista previa de ventanas"
#define SZ_STATICSAMPLE     "Texto est tico de muestra"
#define SZ_BTNSAMPLES       "Botones de muestra: "
#define SZ__NORMAL          "~Normal"
#define SZ__DISABLED        "~Inactivo"


/* --------------------------------------------------------------------------
 COLORWHEEL        Color selection dialog
*/
#define SZ_CLRWTITLE        "Selecci¢n de color"
#define SZ__RED             "~Rojo"
#define SZ__GREEN           "~Verde"
#define SZ__BLUE            "~Azul"


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
#define SZ_PAGE1OF3         "P gina 1 de 3"
#define SZ_PAGE2OF3         "P gina 2 de 3"
#define SZ_PAGE3OF3         "P gina 3 de 3"
#define SZ_GENOPTIONS       "~Opciones generales"
#define SZ_TITLEBAR         "Barras de ~t¡tulo"
#define SZ_ACTIVEWIN        "Ventana activa"
#define SZ_INACTIVEWIN      "Ventana inactiva"
#define SZ_PUSHBUTTON       "~Botones"
#define SZ_DIALOGOPTS       "~Di logos"
#define SZ_PRODINFO         "~Informaci¢n del producto"
#define SZ_SHUTDOWN         "~Concluir"
#define SZ_REBOOT           "~Reiniciar"
#define SZ_SHUTDOWNPROGS    "~Programas"
#define SZ_KILLLIST         "~Lista de cierre"
#define SZ_VERSION          "Versi¢n: %d.%d.%d"
#define SZ_WKEYITEM01       "No realizar ninguna acci¢n"
#define SZ_WKEYITEM02       "Ocultar/mostrar todas las ventanas"
#define SZ_WKEYITEM03       "Mostrar la lista de ventanas"
#define SZ_WKEYITEM04       "Mostrar men£ de contexto"
#define SZ_WKEYITEM05       "Bloquear el Escritorio"
#define SZ_WKEYITEM06       "Abrir la carpeta Unidades"
#define SZ_WKEYITEM07       "Abrir la carpeta Configuraci¢n del sistema"
#define SZ_WKEYITEM08       "Abrir un indicador de mandatos en ventana"
#define SZ_WKEYITEM09       "Mostrar el men£ de escritorio del WarpCenter"
#define SZ_WKEYITEM10       "Mostrar la lista de ventanas del WarpCenter"
#define SZ_WKEYITEM11       "Mostrar la lista de procesos del WarpCenter"
#define SZ_WKEYITEM12       "Mostrar la lista de bandejas del WarpCenter"
#define SZ_WKEYITEM13       "Cambiar a la siguiente bandeja del WarpCenter"
#define SZ_WKEYITEM14       "Mostrar men£ de informaci¢n del WarpCenter"
#define SZ_WKEYITEM15       "Pulsar el Bot¢n de men£ (XCenter arriba)"
#define SZ_WKEYITEM16       "Pulsar el Bot¢n de men£ (XCenter abajo)"
#define SZ_WKEYITEM17       "Mostrar la lista de bandejas del XCenter"
#define SZ_WKEYITEM18       "Cambiar a la siguiente bandeja del XCenter"
#define SZ_WKEYITEM19       "Mostrar bandeja 1 del XCenter"
#define SZ_WKEYITEM20       "Mostrar bandeja 2 del XCenter"
#define SZ_WKEYITEM21       "Mostrar bandeja 3 del XCenter"
#define SZ_WKEYITEM22       "Mostrar bandeja 4 del XCenter"
#define SZ_WKEYITEM23       "Mostrar bandeja 5 del XCenter"
#define SZ_WKEYITEM24       "Mostrar bandeja 6 del XCenter"
#define SZ_WKEYITEM25       "Mostrar bandeja 7 del XCenter"
#define SZ_WKEYITEM26       "Mostrar bandeja 8 del XCenter"
#define SZ_WKEYITEM27       "Mostrar bandeja 9 del XCenter"
#define SZ_WKEYITEM28       "Mostrar bandeja 10 del XCenter"
#define SZ_WKEYITEM29       "Mover el XCenter tras/sobre las otras ventanas"
#define SZ_TITLEBARFONT     "Fuente de las barras de t¡tulo"
#define SZ_FONTDLGSAMPLE    "Styler - texto de muestra"
#define SZ_GRADIENTCOLOR    "Color %s del gradiente"
#define SZ_CLRSHADELEFT     "iz~quierdo"
#define SZ_CLRSHADTOP       "a~lto"
#define SZ_CLRSHADOUTER     "~exterior"
#define SZ_CLRSHADRIGHT     "derec~ho"
#define SZ_CLRSHADBOTTOM    "~bajo"
#define SZ_CLRSHADINNER     "i~nterior"
#define SZ_DIALOGFONT       "Fuente de los di logos"

/* --------------------------------------------------------------------------
 ERRORMSGS         error messages string resources
*/

#define SZERR_WORKERSEMAPHORE \
   "No se ha podido crear el sem foro de sincronizaci¢n de hilos."

#define SZERR_WORKERTHREAD \
   "No se ha podido crear el hilo de trabajo."

#define SZERR_LOADRUNTIMEMODULE \
   "No se ha podido cargar la biblioteca de ejecuci¢n (®SZ_MODULENAME¯.DLL)."

#define SZERR_QUERYPROCADDR \
   "No se ha podido importar los procedimientos del m¢dulo de ejecuci¢n."

#define SZERR_GETCUROPTIONS \
   "No se ha podido recabar las opciones de Styler activas actuales."

#define SZERR_ALLOCATION \
   "No se ha podido asignar memoria."

#define SZERR_OPENPRF \
   "No se ha podido abrir STYLER.INI."

#define SZERR_READPRFDATA \
   "No se ha podido leer los datos del archivo INI."

#define SZERR_INITPM \
   "No se ha podido inicializar el Presentation Manager."

#define SZERR_REGBOXCLASS \
   "No se ha podido registrar el control de la clase de barras/cajas."

#define SZERR_REGXCOMBOCLASS \
   "No se ha podido registrar el control de la lista desplegable mejorada."

#define SZERR_REGCLRBTNCLASS \
   "No se ha podido registrar el control de la clase de botones coloreados."

#define SZERR_GETBMPHANDLES \
   "No se ha podido encontrar los manejadores de imagen de la ventana de vistas previas."

#define SZERR_SETNOTEBOOKPAGES \
   "No se ha podido insertar las p ginas en el cuaderno de opciones."

#define SZERR_REGEXITLIST \
   "No se ha podido registrar la rutina de salida que ejecutar al final de la aplicaci¢n."

#define SZERR_TOOMANYENTRIES \
   "No se puede guardar la lista en el archivo INI porque es demasiado "\
   "larga. Borre algunas entradas e int‚ntelo otra vez."

#define SZERR_SAVEEXCPLIST \
   "No se ha podido guardar la lista de programas a los que no aplicar las "\
   "mejoras de la interfaz de usuario."

#define SZERR_READFILE \
   "No se ha podido leer el archivo: %s."

#define SZERR_INVALIDFILE \
   "El formato del archivo %s no es v lido."

#define SZERR_BMPTOOLARGE \
   "El mapa de bits : %s excede el l¡mite de 64 KBs."

#define SZERR_SAVEDATA \
   "No se ha podido guardar los datos en STYLER.INI."

#define SZERR_APPLYBMP \
   "No se ha podido aplicar la imagen seleccionada."

#define SZERR_GETHPSFORHBMP \
   "Error de datos de imagen: no se ha podido obtener el espacio de presentaci¢n."

#define SZERR_GETBMPHANDLE \
   "Error de datos: no se ha podido obtener la referencia de la imagen."

#define SZERR_COPYFILE \
   "No se ha podido copiar el archivo %s a %s."

#define SZERR_SOURCEEQDEST \
   "La v¡a de acceso de origen es igual a la de destino."

#define SZERR_PROGLISTCOUNT \
   "La lista de programas es demasiado larga. Las £ltimas entradas no se "\
   "almacenar n."

#define SZERR_INVALIDEXE \
   "El archivo %s no tiene una extensi¢n de ejecutable v lida (®.CMD¯, "\
   "®.COM¯, ®.EXE¯)."

#define SZERR_INSPAGE \
   "No se ha podido insertar una p gina en el cuaderno de opciones."

#define SZERR_DELPAGE \
   "No se ha podido borrar una p gina del cuaderno de opciones."

#define SZERR_GETBOTTLIST \
   "Lista de vol£menes arrancables: no se ha podido iniciar el hilo de trabajo."

#define SZERR_HELPSYS \
   "\x0a" "Escriba HELPSYS%04u en el indicador de mandatos para m s informaci¢n."

#define SZERR_GBLDISKHANDLE \
   "No se ha podido obtener el manejador de disco."SZERR_HELPSYS

#define SZERR_GBLREADSECT1  \
   "No se ha podido leer el primer sector del disco duro."SZERR_HELPSYS

#define SZERR_GBLAIRBOOTVER \
   "Versi¢n de AiRBOOT no soportada. Por favor, instale una nueva."

#define SZERR_GBLREADSECT2  \
   "No se ha podido leer el sector de datos de configuraci¢n de AiRBOOT." \
   SZERR_HELPSYS

#define SZERR_GBLINVALIDDATA \
   "Signatura inv lida en los datos de configuraci¢n de AiRBOOT."

#define SZERR_GBLREADSECT3  \
   "No se ha podido leer los sectores de la lista de vol£menes arrancables " \
   "de AiRBOOT."SZERR_HELPSYS

#define SZERR_LVMCODE \
   " (c¢digo de error de LVM: %d)."

#define SZERR_GBLOPENLVM    \
   "No se ha podido abrir el motor LVM"SZERR_LVMCODE

#define SZERR_GBLLVMDRVLIST \
   "No se ha podido obtener la lista de discos"SZERR_LVMCODE

#define SZERR_GBLLVMPARTLIST \
   "No se ha podido obtener la lista de particiones"SZERR_LVMCODE

#define SZERR_GBLLVMVOLINFO \
   "No se ha podido obtener los datos de vol£menes LVM"SZERR_LVMCODE

#define SZERR_INITHELP \
   "No se ha podido cargar el archivo de ayuda de la aplicaci¢n STYLER.HLP."

#endif // #ifndef _TEXT_RES_H_
