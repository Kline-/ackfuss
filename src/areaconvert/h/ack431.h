/*
 * Copyright Matt Goff (Kline) 2009
 * If you use my code, please give credit where it is due.
 * Support provided at www.ackmud.net
 */

#include "include.h"

void read_ack431      ( ifstream &file );
void read_ack431_area ( ifstream &file );
void read_ack431_room ( ifstream &file );
void read_ack431_npc  ( ifstream &file );
void read_ack431_obj  ( ifstream &file );

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

#define ACK431_NFLAG_NONE           0
#define ACK431_NFLAG_IS_NPC         1
#define ACK431_NFLAG_SENTINEL       2
#define ACK431_NFLAG_SCAVENGER      4
#define ACK431_NFLAG_REMEMBER       8
#define ACK431_NFLAG_NO_FLEE        16
#define ACK431_NFLAG_AGGRESSIVE     32
#define ACK431_NFLAG_STAY_AREA      64
#define ACK431_NFLAG_WIMPY          128
#define ACK431_NFLAG_PET            256
#define ACK431_NFLAG_TRAIN          512
#define ACK431_NFLAG_PRACTICE       1024
#define ACK431_NFLAG_MERCENARY      2048
#define ACK431_NFLAG_HEAL           4096
#define ACK431_NFLAG_ADAPT          8192
#define ACK431_NFLAG_UNDEAD         16384
#define ACK431_NFLAG_BANKER         32768
#define ACK431_NFLAG_NO_BODY        65536
#define ACK431_NFLAG_HUNTER         131072
#define ACK431_NFLAG_NOMIND         262144
#define ACK431_NFLAG_POSTMAN        524288
#define ACK431_NFLAG_REWIELD        1048576
#define ACK431_NFLAG_RE_EQUIP       2097152
#define ACK431_NFLAG_INTELLIGENT    4194304
#define ACK431_NFLAG_VAMPIRE        8388608
#define ACK431_NFLAG_BREEDER        16777216
#define ACK431_NFLAG_SOLO           33554432
#define ACK431_NFLAG_WEREWOLF       67108864
#define ACK431_NFLAG_MOUNT          134217728
#define ACK431_NFLAG_NOBLOOD        268435456
