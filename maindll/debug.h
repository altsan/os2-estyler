/* --------------------------------------------------------------------------
 debug.h : when DEBUGGING is defined debugging informations can be written
           to a debug log file.

 2004-05-12 - Alessandro Felice Cantatore

- Notes : ----------------------------------------------------------------
- Contents: --------------------------------------------------------------
-------------------------------------------------------------------------- */

#ifndef _STLR_DEBUG_H_
   #define _STLR_DEBUG_H_

#ifdef DEBUGGING
   VOID _dbgInit(VOID);
   VOID _dbgEnd(VOID);
   VOID _dbgPrintf(PSZ psz, ...);

   #define dbgInit()                  _dbgInit()
   #define dbgEnd()                   _dbgEnd()
   #define dbgPrintf1(a)              _dbgPrintf(a)
   #define dbgPrintf2(a,b)            _dbgPrintf(a,b)
   #define dbgPrintf3(a,b,c)          _dbgPrintf(a,b,c)
   #define dbgPrintf4(a,b,c,d)        _dbgPrintf(a,b,c,d)
   #define dbgPrintf5(a,b,c,d,e)      _dbgPrintf(a,b,c,d,e)
   #define dbgPrintf6(a,b,c,d,e,f)    _dbgPrintf(a,b,c,d,e,f)
   #define dbgPrintf7(a,b,c,d,e,f,g)  _dbgPrintf(a,b,c,d,e,f,g)
#else
   #define dbgInit()                  ((void)0)
   #define dbgEnd()                   ((void)0)
   #define dbgPrintf1(a)              ((void)0)
   #define dbgPrintf2(a,b)            ((void)0)
   #define dbgPrintf3(a,b,c)          ((void)0)
   #define dbgPrintf4(a,b,c,d)        ((void)0)
   #define dbgPrintf5(a,b,c,d,e)      ((void)0)
   #define dbgPrintf6(a,b,c,d,e,f)    ((void)0)
   #define dbgPrintf7(a,b,c,d,e,f,g)  ((void)0)
#endif

#endif // #ifndef _STLR_DEBUG_H_
