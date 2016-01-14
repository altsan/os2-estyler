   

#ifndef DOSQSS_H
#define DOSQSS_H

APIRET APIENTRY DosQuerySysState(ULONG func,ULONG arg1,ULONG arg2,
                        ULONG _res_,PVOID buf,ULONG bufsz);

#define QSS_PROCESS        0x0001    // Requests process information 
#define QSS_SEMAPHORE      0x0002    // Requests semaphore information 
#define QSS_MTE            0x0004    // Requests module information 
#define QSS_FILESYS        0x0008    // Requests file system information 
#define QSS_SHMEMORY       0x0010    // Requests shared memory information 
#define QSS_MODVER         0x0200    // Requests module version information 


// count of threads, processes and modules structure

typedef struct {
        ULONG   cThreads;
        ULONG   cProcs;
        ULONG   cModules;
} QSSCOUNTS, *PQSSCOUNTS;

// thread data structure

typedef struct {
        ULONG   ulType;        /* 256 for thread */
        USHORT  threadid;
        USHORT  slotid;
        ULONG   sleepid;
        ULONG   priority;
        ULONG   systime;
        ULONG   usertime;
        UCHAR   state;
        UCHAR   _reserved1_;    /* padding to ULONG */
        USHORT  _reserved2_;    /* padding to ULONG */
} QSSTHRDDAT, * PQSSTHRDDAT;

// process data structure

typedef struct {
        ULONG   ulType;        // 1 for process
        PQSSTHRDDAT pthrddat;  // thread data
        USHORT  pid;           // process id
        USHORT  ppid;          // parent process id
        ULONG   type;
        ULONG   state;
        ULONG   sessid;
        USHORT  hndmod;
        USHORT  cthreads;
        ULONG   csem32;
        ULONG   _reserved2_;
        USHORT  csem16;
        USHORT  cDLLs;
        USHORT  cShareMem;
        USHORT  fdscnt;
        PUSHORT psem16s;
        PUSHORT pDLLs;
        PUSHORT pShareMems;
        PUSHORT pfds;
} QSSPROCDAT, * PQSSPROCDAT;

typedef struct sema {
        struct sema *next;
        USHORT  refcnt;
        UCHAR   sysflags;
        UCHAR   sysproccnt;
        ULONG   _reserved1_;
        USHORT  index;
        CHAR    name[1];
} QSEMA, *PQSEMA;

typedef struct {
        ULONG   ulType;        /**/
        ULONG   _reserved1_;
        USHORT  _reserved2_;
        USHORT  syssemidx;
        ULONG   index;
        QSEMA   sema;
} QSSSEMDAT, * PQSSSEMDAT;

typedef struct {
        USHORT  pid;
        USHORT  opencnt;
} QSEMOWNER32, *PQSEMOWNER32;

typedef struct {
        PQSEMOWNER32    own;
        PCHAR           name;
        PVOID           semrecs; /* array of associated sema's */
        USHORT          flags;
        USHORT          semreccnt;
        USHORT          waitcnt;
        USHORT          _reserved_;     /* padding to ULONG */
} QSEMSMUX32, *PQSEMSMUX32;

typedef struct {
        PQSEMOWNER32    own;
        PCHAR           name;
        PQSEMSMUX32     mux;
        USHORT          flags;
        USHORT          postcnt;
} QSEMEV32, *PQSEMEV32;

typedef struct {
        PQSEMOWNER32    own;
        PCHAR           name;
        PQSEMSMUX32     mux;
        USHORT          flags;
        USHORT          refcnt;
        USHORT          thrdnum;
        USHORT          _reserved_;     /* padding to ULONG */
} QSEMMUX32, *PQSEMMUX32;

typedef struct semstr32 {
        struct semstr32 *next;
        QSEMEV32 evsem;
        QSEMMUX32  muxsem;
        QSEMSMUX32 smuxsem;
} QSEM32STRUC, *PQSEM32STRUC;

typedef struct shrmem {
        struct shrmem *next;
        USHORT  hndshr;
        USHORT  selshr;
        USHORT  refcnt;
        CHAR    name[1];
} QSHRMEM, *PQSHRMEM;

typedef struct module {
        struct module * next;
        USHORT  hndmod;
        USHORT  type;
        ULONG   refcnt;
        ULONG   segcnt;
        PVOID   _reserved_;
        PCHAR   name;
        USHORT  modref[1];
} QSSMODDAT, * PQSSMODDAT;

typedef struct {
        USHORT  sfn;
        USHORT  refcnt;
        ULONG   flags;
        ULONG   accmode;
        ULONG   filesize;
        USHORT  volhnd;
        USHORT  attrib;
        USHORT  _reserved_;
} QFDS, *PQFDS;

typedef struct qfile {
        ULONG           ulType;        /* 8 for file */
        struct qfile    *next;
        ULONG           opencnt;
        PQFDS           filedata;
        char            name[1];
} QFILE, *PQFILE;

typedef struct {
        PQSSCOUNTS    pcounts;       // number of threads, processes, modules
        PQSSPROCDAT   pprocdat;      // process data
        PQSSSEMDAT    psemadat;
        PQSEM32STRUC    sem32data;      /* not always present */
        PQSHRMEM        shrmemdata;
        PQSSMODDAT    pmoddat;
        PVOID           _reserved2_;
        PQFILE          filedata;       /* only present in FP19 or later or W4 */
} QSSDATA, *PQSSDATA;

#endif
  

