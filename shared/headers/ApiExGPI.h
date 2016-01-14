//===========================================================================\
// ApiExGPI.h: General purpose GPI macros and definitions
//            These definitions are meant to be available to any program
// 12-03-2002 * by Alessandro Felice Cantatore * v...
//===========================================================================/



#ifndef _APIEX_GPI_H_
   #define _APIEX_GPI_H_

// This macro take a background color (RGB value) as input and calculate
// the most visible foreground color as white or black (RGB value).
// It takes into account the fact that the human eye is most sensible
// to the green color.
#define gpiColorOutstanding(clr)      \
   ((((((clr) & 0xff) +               \
     ((((clr) >> 8) & 0xff) << 2) +   \
     ((clr) >> 16)) > 0x3b0)   ||     \
     (clr > 0xf00000)          ||     \
     ((clr & 0xff00) > 0xc000)) ?     \
      0: 0xffffff)
    

#endif // #ifndef _APIEX_GPI_H_
