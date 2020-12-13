/* --------------------------------------------------------------------------
 This script creates the eStylerLite help file.
 Usage:
 BUILDDOC <language>
 where language can be:
     DE (German), EN (English), ES (Spanish), FR (French), IT (Italian),
     JA (Japanese), NL (Dutch), RU (Russian), TW (Chinese-Traditional).
-------------------------------------------------------------------------- */

"@echo off"
arg language
language = filespec(name, language)

/* name of the target to be built */
target = "estyler";

/* get the IPFC parameters (according to the language) */
parms = getIpfcParms(language)
if "" = parms then
   exit;

/* set the path of the directory containing the images */
curpath = getScriptPath();
"SET BEGINLIBPATH="curpath"TOOLS"
artwork = curpath"IPFimages";
call value "IPFCARTWORK", artwork, "OS2ENVIRONMENT";

path = language"\doc"
newpath = directory(path)
if newpath = "" then
   do
      say "Failed to set "path" as working directory."
      "pause"
      exit
   end /* do */


"MODE CO80,102"
curpath"TOOLS\IPFBLD.EXE "target".LST"
"pause"
"IPFC.EXE "parms" "target".IPF"
"cd ..\.."
exit

/* ----------------------------------------------------------------------- */


/* build the IPFC parameters --------------------------------------------- */
getIpfcParms: procedure
   parse arg language

   nlvcodes =  "DE  EN  ES  FR  IT  JA  NL  RU  TW";
   ctrycodes = "049 001 034 033 039 081 031 007 088";
   codepages = "850 850 850 850 850 932 850 866 950";
   langcodes = "DEU ENU ESP FRA ITA JPN NLD RUS CHT";
   wordidx = wordpos(language, nlvcodes);
   if 0 = wordidx then
      do
         call help
         return ""
      end
   country = word(ctrycodes, wordidx);
   codepage = word(codepages, wordidx);
   language = word(langcodes, wordidx);
   if ("" = country) | ("" = codepage) | ("" = language) then
      do
         call help
         return ""
      end /* do */
   parms = "-x -D:"country" -C:"codepage" -L:"language
   return parms


/* show the help message ------------------------------------------------- */
help: procedure
   do i = 2 to 7
      say sourceline(i);
   end /* do */
   "pause"
return


/* get the path of this script ------------------------------------------- */
getScriptPath: procedure
   parse source . . scriptspec
return filespec("DRIVE", scriptspec) || filespec("PATH", scriptspec)

