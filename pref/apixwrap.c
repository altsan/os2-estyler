#define INCL_DOS
#define INCL_DOSMISC
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#define INCL_WIN
#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "apixwrap.h"

ULONG ulsem;

//==========================================================================\
// Subsystem Thread Safe realloc                                            |
//==========================================================================/
PVOID EXPENTRY tsrealloc(PVOID pv, ULONG cb)
{
    if (!cb) return NULL;
    while (lockxchng(&ulsem, 1)) DosSleep(1);
    pv = realloc(pv, cb);
    ulsem = 0;
    return pv;
}


//==========================================================================\
// Subsystem Thread Safe strdup                                             |
//==========================================================================/
PSZ EXPENTRY tsstrdup(PCSZ pcsz)
{
   PSZ psz;
   if (!pcsz) return NULL;
   while (lockxchng(&ulsem, 1)) DosSleep(1);
   psz = strdup(pcsz);
   ulsem = 0;
   return psz;
}


//==========================================================================\
// Open and read file contents into a double-null terminated buffer         |
//==========================================================================/
PSZ EXPENTRY gpReadFile(PSZ pszFile)
{
    FILESTATUS3 fs;
    HFILE hf;
    ULONG ul;
    PSZ psz;

    if (DosOpen(pszFile,  &hf, &ul, 0, FILE_NORMAL,
                OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                OPEN_FLAGS_SEQUENTIAL | OPEN_SHARE_DENYWRITE |
                OPEN_ACCESS_READONLY, NULL))
        goto error0;
    if (DosQueryFileInfo(hf, FIL_STANDARD, &fs, sizeof(fs))) goto error1;
    if (!(psz = (PSZ)tsmalloc(fs.cbFile + 2))) goto error1;
    if (DosRead(hf, psz, fs.cbFile, &ul)) goto error2;
    if (DosClose(hf)) goto error2;

    *(psz + fs.cbFile) = 0;
    *(psz + fs.cbFile + 1) = 0;
    return psz;

    error2: tsfree(psz);
    error1: DosClose(hf);
    error0: return NULL;
}


//==========================================================================\
// Free file buffer                                                         |
//==========================================================================/
VOID EXPENTRY gpFreeReadFile(PSZ pszFileContent)
{
    tsfree(pszFileContent);
}


//==========================================================================\
// Wrapper for WinStartApp with simplified flags for default, full-screen,  |
// maximized or hidden mode.                                                |
//==========================================================================/
HAPP EXPENTRY gpStartApp(HWND hwnd, PSZ pszAppl, PSZ pszParm,
                         PSZ pszPath, PSZ pszEnv, ULONG fl )
{
    PROGDETAILS prgdet;
    memset(&prgdet, 0, sizeof(PROGDETAILS));
    prgdet.Length          = sizeof(PROGDETAILS);
    prgdet.progt.fbVisible = SHE_VISIBLE;
    prgdet.pszTitle        = NULL;
    prgdet.pszExecutable   = pszAppl;
    prgdet.pszParameters   = pszParm;
    prgdet.pszStartupDir   = pszPath;
    prgdet.pszIcon         = NULL;
    prgdet.pszEnvironment  = pszEnv;

    switch( fl ) {
        default:
        case GPSTART_DEFAULT:
            prgdet.swpInitial.hwndInsertBehind = HWND_TOP;
            prgdet.progt.progc = PROG_DEFAULT;
            prgdet.swpInitial.fl = SWP_SHOW | SWP_ZORDER;
            break;
        case GPSTART_FULLSCREEN:
            prgdet.swpInitial.hwndInsertBehind = HWND_TOP;
            prgdet.progt.progc = PROG_FULLSCREEN;
            prgdet.swpInitial.fl = SWP_SHOW | SWP_ZORDER;
            break;
        case GPSTART_HIDDEN:
            prgdet.swpInitial.hwndInsertBehind = HWND_BOTTOM;
            prgdet.progt.progc = PROG_DEFAULT;
            prgdet.swpInitial.fl = SWP_HIDE | SWP_MINIMIZE | SWP_ZORDER;
            break;
        case GPSTART_MAX:
            prgdet.swpInitial.hwndInsertBehind = HWND_TOP;
            prgdet.progt.progc = PROG_DEFAULT;
            prgdet.swpInitial.fl = SWP_SHOW | SWP_MAXIMIZE | SWP_ZORDER;
            break;
    }
    return WinStartApp(hwnd, &prgdet, NULL, NULL,
                       SAF_INSTALLEDCMDLINE | SAF_STARTCHILDAPP);
}

