There are 2 global variables:
- the GLOBAL structure which is declared as  'g'
- the OPTIONS structure which is declared as 'o'

globally visible macros are prefixed by 'm_' and defined in
\headers\macros.h (for instance m_setVersion() )

locally visible macros are defined at the top of the C
source file or in a header file with the same name of the C
file and prefixed by 'm' followed by an uppercase
character (e.g. mIsPushBtn() )

