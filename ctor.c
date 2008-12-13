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

#ifndef DEC_MONEY_H
#include "money.h"
#endif

affect_data::affect_data()
{
 is_free = false;
 next = NULL;
 prev = NULL;
 type = -1;
 duration = -1;
 location = -1;
 modifier = 0;
 bitvector = 0;
 caster = NULL;
 level = 0;
}

area_data::area_data()
{
 age = 15;
 area_num = 0;
 can_read = &str_empty[0];
 can_write = &str_empty[0];
 control = NULL;
 filename = &str_empty[0];
 flags.reset();
 first_area_mobile = NULL;
 first_area_mobprog = NULL;
 first_area_object = NULL;
 first_area_objfunc = NULL;
 first_area_room = NULL;
 first_area_shop = NULL;
 first_area_specfunc = NULL;
 first_reset = NULL;
 gold = 0;
 is_free = false;
 keyword = &str_empty[0];
 last_area_mobile = NULL;
 last_area_mobprog = NULL;
 last_area_object = NULL;
 last_area_objfunc = NULL;
 last_area_room = NULL;
 last_area_shop = NULL;
 last_area_specfunc = NULL;
 last_reset = NULL;
 level_label = str_dup("[ None ]");
 max_level = 1;
 max_vnum = 0;
 modified = false;
 min_level = 1;
 min_vnum = 0;
 name = str_dup("New Area");;
 next = NULL;
 nplayer = 0;
 owner = &str_empty[0];
 prev = NULL;
 reset_msg = str_dup("The area resets. How mystical...");
 reset_rate = 15;
}

brand_data::brand_data()
{
 branded = &str_empty[0];
 branded_by = &str_empty[0];
 dt_stamp = &str_empty[0];
 message = &str_empty[0];
 priority = &str_empty[0];
}

char_data::char_data()
{
 is_free = false;
 act.reset();
 deaf.reset();
}

exit_data::exit_data()
{
 description = &str_empty[0];
 exit_info.reset();
 is_free = false;
 key = 0;
 keyword = &str_empty[0];
 next = NULL;
 to_room = NULL;
 vnum = 0;
}

mob_index_data::mob_index_data()
{
 act.reset();
 ac_mod = 0;
 affected_by = 0;
 alignment = 0;
 area = NULL;
 cast = 0;
 clan = 0;
 count = 0;
 def = 0;
 description = &str_empty[0];
 dr_mod = 0;
 hr_mod = 0;
 first_mprog = NULL;
 hunt_flags = 0;
 is_free = false;
 killed = 0;
 last_mprog = NULL;
 for( short i = 0; i < MAX_SKILL; i++ )
  learned[i] = 0;
 level = 1;
 long_descr = &str_empty[0];
 next = NULL;
 player_name = &str_empty[0];
 position = 0;
 power_cast = 0;
 power_skills = 0;
 progtypes = 0;
 pShop = NULL;
 p_class = 0;
 race = 0;
 race_mods = 0;
 resist = 0;
 sex = 0;
 short_descr = &str_empty[0];
 skills = 0;
 spec_fun = NULL;
 for( short i = 0; i < MAX_SPEED; i++ )
  speed[i] = 0;
 strong_magic = 0;
 suscept = 0;
 target = &str_empty[0];
 vnum = 0;
 weak_magic = 0;
}

note_data::note_data()
{
 is_free = false;
 next = NULL;
 prev = NULL;
 sender = &str_empty[0];
 date = &str_empty[0];
 to_list = &str_empty[0];
 subject = &str_empty[0];
 text = &str_empty[0];
 date_stamp = 0;
}

obj_data::obj_data()
{
 is_free = false;
 extra_flags.reset();
 wear_flags.reset();
}

obj_index_data::obj_index_data()
{
 area = NULL;
 count = 0;
 durability = max_durability;
 extra_flags.reset();
 first_apply = NULL;
 first_exdesc = NULL;
 first_trigger = NULL;
 is_free = false;
 item_apply = 1;
 item_type = 1;
 last_apply = NULL;
 last_exdesc = NULL;
 last_trigger = NULL;
 level = 1;
 long_descr = &str_empty[0];
 max_durability = number_range(2,100);
 name = &str_empty[0];
 next = NULL;
 obj_fun = NULL;
 owner = &str_empty[0];
 short_descr = &str_empty[0];
 speed = number_speed();
 for( short i = 0; i < MAX_OBJ_VALUE; i++ )
  value[i] = 0;
 vnum = 0;
 wear_flags.reset();
 weight = 1;
}

pc_data::pc_data()
{
 is_free = false;
}

room_index_data::room_index_data()
{
 static MONEY_TYPE *room_treasure;

 affected_by = 0;
 area = NULL;
 description = &str_empty[0];
 for( short i = 0; i < MAX_DIR; i++ )
  exit[i] = 0;
 first_content = NULL;
 first_exdesc = NULL;
 first_mark_list = NULL;
 first_person = NULL;
 first_room_affect = NULL;
 first_room_reset = NULL;
 is_free = false;
 last_content = NULL;
 last_exdesc = NULL;
 last_mark_list = NULL;
 last_person = NULL;
 last_room_affect = NULL;
 last_room_reset = NULL;
 light = 0;
 name = str_dup("New Room");
 next = NULL;
 room_flags.reset();
 sector_type = SECT_INSIDE;
 GET_FREE( room_treasure, money_type_free );
#ifdef DEBUG_MONEY
 {
  char testbuf[MSL];
  snprintf( testbuf, MSL,  "loading rooms, vnum %d", pRoomIndex->vnum );
  room_treasure->money_key = str_dup( testbuf );
 }
#endif
 treasure = room_treasure;
 vnum = 0;
}

shop_data::shop_data()
{
 is_free = false;
 next = NULL;
 prev = NULL;
 keeper = 0;
 for( short i = 0; i < MAX_TRADE; i++  )
  buy_type[i] = 0;
 profit_buy = 1;
 profit_sell = 1;
 open_hour = 0;
 close_hour = 0;
}
