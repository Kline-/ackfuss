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
 branded = str_dup("");
 branded_by = str_dup("");
 dt_stamp = str_dup("");
 message = str_dup("");
 priority = str_dup("");
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
}

note_data::note_data()
{
 is_free = false;
 next = NULL;
 prev = NULL;
 sender = str_dup("");
 date = str_dup("");
 to_list = str_dup("");
 subject = str_dup("");
 text = str_dup("");
 date_stamp = 0;
}

pc_data::pc_data()
{
}

room_index_data::room_index_data()
{
 short i = 0;
 static MONEY_TYPE *room_treasure;

 affected_by = 0;
 area = NULL;
 description = &str_empty[0];
 for( i = 0; i < MAX_DIR; i++ )
  exit[i] = NULL;
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
