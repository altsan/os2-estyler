/* --------------------------------------------------------------------------
 bitmaps.c : bitmap and shade handling procedures.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

#pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------


// prototypes ---------------------------------------------------------------
HBITMAP stlrHBmpFromBmpData(HPS hps, PBYTE pbmp, PBMPPAINT pbp, PBMPDAT pbd);
HBITMAP stlrHBmpFromModule(HPS hps, HMODULE hmod, ULONG id,
                           PBMPPAINT pbp, PBMPDAT pbd);
BOOL stlrPrfBmpData(HINI hini, PSZ pszKey, PBYTE pdata);
HBITMAP stlrShadeHBmp(HPS hps, PSHDCREATE psc, ULONG fl,
                      LONG clrLeftTop, LONG clrRightBottom);
HBITMAP stlrGlobalHBmp(HINI hini, PSZ key, PBYTE p, HPS hps,
                       PBMPPAINT pbp, HMODULE hmod, ULONG id, PBMPDAT pbd);
HBITMAP stlrGlobalShadeHBmp(HPS hps, PSHDCREATE psc, ULONG fl,
                            LONG clrLeftTop, LONG clrRightBottom);
VOID stlrFreeGlobalBitmaps(HPS hps, HWND hwnd, PID pid, ULONG flDelete);
VOID stlrFreeGlobalBitmap(HPS hps, HBITMAP hbmp, PID pid);

// imported procedures ------------------------------------------------------
#pragma import(GpiSetBitmapOwner,,"PMGPI.DLL",620)
BOOL APIENTRY GpiSetBitmapOwner(HPS hps, HBITMAP hbmp, PID pid);

// global variables ---------------------------------------------------------

/* --------------------------------------------------------------------------
 If, according to the current settings, bitmap images are to be used to
 paint the titlebar or the pushbuttons, load them as global shared resources.
- Parameters -------------------------------------------------------------
 HINI hini : ESTYLER.INI handle
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID setGlobalBitmaps(HINI hini) {
   PSZ pBuf = NULL;
   HMODULE hModRes = NULLHANDLE;
   HPS hps;
   // allocate the needed resources --------------------------------------
   // the function called safely handle allocations errors
   hps = WinGetScreenPS(HWND_DESKTOP);
   DosLoadModule(NULL, 0, SZ_RESMODULE, &hModRes);
   m_sysMemAlloc(pBuf, 0x10000);
   // active title bar bitmap --------------------------------------------
   if (o.ui.tb.a.bkgnd == TBARBKGNDSHADE) {
      o.ui.tb.a.bmpp.hbmp = stlrGlobalShadeHBmp(hps, (PSHDCREATE)pBuf,
                                           o.ui.tb.a.fl,
                                           o.ui.tb.a.clrLeftTop,
                                           o.ui.tb.a.clrRghtBtm);
   } else if (o.ui.tb.a.bkgnd == TBARBKGNDBMP) {
      o.ui.tb.a.bmpp.hbmp = stlrGlobalHBmp(hini, SZPRO_BMPATBARDATA, pBuf, hps,
                                &o.ui.tb.a.bmpp, hModRes,
                                ((g.scr.cclr > 256) ? IDBMP_TBA: IDBMP_TBA8),
                                NULL);
   } /* endif */
   // in case of failure set to use solid color
   if (o.ui.tb.a.bkgnd && !o.ui.tb.a.bmpp.hbmp)
      o.ui.tb.a.bkgnd = TBARBKGNDSOLIDCLR;
   // active title bar bitmap --------------------------------------------
   if (o.ui.tb.i.bkgnd == TBARBKGNDSHADE) {
      o.ui.tb.i.bmpp.hbmp = stlrGlobalShadeHBmp(hps, (PSHDCREATE)pBuf,
                                           o.ui.tb.i.fl,
                                           o.ui.tb.i.clrLeftTop,
                                           o.ui.tb.i.clrRghtBtm);
   } else if (o.ui.tb.i.bkgnd == TBARBKGNDBMP) {
      o.ui.tb.i.bmpp.hbmp = stlrGlobalHBmp(hini, SZPRO_BMPITBARDATA, pBuf, hps,
                                &o.ui.tb.i.bmpp, hModRes,
                                ((g.scr.cclr > 256) ? IDBMP_TBI: IDBMP_TBI8),
                                NULL);
   } /* endif */
   // in case of failure set to use solid color
   if (o.ui.tb.i.bkgnd && !o.ui.tb.i.bmpp.hbmp)
      o.ui.tb.i.bkgnd = TBARBKGNDSOLIDCLR;
   // pushbutton bitmap --------------------------------------------------
   if (!o.ui.btn.solid) {
      o.ui.btn.bh.cbFix = 0;
      o.ui.btn.bh.cx = 40;
      o.ui.btn.bh.cy = 40;
      o.ui.btn.bmpp.hbmp = stlrGlobalHBmp(hini, SZPRO_BMPBTNDATA, pBuf, hps,
                                &o.ui.btn.bmpp, hModRes,
                                ((g.scr.cclr > 256) ? IDBMP_BTN: IDBMP_BTN8),
                                (PBMPDAT)&o.ui.btn.bh);
   } /* endif */
   // in case of failure set to use solid color
   if (o.ui.btn.solid && !o.ui.btn.bmpp.hbmp)
      o.ui.btn.solid = TRUE;
   // free the resources -------------------------------------------------
   if (hModRes) DosFreeModule(hModRes);
   if (pBuf) m_sysMemFree(pBuf);
   if (hps) WinReleasePS(hps);
}


/* --------------------------------------------------------------------------
 Get a bitmap handle from the content of a bitmap file optionally
 returning the bitmap size.
 Any OS/2 bitmap is valid (either 1.x and 2.x formats).
 If the bitmap file contains a bitmaps array only the first bitmap is
 taken into account.
- Parameters -------------------------------------------------------------
 HPS hps       : presentation space handle.
 PBYTE pbmp    : bitmap data (content of bitmap file).
 PBMPPAINT pbp : (output) if not NULL holds data to paint the bitmap.
 PBMPDAT pbd   : address where to store bitmap data and bitamp bits
                 (if not NULL)
- Return value -----------------------------------------------------------
 HBITMAP : handle of the bitmap or NULLHANDLE in case of error.
-------------------------------------------------------------------------- */
HBITMAP stlrHBmpFromBmpData(HPS hps, PBYTE pbmp, PBMPPAINT pbp, PBMPDAT pbd) {
   HBITMAP hbmp = NULLHANDLE;
   PBITMAPFILEHEADER2 pbfh;
   PBITMAPINFO pbi;
   INT i;
   // if is a bitmap array points to the first bitmap file header
   pbfh = (*((PUSHORT)(pbmp)) == BFT_BITMAPARRAY)?
          &((PBITMAPARRAYFILEHEADER2)(pbmp))->bfh2:
          (PBITMAPFILEHEADER2)(pbmp);
   // if this is a true bitmap get the bitmap handle
   if (pbfh->usType == BFT_BMAP) {
      hbmp = GpiCreateBitmap(hps, &pbfh->bmp2, CBM_INIT, pbmp + pbfh->offBits,
                             (PBITMAPINFO2)&pbfh->bmp2);
      // store bitmap size and hotspots (used for other purposes)
      if (pbp) {
         // format 1.x
         if (pbfh->cbSize == sizeof(BITMAPFILEHEADER)) {
            pbi = ((PBITMAPINFO)&pbfh->bmp2);
            pbp->cx = pbi->cx;
            pbp->cy = pbi->cy;
         // format 2.x
         } else {
            pbp->cx = pbfh->bmp2.cx;
            pbp->cy = pbfh->bmp2.cy;
         } /* endif */
//         // no need to cast PBITMAPFILEHEADER2 to PBITMAPFILEHEADER
//         pbp->dx = pbfh->xHotspot;
//         pbp->dy = pbfh->yHotspot;
      } /* endif */
      // store bitmap minimal header and bitmap bits
      if (pbd) {
         // format 1.x
         if (pbfh->cbSize == sizeof(BITMAPFILEHEADER)) {
            pbi = ((PBITMAPINFO)&pbfh->bmp2);
            // if the current bitmap parameters match the requested parameters
            if ((pbi->cx <= pbd->cx)
                &&
                (pbi->cy <= pbd->cy)
                &&
                (pbi->cBitCount == 8)) {  // only 256 colors bitmaps allowed
               pbd->cbFix = 16;
               pbd->cx = pbi->cx;
               pbd->cy = pbi->cy;
               pbd->cPlanes = 1;
               pbd->cBitCount = 8;
               // copy the palette
               for (i = 0; i < 256; ++i) {
                  pbd->argbColor[i].bBlue = pbi->argbColor[i].bBlue;
                  pbd->argbColor[i].bGreen = pbi->argbColor[i].bGreen;
                  pbd->argbColor[i].bRed = pbi->argbColor[i].bRed;
                  pbd->argbColor[i].fcOptions = 0;
               } /* endfor */
               // copy the bitmap bits
               memcpy(pbd->ab, (PBYTE)pbfh + pbfh->offBits, pbd->cx * pbd->cy);
            } /* endif */
         // format 2.x
         } else {
            // if the current bitmap parameters match the requested parameters
            if ((pbfh->bmp2.cx <= pbd->cx)
                &&
                (pbfh->bmp2.cy <= pbd->cy)
                &&
                (pbfh->bmp2.cBitCount == 8)) {
               pbd->cbFix = 16;
               pbd->cx = pbfh->bmp2.cx;
               pbd->cy = pbfh->bmp2.cy;
               pbd->cPlanes = 1;
               pbd->cBitCount = 8;
               // copy the palette
               memcpy(pbd->argbColor, (PBYTE)&pbfh->bmp2 + pbfh->bmp2.cbFix,
                      (ULONG)(((PBYTE)pbfh + pbfh->offBits) -
                              ((PBYTE)&pbfh->bmp2 + pbfh->bmp2.cbFix)));
               // copy the bitmap bits
               memcpy(pbd->ab, (PBYTE)pbfh + pbfh->offBits, pbd->cx * pbd->cy);
            } /* endif */
         } /* endif */
      } /* endif */
   } /* endif */
   return hbmp;
}


/* --------------------------------------------------------------------------
 Get a bitmap handle and optionally the bitmap size from a bitmap
 contained in a resources DLL.
- Parameters -------------------------------------------------------------
 HPS hps       : presentation space handle.
 HMODULE hmod  : module handle.
 ULONG id      : bitmap resource id.
 PBMPPAINT pbp : (output) if not NULL holds data to paint the bitmap.
 PBMPDAT pbd   : address where to store bitmap data and bitamp bits
                 (if not NULL)
- Return value -----------------------------------------------------------
 HBITMAP : handle of the bitmap or NULLHANDLE in case of error.
-------------------------------------------------------------------------- */
HBITMAP stlrHBmpFromModule(HPS hps, HMODULE hmod, ULONG id,
                           PBMPPAINT pbp, PBMPDAT pbd) {
   BITMAPINFOHEADER bih;
   HBITMAP hbmp;
   PBYTE pBmpData;
   // load the bitmap data (return NULLHANDLE in case of error)
   if (!hmod || DosGetResource(hmod, RT_BITMAP, id, (PPVOID)&pBmpData))
      return NULLHANDLE;
   hbmp = stlrHBmpFromBmpData(hps, pBmpData, pbp, pbd);
   DosFreeResource(pBmpData);
   return hbmp;
}


/* --------------------------------------------------------------------------
 Get the bitmap data from the smartwin.ini file.
- Parameters -------------------------------------------------------------
 HINI hini   : smartwin.ini handle.
 PSZ pszKey  : bitmap to get.
 PBYTE pdata : buffer where the bitmap data must be returned.
               It must be at least 0x10000 bytes.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
BOOL stlrPrfBmpData(HINI hini, PSZ pszKey, PBYTE pdata) {
   ULONG ul = 0x10000;
   if (!hini || !pdata) return FALSE;
   return PrfQueryProfileData(hini, SZPRO_OPTIONS, pszKey, pdata, &ul);
}


/* --------------------------------------------------------------------------
 Create a bitmap in memory setting its content according to the current
 shade settings.
 For horizontal shades a bitmap of 96 pixel * cyTitleBar is created,
 then, to draw the titlebar background, the bitmap is tiled horizontally.
 For vertical shades a bitmap of 1920 pixel * 2 is created, then, to
 draw the titlebar backgroun, the bitmap is stretched horizontally and
 tiled vertically.
- Parameters -------------------------------------------------------------
 HPS hps             : presentation space handle.
 PSHDCREATE psc      : buffer for the shade creation data.
 ULONG fl            : shade creation flags (see stlrGraphics.h).
 LONG clrLeftTop     : left/top color.
 LONG clrRightBottom : right/bottom color.
- Return value -----------------------------------------------------------
 HBITMAP : handle of the bitmap or NULLHANDLE in case of error.
-------------------------------------------------------------------------- */
HBITMAP stlrShadeHBmp(HPS hps, PSHDCREATE psc, ULONG fl,
                      LONG clrLeftTop, LONG clrRightBottom) {
   INT i;            // loop index
   LONG dither,      // needed to calculate the pseudo-dithering when needed
        cbline,      // bytes needed for a line
        lsteps,      // color variations between initial and medium value
        rsteps,      // color variations between medium and end value
        lr,          // red temporary value
        lg,          // green temporary value
        lb,          // blue temporary value
        dr,          // red color delta
        dg,          // green color delta
        db;          // blue color delta
   PBYTE p;          // address to initialize the single bytes
   // parameter check
   if (!hps || !psc) return NULLHANDLE;
   // vertical shade --------------------------------------------------------
   if (fl & CREATESHADE_VERT) {
      memset(psc, 0, SHADEV_CB);
      lsteps = (fl & CREATESHADE_DOUBLE) ? SHADEV_CY / 2: SHADEV_CY;
      rsteps = SHADEV_CY - lsteps;
      // init the bitmap header
      m_shadeInitBmpHdr(&psc->bi, SHADEV_CX, SHADEV_CY);
      // calculate color deltas (color values are shifted left by 22)
      m_shadeInitDeltas(dr, dg, db, clrRightBottom, clrLeftTop, lsteps);
      // shift initial color values by 22
      m_shadeInitRGB(lr, lg, lb, clrLeftTop);
      // parte dall'ultimo byte della bitmap
      p = psc->pb + SHADEV_CX * 3 * SHADEV_CY - 1;
      while (lsteps--) {
         for (i = SHADEV_CX; i; --i) {
            m_shadeVSetPixel(p, lr, lg, lb);
         } /* endfor */
         m_shadeVIncr(lr, lg, lb, dr, dg, db);
      } /* endwhile */
      if (rsteps) {
         m_shadeInitDeltas(dr, dg, db, clrLeftTop, clrRightBottom, rsteps);
         while (rsteps--) {
            for (i = SHADEV_CX; i; --i) {
               m_shadeVSetPixel(p, lr, lg, lb);
            } /* endfor */
            m_shadeVIncr(lr, lg, lb, dr, dg, db);
         } /* endwhile */
      } /* endif */
   // horizontal shdade -----------------------------------------------------
   } else {
      memset(psc, 0, SHADEH_CB);
      lsteps = (fl & CREATESHADE_DOUBLE) ? SHADEH_CX / 2: SHADEH_CX;
      rsteps = SHADEH_CX - lsteps;
      // init the bitmap header
      m_shadeInitBmpHdr(&psc->bi, SHADEH_CX, SHADEH_CY);
      // calculate color deltas (color values are shifted left by 22)
      m_shadeInitDeltas(dr, dg, db, clrRightBottom, clrLeftTop, lsteps);
      // shift initial color values by 22
      m_shadeInitRGB(lr, lg, lb, clrLeftTop);
      for (p = psc->pb; lsteps; --lsteps) {
         m_shadeHDitherIncr(p, dither, lsteps, lr, lg, lb, dr, dg, db);
      } /* endfor */
      if (rsteps) {           // esegue solo quando shade a 3 colori
         m_shadeInitDeltas(dr, dg, db, clrLeftTop, clrRightBottom, rsteps);
         for (; rsteps; --rsteps) {
            m_shadeHDitherIncr(p, dither, rsteps, lr, lg, lb, dr, dg, db);
         } /* endfor */
      } /* endif */
      // dithering raddoppia riga di pixel sfalsandola di 1 posizione
      cbline = SHADEH_CX * 3;
      p = psc->pb + cbline; // inizio seconda riga gradazioni colore
      // copia prima riga nella seconda sfalsandola di 1 pixel a destra
      memcpy((PVOID)(p + 3), psc->pb, cbline - 3);
      // colora primo pixel seconda riga come secondo pixel prima riga
      *p++ = *(psc->pb + 3), *p++ = *(psc->pb + 4), *p = *(psc->pb + 5);
   } /* endif */
   return GpiCreateBitmap(hps, (PBITMAPINFOHEADER2)&psc->bi,
                          CBM_INIT, psc->pb, (PBITMAPINFO2)&psc->bi);
}


/* --------------------------------------------------------------------------
 Look for bitmap data in the ESTYLER.INI file.
 If no data is found get the default bitmap.
 Get the bitmap handle and makes it global.
- Parameters -------------------------------------------------------------
 HINI hini     : smartwin.ini handle.
 PSZ key       : key used to store the bitmap in smartwin.ini.
 PBYTE p       : buffer for getting the bitmap data.
 HPS hps       : presentation space handle.
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
HBITMAP stlrGlobalHBmp(HINI hini, PSZ key, PBYTE p, HPS hps,
                       PBMPPAINT pbp, HMODULE hmod, ULONG id, PBMPDAT pbd) {
   HBITMAP hbmp = NULLHANDLE;
   if (!hps) return NULLHANDLE;
   // get the bitmap handle from smartwin.ini
   if (stlrPrfBmpData(hini, key, p))
      hbmp = stlrHBmpFromBmpData(hps, p, pbp, pbd);
   // get the bitmap handle from the resources DLL
   if (!hbmp && id)
      hbmp = stlrHBmpFromModule(hps, hmod, id, pbp, pbd);
   // if a bitmap handle has been allocated make it global
   if (hbmp)  GpiSetBitmapOwner(hps, hbmp, 0);
   return hbmp;
}


/* --------------------------------------------------------------------------
 Create a shade as a bitmap by setting memory allocated storage to the
 proper color values. If the bitmap creation is succesful make the bitmap
 global and shareable among all the system processes.
- Parameters -------------------------------------------------------------
 HPS hps             : presentation space handle.
 PSHDCREATE psc      : buffer for the shade creation data.
 ULONG fl            : shade creation flags (see stlrGraphics.h).
 LONG clrLeftTop     : left/top color.
 LONG clrRightBottom : right/bottom color.
- Return value -----------------------------------------------------------
 HBITMAP : handle of the bitmap or NULLHANDLE in case of error.
-------------------------------------------------------------------------- */
HBITMAP stlrGlobalShadeHBmp(HPS hps, PSHDCREATE psc, ULONG fl,
                            LONG clrLeftTop, LONG clrRightBottom) {
   HBITMAP hbmp;
   if (NULLHANDLE
       !=
       (hbmp = stlrShadeHBmp(hps, psc, fl, clrLeftTop, clrRightBottom)))
      GpiSetBitmapOwner(hps, hbmp, 0);
   return hbmp;
}


/* --------------------------------------------------------------------------
 Free the global bitmaps.
- Parameters -------------------------------------------------------------
 HPS hps        : if this NULLHANDLE get it from the next parameter.
 HWND hwnd      : window from which to get the HPS. If this is NULLHANDLE
                  get the screen PS.
 PID pid        : process ID (if this is 0 get it via DosGetInfoBlocks() )
 ULONG flDelete : bitmask of the bitmaps to be deleted
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID stlrFreeGlobalBitmaps(HPS hps, HWND hwnd, PID pid, ULONG flDelete) {
   PTIB ptib = NULL;
   PPIB ppib = NULL;
   BOOL releasePS;
   INT i;
   releasePS = (BOOL)!hps;
   if (!pid) {
      DosGetInfoBlocks(&ptib, &ppib);
      pid = ppib->pib_ulpid;
   } /* endif */
   if (!hps) hps = hwnd? WinGetPS(hwnd) : WinGetScreenPS(HWND_DESKTOP);
   if (flDelete & MODIF_ACTWIN) {
      m_freeGlobalBitmap(hps, o.ui.tb.a.bmpp.hbmp, pid);
   } /* endif */
   if (flDelete & MODIF_INACTWIN) {
      m_freeGlobalBitmap(hps, o.ui.tb.i.bmpp.hbmp, pid);
   } /* endif */
   if (flDelete & MODIF_PUSHBTN) {
      m_freeGlobalBitmap(hps, o.ui.btn.bmpp.hbmp, pid);
   } /* endif */
   if (releasePS && hps) WinReleasePS(hps);
}


/* --------------------------------------------------------------------------
 Make a bitmap handle global.
- Parameters -------------------------------------------------------------
 HPS hps      : presentation space handle.
 HBITMAP hbmp : bitmap handle to be made global.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
BOOL stlrMakeGlobalBitmap(HPS hps, HBITMAP hbmp) {
    return GpiSetBitmapOwner(hps, hbmp, 0);
}


/* --------------------------------------------------------------------------
 Free a global bitmap.
- Parameters -------------------------------------------------------------
 HPS hps        : presentation space handle.
 PHBITMAP phbmp : global bitmap to be freed.
 PID pid        : id of the process to set as new bitmap owner.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID stlrFreeGlobalBitmap(HPS hps, HBITMAP hbmp, PID pid) {
   if (hbmp) {
      GpiSetBitmapOwner(hps, hbmp, pid);
      GpiDeleteBitmap(hbmp);
   } /* endif */
}


