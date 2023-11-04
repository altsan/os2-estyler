#if 0
#===========================================================================
# stlrver.h :
#
# --2003  - Alessandro Felice Cantatore
#===========================================================================
#
#
# This file contains version info for both C code
# *and* the makefiles, and gets included in both.
# It relies on the fact that C directives are treated
# as comments by nmake32, while nmake32 directives
# can be hidden from the C compiler using #if 0.
#
# These variables are used by nmake32
#
VERMAJ = 1
VERMIN = 2
VERBUILD = 0
#
#
# The variables below are used by icc.
#
#endif // #if 0

#ifndef _STLRVER_H_
   #define _STLRVER_H_

#define VERMAJ      1
#define VERMIN      2
#define VERBUILD    0

#define CURINIVER   110

#define VERSUPPORTED        45

#endif // #ifndef _STLRVER_H_

