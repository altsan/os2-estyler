/* ESTLRENV.CMD - set up estyler build environment

   Uses ECSINSTSRC from environment if available.
   Otherwise finds guidproc sources assuming well-known location.
   Otherwise uses a hard-coded value which you will have to edit.
   The hard-code values are marked with EDITTHIS comment markers

   2011-08-21 SHL Make more system neutral
*/

/* set some environment variables used by the various D/RBUILD.CMD files */

PARSE SOURCE . . me .
topdir = FILESPEC('DRIVE', me ) || FILESPEC('PATH', me )

/* Try to guess location of guidproc sources */
do 1
  /* Allow preset from environment */
  ECSINSTSRC = value("ECSINSTSRC",, "OS2ENVIRONMENT")
  if ECSINSTSRC \== '' then leave
  /* Try to derive from well known file */
  s = stream(topdir || '..\guidproc\APIexDLL\apiex000.lib', 'C', 'QUERY EXISTS')
  i = lastpos('\', s)
  if i >= 1 then
    i =  lastpos('\', s, i - 1)
  if i >= 2 then do
    ECSINSTSRC = left(s, i - 1)
    leave
  end
  /* Give up and use hard-coded value - EDITTHIS */
  ECSINSTSRC = topdir || "shared"
end /* do */

/* Try to guess location of ACPI toolkit */
do 1
  /* Allow preset from environment */
  ACPITKDIR = value("ACPITKDIR",, "OS2ENVIRONMENT")
  if ACPITKDIR \== '' then leave
  /* Give up and use hard-coded value - EDITTHIS */
  ACPITKDIR = topdir || "shared\acpi_tk"
end /* do */

/* ---------------- end of edit section ------------------- */

APIEXINCL = ECSINSTSRC"\headers"
/*APIEXLIB  = ECSINSTSRC"\APIexDLL\apiex000.lib"*/
APIEXLIB  = ECSINSTSRC"\util\afcutl05.lib"
LVMTKINCL = ECSINSTSRC"\lvm_tk"
LVMLIB    = LVMTKINCL"\lvm.lib"
ACPIINCL  = ACPITKDIR"\h;"ACPITKDIR"\h\platform"
ACPILIB   = ACPITKDIR"\lib\acpi32.lib"

call value "APIEXINCL", APIEXINCL, "OS2ENVIRONMENT";
call value "APIEXLIB", APIEXLIB, "OS2ENVIRONMENT";
call value "LVMTKINCL", LVMTKINCL, "OS2ENVIRONMENT";
call value "LVMLIB", LVMLIB, "OS2ENVIRONMENT";

call value "ACPIINCL", ACPIINCL, "OS2ENVIRONMENT";
call value "ACPILIB", ACPILIB, "OS2ENVIRONMENT";
call value "ACPI_ENABLED", "1", "OS2ENVIRONMENT";

