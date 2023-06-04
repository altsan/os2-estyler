/******************************************************************************
 * DISABLE.CMD                                                                *
 *                                                                            *
 * Disables Styler by removing the DLL from SYS_DLLS.                         *
 *                                                                            *
 * PARAMETERS: None                                                           *
 *                                                                            *
 * RETURN CODES:                                                              *
 *   0    Completed successfully                                              *
 *   1    INI file update failed.                                             *
 ******************************************************************************/
SIGNAL ON NOVALUE

IF ( RxFuncQuery('SysCreateObject') \= 0 ) THEN DO
    CALL RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
    CALL SysLoadFuncs
END

err_ini = DisableStyler()
RETURN err_ini


/******************************************************************************
 * DisableStyler                                                              *
 *                                                                            *
 * Modifies the WPS SYS_DLLS::LoadPerProcess list to remove the Styler DLL.   *
 *                                                                            *
 ******************************************************************************/
DisableStyler: PROCEDURE
    bad     = 0
    estldll = 'ESTLRL12'
    inikey  = 'LoadPerProcess'

    SAY 'Removing Styler DLL from SYS_DLLS:LoadPerProcess ...'
    val = SysIni('USER', 'SYS_DLLS', inikey )
    SAY '    Current:' val

    IF val \= 'ERROR:' THEN DO
        CALL StringTokenize val, ' '
        delidx = SearchTokens( estldll )

        list = ''
        DO i = 1 to tokens.0
            tokens.i = STRIP( tokens.i )
            IF tokens.i == '' | tokens.i == '00'x THEN ITERATE
            IF i \= delidx THEN list = list || tokens.i || ' '
        END
        list = list || '00'x

        SAY '    New:    ' list
        val = SysIni('USER', 'SYS_DLLS', inikey, list )
        IF ( val = 'ERROR:') THEN DO
            SAY 'Error encountered:' val
            bad = 1
        END
        ELSE DO
            SAY 'Success.'
            bad = 0
        END

        DROP tokens.
    END

RETURN bad


/******************************************************************************
 * StringTokenize                                                             *
 *   Utility function to tokenize a string using the given separator.         *
 ******************************************************************************/
StringTokenize: PROCEDURE EXPOSE tokens.
    PARSE ARG string, separator

    IF ( string = '') THEN RETURN string
    IF ( separator = '') THEN separator = ' '

    i        = 0
    tokens.0 = i
    string = STRIP( string, 'B', separator )
    DO WHILE LENGTH( string ) > 0
        x = 1
        y = POS( separator, string, x )
        IF y > 0 THEN DO
            current = SUBSTR( string, 1, y-1 )
            x = y + 1
            i = i + 1
            tokens.i = current
        END
        ELSE DO
            current = STRIP( string, 'B', separator )
            i = i + 1
            tokens.i = current
            x = LENGTH( string ) + 1
        END
        string = SUBSTR( string, x )
    END
    tokens.0 = i

RETURN


/******************************************************************************
 * SearchTokens                                                               *
 *   Utility function to search the 'tokens.' stem and return the index which *
 *   contains the requested string, or 0 if the string is not found.          *
 ******************************************************************************/
SearchTokens: PROCEDURE EXPOSE tokens.
    PARSE ARG target, matchcase

    IF matchcase = '' THEN matchcase = 0

    found = 0
    DO i = 1 TO tokens.0
        IF matchcase = 0 THEN DO
            IF TRANSLATE( target ) = TRANSLATE( tokens.i ) THEN found = i
        END
        ELSE DO
            IF target = tokens.i THEN found = i
        END
    END

RETURN found


/******************************************************************************
 * SIGNAL HANDLERS                                                            *
 ******************************************************************************/
NOVALUE:
    SAY sigl '+++' SOURCELINE( sigl )
    SAY 'Unintialized variable.'
    EXIT sigl
RETURN

