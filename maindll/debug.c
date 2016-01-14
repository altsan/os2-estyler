/* --------------------------------------------------------------------------
 debug.c : write debug information into a log file.
 2004/05/08 - Alessandro Cantatore - Italy
-------------------------------------------------------------------------- */


#pragma strings(readonly)

// includes -----------------------------------------------------------------
#include <stdarg.h>
#include "main.h"

// global variables ---------------------------------------------------------
extern PROCDATA pd;

// definitions --------------------------------------------------------------



// prototypes ---------------------------------------------------------------



#ifdef DEBUGGING

/* --------------------------------------------------------------------------
 Initialize the debugging environment:
 - make a backup copy of the old debug log file.
 - open the new debug log file.
 - write the log file header.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _dbgInit(VOID) {
   ULONG ula;
   CHAR buf[80];
   DosCopy("estlrdbg.log", "estlrdbg.000", DCPY_EXISTING);
   if (DosOpen("estlrdbg.log", &hf, &ula, 0L, FILE_NORMAL,
       OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_REPLACE_IF_EXISTS,
       OPEN_SHARE_DENYNONE | OPEN_ACCESS_READWRITE, NULL)) {
      DosBeep(440, 440);
      return;
   }
   logwrite(hf, "------------- Logging started -------------\n");
   strcpy(buf, "00/00/00 - 00:00:00");
   stlrDateTime(buf);
   buf[19] = 0;
   logwrite(hf, buf);
   sprintf(buf, " - eStylerLite Version %d.%d.%d\n",
           g.version.major, g.version.minor, g.version.build);
   logwrite(hf, buf);
}


/* --------------------------------------------------------------------------
 Debug termination (write ending line and close the file).
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _dbgEnd(VOID) {
   logwrite(hf, "-------------- Logging ended --------------\n");
   DosClose(hf);
}


/* --------------------------------------------------------------------------
 Write a formatted string to a 300 bytes buffer and add it to the debugging
 log file.
- Parameters -------------------------------------------------------------
 PSZ psz : string with formatting informations.
 ...     : optional parameters.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _dbgPrintf(PSZ psz, ...) {
   CHAR buf[300];
   va_list ap;
   if (psz) {
      va_start(ap, psz);
      vsprintf(buf, psz, ap);
      va_end(ap);
      logwrite(hf, buf);
   } /* endif */
}
#endif // DEBUGGING
