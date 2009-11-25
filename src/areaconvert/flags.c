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
    case TYPE_ACKFUSS: flags_ack431_ackfuss(); break;
   }
   break;
 }

 return;
}

void clear_area_flag( string name )
{
 size_t first, last;
 first = area.flags_found.find(name);
 last = name.length() + 1;
 area.flags_found.erase(first,last);
}

void flags_ack431_ackfuss( void )
{
 aflag_ack431_ackfuss();
 rflag_ack431_ackfuss();
 nflag_ack431_ackfuss();
 oflag_ack431_ackfuss();
 return;
}

void aflag_ack431_ackfuss( void )
{
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

 return;
}

void rflag_ack431_ackfuss( void )
{
 list<room_data *>::iterator rt;
 room_data *room;
 int i = 0;

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

void nflag_ack431_ackfuss( void )
{
 list<npc_data *>::iterator nt;
 npc_data *npc;

 for( nt = npc_list.begin(); nt != npc_list.end(); nt++ )
 {
  npc = *nt;
  /* if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_IS_NPC) ) -- do something? this is an automatic bool in ackfuss */
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_SENTINEL) )    npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_SENTINEL);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_SCAVENGER) )   npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_SCAVENGER);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_REMEMBER) )    npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_REMEMBER);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_NO_FLEE) )     npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_NO_FLEE);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_AGGRESSIVE) )  npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_AGGRESSIVE);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_STAY_AREA) )   npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_STAY_AREA);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_WIMPY) )       npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_WIMPY);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_PET) )         npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_PET);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_TRAIN) )       npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_TRAIN);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_PRACTICE) )    npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_PRACTICE);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_MERCENARY) )   npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_MERCENARY);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_HEAL) )        npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_HEAL);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_ADAPT) )       npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_ADAPT);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_UNDEAD) )      npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_UNDEAD);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_BANKER) )      npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_BANKER);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_NO_BODY) )     npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_NO_BODY);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_HUNTER) )      npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_HUNTER);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_NOMIND) )      npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_NO_MIND);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_POSTMAN) )     npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_POSTMAN);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_REWIELD) )     npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_RE_WIELD);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_RE_EQUIP) )    npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_RE_EQUIP);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_INTELLIGENT) ) npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_INTELLIGENT);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_VAMPIRE) )     npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_VAMPIRE);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_BREEDER) )     npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_BREEDER);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_SOLO) )        npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_SOLO);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_WEREWOLF) )    npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_WEREWOLF);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_MOUNT) )       npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_MOUNT);
  if( I_BIT(npc->int_act_flags_in,ACK431_NFLAG_NOBLOOD) )     npc->bitset_act_flags_out.flip(ACKFUSS_NFLAG_NO_BLOOD);
 }

 return;
}

void oflag_ack431_ackfuss( void )
{
 list<obj_data *>::iterator ot;
 list<affect_data *>::iterator at;
 obj_data *obj;
 affect_data *aff;

 for( ot = obj_list.begin(); ot != obj_list.end(); ot++ )
 {
  obj = *ot;
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_GLOW) )         obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_GLOW);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_HUM) )          obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_HUM);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_NODISARM) )     obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_NO_DISARM);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_LOCK) )         obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_LOCK);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_EVIL) )         obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_EVIL);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_INVIS) )        obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_INVIS);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_MAGIC) )        obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_MAGIC);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_NODROP) )       obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_NO_DROP);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_BLESS) )        obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_BLESS);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_ANTI_GOOD) )    obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_ANTI_GOOD);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_ANTI_EVIL) )    obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_ANTI_EVIL);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_ANTI_NEUTRAL) ) obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_ANTI_NEUTRAL);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_NOREMOVE) )     obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_NO_REMOVE);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_INVENTORY) )    obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_INVENTORY);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_NOSAVE) )       obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_NO_SAVE);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_CLAN_EQ) )      obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_CLAN_EQ);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_TRIG_DESTROY) ) obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_TRIG_DESTROY);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_NO_AUCTION) )   obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_NO_AUCTION);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_REMORT) )       obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_REMORT);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_ADEPT) )        obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_ADEPT);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_RARE) )         obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_RARE);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_VAMP) )         obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_VAMP);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_NOLOOT) )       obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_NO_LOOT);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_NOSAC) )        obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_NO_SAC);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_UNIQUE) )       obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_UNIQUE);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_LIFESTEALER) )  obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_LIFESTEALER);
  if( I_BIT(obj->int_extra_flags_in,ACK431_OFLAG_SILVER) )       obj->bitset_extra_flags_out.flip(ACKFUSS_OFLAG_SILVER);

  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_HALO) )           obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_HALO);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_AURA) )           obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_AURA);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_HORNS) )          obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_HORNS);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_HEAD) )           obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_HEAD);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_FACE) )           obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_FACE);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_BEAK) )           obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_BEAK);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_EAR) )            obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_EAR);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_NECK) )           obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_NECK);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_WINGS) )          obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_WINGS);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_SHOULDERS) )      obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_SHOULDERS);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_ARMS) )           obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_ARMS);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_WRIST) )          obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_WRIST);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_HANDS) )          obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_HANDS);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_FINGER) )         obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_FINGER);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_CLAWS) )          obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_CLAWS);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_HOLD_HAND) )      obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_HOLD_HAND);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_ABOUT) )          obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_ABOUT);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_WAIST) )          obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_WAIST);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_BODY) )           obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_BODY);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_TAIL) )           obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_TAIL);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_LEGS) )           obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_LEGS);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_FEET) )           obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_FEET);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_HOOVES) )         obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_HOOVES);
  if( I_BIT(obj->int_wear_flags_in,ACK431_WEAR_TAKE) )           obj->bitset_wear_flags_out.flip(ACKFUSS_WEAR_TAKE);

  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_INFRA) )        S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_INFRA);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_INV) )          S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_INV);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_DET_INV) )      S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_DET_INV);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_SANC) )         S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_SANC);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_HIDE) )         S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_HIDE);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_PROT) )         S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_PROT);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_ENHANCED) )     S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_ENHANCED);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_DET_MAG) )      S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_DET_MAG);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_DET_HID) )      S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_DET_HID);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_DET_EVIL) )     S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_DET_EVIL);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_PASS_DOOR) )    S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_PASS_DOOR);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_DET_POISON) )   S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_DET_POISON);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_FLY) )          S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_FLY);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_KNOW_ALIGN) )   S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_KNOW_ALIGN);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_DET_UNDEAD) )   S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_DET_UNDEAD);
  if( I_BIT(obj->int_item_apply_in,ACK431_IAPPLY_HEATED) )       S_BIT(obj->int_item_apply_out,ACKFUSS_IAPPLY_HEATED);

  for( at = obj->apply_list.begin(); at != obj->apply_list.end(); at++ )
  {
   aff = *at;
   if( aff->int_location_in == ACK431_APPLY_STR )                aff->int_location_out = ACKFUSS_APPLY_STR;
   if( aff->int_location_in == ACK431_APPLY_DEX )                aff->int_location_out = ACKFUSS_APPLY_DEX;
   if( aff->int_location_in == ACK431_APPLY_INT )                aff->int_location_out = ACKFUSS_APPLY_INT;
   if( aff->int_location_in == ACK431_APPLY_WIS )                aff->int_location_out = ACKFUSS_APPLY_WIS;
   if( aff->int_location_in == ACK431_APPLY_CON )                aff->int_location_out = ACKFUSS_APPLY_CON;
   if( aff->int_location_in == ACK431_APPLY_SEX )                aff->int_location_out = ACKFUSS_APPLY_SEX;
   if( aff->int_location_in == ACK431_APPLY_CLASS )              aff->int_location_out = ACKFUSS_APPLY_CLASS;
   if( aff->int_location_in == ACK431_APPLY_LEVEL )              aff->int_location_out = ACKFUSS_APPLY_LEVEL;
   if( aff->int_location_in == ACK431_APPLY_AGE )                aff->int_location_out = ACKFUSS_APPLY_AGE;
   if( aff->int_location_in == ACK431_APPLY_HEIGHT )             aff->int_location_out = ACKFUSS_APPLY_HEIGHT;
   if( aff->int_location_in == ACK431_APPLY_WEIGHT )             aff->int_location_out = ACKFUSS_APPLY_WEIGHT;
   if( aff->int_location_in == ACK431_APPLY_MANA )               aff->int_location_out = ACKFUSS_APPLY_MANA;
   if( aff->int_location_in == ACK431_APPLY_HIT )                aff->int_location_out = ACKFUSS_APPLY_HIT;
   if( aff->int_location_in == ACK431_APPLY_MOVE )               aff->int_location_out = ACKFUSS_APPLY_MOVE;
   /* Skip APPLY_GOLD, unused in AckFUSS */
   if( aff->int_location_in == ACK431_APPLY_EXP )                aff->int_location_out = ACKFUSS_APPLY_EXP;
   if( aff->int_location_in == ACK431_APPLY_AC )                 aff->int_location_out = ACKFUSS_APPLY_AC;
   if( aff->int_location_in == ACK431_APPLY_HITROLL )            aff->int_location_out = ACKFUSS_APPLY_HITROLL;
   if( aff->int_location_in == ACK431_APPLY_DAMROLL )            aff->int_location_out = ACKFUSS_APPLY_DAMROLL;
   if( aff->int_location_in == ACK431_APPLY_SAVING_PARA )        aff->int_location_out = ACKFUSS_APPLY_SAVING_PARA;
   if( aff->int_location_in == ACK431_APPLY_SAVING_ROD )         aff->int_location_out = ACKFUSS_APPLY_SAVING_ROD;
   if( aff->int_location_in == ACK431_APPLY_SAVING_PETRI )       aff->int_location_out = ACKFUSS_APPLY_SAVING_PETRI;
   if( aff->int_location_in == ACK431_APPLY_SAVING_BREATH )      aff->int_location_out = ACKFUSS_APPLY_SAVING_BREATH;
   if( aff->int_location_in == ACK431_APPLY_SAVING_SPELL )       aff->int_location_out = ACKFUSS_APPLY_SAVING_SPELL;
  }
 }

 return;
}
