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
#define SZ__ADD               "~Добавить"
#define SZ__REM               "~Убрать"
#define SZ__EDIT              "~Редактировать"
#define SZ__OK                "~ОК"
#define SZ__CANCEL            "От~менить"
#define SZ__OPEN              "О~ткрыть..."
#define SZ__SAVE              "~Сохранить..."
#define SZ__SAVEAS            "Сохранить ~как..."
#define SZ__EXIT              "В~ыход"
#define SZ__FIND              "~Найти..."
#define SZ_ADDTOTHELIST       "Добавить в список"
#define SZ_FONT_              "~Шрифт:   %s   . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."\
                              " . . . . . . . . . . . . . . . . . . . . . . ."


/* --------------------------------------------------------------------------
 MAINWINDOW        text displayed by the settings notebook parent
*/
// help title
#define SZ_APPNAME               "Styler"
#define SZ_APPLY                 "~Применить"
#define SZ_UNDO                  "~Откатить"
#define SZ__DEFAULT              "По ~умолчанию"
#define SZ_PREVIEW               "~ПредПросмотр"
#define SZ_HELP                  "Справка"
// settings notebook titles
#define SZ_UITITLE               "Улучшение интерфейса пользователя"
#define SZ_SHUTDOWNTITLE         "Выключить систему"
#define SZ_WINKEYTITLE           "Клавиши Windows"

/* --------------------------------------------------------------------------
 UIGEN             general User interface options
*/
#define SZ_UIENABLE              "~Разрешить улучшение интерфейса пользователя"
#define SZ_EXCEPTIONS            "~Не применять улучшения к:"

/* --------------------------------------------------------------------------
 TITLEBARGEN       general titlebar options
*/
#define SZ_TBENABLE              "~Разрешить улучшение строки заголовка"
#define SZ_TBOVERRIDEPP          "~Переопределять параметры представления"
#define SZ__FONT                 "~Шрифт:"
#define SZ_TEXTATLIGN            "~Выравнивание текста:"
#define SZ_LEFTCENTER            "По левому краю\tПо центру\t"

/* --------------------------------------------------------------------------
 TITLEBAR          titlebar background and text options
*/
#define SZ_TYPEOFBKGND           "~Тип фона:"
#define SZ_COMBOBKGND            "Сплошной цвет\tГрадиент\tИзображение\t"
// solid color mode controls
#define SZ_BKGNDCOLOR            "~Цвет фона"
// gradient mode controls
#define SZ_SHADEDIRECTION        "~Направление градиента:"
#define SZ_COMBOSHADE            "Горизонтально\tВертикально\t" \
                                 "Горизонтально - с двух сторон\tВертикально - с двух сторон\t"
#define SZ_SWAPCOLORS            "~Поменять цвета =>"
#define SZ_OUTERCOLOR            "Цвет ~снаружи"
#define SZ_INNERCOLOR            "Цвет вн~утри"
#define SZ_LEFTCOLOR             "Цвет слева"
#define SZ_RIGHTCOLOR            "Цвет справа"
#define SZ_TOPCOLOR              "Цвет сверху"
#define SZ_BOTTOMCOLOR           "Цвет снизу"
// image mode controls
#define SZ_BKGNDIMAGE            "Фоновое ~изображение:"

// !!! NOTE !!! when translating the following string do not remove the
// 3 leading blank spaces !!!!!!!!
#define SZ_DEFIMAGE              "   Изображение по умолчанию"
#define SZ_SCALEIMAGE            "~Масштабировать изображение"
//#define SZ_IMAGEOPTIONS        "Параметры и~зображения..."
#define SZ_ADDIMAGE              "Добавить ~новое изображение..."
// border controls
#define SZ_SHOWBORDER            "По~казать рамку строки заголовка"
#define SZ_LEFTTOPCLR            "Цвет левого/вер~хнего"
#define SZ_RIGHTBTMCLR           "Цвет правого/ни~жнего"
// text options controls
#define SZ_TEXTSHADOW            "Те~нь от текста"
#define SZ_TEXTCOLOR             "Ц~вет текста"
#define SZ_TEXTSHADOWCLR         "Цве~т тени"

/* --------------------------------------------------------------------------
 PUSHBUTTON        pushbutton options
*/
#define SZ_BTNON                 "Улучшение ~кнопок"
#define SZ_BTNTHICK              "~Утолщённая рамка"
#define SZ_BTNFLAT               "~Плоская рамка"
#define SZ_BTNOVRCLR             "Пере~определить цвета не по умолчанию"
#define SZ_BTN3DDEF              "~3D-стиль кнопки по умолчанию"
#define SZ_BTN3DDIS              "3~D-стиль неактивной кнопки"
#define SZ_BTNSOLID              "~Сплошной цвет фона"
//#define SZ_BKGNDIMAGE            "Фоновое ~изображение:"
//#define SZ_BKGNDCOLOR            "~Цвет фона"

/* --------------------------------------------------------------------------
 DIALOG            dialog font options
*/
#define SZ_DLGFONTON             "~Установить шрифт по умолчанию для диалоговых окон"
#define SZ_DLGOVERPP             "~Переопределить шрифты не по умолчанию"

/* --------------------------------------------------------------------------
 PRODINFO          product information dialog
*/
#define SZ_PRODUCT               "Styler for OS/2"
#define SZ_PIVERSION             "Версия %d.%d - сборка № %d"
#define SZ_PICPYRIGHT            "(c) 1998-2004 Alessandro Felice Cantatore"
#define SZ_PICPYRIGHT2           "Частично (c) 2008-2019 Alexander Taylor"
#define SZ_PIURL                 "https://github.com/altsan/os2-estyler"
#define SZ_PILICENSE             "Под лицензией GNU General Public License версии 3"

/* --------------------------------------------------------------------------
 SHUTDOWNGEN       shutdown - general options dialog
*/
#define SZ_SDWNON                "~Использовать расширенное выключение"
#define SZ_SDWNINCLUDE           "Включая следующие параметры выключения: "
#define SZ_SDWNLKSUSP            "~Заблокировать и приостановить"
#define SZ_SDWNSUSP              "Прио~становить"
#define SZ_SDWNOFF               "~Отключить питание"
#define SZ_SDWNORD               "~Выключить"
#define SZ_SDWNREBOOT            "~Перезагрузить"
#define SZ_SDWNANIMATE           "~Анимированный диалог выключения"
#define SZ_SDWNSTEPS             "~Шагов анимации:"
#define SZ_SDWNADVOPTS           "~Расширенные опции..."

/* --------------------------------------------------------------------------
 DLG_SDTIMING      shutdown - general options dialog
*/
#define SZ_SDTIMTITLE            "Выключение - временные параметры"
#define SZ_SDTIMWARNING          "Не меняйте следующие значения, если Вы "\
                                 "полностью не осознаёте последствия."
#define SZ_SDTIMDELAY            "Настройки задержки (десятые секунды) :"
#define SZ_SDTIMLKUPSUSP         "~Заблокировать и приостановить :"
#define SZ_SDTIMSUSPEND          "Прио~становить :"
#define SZ_SDTIMWARPCENTER       "Сохранить настройки ~WarpCenter :"
#define SZ_SDTIMUNROLLWIN        "~Развернуть окна :"
#define SZ_SDTIMSYSMSGBOX        "Окна соо~бщений о выключении системы :"
#define SZ_SDTIMWINSD            "WinS~hutdownSystem :"
#define SZ_SDTIMDOSSD            "DosShu~tdown :"
#define SZ_SDTIMOFF              "~Отключить питание :"
#define SZ_SDTIMKILL             "Dos~KillProcess :"
#define SZ_SDTIMOK               SZ__OK
#define SZ_SDTIMCANCEL           SZ__CANCEL
#define SZ_SDTIMDEFAULT          SZ__DEFAULT
#define SZ_SDTIMHELP             SZ_HELP

/* --------------------------------------------------------------------------
 REBOOTLIST        shutdown - reboot list options dialog
*/
#define SZ_SDWNVOLLIST           "~Разрешить перезагрузку следующих томов:"
#define SZ_SDWNVOLNAME           "~Имя тома:"
#define SZ_SDWNVOLDDESCR         "~Описание тома:"
#define SZ_SDWNREBOOTADD         SZ__ADD
#define SZ_SDWNREBOOTREM         SZ__REM
#define SZ_SDWNREBOOTEDIT        SZ__EDIT
#define SZ_SDWNREBOOTFIND        SZ__FIND

/* --------------------------------------------------------------------------
 WAITGETBOOTLIST   wait-while-searching-bootable-volumes dialog
*/
#define SZ_SDGETBOOTLIST        "Ищем загрузочные тома.\x0a\x0a" \
                                "Подождите пожалуйста..."

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRAMS  shutdown - shutdown programs list dialog
*/
#define SZ_SDWNRUNPROG           "~Выполнить следующие программы при выключении:"
#define SZ_SDWNPROG              "~Программа:"
#define SZ_SDWNPARMS             "Пара~метры:"
#define SZ_SDWNWIN               "~Окно:"
#define SZ_SDWNCOMBOWINS         "Обычное\tМини\tМакси\t"
#define SZ_SDWNPROGADD           SZ__ADD
#define SZ_SDWNPROGREM           SZ__REM
#define SZ_SDWNPROGEDIT          SZ__EDIT

/* --------------------------------------------------------------------------
 SHUTDOWNKILLLIST  shutdown - list of programs to be killed on shutdown dialog
*/
#define SZ_SDWNKILLPROG          "~Прервать работу следующих программ при выключении:"

/* --------------------------------------------------------------------------
 SHUTDOWNSELECTION shutdown selection dialog
*/
// the strings have been already defined above :
// #define SZ_SHUTDOWNTITLE         "Выключить систему"
// #define SZ_SDWNLKSUSP            "~Заблокировать и приостановить"
// #define SZ_SDWNSUSP              "Прио~становить"
// #define SZ_SDWNOFF               "~Отключить питание"
// #define SZ_SDWNORD               "~Выключить"
// #define SZ_SDWNREBOOT            "~Перезагрузить"
#define SZ_SDWNRUNPRGRS          "Выполнение про~граммы выключения"

/* --------------------------------------------------------------------------
 SHUTDOWNPROGRESS  shutdown progress dialog
*/
#define SZ_SSDWNSDWNWAIT    "Подождите, пока система выключится . . ."
#define SZ_SSDWNCOMPLETE    "Выключение завершено."
#define SZ_SSDWNREBOOTING   "После успешного выключения система перезагрузится."
#define SZ_SSDWNPWRINGOFF   "Отключение питания после успешного выключения."
#define SZ_SSDWNSWTCHOFF    "Вы можете безопасно отключить питание компьютера."

/* --------------------------------------------------------------------------
 WINKEY            windows keys options dialog
*/
#define SZ_WINKEYON         "~Использовать клавиши Windows"
#define SZ_WKEYLEFT         "~Левая клавиша Windows:"
#define SZ_WKEYRIGHT        "~Правая клавиша Windows:"
#define SZ_WKEYMENU         "Клавиша Windows ~меню:"

/* --------------------------------------------------------------------------
 PREVIEW           preview window
*/
#define SZ_PREVIEWTITLE     "Стайлер - окно предварительного просмотра"
#define SZ_STATICSAMPLE     "Образец статичного текста"
#define SZ_BTNSAMPLES       "Примеры кнопок: "
#define SZ__NORMAL          "~Обычная"
#define SZ__DISABLED        "~Неактивная"


/* --------------------------------------------------------------------------
 COLORWHEEL        Color selection dialog
*/
#define SZ_CLRWTITLE        "Выбираем цвет"
#define SZ__RED             "~Красный"
#define SZ__GREEN           "~Зелёный"
#define SZ__BLUE            "~Синий"


/* --------------------------------------------------------------------------
 STLRBMPED         Styler bitmap editor - implementation suspended
*/
// #define SZ_DLGSBMPED        "Редактор растровых изображений Стайлера"
// #define SZ_SBMPSCALING      "Параметры масштабирования изображения"
// #define SZ_SBMPTILE         "~Мозаика изображений (без масштабирования)"
// #define SZ_SBMPWHOLE        "Масштабировать ~всё изображение"
// #define SZ_SBMPLRBORDER     "~Левая/правая граница + средняя часть"
// #define SZ_SBMPBTBORDER     "~Нижняя/верхняя граница + средняя часть"
// #define SZ_SBMP9PARTS       "~Углы + границы + средняя часть"
// #define SZ_SBMPBCWIDTH      "~Ширина рамки/края:"
// #define SZ_SBMPBCHEIGHT     "~Высота рамки/края:"
// #define SZ_SBMPPREVIEW      "Предпросмотр изображения"
// #define SZ_SBMPSAMPLEHEIGHT "Высота предпросмотра изображения (~пикселей) :"


/* --------------------------------------------------------------------------
 STRINGRES         string resources.
*/
#define SZ_PAGE1OF3         "Страница 1 из 3"
#define SZ_PAGE2OF3         "Страница 2 из 3"
#define SZ_PAGE3OF3         "Страница 3 из 3"
#define SZ_GENOPTIONS       "Общие настройки"
#define SZ_TITLEBAR         "Строка Заголовка"
#define SZ_ACTIVEWIN        "Активное окно"
#define SZ_INACTIVEWIN      "Неактивное окно"
#define SZ_PUSHBUTTON       "Кнопка"
#define SZ_DIALOGOPTS       "Диалоги"
#define SZ_PRODINFO         "Информация о продукте"
#define SZ_SHUTDOWN         "Выключить"
#define SZ_REBOOT           "Перезагрузить"
#define SZ_SHUTDOWNPROGS    "Программы"
#define SZ_KILLLIST         "Терминаторный список"
#define SZ_VERSION          "Версия: %d.%d.%d"
#define SZ_WKEYITEM01       "Не выполняйте никаких действий"
#define SZ_WKEYITEM02       "Скрыть/показать все окна"
#define SZ_WKEYITEM03       "Показать список окон"
#define SZ_WKEYITEM04       "Показать контекстное меню"
#define SZ_WKEYITEM05       "Заблокировать десктоп"
#define SZ_WKEYITEM06       "Открыть папку Накопители"
#define SZ_WKEYITEM07       "Открыть папку Настройка системы"
#define SZ_WKEYITEM08       "Открыть окно камандной строки"
#define SZ_WKEYITEM09       "Показать меню десктоп WarpCenter"
#define SZ_WKEYITEM10       "Показать список окон WarpCenter"
#define SZ_WKEYITEM11       "Показать список процессов WarpCenter"
#define SZ_WKEYITEM12       "Показать список лотков WarpCenter"
#define SZ_WKEYITEM13       "Переход к следующему лотку WarpCenter"
#define SZ_WKEYITEM14       "Показать информационное меню WarpCenter"
#define SZ_WKEYITEM15       "Показать меню XButton (сверху XCenter)"
#define SZ_WKEYITEM16       "Показать меню XButton (снизу XCenter)"
#define SZ_WKEYITEM17       "Показать список лотков XCenter"
#define SZ_WKEYITEM18       "Переход к следующему лотку XCenter"
#define SZ_WKEYITEM19       "Показать XCenter лоток 1"
#define SZ_WKEYITEM20       "Показать XCenter лоток 2"
#define SZ_WKEYITEM21       "Показать XCenter лоток 3"
#define SZ_WKEYITEM22       "Показать XCenter лоток 4"
#define SZ_WKEYITEM23       "Показать XCenter лоток 5"
#define SZ_WKEYITEM24       "Показать XCenter лоток 6"
#define SZ_WKEYITEM25       "Показать XCenter лоток 7"
#define SZ_WKEYITEM26       "Показать XCenter лоток 8"
#define SZ_WKEYITEM27       "Показать XCenter лоток 9"
#define SZ_WKEYITEM28       "Показать XCenter лоток 10"
#define SZ_WKEYITEM29       "Переместить XCenter за/поверх других окон"
#define SZ_TITLEBARFONT     "Шрифт строки заголовка"
#define SZ_FONTDLGSAMPLE    "Стайлер - простой текст"
#define SZ_GRADIENTCOLOR    "%s цвет градиента"
#define SZ_CLRSHADELEFT     "Слева"
#define SZ_CLRSHADTOP       "Сверху"
#define SZ_CLRSHADOUTER     "Снаружи"
#define SZ_CLRSHADRIGHT     "Справа"
#define SZ_CLRSHADBOTTOM    "Снизу"
#define SZ_CLRSHADINNER     "Внутри"
#define SZ_DIALOGFONT       "Диалог Шрифты"

/* --------------------------------------------------------------------------
 ERRORMSGS         error messages string resources
*/

#define SZERR_WORKERSEMAPHORE \
   "Не удалось создать семафор синхронизации потока."

#define SZERR_WORKERTHREAD \
   "Не удалось создать рабочий поток."

#define SZERR_LOADRUNTIMEMODULE \
   "Не удалось загрузить выполняемый модуль ("SZ_MODULENAME".DLL)."

#define SZERR_QUERYPROCADDR \
   "Не удалось импортировать процедуры выполняемого модуля."

#define SZERR_GETCUROPTIONS \
   "Не удалось получить текущие параметры Стайлера."

#define SZERR_ALLOCATION \
   "Не удалось выделить память."

#define SZERR_OPENPRF \
   "Не удалось открыть STYLER.INI."

#define SZERR_READPRFDATA \
   "Не удалось прочитать данные файла INI."

#define SZERR_INITPM \
   "Не удалось инициализировать Presentation Manager."

#define SZERR_REGBOXCLASS \
   "Не удалось зарегистрировать элемент управления классом box/bar."

#define SZERR_REGXCOMBOCLASS \
   "Не удалось зарегистрировать элемент управления расширенным раскрывающимся списком."

#define SZERR_REGCLRBTNCLASS \
   "Не удалось зарегистрировать элемент управления классом цветной кнопки."

#define SZERR_GETBMPHANDLES \
   "Не удалось получить дескрипторы растрового изображения для окна предварительного просмотра."

#define SZERR_SETNOTEBOOKPAGES \
   "Не удалось вставить страницы диалогов в блокнот настроек."

#define SZERR_REGEXITLIST \
   "Не удалось зарегистрировать процедуру выхода для запуска на стороне приложения."

#define SZERR_TOOMANYENTRIES \
   "В списке слишком много записей. Данные списка не могут быть сохранены в "\
   "файле INI. Удалите некоторые элементы и попробуйте ещё раз."

#define SZERR_SAVEEXCPLIST \
   "Не удалось сохранить список программ, для которых не следует "\
   "применять улучшения интерфейса пользователя."

#define SZERR_READFILE \
   "Не удалось прочитать файл: %s."

#define SZERR_INVALIDFILE \
   "Формат файла: %s недействителен."

#define SZERR_BMPTOOLARGE \
   "Растровое изображение: %s превышает предел в 64 КБ."

#define SZERR_SAVEDATA \
   "Не удалось сохранить данные в STYLER.INI."

#define SZERR_APPLYBMP \
   "Не удалось установить выбранное растровое изображение."

#define SZERR_GETHPSFORHBMP \
   "Ошибка получения данных растрового изображения: не удалось получить дескриптор пространства представления."

#define SZERR_GETBMPHANDLE \
   "Ошибка получения данных растрового изображения: не удалось получить дескриптор растрового изображения."

#define SZERR_COPYFILE \
   "Не удалось скопировать файл %s в %s."

#define SZERR_SOURCEEQDEST \
   "Исходный путь совпадает с путём назначения."

#define SZERR_PROGLISTCOUNT \
   "В списке программ слишком много элементов. "\
   "Последние элементы не будут сохранены."

#define SZERR_INVALIDEXE \
   "Файл %s не имеет допустимого расширения исполняемого файла "\
   "("".CMD"", "".COM"", "".EXE"")."

#define SZERR_INSPAGE \
   "Не удалось вставить страницу в блокнот настроек."

#define SZERR_DELPAGE \
   "Не удалось удалить страницу в блокноте настроек."

#define SZERR_GETBOTTLIST \
   "Получить список загрузочных томов: не удалось запустить поток с заданием."

#define SZERR_HELPSYS \
   "\x0a" "Для получения дополнительной информации введите HELPSYS%04u в окне командной строки."

#define SZERR_GBLDISKHANDLE \
   "Не удалось получить дескриптор диска."SZERR_HELPSYS

#define SZERR_GBLREADSECT1  \
   "Не удалось прочитать первый сектор жёсткого диска."SZERR_HELPSYS

#define SZERR_GBLAIRBOOTVER \
   "Неправильная версия AiRBOOT. Пожалуйста, установите новую версию."

#define SZERR_GBLREADSECT2  \
   "Не удалось прочитать сектор диска, содержащий данные конфигурации AiRBOOT." \
   SZERR_HELPSYS

#define SZERR_GBLINVALIDDATA \
   "Неправильная сигнатура данных конфигурации AiRBOOT."

#define SZERR_GBLREADSECT3  \
   "Не удалось прочитать сектора диска, содержащие список  " \
   "загрузочных томов AiRBOOT."SZERR_HELPSYS

#define SZERR_LVMCODE \
   " (код ошибки LVM: %d)."

#define SZERR_GBLOPENLVM    \
   "Не удалось открыть систему LVM"SZERR_LVMCODE

#define SZERR_GBLLVMDRVLIST \
   "Не удалось получить список дисков"SZERR_LVMCODE

#define SZERR_GBLLVMPARTLIST \
   "Не удалось получить список разделов"SZERR_LVMCODE

#define SZERR_GBLLVMVOLINFO \
   "Не удалось получить данные LVM для тома"SZERR_LVMCODE

#define SZERR_INITHELP \
   "Не удалось загрузить файл справки STYLER.HLP."

#endif // #ifndef _TEXT_RES_H_
