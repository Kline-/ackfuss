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

 for( ;; )
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
  }
 }

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

 for( ;; )
 {
  c = file.get();
cout << " c = " << c;
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

  for( ;; )
  {
   c = file.get();

   if( c == 'S' )
   {
    cout << "got S" << endl;
    break;
   }
   if( c == 'D' )
   {
    cout << "got D" << endl;
    exit_data *exit = new exit_data;
    getline(file,tmp);
    room->exit[str2int(tmp)] = exit;
    getline(file,exit->description,delim);
    getline(file,exit->keyword,delim);
    getline(file,tmp,' '); /* locks */
    getline(file,tmp,' '); exit->key = str2int(tmp);
    getline(file,tmp); exit->vnum = str2int(tmp);
   }
  }
cout << "ended loop" << endl;
  room_list.push_back(room);
 }

 return;
}
