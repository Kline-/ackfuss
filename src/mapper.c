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

/* This code inspired by a snippet from :                                  */

/************************************************************************/
/* mlkesl@stthomas.edu	=====>	Ascii Automapper utility                */
/* Let me know if you use this. Give a newbie some _credit_,            */
/* at least I'm not asking how to add classes...                        */
/* Also, if you fix something could ya send me mail about, thanks       */
/* PLEASE mail me if you use this or like it, that way I will keep it up*/
/************************************************************************/

#include "h/globals.h"

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_CURSOR_H
#include "h/cursor.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_MAPPERC_H
#include "h/mapperc.h"
#endif

int door_marks[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };
int offsets[4][2] = { {-2, 0}, {0, 2}, {2, 0}, {0, -2} };

#define SECT_HERE	SECT_MAX
#define SECT_UNSEEN	( SECT_MAX + 1 )
#define SECT_BLOCKED	( SECT_UNSEEN + 1 )
#define SECT_TOP	( SECT_BLOCKED + 1 )


const struct map_info_type door_info[] = {
   {DOOR_LOCKED, "@@R", "#@@N", "@@f", "Locked door"},
   {DOOR_OPEN, "@@W", "#@@N", "@@f", "Open door"},
   {DOOR_CLOSED, "@@R", "#@@N", "@@f", "Closed Door"},
   {DOOR_NS, "@@W", "|@@N", "@@f", "N/S Exit"},
   {DOOR_EW, "@@W", "-@@N", "@@f", "E/W Exit"},
   {DOOR_NULL, "@@N", " ", "@@f", "Invalid"}
};

const struct map_info_type map_info[] = {
   {SECT_BLOCKED, "@@m", "~@@N", "", "blocked"},
   {SECT_UNSEEN, "@@N@@d", " @@N", "", "unknown"},
   {SECT_HERE, "@@N@@W", "*@@N", "", "you!"},
   {SECT_CITY, "@@g@@i", " @@N", "", "city"},
   {SECT_FIELD, "@@r@@i", " @@N", "", "field"},
   {SECT_FOREST, "@@3", " @@N", "@@W", "forest"},
   {SECT_HILLS, "@@4", " @@N", "@@W", "hills"},
   {SECT_MOUNTAIN, "@@2", " @@N", "", "mountain"},
   {SECT_WATER_SWIM, "@@l@@i", " @@N", "", "shallow water"},
   {SECT_WATER_NOSWIM, "@@1", " @@N", "", "deep running water"},
   {SECT_RECALL_OK, "@@5", " @@N", "", "inside"},
   {SECT_AIR, "@@a@@i", " @@N", "", "air"},
   {SECT_DESERT, "@@y@@i", " @@N", "", "desert"},
   {SECT_INSIDE, "@@d@@i", " @@N", "", "inside"},
   {SECT_TOP, "@@y", "~@@N", "", "bad sector type"}
};

char *get_sector_display( int sector )
{
   short looper;
   for( looper = 0;; looper++ )
      if( ( map_info[looper].sector_type == sector ) || ( map_info[looper].sector_type == SECT_TOP ) )
         break;
   return ( map_info[looper].display_code );
}
char *get_sector_color( int sector )
{
   short looper;
   for( looper = 0;; looper++ )
      if( ( map_info[looper].sector_type == sector ) || ( map_info[looper].sector_type == SECT_TOP ) )
         break;
   return ( map_info[looper].display_color );
}
char *get_invert_color( int sector )
{
   short looper;
   for( looper = 0;; looper++ )
      if( ( map_info[looper].sector_type == sector ) || ( map_info[looper].sector_type == SECT_TOP ) )
         break;
   return ( map_info[looper].invert_color );
}
char *get_door_display( int door )
{
   short looper;
   for( looper = 0;; looper++ )
      if( ( door_info[looper].sector_type == door ) || ( door_info[looper].sector_type == DOOR_NULL ) )
         break;
   return ( door_info[looper].display_code );
}
char *get_door_color( int door )
{
   short looper;
   for( looper = 0;; looper++ )
      if( ( door_info[looper].sector_type == door ) || ( door_info[looper].sector_type == DOOR_NULL ) )
         break;
   return ( door_info[looper].display_color );
}
char *get_sector_name( int sector )
{
   short looper;
   for( looper = 0;; looper++ )
      if( ( map_info[looper].sector_type == sector ) || ( map_info[looper].sector_type == SECT_TOP ) )
         break;
   return ( map_info[looper].desc );
}


/*
 * This code written by Altrag for Ack!Mud 
 */

#define iseol(c)	((c)=='\n' || (c)=='\r')

/* Like one_argument but saves case, and if len isnt NULL, fills it in
 * with the length.  Also accounts for color.  Does NOT account for
 * quoted text. */
char *break_arg( char *str, char *first_arg, int bufsize, int max, int *buflen, int *len )
{
   int slen = 0;
   char *arg = NULL;

   while( isspace( *str ) )
      ++str;
   if( *str == '\\' && str[1] == 'b' && str[2] == 'r' )
   {
      strcpy( first_arg, "\r\n" );
      if( buflen )
         *buflen = 0;
      if( len )
         *len = 0;
      str += 3;
      while( isspace( *str ) )
         ++str;
      return str;
   }
   arg = first_arg;
   while( *str && arg - first_arg < bufsize && slen < max )
   {
      if( isspace( *str ) )
      {
         ++str;
         break;
      }
      else if( *str == '@' && str[1] == '@' && str[2] != '\0' )
      {
         if( arg - first_arg >= max - 3 )
            break;
         *arg++ = *str++;
         *arg++ = *str++;
         *arg++ = *str++;
      }
      else if( *str == '\\' && str[1] == 'b' && str[2] == 'r' )
         break;
      else
      {
         *arg++ = *str++;
         ++slen;
      }
   }
   *arg = '\0';
   if( len )
      *len = slen;
   if( buflen )
      *buflen = arg - first_arg;
   while( isspace( *str ) )
      ++str;
   return str;
}

char *string_justify( char *str, int len, int width, int numwords, int *rlen )
{
   static char buf[MSL];
   char arg[MAX_INPUT_LENGTH];
   int minspaces = numwords - 1;
   int spaces_needed;
   float space_ratio, space_between;
   int i, j = 0, alen;
   char *bp = buf;

   spaces_needed = minspaces + ( width - ( len + 1 ) );
   if( spaces_needed <= minspaces || minspaces <= 0 )
   {
      snprintf( buf, MSL, "%s\r\n", str );
      return buf;
   }
   space_ratio = ( float )spaces_needed / ( float )minspaces;
   space_between = space_ratio;
   for( i = 0; i < minspaces; ++i )
   {
      str = break_arg( str, arg, sizeof( arg ), width, &alen, NULL );
      strcpy( bp, arg );
      bp += alen;
      for( ; j < ( int )( space_between + 0.5 ); ++j )
         *bp++ = ' ';
      space_between += space_ratio;
   }
   str = break_arg( str, arg, sizeof( arg ), width, &alen, NULL );
   strcpy( bp, arg );
   bp += alen;
/*  bp += snprintf(bp, MSL, "\r\n%d:%d:%d", len, width, numwords);
  bp += snprintf(bp, MSL, "\r\n%d:%d:%f", minspaces, spaces_needed, space_ratio);*/
   *bp++ = '\n';
   *bp++ = '\r';
   *bp = '\0';
   if( rlen )
      *rlen = bp - buf;
   return buf;
}

char last_color( char *str )
{
   char *end = '\0';

   for( end = str + strlen( str ) - 2; end > str; --end )
      if( *end == '@' && end[-1] == '@' )
         return end[1];
   return '\0';
}

char *string_format( char *str, int *numlines, int width, int height, bool unjust )
{
   static char ret[MSL];
   char buf[MSL];
   char arg[MIL];
   int alen = 0;
   int blen = 0;
   char *pbuf = buf, *pret = ret;
   int len = 0;
   int currline = 0;
   int last = 0;
   char *sp = NULL;
   char c = '\0';
   int numwords = 0;
   int jlen = 0;

   --height;
   --width;
   *pret = '\0';
   *pbuf = '\0';
   buf[0] = '\0';
   arg[0] = '\0';

   for( sp = break_arg( str, arg, sizeof( arg ), width, &len, &alen ); *arg;
        sp = break_arg( sp, arg, sizeof( arg ), width, &len, &alen ) )
   {
      blen += alen;

      if( blen + 1 >= width || iseol( *arg ) )
      {
         *pbuf++ = '\n';
         *pbuf++ = '\r';
         *pbuf = '\0';
         c = last_color( buf );
         if( unjust || iseol( *arg ) )
         {
            strcpy( pret, buf );
            pret += pbuf - buf;
         }
         else
         {
            strcpy( pret, string_justify( buf, blen - alen, width, numwords, &jlen ) );
            pret += jlen;
         }
         pbuf = buf;
         if( ++currline > height )
            break;
         if( c )
         {
            *pbuf++ = '@';
            *pbuf++ = '@';
            *pbuf++ = c;
         }
         blen = alen;
         if( iseol( *arg ) )
            *arg = '\0';
         numwords = 0;
      }
      else if( pbuf > buf )
      {
         if( pbuf - buf > 2 && pbuf[-2] == '@' && pbuf[-3] == '@' )
         {
            if( pbuf - buf == 3 )
               last = 0;
            else
               last = -4;
         }
         else
            last = -1;
         if( last )
         {
            if( unjust && pbuf[last] == '.' )
               *pbuf++ = ' ', ++blen;
//        if (!iseol(pbuf[last]))
            *pbuf++ = ' ', ++blen;
         }
      }
      strcpy( pbuf, arg );
      pbuf += len;
      ++numwords;
   }
   if( pbuf > buf )
   {
      if( pbuf - buf > 2 && pbuf[-2] == '@' && pbuf[-3] == '@' )
      {
         if( pbuf - buf == 3 )
            last = 0;
         else
            last = -4;
      }
      else
         last = -1;
      if( last && pbuf[last] != '\n' && pbuf[last] != '\r' )
      {
         *pbuf++ = '\n';
         *pbuf++ = '\r';
         ++currline;
      }
   }
   *pbuf = '\0';
   strcpy( pret, buf );
   if( numlines )
      *numlines = currline;
   return ret;
}

char *map_format( char *str, int start, char cmap[MAP_Y][MSL], int *numlines, int term_width, int height, bool unjust )
{
   static char ret[MSL];
   char buf[MSL];
   char arg[MIL];
   int width = ( start < MAP_Y ? term_width - 15 : term_width - 1 );
   int alen = 0;
   int blen = 0;
   char *pbuf = buf, *pret = ret;
   int len = 0;
   int currline = start;
   int last = 0;
   char *sp = NULL;
   char c = '\0';
   int numwords = 0;
   int jlen = 0;

   buf[0] = '\0';
   arg[0] = '\0';
   --height;
   *pbuf = '\0';
   *pret = '\0';
   for( sp = break_arg( str, arg, sizeof( arg ), width, &len, &alen ); *arg;
        sp = break_arg( sp, arg, sizeof( arg ), width, &len, &alen ) )
   {
      blen += alen;
      if( blen + 1 >= width || iseol( *arg ) )
      {
         *pbuf++ = '\n';
         *pbuf++ = '\r';
         *pbuf = '\0';
         c = last_color( buf );
         if( currline < MAP_Y )
            pret += snprintf( pret, MSL, "%s   ", cmap[currline] );
         else if( currline == MAP_Y )
            strcpy( pret, "              " ), pret += 14;
         if( unjust || iseol( *arg ) )
         {
            strcpy( pret, buf );
            pret += pbuf - buf;
         }
         else
         {
            strcpy( pret, string_justify( buf, blen - alen, width, numwords, &jlen ) );
            pret += jlen;
         }
         if( currline == MAP_Y )
            width = term_width - 1;
         pbuf = buf;
         if( ++currline > height )
            break;
         if( c )
         {
            *pbuf++ = '@';
            *pbuf++ = '@';
            *pbuf++ = c;
         }
         blen = alen;
         if( iseol( *arg ) )
            *arg = '\0';
         numwords = 0;
      }
      else if( pbuf > buf )
      {
         if( pbuf - buf > 2 && pbuf[-2] == '@' && pbuf[-3] == '@' )
         {
            if( pbuf - buf == 3 )
               last = 0;
            else
               last = -4;
         }
         else
            last = -1;
         if( last )
         {
            if( unjust && pbuf[last] == '.' )
               *pbuf++ = ' ', ++blen;
//        if (!iseol(pbuf[last]))
            *pbuf++ = ' ', ++blen;
         }
      }
      strcpy( pbuf, arg );
      pbuf += len;
      ++numwords;
   }
/*  snprintf(bug_buf, (2* MIL), "%d:%d", width, blen);
  monitor_chan(bug_buf, MONITOR_DEBUG);*/
   if( pbuf > buf )
   {
      if( pbuf - buf > 2 && pbuf[-2] == '@' && pbuf[-3] == '@' )
      {
         if( pbuf - buf == 3 )
            last = 0;
         else
            last = -4;
      }
      else
         last = -1;
      if( last && pbuf[last] != '\n' && pbuf[last] != '\r' )
      {
         *pbuf++ = '\n';
         *pbuf++ = '\r';
         if( currline < MAP_Y )
            pret += snprintf( pret, MSL, "%s   ", cmap[currline] );
         else if( currline == MAP_Y || ( currline == MAP_Y + 1 && blen <= term_width - 15 ) )
            strcpy( pret, "              " ), pret += 14;
         ++currline;
      }
   }
   *pbuf = '\0';
   strcpy( pret, buf );
   if( numlines )
      *numlines = currline;
   return ret;
}

char *exit_string( CHAR_DATA * ch, ROOM_INDEX_DATA * r )
{
   int e;
   static char buf[128];

   strcpy( buf, "[" );
   for( e = 0; e < 6; e++ )
      if( r->exit[e] && r->exit[e]->to_room && !str_cmp( "", r->exit[e]->keyword ) )
      {
         if( r->exit[e]->exit_info.test(EX_CLOSED) )
         {
            if( r->exit[e]->exit_info.test(EX_NODETECT) )
            {
               continue;
            }
            else if( ( IS_NPC( ch ) ? 50 : ch->pcdata->learned[gsn_find_doors] ) > number_percent(  ) )
            {
               snprintf( buf + strlen( buf ), 128, " (%s)", compass_name[e] );
            }
         }
         else
         {
            snprintf( buf + strlen( buf ), 128, " %s", compass_name[e] );
         }
      }
   strncat( buf, " ]", MSL );
   return buf;
}

void disp_map( char *border, char *pmap, CHAR_DATA * ch )
{
   int cols = ( IS_NPC( ch ) ? 80 : ch->pcdata->term_columns );
   int rows = ( IS_NPC( ch ) || !ch->act.test(ACT_FULL_ANSI ) ? 9999 : 10 );
   char bufs[MAP_Y][MSL];
   char disp[MSL];
   int y, ty = MAP_Y - 1;
   char *x, *ox;

   strcpy( bufs[0], border );
   strcpy( bufs[ty], border );
   x = pmap;
   for( y = 1; y < ty && *x; ++y )
   {
      while( *x == '\n' || *x == '\r' )
         ++x;
      ox = x;
      while( *x != '\n' && *x != '\r' && *x != '\0' )
         ++x;
      snprintf( bufs[y], MSL, "%.*s", static_cast<int>(( x - ox )), ox );
   }
   if( !IS_NPC( ch ) && ch->act.test(ACT_FULL_ANSI ) )
   {
      snprintf( disp, MSL, "%s%s%i;%ir%s%i;%iH%s%s",
               CRS_SAVE_ALL,
               CRS_CMD,
               ch->pcdata->term_rows - 11, ch->pcdata->term_rows - 1, CRS_CMD, ch->pcdata->term_rows - 11, 0, CRS_CMD, "J" );
      send_to_char( disp, ch );
   }
   strcpy( disp, map_format( ch->in_room->name, 0, bufs, &y, cols, rows, TRUE ) );
   strncat( disp, map_format( exit_string( ch, ch->in_room ), y, bufs, &y, cols, rows, TRUE ), MSL-1 );
   strncat( disp, map_format( tagline_format(ch->in_room->description,ch), y, bufs, &y, cols, rows, !ch->act.test(ACT_JUSTIFY ) ), MSL-1 );
   if( y < MAP_Y )
   {
      x = disp + strlen( disp );
      while( y < MAP_Y )
         x += snprintf( x, MSL, "%s\r\n", bufs[y] ), ++y;
   }
   send_to_char( disp, ch );
   if( !IS_NPC( ch ) && ch->act.test(ACT_FULL_ANSI ) )
   {
      snprintf( disp, MSL, "%s%i;%ir%s%i;%iH", CRS_CMD, 0, ch->pcdata->term_rows - 12, CRS_CMD, ch->pcdata->term_rows - 13, 0 );
      send_to_char( disp, ch );
   }
   return;
}




void MapArea( ROOM_INDEX_DATA * room, CHAR_DATA * ch, int x, int y, int min, int max, int line_of_sight )
{
   ROOM_INDEX_DATA *prospect_room;
   EXIT_DATA *pexit;
   CHAR_DATA *victim;
   int door, looper;
   short door_type = 0;
   if( map[x][y] <= 0 )
      return;  /* it's a door, not a room in the map */

   /*
    * marks the room as visited 
    */
   map[x][y] = room->sector_type;

/* displays seen mobs nearby as numbers */

   for( looper = 0, victim = room->first_person; victim; victim = victim->next_in_room )
      if( ( can_see( ch, victim ) ) && ( !is_same_group( ch, victim ) ) )
         looper++;
   if( looper > 0 )
      snprintf( contents[x][y].string, 10, "%i@@N", UMIN( looper, 9 ) );
   else
      contents[x][y].string[0] = '\0';

   for( door = 0; door < MAX_MAP_DIR; door++ )
   {  /* cycles through for each exit */
      if( ( ( pexit = room->exit[door] ) != NULL ) && ( pexit->to_room != NULL ) )

      {  /* if exit there */
         if( ( x < min ) || ( y < min ) || ( x > max ) || ( y > max ) )
            return;

         prospect_room = pexit->to_room;
         extern short rev_dir[];
         if( ( prospect_room->exit[rev_dir[door]] ) && ( prospect_room->exit[rev_dir[door]]->to_room != room ) )
         {  /* if not two way */
            map[x][y] = SECT_BLOCKED;  /* one way into area OR maze */
            return;
         }  /* end two way */

/* selects door symbol */

         door_type = 0;
         if( !pexit->exit_info.test(EX_ISDOOR) )
         {
            if( ( door == 0 ) || ( door == 2 ) )
               door_type = DOOR_NS;
            else
               door_type = DOOR_EW;
         }
         else if( pexit->exit_info.test(EX_LOCKED) )
         {
            door_type = DOOR_LOCKED;
         }
         else if( pexit->exit_info.test(EX_CLOSED) )
         {
            door_type = DOOR_CLOSED;
         }
         else
            door_type = DOOR_OPEN;
         if( ( !IS_NPC( ch ) )
             && ( !str_cmp( pexit->keyword, "" ) )
             && ( ( door_type <= DOOR_OPEN )
                  || ( !pexit->exit_info.test(EX_ISDOOR) )
                  || ( ( pexit->exit_info.test(EX_CLOSED) )
                       && ( !pexit->exit_info.test(EX_NODETECT) )
                       && ( ch->pcdata->learned[gsn_find_doors] > number_percent(  ) )
                       && ( !str_cmp( pexit->keyword, "" ) ) ) ) )
         {
            map[x + door_marks[door][0]][y + door_marks[door][1]] = door_type;
            if( ( door_type < DOOR_CLOSED )
                && ( ( line_of_sight == LOS_INITIAL )
                     || ( door == line_of_sight ) ) && ( map[x + offsets[door][0]][y + offsets[door][1]] == SECT_UNSEEN ) )
            {
               MapArea( pexit->to_room, ch,
                        x + offsets[door][0], y + offsets[door][1], min, max,
                        ( line_of_sight == LOS_INITIAL ) ? door : line_of_sight );
            }
         }
      }
/* end if exit there */
   }
   return;
}

void ShowRoom( CHAR_DATA * ch, int min, int max, int size, int center )
{
   int x, y, looper;
   char outbuf[MSL];
   char catbuf[MSL];
   char borderbuf[MSL];
   char colorbuf[MSL];
   char displaybuf[MSL];
   outbuf[0] = '\0';
   snprintf( outbuf, MSL, "%s", "\r\n" );
   snprintf( borderbuf, MSL, "%s", "@@y+@@W" );
   for( looper = 0; looper <= size + 1; looper++ )
   {
      snprintf( catbuf, MSL, "%s", "-" );
      strncat( borderbuf, catbuf, MSL-1 );
   }
   strncat( borderbuf, "@@y+@@N", MSL );
   for( x = min; x <= max; ++x )
   {  /* every row */

      strncat( outbuf, "@@W| ", MSL );
      for( y = min; y <= max; ++y )
      {  /* every column */
         if( ( y == min ) || ( map[x][y - 1] != map[x][y] ) )
         {
            snprintf( colorbuf, MSL, "%s%s",
                     ( ( map[x][y] <= 0 ) ?
                       get_door_color( map[x][y] ) :
                       get_sector_color( map[x][y] ) ),
                     ( ( contents[x][y].string[0] == '\0' ) ? "" : get_invert_color( map[x][y] ) ) );
            snprintf( displaybuf, MSL, "%s",
                     ( ( map[x][y] <= 0 ) ?
                       get_door_display( map[x][y] ) :
                       ( ( contents[x][y].string[0] == '\0' ) ?
                         get_sector_display( map[x][y] ) : contents[x][y].string ) ) );
            snprintf( catbuf, MSL, "%s%s", colorbuf, displaybuf );
            strncat( outbuf, catbuf, MSL-1 );

         }
         else
         {
            snprintf( catbuf, MSL, "%s", ( map[x][y] <= 0 ) ? get_door_display( map[x][y] ) : get_sector_display( map[x][y] ) );
            strncat( outbuf, catbuf, MSL-1 );
         }

      }
      strncat( outbuf, " @@W|@@N\r\n", MSL );
   }

   disp_map( borderbuf, outbuf, ch );

/* NOTE: with your imp, probably put the three together into one string? */

   return;
}




/* mlk :: shows a map, specified by size */


void ShowMap( CHAR_DATA * ch, int min, int max, int size, int center )
{
   int x, y, looper;
   char outbuf[MSL];
   char catbuf[MSL];
   char borderbuf[MSL];
   char colorbuf[MSL];
   char displaybuf[MSL];
   outbuf[0] = '\0';
   snprintf( outbuf, MSL, "%s", "\r\n" );
   snprintf( borderbuf, MSL, "%s", "@@y+@@W" );
   for( looper = 0; looper <= size + 1; looper++ )
   {
      snprintf( catbuf, MSL, "%s", "-" );
      strncat( borderbuf, catbuf, MSL-1 );
   }
   strncat( borderbuf, "@@y+@@N", MSL );
   for( x = min; x <= max; ++x )
   {  /* every row */

      strncat( outbuf, "@@W| ", MSL );
      for( y = min; y <= max; ++y )
      {  /* every column */
         if( ( y == min ) || ( map[x][y - 1] != map[x][y] ) )
         {
            snprintf( colorbuf, MSL, "%s%s",
                     ( ( map[x][y] <= 0 ) ?
                       get_door_color( map[x][y] ) :
                       get_sector_color( map[x][y] ) ),
                     ( ( contents[x][y].string[0] == '\0' ) ? "" : get_invert_color( map[x][y] ) ) );
            snprintf( displaybuf, MSL, "%s",
                     ( ( map[x][y] <= 0 ) ?
                       get_door_display( map[x][y] ) :
                       ( ( contents[x][y].string[0] == '\0' ) ?
                         get_sector_display( map[x][y] ) : contents[x][y].string ) ) );
            snprintf( catbuf, MSL, "%s%s", colorbuf, displaybuf );
            strncat( outbuf, catbuf, MSL-1 );

         }
         else
         {
            snprintf( catbuf, MSL, "%s", ( map[x][y] <= 0 ) ? get_door_display( map[x][y] ) : get_sector_display( map[x][y] ) );
            strncat( outbuf, catbuf, MSL-1 );
         }

      }
      strncat( outbuf, " @@W|@@N\r\n", MSL );
   }
   send_to_char( "\r\n", ch );
   /*
    * this is the top line of the map itself, currently not part of the mapstring
    */
   send_to_char( borderbuf, ch );
   /*
    * this is the contents of the map
    */
   send_to_char( outbuf, ch );
   /*
    * this is the bottom line of the map
    */
   send_to_char( borderbuf, ch );
   send_to_char( "\r\n", ch );
   send_to_char( "Also try map legend.\r\n", ch );

   return;
}

DO_FUN(do_mapper)
{
   int size, center, x, y, min, max, looper;
   char arg1[MSL];
   char catbuf[MSL];
   char outbuf[MSL];
   one_argument( argument, arg1 );
   if( is_name( arg1, "legend key help" ) )
   {
      snprintf( outbuf, MSL, "@@WMap Legend:@@N\r\n" );
      for( looper = 0; looper < SECT_TOP - 1; looper++ )
      {
         snprintf( catbuf, MSL, "%s%s : @@N%s\r\n",
                  map_info[looper].display_color, map_info[looper].display_code, map_info[looper].desc );
         strncat( outbuf, catbuf, MSL-1 );
      }
      for( looper = 0; looper < 5; looper++ )
      {
         snprintf( catbuf, MSL, "%s%s : @@N%s\r\n",
                  door_info[looper].display_color, door_info[looper].display_code, door_info[looper].desc );
         strncat( outbuf, catbuf, MSL-1 );
      }
      send_to_char( outbuf, ch );
      return;
   }
   size = ( is_number( arg1 ) ) ? atoi( arg1 ) : 11;

   if( size != 7 )
   {
      size = IS_IMMORTAL( ch ) ? size :
         get_curr_int( ch ) / 2 + ( ( !IS_NPC( ch ) ) ? ch->pcdata->learned[gsn_scout] / 25 : 0 );
      if( size % 2 == 0 )
         size += 1;
      size = URANGE( 9, size, MAX_MAP );
   }
   size = URANGE( 3, size, MAX_MAP );

   center = MAX_MAP / 2;

   min = MAX_MAP / 2 - size / 2;
   max = MAX_MAP / 2 + size / 2;

   for( x = 0; x < MAX_MAP; ++x )
      for( y = 0; y < MAX_MAP; ++y )
      {
         map[x][y] = SECT_UNSEEN;
         contents[x][y].string[0] = '\0';
      }

/* starts the mapping with the center room */
   MapArea( ch->in_room, ch, center, center, min - 1, max, LOS_INITIAL );

/* marks the center, where ch is */
   snprintf( contents[center][center].string, 10, "%s", "@@f*@@N" );
   if( size == 7 )
      ShowRoom( ch, min, max, size, center );
   else
      ShowMap( ch, min, max, size, center );
   return;

}
