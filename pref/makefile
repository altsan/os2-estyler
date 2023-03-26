#######################################################################
#
# Styler\pref makefile
#
# This builds Styler's preferences app, styler.exe.
#
# Tools used: nmake32  ipfc  ilink(v5)  rc(v4)


#######################################################################
#
# Variables
#

# this pulls in Styler's version number
!include "$(MAKEDIR)\..\headers\stlrVersion.h"

# compiler, linker, resource compiler, resource binder
CC = icc.exe
CL = ilink.exe
RC = rc.exe
RB = rc.exe

# compiler and linker flags
INCL = /I..\headers /I..\shared\headers /I..\shared\lvm_tk
WARN = /Wall+ext-gnr-got-par-por-ppc-ppt-trd-uni-
EXTRA = /DACPI_ENABLED

# Debug version
!ifdef DEBUG
CFLAGS = /Ti /Ss /Q /Rn /G5 /C $(WARN) $(EXTRA) $(INCL)
LFLAGS = /EXEPACK:2 /PM:PM /NOLOGO /MAP /LINE /DEBUG

# Release version
!else
CFLAGS = /O  /Ss /Q /Rn /G5 /C $(WARN) $(EXTRA) $(INCL)
LFLAGS = /EXEPACK:2 /PM:PM /NOLOGO /MAP

!endif

# In-tree libs
LIBS = ..\shared\lvm_tk\lvm.lib ..\shared\util\afcutl05.lib

# To build with APIEX000.DLL set the following and remove apixwrap.obj from
# both the dependencies and the link file below (there should be a better way to
# automate this). Also define %ECSINSTSRC%, pointing to the GuidProc source dir,
# and change APIEXLIB in ESTLRENV.CMD to APIEX000.LIB (full path).
# EXTRA += /DUSE_APIEX=1

# BldLevelInf.cmd's command line and options
BLDLVL = $(MAKEDIR)\..\shared\util\BldLevelInf.cmd
BLDLVL_OPTS  = -V"$(VERMAJ).$(VERMIN)" -M"$(VERBUILD)"
BLDLVL_OPTS += -P"$(VERMAJ).$(VERMIN)-$(VERBUILD)"
BLDLVL_OPTS += -N"Alessandro Cantatore" -D"Styler Preferences application"

# rc flags
RCFLAGS = -r -n
RBFLAGS = -x2 -n


#######################################################################
#
# Targets
#

all: Styler.exe

Styler.exe:          \
   main.obj          \
   initend.obj       \
   pmctrls.obj       \
   selColor.obj      \
   worker.obj        \
   util.obj          \
   bitmaps.obj       \
   fontdlg.obj       \
   preview.obj       \
   test.obj          \
   pageProdinfo.obj  \
   pageSDGen.obj     \
   pageSDKill.obj    \
   pageSDProgs.obj   \
   pageSDReboot.obj  \
   pageUIBtn.obj     \
   pageUIDlg.obj     \
   pageUIGen.obj     \
   pageUITbar.obj    \
   pageUITbarGen.obj \
   pageWinKeys.obj   \
   dlgSDtiming.obj   \
   apixwrap.obj      \
   eStyler.res       \
   $(LIBS)           \
   eStyler.def
   $(BLDLVL) estyler.def temp.def $(BLDLVL_OPTS)
   $(CL) @<< styler.lrf
      $(LFLAGS)
      /O:$@
      $(LIBS)
      temp.def
      main.obj
      initend.obj
      pmctrls.obj
      selColor.obj
      worker.obj
      util.obj
      bitmaps.obj
      fontdlg.obj
      preview.obj
      test.obj
      pageProdinfo.obj
      pageSDGen.obj
      pageSDKill.obj
      pageSDProgs.obj
      pageSDReboot.obj
      pageUIBtn.obj
      pageUIDlg.obj
      pageUIGen.obj
      pageUITbar.obj
      pageUITbarGen.obj
      pageWinKeys.obj
      dlgSDtiming.obj
      apixwrap.obj
<<
    -@del temp.def > nul 2>&1
    $(RB) $(RBFLAGS) eStyler.res $@


eStyler.res: eStyler.rc ..\\nls\\res\\Styler.ICO
    %cd ..
    $(RC) $(RCFLAGS) pref\eStyler.rc
    %cd $(MAKEDIR)


clean cleanall cleanexe:
    %echo cleaning pref
    -@del *.obj 2> nul
    -@del *.exe 2> nul
    -@del *.map 2> nul
    -@del *.res 2> nul


#######################################################################
#
# Inference rules and dependencies
#

.rc.res:
   $(RC) $(RCFLAGS) $<

.C.obj:
   $(CC) $(CFLAGS) $<

main.obj          : main.c            *.h notebook.h ..\\headers\\*.h
initend.obj       : initend.c         *.h test.h ..\\headers\\*.h
pmctrls.obj       : pmctrls.c         *.h ..\\headers\\*.h
selColor.obj      : selColor.c        *.h ..\\headers\\*.h
worker.obj        : worker.c          *.h ..\\headers\\*.h
util.obj          : util.c            *.h ..\\headers\\*.h
bitmaps.obj       : bitmaps.c         *.h ..\\headers\\*.h
fontdlg.obj       : fontdlg.c         *.h ..\\headers\\*.h
preview.obj       : preview.c         *.h ..\\headers\\*.h
test.obj          : test.c            *.h ..\\headers\\*.h
pageProdinfo.obj  : pageProdinfo.c    *.h ..\\headers\\*.h
pageSDGen.obj     : pageSDGen.c       *.h ..\\headers\\*.h
pageSDKill.obj    : pageSDKill.c      *.h ..\\headers\\*.h
pageSDProgs.obj   : pageSDProgs.c     *.h ..\\headers\\*.h
pageSDReboot.obj  : pageSDReboot.c    *.h ..\\headers\\*.h
pageUIBtn.obj     : pageUIBtn.c       *.h ..\\headers\\*.h
pageUIDlg.obj     : pageUIDlg.c       *.h ..\\headers\\*.h
pageUIGen.obj     : pageUIGen.c       *.h ..\\headers\\*.h
pageUITbar.obj    : pageUITbar.c      *.h ..\\headers\\*.h
pageUITbarGen.obj : pageUITbarGen.c   *.h ..\\headers\\*.h
pageWinKeys.obj   : pageWinKeys.c     *.h ..\\headers\\*.h
dlgSDtiming.obj   : dlgSDtiming.c     *.h ..\\headers\\*.h

