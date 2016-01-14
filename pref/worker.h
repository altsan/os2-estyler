/* --------------------------------------------------------------------------
 worker.h :

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */



#ifndef _WORKER_JOBS_H_
   #define _WORKER_JOBS_H_

/* --------------------------------------------------------------------------
 WORKERTHREAD      worker thread data structures and definitions.
*/

// worker thread function
typedef ULONG (THREADJOBFN)(PVOID);
typedef THREADJOBFN * PTHREADJOBFN;

// delete job function
typedef VOID (DELJOBFN)(PVOID);
typedef DELJOBFN * PDELJOBFN;

// worker thread job
typedef struct _WORKTHREADJOB WORKTHREADJOB;
typedef WORKTHREADJOB * PWORKTHREADJOB;

struct _WORKTHREADJOB {
   PWORKTHREADJOB pNext;  // next job to be executed
   HWND hwndNotify;       // window which must be notified of the job completion
   ULONG jobID;           // job identifier
   PTHREADJOBFN pFunc;    // procedure which must be executed by the worker thread
   PDELJOBFN pDelFunc;    // to free the 'pData' resources (see below)
   PVOID pData;           // data associated with the worker thread job
} ;

typedef struct {
   TID tid;                   // worker thread handle
   HEV hev;                   // event semaphore
   PWORKTHREADJOB pCurJob;    // current job data pointer
   PWORKTHREADJOB pLastJob;   // last job in the list
   BOOL bTerminate;           // if TRUE terminate the application
} WORKERTHREAD, * PWORKERTHREAD;

#define STLRM_WORKERTHREADJOBEND        (WM_USER + 100)


/* --------------------------------------------------------------------------
 BMPFILELIST       structure used to get a list of the files of a given path.
*/
typedef struct _BMPFILEITEM BMPFILEITEM;
typedef BMPFILEITEM * PBMPFILEITEM;

struct _BMPFILEITEM {
   PBMPFILEITEM pnext;            // next item
   CHAR achFile[4];               // bitmap file name (variable size array)
};

// bitmap limits (value set to 0 mean ignore i.e no limit)
typedef struct {
   ULONG cMaxClrs;            // maximum colors as bits per pixel.
   ULONG cxMax;               // maximum bitmap width.
   ULONG cyMax;               // maximum bitmap height.
} BMPLIMITS, * PBMPLIMITS;

typedef struct {
   CHAR achPath[CCHMAXPATH];  // base path and file mask
   PSZ pszCurBmpFile;         // currently selected bitmap file
   PBMPFILEITEM pBmpFile;     // linked list of BMPFILEITEM structures
   BMPLIMITS limit;           // limits to consider the bitmap valid
} LISTBMPFILES, * PLISTBMPFILES;

#define ERROR_LISTBMPFILES               0xffffffff

/* --------------------------------------------------------------------------
 APPLYBMP          structure used to apply a new bitmap.
*/
typedef struct {
   PBMPFILEDATA pBmp;         // bitmap details (see main.h)
   PVOID pPrev;               // previous options
   PVOID pNew;                // new options
   CHAR achBmpFile[CCHMAXPATH]; // full bitmap file name
   ULONG cbData;              // size of bitmap data
   BYTE aBmpData[4];          // bitmap data
} APPLYBMP, * PAPPLYBMP;

// apply-bmp return codes
#define APPLYBMP_SUCCESS            0   // success
#define APPLYBMP_ERR_READFILE       1   // failed to read the file
#define APPLYBMP_ERR_INVALIDFILE    2   // not a bitmap file
#define APPLYBMP_ERR_TOOLARGE       3   // the bitmap is too large
#define APPLYBMP_ERR_SAVEDATA       4   // fail to save data to ini file

/* --------------------------------------------------------------------------
 GETBMPDATA        structure used to get the data of a bitmap via the
                   worker thread.
*/
typedef struct {
   PBMPFILEDATA pBmp;         // bitmap details (see main.h)
   CHAR achFile[CCHMAXPATH];  // full bitmap file name
   BMPPAINT bmpp;             // bitmap handle and bitmap size
   PVOID pOpts;               // options address
   ULONG cbData;              // size of the bitmap data buffer
   BYTE ab[4];                // optional, variable size - bitmap data buffer
} GETBMPDATA, * PGETBMPDATA;

// get-bmp-data return codes
#define GETBMP_SUCCESS              0   // success
#define GETBMP_ERR_HPS              1   // failed to get HPS
#define GETBMP_ERR_READFILE         2   // failed to read the bitmp file
#define GETBMP_ERR_BMPHANDLE        3   // failed to get the bitmap handle

/* --------------------------------------------------------------------------
 ADDBMP            structure used to add a bitmap file (for titlebars, buttons)
*/
typedef struct {
   CHAR achBmpFile[CCHMAXPATH];    // bitmap file to be imported
   CHAR achDestPath[CCHMAXPATH];   // destination path
   CHAR achFileName[CCHMAXPATHCOMP]; // name of the imported file
   BMPLIMITS limits;               // max bitmap width/height/color depth
} ADDBMPFILE, * PADDBMPFILE;
// add-bmp-file return codes
#define ADDBMPF_SUCCESS              0   // success
#define ADDBMPF_ERR_INVALID          1   // invalid bitmap file
#define ADDBMPF_ERR_COPYFILE         2   // failed to copy the file
#define ADDBMPF_ERR_SOURCEQDEST      3   // sourcePath == destPath


/* --------------------------------------------------------------------------
 GETBOOTLIST       structures used to get the list of bootable volumes.
*/
typedef struct {
   HWND hwndWaitMsg;          // handle of the wait dlg box
   ULONG cVols;               // volume count
   CHAR achVol[26][21];       // array of volume names
} GETBOOTLIST, * PGETBOOTLIST;


/* --------------------------------------------------------------------------
 JOBIDS            ID of the jobs executed via the worker thread.
*/
#define STLRWID_LISTBMPFILES             1
#define STLRWID_APPLYBMP                 2
#define STLRWID_GETBMPDATA               3
#define STLRWID_ADDBMP                   4
#define STLRWID_GETBOOTLIST              5

#endif // _WORKER_JOBS_H_
