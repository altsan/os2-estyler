// Redirect memory management functions to thread- and subsystem-safe versions

#define malloc(cb)          tsmalloc(cb)
#define free(p)             tsfree(p)
#define _heapmin()          tsheapmin()
//#define realloc(p,cb)       tsrealloc((p),(cb))
//#define strdup(psz)         tsstrdup(psz)


