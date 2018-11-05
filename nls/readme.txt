eStyler National Language Support Components
============================================

eStyler documentation
---------------------

Needed tools:
- IBM IPFC compiler

Instructions:
- copy en\doc to the required language directory:
  DE (German), EN (English), ES (Spanish), FR (French),
  IT (Italian), JA (Japanese), NL (Dutch), RU (Russian),
  TW (Chinese-Traditional).
- translate the .htm files in the doc directory
  using a plain text editor (do not modify the
  html tags).
- translate the 2nd and 3rd lines of estyler.lst
- performing a full build with 'kmk' from the top-level
  directory will build all eStyler components for all
  defined languages.  To build (only) the documentation
  for a specific language, you can run:
    BUILDDOC.CMD <language>
- NOTE: all bitmaps resources (the one used for the
  default titlebar and pushbutton bitmaps) must be
  valid OS/2 bitmaps of version 1.x.
  If needed open them with the icon editor and save
  them as format 1.x.

eStyler resources
-----------------

Needed tools:
- IBM RC.EXE version 4.x (there is no guarantee that the
  resource scripts be interpreted correctly by a newer
  version)
- IBM Visual Age ILINK.EXE

Instructions:
- copy en\res to the required language directory:
  DE (German), EN (English), ES (Spanish), FR (French),
  IT (Italian), JA (Japanese), NL (Dutch), RU (Russian),
  TW (Chinese-Traditional).
- translate the C header file textres.h
- performing a full build with 'kmk' from the top-level
  directory will build all eStyler components for all
  defined languages.  To build (only) the documentation
  for a specific language, you can run:
     BUILDRES.CMD <language>

Important note:
When the resources for the language XX are compiled,
the script defines NLV_XX (for instance when compiling
German resources, i.e. "DE" language, the NLV_DE is
defined). Therefore when it is necessary to modify
the dialog resources to fit the text captions of a
particular language, it is necessary to:
- open the nls\res folder
- copy the original .dlg file as XXfilename.dlg
  (e.g. main.dlg -> DEmain.dlg)
- modify the main rc script (estylerRes.rc) to include
  the modified files only when NLV_XX is defined.
  For instance:
  #if defined(NLV_DE)
     RCINCLUDE "DEmain.dlg"
  #elif defined(NLV_IT)
     RCINCLUDE "ITmain.dlg"
  #else
     RCINCLUDE "main.dlg"
  #endif
