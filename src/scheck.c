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

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

/*  String checker, Spectrum 11/96
 *
 *  Basic idea is to walk through all the strings we know about, and mark them
 *  as referenced. Then we check for strings that have a reference count thats
 *  different from ptr->usage and log them
 */

/*
 * Things which are walked (anything else must be touched from these):
 *
 * o char_list
 * o descriptor_list
 * o object_list
 * o mob_index hash table
 * o obj_index hash table
 * o room_index hash table
 * o socials table
 * o helps
 * o areas
 * o notes/ideas/etc
 *
 */

/* Main code */

static void touch( char *str )
{
   BufEntry *p;

   if( !str )
      return;

   if( str < string_space || str > top_string )
      return;  /* not in string space */

   p = ( BufEntry * ) ( str - HEADER_SIZE );
   p->ref++;
}

static void clear( void )
{
   /*
    * Set all reference counts to 0
    */

   BufEntry *p;

   for( p = ssm_buf_head; p; p = p->next )
      p->ref = 0;
}

static BufEntry *dump_ptr[2];

static long dump( void )
{
   /*
    * Dump strings that have ref!=usage
    */

   FILE *dumpf;
   BufEntry *p;
   long count = 0;

   dumpf = file_open( "../report/leaks.dmp", "w" );

   for( p = ssm_buf_head; p; p = p->next )
   {
      if( p->usage > 0 && p->usage != p->ref )
      {
         /*
          * things to ignore:
          * * the common DEFAULT_PROMPT prompt string
          * * a '$' (from socials)
          */

         if( !str_cmp( p->buf, DEFAULT_PROMPT ) || !str_cmp( p->buf, "$" ) )
            continue;

         fprintf( dumpf, "usage %2d/%2d, caller %s, string %s\n", p->ref, p->usage, p->caller, p->buf );
         count += abs( p->usage - p->ref );
      }
      dump_ptr[0] = dump_ptr[1];
      dump_ptr[1] = p;
   }

   file_close( dumpf );

   return count;
}

static void walk_mob_index_data( MOB_INDEX_DATA * m )
{
   if( !m )
      return;

   touch( m->player_name );
   touch( m->short_descr );
   touch( m->long_descr );
   touch( m->description );

}

static void walk_ngroup_data( NPC_GROUP_DATA * ngrp )
{
   if( !ngrp )
      return;

   touch( ngrp->enemies );
   touch( ngrp->last_fighting );
   touch( ngrp->wants );
   touch( ngrp->needs );
}

static void walk_ngroups( void )
{
   NPC_GROUP_DATA *ngroup;

   for( ngroup = first_npc_group; ngroup; ngroup = ngroup->next )
   {
      walk_ngroup_data( ngroup );
   }
}

static void walk_note_data( NOTE_DATA * note )
{
   if( !note )
      return;

   touch( note->sender );
   touch( note->date );
   touch( note->to_list );
   touch( note->subject );
   touch( note->text );
}

static void walk_npcdata( NPC_DATA * n )
{
   if( !n )
     return;

   touch( n->short_descr );
}

static void walk_pcdata( PC_DATA * p )
{
   int i;

   if( !p )
      return;

   touch( p->pwd );
   touch( p->bamfin );
   touch( p->bamfout );
   touch( p->title );
   touch( p->room_enter );
   touch( p->room_exit );
   touch( p->who_name );
   touch( p->header );
   touch( p->message );
   touch( p->lastlogin );
   touch( p->load_msg );

   for( i = 0; i < MAX_HOSTS; i++ )
      touch( p->host[i] );

   for( i = 0; i < MAX_IGNORES; i++ )
      touch( p->ignore_list[i] );

   for( i = 0; i < MAX_ALIASES; i++ )
   {
      touch( p->alias[i] );
      touch( p->alias_name[i] );
   }
   for( i = 0; i < MAX_PEDIT; i++ )
   {
      touch( p->pedit_string[i] );
   }
   touch( p->pedit_state );
   touch( p->email_address );
   touch( p->assist_msg );
}
static void walk_shield_data( MAGIC_SHIELD * shield )
{
   if( !shield )
      return;

   touch( shield->name );
   touch( shield->absorb_message_room );
   touch( shield->absorb_message_victim );
   touch( shield->absorb_message_self );
   touch( shield->wearoff_room );
   touch( shield->wearoff_self );

}

static void walk_brand_data( BRAND_DATA * brand )
{
   if( !brand )
      return;

   touch( brand->branded );
   touch( brand->branded_by );
   touch( brand->dt_stamp );
   touch( brand->message );
   touch( brand->priority );
}
static void walk_brands( void )
{
   BRAND_DATA *this_brand;
   DL_LIST *brands;
   for( brands = first_brand; brands; brands = brands->next )
   {
      this_brand = (BRAND_DATA *)brands->this_one;
      walk_brand_data( this_brand );
   }
}

static void walk_shieldlist( MAGIC_SHIELD * shield )
{
   for( ; shield; shield = shield->next )
      walk_shield_data( shield );
}

static void walk_char_data( CHAR_DATA * ch )
{
   if( !ch )
      return;

   walk_npcdata( ch->npcdata );
   walk_pcdata( ch->pcdata );
   walk_shieldlist( ch->first_shield );
}

static void walk_extra_descr_data( EXTRA_DESCR_DATA * ed )
{
   if( !ed )
      return;

   touch( ed->keyword );
   touch( ed->description );
}

static void walk_obj_index_data( OBJ_INDEX_DATA * o )
{
   EXTRA_DESCR_DATA *ed;

   if( !o )
      return;

   for( ed = o->first_exdesc; ed; ed = ed->next )
      walk_extra_descr_data( ed );

   touch( o->name );
   touch( o->short_descr );
   touch( o->long_descr );
   touch( o->owner );
}


static void walk_obj_data( OBJ_DATA * o )
{
   EXTRA_DESCR_DATA *ed;

   if( !o )
      return;

   for( ed = o->first_exdesc; ed; ed = ed->next )
      walk_extra_descr_data( ed );

   touch( o->owner );
   touch( o->name );
   touch( o->short_descr );
   touch( o->long_descr );
}

static void walk_exit_data( EXIT_DATA * e )
{
   if( !e )
      return;

   touch( e->keyword );
   touch( e->description );
}

static void walk_reset_data( RESET_DATA * r )
{
   if( !r )
      return;

   touch( r->notes );
}

static void walk_area_data( AREA_DATA * ad )
{

   RESET_DATA *reset;
   if( !ad )
      return;

   touch( ad->filename );
   touch( ad->name );
   touch( ad->owner );
   touch( ad->can_read );
   touch( ad->can_write );
   touch( ad->keyword );   /* spec- missed strings */
   touch( ad->level_label );  /* spec - missed strings */
   touch( ad->reset_msg );
   for( reset = ad->first_reset; reset; reset = reset->next )
      walk_reset_data( reset );

}

/* spec - for the new rulers stuff */
static void walk_ruler_data( RULER_DATA * ruler )
{
   if( !ruler )
      return;
   touch( ruler->name );
   touch( ruler->affiliation_name );
   touch( ruler->keywords );
}

static void walk_mark_data( MARK_DATA * m )
{
   if( !m )
      return;

   touch( m->message );
   touch( m->author );
}

static void walk_room_index_data( ROOM_INDEX_DATA * r )
{
   int i;
   EXTRA_DESCR_DATA *ed;
   list<MARK_DATA *>::iterator li;
/*  BUILD_DATA_LIST *reset;  */
   if( !r )
      return;

   for( i = 0; i < 6; i++ )
      walk_exit_data( r->exit[i] );
   for( ed = r->first_exdesc; ed; ed = ed->next )
      walk_extra_descr_data( ed );
   for( li = r->mark_list.begin(); li != r->mark_list.end(); li++ )
      walk_mark_data( *li );
   touch( r->name );
   touch( r->description );
}


static void walk_social_type( SOCIAL_TYPE *s )
{


   if( !s )
      return;
   touch( s->name );
   touch( s->char_no_arg );
   touch( s->others_no_arg );
   touch( s->char_found );
   touch( s->others_found );
   touch( s->vict_found );
   touch( s->char_auto );
   touch( s->others_auto );
}

static void walk_descriptor_data( DESCRIPTOR_DATA * d )
{
   if( !d )
      return;

   touch( d->host );
}



static void walk_ban_data( BAN_DATA * b )
{
   touch( b->name );
   touch( b->banned_by );
}



static void walk_socials( void )
{
   extern int maxSocial;
   int i;

   for( i = 0; i < maxSocial; i++ )
      walk_social_type( &social_table[i] );
}

static void walk_chars( void )
{
   CHAR_DATA *ch;
   list<CHAR_DATA *>::iterator li;

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      ch = *li;
      walk_char_data( ch );
   }
}

static void walk_descriptors( void )
{
   DESCRIPTOR_DATA *d;

   for( d = first_desc; d; d = d->next )
      walk_descriptor_data( d );
}

static void walk_objects( void )
{
   OBJ_DATA *o;
   list<OBJ_DATA *>::iterator li;

   for( li = obj_list.begin(); li != obj_list.end(); li++ )
   {
      o = *li;
      walk_obj_data( o );
   }
}

static void walk_areas( void )
{
   AREA_DATA *ad;
   list<AREA_DATA *>::iterator li;

   for( li = area_list.begin(); li != area_list.end(); li++ )
   {
      ad = *li;
      walk_area_data( ad );
   }
}

/* spec- more rulers stuff */
static void walk_rulers( void )
{
   RULER_DATA *r;
   list<RULER_DATA *>::iterator li;

   for( li = ruler_list.begin(); li != ruler_list.end(); li++ )
   {
      r = *li;
      walk_ruler_data( r );
   }
}

static void walk_mob_indexes( void )
{
   MOB_INDEX_DATA *m;
   int i;

   for( i = 0; i < MAX_KEY_HASH; i++ )
      for( m = mob_index_hash[i]; m; m = m->next )
         walk_mob_index_data( m );
}

static void walk_obj_indexes( void )
{
   OBJ_INDEX_DATA *o;
   int i;

   for( i = 0; i < MAX_KEY_HASH; i++ )
      for( o = obj_index_hash[i]; o; o = o->next )
         walk_obj_index_data( o );
}

static void walk_room_indexes( void )
{
   ROOM_INDEX_DATA *r;
   int i;

   for( i = 0; i < MAX_KEY_HASH; i++ )
      for( r = room_index_hash[i]; r; r = r->next )
         walk_room_index_data( r );
}


static void walk_notes( void )
{
 NOTE_DATA *n;
 list<NOTE_DATA *>::iterator li;

 for( li = note_list.begin(); li != note_list.end(); li++ )
 {
   n = *li;
   walk_note_data( n );
 }
}

static void walk_bans( void )
{
   BAN_DATA *b;
   list<BAN_DATA *>::iterator li;

   for( li = ban_list.begin(); li != ban_list.end(); li++ )
   {
      b = *li;
      walk_ban_data( b );
   }
}

static void walk_message_data( MESSAGE_DATA * m )
{
   if( !m )
      return;

   touch( m->message );
   touch( m->author );
   touch( m->title );
}

void walk_messages( MESSAGE_DATA * m )
{
   for( ; m; m = m->next )
      walk_message_data( m );
}
void walk_boards( void )
{
   BOARD_DATA *board;
   list<BOARD_DATA *>::iterator li;

   for( li = board_list.begin(); li != board_list.end(); li++ )
   {
      board = *li;
      walk_messages( board->first_message );
   }
}


void walk_councils( void )
{
   short index;
   extern COUNCIL_DATA super_councils[MAX_SUPER];

   for( index = 0; index < MAX_SUPER; index++ )
      touch( super_councils[index].council_name );
}
void walk_sysdata( void )
{
   touch( sysdata.playtesters );
}


DO_FUN(do_scheck)
{
   char buf[MAX_STRING_LENGTH];
   extern bool disable_timer_abort;
   disable_timer_abort = TRUE;
   clear(  );

   walk_socials(  );
   walk_chars(  );
   walk_descriptors(  );
   walk_objects(  );
   walk_areas(  );
   walk_bans(  );
   walk_mob_indexes(  );
   walk_obj_indexes(  );
   walk_room_indexes(  );
   walk_notes(  );
   walk_councils(  );
   walk_boards(  );
   walk_rulers(  );
   walk_brands(  );
   walk_sysdata(  );
   walk_ngroups(  );

   snprintf( buf, MSL, "%ld leaks dumped to leaks.dmp\r\n", dump(  ) );
   send_to_char( buf, ch );
   disable_timer_abort = FALSE;
}
