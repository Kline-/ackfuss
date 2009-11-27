/*
 * Copyright Matt Goff (Kline) 2009
 * If you use my code, please give credit where it is due.
 * Support provided at www.ackmud.net
 */

#include "h/areaconvert.h"
#include "h/extern.h"

void write_ackfuss( ofstream &file, int type )
{
 write_ackfuss_area(file,type);
 write_ackfuss_room(file,type);
 write_ackfuss_npc(file,type);
 write_ackfuss_obj(file,type);
 write_ackfuss_shop(file,type);
 write_ackfuss_reset(file,type);

 file << "#$" << endl; /* the end */

 return;
}

void write_ackfuss_area( ofstream &file, int type )
{
 char delim = '~';
 int i = 0;

 file << "#AREA" << endl;
 file << "Revision  " << area.revision << endl;
 file << "CanRead  " << area.can_read << delim << endl;
 file << "CanWrite " << area.can_write << delim << endl;
 file << "Flags     ";
 for( i = 0; i < MAX_BITSET; i++ )
  if( area.bitset_flags_out.test(i) )
   file << i << " ";
 file << "EOL" << endl;
 file << "Keyword  " << area.keyword << delim << endl;
 file << "LevLabel " << area.level_label << delim << endl;
 file << "LevRange  " << area.min_level << " " << area.max_level << endl;
 file << "Name      " << area.name << delim << endl;
 file << "Owner    " << area.owner << delim << endl;
 file << "ResetMsg " << area.reset_msg << delim << endl;
 file << "ResetRate " << area.reset_rate << endl;
 file << "VnumRange " << area.min_vnum << " " << area.max_vnum << endl;
 file << "End" << endl;

 return;
}

void write_ackfuss_room( ofstream &file, int type )
{
 char delim = '~';
 list<room_data *>::iterator rt;
 list<extra_data *>::iterator et;
 room_data *room;
 extra_data *extra;
 int i = 0, x = 0;

 for( rt = room_list.begin(); rt != room_list.end(); rt++ )
 {
  room = *rt;
  file << "#ROOM" << endl;
  file << "Vnum    " << room->vnum << endl;
  file << "Desc    " << room->description << delim << endl;
  file << "Flags   ";
  for( i = 0; i < MAX_BITSET; i++ )
   if( room->bitset_flags_out.test(i) )
    file << i << " ";
  file << "EOL" << endl;
  file << "Name    " << room->name << delim << endl;
  file << "Sect    " << room->sector << endl;
  file << "End" << endl;

  for( i = 0; i < MAX_EXIT; i++ )
  {
   if( room->exit[i] )
   {
    file << "#DOOR" << endl;
    file << "Dir     " << i << endl; /* Must be first for sanity checks --Kline */
    file << "Desc    " << room->exit[i]->description << delim << endl;
    file << "Flags   ";
    for( x = 0; x < MAX_BITSET; x++ )
     if( room->exit[i]->bitset_flags_out.test(x) )
      file << x << " ";
    file << "EOL" << endl;
    file << "Key     " << room->exit[i]->key << endl;
    file << "Keyword " << room->exit[i]->keyword << delim << endl;
    file << "Vnum    " << room->exit[i]->vnum << endl;
    file << "End" << endl;
   }
  }

  for( et = room->extra_list.begin(); et != room->extra_list.end(); et++ )
  {
   extra = *et;
   file << "#REXTRA" << endl;
   file << "Desc     " << extra->description << delim << endl;
   file << "Keyword  " << extra->keyword << delim << endl;
   file << "End" << endl;
  }
 }

 return;
}

void write_ackfuss_npc( ofstream &file, int type )
{
 char delim = '~';
 list<npc_data *>::iterator nt;
 npc_data *npc;
 int i = 0;

 for( nt = npc_list.begin(); nt != npc_list.end(); nt++ )
 {
  npc = *nt;
  file << "#MOBILE" << endl;
  file << "Vnum      " << npc->vnum << endl;
  file << "AcMod     " << npc->ac_mod << endl;
  file << "Act       ";
  for( i = 0; i < MAX_BITSET; i++ )
   if( npc->bitset_act_flags_out.test(i) )
    file << i << " ";
  file << "EOL" << endl;
  file << "Affected  " << 0 << endl;
  file << "Alignment " << npc->alignment << endl;
  file << "Cast      " << npc->cast << endl;
  file << "Clan      " << npc->clan << endl;
  file << "Class     " << npc->pclass << endl;
  file << "Def       " << npc->def << endl;
  file << "Desc      " << npc->description << delim << endl;
  file << "DrMod     " << npc->dr_mod << endl;
  file << "HrMod     " << npc->hr_mod << endl;
  file << "Level     " << npc->level << endl;
  file << "LongDesc  " << npc->long_descr << delim << endl;
  file << "PlrName   " << npc->player_name << delim << endl;
  file << "Position  " << npc->position << endl;
  file << "Race      " << npc->race << endl;
  file << "RaceMods  " << npc->race_mod << endl;
  file << "Resist    " << npc->resist << endl;
  file << "Sex       " << npc->sex << endl;
  file << "ShortDesc " << npc->short_descr << delim << endl;
  file << "Skills    " << npc->skills << endl;
  file << "SMagic    " << npc->strong_magic << endl;
  file << "Suscept   " << npc->suscept << endl;
  file << "WMagic    " << npc->weak_magic << endl;
  file << "End" << endl;
 }

 return;
}

void write_ackfuss_obj( ofstream &file, int type )
{
 char delim = '~';
 list<obj_data *>::iterator ot;
 list<affect_data *>::iterator at;
 list<extra_data *>::iterator et;
 obj_data *obj;
 affect_data *aff;
 extra_data *extra;
 int i = 0;

 for( ot = obj_list.begin(); ot != obj_list.end(); ot++ )
 {
  obj = *ot;
  file << "#OBJECT" << endl;
  file << "Vnum       " << obj->vnum << endl;
  file << "ExtraFlags ";
  for( i = 0; i < MAX_BITSET; i++ )
   if( obj->bitset_extra_flags_out.test(i) )
    file << i << " ";
  file << "EOL" << endl;
  file << "ItemApply  " << obj->int_item_apply_out << endl;
  file << "Level      " << obj->level << endl;
  file << "LongDesc   " << obj->long_descr << delim << endl;
  file << "Name       " << obj->name << delim << endl;
  file << "ShortDesc  " << obj->short_descr << delim << endl;
  file << "Type       " << obj->type << endl;
  file << "Values     ";
  for( i = 0; i < MAX_OBJ_VALUE; i++ )
   file << obj->value[i] << " ";
  file << endl;
  file << "WearFlags  ";
  for( i = 0; i < MAX_BITSET; i++ )
   if( obj->bitset_wear_flags_out.test(i) )
    file << i << " ";
  file << "EOL" << endl;
  file << "Weight     " << obj->weight << endl;
  file << "End" << endl;
  for( at = obj->apply_list.begin(); at != obj->apply_list.end(); at++ )
  {
   aff = *at;
   file << "#OAFFECT" << endl;
   file << "Location   " << aff->int_location_out << endl;
   file << "Modifier   " << aff->modifier << endl;
   file << "End" << endl;
  }
  for( et = obj->extra_list.begin(); et != obj->extra_list.end(); et++ )
  {
   extra = *et;
   file << "#OEXTRA" << endl;
   file << "Desc       " << extra->description << delim << endl;
   file << "Keyword    " << extra->keyword << delim << endl;
   file << "End" << endl;
  }
 }

 return;
}

void write_ackfuss_shop( ofstream &file, int type )
{
 list<shop_data *>::iterator st;
 shop_data *shop;
 int i = 0;

 for( st = shop_list.begin(); st != shop_list.end(); st++ )
 {
  shop = *st;
  file << "#SHOP" << endl;
  file << "Keeper     " << shop->keeper << endl;
  file << "BuyType    ";
  for( i = 0; i < MAX_TRADE; i++ )
   file << shop->buy_type[i] << " ";
  file << endl;
  file << "HourClose  " << shop->hour_close << endl;
  file << "HourOpen   " << shop->hour_open << endl;
  file << "ProfBuy    " << shop->prof_buy << endl;
  file << "ProfSell   " << shop->prof_sell << endl;
  file << "End" << endl;
 }

 return;
}

void write_ackfuss_reset( ofstream &file, int type )
{
 list<reset_data *>::iterator rt;
 reset_data *reset;

 file << "#RESETS" << endl;
 for( rt = reset_list.begin(); rt != reset_list.end(); rt++ )
 {
  reset = *rt;
  file << reset->command << " " << reset->arg1 << " " << reset->arg2 << " ";
  if( reset->command[0] == 'G' || reset->command[0] == 'R' )
   file << reset->notes << endl;
  else
   file << reset->arg3 << " " << reset->notes << endl;
 }
 file << "S" << endl;

 return;
}
