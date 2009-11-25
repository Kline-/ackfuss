/*
 * Copyright Matt Goff (Kline) 2009
 * If you use my code, please give credit where it is due.
 * Support provided at www.ackmud.net
 */

#include "include.h"

void write_ackfuss      ( ofstream &file, int type );
void write_ackfuss_area ( ofstream &file, int type );
void write_ackfuss_room ( ofstream &file, int type );
void write_ackfuss_npc  ( ofstream &file, int type );
void write_ackfuss_obj  ( ofstream &file, int type );
void write_ackfuss_shop ( ofstream &file, int type );
void write_ackfuss_reset( ofstream &file, int type );

#define ACKFUSS_AFLAG_NONE          0
#define ACKFUSS_AFLAG_PAYAREA       1
#define ACKFUSS_AFLAG_TELEPORT      2
#define ACKFUSS_AFLAG_BUILDING      3
#define ACKFUSS_AFLAG_NOSHOW        4
#define ACKFUSS_AFLAG_NO_ROOM_AFF   5
#define ACKFUSS_AFLAG_NO_QUEST      6

#define ACKFUSS_EXFLAG_NONE         0
#define ACKFUSS_EXFLAG_ISDOOR       1
#define ACKFUSS_EXFLAG_CLOSED       2
#define ACKFUSS_EXFLAG_LOCKED       3
#define ACKFUSS_EXFLAG_CLIMB        4
#define ACKFUSS_EXFLAG_IMMORTAL     5
#define ACKFUSS_EXFLAG_PICKPROOF    6
#define ACKFUSS_EXFLAG_SMASHPROOF   7
#define ACKFUSS_EXFLAG_PASSPROOF    8
#define ACKFUSS_EXFLAG_NODETECT     9
#define ACKFUSS_EXFLAG_NOMOB        10

#define ACKFUSS_RFLAG_NONE          0
#define ACKFUSS_RFLAG_DARK          1
#define ACKFUSS_RFLAG_REGEN         2
#define ACKFUSS_RFLAG_NO_MOB        3
#define ACKFUSS_RFLAG_INDOORS       4
#define ACKFUSS_RFLAG_NO_MAGIC      5
#define ACKFUSS_RFLAG_HOT           6
#define ACKFUSS_RFLAG_COLD          7
#define ACKFUSS_RFLAG_PK            8
#define ACKFUSS_RFLAG_QUIET         9
#define ACKFUSS_RFLAG_PRIVATE       10
#define ACKFUSS_RFLAG_SAFE          11
#define ACKFUSS_RFLAG_SOLITARY      12
#define ACKFUSS_RFLAG_PET_SHOP      13
#define ACKFUSS_RFLAG_NO_RECALL     14
#define ACKFUSS_RFLAG_NO_TELEPORT   15
#define ACKFUSS_RFLAG_HUNT_MARK     16
#define ACKFUSS_RFLAG_NO_BLOODWALK  17
#define ACKFUSS_RFLAG_NO_PORTAL     18
#define ACKFUSS_RFLAG_NO_REPOP      19

#define ACKFUSS_NFLAG_NONE          0
#define ACKFUSS_NFLAG_BOUGHT_PET    1
#define ACKFUSS_NFLAG_SENTINEL      2
#define ACKFUSS_NFLAG_SCAVENGER     3
#define ACKFUSS_NFLAG_REMEMBER      4
#define ACKFUSS_NFLAG_NO_FLEE       5
#define ACKFUSS_NFLAG_AGGRESSIVE    6
#define ACKFUSS_NFLAG_STAY_AREA     7
#define ACKFUSS_NFLAG_WIMPY         8
#define ACKFUSS_NFLAG_PET           9
#define ACKFUSS_NFLAG_TRAIN         10
#define ACKFUSS_NFLAG_PRACTICE      11
#define ACKFUSS_NFLAG_MERCENARY     12
#define ACKFUSS_NFLAG_HEAL          13
#define ACKFUSS_NFLAG_ADAPT         14
#define ACKFUSS_NFLAG_UNDEAD        15
#define ACKFUSS_NFLAG_BANKER        16
#define ACKFUSS_NFLAG_NO_BODY       17
#define ACKFUSS_NFLAG_HUNTER        18
#define ACKFUSS_NFLAG_NO_MIND       19
#define ACKFUSS_NFLAG_POSTMAN       20
#define ACKFUSS_NFLAG_RE_WIELD      21
#define ACKFUSS_NFLAG_RE_EQUIP      22
#define ACKFUSS_NFLAG_INTELLIGENT   23
#define ACKFUSS_NFLAG_VAMPIRE       24
#define ACKFUSS_NFLAG_BREEDER       25
#define ACKFUSS_NFLAG_SOLO          26
#define ACKFUSS_NFLAG_WEREWOLF      27
#define ACKFUSS_NFLAG_MOUNT         28
#define ACKFUSS_NFLAG_NO_BLOOD      29
#define ACKFUSS_NFLAG_NO_QUEST      30
#define ACKFUSS_NFLAG_QUESTMASTER   31
#define ACKFUSS_NFLAG_NO_PRAY       32
#define ACKFUSS_NFLAG_HOLYLIGHT     33
#define ACKFUSS_NFLAG_WIZINVIS      34
#define ACKFUSS_NFLAG_BUILDER       35
#define ACKFUSS_NFLAG_SILENCE       36
#define ACKFUSS_NFLAG_NO_EMOTE      37
#define ACKFUSS_NFLAG_NO_TELL       38
#define ACKFUSS_NFLAG_LOG           39
#define ACKFUSS_NFLAG_DENY          40
#define ACKFUSS_NFLAG_FREEZE        41
#define ACKFUSS_NFLAG_THIEF         42
#define ACKFUSS_NFLAG_KILLER        43
#define ACKFUSS_NFLAG_NO_SUMMON     44
#define ACKFUSS_NFLAG_NO_VISIT      45
/* 46, empty, used to be ACT_INCOG -- unused */
#define ACKFUSS_NFLAG_SHIFTED       47
#define ACKFUSS_NFLAG_RAGED         48
#define ACKFUSS_NFLAG_AFK           49
#define ACKFUSS_NFLAG_PKOK          50
#define ACKFUSS_NFLAG_AMBASSADOR    51
#define ACKFUSS_NFLAG_CDIPLOMAT     52
#define ACKFUSS_NFLAG_CBOSS         53
#define ACKFUSS_NFLAG_CTREASURER    54
#define ACKFUSS_NFLAG_CARMORER      55
#define ACKFUSS_NFLAG_CLEADER       56
#define ACKFUSS_NFLAG_COUNCIL       57
#define ACKFUSS_NFLAG_RULER         58
#define ACKFUSS_NFLAG_BLIND_PLAYER  59
#define ACKFUSS_NFLAG_TESTER        60
#define ACKFUSS_NFLAG_AUTOEXIT      61
#define ACKFUSS_NFLAG_AUTOLOOT      62
#define ACKFUSS_NFLAG_AUTOSAC       63
#define ACKFUSS_NFLAG_BLANK         64
#define ACKFUSS_NFLAG_BRIEF         65
#define ACKFUSS_NFLAG_COMBINE       66
#define ACKFUSS_NFLAG_PROMPT        67
#define ACKFUSS_NFLAG_TELNET_GA     68
#define ACKFUSS_NFLAG_COLOR         69
#define ACKFUSS_NFLAG_AUTOSPLIT     70
#define ACKFUSS_NFLAG_FULL_ANSI     71
#define ACKFUSS_NFLAG_MAPPER        72
#define ACKFUSS_NFLAG_JUSTIFY       73
#define ACKFUSS_NFLAG_AUTODIG       74
#define ACKFUSS_NFLAG_AUTOBRIEF     75
#define ACKFUSS_NFLAG_GHOST         76
#define ACKFUSS_NFLAG_NO_COOLDOWN   77
#define ACKFUSS_NFLAG_SHUNTER       78
#define ACKFUSS_NFLAG_WHITELIST     79

#define ACKFUSS_OFLAG_NONE          0
#define ACKFUSS_OFLAG_GLOW          1
#define ACKFUSS_OFLAG_HUM           2
#define ACKFUSS_OFLAG_NO_DISARM     3
#define ACKFUSS_OFLAG_LOCK          4
#define ACKFUSS_OFLAG_EVIL          5
#define ACKFUSS_OFLAG_INVIS         6
#define ACKFUSS_OFLAG_MAGIC         7
#define ACKFUSS_OFLAG_NO_DROP       8
#define ACKFUSS_OFLAG_BLESS         9
#define ACKFUSS_OFLAG_ANTI_GOOD     10
#define ACKFUSS_OFLAG_ANTI_EVIL     11
#define ACKFUSS_OFLAG_ANTI_NEUTRAL  12
#define ACKFUSS_OFLAG_NO_REMOVE     13
#define ACKFUSS_OFLAG_INVENTORY     14
#define ACKFUSS_OFLAG_NO_SAVE       15
#define ACKFUSS_OFLAG_CLAN_EQ       16
#define ACKFUSS_OFLAG_TRIG_DESTROY  17
#define ACKFUSS_OFLAG_NO_AUCTION    18
#define ACKFUSS_OFLAG_REMORT        19
#define ACKFUSS_OFLAG_ADEPT         20
#define ACKFUSS_OFLAG_RARE          21
#define ACKFUSS_OFLAG_VAMP          22
#define ACKFUSS_OFLAG_NO_LOOT       23
#define ACKFUSS_OFLAG_NO_SAC        24
#define ACKFUSS_OFLAG_UNIQUE        25
#define ACKFUSS_OFLAG_LIFESTEALER   26
#define ACKFUSS_OFLAG_SILVER        27
#define ACKFUSS_OFLAG_NO_QUEST      28
#define ACKFUSS_OFLAG_NO_TAKE       29

#define ACKFUSS_WEAR_NONE           0
#define ACKFUSS_WEAR_HALO           1
#define ACKFUSS_WEAR_AURA           2
#define ACKFUSS_WEAR_HORNS          3
#define ACKFUSS_WEAR_HEAD           4
#define ACKFUSS_WEAR_FACE           5
#define ACKFUSS_WEAR_BEAK           6
#define ACKFUSS_WEAR_EAR            7
#define ACKFUSS_WEAR_NECK           8
#define ACKFUSS_WEAR_WINGS          9
#define ACKFUSS_WEAR_SHOULDERS      10
#define ACKFUSS_WEAR_ARMS           11
#define ACKFUSS_WEAR_WRIST          12
#define ACKFUSS_WEAR_HANDS          13
#define ACKFUSS_WEAR_FINGER         14
#define ACKFUSS_WEAR_CLAWS          15
#define ACKFUSS_WEAR_HOLD_HAND      16
#define ACKFUSS_WEAR_ABOUT          17
#define ACKFUSS_WEAR_WAIST          18
#define ACKFUSS_WEAR_BODY           19
#define ACKFUSS_WEAR_TAIL           20
#define ACKFUSS_WEAR_LEGS           21
#define ACKFUSS_WEAR_FEET           22
#define ACKFUSS_WEAR_HOOVES         23
#define ACKFUSS_WEAR_TAKE           24

#define ACKFUSS_APPLY_NONE          0
#define ACKFUSS_APPLY_STR           1
#define ACKFUSS_APPLY_DEX           2
#define ACKFUSS_APPLY_INT           3
#define ACKFUSS_APPLY_WIS           4
#define ACKFUSS_APPLY_CON           5
#define ACKFUSS_APPLY_SEX           6
#define ACKFUSS_APPLY_CLASS         7
#define ACKFUSS_APPLY_LEVEL         8
#define ACKFUSS_APPLY_AGE           9
#define ACKFUSS_APPLY_HEIGHT        10
#define ACKFUSS_APPLY_WEIGHT        11
#define ACKFUSS_APPLY_MANA          12
#define ACKFUSS_APPLY_HIT           13
#define ACKFUSS_APPLY_MOVE          14
/*15, empty, used to be APPLY_GOLD -- unused */
#define ACKFUSS_APPLY_EXP           16
#define ACKFUSS_APPLY_AC            17
#define ACKFUSS_APPLY_HITROLL       18
#define ACKFUSS_APPLY_DAMROLL       19
#define ACKFUSS_APPLY_SAVING_PARA   20
#define ACKFUSS_APPLY_SAVING_ROD    21
#define ACKFUSS_APPLY_SAVING_PETRI  22
#define ACKFUSS_APPLY_SAVING_BREATH 23
#define ACKFUSS_APPLY_SAVING_SPELL  24

#define ACKFUSS_IAPPLY_NONE         0
/* used to be NONE, as bit 1 -- stupid */
#define ACKFUSS_IAPPLY_INFRA        2
#define ACKFUSS_IAPPLY_INV          4
#define ACKFUSS_IAPPLY_DET_INV      8
#define ACKFUSS_IAPPLY_SANC         16
#define ACKFUSS_IAPPLY_SNEAK        32
#define ACKFUSS_IAPPLY_HIDE         64
#define ACKFUSS_IAPPLY_PROT         128
#define ACKFUSS_IAPPLY_ENHANCED     256
#define ACKFUSS_IAPPLY_DET_MAG      512
#define ACKFUSS_IAPPLY_DET_HID      1024
#define ACKFUSS_IAPPLY_DET_EVIL     2048
#define ACKFUSS_IAPPLY_PASS_DOOR    4096
#define ACKFUSS_IAPPLY_DET_POISON   8192
#define ACKFUSS_IAPPLY_FLY          16384
#define ACKFUSS_IAPPLY_KNOW_ALIGN   32768
#define ACKFUSS_IAPPLY_DET_UNDEAD   65536
#define ACKFUSS_IAPPLY_HEATED       131072
