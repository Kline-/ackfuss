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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
/* For forks etc. */
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdarg.h>

#ifndef  DEC_ACK_H
#include "globals.h"
#include "tables.h"
#endif

#ifndef DEC_STRFUNS_H
#include "strfuns.h"
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
   for( ;; )
   {
      argument = one_argument( argument, arg1 );
      if( arg1[0] == '\0' )
         break;
      if( ( !str_cmp( "from", arg1 ) ) || ( !str_cmp( "in", arg1 ) ) )
      {
         argument = one_argument( argument, container_name );
      }
      else if( !str_cmp( "to", arg1 ) )
      {
         argument = one_argument( argument, victim_name );
      }
      else
      {
         if( is_number( arg1 ) )
         {
            argument = one_argument( argument, one_object );
            xprintf( holdbuf, "%s %s ", arg1, one_object );
            xcat( object_list, holdbuf );
         }
         else
         {
            xprintf( holdbuf, "1 %s ", arg1 );
            xcat( object_list, holdbuf );
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
   if( *arg == '\0' )
      return FALSE;

   if( *arg == '+' || *arg == '-' )
      arg++;

   for( ; *arg != '\0'; arg++ )
   {
      if( !isdigit( *arg ) )
         return FALSE;
   }

   return TRUE;
}



bool is_name( const char *str, char *namelist )
{
   char name[MAX_INPUT_LENGTH];

   for( ;; )
   {
      namelist = one_argument( namelist, name );
      if( name[0] == '\0' )
         return FALSE;
      if( !str_cmp( str, name ) )
         return TRUE;
   }
}

bool is_name_pfx( const char *str, char *namelist )
{
   char name[MAX_INPUT_LENGTH];

   for( ;; )
   {
      namelist = one_argument( namelist, name );
      if( name[0] == '\0' )
         return FALSE;
      if( !str_prefix( str, name ) )
         return TRUE;
   }
}

char *space_pad( const char *str, short final_size )
{
   short space_pad = my_strlen( str );
   static char padbuf[MSL];

   xprintf( padbuf, "%s", str );
   for( ; space_pad != final_size; space_pad++ )
      xcat( padbuf, " " );
   return padbuf;
}


/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 * Kept for backwards compatibility --Kline
 */
void smash_tilde( char *str )
{
 smash_replace(str,"~","-");
 return;
}

/*
 * Remove and replace in a string.
 * Easier than making X smash_Y functions. --Kline
 */ 
void smash_replace( char *str, char *sold, char *snew )
{
 for( ; *str != '\0'; str++ )
 {
  if( *str == *sold )
   *str = *snew;
 }
 return;
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
   if( astr == NULL )
   {
      /*
       * bug( "Str_cmp: null astr.", 0 );   
       */
      return TRUE;
   }

   if( bstr == NULL )
   {
/*	bug( "Str_cmp: null bstr.", 0 );  */
      return TRUE;
   }
   if( *astr == '^' )
   {
      notrunc = TRUE;
      astr++;
   }
   if( *bstr == '^' )
   {
      notrunc = TRUE;
      bstr++;
   }
   if( astr == NULL )
   {
      /*
       * bug( "Str_cmp: null astr.", 0 );   
       */
      return TRUE;
   }

   if( bstr == NULL )
   {
/*	bug( "Str_cmp: null bstr.", 0 );  */
      return TRUE;
   }


   for( ; *astr || *bstr; astr++, bstr++ )
   {
      if( !notrunc )
         if( *astr == '*' && count )
            return FALSE;

      if( LOWER( *astr ) != LOWER( *bstr ) )
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
   if( astr == NULL )
   {
      bug( "Strn_cmp: null astr.", 0 );
      return TRUE;
   }

   if( bstr == NULL )
   {
      bug( "Strn_cmp: null bstr.", 0 );
      return TRUE;
   }

   for( ; *astr; astr++, bstr++ )
   {
      if( LOWER( *astr ) != LOWER( *bstr ) )
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

   if( ( c0 = LOWER( astr[0] ) ) == '\0' )
      return FALSE;

   sstr1 = strlen( astr );
   sstr2 = strlen( bstr );

   for( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )
   {
      if( c0 == LOWER( bstr[ichar] ) && !str_prefix( astr, bstr + ichar ) )
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
   if( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) )
      return FALSE;
   else
      return TRUE;
}



/*
 * Returns an initial-capped string.
 */
#if 0
char *capitalize( const char *str )
{
   static char strcap[MAX_STRING_LENGTH];
   int i;

   for( i = 0; str[i] != '\0'; i++ )
   {
      if( ( str[i] == '@' ) && ( str[i + 1] == '@' ) && ( str[i + 2] != '\0' ) )
      {
         strcap[i] = str[i];
         strcap[i + 1] = str[i + 1];
         strcap[i + 2] = str[i + 2];
         i += 2;
      }
      else
         strcap[i] = LOWER( str[i] );
   }

   strcap[i] = '\0';
   for( i = 0; strcap[i] != '\0' && !isalpha( strcap[i] ); i++ );
   if( ( i > 0 ) && ( strcap[i] != '\0' ) )
      i++;
   if( strcap[i] != '\0' )
      strcap[i] = UPPER( strcap[i] );
   return strcap;
}
#endif
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
   if( ( strcap[i] = *str ) == '\0' )
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
            if( ignore_next )
               ignore_next = FALSE; /* We ignore the case of the this letter */
            else if( first_letter )
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
            if( ignore_next )
               ignore_next = FALSE; /* We ignore the case of the this letter */
            else if( first_letter )
               first_letter = FALSE;   /* We ignore the case of the this letter */
            else
               strcap[i] = LOWER( strcap[i] );  /* Set this letter to lowercase */
            count = 0;
            break;
         case '@':
            if( ++count >= 2 )   /* If there are two or more '@@'s in a row */
               ignore_next = TRUE;  /* Set the flag to ignore the next letter */
            break;
      }
   }
   while( ( strcap[++i] = *++str ) != '\0' );   /* loop until end of string */

   i = 0;   /* str = oldstr;  Reset variables */
#if 0
   /*
    * Copy strcap back into the old string 
    */
   while( ( *str++ = strcap[i++] ) != '\0' )
      ;

   return ( oldstr );   /* Return pointer to start of old string */
#endif
   return ( strcap );
}



/*
 * We need a replacement for strlen() which will take the color
 * codes into account when reporting a string's length.
 * -- Stephen
 */

int my_strlen( const char *text )
{
   char c;
   int i;
   int status;
   int length;
   int strlen_size;

   status = 0;
   length = 0;
   strlen_size = strlen( text );

   for( i = 0; i < strlen_size; i++ )
   {

      c = text[i];
      length++;

      switch ( status )
      {
         case 0:
         case 1:
            if( c == '@' )
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

char *learnt_name( int learnt )
{
   /*
    * For skills/spells.  Return a string indicating how well the
    * skill is learnt. 
    */

   if( learnt < 1 )
      return "Unknown";
   else if( learnt < 15 )
      return "Awful";
   else if( learnt < 30 )
      return "Weak";
   else if( learnt < 40 )
      return "Poor";
   else if( learnt < 50 )
      return "Average";
   else if( learnt < 60 )
      return "Fair";
   else if( learnt < 70 )
      return "Good";
   else if( learnt < 80 )
      return "Great";
   else if( learnt < 85 )
      return "Superb";
   else if( learnt < 90 )
      return "Amazing";
   else if( learnt < 101 )
      return "Godlike";
   else if( learnt < 102 )
      return "RACE";
   else
      return "Godlike";
}

char *get_adept_name( CHAR_DATA * ch )
{
   /*
    * this is weak for now..will eventually have like 200 total names, based on the remort 
    * classes the adept has   
    */

   switch ( ch->adept_level )
   {

      case 1:
         return "@@W    Mystic    @@N";
         break;
      case 2:
         return "@@a   Templar    @@N";
         break;
      case 3:
         return "@@l Illusionist  @@N";
         break;
      case 4:
         return "@@e   Crusader   @@N";
         break;
      case 5:
         return "@@d   Warlock    @@N";
         break;
      case 6:
         return "@@a   Paladin    @@N";

      case 7:
         return "@@r    Ranger    @@N";
         break;
      case 8:
         return "@@c  Gladiator   @@N";
         break;
      case 9:
         return "@@l    Shogun    @@N";
         break;
      case 10:
         return "@@e    Shamen    @@N";
         break;
      case 11:
         return "@@r    Druid     @@N";
         break;
      case 12:
         return "@@b  Conjurer    @@N";

      case 13:
         return "@@l Elementalist @@N";
         break;
      case 14:
         return "@@m  Runemaster  @@N";

      case 15:
         return "@@d Shadowmaster @@N";
         break;
      case 16:
         return "@@b Beastmaster  @@N";
         break;
      case 17:
         return "@@R   Warlord    @@N";
         break;
      case 18:
         return "@@e  Dragonlord  @@N";
         break;
      case 19:
         return "@@d  Demonlord   @@N";
         break;
      case 20:
         return "@@m  Realm Lord  @@N";



   }
   return "@@W    Adept     @@N";
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

   for( i = 0; i < strlen_size; i++ )
   {

      c = text[i];
      length++;

      switch ( status )
      {
         case 0:
         case 1:
            if( c == '@' )
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

   for( i = 0; i < strlen_size; i++ )
   {

      c = text[i];

      switch ( status )
      {
         case 0:
         case 1:
            if( c == '@' )
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
   if( ( len = my_strlen( text ) ) >= width )
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



char *str_mod( char *mod_string, char *argument )
{


   char arg1[MAX_INPUT_LENGTH];
   char buf[MAX_STRING_LENGTH];
   char *buf2;
   char *buf3 = NULL;
   char *word;
   char temp[MSL];
   int i = 0;
   bool multiple = FALSE;

   if( !str_cmp( argument, "" ) )
   {
      xprintf_2( bug_buf, "Unknown reason for return, argument is -%s-", argument );
      monitor_chan( bug_buf, MONITOR_DEBUG );
      return mod_string;

   }


   if( argument[0] == '+' || argument[0] == '-' )
   {
      buf[0] = '\0';
      smash_tilde( argument );
      if( argument[0] == '+' )
      {
         argument++;

         if( mod_string )
            xcat( buf, mod_string );
         while( isspace( *argument ) )
            argument++;
         if( !str_infix( argument, mod_string ) )
         {
            return mod_string;
         }
         xcat( buf, argument );
         xcat( buf, " " );
      }
      if( argument[0] == '-' )
      {
         argument++;
         if( argument[0] == '\'' )
            multiple = TRUE;
         one_argument( argument, arg1 );

         if( multiple )
         {
            xprintf( temp, "\'%s\'", arg1 );
            xprintf( arg1, "%s", temp );
         }

         if( arg1[0] != '\0' )
         {
            buf2 = str_dup( mod_string );
            buf3 = buf2;
            if( ( word = strstr( buf2, arg1 ) ) == NULL )
            {
               free_string( buf2 );
               return mod_string;
            }
            else
            {
               while( buf2 != word )
                  buf[i++] = *( buf2++ );
               while( ( !isspace( *( buf2++ ) ) ) || ( ( multiple ) && ( ( buf2[0] != '\'' ) && ( buf2[0] != '\0' ) ) ) );
               buf[i] = '\0';
               xcat( buf, buf2 );
            }
         }

      }

      free_string( buf3 );
      word = buf2 = buf3 = NULL;
      free_string( mod_string );
      mod_string = str_dup( buf );
   }
   return mod_string;
}
void rand_arg( char *argument, char *output )
{
   char temp[MSL];
   short counter = 0;
   argument = one_argument( argument, temp );
   while( temp[0] )
   {
      if( number_range( 0, counter ) == 0 )
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

   for( pdot = argument; *pdot != '\0'; pdot++ )
   {
      if( *pdot == '.' )
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
   if( argument == NULL )
      return NULL;
   while( isspace( *argument ) )
      argument++;

   cEnd = ' ';
   if( *argument == '\'' || *argument == '"' )
      cEnd = *argument++;

   while( *argument != '\0' )
   {
      if( *argument == cEnd )
      {
         argument++;
         break;
      }
      *arg_first = LOWER( *argument );
      arg_first++;
      argument++;
   }
   *arg_first = '\0';

   while( isspace( *argument ) )
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
   if( argument == NULL )
      return NULL;
   while( isspace( *argument ) )
      argument++;

   cEnd = ' ';
   if( *argument == '\'' || *argument == '"' )
      cEnd = *argument++;

   while( *argument != '\0' )
   {
      if( *argument == cEnd )
      {
         argument++;
         break;
      }
      *arg_first = ( *argument );
      arg_first++;
      argument++;
   }
   *arg_first = '\0';

   while( isspace( *argument ) )
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
   char *i, *b = buf;
   int slen;

   if( !orig || !strip )
   {
/*    log_f("strip_out called with NULL param");  */
      return "!!OUT!!";
   }
   slen = strlen( strip );
   for( i = strstr( orig, strip ); i; i = strstr( orig, strip ) )
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
   char *i, *b = buf;
   int slen;

   if( !orig || !strip )
   {
/*    log_f("strip_out called with NULL param");  */
      return "!!OUT!!";
   }
   slen = strlen( strip ) + 1;
   for( i = strstr( orig, strip ); i; i = strstr( orig, strip ) )
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

   for( ;; )
   {
      checklist = one_argument( checklist, check_word );
      againstlist = second_list;
      if( check_word[0] == '\0' )
      {
         return FALSE;
      }
      for( ;; )
      {
         againstlist = one_argument( againstlist, against_word );
         if( against_word[0] == '\0' )
            break;
         if( !str_cmp( check_word, against_word ) )
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

   }

   xprintf( log, "Item_type_name: Object: %d.  Unknown Type: %d", obj->pIndexData->vnum, obj->item_type );
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
      case APPLY_GOLD:
         return "gold";
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

   xprintf( buf, "affect_location_name: location %d unknown.", location );
   monitor_chan( buf, MONITOR_OBJ );

   bug( "Affect_location_name: unknown location %d.", location );
   return "(unknown)";
}

const char *raffect_bit_name( int vector )
{
   static char rbuf[512];

   rbuf[0] = '\0';

   if( vector & ROOM_BV_NONE )
      xcat( rbuf, " NONE" );
   if( vector & ROOM_BV_SILENCE )
      xcat( rbuf, " Silence" );
   if( vector & ROOM_BV_SAFE )
      xcat( rbuf, " Safe" );
   if( vector & ROOM_BV_ENCAPS )
      xcat( rbuf, " Seal Room" );
   if( vector & ROOM_BV_SHADE )
      xcat( rbuf, " Shade" );
   if( vector & ROOM_BV_HEAL_REGEN )
      xcat( rbuf, " @@mHealing Light@@N" );
   if( vector & ROOM_BV_HEAL_STEAL )
      xcat( rbuf, " @@dWither Shadow@@N" );
   if( vector & ROOM_BV_MANA_REGEN )
      xcat( rbuf, " @@eMana Flare@@N" );
   if( vector & ROOM_BV_MANA_STEAL )
      xcat( rbuf, " @@dMana Drain@@N" );
   if( vector & ROOM_BV_FIRE_RUNE )
      xcat( rbuf, " @@eFire @@NRune" );
   if( vector & ROOM_BV_FIRE_TRAP )
      xcat( rbuf, " @@eFire @@NTrap" );
   if( vector & ROOM_BV_DAMAGE_TRAP )
      xcat( rbuf, " @@dDamage @@NTrap" );
   if( vector & ROOM_BV_SHOCK_RUNE )
      xcat( rbuf, " @@lShock @@NRune" );
   if( vector & ROOM_BV_SHOCK_TRAP )
      xcat( rbuf, " @@lShock @@NTrap" );
   if( vector & ROOM_BV_HOLD )
      xcat( rbuf, " @@rCage@@N" );
   if( vector & ROOM_BV_POISON_RUNE )
      xcat( rbuf, " @@Poison@@N Rune" );

   return ( rbuf[0] != '\0' ) ? rbuf + 1 : "none";
}

/*
 * Return ascii name of an affect bit vector.
 */
const char *affect_bit_name( int vector )
{
   static char buf[512];

   buf[0] = '\0';
   if( vector & AFF_BLIND )
      xcat( buf, " blind" );
   if( vector & AFF_INVISIBLE )
      xcat( buf, " invisible" );
   if( vector & AFF_DETECT_EVIL )
      xcat( buf, " detect_evil" );
   if( vector & AFF_DETECT_INVIS )
      xcat( buf, " detect_invis" );
   if( vector & AFF_DETECT_MAGIC )
      xcat( buf, " detect_magic" );
   if( vector & AFF_DETECT_HIDDEN )
      xcat( buf, " detect_hidden" );
   if( vector & AFF_HOLD )
      xcat( buf, " hold" );
   if( vector & AFF_SANCTUARY )
      xcat( buf, " sanctuary" );
   if( vector & AFF_FAERIE_FIRE )
      xcat( buf, " faerie_fire" );
   if( vector & AFF_INFRARED )
      xcat( buf, " infrared" );
   if( vector & AFF_CURSE )
      xcat( buf, " curse" );
   if( vector & AFF_CLOAK_FLAMING )
      xcat( buf, " flaming" );
   if( vector & AFF_POISON )
      xcat( buf, " poison" );
   if( vector & AFF_PROTECT )
      xcat( buf, " protect" );
   if( vector & AFF_PARALYSIS )
      xcat( buf, " paralysis" );
   if( vector & AFF_SLEEP )
      xcat( buf, " sleep" );
   if( vector & AFF_SNEAK )
      xcat( buf, " sneak" );
   if( vector & AFF_HIDE )
      xcat( buf, " hide" );
   if( vector & AFF_CHARM )
      xcat( buf, " charm" );
   if( vector & AFF_FLYING )
      xcat( buf, " flying" );
   if( vector & AFF_PASS_DOOR )
      xcat( buf, " pass_door" );
   if( vector & AFF_CLOAK_REFLECTION )
      xcat( buf, " cloak:reflection" );
   if( vector & AFF_CLOAK_ABSORPTION )
      xcat( buf, " cloak:absorption" );
   if( vector & AFF_CLOAK_ADEPT )
      xcat( buf, " cloak:adept" );
   if( vector & AFF_CLOAK_REGEN )
      xcat( buf, " cloak:regeneration" );

   return ( buf[0] != '\0' ) ? buf + 1 : "none";
}

/*
 * Return ascii name of extra flags vector.
 */
const char *extra_bit_name( int extra_flags )
{
   static char buf[512];

   buf[0] = '\0';
   if( extra_flags & ITEM_GLOW )
      xcat( buf, " glow" );
   if( extra_flags & ITEM_HUM )
      xcat( buf, " hum" );
   if( extra_flags & ITEM_NODISARM )
      xcat( buf, " nodisarm" );
   if( extra_flags & ITEM_LOCK )
      xcat( buf, " lock" );
   if( extra_flags & ITEM_EVIL )
      xcat( buf, " evil" );
   if( extra_flags & ITEM_INVIS )
      xcat( buf, " invis" );
   if( extra_flags & ITEM_MAGIC )
      xcat( buf, " magic" );
   if( extra_flags & ITEM_NODROP )
      xcat( buf, " nodrop" );
   if( extra_flags & ITEM_BLESS )
      xcat( buf, " bless" );
   if( extra_flags & ITEM_ANTI_GOOD )
      xcat( buf, " anti-good" );
   if( extra_flags & ITEM_ANTI_EVIL )
      xcat( buf, " anti-evil" );
   if( extra_flags & ITEM_ANTI_NEUTRAL )
      xcat( buf, " anti-neutral" );
   if( extra_flags & ITEM_NOREMOVE )
      xcat( buf, " noremove" );
   if( extra_flags & ITEM_INVENTORY )
      xcat( buf, " inventory" );
   if( extra_flags & ITEM_NOLOOT )
      xcat( buf, " noloot" );
   if( extra_flags & ITEM_NOSAC )
      xcat( buf, " nosac" );
   if( extra_flags & ITEM_REMORT )
      xcat( buf, " remort" );
   if( extra_flags & ITEM_CLAN_EQ )
      xcat( buf, " claneq" );
   if( extra_flags & ITEM_NOSAVE )
      xcat( buf, " nosave" );
   if( extra_flags & ITEM_NO_AUCTION )
      xcat( buf, " no_auction" );
   if( extra_flags & ITEM_RARE )
      xcat( buf, " rare" );
   if( extra_flags & ITEM_VAMP )
      xcat( buf, " vamp" );
   if( extra_flags & ITEM_UNIQUE )
      xcat( buf, " unique" );
   if( extra_flags & ITEM_TRIG_DESTROY )
      xcat( buf, " trigger:destroy" );
   if( extra_flags & ITEM_LIFESTEALER )
      xcat( buf, " lifestealer" );
   if( extra_flags & ITEM_SILVER )
      xcat( buf, " silver" );

   return ( buf[0] != '\0' ) ? buf + 1 : "none";
}

/* Return the ASCII name of a character's race
 * -- Stephen
 */

char *race_name( CHAR_DATA * ch )
{

   if( IS_NPC( ch ) )
      return ( "NPC" );

   if( ch->race >= MAX_RACE || ch->race < 0 )
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

   if( IS_NPC( ch ) )
      xprintf( buf, "NPC" );
   else if( ch->race >= MAX_RACE || ch->race < 0 )
   {
      /*
       * error reporting here one day... maybe 
       */
      xprintf( buf, "unknown!" );
   }
   else
      xprintf( buf, "%s", race_table[ch->race].race_name );

   return ( buf );
}


char *get_family_name( CHAR_DATA * ch )
{

   switch ( ch->pcdata->super->bloodline )
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
   switch ( ch->pcdata->super->bloodline )
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

/*Originaly, i heard on gamedev that someone was looking for a way to stop sprintf
 *from overflowing, ofcourse, people fail to think of functions like strlcpy, which
 *is essentaily a safe sprintf, atleast as it has been described to me.  I however
 *chose to write my own system.  This allows me to capture the function, and line
 *of the overflowing string in question.  Thus allowing me to essentaily find out
 *where the mud overflows, and stop it from happening.  Great little tool.
 *All i ask is that you leave this header in place.  -- Darien of Sandstorm:Mages Sanctuary
 */
int safe_printf( const char *file, const char *function, int line, int size, const char *str, const char *fmt, ... )
{
 char buf[MAS];
 va_list args;
 va_start(args,fmt);
 vsprintf(buf,fmt,args);
 va_end(args);

 /*Max Alloc Size is alot!*/
 if( size > MAS )
 {
  /*Yes, this is a potential loop bug if infact the xprintf does collapse in on itself..*/
  xprintf_2(bug_buf,"xprintf buffer overflow: %s",fmt);
  log_string(bug_buf);
  monitor_chan(bug_buf,MONITOR_DEBUG);

  xprintf_2(bug_buf,"xprintf buffer overflow: File: %s Function: %s Line: %d.",file,function,line);
  log_string(bug_buf);
  monitor_chan(bug_buf,MONITOR_DEBUG);

  return -1;
 }

 if( (unsigned)size < strlen(buf)+1 ) 
 {
  /*Yes, this is a potential loop bug if infact the xprintf does collapse in on itself..*/
  xprintf_2(bug_buf,"xprintf buffer overflow: %s",fmt);
  log_string(bug_buf);
  monitor_chan(bug_buf,MONITOR_DEBUG);

  xprintf_2(bug_buf,"xprintf buffer overflow: File: %s Function: %s Line: %d",file,function,line);
  log_string(bug_buf);
  monitor_chan(bug_buf,MONITOR_DEBUG);

  return -1;
 }
 else
 {
  strcpy((char *)str,buf);
  //Disabled for now.
  //strlcpy(str,buf,size-2);
  //str[size-1] = '\0';

  /*Just double checking.*/
  if( strlen(str) > (unsigned)size - 1 )
  {
   /*Yes, this is a potential loop bug if infact the xprintf does collapse in on itself..*/
   xprintf_2(bug_buf,"xprintf buffer overflow: %s",fmt);
   log_string(bug_buf);
   monitor_chan(bug_buf,MONITOR_DEBUG);

   xprintf_2(bug_buf,"xprintf buffer overflow: File: %s Function: %s Line: %d",file,function,line);
   log_string(bug_buf);
   monitor_chan(bug_buf,MONITOR_DEBUG);
   return -1;
  }
 }
 return strlen(str);
}

void safe_strcat( const char *file, const char *function, int line, int size, const char *prev, const char *next, ... )
{
 char buf[MAS];
 va_list args;
 va_start(args,next);
 vsprintf(buf,next,args);
 va_end(args);

 /*Max Alloc Size is alot!*/
 if( size > MAS )
 {
  xprintf_2(bug_buf,"xcat buffer overflow: %s",next);
  log_string(bug_buf);
  monitor_chan(bug_buf,MONITOR_DEBUG);

  xprintf_2(bug_buf,"xcat buffer overflow: File: %s Function: %s Line: %d.",file,function,line);
  log_string(bug_buf);
  monitor_chan(bug_buf,MONITOR_DEBUG);

  return;
 }

 if( (unsigned)size < strlen(buf)+1 )
 {
  xprintf_2(bug_buf,"xcat buffer overflow: %s",next);
  log_string(bug_buf);
  monitor_chan(bug_buf,MONITOR_DEBUG);

  xprintf_2(bug_buf,"xcat buffer overflow: File: %s Function: %s Line: %d",file,function,line);
  log_string(bug_buf);
  monitor_chan(bug_buf,MONITOR_DEBUG);


  return;
 }
 else
 {
  strcat((char *)prev,buf);

  /*Just double checking.*/
  if( strlen(prev) > (unsigned)size - 1 )
  {
   xprintf_2(bug_buf,"xcat buffer overflow: %s",next);
   log_string(bug_buf);
   monitor_chan(bug_buf,MONITOR_DEBUG);

   xprintf_2(bug_buf,"xcat buffer overflow: File: %s Function: %s Line: %d",file,function,line);
   log_string(bug_buf);
   monitor_chan(bug_buf,MONITOR_DEBUG);
   return;
  }
 }
}

char *strlower( char *ip )
{
 static char buf[MAX_STRING_LENGTH];
 int i;

 for( i = 0; i < (MAX_STRING_LENGTH - 1) && ip[i] != '\0'; i++ )
  buf[i] = tolower(ip[i]);

 buf[i] = '\0';
 return buf;
}

char *strupper( char *ip )
{
 static char buf[MAX_STRING_LENGTH];
 int i;

 for( i = 0; i < (MAX_STRING_LENGTH - 1) && ip[i] != '\0'; i++ )
  buf[i] = toupper(ip[i]);

 buf[i] = '\0';
 return buf;
}
