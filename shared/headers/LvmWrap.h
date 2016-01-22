//========================================================================
// LvmWrap.h : LVM API wrapper
//
// 04-01-2002 * by Alessandro Felice Cantatore *
//========================================================================

// header inclusions

#ifndef APIEX_LVMWRAPPER_H_
   #define APIEX_LVMWRAPPER_H_

#define LvmOpen(bIgnore, pErr) \
   Open_LVM_Engine((bIgnore), (CARDINAL32*)(pErr))
#define LvmClose()                  Close_LVM_Engine()
#define LvmFreeMem(p)               Free_Engine_Memory(p)
#define LvmDrvList(pErr) \
   Get_Drive_Control_Data((CARDINAL32*)(pErr))
#define LvmDrvInfo(hDrv, pErr) \
   Get_Drive_Status((hDrv), (CARDINAL32*)(pErr))
#define LvmPartList(hDrv, pErr) \
   Get_Partitions((hDrv), (CARDINAL32*)(pErr))
#define LvmVolCtlData(pErr) \
   Get_Volume_Control_Data((CARDINAL32*)(pErr))
#define LvmVolInfo(hVol, pErr) \
   Get_Volume_Information((hVol), (CARDINAL32*)(pErr))
#define LvmMinInstSizeSet(MB)\
   Set_Min_Install_Size((MB) << 11)
#define LvmValidOptions(handle, pErr) \
   Get_Valid_Options((handle), (CARDINAL32*)(pErr))
#define LvmSetInstFlg(handle, pErr) \
   Set_Installable((handle), (CARDINAL32*)(pErr))
#define LvmCommit(pErr) \
   Commit_Changes((CARDINAL32*)(pErr))
#define LvmBootMgrHandle(pErr) \
   Get_Boot_Manager_Handle((CARDINAL32*)(pErr))

typedef Drive_Control_Array            DRV_CTLLIST;
typedef Drive_Information_Record       DRV_INFOREC;
typedef Partition_Information_Array    PART_INFOLIST;
typedef Partition_Information_Record   PART_INFO;
typedef PART_INFO * PPART_INFO;
typedef Volume_Control_Array           VOL_CTLLIST;
typedef Volume_Control_Record          VOL_CTLREC;
typedef Volume_Information_Record      VOL_INFO;
typedef VOL_INFO * PVOL_INFO;


#define CB_FSNAME                      FILESYSTEM_NAME_SIZE


#endif // #ifndef APIEX_LVMWRAPPER_H_
