#ifndef STLR_ERROR_H_
   #define STLR_ERROR_H_

// Smart Windows error messages
enum {IDERR_NULL,
      // errori inizializzazione globale (smwdll.c)
      IDERR_NOTVALIDVER,    // 1 errore si cerca di usare su versione non valida
      IDERR_INIOPEN,        // 2 errore apertura eStyler.ini
      IDERR_FRAMECLASS,     // 3 errore rilevazione dati classe frame
      IDERR_NEWFRAME,       // 4 errore registrazione nuova classe frame
      IDERR_TITLEBARCLASS,  // 5 errore rilevazione dati classe titlebar
      IDERR_NEWTITLEBAR,    // 6 errore registrazione nuova classe titlebar
      IDERR_BTNCLASS,       // 7 errore rilevazione dati classe bottone
      IDERR_NEWBTN,         // 8 errore registrazione nuova classe bottone
      IDERR_CREATETIMERMGR, // 9 errore creazione thread timer manager
      IDERR_CREATEINPUTHOOK,// 10 errore creazione input hook
      IDERR_CREATESNMSGHOOK,// 11 errore creazione send message hook
      IDERR_GETRESMOD,      // 12 errore richiesta handle DLL risorse
      // errori inizializzazione per processo
      IDERR_INITPROCHEAP = 100,   // errore inizializzazione heap processo
//      IDERR_ALLOCPROCDATA,  // 1 errore allocazione memoria dati processo
//      IDERR_ALLOCMODNAME,   // 2 allocazione necessaria per lettura nome modulo
//      IDERR_INFOBLOCKS,     // 3 errore richiesta infoblock
      IDERR_QRYMODNAME,     // 4 errore DosQueryModuleName()
      IDERR_CHKEXCALLOC,    // 5 errore allocazione in CheckExceptions
      IDERR_REGEXITFUNC,    // 6 errore registrazione funzione cleanup
      IDERR_CHKEXCREAD,     // 7 errore lettura dati eccezioni
      IDERR_MAX};

#endif
