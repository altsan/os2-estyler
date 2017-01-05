/* --------------------------------------------------------------------------
 shutdown.c: shutdown routines.
-------------------------------------------------------------------------- */


#pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"
#include "error.h"
#include "stlrShutdown.h"
#include "resID.h"
#include "stlrExport.h"

#ifdef ACPI_ENABLED
    #include "acpi.h"
    #include "acpiapi.h"
#endif

// definitions --------------------------------------------------------------
#define mSetRadioBtn(_h_, _id_, _fl_, _ppt_, _y_, _dy_, _cy_) \
   (_fl_) ?                                                     \
      ((_dy_) ?                                                  \
         setCtrlPos((_h_), (_id_), (_ppt_), 10, (_y_) - (_dy_), 0, 0, SWP_MOVE)\
         :                                                                     \
         0)                                                                    \
      :                                                                        \
      (DlgItemShow((_h_), (_id_), FALSE), (_dy_) += 12, (_cy_) -= 12)

// prototypes ---------------------------------------------------------------
PREBOOTLIST getRebootList(VOID);
ULONG selectShutdownMode(HAB hab, PDOSHUTDOWN p);
static MRESULT EXPENTRY shutdownScreenProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static VOID animateShutdownScreen(HWND hwnd, PDOSHUTDOWN p);
static VOID blockMouse(HWND hwnd, INT x, INT y);
static MRESULT EXPENTRY shutdownSelProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static MRESULT initShutdownSelDlg(HWND hwnd, PDOSHUTDOWN p);
static INT formatShutdownSelControls(HWND hwnd, PDOSHUTDOWN p);
static BOOL setCtrlPos(HWND hwnd, ULONG id, PPOINTL ppt,
                LONG x, LONG y, LONG cx, LONG cy, ULONG fl);
static VOID handleCommand(HWND hwnd, ULONG idCmd);
static BOOL fillRebootList(HWND hwnd, PDOSHUTDOWN p);
static VOID handleCtrlMsgs(HWND hwnd, ULONG idCtrl, ULONG idEvent, HWND hCtrl);
static VOID runSysClosingBox(HAB hab, PDOSHUTDOWN p);
static MRESULT EXPENTRY sysTermMsgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID ForceCommitFn( PFN pAddress );

// global variables ---------------------------------------------------------

/* --------------------------------------------------------------------------
 Shutdown thread.
- Parameters -------------------------------------------------------------
 ULONG mode : shutdown mode. This is not 0 when the thread is started to
              form the command line to perform an unattended shutdown.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID _System shutdownThread(ULONG mode) {
   HAB hab;
   HMQ hmq;
   DOSHUTDOWN dsd;
dbgPrintf2("*** shutdown thread creation - mode: %08x\n", mode);
   // get the list of bootable volumes from estyler.ini (error not critical)
   dsd.pRebootList = getRebootList();
   // initialization
   hmq = WinCreateMsgQueue((hab = WinInitialize(0)), 0);
   WinCancelShutdown(hmq, TRUE);
   if (NO_ERROR == DosLoadModule(NULL, 0, SZ_RESMODULE, &dsd.hModRes)) {
      // attended shutdown : display the shutdown screen and selection dialog
      if (mode == SDFL_DLG)
         mode = selectShutdownMode(hab, &dsd);
dbgPrintf2("*** shutdown mode: %08x\n", mode);
      if (mode & SDFL_OKMASK) {
//         g.achsd[0] = '0' + (mode & SDFL_OKMASK) - 1, g.achsd[1] = 0;
         g.achsd[0] = '0' + (mode & SDFL_OKMASK) - 1;
         switch (mode & SDFL_OKMASK) {
            case SDFL_LOCKSUSP:
               WinStartTimer(g.worker.hab, g.worker.hwnd, TID_DELLKSUSP,
                             o.sd.wait.lkupSusp);
               break;
            case SDFL_SUSP:
               WinStartTimer(g.worker.hab, g.worker.hwnd, TID_DELLKSUSP,
                             o.sd.wait.suspend);
               break;
            case SDFL_REBOOT:
               if ((mode & SDFL_REBOOTIDX)
                   &&
                   dsd.pRebootList
                   &&
                   ((mode >> 16) < dsd.pRebootList->ci))
                  strcpy(&g.achsd[1], dsd.pRebootList->aSetBootParm[mode >> 16]);
               // fall through the next cases
            case SDFL_OFF:
            case SDFL_SHUTDOWN:
               o.sd.adv.runProgOnce = ((mode & SDFL_EXECUTE) > 0);
               runSysClosingBox(hab, &dsd);
               break;
         } /* endswitch */
      } /* endif */
      DosFreeModule(dsd.hModRes);
   } else {
      mode = SDFL_ERROR;
   } /* endif */
   // signal completion of the shutdown initialization
   m_postWorkerMsg(STLRM_RESETSHUTDOWNFL, g.worker.hwnd, 0);
   // in case of error execute the minimal shutdown dialog
   if (mode & SDFL_ERROR) {
      m_postWorkerMsg(STLRM_SHUTDOWNERR, g.worker.hwnd, 0);
   // complete the lockup and suspend feature
   } else if ((mode & SDFL_OKMASK) == SDFL_LOCKSUSP) {
      WinLockupSystem(hab);
   } /* endif */
   memLkFree(dsd.pRebootList);
   WinDestroyMsgQueue(hmq);
   WinTerminate(hab);
}


/* --------------------------------------------------------------------------
 Get the list of the bootable volumes stored in eStylerIni.
 The format of each list item is:
 <volumeName>\t<volumeDescription>\x00
 Each item include a volume name followed by a tab character and the
 volume description which is NUL terminated.
 A void item (i.e. an item with no volume name and description, just a NUL
 character) marks the end of the list.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 PREBOOTLIST : reboot list data or NULL in case of error.
-------------------------------------------------------------------------- */
PREBOOTLIST getRebootList(VOID) {
   HINI hini;
   ULONG ul;
   PSZ psz;
   PREBOOTLIST pRebootList = NULL;
   // open estyler.ini and allocate storage to read the bootable volumes list
   if (!(hini = stlrOpenProfile())) return NULL;
   if (!PrfQueryProfileSize(hini, SZPRO_SHUTDWON, SZPRO_SDWNLIST, &ul))
      goto exit_0;
   if (NULL == (pRebootList = memLkAlloc(ul + sizeof(REBOOTLIST) - 4)))
      goto exit_0;
   // read data from estyler.ini
   if (!PrfQueryProfileData(hini, SZPRO_SHUTDWON, SZPRO_SDWNLIST,
                            pRebootList->ablist, &ul))
      goto exit_1;
   for (ul = 0, psz = pRebootList->ablist; *psz; ul++) {
      // store the SETBOOT parameter
      pRebootList->aSetBootParm[ul] = psz;
      if (!(psz = strchr(psz, '\t'))) goto exit_1;
      *psz = 0;
      // store the volume long name (description)
      pRebootList->aVolDescr[ul] = ++psz;
      // get to the next entry
      psz += strlen(psz) + 1;
   } /* endfor */
   if (!ul)
      goto exit_1;
   pRebootList->ci = ul;
   goto exit_0;
exit_1:
   memLkFree(pRebootList);
   pRebootList = NULL;
exit_0:
   PrfCloseProfile(hini);
   return pRebootList;
}


/* --------------------------------------------------------------------------
 Attended shutdown request. Display the dialog to ask the user what kind
 of shutdown he intends to perform (suspend/power off/shutdown/reboot/etc).
- Parameters -------------------------------------------------------------
 HAB hab       : anchor block handle.
 PDOSHUTDOWN p : shutdown selection data structure.
- Return value -----------------------------------------------------------
 ULONG flag describing the user selection.
-------------------------------------------------------------------------- */
ULONG selectShutdownMode(HAB hab, PDOSHUTDOWN p) {
   HWND hwnd;
   QMSG qmsg;
   // initialize the shutdown selection dialog data
   p->flMode = SDFL_ERROR;
   p->r.xLeft = p->r.xRight = g.scr.cx >> 1;
   p->r.yBottom = p->r.yTop = g.scr.cy >> 1;
   // register the shutdown background window class and create the window
   if (WinRegisterClass(hab, WC_SHDWNSCRN,(PFNWP)shutdownScreenProc,
                        CS_HITTEST, 4)) {
       hwnd = WinCreateWindow(HWND_DESKTOP, WC_SHDWNSCRN, NULL,
                              WS_VISIBLE, 0, 0, g.scr.cx, g.scr.cy,
                              NULLHANDLE, HWND_TOP, 0, p, NULL);
dbgPrintf2("*** selectShutdownMode - hwnd = %08x\n", hwnd);
      if (hwnd) {
         while (WinGetMsg(hab, &qmsg, NULLHANDLE, 0, 0))
            WinDispatchMsg(hab, &qmsg);
         WinDestroyWindow(hwnd);
      } /* endif */
   } /* endif */
   return p->flMode;
}


/* --------------------------------------------------------------------------
 Shutdown selection background screen window procedure.
- Parameters -------------------------------------------------------------
 standard window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT
-------------------------------------------------------------------------- */
static
MRESULT EXPENTRY shutdownScreenProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_CREATE:
         WinSetWindowPtr(hwnd, QWL_USER, mp1);
         WinPostMsg(hwnd, STLRM_STARTANIM, mp1, MPVOID);
         break;
      case STLRM_STARTANIM:
         animateShutdownScreen(hwnd, (PDOSHUTDOWN)mp1);
         break;
      case WM_HITTEST:
         blockMouse(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1));
         break;
      default:
         return WinDefWindowProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}


/* --------------------------------------------------------------------------
 Overlay a pattern over the screen.
- Parameters -------------------------------------------------------------
 HWND hwnd : shutdown background window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID animateShutdownScreen(HWND hwnd, PDOSHUTDOWN p) {
   INT i, x0, y0, x1, y1, dx, dy;
   HPS hps;
   RECTL r, rClip;
   i = o.sd.gen.anim? 0: o.sd.gen.steps - 1;
   x0 = y0 = 0;
   x1 = g.scr.cx << 16;
   y1 = g.scr.cy << 16;
   dx = x1 / (o.sd.gen.steps * 2);
   dy = y1 / (o.sd.gen.steps * 2);
   RectSet(&r, 0, 0, g.scr.cx, g.scr.cy);
   DosSetPriority(PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, PRTYD_MAXIMUM, 0);
   if (NULLHANDLE != (hps = WinGetPS(hwnd))) {
      GpiSetPattern(hps, PATSYM_HALFTONE);
      while (++i <= o.sd.gen.steps) {
         WinSetPointerPos(HWND_DESKTOP, p->r.xLeft, p->r.yBottom);
         x0 += dx, y0 += dy, x1 -= dx, y1 -= dy;
         RectSet(&rClip, x0 >> 16, y0 >> 16, x1 >> 16, y1 >> 16);
         if (i < o.sd.gen.steps) GpiExcludeClipRectangle(hps, &rClip);
         GpiMove(hps, (PPOINTL)&r);
         GpiBox(hps, DRO_FILL, (PPOINTL)&r.xRight, 0, 0);
         GpiDestroyRegion(hps, GpiQueryClipRegion(hps));
         GpiSetClipRegion(hps, NULLHANDLE, NULLHANDLE);
      } /* endwhile */
      WinReleasePS(hps);
   } /* endif */
   WinAlarm(HWND_DESKTOP, WA_WARNING);
   DosSetPriority(PRTYS_THREAD, PRTYC_REGULAR, 0, 0);
   WinDlgBox(HWND_DESKTOP, hwnd, shutdownSelProc, p->hModRes, DLG_SHUTDOWNREQ, p);
   WinPostMsg(hwnd, WM_QUIT, MPVOID, MPVOID);
}


/* --------------------------------------------------------------------------
 Prevent mouse motion off the screen center or outside the shutdown
 selection dialog.
- Parameters -------------------------------------------------------------
 HWND hwnd : background window handle.
 INT x     : horizontal mouse coordinate.
 INT y     : vertical mouse coordinate.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID blockMouse(HWND hwnd, INT x, INT y) {
   PDOSHUTDOWN p;
   if (NULL != (p = WinQueryWindowPtr(hwnd, QWL_USER))) {
      if (x < p->r.xLeft) {
         x = p->r.xLeft;
      } else if (x > p->r.xRight) {
         x = p->r.xRight;
      } /* endif */
      if (y < p->r.yBottom) {
         y = p->r.yBottom;
      } else if (y > p->r.yTop) {
         y = p->r.yTop;
      } /* endif */
      WinSetPointerPos(HWND_DESKTOP, x, y);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Shutdown selection dialog window procedure.
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT
-------------------------------------------------------------------------- */
static
MRESULT EXPENTRY shutdownSelProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         return initShutdownSelDlg(hwnd, (PDOSHUTDOWN)mp2);
      case WM_COMMAND:
         handleCommand(hwnd, (ULONG)mp1);
         break;
      case WM_CONTROL:
         handleCtrlMsgs(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), (HWND)mp2);
         break;
      // prevent window motion
      case WM_TRACKFRAME:
         return MRTRUE;
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}


/* --------------------------------------------------------------------------
 Shutdown selection dialog - initialization procedure.
 Format the dialog showing/hiding dialog controls according to the current
 shutdown options.
- Parameters -------------------------------------------------------------
 HWND hwnd      : window handle.
 PDOSHUTDOWN p : shutdown data structure.
- Return value -----------------------------------------------------------
 MRESULT
-------------------------------------------------------------------------- */
static MRESULT initShutdownSelDlg(HWND hwnd, PDOSHUTDOWN p) {
   RECTL r;
   BYTE ab[] = {o.sd.gen.lksusp, o.sd.gen.susp, o.sd.gen.off,
                o.sd.gen.ord, o.sd.gen.reboot};
   INT i;
dbgPrintf1("*** initShutdownSelDlg\n");
   // get initial dialog size
   WinQueryWindowRect(hwnd, &r);
   // get the index of the first control to be enabled
   for (i = 0; i < 5; ++i) if (ab[i]) break;
   // if the previous shutdown selection is no longer available preselect
   // the first enabled control
   if (!ab[g.achsd[0] - '0']) g.achsd[0] = i + '0';
   p->flMode = g.achsd[0] - '0' + 1;
   if (g.achsd[0] == '4') p->flMode |= SDFL_REBOOTIDX;
dbgPrintf3("*** initShutdownSelDlg mode=%08x g.achsd=%s\n", p->flMode, g.achsd);
   // re-check radiobutton corresponding to the previous shutdown selection
   WinCheckButton(hwnd, (RDSSDWN_LOCKSUSP + g.achsd[0] - '0'), 1);
// WinSendDlgItemMsg(hwnd, (RDSSDWN_LOCKSUSP + g.achsd[0] - '0'), BM_CLICK, MPFROMSHORT(TRUE), (MPARAM) 0 );
   // show and move the window controls according to the current settings
   r.yTop -= formatShutdownSelControls(hwnd, p);
   WinSetWindowPtr(hwnd, QWL_USER, p);
   // center the dialog on the screen
   p->r.xLeft = (g.scr.cx - r.xRight) / 2;
   p->r.yBottom = (g.scr.cy - r.yTop) / 2;
   p->r.xRight = p->r.xLeft + r.xRight;
   p->r.yTop = p->r.yBottom + r.yTop;
   WinSetWindowPos(hwnd, HWND_TOP, p->r.xLeft, p->r.yBottom,
                   r.xRight, r.yTop,
                   SWP_SIZE | SWP_MOVE | SWP_SHOW | SWP_ZORDER | SWP_ACTIVATE);
   return MRTRUE;
}


/* --------------------------------------------------------------------------
 Format the dialog showing/hiding dialog controls according to the current
 shutdown options.
- Parameters -------------------------------------------------------------
 HWND hwnd : window handle.
 PDOSHUTDOWN p : shutdown data structure.
- Return value -----------------------------------------------------------
 INT height to be subtracted from the total dialog height.
     That is the height of the controls which are hidden.
-------------------------------------------------------------------------- */
static INT formatShutdownSelControls(HWND hwnd, PDOSHUTDOWN p) {
   POINTL pt;
   INT shift = 0;    // control position shift
   INT ygrp = 36;    // groupbox y coordinate
   INT cygrp = 90;   // groupbox height

   // dialog border sizes
   pt.x = WinSysVal(SV_CXSIZEBORDER);
   pt.y = WinSysVal(SV_CYSIZEBORDER);
   // if a list of programs to be executed on shutdown is defined
   // check the 'Execute shutdown programs' checkbox according to
   // the current settings enabling the checkbox when appropriate
   if (shdwnReadProgList(FALSE)) {         // not-empty program list
      p->flMode |= o.sd.adv.runPrograms ? SDFL_EXECUTE : SDFL_NOEXECUTE;
      WinCheckButton(hwnd, CHK_RUNPROGS, o.sd.adv.runPrograms);
      WinEnableControl(hwnd, CHK_RUNPROGS, (g.achsd[0] >= '2'));
   } else {                                // empty program list
      DlgItemShow(hwnd, CHK_RUNPROGS, FALSE);
      shift = 14;
      ygrp = 22;
   } /* endif */
   // rebootable volumes dropdown box
   if (o.sd.gen.reboot && fillRebootList(hwnd, p)) {
      WinEnableControl(hwnd, COMBOSSDWN, g.achsd[0] == '4');
      if (shift)
         setCtrlPos(hwnd, COMBOSSDWN, &pt, 16, -4 - shift, 0, 0, SWP_MOVE);
   } else {
      DlgItemShow(hwnd, COMBOSSDWN, FALSE);
      shift += 14;
      cygrp -= 14;
   } /* endif */
   // radiobuttons
   mSetRadioBtn(hwnd, RDSSDWN_REBOOT, o.sd.gen.reboot, &pt, 60, shift, cygrp);
   mSetRadioBtn(hwnd, RDSSDWN_ORD, o.sd.gen.ord, &pt, 72, shift, cygrp);
   mSetRadioBtn(hwnd, RDSSDWN_OFF, o.sd.gen.off, &pt, 84, shift, cygrp);
   mSetRadioBtn(hwnd, RDSSDWN_SUSP, o.sd.gen.susp, &pt, 96, shift, cygrp);
   mSetRadioBtn(hwnd, RDSSDWN_LOCKSUSP, o.sd.gen.lksusp, &pt, 108, shift, cygrp);
   // groupbox
   if (shift)
       setCtrlPos(hwnd, GRP_SDWN, &pt, 5, ygrp, 210, cygrp, SWP_SIZE | SWP_MOVE);
   pt.x = 0; pt.y = shift;
   WinMapDlgPoints(hwnd, &pt, 1, TRUE);
   return pt.y;
}


/* --------------------------------------------------------------------------
 Set the position of a dialog controls converting the coordinates from
 dialog points to pixels.
- Parameters -------------------------------------------------------------
 HWND hwnd   : dialog window handle.
 ULONG id    : control id.
 PPOINTL ppt : dialog border sizes.
 LONG x,y    : dialog control left-bottom corner coordinates.
 LONG cx,cy  : dialog control width and height.
 ULONG fl    : WinSetWindowPos flag.
- Return value -----------------------------------------------------------
 BOOL
-------------------------------------------------------------------------- */
static BOOL setCtrlPos(HWND hwnd, ULONG id, PPOINTL ppt,
                LONG x, LONG y, LONG cx, LONG cy, ULONG fl) {
   WPOSSIZE wr = {x, y, cx, cy};
   WinMapDlgPoints(hwnd, (PPOINTL)&wr, 2, TRUE);
   return DlgItemSetPos(hwnd, id, HWND_TOP, wr.x + ppt->x,
                        wr.y + ppt->y, wr.cx, wr.cy, fl);
}


/* --------------------------------------------------------------------------
 On button click dismiss the dialog.
- Parameters -------------------------------------------------------------
 HWND hwnd   : dialog window handle.
 ULONG idCmd : command id.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID handleCommand(HWND hwnd, ULONG idCmd) {
   PDOSHUTDOWN p;
   if (NULL != (p = WinQueryWindowPtr(hwnd, QWL_USER))) {
      if (idCmd == BTNSSDWN_OK) {
         p->flMode |= SDFL_SAVEDATA;
dbgPrintf2("*** handleCommand mode=%08x\n", p->flMode );
      } else {
         p->flMode = SDFL_CANCEL;
      } /* endif */
   } /* endif */
   WinDismissDlg(hwnd, p->flMode);
}


/* --------------------------------------------------------------------------
 Fill the list of re-bootable volumes re-selecting the last volume which
 was booted through the shutdown selection dialog.
- Parameters -------------------------------------------------------------
 HWND hwnd               : window handle.
 PREBOOTLIST pRebootList : list of rebootable volumes.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE not-emtpy/empty list.
-------------------------------------------------------------------------- */
static BOOL fillRebootList(HWND hwnd, PDOSHUTDOWN p) {
   INT i, isel;
   HWND hlist;
   if (p->pRebootList)
   {
      if (NULLHANDLE != (hlist = WinWindowFromID(hwnd, COMBOSSDWN)))
      {  // insert the items in a loop
         for (i = 0, isel = -1; i < p->pRebootList->ci; ++i)
         {
            wLbxItemIns(hlist, LIT_END, p->pRebootList->aVolDescr[i]);
            // if the item matches the last booted volume store its index
            if (
               (isel < 0)
               &&
               !strcmp(&g.achsd[1], p->pRebootList->aSetBootParm[i])
               )
               isel = i;
         } /* endfor */
         // if no match was found select the first item
         if (isel == -1) isel = 0;
         wLbxItemSelect(hlist, isel);
         p->flMode |= (isel << 16);
         return TRUE;
      }
   // if the list is empty reset the last-booted volume data to NUL
   }
   else
      g.achsd[1] = 0;

   return FALSE;
}


/* --------------------------------------------------------------------------
 Shutdown selection dialog notification messages.
- Parameters -------------------------------------------------------------
 HWND hwnd     : dialog window handle.
 ULONG idCtrl  : control id.
 ULONG idEvent : notification code.
 HWND hCtrl     : handle of the control.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID handleCtrlMsgs(HWND hwnd, ULONG idCtrl, ULONG idEvent, HWND hCtrl) {
   PDOSHUTDOWN p;
   INT iitem;
   if (NULL != (p = WinQueryWindowPtr(hwnd, QWL_USER))) {
      // radiobutton selection : store the radiobutton index
      if ((idCtrl >= RDSSDWN_LOCKSUSP) && (idCtrl <= RDSSDWN_REBOOT)) {
         if ((idEvent == BN_CLICKED) || (idEvent == BN_DBLCLICKED)) {
            // enable/disable the "run shutdown programs" check box
            DlgItemEnable(hwnd, CHK_RUNPROGS, (idCtrl >= RDSSDWN_OFF));
            // get the radiobutton index
            idEvent = idCtrl - RDSSDWN_LOCKSUSP + 1;
            p->flMode = (p->flMode & 0xffffff00) | idEvent;
            // enable/disable the re-bootable volumes dropwdown list
            if (idEvent == SDFL_REBOOT) {
               p->flMode |= SDFL_REBOOTIDX;
               if (p->pRebootList) DlgItemEnable(hwnd, COMBOSSDWN, TRUE);
            } else {
               DlgItemEnable(hwnd, COMBOSSDWN, FALSE);
            } /* endif */
         } /* endif */
      // checkbox run programs
      } else if (idCtrl == CHK_RUNPROGS) {
         if ((idEvent == BN_CLICKED) || (idEvent == BN_DBLCLICKED))
            idEvent = WinQueryButtonCheckstate(hwnd, CHK_RUNPROGS) ?
                      SDFL_EXECUTE : SDFL_NOEXECUTE;
            p->flMode = (p->flMode & ~SDFL_EXEMASK) | idEvent;
      // dropdown box item selection
      } else if (idCtrl == COMBOSSDWN) {
         if ((idEvent == CBN_LBSELECT) && p->pRebootList) {
            iitem = wLbxItemSelected(hCtrl);
            if (iitem >= 0) p->flMode = (p->flMode & 0xffff) | (iitem << 16);
dbgPrintf3(" [reboot] item: %d - mode: %08x\n", iitem, p->flMode);
         } /* endif */
      } /* endif */
   } /* endif */
}


/* --------------------------------------------------------------------------
 Create the message box to notify the user that the system is being closed.
- Parameters -------------------------------------------------------------
 HAB hab      : anchor block handle.
 PDOSHUTDOWN p : shutdown selection data structure.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID runSysClosingBox(HAB hab, PDOSHUTDOWN p) {
   QMSG qmsg;
   // save last shutdown selection to OS2.INI
   if (p->flMode & SDFL_SAVEDATA)
      PrfWriteProfileString(HINI_USER, SZPRO_APP, SZPRO_ILAST, g.achsd);
dbgPrintf2("*** runSysClosingBox - g.achsd = %s\n", g.achsd);
   // load the message notifying the user to wait while the system shuts down
   DosSetPriority(PRTYS_THREAD, PRTYC_FOREGROUNDSERVER, PRTYD_MAXIMUM, 0);
   g.hwnd.shutdown = WinLoadDlg(g.hwnd.desktop, NULLHANDLE,
                                sysTermMsgProc, p->hModRes, DLG_LAST, NULL);
dbgPrintf2("*** runSysClosingBox - hwnd = %08x\n", g.hwnd.shutdown);
   if (g.hwnd.shutdown) {
      // tell the worker thread to go on with the shutdown procedure
      m_postWorkerMsg(STLRM_SHUTDOWNSYS, g.worker.hwnd, 0);
      for (;;) {
         if (WinGetMsg(hab, &qmsg, NULLHANDLE, 0, 0)) {
            if ((qmsg.msg == STLRM_EXITSHDWNTHREAD)
                &&
                (qmsg.mp1 == (MPARAM)g.hwnd.shutdown))
               break;
            WinDispatchMsg(hab, &qmsg);
         } /* endif */
      } /* endfor */
      WinDestroyWindow(g.hwnd.shutdown);
      g.hwnd.shutdown = NULLHANDLE;
   } /* endif */
}


/* --------------------------------------------------------------------------
 Procedure of the dialog window used to show the final dialog boxes
 (The system is shutting down.... please wait...).
- Parameters -------------------------------------------------------------
 ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
static
MRESULT EXPENTRY sysTermMsgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   if (msg == STLRM_SHOWSYSCLOSEBOX) {
      RECTL r;
      WinQueryWindowRect(hwnd, &r);
      if (g.achsd[0] == '2') DlgItemShow(hwnd, TXT_LASTPWROFF, TRUE);
      if (g.achsd[0] == '4') DlgItemShow(hwnd, TXT_LASTREBOOT, TRUE);
      WinSetWindowPos(hwnd, HWND_TOP, (g.scr.cx - r.xRight) / 2,
                      (g.scr.cy - r.yTop) / 2, 0, 0,
                      SWP_ACTIVATE | SWP_SHOW | SWP_ZORDER | SWP_MOVE);
      WinSetPointerPos(g.hwnd.desktop, g.scr.cx /2, g.scr.cy /2);
      WinShowPointer(g.hwnd.desktop, FALSE);
      if (g.achsd[0] == '3') {
         DlgItemShow(hwnd, TXT_LASTSDHIDE, TRUE);
         DlgItemShow(hwnd, TXT_LASTSDCOMPLETE, TRUE);
         DlgItemShow(hwnd, TXT_LASTSWTCHOFF, TRUE);
         DosShutdown(0L);
         DosSleep(o.sd.wait.switchOffMsg);
         DlgItemShow(hwnd, TXT_LASTSHDWN, FALSE);
         DlgItemShow(hwnd, TXT_LASTSDHIDE, FALSE);
//       DlgItemShow(hwnd, TXT_LASTSDCOMPLETE, TRUE);
//       DlgItemShow(hwnd, TXT_LASTSWTCHOFF, TRUE);
      } /* endif */
      return MRFALSE;
   } /* endif */
   return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


/* --------------------------------------------------------------------------
 Execute Ctrl-Alt-Del via software.
- Parameters -------------------------------------------------------------
 VOID
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID doCtrlAltDel(VOID) {
   HFILE hf;
   ULONG ul;
   if (!DosOpen("\\DEV\\DOS$", &hf, &ul, 0L, FILE_NORMAL, FILE_OPEN,
                OPEN_SHARE_DENYNONE | OPEN_ACCESS_READWRITE, 0L)) {
      DosDevIOCtl(hf, 0xd5, 0xab, NULL, 0L, NULL, NULL, 0L, NULL);
      DosClose(hf);
   } /* endif */
}

/* --------------------------------------------------------------------------
 Execute a power management function
- Parameters -------------------------------------------------------------
 ULONG fl : APM (or ACPI) command (APM_SUSPEND / APM_POWEROFF).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
VOID doPowerManagement(ULONG fl) {
   HFILE hf;
   ULONG ul;
   SENDPWREV spe;
   ULONG cbparm = sizeof(SENDPWREV);
   USHORT apmrc = 0;

   /* try ACPI first if power-off was requested */
#ifdef ACPI_ENABLED
   if ((fl==APM_POWEROFF) && tryAcpiPwrOff()) return;
#endif

   /* use APM if ACPI not supported */
   spe.usid = 0x6;
   spe.usres = 0;
   spe.us1 = ID_ALL;
   spe.us2 = fl;
   if (!DosOpen(APMDEV, &hf, &ul, 0, FILE_NORMAL, FILE_OPEN, OPNDEVFL, NULL)) {
      ul = sizeof(USHORT);
      DosDevIOCtl(hf, APMCAT, SETPWR, &spe, cbparm, &cbparm, &apmrc, ul, &ul);
      if (fl == APM_POWEROFF) DosSleep(o.sd.wait.onPowerOff);
      dbgPrintf1("APM shutdown complete.\n");
      DosClose(hf);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Power off using the ACPI driver
- Parameters -------------------------------------------------------------
 N/A
- Return value -----------------------------------------------------------
 BOOL
 ACPI action succeeded (y/n)?
-------------------------------------------------------------------------- */
#ifdef ACPI_ENABLED
BOOL tryAcpiPwrOff(VOID)
{
   BOOL fAcpiOK = FALSE;
   ACPI_API_HANDLE Hdl;
   ULONG ul;

   if (g.acpifn.pfnAcpiStartApi && g.acpifn.pfnAcpiGoToSleep && g.acpifn.pfnAcpiEndApi) {
      if (g.acpifn.pfnAcpiStartApi(&Hdl) == NO_ERROR) {
         fAcpiOK = TRUE;
         dbgPrintf1("Initiating ACPI power-off.\n");
         dbgEnd();
         ul = WinCancelShutdown(g.worker.hmq, TRUE);
         ForceCommitFn(g.acpifn.pfnAcpiGoToSleep);
         ul = DosShutdown(0L);
         DosSleep(o.sd.wait.onPowerOff);
         ul = g.acpifn.pfnAcpiGoToSleep(&Hdl, ACPI_STATE_S5);
         // The next lines will probably never be reached if poweroff was successful...
         g.acpifn.pfnAcpiEndApi(&Hdl);
         DosFreeModule(g.hmodAcpi);
      } else dbgPrintf1("Failed to open ACPI API.\n");
   }

   return fAcpiOK;
}
#endif


/* --------------------------------------------------------------------------
 According to the value of the 'getList' parameter get the list of the
 programs to be executed on system shutdown or just check if any entry
 is defined.
- Parameters -------------------------------------------------------------
 BOOL getList : TRUE  allocate storage and read the program list.
                FALSE just check if the list is not empty.
- Return value -----------------------------------------------------------
 PSHDWNPRGLIST : list of program to be executed on system shutdown.
-------------------------------------------------------------------------- */
PSHDWNPRGLIST shdwnReadProgList(BOOL getList) {
   HINI hini;
   ULONG ul;
   PSHDWNPRGLIST pProgramList = NULL;
   // apre estyler.ini
   if (NULLHANDLE == (hini = stlrOpenProfile())) return NULL;
   // get the size of the list
   if (!PrfQueryProfileSize(hini, SZPRO_SHUTDWON, SZPRO_SDWNPRGLIST, &ul))
      goto exit_0;
   // if must just check for the presence of program entries terminate
   if (!getList) {
      pProgramList = (PSHDWNPRGLIST)TRUE;
      goto exit_0;
   } /* endif */
   // allocation error just do not execute the shutdown programs
   if (NULL == (pProgramList = memLkAlloc(ul + sizeof(SHDWNPRGLIST) - 4)))
      goto exit_0;
   // read the list and count the programs
   if (PrfQueryProfileData(hini, SZPRO_SHUTDWON, SZPRO_SDWNPRGLIST,
                           pProgramList->ab, &ul)) {
      for (pProgramList->cprog = 0, pProgramList->pprog = pProgramList->ab;
           *pProgramList->pprog;
           pProgramList->pprog += strlen(pProgramList->pprog) + 1)
         pProgramList->cprog++;
      if (!pProgramList->cprog) goto exit_1;
      pProgramList->pprog = pProgramList->ab;
      pProgramList->icurprog = 0;
   } else {
exit_1:
      memLkFree(pProgramList);
      pProgramList = NULL;
   } /* endif */
exit_0:
   PrfCloseProfile(hini);
   return pProgramList;
}



/* --------------------------------------------------------------------------
 Force a function address to be paged in (committed).  This is used to make
 sure that the ACPI power-off function has been paged into memory before
 DosShutdown() is called.  (The use of 'volatile' also keeps the compiler
 from optimizing this code away.)
 Code contributed by David Azarewicz and Steven Levine.
- Parameters -------------------------------------------------------------
 PFN pAddress : Address of the function being committed to memory.
- Return value -----------------------------------------------------------
VOID
-------------------------------------------------------------------------- */
VOID ForceCommitFn( PFN pAddress )
{
    char  cTmp;
    ULONG ulMemSize  = 1,
          ulMemFlags = 0;

    cTmp = *((volatile char *)pAddress);
    if ( DosQueryMem(( (char *)pAddress ) + 4096,
                       &ulMemSize, &ulMemFlags ) == NO_ERROR )
    {
        if ( ulMemFlags & PAG_COMMIT )
            cTmp = *(( (volatile char *) pAddress ) + 4096 );
    }
}


