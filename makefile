#######################################################################
#
# Styler top-level makefile
#
# This builds the 3 main components of Styler (dll exe nlv)
# as well as specific NLS targets (de en es ja nl ru tw).
# Clean a target by prefixing 'clean' to its name (e.g. cleanja)
#
# Tools used: nmake32  ipfc  ilink(v5)  rc(v4)
#             vacpp(v3.08 or v3.65)
#
#
# Set the path to the ACPI Toolkit headers here or in the environment

!ifndef ACPI_INCL
%setenv ACPI_INCL = $(MAKEDIR)\shared\acpi_tk\h
!endif


#######################################################################
#
# Targets
#
# FYI... the '-=' switch below avoids rebuilding an up-to-date
# target when it has the same timestamp as one of its dependents

all: dll exe nlv

dll:
    %cd $(MAKEDIR)\maindll
    @echo .
    @nmake32 -= $(ARGS)

exe:
    %cd $(MAKEDIR)\pref
    @echo .
    @nmake32 -= $(ARGS)

nlv:
    %cd $(MAKEDIR)\nls
    @echo .
    @nmake32 -= $(ARGS)

de en es ja nl ru tw:
    %set ARGS = $@
    %do nlv


clean cleanall: clean_msg cleandll cleanexe cleannlv

cleandll cleanexe cleannlv:
    %set ARGS = -c $@
    %do $[c,$@,6,8]

cleande cleanen cleanes cleanja cleannl cleanru cleantw:
    %set ARGS = -c $@
    %do nlv

clean_msg:
    @echo Other targets: cleanall cleandll cleanexe cleannlv
    @echo + cleande cleanen cleanes cleanja cleannl cleanru cleantw

