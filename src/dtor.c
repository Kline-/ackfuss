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

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_HASH_H
#include "h/hash.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

affect_data::~affect_data()
{
 is_free = true;
}

area_data::~area_data()
{
 free_string(can_read);
 free_string(can_write);
 free_string(filename);
 free_string(keyword);
 free_string(level_label);
 free_string(name);
 free_string(owner);
 free_string(reset_msg);
}

ban_data::~ban_data()
{
 free_string(name);
 free_string(banned_by);
}

board_data::~board_data()
{
 MESSAGE_DATA *p, *p_next;

 for( p = first_message; p != NULL; p = p_next )
 {
  p_next = p->next;
  delete p;
 }
}

brand_data::~brand_data()
{
 free_string(branded);
 free_string(branded_by);
 free_string(dt_stamp);
 free_string(message);
 free_string(priority);
}

buf_data_struct::~buf_data_struct()
{
 free_string(buf);
}

build_data_list::~build_data_list()
{
 is_free = true;
}

cast_data::~cast_data()
{
 free_string(arg);
}

char_data::~char_data()
{
 is_free = true;

 while( first_carry != NULL )
  extract_obj(first_carry);

 while( first_affect != NULL )
  affect_remove(this,first_affect);

 #ifdef IMC
  imc_freechardata(this);
 #endif
 #ifdef I3
  free_i3chardata(this);
 #endif

 if( lua )
  delete lua;
 if( npcdata )
  delete npcdata;
 if( pcdata )
  delete pcdata;
 delete money;
 delete casting;

 aggro_list.remove(this);
}

descriptor_data::~descriptor_data()
{
 free(outbuf);
}

disabled_data::~disabled_data()
{
 free_string(disabled_by);
}

exit_data::~exit_data()
{
 free_string(description);
 is_free = true;
 free_string(keyword);
}

extra_descr_data::~extra_descr_data()
{
 free_string(description);
 is_free = true;
 free_string(keyword);
}

hash_entry_tp::~hash_entry_tp()
{
 is_free = true;
}

lookup_data::~lookup_data()
{
 free_string(buf);
}

lua_data::~lua_data()
{
 if( L )
  lua_close(L);
 owner = NULL;

 lua_list.remove(this);
}

magic_shield::~magic_shield()
{
 is_free = true;
 free_string(absorb_message_room);
 free_string(absorb_message_victim);
 free_string(absorb_message_self);
 free_string(name);
 free_string(wearoff_room);
 free_string(wearoff_self);
}

mark_data::~mark_data()
{
 free_string(message);
 free_string(author);
}

message_data::~message_data()
{
 free_string(author);
 free_string(title);
 free_string(message);
}

mob_index_data::~mob_index_data()
{
 free_string(description);
 is_free = true;
 free_string(long_descr);
 free_string(player_name);
 free_string(script_name);
 free_string(short_descr);
}

money_type::~money_type()
{
}

note_data::~note_data()
{
 free_string(date);
 free_string(sender);
 free_string(subject);
 free_string(text);
 free_string(to_list);
}

npc_data::~npc_data()
{
 free_string(short_descr);
}

npc_group_data::~npc_group_data()
{
 free_string(enemies);
 is_free = true;
 free_string(needs);
 free_string(wants);
}

obj_data::~obj_data()
{
 AFFECT_DATA *a, *a_next;

 is_free = true;

 for( a = first_apply; a != NULL; a = a_next )
 {
  a_next = a->next;
  delete a;
 }

 free_string(name);
 free_string(owner);
 free_string(script_name);
 free_string(short_descr);
 free_string(long_descr);
 delete money;
 if( lua )
  delete lua;
}

obj_index_data::~obj_index_data()
{
 is_free = true;
 free_string(name);
 free_string(script_name);
 free_string(short_descr);
 free_string(long_descr);
}

pc_data::~pc_data()
{
 for( short i = 0; i < MAX_ALIASES; i++ )
 {
  free_string(alias[i]);
  free_string(alias_name[i]);
 }
 free_string(assist_msg);
 free_string(bamfin);
 free_string(bamfout);
 delete bank_money;
 if( current_brand )
  delete current_brand;
 free_string(email_address);
 free_string(header);
 for( short i = 0; i < MAX_HOSTS; i++ )
  free_string(host[i]);
 for( short i = 0; i < MAX_IGNORES; i++ )
  free_string(ignore_list[i]);
 free_string(lastlogin);
 free_string(load_msg);
 free_string(message);
 free_string(pedit_state);
 for( short i = 0; i < MAX_PEDIT; i++ )
  free_string(pedit_string[i]);
 if( pnote )
  delete pnote;
 free_string(pwd);
 delete quest_info;
 delete records;
 free_string(room_enter);
 free_string(room_exit);
 delete super;
 free_string(title);
 for( short i = 0; i < MAX_HOSTS; i++ )
  free_string(whitelist[i]);
 free_string(who_name);
}

quest_info::~quest_info()
{
}

record_data::~record_data()
{
}

reset_data::~reset_data()
{
 is_free = true;
 free_string(notes);
}

room_affect_data::~room_affect_data()
{
}

room_index_data::~room_index_data()
{
 ROOM_AFFECT_DATA *raf, *raf_next;

 for( raf = this->first_room_affect; raf != NULL; raf = raf_next )
 {
  raf_next = raf->next;
  r_affect_remove(this,raf);
 }

 for_each( mark_list.begin(), mark_list.end(), DeleteObject() );
 free_string(name);
 free_string(description);
 free_string(script_name);
 delete treasure;
 if( lua )
  delete lua;
}

ruler_data::~ruler_data()
{
 free_string(name);
 free_string(affiliation_name);
 free_string(keywords);
}

shop_data::~shop_data()
{
}

super_data::~super_data()
{
}
