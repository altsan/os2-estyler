/* --------------------------------------------------------------------------
 dlgSDtiming.c : shutdown - timing options

 2004-6-4 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------

-------------------------------------------------------------------------- */

// #pragma strings(readonly)

// includes -----------------------------------------------------------------
#include "main.h"

// definitions --------------------------------------------------------------



// prototypes ---------------------------------------------------------------
static BOOL onDlgInit(HWND hwnd);
static VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl);
static VOID checkSpin(HWND hwnd, HWND hSpin, ULONG event, PLONG pVal, PUSHORT pus);
static BOOL onCmdMsg(HWND hwnd, ULONG id);
static VOID setControlsState(HWND hwnd);
static VOID checkDefaultState(HWND hwnd);
static VOID defaultOptions(HWND hwnd);

// global variables ---------------------------------------------------------

/* --------------------------------------------------------------------------
 Shutdown - timing options
- Parameters -------------------------------------------------------------
 Ordinary window procedure parameters.
- Return value -----------------------------------------------------------
 MRESULT.
-------------------------------------------------------------------------- */
MRESULT EXPENTRY sdTimeOptsProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
   switch (msg) {
      case WM_INITDLG:
         onDlgInit(hwnd);
         break;
      case WM_CONTROL:
         if (!(g.state & STLRIS_SKIPNOTIFICATION))
            onCtrlMsg(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), (HWND)mp2);
         break;
      case WM_COMMAND:
         // if OK or CANCEL fall thrugh the next case otherwise return FALSE
         if (onCmdMsg(hwnd, (ULONG)mp1)) break;
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
      default:
         return WinDefDlgProc(hwnd, msg, mp1, mp2);
   } /* endswitch */
   return MRFALSE;
}

/* --------------------------------------------------------------------------
 initialization.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 BOOL : TRUE/FALSE (success/error)
-------------------------------------------------------------------------- */
static BOOL onDlgInit(HWND hwnd) {
   setControlsState(hwnd);
   checkDefaultState(hwnd);
   return TRUE;
}


/* --------------------------------------------------------------------------
 Process notification messages.
- Parameters -------------------------------------------------------------
 HWND hwnd : window handle
 ULONG id       : control ID
 ULONG event : notify code
 HWND hCtrl     : control handle
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID onCtrlMsg(HWND hwnd, ULONG id, ULONG event, HWND hCtrl) {
   LONG lValue;
   switch (id) {
      case SPIN_SDTIMLKUPSUSP:
         checkSpin(hwnd, hCtrl, event, &lValue,
                   &g.pCurOpts->sd.wait.lkupSusp);
         break;
      case SPIN_SDTIMSUSPEND:
         checkSpin(hwnd, hCtrl, event, &lValue,
                   &g.pCurOpts->sd.wait.suspend);
         break;
      case SPIN_SDTIMWARPCENTER:
         checkSpin(hwnd, hCtrl, event, &lValue,
                   &g.pCurOpts->sd.wait.warpCenter);
         break;
      case SPIN_SDTIMUNROLLWIN:
         checkSpin(hwnd, hCtrl, event, &lValue,
                   &g.pCurOpts->sd.wait.unrollWindows);
         break;
      case SPIN_SDTIMSYSMSGBOX:
         checkSpin(hwnd, hCtrl, event, &lValue,
                   &g.pCurOpts->sd.wait.closeBox);
         break;
      case SPIN_SDTIMWINSD:
         checkSpin(hwnd, hCtrl, event, &lValue,
                   &g.pCurOpts->sd.wait.sysShutdown);
         break;
      case SPIN_SDTIMDOSSD:
         checkSpin(hwnd, hCtrl, event, &lValue,
                   &g.pCurOpts->sd.wait.switchOffMsg);
         break;
      case SPIN_SDTIMOFF:
         checkSpin(hwnd, hCtrl, event, &lValue,
                   &g.pCurOpts->sd.wait.onPowerOff);
         break;
      case SPIN_SDTIMKILL:
         checkSpin(hwnd, hCtrl, event, &lValue,
                   &g.pCurOpts->sd.wait.killProcs);
         break;
   } /* endswitch */
}


/* --------------------------------------------------------------------------
 If the value of a spin buton changed update the settings and the state
 of the Default button.
- Parameters -------------------------------------------------------------
 HWND hwnd     : dialog window handle.
 HWND hSpin    : spin button handle.
 ULONG ulEvent : WM_CONTROL notification code.
 PLONG pVal    : value to be updated
 PUSHORT pus   : address of the settings variable.
- Return value -----------------------------------------------------------
 VOID.
-------------------------------------------------------------------------- */
VOID checkSpin(HWND hwnd, HWND hSpin, ULONG event, PLONG pVal, PUSHORT pus) {
   if (handleSpinbtnEvent(hSpin, event, pVal, (LONG)*pus)) {
      *pus = (USHORT)(*pVal * 100);
      checkDefaultState(hwnd);
   } /* endif */
}


/* --------------------------------------------------------------------------
 Process command events.
- Parameters -------------------------------------------------------------
 HWND hwnd : window handle
 ULONG id  : button ID.
- Return value -----------------------------------------------------------
 BOOL : TRUE / FALSE (default / OK-CANCEL button was pressed).
-------------------------------------------------------------------------- */
static BOOL onCmdMsg(HWND hwnd, ULONG id) {
   switch (id) {
      case BTN_SDTIMOK:
         memcpy(&g.pUndoSdOpts->wait, &g.pCurOpts->sd.wait, sizeof(SDTIMING));
         break;
      case BTN_SDTIMCANCEL:
         memcpy(&g.pCurOpts->sd.wait, &g.pUndoSdOpts->wait, sizeof(SDTIMING));
         break;
      case BTN_SDTIMDEFAULT:
         defaultOptions(hwnd);
         return TRUE;
   } /* endswitch */
   return FALSE;
}


/* --------------------------------------------------------------------------
 Set the state of the dialog controls according to the current value of
 the options.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
 BOOL bSetRebootPage: TRUE/FALSE (change/ignore Reboot page state).
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID setControlsState(HWND hwnd) {
   g.state |= STLRIS_SKIPNOTIFICATION;
   dSpinBtnSetValue(hwnd, SPIN_SDTIMLKUPSUSP,
                    g.pCurOpts->sd.wait.lkupSusp / 100);
   dSpinBtnSetValue(hwnd, SPIN_SDTIMSUSPEND,
                    g.pCurOpts->sd.wait.suspend / 100);
   dSpinBtnSetValue(hwnd, SPIN_SDTIMWARPCENTER,
                    g.pCurOpts->sd.wait.warpCenter / 100);
   dSpinBtnSetValue(hwnd, SPIN_SDTIMUNROLLWIN,
                    g.pCurOpts->sd.wait.unrollWindows / 100);
   dSpinBtnSetValue(hwnd, SPIN_SDTIMSYSMSGBOX,
                    g.pCurOpts->sd.wait.closeBox / 100);
   dSpinBtnSetValue(hwnd, SPIN_SDTIMWINSD,
                    g.pCurOpts->sd.wait.sysShutdown / 100);
   dSpinBtnSetValue(hwnd, SPIN_SDTIMDOSSD,
                    g.pCurOpts->sd.wait.switchOffMsg / 100);
   dSpinBtnSetValue(hwnd, SPIN_SDTIMOFF,
                    g.pCurOpts->sd.wait.onPowerOff / 100);
   dSpinBtnSetValue(hwnd, SPIN_SDTIMKILL,
                    g.pCurOpts->sd.wait.killProcs / 100);
   g.state &= ~STLRIS_SKIPNOTIFICATION;
}


/* --------------------------------------------------------------------------
 Compare the options set in the dialog with the default options.
 If there is any different option enable the 'Default' button.
- Parameters -------------------------------------------------------------
 HWND hwnd : dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID checkDefaultState(HWND hwnd) {
   SDTIMING sdt;
   memset(&sdt, 0, sizeof(sdt));
   stlrSetShutdownTimingDefaults(&sdt);
   DlgItemEnable(hwnd, BTN_SDTIMDEFAULT,
                 memcmp(&g.pCurOpts->sd.wait, &sdt, sizeof(sdt)));
}


/* --------------------------------------------------------------------------
 Reset the current settings to the default values.
- Parameters -------------------------------------------------------------
 HWND hwnd : page dialog window handle.
- Return value -----------------------------------------------------------
 VOID
-------------------------------------------------------------------------- */
static VOID defaultOptions(HWND hwnd) {
   stlrSetShutdownTimingDefaults(&g.pCurOpts->sd.wait);
   setControlsState(hwnd);
   DlgItemEnable(hwnd, BTN_SDTIMDEFAULT, FALSE);
}
