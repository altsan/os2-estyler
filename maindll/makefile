#######################################################################
#
# Styler\maindll makefile
#
# This builds Styler hook dll
#
# Tools used: nmake32  ilink(v5)  vacpp(v3.08 or v3.65)
#
#
# Set the path to the ACPI Toolkit headers here or in the environment

!ifndef ACPI_INCL
ACPI_INCL = C:\path\to\acpi\h
!endif


#######################################################################
#
# Variables
#

# this pulls in Styler's version number
!include "$(MAKEDIR)\..\headers\stlrVersion.h"

# versioned name of the dll
STYLER_DLL = estlrl$(VERMAJ)$(VERMIN).dll

# compiler, linker
CC = icc.exe
CL = ilink.exe

# compiler and linker flags
INCL = /I..\headers /I..\shared\headers /I$(ACPI_INCL)
WARN = /Wall+ext-gnr-got-par-por-ppc-ppt-trd-uni-
EXTRA = /DACPI_ENABLED

# To build using the APIEX000 headers set the following:
# EXTRA += /DUSE_APIEX=1

# Debug version
!ifdef DEBUG
CFLAGS = /Ti /Ss /Q /Rn /Ge- /G5 /C $(WARN) $(EXTRA) $(INCL) /DDEBUGGING=2
LFLAGS = /EXEPACK:2 /PM:PM /NOLOGO /MAP /LINE /DEBUG

# Release version
!else
CFLAGS = /O  /Ss /Q /Rn /Ge- /G5 /C $(WARN) $(EXTRA) $(INCL)
LFLAGS = /EXEPACK:2 /PM:PM /NOLOGO /MAP
!endif

# BldLevelInf.cmd's command line and options
BLDLVL = $(MAKEDIR)\..\shared\util\BldLevelInf.cmd
BLDLVL_OPTS  = -V"$(VERMAJ).$(VERMIN)" -M"$(VERBUILD)"
BLDLVL_OPTS += -P"$(VERMAJ).$(VERMIN)-$(VERBUILD)"
BLDLVL_OPTS += -N"Alessandro Cantatore" -D"Styler hook library"

#######################################################################
#
# Targets
#
# Note: ilink will warn "entry point ignored; specified previously"
# because Styler's custom _DLL_InitTerm replaces the C-RTL's version.

all: $(STYLER_DLL)

$(STYLER_DLL): \
   main.obj \
   bitmaps.obj \
   process.obj \
   button.obj \
   debug.obj \
   export.obj \
   frame.obj \
   hooks.obj \
   shutdown.obj \
   titlebar.obj \
   utils.obj \
   workthread.obj \
   estyler.def
   $(BLDLVL) estyler.def temp.def $(BLDLVL_OPTS)
   $(CL) @<< estlrl.lrf
      $(LFLAGS)
      /O:$@
      temp.def
      main.obj
      bitmaps.obj
      process.obj
      button.obj
      debug.obj
      export.obj
      frame.obj
      hooks.obj
      shutdown.obj
      titlebar.obj
      utils.obj
      workthread.obj
<<
   -@del temp.def > nul 2>&1


clean cleanall cleandll:
    %echo cleaning maindll
    -@del *.obj 2> nul
    -@del *.dll 2> nul
    -@del *.map 2> nul


#######################################################################
#
# Inference rules and dependencies
#

.c.obj:
   $(CC) $(CFLAGS) $<

main.obj       : main.c          *.h ..\headers\*.h
bitmaps.obj    : bitmaps.c       *.h ..\headers\*.h
process.obj    : process.c       *.h ..\headers\*.h
button.obj     : button.c        *.h ..\headers\*.h
debug.obj      : debug.c         *.h ..\headers\*.h
export.obj     : export.c        *.h ..\headers\*.h
frame.obj      : frame.c         *.h ..\headers\*.h
hooks.obj      : hooks.c         *.h ..\headers\*.h
shutdown.obj   : shutdown.c      *.h ..\headers\*.h
titlebar.obj   : titlebar.c      *.h ..\headers\*.h
utils.obj      : utils.c         *.h ..\headers\*.h
workthread.obj : workthread.c    *.h ..\headers\*.h

