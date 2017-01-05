/* --------------------------------------------------------------------------
 test.c : routines used to test the preferences application without the
          runtime DLL.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"
#ifdef _STLR_TEST_MODE_

// definitions --------------------------------------------------------------


// prototypes ---------------------------------------------------------------
static VOID setUserInterfaceDefaults(POPTIONS pOpts);

// global variables ---------------------------------------------------------


/* --------------------------------------------------------------------------
 Return the current eStylerLite version number
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 PSTLRVER address of the structure containing the version details.
-------------------------------------------------------------------------- */
PSTLRVER _stlrGetVer(VOID) {
   m_setVersion(&g.version);
   return &g.version;
}


/* --------------------------------------------------------------------------
 Return the address of the structure holding the overall options.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 POPTIONS address of the structure holding the current eStylerLite options
-------------------------------------------------------------------------- */
POPTIONS _stlrGetOptionsDataAddress(VOID) {
   POPTIONS pOpts;
   HINI hini;
   ULONG ul;
   BOOL rc;
   // allocate storage for all the settable options
   if (NULL == (pOpts = malloc(sizeof(OPTIONS)))) return NULL;
   memset(pOpts, 0, sizeof(OPTIONS));
   // open ESTYLER.INI
   hini = _stlrOpenProfile();
   // if cannot get the user interface options set the default values
   ul = sizeof(UIOPTIONS);
   if (!hini
       ||
       !PrfQueryProfileData(hini, SZPRO_OPTIONS, SZPRO_UINTERFACE,
                            &pOpts->ui, &ul))
      setUserInterfaceDefaults(pOpts);
   // if cannot get the shutdown options set the default values
   ul = sizeof(SHUTDOWN);
   if (!hini
       ||
       !PrfQueryProfileData(hini, SZPRO_OPTIONS, SZPRO_SHUTDWON, &pOpts->sd, &ul)) {
      _stlrSetShutdownDefaults(hini, &pOpts->sd.gen);
      _stlrSetShutdownTimingDefaults(&pOpts->sd.wait);
   } /* endif */
   // get the Windows keys options (default values are 0)
   ul = sizeof(WINKEY);
   if (hini)
      PrfQueryProfileData(hini, SZPRO_OPTIONS, SZPRO_WINKEYS, &pOpts->wk, &ul);
   if (hini) PrfCloseProfile(hini);
   // check if there are any user defined exceptions
   rc = PrfQueryProfileSize(HINI_USER, SZPRO_APP, SZEXC_GENERAL, &ul);
   pOpts->ui.exception = rc && ul > 0;
   return pOpts;
error_0:
   free(pOpts);
   return NULL;
}


/* --------------------------------------------------------------------------
 Open ESTYLER.INI in the executable path (this is just for testing !).
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 HINI : ini file handle or NULLHANDLE in case of error.
-------------------------------------------------------------------------- */
HINI _stlrOpenProfile(VOID) {
   CHAR achIniPath[CCHMAXPATH];         // path file inizializzazione
   makeFullPathName(achIniPath, SZPRO_PROFNAME);
   return PrfOpenProfile(NULLHANDLE, achIniPath);
}


/* --------------------------------------------------------------------------
 Return the current screen resolution and color depth.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 PSCRRES address of the screen resolution/color depth structure.
-------------------------------------------------------------------------- */
PSCRRES _stlrQryScreenRes(VOID) {
   HDC hdc = NULLHANDLE;
   HPS hps = NULLHANDLE;
   if (NULLHANDLE != (hps = WinGetScreenPS(HWND_DESKTOP))) {
      if (NULLHANDLE != (hdc = GpiQueryDevice(hps))) {
         DevQueryCaps(hdc, CAPS_WIDTH, 2, &g.scr.cx);
         DevQueryCaps(hdc, CAPS_COLORS, 1, &g.scr.cclr);
      } /* endif */
      WinReleasePS(hps);
   } /* endif */
   return &g.scr;
}


/* --------------------------------------------------------------------------
 Set the default user interface options.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setUserInterfaceDefaults(POPTIONS pOpts) {
   _stlrSetTitlebarGeneralDefaults(&pOpts->ui.tb);
   _stlrSetTitlebarHiliteDefaults(&pOpts->ui.tb.a, TRUE);
   _stlrSetTitlebarHiliteDefaults(&pOpts->ui.tb.i, FALSE);
   _stlrSetButtonDefaults(&pOpts->ui.btn);
   _stlrSetDlgDefaults(&pOpts->ui.dlg);
}

/* --------------------------------------------------------------------------
 Set the default values of the general titlebar options
- Parameters -------------------------------------------------------------
 PTITLEBAR ptbo : general titlebar options.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _stlrSetTitlebarGeneralDefaults(PTITLEBAR ptbo) {
   strcpy(ptbo->achFont, "9.WarpSans Bold");
   ptbo->on = 1;                    // enable enhancements
   ptbo->overridePP = 1;            // override presentation parameters
   ptbo->center = 1;                // center the text
}


/* --------------------------------------------------------------------------
 Set the active/inactive titlebar default values.
- Parameters -------------------------------------------------------------
 PTBARHILITE ptbh : active/inactive titlebar options.
 BOOL bActive     : TRUE/FALSE (active/inactive titlebar).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _stlrSetTitlebarHiliteDefaults(PTBARHILITE ptbh, BOOL bActive) {
   ptbh->bkgnd = m_titleBarDefBkgnd(g.scr.cclr);
   ptbh->fl = TBO_FLDEFAULTS | m_titleBarDefBorder(ptbh->bkgnd);
   if (bActive) {
      if (ptbh->bkgnd) {
         ptbh->clrLeftTop = 0xcccccc;
         ptbh->clrRghtBtm = 0x2020a4;
      } else {
         ptbh->clrLeftTop = 0x80;
         ptbh->clrRghtBtm = 0xcccccc;
      } /* endif */
      ptbh->clrTopLeftBorder = 0;
      ptbh->clrRightBottomBorder = 0xffffff;
      ptbh->clrFgTxt = 0xffffcc;
      ptbh->clrBgTxt = 0;
   } else {
      ptbh->clrLeftTop = 0xcccccc;
      ptbh->clrRghtBtm = 0xa0a0a0;
      ptbh->clrTopLeftBorder = 0;
      ptbh->clrRightBottomBorder = 0xffffff;
      ptbh->clrFgTxt = 0x606060;
      ptbh->clrBgTxt = 0xffffff;
   } /* endif */
}


/* --------------------------------------------------------------------------
 Set the default values of the button options.
- Parameters -------------------------------------------------------------
 PBTNOPT pbo : pushbutton options.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _stlrSetButtonDefaults(PBTNOPT pbo) {
   // pushbutton options
   pbo->on = 1;                     // enable enhancements
   pbo->border = (g.scr.cx > 1280); // thick border if horz res >= 1280
   pbo->flat = 0;
   pbo->overPP = 0;
   pbo->def3D = 0;
   pbo->dis3D = 1;
   pbo->solid = (g.scr.cclr < 256); // solid color if screen res < 256 colors
   pbo->color = 0xcccccc;
}


/* --------------------------------------------------------------------------
 Set the default values of the dialog options.
- Parameters -------------------------------------------------------------
 PDLGOPTIONS pdo: dialog options.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _stlrSetDlgDefaults(PDLGOPTIONS pdo) {
   strcpy(pdo->achFont, SZ_DEFDLGFONT);
   pdo->on = 1;                    // enable enhancements
   pdo->overridePP = 0;            // don't override presentation parameters
}


/* --------------------------------------------------------------------------
 Set the default shutdown values.
- Parameters -------------------------------------------------------------
 HINI hini      : ESTYLER.INI handle.
 PSDGENERAL psd : shutdown general options.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _stlrSetShutdownDefaults(HINI hini, PSDGENERAL psd) {
   if (hini)
      PrfWriteProfileData(hini, SZPRO_SHUTDWON, SZPRO_KILLPROGS,
                          SZPRO_DEFKILLPRG, CB_DEFKILLPRG);
   psd->steps = 100;
   psd->on = 1;
   psd->lksusp = 0;
   psd->susp = 0;
   psd->off = 0;
   psd->reboot = 1;
   psd->ord = 1;
   psd->anim = 0;
}


/* --------------------------------------------------------------------------
 Set the default values for the shutdown timings.
- Parameters -------------------------------------------------------------
 PSDTIMING psdt : shutdown timing data structure.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _stlrSetShutdownTimingDefaults(PSDTIMING psdt) {
   psdt->lkupSusp = 8000;
   psdt->suspend = 0;
   psdt->warpCenter = 1000;
   psdt->unrollWindows = 1000;
   psdt->closeBox = 6000;
   psdt->sysShutdown = 3000;
   psdt->switchOffMsg = 2000;
   psdt->onPowerOff = 3000;
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
HBITMAP _stlrShadeHBmp(HPS hps, PSHDCREATE psc, ULONG fl,
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
HBITMAP _stlrHBmpFromBmpData(HPS hps, PBYTE pbmp, PBMPPAINT pbp, PBMPDAT pbd) {
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
HBITMAP _stlrHBmpFromModule(HPS hps, HMODULE hmod, ULONG id,
                            PBMPPAINT pbp, PBMPDAT pbd) {
   BITMAPINFOHEADER bih;
   HBITMAP hbmp;
   PBYTE pBmpData;
   // load the bitmap data (return NULLHANDLE in case of error)
   if (DosGetResource(hmod, RT_BITMAP, id, (PPVOID)&pBmpData))
      return NULLHANDLE;
   hbmp = _stlrHBmpFromBmpData(hps, pBmpData, pbp, pbd);
   DosFreeResource(pBmpData);
   return hbmp;
}

/* --------------------------------------------------------------------------
 Dummy procedure.
- Parameters -------------------------------------------------------------
 HPS hps      : presentation space handle.
 HBITMAP hbmp : bitmap handle to be made global.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
BOOL _stlrMakeGlobalBitmap(HPS hps, HBITMAP hbmp) {
    return TRUE;
}


/* --------------------------------------------------------------------------
 Dummy procedure.
- Parameters -------------------------------------------------------------
 HPS hps        : presentation space handle.
 PHBITMAP phbmp : global bitmap to be freed.
 PID pid        : id of the process to set as new bitmap owner.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _stlrFreeGlobalBitmap(HPS hps, PHBITMAP phbmp, PID pid) {
}

#endif
