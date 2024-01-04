#######################################################################
#
# Styler\install makefile
#
# This builds Styler WarpIN install package
#
# Tools used: nmake32  WarpIN
#

WIC = runwic.cmd

#######################################################################
#
# Variables
#
TOPDIR = ..

# this pulls in Styler's version number
!include "$(MAKEDIR)\..\headers\stlrVersion.h"

WPI_SUFFIX = $(VERMAJ)_$(VERMIN)_$(VERBUILD)

PKG_DIRS_COMMON = 1 2 3 3\bitmap 3\bitmap\button 3\bitmap\tbkgnds 4

#######################################################################
#
# Targets
#

all: base en es ru

base:
    %foreach d in $(PKG_DIRS_COMMON)
      -mkdir $(MAKEDIR)\$(d) 2> nul
    %endfor
    copy $(TOPDIR)\shared\util\afcutl05.dll $(MAKEDIR)\1\afcutl05.dll
    copy $(TOPDIR)\maindll\estlrl12.dll $(MAKEDIR)\1\estlrl12.dll
    copy $(TOPDIR)\maindll\estlrl12.map $(MAKEDIR)\2\estlrl12.map
    copy $(TOPDIR)\maindll\estlrl12.sym $(MAKEDIR)\2\estlrl12.sym
    copy $(TOPDIR)\pref\styler.exe $(MAKEDIR)\2\styler.exe
    copy $(TOPDIR)\pref\styler.map $(MAKEDIR)\2\styler.map
    copy $(TOPDIR)\pref\styler.sym $(MAKEDIR)\2\styler.sym
    copy $(MAKEDIR)\disable.cmd $(MAKEDIR)\2\disable.cmd
    copy $(MAKEDIR)\enable.cmd $(MAKEDIR)\2\enable.cmd
    copy $(TOPDIR)\CLI^-shutdown\shutdown.map $(MAKEDIR)\2\shutdown.map
    copy $(TOPDIR)\CLI^-shutdown\shutdown.sym $(MAKEDIR)\2\shutdown.sym
    copy $(TOPDIR)\CLI^-shutdown\shutdown.exe $(MAKEDIR)\4\shutdown.exe
    copy $(TOPDIR)\pref\bitmap\button\*.bmp $(MAKEDIR)\3\bitmap\button
    copy $(TOPDIR)\pref\bitmap\tbkgnds\*.bmp $(MAKEDIR)\3\bitmap\tbkgnds

nlv:
    %set l = $(ARGS)
    %if $l==de
      %set CTRY = 49
    %elif $l==es
      %set CTRY = 34
    %elif $l==ja
      %set CTRY = 81
    %elif $l==nl
      %set CTRY = 31
    %elif $l==ru
      %set CTRY = 07
    %elif $l==tw
      %set CTRY = 88
    %else
      %set CTRY = 01
    %endif
    %echo Copying $l files
    -mkdir $(MAKEDIR)\10$(CTRY) 2> nul
    -mkdir $(MAKEDIR)\20$(CTRY) 2> nul
    copy $(TOPDIR)\nls\$l\res\estlrlrs.dll $(MAKEDIR)\10$(CTRY)\estlrlrs.dll
    copy $(TOPDIR)\nls\$l\doc\styler.hlp $(MAKEDIR)\20$(CTRY)\styler.hlp
    $(WIC) styler-$l-$(WPI_SUFFIX).wpi -a 1 -r -c1 *
    $(WIC) styler-$l-$(WPI_SUFFIX).wpi -a 2 -r -c2 *
    $(WIC) styler-$l-$(WPI_SUFFIX).wpi -a 3 -r -c3 *
    $(WIC) styler-$l-$(WPI_SUFFIX).wpi -a 4 -r -c4 *
    $(WIC) styler-$l-$(WPI_SUFFIX).wpi -a 10$(CTRY) -r -c10$(CTRY) *
    $(WIC) styler-$l-$(WPI_SUFFIX).wpi -a 20$(CTRY) -r -c20$(CTRY) *
    $(WIC) styler-$l-$(WPI_SUFFIX).wpi -s styler-$l.wis


de en es ja nl ru tw:
    %set ARGS = $@
    %do nlv


clean cleanall cleaninstall:
    %echo cleaning install
    -@del styler-*-$(WPI_SUFFIX).wpi 2> nul
    -@del 1\*.dll 2> nul
    -@del 2\*.exe 2> nul
    -@del 2\*.map 2> nul
    -@del 2\*.sym 2> nul
    -@del 2\*.cmd 2> nul
    -@del 2\*.ini 2> nul
    -@del 3\bitmap\button\*.bmp 2> nul
    -@del 3\bitmap\tbkgnds\*.bmp 2> nul
    -@del 4\*.exe 2> nul
    -@del 1001\*.dll 2> nul
    -@del 2001\*.hlp 2> nul
    -@del 1007\*.dll 2> nul
    -@del 2007\*.hlp 2> nul
    -@del 1031\*.dll 2> nul
    -@del 2031\*.hlp 2> nul
    -@del 1034\*.dll 2> nul
    -@del 2034\*.hlp 2> nul
    -@del 1049\*.dll 2> nul
    -@del 2049\*.hlp 2> nul
    -@del 1081\*.dll 2> nul
    -@del 2081\*.hlp 2> nul
    -@del 1088\*.dll 2> nul
    -@del 2088\*.hlp 2> nul

