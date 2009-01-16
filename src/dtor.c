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

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_HASH_H
#include "hash.h"
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

char_data::~char_data()
{
 is_free = true;

 while( first_carry != NULL )
  extract_obj(first_carry);

 while( first_affect != NULL )
  affect_remove(this,first_affect);

 free_string(name);
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

 if( current_brand )
  delete current_brand;
 if( npcdata )
  delete npcdata;
 if( pnote )
  delete pnote;
 if( pcdata )
  delete pcdata;
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

fight_data::~fight_data()
{
}

hash_entry_tp::~hash_entry_tp()
{
 is_free = true;
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
 free_string(short_descr);
 free_string(target);
}

note_data::~note_data()
{
 free_string(date);
 is_free = true;
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
 for( short i = 0; i < MAX_ALIASES; i++ )
 {
  free_string(alias[i]);
  free_string(alias_name[i]);
 }
 free_string(assist_msg);
 free_string(bamfin);
 free_string(bamfout);
 free_string(email_address);
 free_string(header);
 free_string(host);
 for( short i = 0; i < MAX_IGNORES; i++ )
  free_string(ignore_list[i]);
 is_free = true;
 free_string(lastlogin);
 free_string(load_msg);
 free_string(message);
 free_string(pedit_state);
 for( short i = 0; i < MAX_PEDIT; i++ )
  free_string(pedit_string[i]);
 free_string(pwd);
 delete quest_info;
 delete records;
 free_string(room_enter);
 free_string(room_exit);
 delete super;
 free_string(title);
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
 free_string(name);
 free_string(description);
}

shop_data::~shop_data()
{
 is_free = true;
}

super_data::~super_data()
{
}
