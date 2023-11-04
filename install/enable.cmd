/******************************************************************************
 * ENABLE.CMD                                                                 *
 *                                                                            *
 * Enables Styler by adding the DLL to SYS_DLLS.                              *
 *                                                                            *
 * PARAMETERS: None                                                           *
 *                                                                            *
 * RETURN CODES:                                                              *
 *   0    Install completed successfully                                      *
 *   1    INI file update failed.                                             *
 ******************************************************************************/
SIGNAL ON NOVALUE

IF ( RxFuncQuery('SysCreateObject') \= 0 ) THEN DO
    CALL RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
    CALL SysLoadFuncs
END

err_ini = EnableStyler()
RETURN err_ini


/******************************************************************************
 * EnableStyler                                                               *
 *                                                                            *
 * Modifies the WPS SYS_DLLS::LoadPerProcess list to load the eStyler DLL.    *
 * Replaces the old (v1.0) version if found.                                  *
 ******************************************************************************/
EnableStyler: PROCEDURE
    bad     = 0
    estldll = "ESTLRL12"
    olddll = "ESTLRL1"
    inikey  = "LoadPerProcess"

    SAY "Adding Styler DLL to SYS_DLLS:LoadPerProcess ..."
    val = SysIni("USER", "SYS_DLLS", inikey )
    SAY '    Current:' val

    IF val \= 'ERROR:' THEN DO
        IF POS( estldll, val ) == 0 THEN DO
            IF POS( olddll'0', val ) > 0 THEN
                val = OVERLAY( estldll, val, POS( olddll'0', val ))
            ELSE IF POS( olddll'1', val ) > 0 THEN
                val = OVERLAY( estldll, val, POS( olddll'1', val ))
            ELSE
                val = STRIP( STRIP( val, "T", X2C("00"))) || " " || estldll || " " || "00"x
            SAY '    New:    ' val
            val = SysIni("USER", "SYS_DLLS", inikey, val )
        END
    END
    IF val == 'ERROR:' THEN DO
        SAY 'Update failed.'
        bad = 1
    END

RETURN bad


/******************************************************************************
 * SIGNAL HANDLERS                                                            *
 ******************************************************************************/
NOVALUE:
    SAY sigl '+++' SOURCELINE( sigl )
    SAY 'Unintialized variable.'
    EXIT sigl
RETURN

