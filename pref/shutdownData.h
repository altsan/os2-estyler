/* --------------------------------------------------------------------------
 shutdownData.h : shutdown settings specific data structures and definitions

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------
 SHUTDOWNDATA      bootable volume/run program/kill program lists for UNDO.
-------------------------------------------------------------------------- */



#ifndef _SHUTDWONDATA_H_
   #define _SHUTDWONDATA_H_

/* --------------------------------------------------------------------------
 SHUTDOWNDATA      bootable volume/run program/kill program lists for UNDO.
                   These are read on program initialization.
*/
typedef struct {
   HWND hwndReboot;           // handle of the reboot page window
   PSZ pBootVols;             // list of rebootable volumes
   ULONG cbBootVols;          // size in bytes of the rebootable volumes list
   PSZ pAppliedBootVols;      // last applied list of bootable volumes
   ULONG cbAppliedBootVols;   // size of the last applied list of bootable volumes
   PSZ pRunProgs;             // list of programs to be executed on shutdown
   ULONG cbRunProgs;          // size in bytes of the program list
   PSZ pAppliedRunProgs;      // list of programs to be executed on shutdown
   ULONG cbAppliedRunProgs;   // size in bytes of the program list
   PSZ pKillProgs;            // list of programs to be killed on shutdown
   ULONG cbKillProgs;         // size in bytes of the kill list
   PSZ pAppliedKillProgs;     // list of programs to be killed on shutdown
   ULONG cbAppliedKillProgs;  // size in bytes of the kill list
} SHUTDOWNDATA, * PSHUTDOWNDATA;


#endif // #ifndef _SHUTDWONDATA_H_
