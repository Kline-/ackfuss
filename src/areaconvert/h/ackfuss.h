/*
 * Copyright Matt Goff (Kline) 2009
 * If you use my code, please give credit where it is due.
 * Support provided at www.ackmud.net
 */

#include "include.h"

void write_ackfuss      ( ofstream &file, int type );
void write_ackfuss_area ( ofstream &file, int type );
void write_ackfuss_room ( ofstream &file, int type );

#define ACKFUSS_AFLAG_NONE           0
#define ACKFUSS_AFLAG_PAYAREA        1
#define ACKFUSS_AFLAG_TELEPORT       2
#define ACKFUSS_AFLAG_BUILDING       3
#define ACKFUSS_AFLAG_NOSHOW         4
#define ACKFUSS_AFLAG_NO_ROOM_AFF    5
#define ACKFUSS_AFLAG_NO_QUEST       6

#define ACKFUSS_EXFLAG_NONE          0
#define ACKFUSS_EXFLAG_ISDOOR        1
#define ACKFUSS_EXFLAG_CLOSED        2
#define ACKFUSS_EXFLAG_LOCKED        3
#define ACKFUSS_EXFLAG_CLIMB         4
#define ACKFUSS_EXFLAG_IMMORTAL      5
#define ACKFUSS_EXFLAG_PICKPROOF     6
#define ACKFUSS_EXFLAG_SMASHPROOF    7
#define ACKFUSS_EXFLAG_PASSPROOF     8
#define ACKFUSS_EXFLAG_NODETECT      9
#define ACKFUSS_EXFLAG_NOMOB         10

#define ACKFUSS_RFLAG_NONE           0
#define ACKFUSS_RFLAG_DARK           1
#define ACKFUSS_RFLAG_REGEN          2
#define ACKFUSS_RFLAG_NO_MOB         3
#define ACKFUSS_RFLAG_INDOORS        4
#define ACKFUSS_RFLAG_NO_MAGIC       5
#define ACKFUSS_RFLAG_HOT            6
#define ACKFUSS_RFLAG_COLD           7
#define ACKFUSS_RFLAG_PK             8
#define ACKFUSS_RFLAG_QUIET          9
#define ACKFUSS_RFLAG_PRIVATE        10
#define ACKFUSS_RFLAG_SAFE           11
#define ACKFUSS_RFLAG_SOLITARY       12
#define ACKFUSS_RFLAG_PET_SHOP       13
#define ACKFUSS_RFLAG_NO_RECALL      14
#define ACKFUSS_RFLAG_NO_TELEPORT    15
#define ACKFUSS_RFLAG_HUNT_MARK      16
#define ACKFUSS_RFLAG_NO_BLOODWALK   17
#define ACKFUSS_RFLAG_NO_PORTAL      18
#define ACKFUSS_RFLAG_NO_REPOP       19
