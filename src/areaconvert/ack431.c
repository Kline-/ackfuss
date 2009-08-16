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

 read_ack431_area(file);

 return;
}

void read_ack431_area( ifstream &file )
{
 string tmp;
 char delim = '~';
 char c;

 file.ignore(6);    /* #AREA */
 getline(file,area.name,delim);

 for( ;; )
 {
  c = file.get();

  if( c == '#' )
   break;

  switch( c )
  {
   case 'B': getline(file,tmp); S_BIT(area.int_flags,ACK431_AFLAG_BUILDING); area_flags_found += "building ";        break;
   case 'F': getline(file,tmp); area.reset_rate = str2int(tmp);      break;
   case 'I': file.ignore(1); getline(file,tmp,' '); area.min_level = str2int(tmp); getline(file,tmp); area.max_level = str2int(tmp); break;
   case 'K': getline(file,area.keyword,delim);                       break;
   case 'L': getline(file,area.level_label,delim);                   break;
   case 'M': getline(file,tmp); S_BIT(area.int_flags,ACK431_AFLAG_NO_ROOM_AFF); area_flags_found += "no_room_affs "; break;
   case 'N': getline(file,tmp);                                      break;
   case 'O': getline(file,area.owner,delim);                         break;
   case 'P': getline(file,tmp); S_BIT(area.int_flags,ACK431_AFLAG_PAYAREA); area_flags_found += "pay_area ";         break;
   case 'Q': getline(file,tmp); area.revision = str2int(tmp);        break;
   case 'R': getline(file,area.can_read,delim);                      break;
   case 'S': getline(file,tmp); S_BIT(area.int_flags,ACK431_AFLAG_NOSHOW); area_flags_found += "no_show ";           break;
   case 'T': getline(file,tmp); S_BIT(area.int_flags,ACK431_AFLAG_TELEPORT); area_flags_found += "teleport ";        break;
   case 'U': getline(file,area.reset_msg,delim);                     break;
   case 'V': file.ignore(1); getline(file,tmp,' '); area.min_vnum = str2int(tmp); getline(file,tmp); area.max_vnum = str2int(tmp); break;
   case 'W': getline(file,area.can_write,delim);                     break;
   case 'X': getline(file,tmp);                                      break;
  }
 }

 return;
}
