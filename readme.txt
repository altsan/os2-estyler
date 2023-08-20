=====================================================
IMPORTANT NOTICE
=====================================================
The current sources can now be built using IBM NMake32.  The
previous kbuild system may still work (no promises) but is
not currently maintained.

Building Styler requires the IBM C/C++ Compiler (version 
3.65 is recommended) and the OS/2 4.5x Toolkit.

- Run 'nmake32' from the top-level directory to build all components.
- Run 'nmake32' from the 'install' directory to build the WarpIN
  install package.

The proper environment for the IBM compiler must be present.

To use kbuild, create localConfig.kmk as appropriate and run
'kmk' to build. You must have kbuild, as well as the OS/2
Toolkit and the IBM compiler, installed.

The instructions below describe the old build system as
implemented by Alessandro Cantatore. They are officially
deprecated and should no longer be used (and in any event are
unlikely to work with the current codebase).

=====================================================
ORIGINAL README.TXT TEXT FOLLOWS
=====================================================


-[ ESTLRENV.CMD ]------------------------------------------

The preferences notebook of eStyler shares some headers and
libraries with the eCS installer.
ESTLRLENV.CMD is used to define the path of the eCS installer.
To correctly build eStyler ESTLRENV.CMD must be modified.


-[ headers ]-----------------------------------------------

This directory contains some header files used both by the
main eStyler dll (i.e. estlrl11.dll), the resources dll
(ESTLRLRS.dll) and the settings notebook program (estyler.exe).

Profile.txt contains some notes about the organization of
eStyler.ini.


-[ nls ]---------------------------------------------------

This directory contains the files needed for building
the resources DLL.
To build non-English language versions, copy the content
of the en subdirectory to the appropriate subdirectory
(de, es, fr, it, nl, ru), translate the files and drag
the subfolder contatining the translated files (e.g. fr)
over BUILDDOC.CMD to build a new ESTYLER.HLP file and
over BUILDRES.CMD to build a new ESTLRLRS.DLL file or
type from a command line:
BUILDDOC <languagedir>
BUILDRES <languagedir>

The subdirectory IPFimages is provided for hosting bitmaps
used by the ESTYLER.HLP file. Currently it is empty as
the help file does not display any image.

The subdirectory res contains common files used to build
the resources DLL.

The subdirectory tools contains a program to translate
the documentation from html to ipf format.

The file nls/readme.txt contains more detailed informations
about national language version support.

Note: the program used to build the help file translating
html documentation to ipf will be updated soon.


-[ maindll ]-----------------------------------------------

This directory contains the files needed to build the
main eStyler DLL (estlrl11.dll).
The DLL created via DBUILD.CMD prints some messages to a log
file for debugging purposes.
The DLL created via RBUILD.CMD does not contain any debug
code and is the one which should be distributed to the users.


-[ pref ]--------------------------------------------------

This directory contains the files used to build the settings
notebook program.
Two scripts: DBUILD.CMD and RBUILD.CMD are provided to build
the debug and the release version of the program.