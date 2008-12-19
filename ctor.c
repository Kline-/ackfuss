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

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

affect_data::affect_data()
{
 bitvector = 0;
 caster = NULL;
 duration = -1;
 is_free = false;
 level = 0;
 location = -1;
 modifier = 0;
 next = NULL;
 prev = NULL;
 type = -1;
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
 first_area_room = NULL;
 first_area_shop = NULL;
 first_reset = NULL;
 gold = 0;
 is_free = false;
 keyword = &str_empty[0];
 last_area_mobile = NULL;
 last_area_mobprog = NULL;
 last_area_object = NULL;
 last_area_room = NULL;
 last_area_shop = NULL;
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
 act.reset();
 act_build = 0;
 ac_mod = 0;
 adept_level = 0;
 affected_by = 0;
 alignment = 0;
 armor = 100;
 balance = 0;
 bank_money = NULL;
 build_vnum = 0;
 carry_number = 0;
 carry_weight = 0;
 cast = 0;
 clan = 0;
 current_brand = NULL;
 damroll = 5;
 def = 0;
 deaf.reset();
 death_cnt = 0;
 desc = NULL;
 description = &str_empty[0];
 dr_mod = 0;
 exp = 0;
 extract_timer = 0;
 fighting = NULL;
 first_affect = NULL;
 first_carry = NULL;
 first_mpact = NULL;
 first_saved_aff = NULL;
 first_shield = NULL;
 gold = 0;
 hit = 50;
 hitroll = 5;
 hr_mod = 0;
 hunting = NULL;
 hunt_flags = 0;
 hunt_for = NULL;
 hunt_home = NULL;
 hunt_obj = NULL;
 incog = 0;
 invis = 0;
 intell_exp = 0;
 in_room = NULL;
 is_free = false;
 is_quitting = false;
 last_affect = NULL;
 last_carry = NULL;
 last_mpact = NULL;
 last_note = 0;
 last_saved_aff = NULL;
 last_shield = NULL;
 leader = NULL;
 level = 0;
 login_sex = -1;
 logon = current_time;
 long_descr = &str_empty[0];
 long_descr_orig = &str_empty[0];
 for( short i = 0; i < MAX_CLASS; i++ )
  lvl[i] = 0;
 for( short i = 0; i < MAX_CLASS; i++ )
  lvl2[i] = 0;
 mana = 50;
 master = NULL;
 max_hit = 50;
 max_mana = 50;
 max_move = 50;
 money = NULL;
 move = 50;
 mpactnum = 0;
 name = &str_empty[0];
 next = NULL;
 next_in_room = NULL;
 ngroup = NULL;
 npc = false;
 npcdata = NULL;
 num_followers = 0;
 old_body = NULL;
 old_prompt = &str_empty[0];
 pcdata = NULL;
 pIndexData = NULL;
 played = 0;
 pnote = NULL;
 poly_level = 0;
 position = POS_STANDING;
 practice = 21;
 prev = NULL;
 prev_in_room = NULL;
 prompt = &str_empty[0];
 p_class = 0;
 race = 0;
 race_mods = 0;
 reply = NULL;
 rider = NULL;
 riding = NULL;
 save_time = 0;
 saving_throw = 0;
 searching = &str_empty[0];
 sentence = 0;
 sex = SEX_NEUTRAL;
 sitting = NULL;
 for( short i = 0; i < MAX_SPEED; i++ )
  speed[i] = 0;
 stance = 0;
 stance_ac_mod = 0;
 stance_dr_mod = 0;
 stance_hr_mod = 0;
 stun_timer = 0;
 switched = false;
 target = &str_empty[0];
 timer = 0;
 trust = 0;
 using_named_door = false;
 wait = 0;
 was_in_room = NULL;
 wimpy = 0;
 wizbit = false;
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
 date = &str_empty[0];
 date_stamp = 0;
 is_free = false;
 next = NULL;
 prev = NULL;
 sender = &str_empty[0];
 subject = &str_empty[0];
 text = &str_empty[0];
 to_list = &str_empty[0];
}

npc_data::npc_data()
{
 reset = NULL;
 resist = 0;
 short_descr = &str_empty[0];
 skills = 0;
 spec_fun = NULL;
 strong_magic = 0;
 suscept = 0;
 weak_magic = 0;
}

obj_data::obj_data()
{
 is_free = false;
 next = NULL;
 prev = NULL;
 extra_flags.reset();
 wear_flags.reset();
 reset = NULL;
 next_in_carry_list = NULL;
 prev_in_carry_list = NULL;
 first_in_carry_list = NULL;
 last_in_carry_list = NULL;
 next_in_room = NULL;
 prev_in_room = NULL;
 first_in_room = NULL;
 last_in_room = NULL;
 first_content = NULL;
 last_content = NULL;
 next_content = NULL;
 prev_content = NULL;
 in_obj = NULL;
 obj_fun = NULL;
 carried_by = NULL;
 first_exdesc = NULL;
 last_exdesc = NULL;
 first_apply = NULL;
 last_apply = NULL;
 pIndexData = NULL;
 in_room = NULL;
 owner = &str_empty[0];
 name = &str_empty[0];
 short_descr = &str_empty[0];
 long_descr = &str_empty[0];
 item_type = 0;
 item_apply = 0;
 wear_loc = 0;
 weight = 0;
 cost = 0;
 level = 0;
 timer = 0;
 for( short i = 0; i < MAX_OBJ_VALUE; i++ )
  value[i] = 0;
 durability = 0;
 max_durability = 0;
 money = NULL;
 speed = 1.00;
}

obj_index_data::obj_index_data()
{
 area = NULL;
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
 for( short i = 0; i < MAX_ALIASES; i++ )
 {
  alias[i] = &str_empty[0];
  alias_name[i] = &str_empty[0];
 }
 assist_msg = &str_empty[0];
 bamfin = &str_empty[0];
 bamfout = &str_empty[0];
 clan = 0;
 for( short i = 0; i < MAX_COND; i++ )
  condition[i] = 0;
 for( short i = 0; i < MAX_COLOR; i++ )
  color[i] = 0;
 dimcol = '\0';
 email_address = &str_empty[0];
 failures = 0;
 header = &str_empty[0];
 hicol = '\0';
 host = str_dup("Unknown!");
 hp_from_gain = 0;
 for( short i = 0; i < MAX_IGNORES; i++ )
  ignore_list[i] = &str_empty[0];
#ifdef IMC
 imcchardata = NULL;
#endif
 is_free = false;
 lastlogin = str_dup("Unknown!");
 for( short i = 0; i < MAX_SKILL; i++ )
  learned[i] = 0;
 load_msg = &str_empty[0];
 mana_from_gain = 0;
 max_con = 0;
 max_dex = 0;
 max_int = 0;
 max_str = 0;
 max_wis = 0;
 message = &str_empty[0];
 mod_con = 0;
 mod_dex = 0;
 mod_int = 0;
 mod_str = 0;
 mod_wis = 0;
 monitor.reset();
 move_from_gain = 0;
 movement = 0;
 next = NULL;
 for( short i = 0; i < MAX_CLASS; i++ )
  order[i] = 0;
 pagelen = 0;
 pedit_state = &str_empty[0];
 for( short i = 0; i < MAX_PEDIT; i++ )
  pedit_string[i] = &str_empty[0];
 perm_con = 0;
 perm_dex = 0;
 perm_int = 0;
 perm_str = 0;
 perm_wis = 0;
 prev = NULL;
 pwd = &str_empty[0];
 quest_info = NULL;
 quest_points = 0;
 recall_vnum = 0;
 records = NULL;
 room_enter = &str_empty[0];
 room_exit = &str_empty[0];
 ruler_rank = 0;
 super = NULL;
 term_columns = 0;
 term_rows = 0;
 title = &str_empty[0];
 valid_email = false;
 who_name = str_dup("off");;
}

reset_data::reset_data()
{
 is_free = false;
 next = NULL;
 prev = NULL;
 command = '\0';
 arg1 = 0;
 arg2 = 0;
 arg3 = 0;
 notes = &str_empty[0];
 count = 0;
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
 for( short i = 0; i < MAX_TRADE; i++  )
  buy_type[i] = 0;
 close_hour = 0;
 is_free = false;
 keeper = 0;
 next = NULL;
 open_hour = 0;
 prev = NULL;
 profit_buy = 1;
 profit_sell = 1;
}
