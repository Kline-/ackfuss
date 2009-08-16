/*
 * Copyright Matt Goff (Kline) 2009
 * If you use my code, please give credit where it is due.
 * Support provided at www.ackmud.net
 */

#include "include.h"

void read_ack431      ( ifstream &file );
void read_ack431_area ( ifstream &file );

#define ACK431_AFLAG_NONE           0
#define ACK431_AFLAG_PAYAREA        1
#define ACK431_AFLAG_TELEPORT       2
#define ACK431_AFLAG_BUILDING       4
#define ACK431_AFLAG_NOSHOW         8
#define ACK431_AFLAG_NO_ROOM_AFF    16
