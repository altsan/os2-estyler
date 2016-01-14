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

# Debug version
!ifdef DEBUG
CFLAGS = /I..\headers /I$(APIEXINCL) /Ss /Q /Ti /Rn /Ge- /G5 /C /DDEBUGGING=2
LFLAGS = /DE /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L

!else
# RELEASE version
CFLAGS = /I..\headers /I$(APIEXINCL) /Ss /Q /O /Oc /Rn /Ge- /G5 /C
LFLAGS = /E:2 /PM:PM /PACKD /A:4 /OPTF /NOL /M /L

!endif

!ifdef ACPI_ENABLED
CFLAGS = $(CFLAGS) /I$(ACPIINCL) /DACPI_ENABLED
# LIBS   = $(ACPILIB)

!else
LIBS=

!endif


.c.obj:
   $(CC) $(CFLAGS) $<

all: estlrl11.dll

# ESTLRL11.DLL
estlrl11.dll: \
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
   $(CL) @<<
      $(LFLAGS)
      /O:estlrl11.dll
      estyler.def
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

