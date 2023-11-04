Styler National Language Support Components
===========================================

To build a new languag using NMake32:

1) Modify `nls\makefile` (relative to the top-level repository directory)
   and copy the 'set_en' target block to 'set_ZZ>' where ZZ is the
   two-letter language code of the new language.  

2) Next, modify `makefile` in the top-level directory.  Add the new
   language code to the target list in the relevant block (it should be
   the last target before 'clean cleanall').  Then add 'cleanZZ' (where
   ZZ is the new language code) to the target list in the second last 
   block in the file.  Finally, add 'cleanZZ' to the status message in
   the very last line of the file.

NOTE: To build using kbuild, modify `Makefile.kmk` as follows:

1) Copy the block of lines prefixed with estylerEN_ and estylerEN-docs_ 
   to before the line `### New language defs go above this line`.  In
   all of the newly-copied lines, change 'EN' to the new two-letter 
   language code (same as the directory name, see below), in uppercase;
   and all instances of the language code 'en' in the same way, but in
   lowercase.  e.g.
	estylerEN_INST              = $(INST_BIN)/en/
   is copied to 
	estylerDE_INST              = $(INST_BIN)/de/
   etc.

2) Under the 'compile help files' section, copy the definition block for
   `$(PATH_OBJ)/en/styler.hlp` to the end of the file, and change the 'EN'
   and 'en' language codes in the new block as above.

3) In the `DLLS` line near the top of the file, add the target `estylerXX`
   where 'XX' is the language code.

4) In the `INSTALLS` line near the top of the file, add the target 
   `estylerXX-docs` where 'XX' is the language code.


Styler documentation
--------------------

Needed tools:
- IBM IPFC compiler

Instructions:
- copy en\doc to the required language directory:
    DE (German), EN (English), ES (Spanish), FR (French),
    IT (Italian), JA (Japanese), NL (Dutch), RU (Russian),
    TW (Chinese-Traditional), KO (Korean).
- translate the .htm files in the doc directory using a plain text
  editor (do not modify the html tags).
- translate the 2nd and 3rd lines of estyler.lst
- performing a full build from the top-level directory will build all
  Styler components for all defined languages.  To build (only) the
  documentation for a specific language, you can run:
    BUILDDOC.CMD <language>
- NOTE: all bitmaps resources (the one used for the default titlebar
  and pushbutton bitmaps) must be valid OS/2 bitmaps of version 1.x.
  If needed open them with the icon editor and save them as format 1.x.

Styler resources
----------------

Needed tools:
- IBM RC.EXE version 4.x (there is no guarantee that the resource
  scripts be interpreted correctly by a newer version).  The OS/2
  Toolkit includes a copy of this version as `rc16.exe`.
- IBM Visual Age ILINK.EXE

Instructions:
- copy en\res to the required language directory:
    DE (German), EN (English), ES (Spanish), FR (French),
    IT (Italian), JA (Japanese), NL (Dutch), RU (Russian),
    TW (Chinese-Traditional).
- translate the C header file textres.h
- performing a full build with 'kmk' from the top-level directory will
  build all Styler components for all defined languages.  To build
  (only) the resources for a specific language, you can run:
    BUILDRES.CMD <language>

### Important note:
When the resources for the language XX are compiled, the script defines
NLV_XX (for instance when compiling German resources, i.e. "DE"
language, the NLV_DE is defined). Therefore when it is necessary to
modify the dialog resources to fit the text captions of a particular
language, it is necessary to:
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
