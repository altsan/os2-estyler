/* --------------------------------------------------------------------------
 bitmap.c : bitmap related functions.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------


// prototypes ---------------------------------------------------------------
static ULONG readBmpFile(PSZ pszFile, PBITMAPFILEHEADER pBmp, PULONG pCbBmp);
static BOOL saveBmpData(PAPPLYBMP pa);
static BOOL isValidBmpFile(PSZ pszFileName, PBMPLIMITS pbl);
static ULONG calcBmpDataSize(ULONG cx, ULONG cy, ULONG cClrs);
static BOOL addImageJob(PADDBMPFILE pabf);
static VOID freeAddImageRes(PADDBMPFILE pabf);

// global variables ---------------------------------------------------------


/* --------------------------------------------------------------------------
 Job executed by the worker thread:
 get a bitmap handle, bitmap size and optionally bitmap full data from a
 file or from the resources.
- Parameters -------------------------------------------------------------
 PGETBMPDATA pgb : structure used to store the bitmap data.
- Return value -----------------------------------------------------------
 ULONG : 0 (success) or error code.
-------------------------------------------------------------------------- */
ULONG getBitmapDataJob(PGETBMPDATA pgb) {
   ULONG rc;
   HPS hps;
   PBMPDAT pbd;
   PBYTE pData;
   pbd = pgb->cbData ? (PBMPDAT)pgb->ab: NULL;
   // if a bitmap file get the data from the file
   if (*pgb->achFile) {
      if (NULL == (pData = (PBYTE)gpReadFile(pgb->achFile)))
         return GETBMP_ERR_READFILE;
   } else {
      pData = NULL;
   } /* endif */
   if (NULLHANDLE == (hps = WinGetPS(g.appl.hwnd))) {
      return GETBMP_ERR_HPS;
//      goto exit_0;
   } /* endif */
   pgb->bmpp.hbmp = stlrHBmp(hps, pData, &pgb->bmpp, pgb->pBmp->idRes, pbd);
   rc = (NULLHANDLE == pgb->bmpp.hbmp) ? GETBMP_ERR_BMPHANDLE : GETBMP_SUCCESS;
   WinReleasePS(hps);
exit_0:
    if (pData)
      gpFreeReadFile(pData);
   return rc;
}


/* --------------------------------------------------------------------------
 Free the resources allocated to get the handle of a new bitmap via the
 worker thread.
- Parameters -------------------------------------------------------------
 PGETBMPDATA pgb : structure used to store the bitmap data.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID freeGetBitmapDataRes(PGETBMPDATA pgb) {
   free(pgb);
}


/* --------------------------------------------------------------------------
 Display a proper error message when the get-bitmap-data job fails.
- Parameters -------------------------------------------------------------
 ULONG rc        : error code.
 PGETBMPDATA pgb : structure used to store the bitmap data.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID handleGetBitmapJobError(ULONG rc, PGETBMPDATA pgb) {
   switch (rc) {
      case GETBMP_ERR_HPS:
         errorBox(ISERR_GETHPSFORHBMP);
         break;
      case GETBMP_ERR_READFILE:
         errorBox(ISERR_READFILE, pgb->achFile);
         break;
      case GETBMP_ERR_BMPHANDLE:
         errorBox(ISERR_GETBMPHANDLE);
         break;
   } /* endswitch */
}

/* --------------------------------------------------------------------------
 Start the apply-titlebar worker thread job.
- Parameters -------------------------------------------------------------
 HWND hwnd     : dialog window handle.
 PSZ pszPath   : bitmap path (relative to the executable path).
 PSZ pszFile   : name of the bitmap file.
 PAPPLYBMP pa : data needed for applying the new titlebar options.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
BOOL applyBitmap(HWND hwnd, PSZ pszPath, PSZ pszFile, PAPPLYBMP pa) {
   PAPPLYBMP pApplyBmp;
   ULONG cb;
   // if a non-default bitmap is being set allocate storage for the bitmap data
   cb = sizeof(APPLYBMP);
   if (*pszFile) cb += 0x10000;
   if (NULL == (pApplyBmp = malloc(cb)))
      return handleError(ISERR_ALLOCATION, FALSE);
   memcpy(pApplyBmp, pa, sizeof(APPLYBMP));
   if (*pszFile) {
      makeFullPathName(pApplyBmp->achBmpFile, pszPath);
      strcat(pApplyBmp->achBmpFile, pszFile);
   } else {
      *pApplyBmp->achBmpFile = '\x00';
   } /* endif */
   return workerJobAdd(hwnd, STLRWID_APPLYBMP,
                       (PTHREADJOBFN)applyBitmapJob,
                       (PDELJOBFN)freeApplyBitmapRes, pApplyBmp);
}


/* --------------------------------------------------------------------------
 Job executed by the worker thread:
 - the bitmap file is read, if it is a bitmap array the first bitmap is
   read and is converted to the format 1.x.
 - write to estyler.ini the name of the bitmap file and the bitmap data
   (converted to the 1.x format).
- Parameters -------------------------------------------------------------
 PAPPLYBMP pa : worker job related data.
- Return value -----------------------------------------------------------
 ULONG : 0 (success), 1 (cannot read the file),
-------------------------------------------------------------------------- */
ULONG applyBitmapJob(PAPPLYBMP pa) {
   ULONG rc;
   // if a non-default bitmap is set read the bitmap file
   if (*pa->achBmpFile
       &&
       (APPLYBMP_SUCCESS
          !=
          (rc = readBmpFile(pa->achBmpFile,
                            (PBITMAPFILEHEADER)pa->aBmpData, &pa->cbData))))
      return rc;
   // save the bitmap name and data in OS2.INI
   if (!saveBmpData(pa)) return APPLYBMP_ERR_SAVEDATA;
   return APPLYBMP_SUCCESS;
}


/* --------------------------------------------------------------------------
 Free the resources allocated to apply a new bitmap.
- Parameters -------------------------------------------------------------
 PAPPLYBMP pa : worker job related data.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID freeApplyBitmapRes(PAPPLYBMP pa) {
   free(pa);
}


/* --------------------------------------------------------------------------
 If apply-option worker thread job failed display a proper error message.
- Parameters -------------------------------------------------------------
 ULONG rc     : worker thread job return value (0 = success).
 PAPPLYBMP pa : apply-bitmap-options data structure.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID handleApplyBitmapJobError(ULONG rc, PAPPLYBMP pa) {
   switch (rc) {
      case APPLYBMP_ERR_READFILE:
         errorBox(ISERR_READFILE, pa->achBmpFile);
         break;
      case APPLYBMP_ERR_INVALIDFILE:
         errorBox(ISERR_INVALIDFILE, pa->achBmpFile);
         break;
      case APPLYBMP_ERR_TOOLARGE:
         errorBox(ISERR_BMPTOOLARGE, pa->achBmpFile);
         break;
      case APPLYBMP_ERR_SAVEDATA:
         errorBox(ISERR_SAVEDATA);
         break;
   } /* endswitch */
   resetCommonButton(BTN_APPLY, PGFL_APPLY_ENABLED, PGFL_APPLY_ENABLED);
}


/* --------------------------------------------------------------------------
 Read a bitmap file and if the file is valid convert it to the 1.x format
 checking that the total size is less than 64 KBs.
- Parameters -------------------------------------------------------------
 PSZ pszFile            : bitmap file name.
 PBITMAPFILEHEADER pBmp : buffer where to write the converted bitmap data.
 PULONG pCbBmp          : total size of the resulting bitmap data.
- Return value -----------------------------------------------------------
 ULONG : 0 (success), or error code.
-------------------------------------------------------------------------- */
static ULONG readBmpFile(PSZ pszFile, PBITMAPFILEHEADER pBmp, PULONG pCbBmp) {
   ULONG rc = APPLYBMP_SUCCESS;
   ULONG cClrs, cbPal, cbBmp;
   PBYTE pData;
   PBITMAPFILEHEADER2 pbfh;
   RGB* prgb;
   RGB2* prgb2;
   if (NULL == (pData = (PBYTE)gpReadFile(pszFile)))
      return APPLYBMP_ERR_READFILE;

   // if the file is a bitmap array find the first BITMAPFILEHEADER structure
   pbfh = (*((PUSHORT)(pData)) == BFT_BITMAPARRAY)?
             &((PBITMAPARRAYFILEHEADER2)(pData))->bfh2:
             (PBITMAPFILEHEADER2)(pData);
   // if this is a valid bitmap file
   if (pbfh->usType == BFT_BMAP) {
      // format 1.x
      if (pbfh->cbSize == sizeof(BITMAPFILEHEADER)) {
         // copy the file header
         memcpy(pBmp, pbfh, pbfh->cbSize);
         // if there is a palette copy it
         if (pBmp->bmp.cBitCount <= 8) {
            cbPal = sizeof(RGB) * (1 << pBmp->bmp.cBitCount);
            memcpy(((PBITMAPINFO)&pBmp->bmp)->argbColor,
                   ((PBITMAPINFO)&pbfh->bmp2)->argbColor,
                   cbPal);
         } else {
            cbPal = 0;
         } /* endif */
      // format 2.x
      } else {
         // bitmap file header
         memcpy(pBmp, pbfh, 14);
         pBmp->cbSize = sizeof(BITMAPFILEHEADER);
         pBmp->bmp.cbFix = sizeof(BITMAPINFOHEADER);
         pBmp->bmp.cx = pbfh->bmp2.cx;
         pBmp->bmp.cy = pbfh->bmp2.cy;
         pBmp->bmp.cPlanes = pbfh->bmp2.cPlanes;
         pBmp->bmp.cBitCount = pbfh->bmp2.cBitCount;
         // if there is a palette converts it to the 1.x format
         if (pbfh->bmp2.cBitCount <= 8) {
            cClrs = (1 << pbfh->bmp2.cBitCount);
            cbPal = sizeof(RGB) * cClrs;
            // only a subset of the palette has been defined
            if (pbfh->bmp2.cbFix > offsetof(BITMAPINFO2, cclrUsed)
                &&
                pbfh->bmp2.cclrUsed) {
               memset(((PBITMAPINFO)&pBmp->bmp)->argbColor, 0, cbPal);
               cClrs = pbfh->bmp2.cclrUsed;
            } /* endif */
            prgb = ((PBITMAPINFO)&pBmp->bmp)->argbColor;
            prgb2 = ((PBITMAPINFO2)&pbfh->bmp2)->argbColor;
            while (cClrs--) {
               prgb->bBlue = prgb2->bBlue;
               prgb->bGreen = prgb2->bGreen;
               prgb->bRed = prgb2->bRed;
               prgb++, prgb2++;
            } /* endwhile */
         } else {
            cbPal = 0;
         } /* endif */
      } /* endif */
      // copy the bitmap data
      pBmp->offBits = pBmp->cbSize + cbPal;
      cbBmp = calcBmpDataSize(pBmp->bmp.cx, pBmp->bmp.cy, pBmp->bmp.cBitCount);
      *pCbBmp = cbBmp + pBmp->offBits;
      if (*pCbBmp < 0x10000) {
         memcpy((PBYTE)pBmp + pBmp->offBits, pData + pbfh->offBits, cbBmp);
      } else {
         *pCbBmp = 0;
         rc = APPLYBMP_ERR_TOOLARGE;
      } /* endif */
   } else {
      rc = APPLYBMP_ERR_INVALIDFILE;
   } /* endif */
   gpFreeReadFile(pData);
   return rc;
}


/* --------------------------------------------------------------------------
 Save the bitmap data to the ini files.
- Parameters -------------------------------------------------------------
 PAPPLYBMP pa : apply-titlebar-options worker job related data.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
static BOOL saveBmpData(PAPPLYBMP pa) {
   BOOL rc = FALSE;
   HINI hini;
   PSZ pszFile = NULL;
   PBYTE pData = NULL;
   ULONG cbData = 0;
   if (NULLHANDLE != (hini = stlrOpenProfile())) {
      if (*pa->achBmpFile) {
         pszFile = strrchr(pa->achBmpFile, '\\') + 1;
         pData = pa->aBmpData;
         cbData = pa->cbData;
      } else {
         pszFile = pData = NULL;
         cbData = 0;
      } /* endif */
      // save the bitmap name and data in OS2.INI
      if (setProfileString(hini, SZPRO_OPTIONS, pa->pBmp->pszNameKey,
                                pszFile)
          &&
          setProfileData(hini, SZPRO_OPTIONS, pa->pBmp->pszDataKey,
                         pData, cbData)) {
         rc = TRUE;
      } /* endif */
      PrfCloseProfile(hini);
   } /* endif */
   return rc;
}


/* --------------------------------------------------------------------------
 Use the worker thread to scan the relative path pszFileSpec for valid
 (matching the limits set in pBmpLimit) bitmap files.
- Parameters -------------------------------------------------------------
 HWND hwnd            : dialog window handle.
 PSZ pszFileSpec      : relative path to be searched for bitmap files.
 PSZ pszCurBmpFile    : current selected bitmap file name.
 PBMPLIMITS pBmpLimit : limits used to consider a bitmap valid.
                        A NULL value means no limits.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID listBitmaps(HWND hwnd, PSZ pszFileSpec,
                 PSZ pszCurBmpFile, PBMPLIMITS pBmpLimit) {
   PLISTBMPFILES plbf;
   plbf = malloc(sizeof(LISTBMPFILES));
   memset(plbf, 0, sizeof(LISTBMPFILES));
   makeFullPathName(plbf->achPath, pszFileSpec);
   plbf->pszCurBmpFile = pszCurBmpFile;
   if (pBmpLimit) plbf->limit = *pBmpLimit;
   workerJobAdd(hwnd, STLRWID_LISTBMPFILES,
                (PTHREADJOBFN)listBitmapsJob, (PDELJOBFN)freeBitmapsList, plbf);
}


/* --------------------------------------------------------------------------
 Scan a directory for valid bitmap files.
- Parameters -------------------------------------------------------------
 PLISTBMPFILES plbf: job data.
- Return value -----------------------------------------------------------
 ULONG : count of valid bitmap files.
-------------------------------------------------------------------------- */
ULONG listBitmapsJob(PLISTBMPFILES plbf) {
   HDIR hDir = HDIR_CREATE;
   FILEFINDBUF3 ff;
   PBMPFILEITEM p;
   ULONG ul = 1;
   ULONG cFiles = 0;
   ULONG ulRet = ERROR_LISTBMPFILES;
   PSZ pEndPath = plbf->achPath + strlen(plbf->achPath);
   p = (PBMPFILEITEM)&plbf->pBmpFile; // to auto-initialize the list items
   strcpy(pEndPath, "*.bmp");
   if (DosFindFirst(plbf->achPath, &hDir, ANYFILE,
                    &ff, sizeof(ff), &ul, FIL_STANDARD))
      return 0;
   do {
      strcpy(pEndPath, ff.achName);
      if (isValidBmpFile(plbf->achPath, &plbf->limit)) {
         // add a BMPFILEITEM structure to the list
         if (NULL == (p->pnext = malloc(5 + ff.cchName))) goto exit_0;
         p = p->pnext;
         p->pnext = NULL;
         memcpy(p->achFile, ff.achName, ff.cchName + 1);
         cFiles++;
      } /* endif */
   } while (!DosFindNext (hDir, &ff, sizeof(ff), &ul)); /* enddo */
   ulRet = cFiles;
exit_0:
   DosFindClose(hDir);
   return ulRet;
}


/* --------------------------------------------------------------------------
 Check if a bitmap file is valid: i.e. the total bitmap data is less than
 64KB and the bitmap size and color depth are within the defined limits.
- Parameters -------------------------------------------------------------
 PSZ pszFileName : bitmap file name.
 PBMPLIMITS pbl  : maximum allowed size and color depth:
          ULONG cMaxClrs : maximum colors. Bitmaps exceeding this value are
                           to be considered invalid. (0 means no limit).
          ULONG cxMax    : maximum bitmap width.
          ULONG cyMax    : maximum bitmap height.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (valid/invalid bitmap file).
-------------------------------------------------------------------------- */
static BOOL isValidBmpFile(PSZ pszFileName, PBMPLIMITS pbl) {
   PBITMAPFILEHEADER2 pbfh;
   PBYTE pData;
   BOOL rc = FALSE;
   ULONG cx, cy, cBits, cbData;
   if (NULL == (pData = (PBYTE)gpReadFile(pszFileName))) return FALSE;
   // if the file is a bitmap array find the first BITMAPFILEHEADER structure
   pbfh = (*((PUSHORT)(pData)) == BFT_BITMAPARRAY)?
          &((PBITMAPARRAYFILEHEADER2)(pData))->bfh2:
          (PBITMAPFILEHEADER2)(pData);
   if (pbfh->usType != BFT_BMAP) goto exit_0;
   // check the bitmap format (1.x or 2.x)
   // if this is a bitmap 1.x
   if (pbfh->cbSize == sizeof(BITMAPFILEHEADER)) {
      cx = ((PBITMAPINFO)&pbfh->bmp2)->cx;
      cy = ((PBITMAPINFO)&pbfh->bmp2)->cy;
      cBits = ((PBITMAPINFO)&pbfh->bmp2)->cBitCount;
   // bitmap format 2.x
   } else {
      cx = pbfh->bmp2.cx;
      cy = pbfh->bmp2.cy;
      cBits = pbfh->bmp2.cBitCount;
   } /* endif */
   // check the width, height and color depth limits
   if ((pbl->cMaxClrs && (cBits > pbl->cMaxClrs))
          ||
          (pbl->cxMax && (cx > pbl->cxMax))
          ||
          (pbl->cyMax && (cy > pbl->cyMax)))
         goto exit_0;
   // check the data size limits (the bitmap is later converted to format 1.x)
   cbData = sizeof(BITMAPFILEHEADER) + calcBmpDataSize(cx, cy, cBits);
   if (cBits <= 8) cbData += sizeof(RGB) * (1 << cBits);
   rc = (cbData < 0x10000);
exit_0:
   gpFreeReadFile(pData);
   return rc;
}


/* --------------------------------------------------------------------------
 Calculate the size used to store bitmap data (excluding the the headers).
- Parameters -------------------------------------------------------------
 ULONG cx    : bitmap width.
 ULONG cy    : bitmap height.
 ULONG cClrs : count of colors (as bits per pixel).
- Return value -----------------------------------------------------------
 ULONG : bitmap data size.
-------------------------------------------------------------------------- */
static ULONG calcBmpDataSize(ULONG cx, ULONG cy, ULONG cClrs) {
   ULONG cbLine;
   switch (cClrs) {
      case 1: cbLine = RNDUP(cx, 8) / 8; break;   // black & white
      case 4: cbLine = RNDUP(cx, 2) / 2; break;   // 16 colors
      case 8: cbLine = cx;               break;   // 256 colors
      default: cbLine = cx * 3;          break;   // 65 K or true color
   } /* endswitch */
   cbLine = RNDUP(cbLine, 4);
   return cbLine * cy;
}


/* --------------------------------------------------------------------------

- Parameters -------------------------------------------------------------
 PLISTBMPFILES plbf : structure used to store the data of bitmap files.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID freeBitmapsList(PLISTBMPFILES plbf) {
   PBMPFILEITEM p;
   while (plbf->pBmpFile) {
      p = plbf->pBmpFile;
      plbf->pBmpFile = p->pnext;
      free(p);
   } /* endwhile */
   free(plbf);
}


/* --------------------------------------------------------------------------
 Insert the bitmap file names found by the worker thread in a combobox
 inserting a 'default image' item as first item and selecting the current
 item.
- Parameters -------------------------------------------------------------
 HWND hwnd : combobox window handle.
 PLISTBMPFILES plbf : list of bitmap files.
 BOOL bEnable       : if TRUE enable the listbox/combobox window.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID fillBitmapList(HWND hLbox, PLISTBMPFILES plbf, BOOL bEnable) {
   PBMPFILEITEM p;
   CHAR buf[256];
   g.state |= STLRIS_SKIPNOTIFICATION;
   for (p = plbf->pBmpFile; p; p = p->pnext) {
      wLbxItemIns(hLbox, LIT_SORTASCENDING, p->achFile);
   } /* endfor */
   loadString(IDS_DEFIMAGE, buf);
   wLbxItemIns(hLbox, 0, buf);
   selectListItem(hLbox, plbf->pszCurBmpFile);
//   WinSetWindowText(hLbox, *plbf->pszCurBmpFile ? plbf->pszCurBmpFile: buf);
   if (bEnable) WinEnableWindow(hLbox, TRUE);
   g.state &= ~STLRIS_SKIPNOTIFICATION;
}


/* --------------------------------------------------------------------------
 Look for bitmap data in the ESTYLER.INI file.
 If no data is found get the default bitmap.
 Get the bitmap handle.
- Parameters -------------------------------------------------------------
 HPS hps       : presentation space handle.
 PBYTE pData   : buffer holding the bitmap data.
 PBMPPAINT pbp : (output) if not NULL holds data to paint the bitmap.
 HMODULE hmod  : resource DLL module handle.
 ULONG id      : id of bitmap resource in the resource DLL.
                 If this is 0 then no bitmap resource is loaded.
 PBMPDAT pbd   : address where to store bitmap data and bitamp bits
                 (if not NULL)
- Return value -----------------------------------------------------------
 HBITMAP : bitmap handle. NULLHANDLE in case of error or for the default
           bitmaps.
-------------------------------------------------------------------------- */
HBITMAP stlrHBmp(HPS hps, PBYTE pData, PBMPPAINT pbp, ULONG id, PBMPDAT pbd) {
   // get the bitmap handle from smartwin.ini
   return pData ? stlrHBmpFromBmpData(hps, pData, pbp, pbd):
                  stlrHBmpFromModule(hps, g.hResLib, id, pbp, pbd);
}


/* --------------------------------------------------------------------------
 Free a global bitmap.
- Parameters -------------------------------------------------------------
 HWND hwnd   : window handle.
 HBITMAP hbmp: global bitmap handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID freeGlobalBitmap(HWND hwnd, HBITMAP hbmp) {
   HPS hps;
   PID pid;
   if (hbmp
       &&
       WinQueryWindowProcess(hwnd, &pid, NULL)
       &&
       (NULLHANDLE != (hps = WinGetPS(hwnd)))) {
      stlrFreeGlobalBitmap(hps, hbmp, pid);
      WinReleasePS(hps);
   } /* endif */
}


/* --------------------------------------------------------------------------
 According to the current settings get the proper titlebar bitmap handle.
- Parameters -------------------------------------------------------------
 HPS hps          : presentation space handle. (Allocate if NULL).
 PBYTE pData      : bitmap data (content of bitmap file).
 PTBARHILITE ptbh : active/inactive titlebar options.
 PSHDCREATE pshc  : gradient creation structure. (Allocate if NULL).
 ULONG idRes      : resource id of the default bitmap.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
BOOL getTitlebarHbmp(HPS hps, PBYTE pData,
                     PTBARHILITE ptbh, PSHDCREATE pshc, ULONG idRes) {
   BOOL rc = FALSE;
   ULONG fl = 0;
   if (!hps) {
      if (NULLHANDLE == (hps = WinGetScreenPS(HWND_DESKTOP)))
         return FALSE;
      fl |= 0x01;
   } /* endif */
   if (ptbh->bkgnd == TBARBKGNDSHADE) {
      if (!pshc) {
         if (NULL == (pshc = (PSHDCREATE)malloc(CB_SHADEALLOC)))
            goto exit_0;
         fl |= 0x02;
      } /* endif */
      rc = ptbh->bmpp.hbmp = stlrShadeHBmp(hps, pshc, ptbh->fl,
                                           ptbh->clrLeftTop, ptbh->clrRghtBtm);
   } else if (ptbh->bkgnd == TBARBKGNDBMP) {
      rc = ptbh->bmpp.hbmp = stlrHBmp(hps, pData, &ptbh->bmpp, idRes, NULL);
   } else {
      rc = TRUE;
   } /* endif */
exit_0:
   if (fl & 0x01) WinReleasePS(hps);
   if (fl & 0x02) free(pshc);
   return rc;
}


/* --------------------------------------------------------------------------
 According to the current settings get the proper pushbutton bitmap handle.
- Parameters -------------------------------------------------------------
 HPS hps          : presentation space handle. (Allocate if NULL).
 PBYTE pData      : bitmap data (content of bitmap file).
 PBTNOPT pbtno    : current pushbutton options.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
BOOL getBtnHbmp(HPS hps, PBYTE pData, PBTNOPT pbtno) {
   BOOL bFreeHPS = FALSE;
   if (!hps) {
      if (NULLHANDLE == (hps = WinGetScreenPS(HWND_DESKTOP)))
         return FALSE;
      bFreeHPS = TRUE;
   } /* endif */
   pbtno->bh.cbFix = 0;
   pbtno->bh.cx = 40;
   pbtno->bh.cy = 40;
   pbtno->bmpp.hbmp = stlrHBmp(hps, pData, &pbtno->bmpp,
                               ((g.scr.cclr > 256) ? IDBMP_BTN: IDBMP_BTN8),
                               (PBMPDAT)&pbtno->bh);
   if (bFreeHPS) WinReleasePS(hps);
   return (BOOL)pbtno->bmpp.hbmp;
}


/* --------------------------------------------------------------------------
 Show the file selection dialog.
 If the user select a bitmap file:
    if the bitmap file is valid:
       - copy the bitmap file to the bitmap file path.
       - update the bitmap file list.
- Parameters -------------------------------------------------------------
 HWND hwnd   : dialog window handle.
 PSZ pszPath : path where the image must be added.
 PBMPLIMITS pBmpLimit : limits used to consider a bitmap valid.
                        A NULL value means no limits.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
BOOL addImage(HWND hwnd, PSZ pszPath, PBMPLIMITS pBmpLimit) {
   PADDBMPFILE pabf;
   if (addFileDlg(hwnd, "*.BMP")) {
      if (NULL == (pabf = malloc(sizeof(ADDBMPFILE))))
         return handleError(ISERR_ALLOCATION, FALSE);
      memset(pabf, 0, sizeof(ADDBMPFILE));
      strcpy(pabf->achBmpFile, g.achFileSel);
      makeFullPathName(pabf->achDestPath, pszPath);
      if (pBmpLimit) pabf->limits = *pBmpLimit;
      return workerJobAdd(hwnd, STLRWID_ADDBMP, (PTHREADJOBFN)addImageJob,
                          (PDELJOBFN)freeAddImageRes, pabf);
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 - check if the bitmap file is valid,
 - create a unique name for the destination file,
 - copy the file.
- Parameters -------------------------------------------------------------
 PADDBMPFILE pabf : data used to add a bitmap file.
- Return value -----------------------------------------------------------
 ULONG : 0 (success) or error code.
-------------------------------------------------------------------------- */
static BOOL addImageJob(PADDBMPFILE pabf) {
   CHAR bmpFileName[CCHMAXPATHCOMP];
   PSZ p, pDest;
   APIRET rc;
   BOOL bLimitName;
   INT i;
   // check if the source and the destination paths coincide
   pDest = pabf->achDestPath + strlen(pabf->achDestPath);
   p = strrchr(pabf->achBmpFile, '\\');
   *p = *(pDest - 1) = 0;
   if (WCS_EQ
       ==
       WinCompareStrings(0, 0, 0, pabf->achBmpFile, pabf->achDestPath, 0))
      return ADDBMPF_ERR_SOURCEQDEST;
   // reset the path separators
   *p = *(pDest - 1) = '\\';
   // check if the bitmap file is valid
   if (!isValidBmpFile(pabf->achBmpFile, &pabf->limits))
      return ADDBMPF_ERR_INVALID;
   // get the name of the bitmap file (excluding the extension)
   strcpy(bmpFileName, p + 1);
   if (NULL != (p = strrchr(bmpFileName, '.'))) *p = 0;
   sprintf(pabf->achFileName, "%s.BMP", bmpFileName);
   // copy the bitmap file changing the destination name if the file
   // already exists or if the target path doesn't support long file
   // names and the file name is longer than 8 characters
   for (i = 0, rc = 1, bLimitName = FALSE; rc; ) {
      strcpy(pDest, pabf->achFileName);
      rc = DosCopy(pabf->achBmpFile, pabf->achDestPath, 0);
      if (rc == ERROR_ACCESS_DENIED) {            // file already exists
         if (i == 999) break;
         if (bLimitName) {
            sprintf(pabf->achFileName, "%.5s%03d.BMP", bmpFileName, i++);
         } else {
            sprintf(pabf->achFileName, "%s_%03d.BMP", bmpFileName, i++);
         } /* endif */
      } else if (rc == ERROR_INVALID_NAME) {
         if (i > 999) break;
         sprintf(pabf->achFileName, "FILE_%03d.BMP", i++);
      } else if (rc == ERROR_FILENAME_EXCED_RANGE) {
         bLimitName = TRUE;
         sprintf(pabf->achFileName, "%s.8s.BMP", bmpFileName);
      } else {
         break;
      } /* endif */
   } /* endfor */
   return rc ? ADDBMPF_ERR_COPYFILE: ADDBMPF_SUCCESS;
}


/* --------------------------------------------------------------------------
 Release the storage used for adding a bitmap file.
- Parameters -------------------------------------------------------------
 PADDBMPFILE pabf : data used to add a bitmap file.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID freeAddImageRes(PADDBMPFILE pabf) {
   free(pabf);
}


/* --------------------------------------------------------------------------
 On completion of the add-bitmap-file job executed by the worker thread,
 if no error occurred add the name of the added file to the list of
 bitmap files.
- Parameters -------------------------------------------------------------
 HWND hwnd        : combobox window handle.
 ULONG rc         : worker thread job return code.
 PADDBMPFILE pabf : add-bitmap-file data.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID addImageJobEnd(HWND hwnd, ULONG rc, PADDBMPFILE pabf) {
   if (rc) {
      handleAddImageJobError(rc, pabf);
   } else {
      wLbxItemIns(hwnd, LIT_SORTASCENDING, pabf->achFileName);
   } /* endif */
}


/* --------------------------------------------------------------------------
 If the add-bitmap-file job failed display a proper error message.
- Parameters -------------------------------------------------------------
 ULONG rc     : worker thread job return value (0 = success).
 PADDBMPFILE pabf : add-bitmap-file data.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID handleAddImageJobError(ULONG rc, PADDBMPFILE pabf) {
   PSZ p;
   switch (rc) {
      case ADDBMPF_ERR_INVALID:
         errorBox(ISERR_INVALIDFILE, pabf->achBmpFile);
         break;
      case ADDBMPF_ERR_COPYFILE:
         errorBox(ISERR_COPYFILE, pabf->achBmpFile, pabf->achFileName);
         break;
      case ADDBMPF_ERR_SOURCEQDEST:
         p = strrchr(pabf->achBmpFile, '\\');
         *p = '\x00';
         errorBox(ISERR_SOURCEEQDEST, pabf->achBmpFile);
         break;
   } /* endswitch */
}


