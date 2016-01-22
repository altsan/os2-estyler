/**/
"@echo off"
"set debug="
"mode co80,102"
call "..\ESTLRENV.CMD"

/* update the build number in estyler.def and stlrVersion.h                 */
if \ updateBuildNo("estyler.def", "..\headers\stlrVersion.h") then
   exit

/* if the flag file doesn't exist create it and touch all the sources */
if "" \= fileExists("@debug@") then
   do
      "del @debug@ > nul"
      call touchAllFiles "*.c"
   end /* do */

do forever
   "cls"
   "nmake32 -nologo"
   if RC = 0 then leave
   say "Premere un tasto qualsiasi per rieseguire NMAKE"
   say "Premere Ctrl-C per terminare."
   "pause > nul"
end /* do */
say "eStylerLite has been succesfully built"
"pause > nul"
exit

/* --------------------------------------------------------------------------
*/
extendPath:
   curDir = directory();
   parDir = directory("..");
   setval = parDir"\headers;"parDir"\APIexH;"
   incval = value("INCLUDE",,"OS2ENVIRONMENT");
   call value "INCLUDE", setval||incval, "OS2ENVIRONMENT"
   call directory curDir
   return

/* --------------------------------------------------------------------------
*/
touchAllFiles: procedure
parse arg filesp
   call SysFileTree filesp, "files", "FO"
   do i = 1 to files.0
      "touch "files.i
   end /* do */
   return

/* --------------------------------------------------------------------------
 write outdata to filename
 args   : name of the file to be written
          data to be written to the file
 expose : none
 return : 1/0 (success/error)
---------------------------------------------------------------------------*/
writeFile: procedure
   parse arg filename, outdata

   remaining = charout(filename, outdata, 1);
   call charout filename
   if 0 \= remaining then
      return 0
   return 1

/* --------------------------------------------------------------------------
 check for the existence of a file or a directory
 args   : filename name of the file or directory to be checked
 expose : none
 return : full file name or "" if the file or directory doesn't exist
---------------------------------------------------------------------------*/
fileExists: procedure
   parse arg filename

   call SysFileTree filename, "fe", "BO"
   if 1 = fe.0 then
      return fe.1
   return ""

/* ------------------------------------------------------------------------ */
/* update the build number in estyler.def and stlrVersion.h                 */
updateBuildNo: procedure
parse arg deffile, hdrfile

   /* read the files */
   defData = readFile(deffile);
   if defData = "" then
      return 0;
   hdrData = readFile(hdrfile);
   if verData = "" then
      return 0;

   /* get build no from the def file */
   buildVer = ""
   deftag1 = "#@eStylerLite V.1.1.";
   deftag2 = " - (c) 1998-2004 Alessandro Cantatore";
   parse var defData defStart (deftag1) buildVer (deftag2) defEnd;
   if buildVer = "" then
      do
         say " failed to get the build number from "deffile
         return 0;
      end /* do */
   parse var buildver buildver dbg .

   /* get build no from the header file */
   buildVer2 = ""
   hdrtag1 = "/*<buildno>*/ ";
   hdrtag2 = " /*</buildno>*/";
   parse var hdrData hdrStart (hdrtag1) buildVer2 (hdrtag2) hdrEnd;
   if buildVer2 = "" then
      do
         say " failed to get the build number from "hdrfile
         return 0;
      end /* do */

   /* check if the build no. is the same in both the files */
   if buildVer2 > buildVer then
      buildVer = buildVer2

   /* update the build no. in both the files */
/*
   defData = defStart""deftag1""buildVer + 1""deftag2""defEnd;
   hdrData = hdrStart""hdrtag1""buildVer + 1""hdrtag2""hdrEnd;
*/
   defData = defStart""deftag1""buildVer""deftag2""defEnd;
   hdrData = hdrStart""hdrtag1""buildVer""hdrtag2""hdrEnd;

   /* write the new data to the files */
   if \writeFile(deffile, defData) then
      return 0;
   if \writeFile(hdrfile, hdrData) then
      return 0;
   return 1;


/* ------------------------------------------------------------------------ */
readFile: procedure
parse arg filename
   flen = chars(filename)
   data = charin(filename, 1, flen)
   if data = "" then
      say "errore lettura "filename"."
   call charout filename
   return data


