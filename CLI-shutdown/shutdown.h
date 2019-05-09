/* --------------------------------------------------------------------------
 shutdown.h : command line interface shutdonw for eStyler

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------
 RETURNCODES       program return codes.
 IMPORTPROC        imported procedure definition.
 TEXTMESSAGES      error and text messages.
-------------------------------------------------------------------------- */



#ifndef _CLI_SHUTDOWN_H_
   #define _CLI_SHUTDOWN_H_
   #define INCL_DOSFILEMGR
   #define INCL_DOSMODULEMGR
   #define INCL_DOSMISC
   #define INCL_WIN

   #include <os2.h>
   #include <stdlib.h>
   #include <stdio.h>
   #include <string.h>

   #include "..\headers\stlrDefs.h"
   #include "..\headers\stlrShutdown.h"
   #include "..\headers\stlrStrings.h"


/* --------------------------------------------------------------------------
 RETURNCODES       program return codes.
*/
#define RC_CLISD_SUCCESS              0  // success
#define RC_CLISD_GETOBJWINHANDLE      2  // failed to get worker thread handle
#define RC_CLISD_INVALIDPARMS         4  // invalid parameters
#define RC_CLISD_ERRPOSTMSG           6  // failed to post shutdown request


/* --------------------------------------------------------------------------
 IMPORTPROC        imported procedure definition.
                   Get the handle of the worker thread object window of Styler/2.
*/
typedef HWND (STLR_GETWORKERHWND_FN)(VOID);
typedef STLR_GETWORKERHWND_FN * PSTLR_GETWORKERHWND_FN;
#define IPROC_GETWORKERHWND  5


/* --------------------------------------------------------------------------
 HELP              online help is implemented by displaying the proper page
                   of estyler.hlp.
*/
#define SZ_HELPPANEL    "SHUTDOWN.EXE"

/* --------------------------------------------------------------------------
 TEXTMESSAGES      error and text messages.
*/
#define SZERR_WORKERHANDLE \
" Failed to get the eStyler worker thread object window handle.\r\n"
#define SZERR_INVALIDPARMS \
" Invalid parameters. Type \"SHUTDOWN /? for more details.\r\n"
#define SZERR_POSTMSG \
" Failed to post the shutdown request to the eStyler worker thread.\r\n"

#endif // #ifndef _CLI_SHUTDOWN_H_
