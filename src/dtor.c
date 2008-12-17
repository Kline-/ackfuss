/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "globals.h"

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

affect_data::~affect_data()
{
 is_free = true;
}

area_data::~area_data()
{
 is_free = true;
}

brand_data::~brand_data()
{
 free_string(branded);
 free_string(branded_by);
 free_string(dt_stamp);
 free_string(message);
 free_string(priority);
}

char_data::~char_data()
{
 int i = 0;
 MPROG_ACT_LIST *mpact;
 CHAR_DATA *rch;
 AFFECT_DATA *paf;
 ROOM_INDEX_DATA *room;
 ROOM_AFFECT_DATA *raf;

 is_free = true;

 while( first_carry != NULL )
  extract_obj(first_carry);

 while( first_affect != NULL )
  affect_remove(this,first_affect);

 while( (mpact = first_mpact) != NULL )
 {
  first_mpact = mpact->next;
  PUT_FREE(mpact,mpact_free);
 }

 for( rch = first_char; rch; rch = rch->next )
 {
  if( rch->master == this )
   rch->master = NULL;
  if( rch->leader == this )
   rch->leader = NULL;
  if( rch->fighting == this )
   rch->fighting = NULL;
  if( rch->reply == this )
   rch->reply = NULL;
  if( rch->hunting == this )
  {
   if( IS_NPC(rch) && !IS_NPC(this) )
   {
    free_string(rch->searching);
    rch->searching = str_dup(name);
   }
   else if( !IS_NPC(rch) )
    send_to_char("@@RYou seem to have lost your prey.\n\r",rch);
   end_hunt(rch);
  }
  if( rch->hunt_for == this )
   rch->hunt_for = NULL;
  if( rch->old_body == this )
  {
   do_return(rch,"");
   rch->old_body = NULL;
  }
  for( mpact = rch->first_mpact; mpact; mpact = mpact->next )
  {
   if( mpact->ch == this )
    mpact->ch = NULL;
   if( mpact->vo == this )
    mpact->vo = NULL;
  }
  for( paf = rch->first_affect; paf; paf = paf->next )
   if( paf->caster == this )
    paf->caster = NULL;
 }

 for( i = 0; i < MAX_KEY_HASH; i++ )
  for( room = room_index_hash[i]; room; room = room->next )
   for( raf = room->first_room_affect; raf; raf = raf->next )
    if( raf->caster == this )
     raf->caster = NULL;

 free_string(name);
 free_string(short_descr);
 free_string(long_descr);
 free_string(long_descr_orig);
 free_string(description);
 free_string(prompt);
 free_string(old_prompt);
 free_string(searching);
 free_string(target);

 #ifdef IMC
  imc_freechardata(this);
 #endif
 #ifdef I3
  free_i3chardata(this);
 #endif

 delete current_brand;
 delete pnote;
 delete pcdata;
}

exit_data::~exit_data()
{
 is_free = true;
 free_string(description);
 free_string(keyword);
}

mob_index_data::~mob_index_data()
{
 is_free = true;
 free_string(player_name);
 free_string(short_descr);
 free_string(description);
}

note_data::~note_data()
{
 is_free = true;
 free_string(text);
 free_string(subject);
 free_string(to_list);
 free_string(date);
 free_string(sender);
}

obj_data::~obj_data()
{
 is_free = true;
 free_string(name);
 free_string(owner);
 free_string(short_descr);
 free_string(long_descr);
}

obj_index_data::~obj_index_data()
{
 is_free = true;
 free_string(name);
 free_string(short_descr);
 free_string(long_descr);
}

pc_data::~pc_data()
{
 PUT_FREE(quest_info,quest_info_free);
 PUT_FREE(records,record_free);
 PUT_FREE(super,super_free);
 free_string(pwd);
 free_string(bamfin);
 free_string(room_enter);
 free_string(room_exit);
 free_string(bamfout);
 free_string(title);
 free_string(host);
 free_string(header);
 free_string(message);
 free_string(who_name);
 free_string(lastlogin);
 free_string(assist_msg);
 for( short i = 0; i < MAX_ALIASES; i++ )
 {
  free_string(alias_name[i]);
  free_string(alias[i]);
 }
 for( short i = 0; i < MAX_IGNORES; i++ )
  free_string(ignore_list[i]);
 free_string(load_msg);
 free_string(pedit_state);
 for( short i = 0; i < 5; i++ )
  free_string(pedit_string[i]);
 free_string(email_address);
 is_free = true;
}

reset_data::~reset_data()
{
 is_free = true;
 free_string(notes);
}

room_index_data::~room_index_data()
{
 free_string(name);
 free_string(description);
}

shop_data::~shop_data()
{
 is_free = true;
}
