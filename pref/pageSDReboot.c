/* --------------------------------------------------------------------------
 pageSDReboot.c : shutdown - reboot volumes list - settings dialog procedure.

 2004-6-4 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"
#include "bootList.h"

// definitions --------------------------------------------------------------
#define CHECK_APPLYSTATE     1
#define CHECK_UNDOSTATE      2

// prototypes ---------------------------------------------------------------
static BOOL onDlgInit(HWND hwnd);
static VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl);
static VOID onCmdMsg(HWND hwnd, ULONG id);
static VOID onWorkerJobEnd(HWND hwnd, ULONG jobId, ULONG rc);
static VOID fillBootableVolumesList(HWND hwnd);
static VOID handleEntryFieldContentChange(HWND hwnd);
static VOID checkOptionsChanged(HWND hwnd, ULONG fl);
static VOID applyOptions(HWND hwnd);
static VOID undoOptions(HWND hwnd);
static VOID addItem(HWND hwnd);
static VOID delItem(HWND hwnd);
static VOID editItem(HWND hwnd);
static PSZ getListContent(HWND hwnd, PULONG pCb);
static BOOL getBootList(HWND hwnd);
static MRESULT EXPENTRY waitDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static ULONG getBootListJob(PGETBOOTLIST pgbl);
static VOID freeBootListData(PGETBOOTLIST pgbl);
static ULONG checkAirBoot(PGETBOOTLIST pgbl);
static ULONG getVolList(HFILE hDrive, PAIRBOOTVOLREC pVolRec,
                 PGETBOOTLIST pgbl, ULONG cVols);
static ULONG readDiskSector(HFILE hDrive, ULONG sector, PBYTE buf);
static BOOL isValidAirBootVersion(PAIRBOOTMAIN pabm);
static ULONG checkLVM(PGETBOOTLIST pgbl);
static VOID addFoundVolumes(HWND hwnd, PGETBOOTLIST pgbl);

// global variables ---------------------------------------------------------
static BOOL g_edit = FALSE;

/* --------------------------------------------------------------------------
 Shutdown - reboot volumes list - settings dialog procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY sdRebootPageProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         onDlgInit(hwnd);
         break;
      case WM_CONTROL:
         onCtrlMsg(hwnd,  SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), (HWND)mp2);
         break;
      case WM_COMMAND:
         onCmdMsg(hwnd, (ULONG)mp1);
         break;
      case STLRM_WORKERTHREADJOBEND:
         onWorkerJobEnd(hwnd, (ULONG)mp1, (ULONG)mp2);
         break;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}

/* --------------------------------------------------------------------------
 initialization.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL onDlgInit(HWND hwnd) {
   g.pShdwnData->hwndReboot = hwnd;
   initPage(hwnd);
   fillBootableVolumesList(hwnd);
   return TRUE;
}


/* --------------------------------------------------------------------------
 Process notification messages.
- Parameters -------------------------------------------------------------
 HWND hwnd : window handle
 ULONG id       : control ID
 ULONG event : notify code
 HWND hCtrl     : control handle
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl) {
   switch (id) {
      case LBX_SDWNREBOOTLIST:
         if (event == LN_SELECT)
            handleItemStateChange(hwnd, hCtrl, NULL, 0,
                                  BTN_SDWNREBOOTREM, BTN_SDWNREBOOTUP);
         break;
      case EF_SDWNVOLNAME:
      case EF_SDWNVOLDESCR:
         if (event == EN_CHANGE)
            handleEntryFieldContentChange(hwnd);
         break;
   } /* endswitch */
}


/* --------------------------------------------------------------------------
 Process command events.
- Parameters -------------------------------------------------------------
 HWND hwnd : window handle
 ULONG id  : button ID.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID onCmdMsg(HWND hwnd, ULONG id) {
   BOOL bSettingsChanged = FALSE;
   switch (id) {
      case BTN_APPLY:
         applyOptions(hwnd);
         break;
      case BTN_UNDO:
         undoOptions(hwnd);
         break;
      case BTN_SDWNREBOOTUP:
         moveItem(hwnd, LBX_SDWNREBOOTLIST, -1);
         bSettingsChanged = TRUE;
         break;
      case BTN_SDWNREBOOTDOWN:
         moveItem(hwnd, LBX_SDWNREBOOTLIST, 1);
         bSettingsChanged = TRUE;
         break;
      case BTN_SDWNREBOOTADD:
         addItem(hwnd);
         bSettingsChanged = TRUE;
         break;
      case BTN_SDWNREBOOTREM:
         delItem(hwnd);
         bSettingsChanged = TRUE;
         break;
      case BTN_SDWNREBOOTEDIT:
         editItem(hwnd);
         break;
      case BTN_SDWNREBOOTFIND:
         getBootList(hwnd);
         break;
   } /* endswitch */
   if (bSettingsChanged)
      checkOptionsChanged(hwnd, CHECK_APPLYSTATE | CHECK_UNDOSTATE);
}


/* --------------------------------------------------------------------------
 Handle termination of a worker thread job.
- Parameters -------------------------------------------------------------
 HWND hwnd   : dialog window handle.
 ULONG jobId : job identifier.
 ULONG rc    : return code.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID onWorkerJobEnd(HWND hwnd, ULONG jobId, ULONG rc) {
   if (jobId == STLRWID_GETBOOTLIST) {
      if (rc == GBL_NOERROR) {
         addFoundVolumes(hwnd, (PGETBOOTLIST)g.worker.pCurJob->pData);
      } else {
         errorBox(ISERR_GETBOTTLIST + (rc >> 16), rc & 0xffff);
      } /* endif */
   } /* endif */
   WinEnableWindow(g.appl.hwnd, TRUE);
   WinDestroyWindow(((PGETBOOTLIST)g.worker.pCurJob->pData)->hwndWaitMsg);
   workerJobUnlink();
}


/* --------------------------------------------------------------------------
 Fill the list of the bootable volumes according to the previous settings.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID fillBootableVolumesList(HWND hwnd) {
   PSZ p;
   CHAR buf[64];
   PSZ pDescr;
   ULONG cbDescr;
   hwnd = DlgItemHwnd(hwnd, LBX_SDWNREBOOTLIST);
   if (g.pShdwnData->pBootVols) {
      for (p = g.pShdwnData->pBootVols; *p; p += strlen(p) + 1) {
         if (NULL != (pDescr = strchr(p, '\t'))) {
            *pDescr = '\x00';
            cbDescr = strlen(pDescr + 1);
            sprintf(buf, "%s (%s)", pDescr + 1, p);
            *pDescr = '\t';
            wLbxItemAndHndIns(hwnd, LIT_END, buf, cbDescr);
         } /* endif */
      } /* endfor */
   } /* endif */
}


/* --------------------------------------------------------------------------
 On entryfield content change, update the enable state of the 'Add' button.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID handleEntryFieldContentChange(HWND hwnd) {
   HWND hAddBtn = DlgItemHwnd(hwnd, BTN_SDWNREBOOTADD);
   BOOL bTextPresent = WinQueryDlgItemTextLength(hwnd, EF_SDWNVOLNAME) &&
                       WinQueryDlgItemTextLength(hwnd, EF_SDWNVOLDESCR);
   WinEnableWindow(hAddBtn, bTextPresent);
   WinSendMsg(hAddBtn, BM_SETDEFAULT, (MPARAM)bTextPresent, MPVOID);
}


/* --------------------------------------------------------------------------
 Compare the options set in the dialog with the active options.
 If there is any different option enable the 'Apply' and or the 'Undo' button.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
 ULONG fl  : options to check.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID checkOptionsChanged(HWND hwnd, ULONG fl) {
   PSZ pList;
   ULONG cb;
   BOOL bEnable;
   cb = 1;
   pList = getListContent(hwnd, &cb);
   if (pList || !cb) {
      if (fl & CHECK_APPLYSTATE) {
         bEnable = (cb != g.pShdwnData->cbAppliedBootVols)
                   ||
                   memcmp(pList, g.pShdwnData->pAppliedBootVols, cb);
         if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_APPLY))
            resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED,
                              (bEnable ? PGFL_APPLY_ENABLED : 0));
      } /* endif */
      if (fl & CHECK_UNDOSTATE) {
         bEnable = (cb != g.pShdwnData->cbBootVols)
                   ||
                   memcmp(pList, g.pShdwnData->pBootVols, cb);
         if (bEnable != DlgItemIsEnabled(g.appl.hwnd, BTN_UNDO))
            resetCommonButton(BTN_UNDO, PGFL_UNDO_ENABLED,
                              (bEnable ? PGFL_UNDO_ENABLED : 0));
      } /* endif */
   } /* endif */
   free(pList);
}


/* --------------------------------------------------------------------------
 Save the bootable volumes list in eStyler.ini.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID applyOptions(HWND hwnd) {
   HINI hini;
   // free the previously applied list (if any)
   if (g.pShdwnData->pAppliedBootVols)
      free(g.pShdwnData->pAppliedBootVols);
   g.pShdwnData->cbAppliedBootVols = 1;
   g.pShdwnData->pAppliedBootVols = getListContent(hwnd,
                                              &g.pShdwnData->cbAppliedBootVols);
   // if the listbox content has been succesfully read or is empty
   if (g.pShdwnData->pAppliedBootVols || !g.pShdwnData->cbAppliedBootVols) {
      if (NULLHANDLE != (hini = stlrOpenProfile())) {
         if (setProfileData(hini, SZPRO_SHUTDWON, SZPRO_SDWNLIST,
                            g.pShdwnData->pAppliedBootVols,
                            g.pShdwnData->cbAppliedBootVols)) {
            resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED, 0);
         } else {
            errorBox(ISERR_SAVEDATA);
         } /* endif */
         PrfCloseProfile(hini);
      } else {
         errorBox(ISERR_OPENPRF);
      } /* endif */
   } /* endif */
}


/* --------------------------------------------------------------------------
 Reset the current settings to the values they had when the preferences
 notebook was opened.
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID undoOptions(HWND hwnd) {
   HWND hwndLbox;
   hwndLbox = DlgItemHwnd(hwnd, LBX_SDWNREBOOTLIST);
   // empty the listbox displaying the list of the bootable volumes
   wLbxEmpty(hwndLbox);
   // fills it with the data retrieved on program initialization
   fillBootableVolumesList(hwnd);
   // reset the state of the Apply and Undo buttons
   resetCommonButton(BTN_UNDO, PGFL_UNDO_ENABLED, 0);
   checkOptionsChanged(hwnd, CHECK_APPLYSTATE);
   handleItemStateChange(hwnd, hwndLbox, NULL, 0,
                         BTN_SDWNREBOOTREM, BTN_SDWNREBOOTUP);
}


/* --------------------------------------------------------------------------
 Add a bootable volume to the list.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID addItem(HWND hwnd) {
   CHAR buf[64];
   ULONG cb, cbDescr;
   HWND hwndLbox;
   cbDescr = WinQueryDlgItemText(hwnd, EF_SDWNVOLDESCR, 33, buf);
   memcpy(buf + cbDescr, " (", 2);
   cb = cbDescr + 2;
   cb += WinQueryDlgItemText(hwnd, EF_SDWNVOLNAME, 21, buf + cb);
   memcpy(buf + cb, ")", 2);
   hwndLbox = DlgItemHwnd(hwnd, LBX_SDWNREBOOTLIST);
   // item modification mode
   if (g_edit) {
      handleItemStateChange(hwnd, hwndLbox, buf, cbDescr,
                            BTN_SDWNREBOOTREM, BTN_SDWNREBOOTUP);
      DlgItemEnable(hwnd, BTN_SDWNREBOOTFIND, TRUE);
      g_edit = FALSE;
   // a new item is being inserted
   } else {
      wLbxItemAndHndIns(hwndLbox, LIT_END, buf, cbDescr);
      handleItemStateChange(hwnd, hwndLbox, NULL, 0,
                            BTN_SDWNREBOOTREM, BTN_SDWNREBOOTUP);
   } /* endif */
   WinSetDlgItemText(hwnd, EF_SDWNVOLNAME, "");
   WinSetDlgItemText(hwnd, EF_SDWNVOLDESCR, "");
   WinSetFocus(HWND_DESKTOP, DlgItemHwnd(hwnd, EF_SDWNVOLNAME));
}


/* --------------------------------------------------------------------------
 Remove a bootable volume to the list.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID delItem(HWND hwnd) {
   HWND hwndLbox;
   INT iItem;
   hwndLbox = DlgItemHwnd(hwnd, LBX_SDWNREBOOTLIST);
   iItem = wLbxItemSelected(hwndLbox);
   WinDeleteLboxItem(hwndLbox, iItem);
   dlgItemMultiEnable(hwnd, BTN_SDWNREBOOTREM, BTN_SDWNREBOOTEDIT, FALSE);
   dlgItemMultiEnable(hwnd, BTN_SDWNREBOOTUP, BTN_SDWNREBOOTDOWN, FALSE);
}


/* --------------------------------------------------------------------------
 Modify the selected item.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID editItem(HWND hwnd) {
   HWND hwndLbox;
   CHAR buf[64];
   PSZ p;
   INT iItem;
   ULONG cb, cbDescr;
   hwndLbox = DlgItemHwnd(hwnd, LBX_SDWNREBOOTLIST);
   if (0 <= (iItem = wLbxItemSelected(hwndLbox))) {
      g_edit = TRUE;
      WinEnableWindow(hwndLbox, FALSE);
      cb = wLbxItemText(hwndLbox, iItem, sizeof(buf), buf);
      cbDescr = wLbxItemHnd(hwndLbox, iItem);
      buf[cbDescr] =  buf[cb - 1] = '\x00';
      WinSetDlgItemText(hwnd, EF_SDWNVOLNAME, buf + cbDescr + 2);
      WinSetDlgItemText(hwnd, EF_SDWNVOLDESCR, buf);
      dlgItemMultiEnable(hwnd, BTN_SDWNREBOOTREM, BTN_SDWNREBOOTFIND, FALSE);
      dlgItemMultiEnable(hwnd, BTN_SDWNREBOOTUP, BTN_SDWNREBOOTDOWN, FALSE);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Translate the listbox content to the data format used by eStyler.ini.
- Parameters -------------------------------------------------------------
 HWND hwnd  : dialog window handle.
 PULONG pCb : formatted data size.
- Return value -----------------------------------------------------------
 PSZ : address of the formatted data.
-------------------------------------------------------------------------- */
static PSZ getListContent(HWND hwnd, PULONG pCb) {
   INT cItems, cb, i, cbDescr, cbVolName;
   PSZ pList, p;
   CHAR buf[64];
   hwnd = DlgItemHwnd(hwnd, LBX_SDWNREBOOTLIST);
   if (!(cItems = wLbxItemCount(hwnd))) {
      *pCb = 0;
      return NULL;
   } /* endif */
   cb = 0x10000;
   if (NULL == (pList = malloc(cb)))
      return handleError(ISERR_ALLOCATION, NULL);
   for (p = pList, i = 0; i < cItems; ++i) {
      cbDescr = wLbxItemHnd(hwnd, i);
      cbVolName = wLbxItemText(hwnd, i, sizeof(buf), buf) - cbDescr - 3;
      memcpy(p, buf + cbDescr + 2, cbVolName);
      p[cbVolName] = '\t';
      buf[cbDescr] = '\x00';
      memcpy(p + cbVolName + 1, buf, cbDescr + 1);
      p += cbVolName + cbDescr + 2;
   } /* endfor */
   *p = 0;
   *pCb = (ULONG)(p - pList) + 1;
   return pList;
}

/* --------------------------------------------------------------------------
 Get a list of bootable volumes via the worker thread.
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL getBootList(HWND hwnd) {
   HWND hwndWaitMsg;
   PGETBOOTLIST pgbl;
   if (NULL == (pgbl = malloc(sizeof(GETBOOTLIST))))
      return handleError(ISERR_ALLOCATION, FALSE);
   WinEnableWindow(g.appl.hwnd, FALSE);
   memset(pgbl, 0, sizeof(GETBOOTLIST));
   pgbl->hwndWaitMsg = WinLoadDlg(g.appl.hwnd, hwnd, waitDlgProc, g.hResLib,
                                  DLG_SDGETBOOLLIST, NULL);
   if (!pgbl->hwndWaitMsg
       ||
       !workerJobAdd(hwnd, STLRWID_GETBOOTLIST,
                     (PTHREADJOBFN)getBootListJob,
                     (PDELJOBFN)freeBootListData,
                     pgbl)
      ) {
      if (pgbl->hwndWaitMsg) WinDestroyWindow(pgbl->hwndWaitMsg);
      free(pgbl);
      WinEnableWindow(g.appl.hwnd, TRUE);
      return handleError(ISERR_ALLOCATION, FALSE);
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 Procedure of the wait-while-searching-bootable-volumes dialog
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
static
MRESULT EXPENTRY waitDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if (msg == WM_INITDLG) {
      RECTL rMain, rDlg;
      WinQueryWindowRect(g.appl.hwnd, &rMain);
      WinQueryWindowRect(hwnd, &rDlg);
      WinSetWindowPos(hwnd, HWND_TOP,
                      (rMain.xRight - rDlg.xRight) / 2,
                      (rMain.yTop - rDlg.yTop) / 2,
                      0, 0, SWP_MOVE | SWP_ZORDER);
      return MRFALSE;
   } /* endif */
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


/* --------------------------------------------------------------------------
 Check if the boot manager is AiRBOOT and get the list of the bootable volumes
 otherwise get the list of bootable volumes via the LVM interface.
- Parameters -------------------------------------------------------------
 PGETBOOTLIST pgbl : structure used to get the list of bootable volumes.
- Return value -----------------------------------------------------------
 ULONG : 0 (success) or error code.
-------------------------------------------------------------------------- */
static ULONG getBootListJob(PGETBOOTLIST pgbl) {
   ULONG rc;
   if (GBL_AIRBOOTNOTFOUND == (rc = checkAirBoot(pgbl)))
      return checkLVM(pgbl);
   return rc;
}


/* --------------------------------------------------------------------------
 Free the structure holding the list of the bootable volumes.
- Parameters -------------------------------------------------------------
 PGETBOOTLIST pgbl : structure used to get the list of bootable volumes.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID freeBootListData(PGETBOOTLIST pgbl) {
   free(pgbl);
}

/* --------------------------------------------------------------------------
 Check if AiRBOOT is the current boot manager and get the list of the
 bootable volumes.
- Parameters -------------------------------------------------------------
 PGETBOOTLIST pgbl : structure used to get the list of bootable volumes.
- Return value -----------------------------------------------------------
 ULONG : return code (0 = success).
-------------------------------------------------------------------------- */
static ULONG checkAirBoot(PGETBOOTLIST pgbl) {
   BYTE abSector[1024];
   APIRET rc;
   HFILE hDrive;
   ULONG cVols;
   PAIRBOOTVOLREC pVolRec;
   rc = DosPhysicalDisk(INFO_GETIOCTLHANDLE, &hDrive, 2, "1:", 3);
   if (NO_ERROR != rc) return GBL_ERRDISKHANDLE | rc;
   // read the first track
   if (NO_ERROR != (rc = readDiskSector(hDrive, 0, abSector))) {
      rc |= GBL_ERRREADSECTOR1;
   // is the AiRBOOT signature at offset 2 ?
   } else if (memcmp(GBL_AIRBOOTSIGNATURE, abSector + 2, 7)) {
      rc = GBL_AIRBOOTNOTFOUND;
   // check the airboot version (>= 1.02)
   } else if (!isValidAirBootVersion((PAIRBOOTMAIN)abSector)) {
      rc = GBL_ERRAIRBOOTVER;
   // read the sector containing the current AIRBOOT configuration
   // to get the count of the volumes
   } else if (NO_ERROR != (rc = readDiskSector(hDrive, 54, abSector))) {
      rc |= GBL_ERRREADSECTOR2;
   // check the airboot configuration signature
   } else if (memcmp(abSector, GBL_AIRBOOTCFGSIGNATURE, 13)) {
      rc = GBL_ERRINVALIDDATA;
   // get the count of the volumes detected by airboot
   } else {
      rc = getVolList(hDrive, (PAIRBOOTVOLREC)abSector, pgbl,
                      ((PAIRBOOTCFG)abSector)->cVolumes);
   } /* endif */
   DosPhysicalDisk(INFO_FREEIOCTLHANDLE, NULL, 0, &hDrive, 2);
   return rc;
}


/* --------------------------------------------------------------------------
 Get the volume list from the AiRBOOT volume records array.
- Parameters -------------------------------------------------------------
 HFILE hDrive           : handle of disk 1.
 PAIRBOOTVOLREC pVolRec : buffer for reading sectors 55 and 56.
 PGETBOOTLIST pgbl      : buffer to store the list of bootable volumes.
 ULONG cVols            : count of volumes.
- Return value -----------------------------------------------------------
 ULONG return code (0 = success).
-------------------------------------------------------------------------- */
static
ULONG getVolList(HFILE hDrive, PAIRBOOTVOLREC pVolRec,
                 PGETBOOTLIST pgbl, ULONG cVols) {
   ULONG rc;
   INT i, j, k;
   PSZ p;
   // read the airboot volume records
   if ((NO_ERROR != (rc = readDiskSector(hDrive, 55, (PBYTE)pVolRec)))
       ||
       (NO_ERROR != (rc = readDiskSector(hDrive, 56, ((PBYTE)pVolRec) + 512))))
      return rc | GBL_ERRREADSECTOR3;
   for (i = j = 0; i < cVols; ++i) {
      if (pVolRec[i].flag & GLB_AIRBOOTBOOTABLEFL) {
         memcpy(pgbl->achVol[j], pVolRec[i].name, 11);
         // strip trailing spaces
         for (k = strlen(pgbl->achVol[j]) - 1; k >= 0; k--) {
            if (pgbl->achVol[j][k] != ' ') {
               if (k < 10) pgbl->achVol[j][k + 1] = 0;
               break;
            } /* endif */
         } /* endfor */
         j++;
      } /* endif */
   } /* endif */
   pgbl->cVols = j;
   return (ULONG)NO_ERROR;
}


/* --------------------------------------------------------------------------
 Read the content of a disk sector.
- Parameters -------------------------------------------------------------
 HFILE hDrive  : disk drive handle.
 ULONG sector  : sector to read.
 BYTE buf      : buffer where to write the content of the sector.
- Return value -----------------------------------------------------------
 ULONG : return code (0 = success).
-------------------------------------------------------------------------- */
static ULONG readDiskSector(HFILE hDrive, ULONG sector, PBYTE buf) {
   ULONG cyl, head, sect, ulParm, cbParm, cbData;
   TRACKLAYOUT tl;
   cbParm = sizeof(TRACKLAYOUT);
   cbData = 512;
   tl.bCommand = 0;
   tl.usHead = (sector / 63) % 255;
   tl.usCylinder = sector / (255 * 63);
   tl.usFirstSector = 0;
   tl.cSectors = 1;
   tl.TrackTable[0].usSectorNumber = sector % 63 + 1;
   tl.TrackTable[0].usSectorSize = 512;
   return (ULONG) DosDevIOCtl(hDrive, IOCTL_PHYSICALDISK, PDSK_READPHYSTRACK,
                              &tl, cbParm, &cbParm, buf, cbData, &cbData);
}


/* --------------------------------------------------------------------------
 Return TRUE if a valid AiRBOOT version is installed.
- Parameters -------------------------------------------------------------
 PAIRBOOTMAIN pabm : relevant data of the first disk sector.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (valid/invalid version).
-------------------------------------------------------------------------- */
static BOOL isValidAirBootVersion(PAIRBOOTMAIN pabm) {
   return ((pabm->verMaj << 8) + pabm->verMin) >= GBL_AIRBOOTVALIDVER;
}


/* --------------------------------------------------------------------------
 Get the list of bootable volumes recognized by an LVM aware boot manager.
- Parameters -------------------------------------------------------------
 PGETBOOTLIST pgbl : structure used to get the list of bootable volumes.
- Return value -----------------------------------------------------------
 ULONG return code (0 = success).
-------------------------------------------------------------------------- */
static ULONG checkLVM(PGETBOOTLIST pgbl) {
   INT i, j, k;                  // loop indexes
   ULONG rc;                     // error code
   DRV_CTLLIST drvList;          // drive list data
   PART_INFOLIST partList;       // partition list data
   VOL_INFO volInfo;             // single volume informations
   // initialize the LVM engine
   LvmOpen(FALSE, &rc);
   if (rc) {
      return rc | GBL_ERROPENLVM;
   } else {
      // get a list of the available drives
      drvList = LvmDrvList(&rc);
      if (rc) {
         rc |= GBL_ERRLVMDRVLIST;
      } else {
         // loop through the drive list
         for (i = 0, k = 0; i < drvList.Count; ++i) {
            // loops through the partitions of the current drive
            partList = LvmPartList(drvList.Drive_Control_Data[i].Drive_Handle,
                                   &rc);
            if (rc) {
               rc |= GBL_ERRPARTLIST;
            } else {
               for (j = 0; j < partList.Count; ++j) {
                  // if listed on the boot manager
                  if (partList.Partition_Array[j].On_Boot_Manager_Menu) {
                     memcpy(pgbl->achVol[k],
                            partList.Partition_Array[j].Partition_Name,
                            20);
                     k++;
                  // LVM compatible bootable
                  } else if (partList.Partition_Array[j].Volume_Handle) {
                     volInfo = LvmVolInfo(partList.Partition_Array[j].Volume_Handle,
                                          &rc);
                     if (rc) {
                        rc |= GBL_ERRVOLINFO;
                        break;
                     } else if (volInfo.Bootable) {
                        memcpy(pgbl->achVol[k],
                               partList.Partition_Array[j].Partition_Name,
                               20);
                        k++;
                     } /* endif */
                  } /* endif */
               } /* endfor */
               LvmFreeMem(partList.Partition_Array);
            } /* endif */
         } /* endfor */
         LvmFreeMem(drvList.Drive_Control_Data);
         pgbl->cVols = k;
      } /* endif */
   } /* endif */
   LvmClose();
   return rc;
}


/* --------------------------------------------------------------------------
 Check one by one the bootable volumes and if not yet in the list adds
 them using the volume name for description.
- Parameters -------------------------------------------------------------
 HWND hwnd         : dialog window handle.
 PGETBOOTLIST pgbl : find-bootable-volumes data structure.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID addFoundVolumes(HWND hwnd, PGETBOOTLIST pgbl) {
   INT i;
   CHAR buf[56];
   HWND hLbox = DlgItemHwnd(hwnd, LBX_SDWNREBOOTLIST);
   for (i = 0; i < pgbl->cVols; ++i) {
      if (pgbl->achVol[i][0]) {
         sprintf(buf, "(%s)", pgbl->achVol[i]);
         if (LIT_NONE
             ==
             wLbxItemTextSearch(hLbox,
                                LSS_CASESENSITIVE | LSS_SUBSTRING,
                                LIT_FIRST,
                                buf)
            ) {
            sprintf(buf, "%s (%s)", pgbl->achVol[i], pgbl->achVol[i]);
            wLbxItemAndHndIns(hLbox, LIT_END, buf, strlen(pgbl->achVol[i]));
         } /* endif */
      } /* endif */
   } /* endfor */
   if (pgbl->cVols)
      checkOptionsChanged(hwnd, CHECK_APPLYSTATE | CHECK_UNDOSTATE);
}
