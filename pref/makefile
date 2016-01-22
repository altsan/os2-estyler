# Build preferences setting utility
# $Id: makefile,v 1.3 2011/08/25 22:24:12 shl Exp $

# To add new extensions to the inference rules uncomment and redefine this:
#.SUFFIXES:
#
#.SUFFIXES: \
#    .C .obj .rc .res

# compiler, linker, resource compiler, resource binder MACRO
CC = icc.exe
CL = ilink.exe
RC = rc.exe
RB = rc.exe

# compiler and linker flags

EXTRAFLAGS =
# To build with APIEX000.DLL set the following and remove apixwrap.obj from
# both the dependencies and the link file below (there should be a better way to
# automate this). Also define %ECSINSTSRC%, pointing to the GuidProc source dir,
# and change APIEXLIB in ESTLRENV.CMD to APIEX000.LIB (full path).
# EXTRAFLAGS = /DUSE_APIEX=1

# Debug version
!ifdef DEBUG

CFLAGS = /I..\\headers /I$(APIEXINCL) /I$(LVMTKINCL) /Ss /Ti /Rn /G5 /C /Q
LFLAGS = /DE /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L

!else
# RELEASE version

CFLAGS = /I..\\headers /I$(APIEXINCL) /I$(LVMTKINCL) /Ss /O /Oc /Ol /Rn /G5 /C /Q $(EXTRAFLAGS)
LFLAGS = /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L

!endif

RCFLAGS = -r $(EXTRAFLAGS)
RBFLAGS = -x2

.rc.res:
   $(RC) $(RCFLAGS) $<

.C.obj:
   $(CC) $(CFLAGS) $<

all: eStyler.exe

eStyler.exe:         \
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
   $(APIEXLIB)       \
   $(LVMLIB)         \
   eStyler.def
   $(CL) @<<
      $(LFLAGS)
      /O:eStyler.exe
      $(APIEXLIB)
      $(LVMLIB)
      eStyler.def
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
    $(RB) $(RBFLAGS) eStyler.res eStyler.exe

eStyler.res:        eStyler.rc        ..\\nls\\res\\Styler.ICO
    $(RC) $(RCFLAGS) eStyler.rc eStyler.res

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


