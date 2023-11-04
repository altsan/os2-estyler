/* --------------------------------------------------------------------------
 worker.c : worker thread module.

 2004-- - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------


// prototypes ---------------------------------------------------------------
static VOID EXPENTRY workerThreadProc(ULONG ul);

// global variables ---------------------------------------------------------


// entry point --------------------------------------------------------------

/* --------------------------------------------------------------------------
 Create the event semaphore used to communicate with the worker thread
 and start the worker thread.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
BOOL startWorkerThread(VOID) {
   if (DosCreateEventSem(NULL, &g.worker.hev, 0L, FALSE))
      return handleError(ISERR_WORKERSEMAPHORE, FALSE);
   g.state |= STLRIS_SEMAPHOREOK;
   if (DosCreateThread(&g.worker.tid, (PFNTHREAD)workerThreadProc, 0,
                        CREATE_READY | STACK_SPARSE, 0x4000))
      return handleError(ISERR_WORKERTHREAD, FALSE);
   DosSleep(1);
   return TRUE;
}


/* --------------------------------------------------------------------------
 Worker thread.
- Parameters -------------------------------------------------------------
 ULONG ul : ignored.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID EXPENTRY workerThreadProc(ULONG ul) {
   ULONG ulPost;
   ULONG ulReturn;
   DosSetPriority(PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, 31, 0);
   // infinite loop
   for (;;) {
      DosWaitEventSem(g.worker.hev, SEM_INDEFINITE_WAIT);
      // program termination
      if (g.worker.bTerminate) {
         WinPostMsg(g.appl.hwnd, WM_QUIT, MPVOID, MPVOID);
         break;
      } /* endif */
      // reset the event semaphore
      DosResetEventSem(g.worker.hev, &ulPost);
      // calls the procedure stored in the global structure
      ulReturn = g.worker.pCurJob->pFunc(g.worker.pCurJob->pData);
      // signals to the main window procedure that the procedure has been
      // executed
      WinPostMsg(g.worker.pCurJob->hwndNotify, STLRM_WORKERTHREADJOBEND,
                 (MPARAM)g.worker.pCurJob->jobID, (MPARAM)ulReturn);
   } /* endfor */
}


/* --------------------------------------------------------------------------
 Allocate a WORKTHREADJOB structure, initialize it and signal the worker
 thread to execute the associated procedure.
- Parameters -------------------------------------------------------------
 HWND hwndNotfy     : window which must be notified of the job completion.
 ULONG jobID        : job identifier.
 PTHREADJOBFN pFunc : procedure which must be executed by the worker thread.
 PDELJOBFN pDelFunc : procedure which must be called to free the resources
                      associated with the current job (it gets pData as
                      parameter).
                      This is needed as all pending jobs are automatically
                      deleted when the program is terminated by the user.
 PVOID pData        : data associated with the worker thread job.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error).
-------------------------------------------------------------------------- */
BOOL workerJobAdd(HWND hwndNotify, ULONG jobID,
                  PTHREADJOBFN pFunc, PDELJOBFN pDelFunc, PVOID pData) {
   PWORKTHREADJOB pwtj;
   if (NULL == (pwtj = malloc(sizeof(WORKTHREADJOB))))
      return handleError(ISERR_ALLOCATION, FALSE);
   pwtj->pNext = NULL;
   pwtj->hwndNotify = hwndNotify;
   pwtj->jobID = jobID;
   pwtj->pFunc = pFunc;
   pwtj->pDelFunc = pDelFunc;
   pwtj->pData = pData;
   if (!g.worker.pCurJob) {
      g.worker.pCurJob = g.worker.pLastJob = pwtj;
      // if the worker thread is not executing any job post the semaphore
      // otherwise the semaphore will be posted when the current job is
      // completed (see workerJobUnlink() )
      DosPostEventSem(g.worker.hev);
   } else {
      g.worker.pLastJob->pNext = pwtj;
      g.worker.pLastJob = pwtj;
   } /* endif */
   return TRUE;
}


/* --------------------------------------------------------------------------
 Delete a completed worker thread job from the list.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID workerJobUnlink(VOID) {
   PWORKTHREADJOB pjob;
   pjob = g.worker.pCurJob;
   // free the resources previously allocated for the job
   if (pjob->pDelFunc) pjob->pDelFunc(pjob->pData);
   // unlink the job
   g.worker.pCurJob = pjob->pNext;
   // if there is any other job to be executed post the event semaphore
   if (g.worker.pCurJob) {
      DosPostEventSem(g.worker.hev);
   // if there are no other job left reset the value of the last job pointer
   } else {
      g.worker.pLastJob = NULL;
   } /* endif */
   free(pjob);
}
