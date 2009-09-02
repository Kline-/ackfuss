/*
 * Copyright Matt Goff (Kline) 2009
 * If you use my code, please give credit where it is due.
 * Support provided at www.ackmud.net
 */

#include "h/areaconvert.h"
#include "h/extern.h"

void flag_handler( int typein, int typeout )
{
 switch( typein )
 {
  case TYPE_ACK431:
   switch( typeout )
   {
    case TYPE_ACKFUSS: rflag_ack431_ackfuss(); break;
   }
   break;
 }

 return;
}

void clear_area_flag( string name )
{
 size_t first, last;

 first = area.flags_found.find(name);
 last = first + name.length() + 1;
 area.flags_found.erase(first,last);
}

void rflag_ack431_ackfuss( void )
{
 list<room_data *>::iterator rt;
 room_data *room;
 int i = 0;

 if( I_BIT(area.int_flags_in,ACK431_AFLAG_BUILDING) )
  { area.bitset_flags_out.flip(ACKFUSS_AFLAG_BUILDING); clear_area_flag("building"); }
 if( I_BIT(area.int_flags_in,ACK431_AFLAG_NO_ROOM_AFF) )
  { area.bitset_flags_out.flip(ACKFUSS_AFLAG_NO_ROOM_AFF); clear_area_flag("no_room_affs"); }
 if( I_BIT(area.int_flags_in,ACK431_AFLAG_PAYAREA) )
  { area.bitset_flags_out.flip(ACKFUSS_AFLAG_PAYAREA); clear_area_flag("pay_area"); }
 if( I_BIT(area.int_flags_in,ACK431_AFLAG_NOSHOW) )
  { area.bitset_flags_out.flip(ACKFUSS_AFLAG_NOSHOW); clear_area_flag("no_show"); }
 if( I_BIT(area.int_flags_in,ACK431_AFLAG_TELEPORT) )
  { area.bitset_flags_out.flip(ACKFUSS_AFLAG_TELEPORT); clear_area_flag("teleport"); }

 for( rt = room_list.begin(); rt != room_list.end(); rt++ )
 {
  room = *rt;
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_DARK) )         room->bitset_flags_out.flip(ACKFUSS_RFLAG_DARK);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_REGEN) )        room->bitset_flags_out.flip(ACKFUSS_RFLAG_REGEN);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_NO_MOB) )       room->bitset_flags_out.flip(ACKFUSS_RFLAG_NO_MOB);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_INDOORS) )      room->bitset_flags_out.flip(ACKFUSS_RFLAG_INDOORS);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_NO_MAGIC) )     room->bitset_flags_out.flip(ACKFUSS_RFLAG_NO_MAGIC);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_HOT) )          room->bitset_flags_out.flip(ACKFUSS_RFLAG_HOT);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_COLD) )         room->bitset_flags_out.flip(ACKFUSS_RFLAG_COLD);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_PK) )           room->bitset_flags_out.flip(ACKFUSS_RFLAG_PK);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_QUIET) )        room->bitset_flags_out.flip(ACKFUSS_RFLAG_QUIET);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_PRIVATE) )      room->bitset_flags_out.flip(ACKFUSS_RFLAG_PRIVATE);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_SAFE) )         room->bitset_flags_out.flip(ACKFUSS_RFLAG_SAFE);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_SOLITARY) )     room->bitset_flags_out.flip(ACKFUSS_RFLAG_SOLITARY);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_PET_SHOP) )     room->bitset_flags_out.flip(ACKFUSS_RFLAG_PET_SHOP);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_NO_RECALL) )    room->bitset_flags_out.flip(ACKFUSS_RFLAG_NO_RECALL);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_NO_TELEPORT) )  room->bitset_flags_out.flip(ACKFUSS_RFLAG_NO_TELEPORT);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_HUNT_MARK) )    room->bitset_flags_out.flip(ACKFUSS_RFLAG_HUNT_MARK);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_NOBLOODWALK) )  room->bitset_flags_out.flip(ACKFUSS_RFLAG_NO_BLOODWALK);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_NO_PORTAL) )    room->bitset_flags_out.flip(ACKFUSS_RFLAG_NO_PORTAL);
  if( I_BIT(room->int_flags_in,ACK431_RFLAG_NO_REPOP) )     room->bitset_flags_out.flip(ACKFUSS_RFLAG_NO_REPOP);

  for( i = 0; i < MAX_EXIT; i++ )
  {
   if( room->exit[i] )
   {
    if( I_BIT(room->exit[i]->int_flags_in,ACK431_EXFLAG_ISDOOR) )     room->exit[i]->bitset_flags_out.flip(ACKFUSS_EXFLAG_ISDOOR);
    if( I_BIT(room->exit[i]->int_flags_in,ACK431_EXFLAG_CLOSED) )     room->exit[i]->bitset_flags_out.flip(ACKFUSS_EXFLAG_CLOSED);
    if( I_BIT(room->exit[i]->int_flags_in,ACK431_EXFLAG_LOCKED) )     room->exit[i]->bitset_flags_out.flip(ACKFUSS_EXFLAG_LOCKED);
    if( I_BIT(room->exit[i]->int_flags_in,ACK431_EXFLAG_CLIMB) )      room->exit[i]->bitset_flags_out.flip(ACKFUSS_EXFLAG_CLIMB);
    if( I_BIT(room->exit[i]->int_flags_in,ACK431_EXFLAG_IMMORTAL) )   room->exit[i]->bitset_flags_out.flip(ACKFUSS_EXFLAG_IMMORTAL);
    if( I_BIT(room->exit[i]->int_flags_in,ACK431_EXFLAG_PICKPROOF) )  room->exit[i]->bitset_flags_out.flip(ACKFUSS_EXFLAG_PICKPROOF);
    if( I_BIT(room->exit[i]->int_flags_in,ACK431_EXFLAG_SMASHPROOF) ) room->exit[i]->bitset_flags_out.flip(ACKFUSS_EXFLAG_SMASHPROOF);
    if( I_BIT(room->exit[i]->int_flags_in,ACK431_EXFLAG_PASSPROOF) )  room->exit[i]->bitset_flags_out.flip(ACKFUSS_EXFLAG_PASSPROOF);
    if( I_BIT(room->exit[i]->int_flags_in,ACK431_EXFLAG_NODETECT) )   room->exit[i]->bitset_flags_out.flip(ACKFUSS_EXFLAG_NODETECT);
   }
  }
 }

 return;
}
