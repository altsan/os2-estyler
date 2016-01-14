/* --------------------------------------------------------------------------
 shutdown.h : structures and definitions used for the system shutdown
              enhancements
 2004/05/08 - Alessandro Cantatore - Italy
 Notes: ------------------------------------------------------------------
 Contents: ---------------------------------------------------------------
 SENDPWREV         structure used to execute a power management function.
 POWERMGRDEFS      definitions used to execute power management functions.
 SHUTDOWNREQPARM   2nd message parameter of the STLRM_SHUTDOWNREQ PM message.
 REBOOTLIST        list of bootable volumes stored in eStyler.ini
 SHDWNPRGLIST      list of programs to be executed on shutdown.
 KILL_LIST         list of programs to be killed on system shutdown.
 SHUTDOWNSEL       structure used by the shutdown selection dialog
 CLASSNAMES        window class names
 PROTOTYPES        function prototypes
-------------------------------------------------------------------------- */

#ifndef _STLR_SHUTDOWN_H_
   #define _STLR_SHUTDOWN_H_


/* --------------------------------------------------------------------------
 SENDPWREV         structure used to execute a power management function
                   via DosDevIOCtl().
*/
#pragma pack(2)
typedef struct {
   USHORT usid;
   USHORT usres;
   USHORT us1;
   USHORT us2;
} SENDPWREV, * PSENDPWREV;
#pragma pack()

/* --------------------------------------------------------------------------
 POWERMGRDEFS      definitions used to execute power management functions
                   via DosDevIOCtl.
*/
#define ID_ALL                  0x0001
#define ID_DISPLAY              0x0100
#define ID_SECSTORAGE           0x0200
#define ID_LPT                  0x0300
#define ID_COM                  0x0400
#define ID_NETWORK              0x0500
#define ID_PCMCIA               0x0600
#define ID_BATTERY              0x8000

#define APMDEV                  "\\DEV\\APM$"

#define OPNDEVFL  \
 (OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYNONE | OPEN_FLAGS_FAIL_ON_ERROR)

#define APMCAT                  0xc
#define SETPWR                  0x40

/* --------------------------------------------------------------------------
 SHUTDOWNREQPARM   Shutdown flags indicating the state or the return value
                   of the shutdown selection dialog or the 2nd message
                   parameter of the STLRM_SHUTDOWNREQ PM message.
                   These flags are used to execute unattended shutdown
                   from the command line.
                   The index of the volume to be rebooted is stored in the
                   highest 16 bits of the shutdown flags (i.e. the real
                   index is == flagValue >> 16).
*/
#define SDFL_DLG            0x0000      // show the shutdown dialog
#define SDFL_CANCEL         0x0000      // dialog dismissed via Cancel
#define SDFL_LOCKSUSP       0x0001      // lockup and suspend
#define SDFL_SUSP           0x0002      // suspend
#define SDFL_OFF            0x0003      // power off
#define SDFL_SHUTDOWN       0x0004      // ordinary system shutdown
#define SDFL_REBOOT         0x0005      // reboot
#define SDFL_OKMASK         0x000f
#define SDFL_REBOOTIDX      0x0010      // reboot the selected volume
#define SDFL_REBOOTMASK     (SDFL_REBOOT | SDFL_REBOOTMASK)
#define SDFL_EXECUTE        0x1000      // execute the shutdown programs
#define SDFL_NOEXECUTE      0x2000      // do not execute the shutdown programs
#define SDFL_EXEMASK        (SDFL_EXECUTE | SDFL_NOEXECUTE)
#define SDFL_SAVEDATA       0x4000      // save last shutdown selection to
                                        // OS2.INI
#define SDFL_ERROR          0x8000      // create dialog error
#define SDFL_HELPREQUEST    0x8000      // used only by SHUTDOWN.EXE

/* --------------------------------------------------------------------------
 REBOOTLIST        list of bootable volumes stored in eStyler.ini

 The reboot list in estyler.ini v.1 is stored as:
 xxxxx yyyyy<\x00>
 xxxxx yyyyy<\x00>
 <\x00>
 where:
 'xxxxx'   is the volume name
 'yyyyy'   is the volume long name (description) displayed in the shutdown
           dialog
 '<\x00>'  is the NUL character (ASCII value 0)
 Each entry is terminated by a NUL and a double NUL mark the end of the list
 '' and '' are the tags enclosing the volume description.

 Since version 1.1 the format of the reboot list is:
 xxxxx<\t>yyyyy<\x00>
 xxxxx<\t>yyyyy<\x00>
 xxxxx<\t>yyyyy<\x00>
 <\x00>
 The difference is that volume name and descriptions are separated by
 a tab character <\t> and do not use any separation tag.
*/
typedef struct {
   ULONG ci;                // count of bootable volumes
   PSZ aSetBootParm[26];    // address of SETBOOT parameter string
   PSZ aVolDescr[26];       // address of bootable volume descriptions
   BYTE ablist[4];          // bootable volumes data
} REBOOTLIST, * PREBOOTLIST;

/* --------------------------------------------------------------------------
 SHDWNPRGLIST      structure storing the list of programs to be executed on
                   system shutdown.
 The program list in eStyler.ini version 1.0 is stored as:
 Wxxxx ppppZ
 where:
 W          is an optional flag indicating if the program must be run
            minimized '<' or maximized '>'
 xxxx       is the program name
 pppp       are the program parameters
 Z          is a NUL (character with ASCII value 0) character

 A double NUL marks the end of the list.
*/
#pragma pack(2)
typedef struct {
   PSZ pprog;               // current program data string
   USHORT cprog;            // count of listed programs
   USHORT icurprog;         // index of the current program being executed
   HAPP happ;               // handle of the program being executed
   BYTE ab[4];              // variable size buffer contating the program data
} SHDWNPRGLIST, * PSHDWNPRGLIST;

#pragma pack()

/* --------------------------------------------------------------------------
 KILL_LIST         list of programs to be killed on system shutdown.
*/
#ifndef _CLI_SHUTDOWN_H_
   #include "stlrProcKill.h"

   typedef struct {
      CHAR ach[260];
      PQSSDATA p;
      PSZ pexe;
      CHAR achlist[4];
   } KILL_LIST, * PKILL_LIST;

   #define CBFIX_KILLST          (sizeof(KILL_LIST) - 4)

#endif // ifndef _CLI_SHUTDOWN_H_

/* --------------------------------------------------------------------------
 SHUTDOWNSEL       structure used by the shutdown selection dialog
*/
#pragma pack(2)
typedef struct {
   ULONG flMode;                 // selected shutdown mode
   HMODULE hModRes;              // resources module handle
   PREBOOTLIST pRebootList;      // list of re-bootable volumes
   RECTL r;                      // dialog coordinates (used to keep the
                                 // mouse inside the dialog
} DOSHUTDOWN, * PDOSHUTDOWN;
#pragma pack()

/* --------------------------------------------------------------------------
 CLASSNAMES        window class names
*/
// window classes
#define WC_SHDWNSCRN  "stlrShutdownScreen"  // shutdown screen

/* --------------------------------------------------------------------------
 PROTOTYPES        function prototypes
*/
VOID _System shutdownThread(ULONG mode);
VOID doCtrlAltDel(VOID);
VOID doPowerManagement(ULONG fl);

#ifdef ACPI_ENABLED
BOOL tryAcpiPwrOff(VOID);
#endif

PSHDWNPRGLIST shdwnReadProgList(BOOL getList);


#endif // #ifndef _STLR_SHUTDOWN_H_
