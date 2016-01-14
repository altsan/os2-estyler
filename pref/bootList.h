/* --------------------------------------------------------------------------
 bootlist.h : structures, definitions and macros to interface with
              AiRBOOT and the LVM engine to retrieve the list of the
              bootable volumes.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */



#ifndef _BOOTLIST_H_
   #define _BOOTLIST_H_

   #include "lvm_intr.h"
   #include "LvmWrap.h"

   // airboot signature at offset 2 of sector 0
   #define GBL_AIRBOOTSIGNATURE        "AiRBOOT"
   // airboot configuration signature at offset 0 of sector 54
   #define GBL_AIRBOOTCFGSIGNATURE     "AiRCFG-TABLE­"
   // bootable volume flag
   #define GLB_AIRBOOTBOOTABLEFL             0x01
   // valid airboot version
   #define GBL_AIRBOOTVALIDVER         0x00000102

   // --- AiRBOOT handling code : error messages --------------------
   // failed to get the disk handle
   #define GBL_ERRDISKHANDLE           0x00010000
   // failed to read disk sector 0
   #define GBL_ERRREADSECTOR1          0x00020000
   // unsupported airboot version
   #define GBL_ERRAIRBOOTVER           0x00030000
   // failed to read airboot configuration (sector 54)
   #define GBL_ERRREADSECTOR2          0x00040000
   // invalid airboot configuration signature
   #define GBL_ERRINVALIDDATA          0x00050000
   // failed to read airboot volume records (sectors 55-56)
   #define GBL_ERRREADSECTOR3          0x00060000

   // --- LVM handling code : error messages ------------------------
   // failed to open the LVM engine
   #define GBL_ERROPENLVM              0x00070000
   // Get_Drive_Control_Data failed
   #define GBL_ERRLVMDRVLIST           0x00080000
   // Get_Partitions failed
   #define GBL_ERRPARTLIST             0x00090000
   // Get_Volume_Information failed
   #define GBL_ERRVOLINFO              0x000a0000

   // --- other return codes ----------------------------------------
   // airboot not installed
   #define GBL_AIRBOOTNOTFOUND         0xffff0000
   // success
   #define GBL_NOERROR                 0x00000000

#pragma pack(1)
// main Airboot data structure (up to the relevant data)
typedef struct {
   USHORT reserved;
   CHAR signature[7];         // AiRBOOT
   BYTE day;
   BYTE month;
   USHORT year;
   BYTE verMaj;               // version major
   BYTE verMin;               // version minor
} AIRBOOTMAIN, * PAIRBOOTMAIN;

// Airboot configuration data structure (up to the relevant data)
typedef struct {
   CHAR signature[13];        // AiRCFG-TABLE­
   BYTE verMaj;
   BYTE verMin;
   BYTE lang;
   ULONG counter;
   USHORT checksum;
   BYTE cVolumes;             // count of volumes
} AIRBOOTCFG, * PAIRBOOTCFG;


// AIRBOOT drive geometry description
typedef struct {
   BYTE cyl;
   BYTE head;
   BYTE sect;
} AIRBOOTDRIVEGEOM;

// airboot volume record
typedef struct {
   ULONG sn;                     // serial number / LVM partition id (0 based)
   CHAR name[11];                // partition name
   BYTE drive;                   // drive or partition (?)
   BYTE id;                      // partition type
   BYTE flag;                    // status flags
                                 // BOOTABLE          0x0001
                                 // antivirus         0x0002
                                 // hide feature      0x0004
                                 // logical drv let.  0x0008
                                 // MS partition hack 0x0010
   USHORT checksum;              // boot record checksum
   AIRBOOTDRIVEGEOM dgBootRec;
   AIRBOOTDRIVEGEOM dgPartTbl;
   ULONG startBootRec;
   ULONG startPartTbl;
} AIRBOOTVOLREC, * PAIRBOOTVOLREC;

#pragma pack()

#endif // #ifndef _BOOTLIST_H_
