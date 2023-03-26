#######################################################################
#
# Styler\nls makefile
#
# This builds Styler's NLS-specific resource dlls and help files.
# Available targets are: all de en es ja nl ru tw
#                        clean cleanXX (where XX = NLV)
#
# Note that the top-level target 'NLV' _must_ have a different name than
# the 'NLS' directory to keep nmake32 from thinking 'NLS' is up-to-date.
#
# Tools used: nmake32  ipfc  ilink(v5)  rc(v4)
#
# N.B. you can also build a specific NLV from the top-level makefile


#######################################################################
#
# Variables
#

# this pulls in Styler's version number
!include "$(MAKEDIR)\..\headers\stlrVersion.h"

# no inference rules are used, so no suffixes
.SUFFIXES:

# all supported languages
NLV_LIST = de en es ja nl ru tw

# languages with help files
DOC_LIST = de en es ja nl ru

# source directory for the resources dll
SRC_PATH = $(MAKEDIR)\res

# parts of RC's command line
RC_OPTS = -r -n -d NLV_$(NLV) -cp $(CODEPAGE) -cc $(COUNTRY)
RC_INCL = -i $(MAKEDIR)\$(NLV)\res -i $(MAKEDIR)\..\headers -i $(MAKEDIR)\..\shared\headers

# BldLevelInf.cmd's command line and options
BLDLVL = $(MAKEDIR)\..\shared\util\BldLevelInf.cmd
BLDLVL_OPTS  = -V"$(VERMAJ).$(VERMIN)" -M"$(VERBUILD)"
BLDLVL_OPTS += -P"$(VERMAJ).$(VERMIN)-$(VERBUILD)"
BLDLVL_OPTS += -N"Alessandro Cantatore" -D"Styler $(NLV) resources"

BLDLVL_SRC   = $(MAKEDIR)\$(NLV)\res\estlrlrs.def
BLDLVL_TMP   = $(MAKEDIR)\$(NLV)\res\temp.def

# target path(s) for help file(s)
DOC_PATH = $(MAKEDIR)\$(NLV)\doc

# parts of IPFC's command line
IPF_OPTS = -D:$(COUNTRY) -C:$(CODEPAGE) -L:$(LANGCODE)


#######################################################################
#
# Targets
#

all: $(NLV_LIST)

# Generate a target for each NLV's resource dll and a separate target
# with the same name for NLVs that have help files (i.e. all but TW).
#
# N.B. The $(XX) variable is only usable at read-time. To get the correct
# values at run-time, each target has a dependency on a psuedotarget that
# will set various NLV variables before any other commands run.

# generate targets to build the resource dll
!foreach XX [in] $(NLV_LIST)

$(XX): \
        set_$(XX) \
        $(MAKEDIR)\$(XX)\res\estlrlrs.dll

$(MAKEDIR)\$(XX)\res\estlrlrs.dll: \
        $(MAKEDIR)\$(XX)\res\estlrlrs.res \
        $(MAKEDIR)\$(XX)\res\estlrlrs.def \
        $(SRC_PATH)\estlrlrs.obj
    @echo .
    $(BLDLVL) $(BLDLVL_SRC) $(BLDLVL_TMP) $(BLDLVL_OPTS)
    ilink /NOL /DLL /OUT:$@ $(SRC_PATH)\estlrlrs.obj $(BLDLVL_TMP)
    -@del $(BLDLVL_TMP) > nul 2>&1
    rc -x2 -n $*.res $@

$(MAKEDIR)\$(XX)\res\estlrlrs.res: \
        $(SRC_PATH)\*.rc \
        $(SRC_PATH)\*.dlg \
        $(MAKEDIR)\$(XX)\res\textres.h
    %cd $(SRC_PATH)
    @echo .
    rc $(RC_OPTS) $(RC_INCL) $(SRC_PATH)\estylerres.rc $@
    %cd $(MAKEDIR)

!endfor

# generate targets to build the help file
!foreach XX [in] $(DOC_LIST)

$(XX): \
        set_$(XX) \
        $(MAKEDIR)\$(XX)\doc\styler.hlp

$(MAKEDIR)\$(XX)\doc\styler.hlp: \
        $(MAKEDIR)\$(XX)\doc\estyler.ipf
    %cd $(DOC_PATH)
    @echo .
    ipfc $(IPF_OPTS) estyler.ipf styler.hlp
    %cd $(MAKEDIR)

$(MAKEDIR)\$(XX)\doc\estyler.ipf: \
        $(MAKEDIR)\$(XX)\doc\estyler.lst \
        $(MAKEDIR)\$(XX)\doc\*.html
    %cd $(DOC_PATH)
    @echo .
    SET BEGINLIBPATH=$(MAKEDIR)\tools
    $(MAKEDIR)\tools\ipfbld estyler.lst
    %cd $(MAKEDIR)

!endfor


# NLV psuedotargets to set variables at run-time

set_de:
    %set NLV = DE
    %set CODEPAGE = 850
    %set COUNTRY = 049
    %set LANGCODE = DEU

set_en:
    %set NLV = EN
    %set CODEPAGE = 850
    %set COUNTRY = 001
    %set LANGCODE = ENU

set_es:
    %set NLV = ES
    %set CODEPAGE = 850
    %set COUNTRY  = 034
    %set LANGCODE = ESP

set_ja:
    %set NLV = JA
    %set CODEPAGE = 932
    %set COUNTRY = 081
    %set LANGCODE = JPN

set_nl:
    %set NLV = NL
    %set CODEPAGE = 850
    %set COUNTRY = 031
    %set LANGCODE = NLD

set_ru:
    %set NLV = RU
    %set CODEPAGE = 866
    %set COUNTRY  = 007
    %set LANGCODE = RUS

set_tw:
    %set NLV = TW
    %set CODEPAGE = 950
    %set COUNTRY = 088
    %set LANGCODE = CHT


# Cleanup

!foreach XX [in] $(NLV_LIST)
!set CLEAN_LIST = $(CLEAN_LIST) clean$(XX)
!endfor

clean cleanall cleannlv: cleanmsg $(CLEAN_LIST)

!foreach XX [in] de en es ja nl ru tw
clean$(XX):
    %set CC = $[c,$@,6,7]
    -@del $(MAKEDIR)\$(CC)\res\estlrlrs.res 2> nul
    -@del $(MAKEDIR)\$(CC)\res\estlrlrs.dll 2> nul
    -@del $(MAKEDIR)\$(CC)\doc\estyler.ipf 2> nul
    -@del $(MAKEDIR)\$(CC)\doc\styler.hlp 2> nul
!endfor

cleanmsg:
    @echo cleaning all NLS directories

