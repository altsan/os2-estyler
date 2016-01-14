/**/
"@echo off"
"set debug=1"
/*"mode co80,102"*/
call "..\ESTLRENV.CMD"

/* call extendPath */

/* if the flag file doesn't exist create it and touch all the sources */
if "" = fileExists("@debug@") then
   do
      call writeFile "@debug@", "this is a flag file: do not delete!"
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
