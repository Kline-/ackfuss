/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Ack 2.2 improvements copyright (C) 1994 by Stephen Dooley              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *       _/          _/_/_/     _/    _/     _/    ACK! MUD is modified    *
 *      _/_/        _/          _/  _/       _/    Merc2.0/2.1/2.2 code    *
 *     _/  _/      _/           _/_/         _/    (c)Stephen Zepp 1998    *
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3          *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#include "h/globals.h"

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

#ifndef DEC_STRFUNS_H
#include "h/strfuns.h"
#endif

void pre_parse( char *list, char *victimname, char *containername, char *things )
{
    char arg1[MSL];
    char container_name[MSL];
    char one_object[MSL];
    char holdbuf[MSL];
    char victim_name[MSL];
    char object_list[MSL];
    char *argument = list;
    container_name[0] = '\0';
    victim_name[0] = '\0';
    object_list[0] = '\0';
    for ( ;; )
    {
        argument = one_argument( argument, arg1 );
        if ( arg1[0] == '\0' )
            break;
        if ( ( !str_cmp( "from", arg1 ) ) || ( !str_cmp( "in", arg1 ) ) )
        {
            argument = one_argument( argument, container_name );
        }
        else if ( !str_cmp( "to", arg1 ) )
        {
            argument = one_argument( argument, victim_name );
        }
        else
        {
            if ( is_number( arg1 ) )
            {
                argument = one_argument( argument, one_object );
                snprintf( holdbuf, MSL, "%s %s ", arg1, one_object );
                strncat( object_list, holdbuf, MSL );
            }
            else
            {
                snprintf( holdbuf, MSL, "1 %s ", arg1 );
                strncat( object_list, holdbuf, MSL );
            }
        }
    }
    strcpy( victimname, victim_name );
    strcpy( things, object_list );
    strcpy( containername, container_name );
    return;
}



/*
 * Return true if an argument is completely numeric.
 */
bool is_number( char *arg )
{
    if ( *arg == '\0' )
        return FALSE;

    if ( *arg == '+' || *arg == '-' )
        arg++;

    for ( ; *arg != '\0'; arg++ )
    {
        if ( !isdigit( *arg ) )
            return FALSE;
    }

    return TRUE;
}



bool is_name( const char *str, const char *namelist )
{
    char name[MAX_INPUT_LENGTH];
    char* _namelist = const_cast<char*>(namelist);

    for ( ;; )
    {
        _namelist = one_argument( _namelist, name );
        if ( name[0] == '\0' )
            return FALSE;
        if ( !str_cmp( str, name ) )
            return TRUE;
    }
}

bool is_name_pfx( const char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH];

    for ( ;; )
    {
        namelist = one_argument( namelist, name );
        if ( name[0] == '\0' )
            return FALSE;
        if ( !str_prefix( str, name ) )
            return TRUE;
    }
}

char *space_pad( const char *str, short final_size )
{
    short space_pad = nocol_strlen( str );
    static char padbuf[MSL];

    snprintf( padbuf, MSL, "%s", str );

    if ( space_pad > final_size )
    {
        padbuf[final_size] = '\0';
        return padbuf;
    }

    for ( ; space_pad < final_size; space_pad++ )
        strncat( padbuf, " ", MSL );
    return padbuf;
}


/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 * Kept for backwards compatibility --Kline
 */
void smash_tilde( char *str )
{
    smash_replace(str, "~", "-");
    return;
}

/*
 * Remove and replace in a string.
 * Easier than making X smash_Y functions. --Kline
 */
void smash_replace( char *str, char *sold, char *snew )
{
    for ( ; *str != '\0'; str++ )
    {
        if ( *str == *sold )
            *str = *snew;
    }
    return;
}

/* Lets start working strings in here. Helping slowly do the conversion. --Kline */
bool str_cmp( const char *astr, string str )
{
    int count = 0;
    bool notrunc = false;
    const char *bstr = str.c_str();

    if ( astr == NULL )
        return true;
    if ( bstr == NULL )
        return true;
    if ( *astr == '^' )
    {
        notrunc = true;
        astr++;
    }
    if ( *bstr == '^' )
    {
        notrunc = true;
        bstr++;
    }
    if ( astr == NULL )
        return true;
    if ( bstr == NULL )
        return true;
    for ( ; *astr || *bstr; astr++, bstr++ )
    {
        if ( !notrunc )
            if ( *astr == '*' && count )
                return false;
        if ( LOWER(*astr) != LOWER(*bstr) )
            return true;
        count++;
    }

    return false;
}

bool str_cmp( string str, const char *bstr )
{
    int count = 0;
    bool notrunc = false;
    const char *astr = str.c_str();

    if ( astr == NULL )
        return true;
    if ( bstr == NULL )
        return true;
    if ( *astr == '^' )
    {
        notrunc = true;
        astr++;
    }
    if ( *bstr == '^' )
    {
        notrunc = true;
        bstr++;
    }
    if ( astr == NULL )
        return true;
    if ( bstr == NULL )
        return true;
    for ( ; *astr || *bstr; astr++, bstr++ )
    {
        if ( !notrunc )
            if ( *astr == '*' && count )
                return false;
        if ( LOWER(*astr) != LOWER(*bstr) )
            return true;
        count++;
    }

    return false;
}

/*
 * Compare strings, case insensitive.
 * Return TRUE if different
 *   (compatibility with historical functions).
 */
bool str_cmp( const char *astr, const char *bstr )
{
    int count = 0;
    bool notrunc = FALSE;
    if ( astr == NULL )
    {
        /*
         * bug( "Str_cmp: null astr.", 0 );
         */
        return TRUE;
    }

    if ( bstr == NULL )
    {
        /*  bug( "Str_cmp: null bstr.", 0 );  */
        return TRUE;
    }
    if ( *astr == '^' )
    {
        notrunc = TRUE;
        astr++;
    }
    if ( *bstr == '^' )
    {
        notrunc = TRUE;
        bstr++;
    }
    if ( astr == NULL )
    {
        /*
         * bug( "Str_cmp: null astr.", 0 );
         */
        return TRUE;
    }

    if ( bstr == NULL )
    {
        /*  bug( "Str_cmp: null bstr.", 0 );  */
        return TRUE;
    }


    for ( ; *astr || *bstr; astr++, bstr++ )
    {
        if ( !notrunc )
            if ( *astr == '*' && count )
                return FALSE;

        if ( LOWER( *astr ) != LOWER( *bstr ) )
            return TRUE;
        count++;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for prefix matching.
 * Return TRUE if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
        bug( "Strn_cmp: null astr.", 0 );
        return TRUE;
    }

    if ( bstr == NULL )
    {
        bug( "Strn_cmp: null bstr.", 0 );
        return TRUE;
    }

    for ( ; *astr; astr++, bstr++ )
    {
        if ( LOWER( *astr ) != LOWER( *bstr ) )
            return TRUE;
    }

    return FALSE;
}



/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns TRUE is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;
    int ichar;
    char c0;

    if ( ( c0 = LOWER( astr[0] ) ) == '\0' )
        return FALSE;

    sstr1 = strlen( astr );
    sstr2 = strlen( bstr );

    for ( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )
    {
        if ( c0 == LOWER( bstr[ichar] ) && !str_prefix( astr, bstr + ichar ) )
            return FALSE;
    }

    return TRUE;
}



/*
 * Compare strings, case insensitive, for suffix matching.
 * Return TRUE if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;

    sstr1 = strlen( astr );
    sstr2 = strlen( bstr );
    if ( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) )
        return FALSE;
    else
        return TRUE;
}

/* Capitalize function by KaVir: 3th December 1997.
 *
 * Pass in a pointer to the string to capitalize.  The function will return
 * a pointer to the corrected string, however for safety purposes the original
 * pointer will also be set to point to the new string.  Thus if you do:
 *    char *normal_string = "a piece of text";
 *    char *cap_string = capitalize( normal_string );
 * Then both cap_string and normal_string will contain the capitalized string.
 *
 * The precise rules of capitalization are as follows:
 *    1) The first non-colorcode alphabetic character will be uppercase.
 *    2) Any alphabetic character following two or more @'s will be unchanged.
 *    3) Any other alphabetic character will be set to lowercase.
 *    4) Any other non-alphabetic characters will be ignored.
 */
char *capitalize( const char *str )
{
    static char strcap[MAX_STRING_LENGTH]; /* Permanent store */
    /*    char *oldstr = str;  */
    int i = 0;  /* Position in strcap */
    int count = 0; /* Number of previous '@'s encountered */
    bool first_letter = TRUE;  /* You will UPPER the first letter you find */
    bool ignore_next = FALSE;  /* Ignore the case of the next letter you find */

    /*
     * If this occurs, then you have a bug elsewhere
     */
    if ( ( strcap[i] = *str ) == '\0' )
    {
        bug( "Capitalize:  Attempted to capitalize a NULL string.", 0 );
        return NULL;
    }

    do /* Begin looping through the string, checking each character */
    {
        /*
         * Should be more efficient than all those 'if' statements ;)
         */
        switch ( strcap[i] )
        {
            default:   /* Non-alphabetic letters and not '@' */
                ignore_next = FALSE; /* Not a color code */
                count = 0;
                break;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':  /* Any lowercase letter */
                if ( ignore_next )
                    ignore_next = FALSE; /* We ignore the case of the this letter */
                else if ( first_letter )
                {
                    first_letter = FALSE;   /* Reset the flag */
                    strcap[i] = UPPER( strcap[i] );  /* We set this letter to uppercase */
                }
                count = 0;
                break;
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':  /* Any uppercase letter */
                /*
                 * Then its the first letter in the string
                 */
                if ( ignore_next )
                    ignore_next = FALSE; /* We ignore the case of the this letter */
                else if ( first_letter )
                    first_letter = FALSE;   /* We ignore the case of the this letter */
                else
                    strcap[i] = LOWER( strcap[i] );  /* Set this letter to lowercase */
                count = 0;
                break;
            case '@':
                if ( ++count >= 2 )  /* If there are two or more '@@'s in a row */
                    ignore_next = TRUE;  /* Set the flag to ignore the next letter */
                break;
        }
    }
    while ( ( strcap[++i] = *++str ) != '\0' );  /* loop until end of string */

    i = 0;   /* str = oldstr;  Reset variables */

    return ( strcap );
}

char *learnt_name( int learnt )
{
    /*
     * For skills/spells.  Return a string indicating how well the
     * skill is learnt.
     */

    if ( learnt < 1 )
        return "Unknown";
    else if ( learnt < 15 )
        return "Awful";
    else if ( learnt < 30 )
        return "Weak";
    else if ( learnt < 40 )
        return "Poor";
    else if ( learnt < 50 )
        return "Average";
    else if ( learnt < 60 )
        return "Fair";
    else if ( learnt < 70 )
        return "Good";
    else if ( learnt < 80 )
        return "Great";
    else if ( learnt < 85 )
        return "Superb";
    else if ( learnt < 90 )
        return "Amazing";
    else if ( learnt < 101 )
        return "Godlike";
    else if ( learnt < 102 )
        return "RACE";
    else
        return "Godlike";
}

int nocol_strlen( const char *text )
{
    char c;
    int i;
    int status;
    int length;
    int strlen_size;

    status = 0;
    length = 0;
    strlen_size = strlen( text );

    for ( i = 0; i < strlen_size; i++ )
    {

        c = text[i];
        length++;

        switch ( status )
        {
            case 0:
            case 1:
                if ( c == '@' )
                    status++;
                else
                    status = 0;
                break;
            case 2:
                length -= 3;   /* Subtract for '@@x' */
                status = 0;
                break;
        }
    }
    return ( length );
}

int ccode_len( const char *text, short desired )
{
    char c;
    int i;
    int status;
    int length;
    int strlen_size;

    status = 0;
    length = 0;
    strlen_size = strlen( text );

    for ( i = 0; i < strlen_size; i++ )
    {

        c = text[i];

        switch ( status )
        {
            case 0:
            case 1:
                if ( c == '@' )
                    status++;
                else
                    status = 0;
                break;
            case 2:
                length += 3;   /* Subtract for '@@x' */
                status = 0;
                break;
        }
    }
    return ( length + desired );
}

/* END NOTE */

/* A Function to "center" text, and return a string with
 * the required amount of white space either side of the
 * original text.  --Stephen
 */

char *center_text( char *text, int width )
{
    /*
     * This could do with a LOT of improvement sometime!
     */
    /*
     * Improvements done.. -- Altrag
     */
    static char foo[MAX_STRING_LENGTH];
    int len, diff;

    /*
     * Don't bother with strings too long..
     */
    if ( ( len = nocol_strlen( text ) ) >= width )
        return text;

    diff = strlen( text ) - len;

    /*
     * Add the spaces and terminate string
     */
    memset( foo, ' ', width + diff );
    foo[width + diff] = '\0';

    /*
     * Copy old string over
     */
    memcpy( &foo[( width - len ) / 2], text, len + diff );

    return foo;
}

string str_mod( string& mod_string, char* argument )
{


    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char *buf2;
    char *buf3 = NULL;
    char *word;
    char temp[MSL];
    int i = 0;
    bool multiple = FALSE;

    if ( !str_cmp( argument, "" ) )
    {
        snprintf( bug_buf, (2 * MIL), "Unknown reason for return, argument is -%s-", argument );
        monitor_chan( bug_buf, MONITOR_DEBUG );
        return mod_string;

    }


    if ( argument[0] == '+' || argument[0] == '-' )
    {
        buf[0] = '\0';
        smash_tilde( argument );
        if ( argument[0] == '+' )
        {
            argument++;

            if ( !mod_string.empty() )
                strncat( buf, CSTR(mod_string), MSL );
            while ( isspace( *argument ) )
                argument++;
            if ( !str_infix( argument, CSTR(mod_string) ) )
            {
                return mod_string;
            }
            strncat( buf, argument, MSL );
            strncat( buf, " ", MSL );
        }
        if ( argument[0] == '-' )
        {
            argument++;
            if ( argument[0] == '\'' )
                multiple = TRUE;
            one_argument( argument, arg1 );

            if ( multiple )
            {
                snprintf( temp, MSL, "\'%s\'", arg1 );
                snprintf( arg1, MIL, "%s", temp );
            }

            if ( arg1[0] != '\0' )
            {
                buf2 = str_dup( CSTR(mod_string) );
                buf3 = buf2;
                if ( ( word = strstr( buf2, arg1 ) ) == NULL )
                {
                    free_string( buf2 );
                    return mod_string;
                }
                else
                {
                    while ( buf2 != word )
                        buf[i++] = *( buf2++ );
                    while ( ( !isspace( *( buf2++ ) ) ) || ( ( multiple ) && ( ( buf2[0] != '\'' ) && ( buf2[0] != '\0' ) ) ) );
                    buf[i] = '\0';
                    strncat( buf, buf2, MSL );
                }
            }

        }

        free_string( buf3 );
        word = buf2 = buf3 = NULL;
        mod_string = buf;
    }
    return mod_string;
}

void rand_arg( char *argument, char *output )
{
    char temp[MSL];
    short counter = 0;
    argument = one_argument( argument, temp );
    while ( temp[0] )
    {
        if ( number_range( 0, counter ) == 0 )
            strcpy( output, temp );
        counter++;
        argument = one_argument( argument, temp );
    }
}


/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;

    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
        if ( *pdot == '.' )
        {
            *pdot = '\0';
            number = atoi( argument );
            *pdot = '.';
            strcpy( arg, pdot + 1 );
            return number;
        }
    }

    strcpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;
    if ( argument == NULL )
        return NULL;
    while ( isspace( *argument ) )
        argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
        cEnd = *argument++;

    while ( *argument != '\0' )
    {
        if ( *argument == cEnd )
        {
            argument++;
            break;
        }
        *arg_first = LOWER( *argument );
        arg_first++;
        argument++;
    }
    *arg_first = '\0';

    while ( isspace( *argument ) )
        argument++;

    return argument;
}

/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_word( char *argument, char *arg_first )
{
    char cEnd;
    if ( argument == NULL )
        return NULL;
    while ( isspace( *argument ) )
        argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
        cEnd = *argument++;

    while ( *argument != '\0' )
    {
        if ( *argument == cEnd )
        {
            argument++;
            break;
        }
        *arg_first = ( *argument );
        arg_first++;
        argument++;
    }
    *arg_first = '\0';

    while ( isspace( *argument ) )
        argument++;

    return argument;
}


char *two_args( char *argument, char *arg_first, char *arg_second )
{
    char *temp_arg = argument;

    temp_arg = one_argument( temp_arg, arg_first );
    temp_arg = one_argument( temp_arg, arg_second );
    return temp_arg;
}


char *strip_out( const char *orig, const char *strip )
{
    static char buf[MAX_STRING_LENGTH];
    const char *i;
    char *b = buf;
    int slen;

    if ( !orig || !strip )
    {
        /*    log_f("strip_out called with NULL param");  */
        return "!!OUT!!";
    }
    slen = strlen( strip );
    for ( i = strstr( orig, strip ); i; i = strstr( orig, strip ) )
    {
        strncpy( b, orig, ( i - orig ) );
        b += i - orig;
        orig = i + slen;
    }
    strcpy( b, orig );
    return buf;
}


/* this code donated by Altrag */
char *strip_color( const char *orig, const char *strip )
{
    static char buf[MAX_STRING_LENGTH];
    const char *i;
    char *b = buf;
    int slen;

    if ( !orig || !strip )
    {
        /*    log_f("strip_out called with NULL param");  */
        return "!!OUT!!";
    }
    slen = strlen( strip ) + 1;
    for ( i = strstr( orig, strip ); i; i = strstr( orig, strip ) )
    {
        strncpy( b, orig, ( i - orig ) );
        b += i - orig;
        orig = i + slen;
    }
    strcpy( b, orig );
    return buf;
}

char *get_tribe_standing_name( int standing )
{


    switch ( standing )
    {
        case 0:
            return "@@mEternal@@N";
            break;
        case 1:
            return "@@rEldest@@N";
            break;
        case 2:
            return "@@rMate@@N";
            break;
        case 3:
            return "@@eWarder@@N";
            break;
        case 4:
            return "@@GGuardian@@N";
            break;
        case 5:
            return "@@aSentry@@N";
            break;
        case 6:
            return "@@yElder@@N";
            break;
        case 7:
            return "@@rAdult@@N";
            break;
        case 8:
            return "@@RYounger@@N";
            break;
        case 9:
            return "@@bCub@@N";
            break;

    }
    return "@@WCUB@@N";
}

char *get_moon_phase_name( void )
{



    switch ( weather_info.moon_phase )
    {
        case MOON_NEW:
            return "@@bDark@@N";
            break;
        case MOON_WAX_CRE:
            return "@@aWaxing @@dCresent@@N";
            break;
        case MOON_WAX_HALF:
            return "@@aWaxing @@gHalf@@N";
            break;
        case MOON_WAX_GIB:
            return "@@aWaxing @@WGibbous@@N";
            break;
        case MOON_FULL:
            return "@@WFULL@@N";
            break;
        case MOON_WAN_GIB:
            return "@@cWaning @@WGibbous@@N";
            break;
        case MOON_WAN_HALF:
            return "@@cWaning @@gHalf@@N";
            break;
        case MOON_WAN_CRE:
            return "@@cWaning @@dCrescent@@N";
            break;

    }
    return "@@eDESTROYED!!!@@N";
}


bool list_in_list( char *first_list, char *second_list )
{
    char check_word[MAX_INPUT_LENGTH];
    char against_word[MSL];
    char *checklist;
    char *againstlist;

    checklist = first_list;
    againstlist = second_list;

    for ( ;; )
    {
        checklist = one_argument( checklist, check_word );
        againstlist = second_list;
        if ( check_word[0] == '\0' )
        {
            return FALSE;
        }
        for ( ;; )
        {
            againstlist = one_argument( againstlist, against_word );
            if ( against_word[0] == '\0' )
                break;
            if ( !str_cmp( check_word, against_word ) )
            {
                return TRUE;
            }

        }
    }
}


/*
 * Return ascii name of an item type.
 */
char *item_type_name( OBJ_DATA * obj )
{
    char log[MAX_STRING_LENGTH];

    switch ( obj->item_type )
    {
        case ITEM_LIGHT:
            return "light";
        case ITEM_SCROLL:
            return "scroll";
        case ITEM_WAND:
            return "wand";
        case ITEM_STAFF:
            return "staff";
        case ITEM_BEACON:
            return "beacon";
        case ITEM_WEAPON:
            return "weapon";
        case ITEM_TREASURE:
            return "treasure";
        case ITEM_ARMOR:
            return "armor";
        case ITEM_POTION:
            return "potion";
        case ITEM_FURNITURE:
            return "furniture";
        case ITEM_TRASH:
            return "trash";
        case ITEM_CONTAINER:
            return "container";
        case ITEM_QUEST:
            return "quest";
        case ITEM_DRINK_CON:
            return "drink container";
        case ITEM_KEY:
            return "key";
        case ITEM_FOOD:
            return "food";
        case ITEM_MONEY:
            return "money";
        case ITEM_BOAT:
            return "boat";
        case ITEM_CORPSE_NPC:
            return "npc corpse";
        case ITEM_CORPSE_PC:
            return "pc corpse";
        case ITEM_FOUNTAIN:
            return "fountain";
        case ITEM_PILL:
            return "pill";
        case ITEM_BOARD:
            return "bulletin board";
        case ITEM_CLUTCH:
            return "clutch";
        case ITEM_TRIGGER:
            return "trigger";
        case ITEM_STAKE:
            return "stake";
        case ITEM_SOUL:
            return "soul";
        case ITEM_PIECE:
            return "piece";
        case ITEM_SPELL_MATRIX:
            return "spell matrix";
        case ITEM_ENCHANTMENT:
            return "enchantment";
        case ITEM_PORTAL:
            return "portal";
        case ITEM_CHARM:
            return "charm";
        case ITEM_ANVIL:
            return "anvil";
        case ITEM_COOKING_FIRE:
            return "cooking fire";
        case ITEM_ALCHEMY_LAB:
            return "alchemy lab";
        case ITEM_FORGE:
            return "forge";
        case ITEM_LOOM:
            return "loom";

    }

    snprintf( log, MSL, "Item_type_name: Object: %d.  Unknown Type: %d", obj->pIndexData->vnum, obj->item_type );
    monitor_chan( log, MONITOR_OBJ );
    bug( log, 0 );
    return "(unknown)";
}




/*
 * Return ascii name of an affect location.
 */
char *affect_loc_name( int location )
{
    char buf[MAX_STRING_LENGTH];

    switch ( location )
    {
        case APPLY_NONE:
            return "none";
        case APPLY_STR:
            return "strength";
        case APPLY_DEX:
            return "dexterity";
        case APPLY_INT:
            return "intelligence";
        case APPLY_WIS:
            return "wisdom";
        case APPLY_CON:
            return "constitution";
        case APPLY_SEX:
            return "sex";
        case APPLY_CLASS:
            return "class";
        case APPLY_LEVEL:
            return "level";
        case APPLY_AGE:
            return "age";
        case APPLY_HEIGHT:
            return "height";
        case APPLY_WEIGHT:
            return "weight";
        case APPLY_MANA:
            return "mana";
        case APPLY_HIT:
            return "hp";
        case APPLY_MOVE:
            return "moves";
        case APPLY_EXP:
            return "experience";
        case APPLY_AC:
            return "armor class";
        case APPLY_HITROLL:
            return "hit roll";
        case APPLY_DAMROLL:
            return "damage roll";
        case APPLY_SAVING_PARA:
            return "save vs paralysis";
        case APPLY_SAVING_ROD:
            return "save vs rod";
        case APPLY_SAVING_PETRI:
            return "save vs petrification";
        case APPLY_SAVING_BREATH:
            return "save vs breath";
        case APPLY_SAVING_SPELL:
            return "save vs spell";
    }

    snprintf( buf, MSL, "affect_location_name: location %d unknown.", location );
    monitor_chan( buf, MONITOR_OBJ );

    bug( "Affect_location_name: unknown location %d.", location );
    return "(unknown)";
}

const char *raffect_bit_name( int vector )
{
    static char rbuf[MSL];

    rbuf[0] = '\0';

    if ( vector & ROOM_BV_NONE )
        strncat( rbuf, " NONE", MSL );
    if ( vector & ROOM_BV_SILENCE )
        strncat( rbuf, " Silence", MSL );
    if ( vector & ROOM_BV_SAFE )
        strncat( rbuf, " Safe", MSL );
    if ( vector & ROOM_BV_ENCAPS )
        strncat( rbuf, " Seal Room", MSL );
    if ( vector & ROOM_BV_SHADE )
        strncat( rbuf, " Shade", MSL );
    if ( vector & ROOM_BV_HEAL_REGEN )
        strncat( rbuf, " @@mHealing Light@@N", MSL );
    if ( vector & ROOM_BV_HEAL_STEAL )
        strncat( rbuf, " @@dWither Shadow@@N", MSL );
    if ( vector & ROOM_BV_MANA_REGEN )
        strncat( rbuf, " @@eMana Flare@@N", MSL );
    if ( vector & ROOM_BV_MANA_STEAL )
        strncat( rbuf, " @@dMana Drain@@N", MSL );
    if ( vector & ROOM_BV_FIRE_RUNE )
        strncat( rbuf, " @@eFire @@NRune", MSL );
    if ( vector & ROOM_BV_FIRE_TRAP )
        strncat( rbuf, " @@eFire @@NTrap", MSL );
    if ( vector & ROOM_BV_DAMAGE_TRAP )
        strncat( rbuf, " @@dDamage @@NTrap", MSL );
    if ( vector & ROOM_BV_SHOCK_RUNE )
        strncat( rbuf, " @@lShock @@NRune", MSL );
    if ( vector & ROOM_BV_SHOCK_TRAP )
        strncat( rbuf, " @@lShock @@NTrap", MSL );
    if ( vector & ROOM_BV_HOLD )
        strncat( rbuf, " @@rCage@@N", MSL );
    if ( vector & ROOM_BV_POISON_RUNE )
        strncat( rbuf, " @@Poison@@N Rune", MSL );

    return ( rbuf[0] != '\0' ) ? rbuf + 1 : "none";
}

/*
 * Return ascii name of an affect bit vector.
 */
const char *affect_bit_name( int vector )
{
    static char buf[MSL];

    buf[0] = '\0';
    if ( vector & AFF_BLIND )
        strncat( buf, " blind", MSL );
    if ( vector & AFF_INVISIBLE )
        strncat( buf, " invisible", MSL );
    if ( vector & AFF_DETECT_EVIL )
        strncat( buf, " detect_evil", MSL );
    if ( vector & AFF_DETECT_INVIS )
        strncat( buf, " detect_invis", MSL );
    if ( vector & AFF_DETECT_MAGIC )
        strncat( buf, " detect_magic", MSL );
    if ( vector & AFF_DETECT_HIDDEN )
        strncat( buf, " detect_hidden", MSL );
    if ( vector & AFF_HOLD )
        strncat( buf, " hold", MSL );
    if ( vector & AFF_SANCTUARY )
        strncat( buf, " sanctuary", MSL );
    if ( vector & AFF_FAERIE_FIRE )
        strncat( buf, " faerie_fire", MSL );
    if ( vector & AFF_INFRARED )
        strncat( buf, " infrared", MSL );
    if ( vector & AFF_CURSE )
        strncat( buf, " curse", MSL );
    if ( vector & AFF_CLOAK_FLAMING )
        strncat( buf, " flaming", MSL );
    if ( vector & AFF_POISON )
        strncat( buf, " poison", MSL );
    if ( vector & AFF_PROTECT )
        strncat( buf, " protect", MSL );
    if ( vector & AFF_PARALYSIS )
        strncat( buf, " paralysis", MSL );
    if ( vector & AFF_SLEEP )
        strncat( buf, " sleep", MSL );
    if ( vector & AFF_SNEAK )
        strncat( buf, " sneak", MSL );
    if ( vector & AFF_HIDE )
        strncat( buf, " hide", MSL );
    if ( vector & AFF_CHARM )
        strncat( buf, " charm", MSL );
    if ( vector & AFF_FLYING )
        strncat( buf, " flying", MSL );
    if ( vector & AFF_PASS_DOOR )
        strncat( buf, " pass_door", MSL );
    if ( vector & AFF_CLOAK_REFLECTION )
        strncat( buf, " cloak:reflection", MSL );
    if ( vector & AFF_CLOAK_ABSORPTION )
        strncat( buf, " cloak:absorption", MSL );
    if ( vector & AFF_CLOAK_ADEPT )
        strncat( buf, " cloak:adept", MSL );
    if ( vector & AFF_CLOAK_REGEN )
        strncat( buf, " cloak:regeneration", MSL );

    return ( buf[0] != '\0' ) ? buf + 1 : "none";
}

/*
 * Return ascii name of exit flags bitset.
 */
const char *exit_bit_name( bitset<MAX_BITSET> &exit_flags )
{
    static char buf[MSL];

    buf[0] = '\0';
    if ( exit_flags.test(EX_CLOSED) )
        strncat( buf, " closed", MSL );
    if ( exit_flags.test(EX_LOCKED) )
        strncat( buf, " locked", MSL );
    if ( exit_flags.test(EX_CLIMB) )
        strncat( buf, " climb", MSL );
    if ( exit_flags.test(EX_IMMORTAL) )
        strncat( buf, " immortal", MSL );
    if ( exit_flags.test(EX_PICKPROOF) )
        strncat( buf, " pickproof", MSL );
    if ( exit_flags.test(EX_SMASHPROOF) )
        strncat( buf, " smashproof", MSL );
    if ( exit_flags.test(EX_PASSPROOF) )
        strncat( buf, " passproof", MSL );
    if ( exit_flags.test(EX_NODETECT) )
        strncat( buf, " nodetect", MSL );
    if ( exit_flags.test(EX_NOMOB) )
        strncat( buf, " nomob", MSL );

    return ( buf[0] != '\0' ) ? buf + 1 : "none";
}

/*
 * Return ascii name of extra flags bitset.
 */
const char *extra_bit_name( bitset<MAX_BITSET> &extra_flags )
{
    static char buf[MSL];

    buf[0] = '\0';
    if ( extra_flags.test(ITEM_EXTRA_GLOW) )
        strncat( buf, " glow", MSL );
    if ( extra_flags.test(ITEM_EXTRA_HUM) )
        strncat( buf, " hum", MSL );
    if ( extra_flags.test(ITEM_EXTRA_NO_DISARM) )
        strncat( buf, " nodisarm", MSL );
    if ( extra_flags.test(ITEM_EXTRA_LOCK) )
        strncat( buf, " lock", MSL );
    if ( extra_flags.test(ITEM_EXTRA_EVIL) )
        strncat( buf, " evil", MSL );
    if ( extra_flags.test(ITEM_EXTRA_INVIS) )
        strncat( buf, " invis", MSL );
    if ( extra_flags.test(ITEM_EXTRA_MAGIC) )
        strncat( buf, " magic", MSL );
    if ( extra_flags.test(ITEM_EXTRA_NO_DROP) )
        strncat( buf, " nodrop", MSL );
    if ( extra_flags.test(ITEM_EXTRA_BLESS) )
        strncat( buf, " bless", MSL );
    if ( extra_flags.test(ITEM_EXTRA_ANTI_GOOD) )
        strncat( buf, " anti-good", MSL );
    if ( extra_flags.test(ITEM_EXTRA_ANTI_EVIL) )
        strncat( buf, " anti-evil", MSL );
    if ( extra_flags.test(ITEM_EXTRA_ANTI_NEUTRAL) )
        strncat( buf, " anti-neutral", MSL );
    if ( extra_flags.test(ITEM_EXTRA_NO_REMOVE) )
        strncat( buf, " noremove", MSL );
    if ( extra_flags.test(ITEM_EXTRA_INVENTORY) )
        strncat( buf, " inventory", MSL );
    if ( extra_flags.test(ITEM_EXTRA_NO_LOOT) )
        strncat( buf, " noloot", MSL );
    if ( extra_flags.test(ITEM_EXTRA_NO_SAC) )
        strncat( buf, " nosac", MSL );
    if ( extra_flags.test(ITEM_EXTRA_REMORT) )
        strncat( buf, " remort", MSL );
    if ( extra_flags.test(ITEM_EXTRA_CLAN_EQ) )
        strncat( buf, " claneq", MSL );
    if ( extra_flags.test(ITEM_EXTRA_NO_SAVE) )
        strncat( buf, " nosave", MSL );
    if ( extra_flags.test(ITEM_EXTRA_NO_AUCTION) )
        strncat( buf, " no_auction", MSL );
    if ( extra_flags.test(ITEM_EXTRA_RARE) )
        strncat( buf, " rare", MSL );
    if ( extra_flags.test(ITEM_EXTRA_VAMP) )
        strncat( buf, " vamp", MSL );
    if ( extra_flags.test(ITEM_EXTRA_UNIQUE) )
        strncat( buf, " unique", MSL );
    if ( extra_flags.test(ITEM_EXTRA_TRIG_DESTROY) )
        strncat( buf, " trigger:destroy", MSL );
    if ( extra_flags.test(ITEM_EXTRA_LIFESTEALER) )
        strncat( buf, " lifestealer", MSL );
    if ( extra_flags.test(ITEM_EXTRA_SILVER) )
        strncat( buf, " silver", MSL );
    if ( extra_flags.test(ITEM_EXTRA_NO_QUEST) )
        strncat( buf, " noquest", MSL );
    if ( extra_flags.test(ITEM_EXTRA_NO_TAKE) )
        strncat( buf, " notake", MSL );

    return ( buf[0] != '\0' ) ? buf + 1 : "none";
}

/*
 * Return ascii name of wear flags bitset.
 */
const char *wear_bit_name( bitset<MAX_BITSET> &wear_flags )
{
    static char buf[MSL];

    buf[0] = '\0';
    if ( wear_flags.test(ITEM_WEAR_HALO) )
        strncat( buf, " halo", MSL );
    if ( wear_flags.test(ITEM_WEAR_AURA) )
        strncat( buf, " aura", MSL );
    if ( wear_flags.test(ITEM_WEAR_HORNS) )
        strncat( buf, " horns", MSL );
    if ( wear_flags.test(ITEM_WEAR_HEAD) )
        strncat( buf, " head", MSL );
    if ( wear_flags.test(ITEM_WEAR_FACE) )
        strncat( buf, " face", MSL );
    if ( wear_flags.test(ITEM_WEAR_BEAK) )
        strncat( buf, " beak", MSL );
    if ( wear_flags.test(ITEM_WEAR_EAR) )
        strncat( buf, " ear", MSL );
    if ( wear_flags.test(ITEM_WEAR_NECK) )
        strncat( buf, " neck", MSL );
    if ( wear_flags.test(ITEM_WEAR_WINGS) )
        strncat( buf, " wings", MSL );
    if ( wear_flags.test(ITEM_WEAR_SHOULDERS) )
        strncat( buf, " shoulders", MSL );
    if ( wear_flags.test(ITEM_WEAR_ARMS) )
        strncat( buf, " arms", MSL );
    if ( wear_flags.test(ITEM_WEAR_WRIST) )
        strncat( buf, " wrist", MSL );
    if ( wear_flags.test(ITEM_WEAR_HANDS) )
        strncat( buf, " hands", MSL );
    if ( wear_flags.test(ITEM_WEAR_FINGER) )
        strncat( buf, " finger", MSL );
    if ( wear_flags.test(ITEM_WEAR_CLAWS) )
        strncat( buf, " claws", MSL );
    if ( wear_flags.test(ITEM_WEAR_HOLD_HAND) )
        strncat( buf, " hold", MSL );
    if ( wear_flags.test(ITEM_WEAR_ABOUT) )
        strncat( buf, " about", MSL );
    if ( wear_flags.test(ITEM_WEAR_WAIST) )
        strncat( buf, " waist", MSL );
    if ( wear_flags.test(ITEM_WEAR_BODY) )
        strncat( buf, " body", MSL );
    if ( wear_flags.test(ITEM_WEAR_TAIL) )
        strncat( buf, " tail", MSL );
    if ( wear_flags.test(ITEM_WEAR_LEGS) )
        strncat( buf, " legs", MSL );
    if ( wear_flags.test(ITEM_WEAR_FEET) )
        strncat( buf, " feet", MSL );
    if ( wear_flags.test(ITEM_WEAR_HOOVES) )
        strncat( buf, " hooves", MSL );

    return ( buf[0] != '\0' ) ? buf + 1 : "none";
}


/* Return the ASCII name of a character's race
 * -- Stephen
 */

char *race_name( CHAR_DATA * ch )
{

    if ( IS_NPC( ch ) )
        return ( "NPC" );

    if ( ch->race >= MAX_RACE || ch->race < 0 )
    {
        /*
         * error reporting here one day... maybe
         */
        return ( "???" );
    }

    return ( race_table[ch->race].race_title );
}

char *short_race_name( CHAR_DATA * ch )
{
    static char buf[MAX_STRING_LENGTH];

    if ( IS_NPC( ch ) )
        snprintf( buf, MSL, "NPC" );
    else if ( ch->race >= MAX_RACE || ch->race < 0 )
    {
        /*
         * error reporting here one day... maybe
         */
        snprintf( buf, MSL, "unknown!" );
    }
    else
        snprintf( buf, MSL, "%s", race_table[ch->race].race_name );

    return ( buf );
}

char *get_family_name( CHAR_DATA * ch )
{

    switch ( ch->super->bloodline )
    {
        default:
            return "Renegade!";
        case 1:
            return "@@WNOT SET@@N";
        case 2:
            return "@@WNOT SET@@N";
        case 3:
            return "@@WNOT SET@@N";
        case 4:
            return "@@WNOT SET@@N";
    }

}

char *get_tribe_name( CHAR_DATA * ch )
{
    switch ( ch->super->bloodline )
    {
        default:
            return "Renegade!";
        case 1:
            return "@@WNOT SET@@N";
        case 2:
            return "@@WNOT SET@@N";
        case 3:
            return "@@WNOT SET@@N";
        case 4:
            return "@@WNOT SET@@N";
    }

}

char *strlower( char *ip )
{
    static char buf[MAX_STRING_LENGTH];
    int i;

    for ( i = 0; i < (MAX_STRING_LENGTH - 1) && ip[i] != '\0'; i++ )
        buf[i] = tolower(ip[i]);

    buf[i] = '\0';
    return buf;
}

char *strupper( char *ip )
{
    static char buf[MAX_STRING_LENGTH];
    int i;

    for ( i = 0; i < (MAX_STRING_LENGTH - 1) && ip[i] != '\0'; i++ )
        buf[i] = toupper(ip[i]);

    buf[i] = '\0';
    return buf;
}

void ch_printf( CHAR_DATA * ch, const char *fmt, ... )
{
    char buf[MAX_STRING_LENGTH];

    va_list args;

    va_start( args, fmt );
    vsprintf( buf, fmt, args );
    va_end( args );

    send_to_char( buf, ch );
}

/*
 * This code is originally by imapopsyckle@hotmail.com
 * Modified by Chris for Merc/Rom
 */
char *tagline_format( const char *txt, CHAR_DATA *ch )
{
    char output[MSL];
    char tagline[MSL];
    char buf[MSL];
    char arg1[256];
    char arg2[256];
    int value;
    bool tag = false;
    const char *desc;
    char *p_output;
    char *common;
    static char *retc;

    output[0] = '\0';
    tagline[0] = '\0';
    buf[0] = '\0';
    arg1[0] = '\0';
    arg2[0] = '\0';
    p_output = output;

    for ( desc = txt; *desc; desc++ )
    {
        /* reset */
        tag = false;
        value = 0;

        if ( *desc != '[' )
        {
            *p_output = *desc;
            *++p_output = '\0';
            continue;
        }

        if ( *++desc != '#' )
        {
            *p_output = *--desc;
            *++p_output = '\0';
            continue;
        }

        /* Skip beginning '[#'. */
        ++desc;

        /* Read in the tag. */
        common = buf;
        while ( *desc != ':' )
        {
            if ( *desc == '\0' )
            {
                snprintf(log_buf, (2 * MIL), "Error: tag has no parser ':'. Room: %d.", ch->in_room->vnum);
                log_f(log_buf);
                *common = *desc;
                break;
            }
            *common = *desc;
            *++common = '\0';
            ++desc;
        }

        /* Skip the colon and space ': '. */
        desc += 2;

        /* Read in the description. */
        common = tagline;
        while ( *desc != ']' )
        {
            if ( *desc == '\0' )
            {
                snprintf(log_buf, (2 * MIL), "Error: tag has no ending ']'. Room: %d.", ch->in_room->vnum);
                log_f(log_buf);
                *common = *desc;
                break;
            }
            *common = *desc;
            *++common = '\0';
            ++desc;
        }

        /* Separate the arguments. */
        common = buf;
        common = one_argument(common, arg1);
        common = one_argument(common, arg2);

        if ( is_number(common) )
            value = atoi(common);
        else if ( strlen(common) > 0 )
        {
            snprintf(log_buf, (2 * MIL), "Tag in room %d is odd: %s %s %s.", ch->in_room->vnum, arg1, arg2, common);
            log_f(log_buf);
        }

        if ( !str_cmp(arg1, "name") ) /* Special exception so we can search and replace */
        {
            char p_name[128];
            char *t = tagline;
            char *n;
            common = buf;
            tag = true;

            while ( *t != '\0' )
            {
                if ( *t != '$' )
                {
                    *common = *t;
                    *++common = '\0';
                    ++t;
                    continue;
                }

                snprintf(p_name, 128, "%s", ch->name.c_str());
                n = p_name;

                while ( *n != '\0' )
                {
                    *common = *n;
                    *++common = '\0';
                    ++n;
                }
                ++t;
            }

            snprintf(tagline, MSL, "%s", buf);
        }
        else if ( check_tag(arg1, arg2, value, ch) )
            tag = true;

        /* Copy in the tagline. */
        if ( tag )
        {
            common = tagline;

            while ( *common != '\0' )
            {
                if ( strlen(output) >= (MAX_STRING_LENGTH - 1) )
                    break;

                *p_output = *common;
                ++common;
                *++p_output = '\0';
            }
        }

    }

    retc = str_dup( output );
    return retc;
}

bool check_tag( char *arg1, char *arg2, int value, CHAR_DATA *ch )
{
    bool retval = false;

    switch ( UPPER(arg1[0]) )
    {
        case 'A':
            if ( !str_cmp(arg1, "ac") && evaluate_tag(arg2, GET_AC(ch), value) )   { retval = true; break; }
            if ( !str_cmp(arg1, "age") && evaluate_tag(arg2, get_age(ch), value) ) { retval = true; break; }
            if ( !str_cmp(arg1, "alignment") )
            {
                if ( !str_cmp(arg2, "good") && IS_GOOD(ch) )       { retval = true; break; }
                if ( !str_cmp(arg2, "neutral") && IS_NEUTRAL(ch) ) { retval = true; break; }
                if ( !str_cmp(arg2, "evil") && IS_EVIL(ch) )       { retval = true; break; }
                if ( evaluate_tag(arg2, ch->alignment, value) )     { retval = true; break; }
                break;
            }
            break;

        case 'C':
            if ( !str_cmp(arg1, "clan") && !str_cmp(arg2, strip_color(clan_table[ch->clan].clan_name, "@@")) )           { retval = true; break; }
            if ( !str_cmp(arg1, "class") && !str_cmp(arg2, class_table[ch->order[0]].who_name) ) { retval = true; break; }
            if ( !str_cmp(arg1, "con") && evaluate_tag(arg2, get_curr_con(ch), value) ) { retval = true; break; }
            break;

        case 'D':
            if ( !str_cmp(arg1, "day") && evaluate_tag(arg2, time_info.day, value) )    { retval = true; break; }
            if ( !str_cmp(arg1, "dex") && evaluate_tag(arg2, get_curr_dex(ch), value) ) { retval = true; break; }
            if ( !str_cmp(arg1, "dr") && evaluate_tag(arg2, GET_DAMROLL(ch), value) )   { retval = true; break; }
            if ( !str_cmp(arg1, "drunk") && IS_DRUNK(ch) )                            { retval = true; break; }
            break;

        case 'H':
            if ( !str_cmp(arg1, "hp") && evaluate_tag(arg2, ch->hit, value) )         { retval = true; break; }
            if ( !str_cmp(arg1, "hr") && evaluate_tag(arg2, GET_HITROLL(ch), value) ) { retval = true; break; }
            if ( !str_cmp(arg1, "hungry") && IS_HUNGRY(ch) )                        { retval = true; break; }
            break;

        case 'I':
            if ( !str_cmp(arg1, "immortal") && IS_IMMORTAL(ch) )                                 { retval = true; break; }
            if ( !str_cmp(arg1, "int") && evaluate_tag(arg2, get_curr_int(ch), value) )            { retval = true; break; }
            if ( !str_cmp(arg1, "isname") && !str_cmp(arg2, ch->name) )                           { retval = true; break; }
            if ( !str_cmp(arg1, "istitle") && !str_cmp(arg2, strip_color(ch->get_title(), "@@")) ) { retval = true; break; }
            break;

        case 'L':
            if ( !str_cmp(arg1, "level") && evaluate_tag(arg2, ch->get_level("psuedo"), value) ) { retval = true; break; }
            break;

        case 'M':
            if ( !str_cmp(arg1, "maxhp") && evaluate_tag(arg2, ch->max_hit, value) )     { retval = true; break; }
            if ( !str_cmp(arg1, "maxmp") && evaluate_tag(arg2, ch->max_mana, value) )    { retval = true; break; }
            if ( !str_cmp(arg1, "maxmv") && evaluate_tag(arg2, ch->max_move, value) )    { retval = true; break; }
            if ( !str_cmp(arg1, "month") && evaluate_tag(arg2, time_info.month, value) ) { retval = true; break; }
            if ( !str_cmp(arg1, "moonloc") )
            {
                if ( !str_cmp(arg2, "down") && weather_info.moon_loc == MOON_DOWN ) { retval = true; break; }
                if ( !str_cmp(arg2, "rise") && weather_info.moon_loc == MOON_RISE ) { retval = true; break; }
                if ( !str_cmp(arg2, "low") && weather_info.moon_loc == MOON_LOW )   { retval = true; break; }
                if ( !str_cmp(arg2, "peak") && weather_info.moon_loc == MOON_PEAK ) { retval = true; break; }
                if ( !str_cmp(arg2, "fall") && weather_info.moon_loc == MOON_FALL ) { retval = true; break; }
                if ( !str_cmp(arg2, "set") && weather_info.moon_loc == MOON_SET )   { retval = true; break; }
                break;
            }
            if ( !str_cmp(arg1, "moonphase") )
            {
                if ( !str_cmp(arg2, "new") && weather_info.moon_phase == MOON_NEW )          { retval = true; break; }
                if ( !str_cmp(arg2, "waxcre") && weather_info.moon_phase == MOON_WAX_CRE )   { retval = true; break; }
                if ( !str_cmp(arg2, "waxhalf") && weather_info.moon_phase == MOON_WAX_HALF ) { retval = true; break; }
                if ( !str_cmp(arg2, "waxgib") && weather_info.moon_phase == MOON_WAX_GIB )   { retval = true; break; }
                if ( !str_cmp(arg2, "full") && weather_info.moon_phase == MOON_FULL )        { retval = true; break; }
                if ( !str_cmp(arg2, "wangib") && weather_info.moon_phase == MOON_WAN_GIB )   { retval = true; break; }
                if ( !str_cmp(arg2, "wanhalf") && weather_info.moon_phase == MOON_WAN_HALF ) { retval = true; break; }
                if ( !str_cmp(arg2, "wancre") && weather_info.moon_phase == MOON_WAN_CRE )   { retval = true; break; }
                break;
            }
            if ( !str_cmp(arg1, "mp" ) && evaluate_tag(arg2, ch->mana, value) ) { retval = true; break; }
            if ( !str_cmp(arg1, "mv" ) && evaluate_tag(arg2, ch->move, value) ) { retval = true; break; }
            break;

        case 'P':
            if ( !str_cmp(arg1, "position") )
            {
                if ( !str_cmp(arg2, "resting") && ch->position == POS_RESTING )   { retval = true; break; }
                if ( !str_cmp(arg2, "standing") && ch->position == POS_STANDING ) { retval = true; break; }
                if ( !str_cmp(arg2, "fighting") && ch->position == POS_FIGHTING ) { retval = true; break; }
                break;
            }
            break;

        case 'R':
            if ( !str_cmp(arg1, "race") && !str_cmp(arg2, race_table[ch->race].race_title) ) { retval = true; break; }
            if ( !str_cmp(arg1, "remort") )
            {
                for ( short i = 0; i < MAX_CLASS; i++ )
                    if ( !str_cmp(arg2, remort_table[i].who_name) && ch->get_level("remortal") > 0 ) { retval = true; break; }
                break;
            }
            break;

        case 'S':
            if ( !str_cmp(arg1, "sex") )
            {
                if ( !str_cmp(arg2, "neutral") && ch->sex == SEX_NEUTRAL ) { retval = true; break; }
                if ( !str_cmp(arg2, "male") && ch->sex == SEX_MALE )       { retval = true; break; }
                if ( !str_cmp(arg2, "female") && ch->sex == SEX_FEMALE )   { retval = true; break; }
                break;
            }
            if ( !str_cmp(arg1, "str") && evaluate_tag(arg2, get_curr_str(ch), value) ) { retval = true; break; }
            if ( !str_cmp(arg1, "sun") )
            {
                if ( !str_cmp(arg2, "sunrise") && weather_info.sunlight == SUN_LIGHT )  { retval = true; break; }
                if ( !str_cmp(arg2, "night") && weather_info.sunlight == SUN_DARK )     { retval = true; break; }
                if ( !str_cmp(arg2, "day") && weather_info.sunlight == SUN_RISE )       { retval = true; break; }
                if ( !str_cmp(arg2, "sunset") && weather_info.sunlight == SUN_SET )     { retval = true; break; }
                break;
            }
            if ( !str_cmp(arg1, "super") )
            {
                if ( !str_cmp(arg2, "vamp") && IS_VAMP(ch) )     { retval = true; break; }
                if ( !str_cmp(arg2, "wolf") && IS_WOLF(ch) )     { retval = true; break; }
                if ( !str_cmp(arg2, "hunter") && IS_HUNTER(ch) ) { retval = true; break; }
            }
            break;

        case 'T':
            if ( !str_cmp(arg1, "thirsty") && IS_THIRSTY(ch) )                       { retval = true; break; }
            if ( !str_cmp(arg1, "time") && evaluate_tag(arg2, time_info.hour, value) ) { retval = true; break; }
            break;

        case 'W':
            if ( !str_cmp(arg1, "weather") )
            {
                if ( !str_cmp(arg2, "clear") && weather_info.sky == SKY_CLOUDLESS )     { retval = true; break; }
                if ( !str_cmp(arg2, "cloudy") && weather_info.sky == SKY_CLOUDY )       { retval = true; break; }
                if ( !str_cmp(arg2, "rain") && weather_info.sky == SKY_RAINING )        { retval = true; break; }
                if ( !str_cmp(arg2, "lightning") && weather_info.sky == SKY_LIGHTNING ) { retval = true; break; }
                break;
            }
            if ( !str_cmp(arg1, "wis") && evaluate_tag(arg2, get_curr_wis(ch), value) ) { retval = true; break; }
            break;
    }

    return retval;
}

bool evaluate_tag( const char *opr, int v1, int v2 )
{
    bool retval = false;

    if ( !str_cmp(opr, ">") && v1 > v2 )
        retval = true;
    if ( !str_cmp(opr, ">") && v1 > v2 )
        retval = true;
    if ( !str_cmp(opr, "<") && v1 < v2 )
        retval = true;
    if ( !str_cmp(opr, "==") && v1 == v2 )
        retval = true;
    if ( !str_cmp(opr, ">=") && v1 >= v2 )
        retval = true;
    if ( !str_cmp(opr, "<=") && v1 <= v2 )
        retval = true;
    if ( !str_cmp(opr, "!=") && v1 != v2 )
        retval = true;

    return retval;
}

const char *gen_rand_string( int length )
{
    int i, r;
    char tmp[2];
    static char output[MSL];
    const char valid[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    srand ( current_time );

    for ( i = 0; i < length; i++ )
    {
        r = rand() % strlen(valid);
        snprintf( tmp, 2, "%c", valid[r] );
        if ( i == 0 )
            snprintf ( output, length+1, "%s", tmp );
        else
            strncat ( output, tmp, length+1 );
    }

    return output;
}

const char *current_time_str( void )
{
    char *strtime;
    static char output[MSL];

    strtime = ctime( &current_time );
    strtime[strlen( strtime ) - 1] = '\0';
    snprintf( output, MSL, "%s", strtime );

    return output;
}

const char *who( const char *what, CHAR_DATA *looker )
{
    static string output;
    char *_what = const_cast<char *>(what);
    char arg1[MSL] = {'\0'}, arg2[MSL] = {'\0'}, buf1[MSL] = {'\0'}, buf2[MSL] = {'\0'};
    bool imm = false, apt = false, rmt = false, mrt = false;
    short found = 0;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *pers;

    output.clear();
    _what = one_argument( _what, arg1 );
    _what = one_argument( _what, arg2 );

    if ( arg1[0] != '\0' )
    {
        // Check for level/group/class/etc restrictions
        return output.c_str();
    }
    else
    {
        // Default output
        output += "\r\nWHO Listing: " mudnamecolor "\r\n";
        output += "@@R+-----------------------------------------------------------------------------+\r\n";
        output += "| @@mSo Mo An Ki Ne@@R                                                              |\r\n";
        output += "| @@bMa Cl Th Wa Ps @@eRace Clan  ABJPW   Player     Title                  Flags   @@R|\r\n";
        output += "|---------------------------------+-------------------------------------------|\r\n";

        // Find what tiers to show
        for ( d = first_desc; d != NULL; d = d->next )
        {
            if ( d->connected != CON_PLAYING || d->character->act.test(ACT_WIZINVIS) || d->character->stance == STANCE_AMBUSH )
                continue;
            if ( d->character->get_level() >= LEVEL_HERO )
                imm = true;
            if ( d->character->get_level("adept") > 0 && d->character->get_level() < LEVEL_HERO )
                apt = true;
            if ( d->character->get_level("maxremortal") > 0 && d->character->get_level("adept") < 1 && d->character->get_level() < LEVEL_HERO )
                rmt = true;
            if ( d->character->get_level("maxremortal") < 1 && d->character->get_level("adept") < 1 && d->character->get_level() < LEVEL_HERO )
                mrt = true;
            found++;
        }

        if ( imm ) //Display imms
        {
            output += "@@R|---------------------------------|----------@@lImmortals@@R------------------------|@@N\r\n";
            for ( d = first_desc; d != NULL; d = d->next )
            {
                if ( d->connected != CON_PLAYING || d->character->act.test(ACT_WIZINVIS) || d->character->stance == STANCE_AMBUSH )
                    continue;
                if ( d->character->get_level() < LEVEL_HERO )
                    continue;
                pers = ( d->original != NULL ) ? d->original : d->character;
                output += "@@R| "; output += who_pers( pers ); output += " @@R|@@N\r\n";
            }
        }

        if ( apt ) //Display adepts
        {
            output += "@@R|---------------------------------|------------@@WAdepts@@R-------------------------|@@N\r\n";
            for ( d = first_desc; d != NULL; d = d->next )
            {
                if ( d->connected != CON_PLAYING || d->character->act.test(ACT_WIZINVIS) || d->character->stance == STANCE_AMBUSH )
                    continue;
                if ( d->character->get_level("adept") < 1 || d->character->get_level() >= LEVEL_HERO )
                    continue;
                pers = ( d->original != NULL ) ? d->original : d->character;
                output += "@@R| "; output += who_pers( pers ); output += " @@R|@@N\r\n";
            }
        }

        if ( rmt ) //Display remorts
        {
            output += "@@R|---------------------------------|----------@@mRemortals@@R------------------------|@@N\r\n";
            for ( d = first_desc; d != NULL; d = d->next )
            {
                if ( d->connected != CON_PLAYING || d->character->act.test(ACT_WIZINVIS) || d->character->stance == STANCE_AMBUSH )
                    continue;
                if ( d->character->get_level("maxremortal") < 1 || d->character->get_level("adept") > 0 || d->character->get_level() >= LEVEL_HERO )
                    continue;
                pers = ( d->original != NULL ) ? d->original : d->character;
                output += "@@R| "; output += who_pers( pers ); output += " @@R|@@N\r\n";
            }
        }

        if ( mrt ) //Display morts
        {
            output += "@@R|---------------------------------|-----------@@cMortals@@R-------------------------|@@N\r\n";
            for ( d = first_desc; d != NULL; d = d->next )
            {
                if ( d->connected != CON_PLAYING || d->character->act.test(ACT_WIZINVIS) || d->character->stance == STANCE_AMBUSH )
                    continue;
                if ( d->character->get_level("maxremortal") > 0 || d->character->get_level("adept") > 0 || d->character->get_level() >= LEVEL_HERO )
                    continue;
                pers = ( d->original != NULL ) ? d->original : d->character;
                output += "@@R| "; output += who_pers( pers ); output += " @@R|@@N\r\n";
            }

        }

        output += "@@R|---------------------------------+-------------------------------------------|\r\n";
        snprintf( buf1, MSL, "(%d Player%s)  KEY: (A)fk  (B)uilding  (K)iller  (P)kok  (T)hief  (W)riting", found, found == 1 ? "" : "s" );
        snprintf( buf2, MSL, "@@R|@@N %s @@R|\r\n", center_text( buf1, 75 ) );
        output += buf2;
        snprintf( buf1, MSL, " (*) Clan Boss  (L) Clan Leader  (!) Clan Armorer " );
        snprintf( buf2, MSL, "@@R|@@N %s @@R|\r\n", center_text( buf1, 75 ) );
        output += buf2;
        snprintf( buf1, MSL, "There has been a maximum of %d player%s logged on this session.", mudinfo.max_players_reboot, mudinfo.max_players_reboot == 1 ? "" : "s" );
        snprintf( buf2, MSL, "@@R|@@N %s @@R|\r\n", center_text( buf1, 75 ) );
        output += buf2;
        output += "@@R+-----------------------------------------------------------------------------+\r\n@@N";

        return output.c_str();
    }

    return output.c_str();
}

const char *who_pers( CHAR_DATA *pers )
{
 char buf1[MSL] = {'\0'}, buf2[MSL] = {'\0'}, ntbuf[MSL] = {'\0'};
 char cjob[2] = {'\0'};
 char flags1[10] = {'\0'}, flags2[10] = {'\0'};
 static const char *output;
 string format;

 if( !pers )
  return "";

 /* Clan job, if any */
 if ( pers->act.test(ACT_CBOSS) )
     snprintf( cjob, 2, "*" );
 else if ( pers->act.test(ACT_CLEADER) )
     snprintf( cjob, 2, "L" );
 else if ( pers->act.test(ACT_CARMORER) )
     snprintf( cjob, 2, "!" );
 else
     snprintf( cjob, 2, " " );

 /* Flags on left side of output */
 if ( pers->act.test(ACT_AFK) )
     snprintf( flags1, 10, "A" );
 else
     snprintf( flags1, 10, " " );
 if ( pers->position == POS_BUILDING )
     strncat( flags1, "B", 10 );
 else
     strncat( flags1, " ", 10 );
 strncat( flags1, cjob, 10 );
 if ( pers->act.test(ACT_PKOK) )
     strncat( flags1, "P", 10 );
 else
     strncat( flags1, " ", 10 );
 if ( pers->position == POS_WRITING )
     strncat( flags1, "W", 10 );
 else
     strncat( flags1, " ", 10 );

 /* Flags on right side of output */
 if ( pers->act.test(ACT_WIZINVIS) )
     snprintf( flags2, 10, "I" );
 if ( pers->act.test(ACT_KILLER) )
 {
     if ( flags2[0] == '\0' )
         snprintf( flags2, 10, "K" );
     else
         strncat( flags2, "K", 10 );
 }
 if ( pers->act.test(ACT_THIEF) )
 {
     if ( flags2[0] == '\0' )
         snprintf( flags2, 10, "T" );
     else
         strncat( flags2, "T", 10 );
 }
 if ( pers->timer > 5 )
     snprintf( flags2, 10, "IDLE:%2d", pers->timer );

 /* Sort out name + title length */
 snprintf( buf2, MSL, "%s%s", pers->get_name(), pers->get_title() );
 snprintf( ntbuf, MSL, "%s", color_format(buf2,33,true) );

 /* Make the magic happen */
 snprintf( buf1, MSL, "%s  %3s%5s  %s @@R|@@N %s @@e%-7s@@N", pers->get_whoname(), race_table[pers->race].race_name, clan_table[pers->clan].clan_name,
                                                              flags1, ntbuf, flags2 );
 output = str_dup( buf1 );

 return output;
}

/* Return a properly justified string of <length> while accounting for properly spacing due to color codes */
const char *color_format( const char *input, int length, bool left_just )
{
 int len = substr_cnt(input,"@@");
 char buf[MSL] = {'\0'};
 static const char *output;
 string format;

 snprintf( buf, MSL, "%d", (length+(len*3)) );
 if( left_just )
  format = "%-";
 else
  format = "%";
 format += buf; format += "."; format += buf; format += "s";
 snprintf( buf, MSL, format.c_str(), input );

 output = str_dup( buf );

 return output;
}

/* Count the number of times <item> appears in <input> */
size_t substr_cnt( const char *input, const char *item )
{
 string output;
 size_t pos = 0;
 int cnt = 0;

 output = input;
 for ( (pos = output.find(item)); pos != string::npos; (pos = output.find(item,pos+1)) )
  cnt++;

 return cnt;
}

char *comma_print( unsigned long n )
{
    static int comma = '\0';
    static char retbuf[30];
    char *p = &retbuf[sizeof(retbuf)-1];
    int i = 0;

    if( comma == '\0' )
    {
        struct lconv *lcp = localeconv();

        if( lcp != NULL )
        {
            if( lcp->thousands_sep != NULL && *lcp->thousands_sep != '\0' )
                comma = *lcp->thousands_sep;
            else
                comma = ',';
        }
    }

    *p = '\0';

    do
    {
        if( i%3 == 0 && i != 0 )
            *--p = comma;
        *--p = '0' + n % 10;
        n /= 10;
        i++;
    }
    while( n != 0 );

    return p;
}

/* Courtesy of Yaz of 4th Realm */
char *initial( const char *str )
{
    static char strint[MAX_STRING_LENGTH];

    strint[0] = LOWER( str[0] );
    return strint;

}

char *cap_nocol( const char *str )
{
    static char strcap[MAX_STRING_LENGTH];
    int i;

    for ( i = 0; str[i] != '\0'; i++ )
        strcap[i] = LOWER( str[i] );
    strcap[i] = '\0';
    strcap[0] = UPPER( strcap[0] );
    return strcap;
}
