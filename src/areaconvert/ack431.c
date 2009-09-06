/*
 * Copyright Matt Goff (Kline) 2009
 * If you use my code, please give credit where it is due.
 * Support provided at www.ackmud.net
 */

#include "h/areaconvert.h"
#include "h/extern.h"

void read_ack431( ifstream &file )
{
 string tmp;

 while( !file.eof() )
 {
  getline(file,tmp);

  if( tmp[0] == '#' )
  {
   if( tmp[1] == '$' )
    break;

   if( tmp == "#AREA" )
    read_ack431_area(file);
   else if( tmp == "#ROOMS" )
    read_ack431_room(file);
   else if( tmp == "#MOBILES" )
    read_ack431_npc(file);
   else if( tmp == "#OBJECTS" )
    read_ack431_obj(file);
  }
 }

 return;
}

void read_ack431_area( ifstream &file )
{
 string tmp;
 char delim = '~';
 char c;

 getline(file,area.name,delim);

 while( !file.eof() )
 {
  c = file.get();

  if( c == '#' )
   break;

  switch( c )
  {
   case 'B': getline(file,tmp); S_BIT(area.int_flags_in,ACK431_AFLAG_BUILDING); area.flags_found += "building ";        break;
   case 'F': getline(file,tmp); area.reset_rate = str2int(tmp);      break;
   case 'I': file.ignore(1); getline(file,tmp,' '); area.min_level = str2int(tmp); getline(file,tmp); area.max_level = str2int(tmp); break;
   case 'K': getline(file,area.keyword,delim);                       break;
   case 'L': getline(file,area.level_label,delim);                   break;
   case 'M': getline(file,tmp); S_BIT(area.int_flags_in,ACK431_AFLAG_NO_ROOM_AFF); area.flags_found += "no_room_affs "; break;
   case 'N': getline(file,tmp);                                      break;
   case 'O': getline(file,area.owner,delim);                         break;
   case 'P': getline(file,tmp); S_BIT(area.int_flags_in,ACK431_AFLAG_PAYAREA); area.flags_found += "pay_area ";         break;
   case 'Q': getline(file,tmp); area.revision = str2int(tmp);        break;
   case 'R': getline(file,area.can_read,delim);                      break;
   case 'S': getline(file,tmp); S_BIT(area.int_flags_in,ACK431_AFLAG_NOSHOW); area.flags_found += "no_show ";           break;
   case 'T': getline(file,tmp); S_BIT(area.int_flags_in,ACK431_AFLAG_TELEPORT); area.flags_found += "teleport ";        break;
   case 'U': getline(file,area.reset_msg,delim);                     break;
   case 'V': file.ignore(1); getline(file,tmp,' '); area.min_vnum = str2int(tmp); getline(file,tmp); area.max_vnum = str2int(tmp); break;
   case 'W': getline(file,area.can_write,delim);                     break;
   case 'X': getline(file,tmp);                                      break;
  }
 }

 file.unget();
 return;
}

void read_ack431_room( ifstream &file )
{
 string tmp;
 room_data *room;
 int vnum = 0;
 char delim = '~';
 char c;

 while( !file.eof() )
 {
  tmp.clear();
  while( (c = file.get()) != '#' );

  if( c == '#' )
   getline(file,tmp); vnum = str2int(tmp);

  if( vnum == 0 ) /* reached the end */
   break;

  room = new room_data;
  room->vnum = vnum;
  getline(file,room->name,delim);
  getline(file,room->description,delim);
  getline(file,tmp,' '); room->int_flags_in = str2int(tmp);
  getline(file,tmp); room->sector = str2int(tmp);

  while( !file.eof() )
  {
   c = file.get();

   if( c == 'S' ) /* stop */
    break;
   if( c == 'D' ) /* door */
   {
    exit_data *exit = new exit_data;
    getline(file,tmp);
    room->exit[str2int(tmp)] = exit;
    getline(file,exit->description,delim);
    getline(file,exit->keyword,delim);
    getline(file,tmp,' '); exit->int_flags_in = str2int(tmp);
    getline(file,tmp,' '); exit->key = str2int(tmp);
    getline(file,tmp); exit->vnum = str2int(tmp);
   }
   if( c == 'E' ) /* extra descrip */
   {
    extra_data *extra = new extra_data;
    getline(file,extra->keyword,delim);
    getline(file,extra->description,delim);
    room->extra_list.push_back(extra);
   }
  }

  room_list.push_back(room);
 }

 return;
}

void read_ack431_npc( ifstream &file )
{
 string tmp;
 npc_data *npc;
 int vnum = 0;
 char delim = '~';
 char c;

 while( !file.eof() )
 {
  tmp.clear();
  while( (c = file.get()) != '#' );

  if( c == '#' )
   getline(file,tmp); vnum = str2int(tmp);

  if( vnum == 0 ) /* reached the end */
   break;

  npc = new npc_data;
  npc->vnum = vnum;
  getline(file,npc->player_name,delim);
  getline(file,npc->short_descr,delim);
  getline(file,npc->long_descr,delim);
  getline(file,npc->description,delim);
  getline(file,tmp,' '); npc->int_act_flags_in = str2int(tmp);
  getline(file,tmp,' '); npc->affected_by = str2int(tmp);
  getline(file,tmp,' '); npc->alignment = str2int(tmp);
  file.ignore(1); /* S */
  getline(file,tmp,' '); npc->level = str2int(tmp);
  getline(file,tmp); npc->sex = str2int(tmp);
  getline(file,tmp,' '); npc->ac_mod = str2int(tmp);
  getline(file,tmp,' '); npc->hr_mod = str2int(tmp);
  getline(file,tmp); npc->dr_mod = str2int(tmp);
  c = file.get(); /* ! */
  if( c == '!' )
  {
   getline(file,tmp,' '); npc->pclass = str2int(tmp);
   getline(file,tmp,' '); npc->clan = str2int(tmp);
   getline(file,tmp,' '); npc->race = str2int(tmp);
   getline(file,tmp,' '); npc->position = str2int(tmp);
   getline(file,tmp,' '); npc->skills = str2int(tmp);
   getline(file,tmp,' '); npc->cast = str2int(tmp);
   getline(file,tmp); npc->def = str2int(tmp);
  }
  else
   file.unget();
  c = file.get(); /* | */
  if( c == '|' )
  {
   getline(file,tmp,' '); npc->strong_magic = str2int(tmp);
   getline(file,tmp,' '); npc->weak_magic = str2int(tmp);
   getline(file,tmp,' '); npc->race_mod = str2int(tmp);
   getline(file,tmp,' '); npc->power_skill = str2int(tmp);
   getline(file,tmp,' '); npc->power_cast = str2int(tmp);
   getline(file,tmp,' '); npc->resist = str2int(tmp);
   getline(file,tmp); npc->suscept = str2int(tmp);
  }
  else
   file.unget();

  npc_list.push_back(npc);
 }

 return;
}

void read_ack431_obj( ifstream &file )
{
 string tmp;
 obj_data *obj;
 int vnum = 0;
 int i = 0;
 char delim = '~';
 char c;

 while( !file.eof() )
 {
  tmp.clear();
  while( (c = file.get()) != '#' );

  if( c == '#' )
   getline(file,tmp); vnum = str2int(tmp);

  if( vnum == 0 ) /* reached the end */
   break;

  obj = new obj_data;
  obj->vnum = vnum;
  obj->level = 1; /* Level may or may not be provided in some ACK431 area files */
  getline(file,obj->name,delim);
  getline(file,obj->short_descr,delim);
  getline(file,obj->long_descr,delim);
  getline(file,tmp,' '); obj->type = str2int(tmp);
  getline(file,tmp,' '); obj->int_extra_flags_in = str2int(tmp);
  getline(file,tmp,' '); obj->int_wear_flags_in = str2int(tmp);
  getline(file,tmp); obj->item_apply = str2int(tmp);
  for( i = 0; i < MAX_OBJ_VALUE-1; i++ )
   getline(file,tmp,' '); obj->value[i] = str2int(tmp);
  getline(file,tmp); obj->value[MAX_OBJ_VALUE-1] = str2int(tmp);
  getline(file,tmp); obj->weight = str2int(tmp);

  while( !file.eof() )
  {
   c = file.get();

   if( c == 'A' ) /* affect */
   {
    affect_data *aff = new affect_data;
    getline(file,tmp,' '); aff->int_location_in = str2int(tmp);
    getline(file,tmp); aff->modifier = str2int(tmp);
    obj->apply_list.push_back(aff);
   }
   else if( c == 'E' ) /* extra descr */
   {
   }
   else if( c == 'L' ) /* item level */
   {
    getline(file,tmp); obj->level = str2int(tmp);
   }
   else /* nothing interesting found */
   {
    file.unget();
    break;
   }
  }

  obj_list.push_back(obj);
 }

 return;
}
