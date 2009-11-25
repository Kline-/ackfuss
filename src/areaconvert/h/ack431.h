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
void read_ack431_shop ( ifstream &file );
void read_ack431_reset( ifstream &file );

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

#define ACK431_OFLAG_NONE           0
#define ACK431_OFLAG_GLOW           1
#define ACK431_OFLAG_HUM            2
#define ACK431_OFLAG_NODISARM       4
#define ACK431_OFLAG_LOCK           8
#define ACK431_OFLAG_EVIL           16
#define ACK431_OFLAG_INVIS          32
#define ACK431_OFLAG_MAGIC          64
#define ACK431_OFLAG_NODROP         128
#define ACK431_OFLAG_BLESS          256
#define ACK431_OFLAG_ANTI_GOOD      512
#define ACK431_OFLAG_ANTI_EVIL      1024
#define ACK431_OFLAG_ANTI_NEUTRAL   2048
#define ACK431_OFLAG_NOREMOVE       4096
#define ACK431_OFLAG_INVENTORY      8192
#define ACK431_OFLAG_NOSAVE         16384
#define ACK431_OFLAG_CLAN_EQ        32768
#define ACK431_OFLAG_TRIG_DESTROY   65536
#define ACK431_OFLAG_NO_AUCTION     131072
#define ACK431_OFLAG_REMORT         262144
#define ACK431_OFLAG_ADEPT          524288
#define ACK431_OFLAG_RARE           1048576
#define ACK431_OFLAG_VAMP           2097152
#define ACK431_OFLAG_NOLOOT         4194304
#define ACK431_OFLAG_NOSAC          8388608
#define ACK431_OFLAG_UNIQUE         16777216
#define ACK431_OFLAG_LIFESTEALER    33554432
#define ACK431_OFLAG_SILVER         67108864

#define ACK431_WEAR_NONE            0
#define ACK431_WEAR_HALO            1
#define ACK431_WEAR_AURA            2
#define ACK431_WEAR_HORNS           4
#define ACK431_WEAR_HEAD            8
#define ACK431_WEAR_FACE            16
#define ACK431_WEAR_BEAK            32
#define ACK431_WEAR_EAR             64
#define ACK431_WEAR_NECK            128
#define ACK431_WEAR_WINGS           256
#define ACK431_WEAR_SHOULDERS       512
#define ACK431_WEAR_ARMS            1024
#define ACK431_WEAR_WRIST           2048
#define ACK431_WEAR_HANDS           4096
#define ACK431_WEAR_FINGER          8192
#define ACK431_WEAR_CLAWS           16384
#define ACK431_WEAR_HOLD_HAND       32768
#define ACK431_WEAR_ABOUT           65536
#define ACK431_WEAR_WAIST           131072
#define ACK431_WEAR_BODY            262144
#define ACK431_WEAR_TAIL            524288
#define ACK431_WEAR_LEGS            1048576
#define ACK431_WEAR_FEET            2097152
#define ACK431_WEAR_HOOVES          4194304
#define ACK431_WEAR_TAKE            8388608

#define ACK431_APPLY_NONE           0
#define ACK431_APPLY_STR            1
#define ACK431_APPLY_DEX            2
#define ACK431_APPLY_INT            3
#define ACK431_APPLY_WIS            4
#define ACK431_APPLY_CON            5
#define ACK431_APPLY_SEX            6
#define ACK431_APPLY_CLASS          7
#define ACK431_APPLY_LEVEL          8
#define ACK431_APPLY_AGE            9
#define ACK431_APPLY_HEIGHT         10
#define ACK431_APPLY_WEIGHT         11
#define ACK431_APPLY_MANA           12
#define ACK431_APPLY_HIT            13
#define ACK431_APPLY_MOVE           14
#define ACK431_APPLY_GOLD           15
#define ACK431_APPLY_EXP            16
#define ACK431_APPLY_AC             17
#define ACK431_APPLY_HITROLL        18
#define ACK431_APPLY_DAMROLL        19
#define ACK431_APPLY_SAVING_PARA    20
#define ACK431_APPLY_SAVING_ROD     21
#define ACK431_APPLY_SAVING_PETRI   22
#define ACK431_APPLY_SAVING_BREATH  23
#define ACK431_APPLY_SAVING_SPELL   24

#define ACK431_IAPPLY_NONE          0
/* used to be NONE, as bit 1 -- stupid */
#define ACK431_IAPPLY_INFRA         2
#define ACK431_IAPPLY_INV           4
#define ACK431_IAPPLY_DET_INV       8
#define ACK431_IAPPLY_SANC          16
#define ACK431_IAPPLY_SNEAK         32
#define ACK431_IAPPLY_HIDE          64
#define ACK431_IAPPLY_PROT          128
#define ACK431_IAPPLY_ENHANCED      256
#define ACK431_IAPPLY_DET_MAG       512
#define ACK431_IAPPLY_DET_HID       1024
#define ACK431_IAPPLY_DET_EVIL      2048
#define ACK431_IAPPLY_PASS_DOOR     4096
#define ACK431_IAPPLY_DET_POISON    8192
#define ACK431_IAPPLY_FLY           16384
#define ACK431_IAPPLY_KNOW_ALIGN    32768
#define ACK431_IAPPLY_DET_UNDEAD    65536
#define ACK431_IAPPLY_HEATED        131072
