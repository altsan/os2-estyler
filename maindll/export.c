/* --------------------------------------------------------------------------
 export.c : procedure exported to be used by the preferences applications
 2004/05/08 - Alessandro Cantatore - Italy

-------------------------------------------------------------------------- */

#pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// prototypes ---------------------------------------------------------------

// global variables ---------------------------------------------------------

/* --------------------------------------------------------------------------


/* --------------------------------------------------------------------------
 Return the current screen resolution and color depth.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 PSCRRES address of the screen resolution/color depth structure.
-------------------------------------------------------------------------- */
PSCRRES stlrQryScreenRes(VOID) {
   return &g.scr;
}


/* --------------------------------------------------------------------------
 Return the current eStylerLite version number
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 PSTLRVER address of the structure containing the version details.
-------------------------------------------------------------------------- */
PSTLRVER stlrGetVer(VOID) {
   return &g.version;
}


/* --------------------------------------------------------------------------
 Return the address of the structure holding the overall options.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 POPTIONS address of the structure holding the current eStylerLite options
-------------------------------------------------------------------------- */
POPTIONS stlrGetOptionsDataAddress(VOID) {
   return &o;
}


/* --------------------------------------------------------------------------
 Return the handle of the object window running in the worker thread.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 HWND handle of the object window.
-------------------------------------------------------------------------- */
HWND stlrGetWorkerThreadHandle(VOID) {
   return g.worker.hwnd;
}
