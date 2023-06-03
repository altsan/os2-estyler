#######################################################################
#
# Styler\CLI-shutdown makefile
#
# This builds Styler command-line shutdown
#
# Tools used: nmake32  ilink(v5)  vacpp(v3.08 or v3.65)
#
#
# Set the path to the ACPI Toolkit headers here or in the environment

#######################################################################
#
# Variables
#

# this pulls in Styler's version number
!include "$(MAKEDIR)\..\headers\stlrVersion.h"

# target executable
SHUTDOWN_EXE = shutdown.exe

# compiler, linker
CC = icc.exe
CL = ilink.exe

# compiler and linker flags
INCL = /I..\headers
WARN = /Wall+ext-gnr-got-par-por-ppc-ppt-trd-uni-

# Debug version
!ifdef DEBUG
CFLAGS = /Ti /Ss /Q /Rn /G5 /C $(WARN) $(EXTRA) $(INCL)
LFLAGS = /EXEPACK:2 /PM:VIO /NOLOGO /MAP /LINE /DEBUG

# Release version
!else
CFLAGS = /O  /Ss /Q /Rn /G5 /C $(WARN) $(EXTRA) $(INCL)
LFLAGS = /EXEPACK:2 /PM:VIO /NOLOGO /MAP
!endif

# BldLevelInf.cmd's command line and options
BLDLVL = $(MAKEDIR)\..\shared\util\BldLevelInf.cmd
BLDLVL_OPTS  = -V"$(VERMAJ).$(VERMIN)" -M"$(VERBUILD)"
BLDLVL_OPTS += -P"$(VERMAJ).$(VERMIN)-$(VERBUILD)"
BLDLVL_OPTS += -N"Alessandro Cantatore" -D"Styler enhanced shutdown command"

#######################################################################
#
# Targets
#
# Note: ilink will warn "entry point ignored; specified previously"
# because Styler's custom _DLL_InitTerm replaces the C-RTL's version.

all: $(SHUTDOWN_EXE) symbols

$(SHUTDOWN_EXE): \
   shutdown.obj
   echo off >temp.def
   $(BLDLVL) temp.def $(BLDLVL_OPTS)
   $(CL) @<< shutdown.lrf
      $(LFLAGS)
      /O:$@
      temp.def
      shutdown.obj
<<
   -@del temp.def > nul 2>&1


symbols: shutdown.map
   @mapsym $<


clean cleanall cleancli:
    %echo cleaning CLI-shutdown
    -@del *.obj 2> nul
    -@del *.exe 2> nul
    -@del *.map 2> nul


#######################################################################
#
# Inference rules and dependencies
#

.c.obj:
   $(CC) $(CFLAGS) $<

shutdown.obj   : shutdown.c      *.h ..\headers\*.h

