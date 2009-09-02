/*
 * Copyright Matt Goff (Kline) 2009
 * If you use my code, please give credit where it is due.
 * Support provided at www.ackmud.net
 */

#include "include.h"

void read_ack431      ( ifstream &file );
void read_ack431_area ( ifstream &file );
void read_ack431_room ( ifstream &file );

#define ACK431_AFLAG_NONE           0
#define ACK431_AFLAG_PAYAREA        1
#define ACK431_AFLAG_TELEPORT       2
#define ACK431_AFLAG_BUILDING       4
#define ACK431_AFLAG_NOSHOW         8
#define ACK431_AFLAG_NO_ROOM_AFF    16

#define ACK431_EXFLAG_NONE          0
#define ACK431_EXFLAG_ISDOOR        1
#define ACK431_EXFLAG_CLOSED        2
#define ACK431_EXFLAG_LOCKED        4
#define ACK431_EXFLAG_CLIMB         8
#define ACK431_EXFLAG_IMMORTAL      16
#define ACK431_EXFLAG_PICKPROOF     32
#define ACK431_EXFLAG_SMASHPROOF    64
#define ACK431_EXFLAG_PASSPROOF     128
#define ACK431_EXFLAG_NODETECT      256

#define ACK413_RFLAG_NONE           0
#define ACK431_RFLAG_DARK           1
#define ACK431_RFLAG_REGEN          2
#define ACK431_RFLAG_NO_MOB         4
#define ACK431_RFLAG_INDOORS        8
#define ACK431_RFLAG_NO_MAGIC       16
#define ACK431_RFLAG_HOT            32
#define ACK431_RFLAG_COLD           64
#define ACK431_RFLAG_PK             128
#define ACK431_RFLAG_QUIET          256
#define ACK431_RFLAG_PRIVATE        512
#define ACK431_RFLAG_SAFE           1024
#define ACK431_RFLAG_SOLITARY       2048
#define ACK431_RFLAG_PET_SHOP       4096
#define ACK431_RFLAG_NO_RECALL      8192
#define ACK431_RFLAG_NO_TELEPORT    16384
#define ACK431_RFLAG_HUNT_MARK      32768
#define ACK431_RFLAG_NOBLOODWALK    65536
#define ACK431_RFLAG_NO_PORTAL      131072
#define ACK431_RFLAG_NO_REPOP       262144
