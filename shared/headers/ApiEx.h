//========================================================================
// ApiEx.h : generic macros and definitions
//
// 10-01-2002 * by Alessandro Felice Cantatore *
//========================================================================

// header inclusions

#ifndef APIEX_GENERIC_H_
   #define APIEX_GENERIC_H_

// aligns n to the next m multiple (where m is a power of 2)
#define RNDUP(n, m)   (((ULONG)(n) + (m) - 1) & ~((m) - 1))
// aligns n to the previous m multiple (where m is a power of 2)
#define RNDDWN(n, m)  ((ULONG)(n) & ~((m) - 1))

// count of array elements
#define ciArray(array)    (sizeof(array) / sizeof(array[0]))

// DosFindFirst mask
#define ANYFILE    (FILE_ARCHIVED | FILE_SYSTEM | FILE_HIDDEN | FILE_READONLY)

// Convert the address of a FILEFINDBUF3 structure to the address of a
// FILESTATUS3 structure
#define FILEFIND_TO_FILESTAT(p)  ((PFILESTATUS3)((PBYTE)(p) + sizeof(ULONG)))


#endif // #ifndef APIEX_GENERIC_H_
