/* --------------------------------------------------------------------------
 utils.c : utility functions.
 2004/05/08 - Alessandro Cantatore - Italy
-------------------------------------------------------------------------- */

#pragma strings(readonly)

#include "main.h"


// prototypes ---------------------------------------------------------------
HFILE stlrOpenLog(VOID);


/* --------------------------------------------------------------------------
 Write data to the log file.
- Parameters -------------------------------------------------------------
 HFILE hf : log file handle.
 PSZ psz  : data to be written to the file.
- Return value -----------------------------------------------------------
 ULONG length of written data in bytes.
-------------------------------------------------------------------------- */
ULONG logwrite(HFILE hf, PSZ psz) {
   ULONG ulWrote;
   DosWrite(hf, psz, strlen(psz), &ulWrote);
   return ulWrote;
}


/* --------------------------------------------------------------------------
 Open the log file.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 HFILE file handle or ((HFILE)-1) in case of error.
-------------------------------------------------------------------------- */
HFILE stlrOpenLog(VOID) {
   ULONG ul;
   CHAR buf[16];
   HFILE hf = 0;
   buf[0] = g.is.bootdrv == 'A'? 'B': g.is.bootdrv;
   strcpy(&buf[1], SZ_LOGFILE);
   if (DosOpen(buf, &hf, &ul, 0L, FILE_NORMAL,
               OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
               OPEN_SHARE_DENYNONE | OPEN_ACCESS_WRITEONLY, NULL) ||
       DosSetFilePtr(hf, 0, FILE_END, &ul)) {
      if (hf) DosClose(hf);
      return (HFILE)-1;
   } // end if
   return hf;
}


/* --------------------------------------------------------------------------
 Write to the buffer 'psz' current date and time.
- Parameters -------------------------------------------------------------
 PSZ psz: buffer.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
BOOL stlrDateTime(PSZ psz) {
   COUNTRYCODE cc = {0, 0};
   COUNTRYINFO ci;
   DATETIME dt;
   ULONG ul = 0;
   // rileva informazioni relative formato dati secondo nazionalit…
   memset(&ci, 0, sizeof(COUNTRYINFO));
   if (DosQueryCtryInfo(sizeof(COUNTRYINFO), &cc, &ci, &ul))
      return FALSE;
   DosGetDateTime(&dt);
   psz[2] = psz[5] = *ci.szDateSeparator;
   psz[11] += dt.hours / 10;
   psz[12] += dt.hours % 10;
   psz[13] = psz[16] = *ci.szTimeSeparator;
   psz[14] += dt.minutes / 10;
   psz[15] += dt.minutes % 10;
   psz[17] += dt.seconds / 10;
   psz[18] += dt.seconds % 10;
   dt.year %= 100;
   switch (ci.fsDateFmt) {
      case(1):                                         /* dd/mm/yy */
         psz[0] += dt.day / 10; psz[1] += dt.day % 10;
         psz[3] += dt.month / 10; psz[4] += dt.month % 10;
         psz[6] += dt.year / 10; psz[7] += dt.year % 10;
         break;
      case(2):                                         /* yy/mm/dd */
         psz[0] += dt.year / 10; psz[1] += dt.year % 10;
         psz[3] += dt.month / 10; psz[4] += dt.month % 10;
         psz[6] += dt.day / 10; psz[7] += dt.day % 10;
         break;
      default:                                         /* mm/dd/yy */
         psz[0] += dt.month / 10; psz[1] += dt.month % 10;
         psz[3] += dt.day / 10; psz[4] += dt.day % 10;
         psz[6] += dt.year / 10; psz[7] += dt.year % 10;
         break;
   } /* endswitch */
   return TRUE;
}


/* --------------------------------------------------------------------------
 Write an error message to the log file.
- Parameters -------------------------------------------------------------
 ULONG ulmsg : message id.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
BOOL stlrlog(ULONG ulmsg) {
   HFILE hf;
   CHAR ach[32];
   if (!ulmsg) return TRUE;
   if (-1 == (INT)(hf = stlrOpenLog())) goto err0;
   strcpy(ach, " 00/00/00 - 00:00:00 - #000\n");
   if (!stlrDateTime(ach + 1)) goto err1;
   ach[24] = '0' + ulmsg / 100;
   ach[25] = '0' + (ulmsg % 100) / 10;
   ach[26] = '0' + ulmsg % 10;
   if (!logwrite(hf, ach)) goto err1;
   DosClose(hf);
   return TRUE;
err1:
   DosClose(hf);
err0:
   DosBeep(440, 500);
   return FALSE;
}


/* --------------------------------------------------------------------------
 Compare an executable name (optionally including the path specification)
 with a simple executable file name (not including the path specifications).
- Parameters -------------------------------------------------------------
 PSZ pszexe: executable name (optionally including path specification).
 PSZ pszcmp: executable name to be compared with 'pszexe'.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (matching/mismatching executable names).
-------------------------------------------------------------------------- */
BOOL matchExeName(PSZ pszexe, PSZ pszcmp) {
   PSZ psz = strrchr(pszexe, '\\');
   if (!psz) psz = pszexe;
   else ++psz;
   return !strcmp(psz, pszcmp);
}


/* --------------------------------------------------------------------------
 DosQuerySysInfo() API wrapper.
- Parameters -------------------------------------------------------------
 ULONG fl: requested system information.
- Return value -----------------------------------------------------------
 ULONG : requested value.
-------------------------------------------------------------------------- */
ULONG sysInfo(ULONG fl) {
   DosQuerySysInfo(fl, fl, &fl, sizeof(ULONG));
   return fl;
}


/* --------------------------------------------------------------------------
 Draw a 3D border within the rectangle pr the bottom/left corner of the
 rectangle is moved up and right of 'cpBorder' pixels, while the upper/right
 corner is moved down and left, so the resulting rectangle width and height
 are decreased of cpBorder * 2 pixels.
- Parameters -------------------------------------------------------------
 HPS hps     : presentation space handle
 PRECTL pr   : address of rectangle to be drawn with a 3D border
 ULONG clrul : upper-left border color
 ULONG clrbr : bottom-right border color
 ULONG cpbrd : border thickness in pixels
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID draw3Dborder(HPS hps, PRECTL pr, LONG clrul, LONG clrbr, ULONG cpBorder) {
   POINTL apt[2];
   while (cpBorder--) {
      // bordo sinistro e superiore
      GpiSetColor(hps, clrul);
      PointSet(apt, pr->xLeft, pr->yBottom);
      GpiMove(hps, apt);
      PointSet(apt, pr->xLeft, pr->yTop);
      PointSet(apt + 1, pr->xRight, pr->yTop);
      GpiPolyLine(hps, 2, apt);
      // bordo destro e inferiore
      GpiSetColor(hps, clrbr);
      PointSet(apt, apt[1].x, apt[1].y);
      GpiMove(hps, apt);
      PointSet(apt, pr->xRight, pr->yBottom);
      PointSet(apt + 1, pr->xLeft + 1, pr->yBottom);
      GpiPolyLine(hps, 2, apt);
      RectInflate(pr, -1, -1);
   } /* endfor */
}
