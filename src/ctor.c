/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#include "h/globals.h"

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HASH_H
#include "h/hash.h"
#endif

#ifndef DEC_LUASCRIPT_H
#include "h/luascript.h"
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

 //affect_list.push_back(this); -- Added manually when allocing new perm affects on first boot. --Kline
}

area_data::area_data()
{
 age = 15;
 area_num = 0;
 can_read = &str_empty[0];
 can_write = &str_empty[0];
 filename = &str_empty[0];
 flags.reset();
 first_area_mobile = NULL;
 first_area_object = NULL;
 first_area_room = NULL;
 first_area_shop = NULL;
 first_reset = NULL;
 keyword = &str_empty[0];
 last_area_mobile = NULL;
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
 name = str_dup("New Area");
 owner = &str_empty[0];
 reset_msg = str_dup("The area resets. How mystical...");
 reset_rate = 15;

 area_list.push_back(this);
}

ban_data::ban_data()
{
 name = &str_empty[0];
 banned_by = &str_empty[0];
 newbie = false;

 ban_list.push_back(this);
}

board_data::board_data()
{
 vnum = 0;
 first_message = NULL;
 last_message = NULL;
 min_read_lev = 0;
 min_write_lev = 0;
 expiry_time = 0;
 clan = -1;

 board_list.push_back(this);
}

brand_data::brand_data()
{
 branded = &str_empty[0];
 branded_by = &str_empty[0];
 dt_stamp = &str_empty[0];
 message = &str_empty[0];
 priority = &str_empty[0];
}

buf_data_struct::buf_data_struct()
{
 is_free = false;
 next = NULL;
 prev = NULL;
 ch = NULL;
 dest = NULL;
 buf = &str_empty[0];
 returnfunc = NULL;
 returnparm = NULL;
 old_char_pos = 0;
}

build_data_list::build_data_list()
{
 is_free = false;
 next = NULL;
 prev = NULL;
 data = NULL;

 build_dat_list.push_back(this);
}

cast_data::cast_data()
{
 max_time = 0;
 pushback = 0;
 time = 0;
 arg = &str_empty[0];
}

char_data::char_data()
{
 act.reset();
 affected_by = 0;
 alignment = 0;
 armor = 100;
 carry_number = 0;
 carry_weight = 0;
 casting = new CAST_DATA;
 clan = -1;
 for( short i = 0; i < MAX_COOLDOWN; i++ )
  cooldown[i] = 0;
 damroll = 5;
 deaf.reset();
 desc = NULL;
 description.clear();
 exp = 0;
 fighting = NULL;
 first_affect = NULL;
 first_carry = NULL;
 first_saved_aff = NULL;
 first_shield = NULL;
 hit = 50;
 hitroll = 5;
 hunting = NULL;
 hunt_flags = 0;
 hunt_obj = NULL;
 in_room = NULL;
 is_free = false;
 is_quitting = false;
 lua = NULL;
 last_affect = NULL;
 last_carry = NULL;
 last_saved_aff = NULL;
 last_shield = NULL;
 leader = NULL;
 level = 0;
 logon = current_time;
 long_descr.clear();
 long_descr_orig.clear();
 for( short i = 0; i < MAX_CLASS; i++ )
  lvl[i] = -1;
 for( short i = 0; i < MAX_CLASS; i++ )
  lvl2[i] = -1;
 mana = 50;
 master = NULL;
 max_hit = 50;
 max_mana = 50;
 max_move = 50;
 money = new MONEY_TYPE;
 move = 50;
 name = &str_empty[0];
 next_in_room = NULL;
 npc = false;
 npcdata = NULL;
 num_followers = 0;
 old_body = NULL;
 old_prompt = &str_empty[0];
 pcdata = NULL;
 played = 0;
 poly_level = 0;
 position = POS_STANDING;
 prev_in_room = NULL;
 prompt = &str_empty[0];
 p_class = 0;
 race = 0;
 race_mods = 0;
 reply = NULL;
 rider = NULL;
 riding = NULL;
 saving_throw = 0;
 searching = &str_empty[0];
 sex = SEX_NEUTRAL;
 sitting = NULL;
 for( short i = 0; i < MAX_SPEED; i++ )
  speed[i] = 0;
 stance = STANCE_WARRIOR;
 stance_ac_mod = 0;
 stance_dr_mod = 0;
 stance_hr_mod = 0;
 stun_timer = 0;
 switched = false;
 timer = 0;
 trust = 0;
 using_named_door = false;
 wait = 0;
 was_in_room = NULL;
 wimpy = 0;
 wizbit = false;

 char_list.push_back(this);
}

descriptor_data::descriptor_data()
{
}

disabled_data::disabled_data()
{
 command = NULL;
 disabled_by = &str_empty[0];
 level = 0;

 disabled_list.push_back(this);
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

 exit_list.push_back(this);
}

extra_descr_data::extra_descr_data()
{
 is_free = false;
 description = &str_empty[0];
 keyword = &str_empty[0];
 next = NULL;
 prev = NULL;

 exdesc_list.push_back(this);
}

hash_entry_tp::hash_entry_tp()
{
 is_free = false;
 next = NULL;
 reference = NULL;
 key = 0;
}

lookup_data::lookup_data()
{
 buf = &str_empty[0];
 d = NULL;
}

lua_data::lua_data()
{
 L = luaL_newstate();
 loaded = false;
 owner = NULL;
 type = LUA_TYPE_NONE;

 lua_list.push_back(this);
}

magic_shield::magic_shield()
{
 is_free = false;
 next = NULL;
 prev = NULL;
 type = 0;
 harmful = false;
 attack_dam = 0;
 percent = 0;
 hits = 0;
 sn = 0;
 absorb_message_room = &str_empty[0];
 absorb_message_victim = &str_empty[0];
 absorb_message_self = &str_empty[0];
 name = &str_empty[0];
 wearoff_room = &str_empty[0];
 wearoff_self = &str_empty[0];
}

mark_data::mark_data()
{
 room_vnum = 0;
 message = &str_empty[0];
 author = &str_empty[0];
 duration = 0;
 type = 0;
}

message_data::message_data()
{
 is_free = false;
 next = NULL;
 prev = NULL;
 board = NULL;
 datetime = 0;
 author = &str_empty[0];
 title = &str_empty[0];
 message = &str_empty[0];
}

mob_index_data::mob_index_data()
{
 act.reset();
 ac_mod = 0;
 affected_by = 0;
 alignment = 0;
 area = NULL;
 cast = 0;
 clan = -1;
 def = 0;
 description = &str_empty[0];
 dr_mod = 0;
 hr_mod = 0;
 hunt_flags = 0;
 is_free = false;
 killed = 0;
 for( short i = 0; i < MAX_SKILL; i++ )
  learned[i] = 0;
 level = 1;
 long_descr = &str_empty[0];
 next = NULL;
 player_name = &str_empty[0];
 position = 0;
 pShop = NULL;
 p_class = 0;
 race = 0;
 race_mods = 0;
 resist = 0;
 script_name = &str_empty[0];
 sex = 0;
 short_descr = &str_empty[0];
 skills = 0;
 spec_fun = NULL;
 for( short i = 0; i < MAX_SPEED; i++ )
  speed[i] = 0;
 strong_magic = 0;
 suscept = 0;
 vnum = 0;
 weak_magic = 0;

 mob_index_list.push_back(this);
}

money_type::money_type()
{
 for( short i = 0; i < MAX_CURRENCY; i++ )
  cash_unit[i] = 0;
}

note_data::note_data()
{
 date = &str_empty[0];
 date_stamp = 0;
 sender = &str_empty[0];
 subject = &str_empty[0];
 text = &str_empty[0];
 to_list = &str_empty[0];

 note_list.push_back(this);
}

npc_data::npc_data()
{
 ac_mod = 0;
 cast = 0;
 def = 0;
 dr_mod = 0;
 extract_timer = -1;
 hr_mod = 0;
 hunt_for = NULL;
 hunt_home = NULL;
 ngroup = NULL;
 pIndexData = NULL;
 reset = NULL;
 resist = 0;
 short_descr = &str_empty[0];
 skills = 0;
 spec_fun = NULL;
 strong_magic = 0;
 suscept = 0;
 weak_magic = 0;
}

npc_group_data::npc_group_data()
{
 enemies = &str_empty[0];
 first_follower = NULL;
 is_free = false;
 last_fighting = NULL;
 last_follower = NULL;
 leader = NULL;
 needs = &str_empty[0];
 next = NULL;
 prev = NULL;
 seek_room = NULL;
 state = 0;
 wants = &str_empty[0];
}

obj_data::obj_data()
{
 is_free = false;
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
 script_name = &str_empty[0];
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
 money = new MONEY_TYPE;
 speed = 1.00;
 lua = NULL;

 obj_list.push_back(this);
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
 script_name = &str_empty[0];
 short_descr = &str_empty[0];
 speed = number_speed();
 for( short i = 0; i < MAX_OBJ_VALUE; i++ )
  value[i] = 0;
 vnum = 0;
 wear_flags.reset();
 weight = 1;

 obj_index_list.push_back(this);
}

pc_data::pc_data()
{
 adept_level = -1;
 for( short i = 0; i < MAX_ALIASES; i++ )
 {
  alias[i] = &str_empty[0];
  alias_name[i] = &str_empty[0];
 }
 assist_msg = str_dup("'@@eBANZAI!!@@N $N must be assisted!!'");;
 bamfin = &str_empty[0];
 bamfout = &str_empty[0];
 bank_money = new MONEY_TYPE;
 build_mode = 0;
 build_vnum = 0;
 for( short i = 0; i < MAX_COND; i++ )
  condition[i] = 0;
 condition[COND_THIRST] = 48;
 condition[COND_FULL] = 48;
 for( short i = 0; i < MAX_COLOR; i++ )
  color[i] = 0;
 current_brand = NULL;
 death_cnt = -1;
 dimcol = 'b';
 email_address = &str_empty[0];
 failures = 0;
 header = &str_empty[0];
 hicol = 'y';
 for( short i = 0; i < MAX_HOSTS; i++ )
  host[i] = str_dup("Unknown!");
 hp_from_gain = -1;
 for( short i = 0; i < MAX_IGNORES; i++ )
  ignore_list[i] = &str_empty[0];
#ifdef IMC
 imcchardata = NULL;
#endif
 invis = 0;
 lastlogin = str_dup("Unknown!");
 last_note = 0;
 for( short i = 0; i < MAX_SKILL; i++ )
  learned[i] = 0;
 load_msg = &str_empty[0];
 login_sex = SEX_NEUTRAL;
 mana_from_gain = -1;
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
 move_from_gain = -1;
 movement = 0;
 for( short i = 0; i < MAX_CLASS; i++ )
  order[i] = 0;
 pagelen = 20;
 pedit_state = &str_empty[0];
 for( short i = 0; i < MAX_PEDIT; i++ )
  pedit_string[i] = &str_empty[0];
 perm_con = 13;
 perm_dex = 13;
 perm_int = 13;
 perm_str = 13;
 perm_wis = 13;
 pnote = NULL;
 practice = 21;
 pwd = &str_empty[0];
 quest_info = new QUEST_INFO;
 quest_points = 0;
 recall_vnum = ROOM_VNUM_TEMPLE;
 records = new RECORD_DATA;
 room_enter = &str_empty[0];
 room_exit = &str_empty[0];
 ruler_rank = 0;
 save_time = 0;
 sentence = 0;
 super = new SUPER_DATA;
 term_columns = 80;
 term_rows = 25;
 title = &str_empty[0];
 valid_email = false;
 for( short i = 0; i < MAX_HOSTS; i++ )
  whitelist[i] = &str_empty[0];
 who_name = str_dup("off");
}

quest_info::quest_info()
{
 for( short i = 0; i < QUEST_MAX_DATA; i++ )
 {
  amount[i] = 0;
  quest_hint[i] = false;
  quest_item_vnum[i] = 0;
  quest_mob_vnum[i] = 0;
 }
 is_questing = false;
 quest_complete = false;
 for( short i = 0; i < QUEST_MAX_REWARD; i++ )
  quest_reward[i] = 0;
 quest_type = 0;
 time_left = 0;
 wait_time = 0;
}

record_data::record_data()
{
 crusade = 0;
 mdam_amt = 0;
 mdam_gsn = 0;
 pdam_amt = 0;
 pdam_gsn = 0;
 pd = 0;
 pk = 0;
 md = 0;
 mk = 0;
 mquest_c = 0;
 mquest_f = 0;
 qp = 0;
 qp_tot = 0;
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

 reset_list.push_back(this);
}

room_affect_data::room_affect_data()
{
 is_free = false;
 next = NULL;
 prev = NULL;
 duration = 0;
 level = 0;
 type = 0;
 bitvector = 0;
 applies_spell = 0;
 modifier = 0;
 location = 0;
 caster = NULL;
}

room_index_data::room_index_data()
{
 affected_by = 0;
 area = NULL;
 description = &str_empty[0];
 for( short i = 0; i < MAX_DIR; i++ )
  exit[i] = 0;
 first_content = NULL;
 first_exdesc = NULL;
 first_person = NULL;
 first_room_affect = NULL;
 first_room_reset = NULL;
 last_content = NULL;
 last_exdesc = NULL;
 last_person = NULL;
 last_room_affect = NULL;
 last_room_reset = NULL;
 light = 0;
 lua = NULL;
 mark_list.clear();
 name = str_dup("New Room");
 next = NULL;
 room_flags.reset();
 script_name = &str_empty[0];
 sector_type = SECT_INSIDE;
 treasure = new MONEY_TYPE;
 vnum = 0;

 room_index_list.push_back(this);
}

ruler_data::ruler_data()
{
 name = &str_empty[0];
 ruler_rank = 0;
 affiliation_name = &str_empty[0];
 affiliation_index  = 0;
 keywords = &str_empty[0];
 flags = 0;
}

shop_data::shop_data()
{
 for( short i = 0; i < MAX_TRADE; i++  )
  buy_type[i] = 0;
 close_hour = 0;
 keeper = 0;
 open_hour = 0;
 profit_buy = 1;
 profit_sell = 1;

 shop_list.push_back(this);
}

super_data::super_data()
{
 bloodline = 0;
 energy = 24;
 energy_max = 0;
 exp = 0;
 generation = 0;
 level = 0;
 skills_learned = 0;
 skills_max = 0;
 pracs = 0;
}
