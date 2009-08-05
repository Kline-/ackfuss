/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Ack 2.2 improvements copyright (C) 1994 by Stephen Dooley              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *       _/          _/_/_/     _/    _/     _/    ACK! MUD is modified    *
 *      _/_/        _/          _/  _/       _/    Merc2.0/2.1/2.2 code    *
 *     _/  _/      _/           _/_/         _/    (c)Stephen Zepp 1998    *
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3          *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_CONFIG_H

/*
 *  Your mud info here :) Zen
 */
#define mudnamecolor  "@@lAckFUSS Dev@@N"
#define mudnamenocolor  "AckFUSS Dev"
#define sacgodname  "Kline"
#define goodgodname  "Good"
#define neutralgodname  "Neutral"
#define evilgodname  "Evil"
#define UPGRADE_REVISION 15

/*
 * String and memory management parameters.
 */
#define MAX_ALLOC_SIZE           262144
#define MAS                      MAX_ALLOC_SIZE
#define MAX_KEY_HASH             2048
#define MKH                      MAX_KEY_HASH
#define MAX_STRING_LENGTH        8192
#define MSL			 MAX_STRING_LENGTH
#define MAX_INPUT_LENGTH         2048
#define MIL                      MAX_INPUT_LENGTH
#define MAX_AREAS                1000
#define MAX_VNUM                 16777216
#define MAX_BITSET               256    /* Total bits per bitset */
#define MAX_PERM_BLOCK           131072
#define COMPRESS_BUF_SIZE        MAX_STRING_LENGTH

#define BOOT_DB_ABORT_THRESHOLD			  25
#define RUNNING_ABORT_THRESHOLD			  10
#define ALARM_FREQUENCY               20

/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.
 */
#define MAX_CLAN_EQ		     6   /* Number of clan eq items */
#define MAX_COLOR		    22   /* eg look, prompt, shout */
#define MAX_ANSI		    28   /* eg red, black, etc (was 11) */
#define MAX_ALIASES		     6
#define MAX_HOSTS                    3   /* number of login sites/whitelist */
#define MAX_IGNORES		     3
#define MAX_RACE                     20
#define MAX_CLAN                     9 /* number of clans */
#define MAX_SKILL                  300
#define MAX_CLASS                    5
#define MAX_LEVEL                   85
#define MOUNT_COST                  10
#define LEVEL_HERO                 (MAX_LEVEL - 4)
#define LEVEL_IMMORTAL             (MAX_LEVEL - 3)
#define MAX_VAMP_LEVEL              20
#define MAX_WOLF_LEVEL		    20
#define ADVANCE_VAMP                16
#define ADVANCE_ADEPT               32
#define ADVANCE_WOLF		    64
#define MAX_FAMILY                   5
#define MAX_TRIBE		     5
#define MAX_SUPER                    4
#define MAX_STANCE		13
#define MAX_TOT_STATS               90   /* Total sum of player stats when creating */
#define MAX_HISTORY                 20   /* History per channel */
#define MAX_MOVE_DISPLAY             5   /* Number of rooms to auto-look while moving before they go brief */
#define MAX_PEDIT                    5
#define MAX_COND                     3
#define MAX_GREETING                 5   /* Number of different login screen files: greeting1, greeting2, etc */
#define MAX_EXP                      2000000000 /* signed int is 2147483647, safely below this limit */
                     /*
                      * total number of supernatural types in the game  
                      */
#define QUORUM_NUMBER			4
                     /*
                      * number of members to be a quorum for a council 
                      */

#define MAX_TRADE        5

/*
 * Liquids.
 */
#define LIQ_WATER        0
#define LIQ_MAX         17

/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_MARTIAL		     -3  /* for martial arts */
#define TYPE_UNDEFINED               -2
#define TYPE_HIT                     1000

/*
 * Lua owner types. --Kline
 */
#define LUA_TYPE_NONE 0
#define LUA_TYPE_CH   1
#define LUA_TYPE_OB   2
#define LUA_TYPE_RM   3

/*
 * For speed combat, hands. --Kline
 */
#define MAX_SPEED  4
#define SPEED_RH   0
#define SPEED_LH   1
#define SPEED_TAIL 2
#define SPEED_FANG 3

/*
 * Melee cooldown system. --Kline
 */
#define MAX_COOLDOWN 2
#define COOLDOWN_NONE -1
#define COOLDOWN_OFF   0
#define COOLDOWN_DEF   1

/*
 * For charm item bonuses. --Kline
 */
#define CHARM_AFF               0
#define CHARM_BONUS             1

#define CHARM_AFF_EXP           0 /* Boost exp while in inv...shhhh */
#define CHARM_AFF_GOLD          1 /* Boost gold drops while in inv...shhhh */
#define CHARM_AFF_BATTLE        2 /* Boost melee dealt / reduce melee taken while in inv...shhhh */
#define CHARM_AFF_MAGE          3 /* Reduce spell cost / incrase spell dmg while in inv...shhhh */
#define CHARM_AFF_REGEN         4 /* Increase regen while in inv...shhhh */
#define CHARM_AFF_HUNGERLESS    5 /* Prevents hunger/thirst while in inv...shhhh */

/*
 *  Target types.
 */
#define TAR_IGNORE                  0
#define TAR_CHAR_OFFENSIVE          1
#define TAR_CHAR_DEFENSIVE          2
#define TAR_CHAR_SELF               3
#define TAR_OBJ_INV                 4

/* Used as flags in skill_table */

#define MORTAL 1
#define REMORT 2
#define ADEPT  3

#define NORM   1
#define VAMP   2
#define WOLF   3

/*
 * Quest mob personalities
 */

#define QUEST_RUDE 	1
#define QUEST_NICE	2
#define QUEST_WIMPY	4
#define QUEST_COOL	8
#define QUEST_MEAN	16

#define C_TYPE_MISC     0
#define C_TYPE_COMM     1
#define C_TYPE_CONFIG   2
#define C_TYPE_INFO     3
#define C_TYPE_ACTION   4
#define C_TYPE_OBJECT   5
#define C_TYPE_IMM      6

#define C_SHOW_NEVER    -1
#define C_SHOW_ALWAYS     0
#define C_SHOW_SKILL      1

/*
 * vnums for quest items.
 * One of these will be picked at random and placed onto a random mob
 * (one the player has a chance of killing!).
 * We'll define a range of possible vnums here - easier to expand (:
 * The area questitems.are (vnum 101-200) is reserved for this use.
 * As you add more items (for more variety!) incrase the max on this range.
 */

#define OBJ_VNUM_QUEST_MIN		101
#define OBJ_VNUM_QUEST_MAX		110

/*
 * vnums for deathmatch arena.
 * Upon starting a DM, all players will be transfered somewhere
 * randomly within this range. Make sure it exists!
 *
 */
#define DM_ARENA_START    201
#define DM_ARENA_END      300

#define PULSE_PER_SECOND          sysdata.pulse /* Configurable in-game, essential to speed based combat. --Kline */
#define PULSE_COMBAT              (  1                     )
#define PULSE_VIOLENCE            (  2  * PULSE_PER_SECOND )
#define PULSE_MOBILE              (  4  * PULSE_PER_SECOND )
#define PULSE_OBJFUN		  (  4  * PULSE_PER_SECOND )
#define PULSE_TICK                ( 60  * PULSE_PER_SECOND )
#define PULSE_MESSAGE		  ( 12  * PULSE_PER_SECOND )
#define PULSE_ROOMS		  ( 30  * PULSE_PER_SECOND )
#define PULSE_AREA                ( 80  * PULSE_PER_SECOND )
#define PULSE_AUCTION		  ( 30  * PULSE_PER_SECOND )
#define PULSE_RAUCTION            ( 30  * PULSE_PER_SECOND )
#define PULSE_MAUCTION            ( 30  * PULSE_PER_SECOND )


#define VAMPIRE_RECALL		 9001
#define LIQUID_BLOOD		 13
#define WOLF_RECALL          3001

/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
#define OBJ_VNUM_MONEY_ONE            1
#define OBJ_VNUM_MONEY_SOME           2
#define OBJ_VNUM_CORPSE_NPC           3
#define OBJ_VNUM_CORPSE_PC            4
#define OBJ_VNUM_SEVERED_HEAD         5
#define OBJ_VNUM_TORN_HEART           6
#define OBJ_VNUM_SLICED_ARM           7
#define OBJ_VNUM_SLICED_LEG           8
#define OBJ_VNUM_BRAIN                9
#define OBJ_VNUM_MUSHROOM            10
#define OBJ_VNUM_LIGHT_BALL          11
#define OBJ_VNUM_SPRING              12
#define OBJ_VNUM_WINDOW              13
#define OBJ_VNUM_FIREBLADE           14
#define OBJ_VNUM_FOOD		     15
#define OBJ_VNUM_BEACON              16
#define OBJ_VNUM_PORTAL		     17
#define OBJ_VNUM_SOUL_POTION         18
#define OBJ_VNUM_CAPTURED_SOUL       19
#define OBJ_VNUM_CONFLAGRATION	     20
#define OBJ_VNUM_MINO_PASS           21

/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_LIMBO            1
#define ROOM_VNUM_JAIL             2
#define ROOM_VNUM_BODIES           3    /* for morphed players */
#define ROOM_VNUM_CHAT             301
#define ROOM_VNUM_HALLS            302
#define ROOM_VNUM_ETHEREAL_PLANE   401
#define ROOM_VNUM_TEMPLE           1001
#define ROOM_VNUM_ALTAR            1002
#define ROOM_VNUM_ARMOR_DONATE     1012
#define ROOM_VNUM_WEAPON_DONATE    1013
#define ROOM_VNUM_MISC_DONATE      1014
#define ROOM_VNUM_SCHOOL           1051
#define ROOM_VNUM_BUILDER	   1019  /* Bulder-flagged players only */
#define ROOM_VNUM_CLAN		   1022  /* for leaders only */
#define ROOM_VNUM_DM_RECALL        ROOM_VNUM_ALTAR
#define ROOM_VNUM_INT_HEAL	   ROOM_VNUM_ALTAR

/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_STALKER           2
#define MOB_VNUM_CITYGUARD         3060
#define MOB_VNUM_VAMPIRE           3404
#define MOB_VNUM_DOGGY             99
#define MOB_VNUM_ZOMBIE           11004
#define MOB_VNUM_WATERELEM        1028
#define MOB_VNUM_SKELETON         1028
#define MOB_VNUM_FIREELEM         1028
#define MOB_VNUM_TRAINER                1042
#define MOB_VNUM_EARTHELEM              1028
#define MOB_VNUM_IRON_GOLEM             1029
#define MOB_VNUM_SOUL_THIEF             1030
#define MOB_VNUM_HOLY_AVENGER           1031
#define MOB_VNUM_PEGASUS                1037
#define MOB_VNUM_NIGHTMARE              1038
#define MOB_VNUM_ELEM_BEAST             1039
#define MOB_VNUM_INT_DEVOURER           1040
#define MOB_VNUM_SHADOW_HOUND           1041
#define MOB_VNUM_DIAMOND_GOLEM  1032
#define MOB_VNUM_COMBAT_ELEMENTAL 1036 /* this needs to be a new mob ZENFIX */

#define STANCE_WARRIOR		0
#define STANCE_CASTER		1
#define STANCE_AMBUSH		2
#define STANCE_AC_BEST		3
#define STANCE_HR_BEST		4
#define STANCE_DR_BEST		5
#define STANCE_AC_WORST		6
#define STANCE_HR_WORST		7
#define STANCE_DR_WORST		8
#define STANCE_SUPER_FIGHTER	9
#define STANCE_SUPER_SPEED	10
#define STANCE_WIZARD           11
#define STANCE_MAGI             12

#define SUPER_NONE   0
#define SUPER_VAMP   1
#define SUPER_WOLF   2
#define SUPER_UNDEAD 3
#define SUPER_HUNTER 4

/* defines for enchant.c  */
#define ENCHANT_EXTRA_FLAGS	-1
#define ENCHANT_APPLY_FLAGS	-2
#define ENCHANT_OBJFUNS		-3

#define RACE_MOD_NONE		BIT_0
#define RACE_MOD_FAST_HEAL	BIT_1
#define RACE_MOD_SLOW_HEAL	BIT_2
#define RACE_MOD_STRONG_MAGIC	BIT_3
#define RACE_MOD_WEAK_MAGIC	BIT_4
#define RACE_MOD_NO_MAGIC	BIT_5
#define RACE_MOD_IMMUNE_POISON  BIT_6
#define RACE_MOD_RESIST_SPELL	BIT_7
#define RACE_MOD_WOODLAND	BIT_8
#define RACE_MOD_DARKNESS	BIT_9
#define RACE_MOD_HUGE		BIT_10
#define RACE_MOD_LARGE		BIT_11
#define RACE_MOD_TINY		BIT_12
#define RACE_MOD_SMALL		BIT_13
#define RACE_MOD_TAIL		BIT_14
#define RACE_MOD_TOUGH_SKIN   BIT_15
#define RACE_MOD_STONE_SKIN   BIT_16
#define RACE_MOD_IRON_SKIN    BIT_17
#define RACE_MOD_FAST_CAST    BIT_18

/*
 *   Spell Damage Types
 */


/* Magic Shield types
	-Damane 4/27/96 */
#define SHIELD_NONE	0
#define FLAME_SHIELD	1
#define SHOCK_SHIELD	2
#define SHADOW_SHIELD	3
#define ICE_SHIELD	4
#define PSI_SHIELD	5


/* These are for skill_table lookup funcs... to save writing 2 functions */
#define RETURN_BEST_LEVEL	1
#define RETURN_BEST_CLASS	2

/*
 * God Levels
 */
#define L_GOD           MAX_LEVEL
#define L_SUP           L_GOD - 1
#define L_DEI           L_SUP - 1
#define L_ANG           L_DEI - 1
#define L_HER           L_ANG - 1

#define CLAN_ONLY	 -1
#define BOSS_ONLY	 -2
#define VAMP_ONLY	 -3
#define WOLF_ONLY	 -4

/*
 * Time and weather stuff.
 */
#define SUN_DARK                    0
#define SUN_RISE                    1
#define SUN_LIGHT                   2
#define SUN_SET                     3

#define SKY_CLOUDLESS               0
#define SKY_CLOUDY                  1
#define SKY_RAINING                 2
#define SKY_LIGHTNING               3

#define MOON_DOWN	0
#define MOON_RISE	1
#define MOON_LOW	2
#define MOON_PEAK	3
#define MOON_FALL	4
#define MOON_SET	5

#define MOON_NEW	0
#define MOON_WAX_CRE	1
#define MOON_WAX_HALF	2
#define MOON_WAX_GIB	3
#define MOON_FULL	4
#define MOON_WAN_GIB	5
#define MOON_WAN_HALF	6
#define MOON_WAN_CRE	7

/*
 * Connected state for a channel.
 */

/* These values referenced by users command, BTW */

#define CON_SETTING_STATS                1
#define CON_PLAYING                      0
#define CON_GET_NAME                     -1
                  /*
                   * All non connected 
                   */
#define CON_GET_OLD_PASSWORD             -2
                  /*
                   * states changed to  
                   */
#define CON_CONFIRM_NEW_NAME             -3
                  /*
                   * negative so that   
                   */
#define CON_GET_NEW_PASSWORD             -4
                  /*
                   * Hotboot can shut out 
                   */
#define CON_CONFIRM_NEW_PASSWORD         -5
                  /*
                   * All nonplaying chars 
                   */
#define CON_GET_NEW_SEX                  -6
                  /*
                   * ---Flar 
                   */
#define CON_GET_NEW_CLASS                -7
#define CON_GET_RACE                     -9
#define CON_READ_MOTD                   -10
#define CON_GET_STATS                   -11
#define CON_FINISHED			-12
#define CON_MENU		        -13
#define CON_COPYOVER_RECOVER		-14
                  /*
                   * For Hotreboot 
                   */
#define CON_QUITTING			-15
#define CON_RECONNECTING                -16

/* values used to check a new player has selected all options.... */
#define CHECK_RACE			  BIT_1
#define CHECK_CLASS			  BIT_2
#define CHECK_SEX			  BIT_3
#define CHECK_STATS			  BIT_4

/*
 * TO types for act.
 */
#define TO_ROOM             0
#define TO_NOTVICT          1
#define TO_VICT             2
#define TO_CHAR             3

/*
 * Area flags.
 * Used in #AREA
 */
#define AFLAG_NONE           0
#define AFLAG_PAYAREA        1
#define AFLAG_TELEPORT       2
#define AFLAG_BUILDING       3
#define AFLAG_NOSHOW         4 /* Show area name on 'areas' command? */
#define AFLAG_NO_ROOM_AFF    5
#define AFLAG_NO_QUEST       6

/*
 * Room flags.
 * Used in #ROOMS.
 */
#define RFLAG_NONE          0   /* durrr...                     */
#define RFLAG_DARK          1   /* room is dark, need a light   */
#define RFLAG_REGEN         2   /* increased regen in this room */
#define RFLAG_NO_MOB        3   /* NPCs can't enter             */
#define RFLAG_INDOORS       4   /* indoors, no weather          */
#define RFLAG_NO_MAGIC      5   /* no casting                   */
#define RFLAG_HOT           6   /* players lose hp each tick    */
#define RFLAG_COLD          7   /* players lose hp each tick    */
#define RFLAG_PK            8   /* players may freely pk here   */
#define RFLAG_QUIET         9   /* room is quiet                */
#define RFLAG_PRIVATE       10  /* 2 pc/npc maximum             */
#define RFLAG_SAFE          11  /* no fighting                  */
#define RFLAG_SOLITARY      12  /* 1 pc/npc maximum             */
#define RFLAG_PET_SHOP      13  /* respawn mobs are pets        */
#define RFLAG_NO_RECALL     14  /* no recalling                 */
#define RFLAG_NO_TELEPORT   15  /* can't teleport in/out of     */
#define RFLAG_HUNT_MARK	    16  /* Dont ever set!               */
#define RFLAG_NO_BLOODWALK  17  /* vamps can't bloodwalk in     */
#define RFLAG_NO_PORTAL     18  /* portals will collapse on use */
#define RFLAG_NO_REPOP      19  /* objs only spawn if no chars  */

/*
 * Directions.
 * Used in #DOOR.
 */
#define DIR_NORTH                     0
#define DIR_EAST                      1
#define DIR_SOUTH                     2
#define DIR_WEST                      3
#define DIR_UP                        4
#define DIR_DOWN                      5
#define MAX_DIR                       6

/*
 * Exit flags.
 * Used in #DOOR.
 */
#define EX_NONE                       0  /* durrrr                         */
#define EX_ISDOOR                     1  /* must be set to set other flags */
#define EX_CLOSED                     2  /* auto-close on repop            */
#define EX_LOCKED                     3  /* auto-lock on repop             */
#define EX_CLIMB		      4  /* must have climb/fly to enter   */
#define EX_IMMORTAL		      5  /* must be immortal to enter      */
#define EX_PICKPROOF                  6  /* can not pick lock              */
#define EX_SMASHPROOF		      7  /* can not smash                  */
#define EX_PASSPROOF		      8  /* can not pass door              */
#define EX_NODETECT		      9
#define EX_NOMOB                     10

/*
 * Sector types.
 * Used in #ROOM
 */
#define SECT_NULL                     0
#define SECT_CITY                     1
#define SECT_FIELD                    2
#define SECT_FOREST                   3
#define SECT_HILLS                    4
#define SECT_MOUNTAIN                 5
#define SECT_WATER_SWIM               6
#define SECT_WATER_NOSWIM             7
#define SECT_RECALL_OK                8
#define SECT_AIR                      9
#define SECT_DESERT                  10
#define SECT_INSIDE                  11

#define SECT_MAX                     12



/*
 * Equpiment wear locations.
 * Used in #RESETS.
 */
#define WEAR_NONE               -1
#define WEAR_LIGHT              0
#define WEAR_HALO               1
#define WEAR_AURA               2
#define WEAR_HORNS              3
#define WEAR_HEAD               4
#define WEAR_FACE               5
#define WEAR_BEAK               6
#define WEAR_EAR_L              7
#define WEAR_EAR_R              8
#define WEAR_NECK_1             9
#define WEAR_NECK_2             10
#define WEAR_WINGS              11
#define WEAR_SHOULDERS          12
#define WEAR_ARMS               13
#define WEAR_WRIST_L            14
#define WEAR_WRIST_R            15
#define WEAR_HANDS              16
#define WEAR_FINGER_L           17
#define WEAR_FINGER_R           18
#define WEAR_CLAWS              19
#define WEAR_HOLD_HAND_L        20
#define WEAR_HOLD_HAND_R        21
#define WEAR_ABOUT              22
#define WEAR_WAIST              23
#define WEAR_BODY               24
#define WEAR_TAIL               25
#define WEAR_LEGS               26
#define WEAR_FEET               27
#define WEAR_HOOVES             28

#define MAX_WEAR                     29


/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (End of this section ... stop here)                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Conditions.
 */
#define COND_DRUNK                    0
#define COND_FULL                     1
#define COND_THIRST                   2



/*
 * Positions.
 */
#define POS_DEAD                      0
#define POS_MORTAL                    1
#define POS_INCAP                     2
#define POS_STUNNED                   3
#define POS_SLEEPING                  4
#define POS_RESTING                   5
#define POS_FIGHTING                  6
#define POS_STANDING                  7
#define POS_WRITING                   8
#define POS_BUILDING                  9
#define POS_RIDING                   10

/* 
 * ACT bits for players and mobs. Uses a bitset.
 * I realize this could be an enum, but this allows comments next
 * to each define and, imo, is easier to read if you add/remove one.
 * --Kline
 */
#define ACT_NONE         0    /* Empty, don't use               */
#define ACT_BOUGHT_PET   1    /* Player bought a pet this level */
#define ACT_SENTINEL     2    /* Stays in one room              */
#define ACT_SCAVENGER    3    /* Picks up objects               */
#define ACT_REMEMBER     4    /* Remembers target               */
#define ACT_NO_FLEE      5    /* Can't flee from mob            */
#define ACT_AGGRESSIVE   6    /* Attacks PC's                   */
#define ACT_STAY_AREA    7    /* Won't leave area               */
#define ACT_WIMPY        8    /* Flees when hurt                */
#define ACT_PET          9    /* Auto set for pets              */
#define ACT_TRAIN        10   /* Can train PC's                 */
#define ACT_PRACTICE     11   /* Can practice PC's              */
#define ACT_MERCENARY    12   /* Is a mercenary                 */
#define ACT_HEAL         13   /* Sells spells                   */
#define ACT_ADAPT        14   /* Adapts weapons                 */
#define ACT_UNDEAD       15   /* Undead...spooky                */
#define ACT_BANKER       16   /* Is a *B*anker :P               */
#define ACT_NO_BODY      17   /* No body for damage             */
#define ACT_HUNTER       18   /* Hunts attackers                */
#define ACT_NO_MIND      19   /* Psi attack no-no               */
#define ACT_POSTMAN      20   /* Postmaster                     */
#define ACT_RE_WIELD     21   /* Uses better weapons            */
#define ACT_RE_EQUIP     22   /* Uses better equipment          */
#define ACT_INTELLIGENT  23   /* For shitkicking mobs           */
#define ACT_VAMPIRE      24   /* Vampire                        */
#define ACT_BREEDER      25   /* Mob can breed                  */
#define ACT_SOLO   	 26   /* Buffed for solo combat         */
#define ACT_WEREWOLF     27   /* Werewolf                       */
#define ACT_MOUNT        28   /* Mountable MOB                  */
#define ACT_NO_BLOOD     29   /* No bloodwalk                   */
#define ACT_NO_QUEST     30   /* Disable from mquest            */
#define ACT_QUESTMASTER  31   /* Questmaster for mquest         */
#define ACT_NO_PRAY      32   /* Can't use pray                 */
#define ACT_HOLYLIGHT    33   /* Can see everything             */
#define ACT_WIZINVIS     34   /* Invisible to lower levels      */
#define ACT_BUILDER      35   /* Is able to use the OLC         */
#define ACT_SILENCE      36   /* Can't use channels             */
#define ACT_NO_EMOTE     37   /* Can't use emotes/socials       */
#define ACT_NO_TELL      38   /* Can't use tell                 */
#define ACT_LOG          39   /* All actions are logged         */
#define ACT_DENY         40   /* Can't login                    */
#define ACT_FREEZE       41   /* Frozen in place, no commands   */
#define ACT_THIEF        42   /* Thief! Wanted!                 */
#define ACT_KILLER       43   /* Pker! Wanted!                  */
#define ACT_NO_SUMMON    44   /* Doesn't want to be summoned    */
#define ACT_NO_VISIT     45   /* Doesn't want to be visited     */
/* 46, empty, used to be ACT_INCOG -- unused                    */
#define ACT_SHIFTED      47   /* Wolf-form Werewolf             */
#define ACT_RAGED        48   /* Enraged Werewolf               */
#define ACT_AFK          49   /* AFK -- duh? :P                 */
#define ACT_PKOK         50   /* Full PKer                      */
#define ACT_AMBASSADOR   51   /* Newbie helper mortals          */
#define ACT_CDIPLOMAT    52   /* Clan Diplomat                  */
#define ACT_CBOSS        53   /* Clan Boss                      */
#define ACT_CTREASURER   54   /* Clan Treasurer                 */
#define ACT_CARMORER     55   /* Clan Armorer                   */
#define ACT_CLEADER      56   /* Clan Leader                    */
#define ACT_COUNCIL      57   /* Super council member           */
#define ACT_RULER        58   /* Zone ruler                     */
#define ACT_BLIND_PLAYER 59   /* IRL blind, special output      */
#define ACT_TESTER       60   /* Play testers                   */
#define ACT_AUTOEXIT     61   /* Automatically show exits       */
#define ACT_AUTOLOOT     62   /* Automatically loot corpses     */
#define ACT_AUTOSAC      63   /* Automatically sac corpses      */
#define ACT_BLANK        64   /* Blank line before prompt       */
#define ACT_BRIEF        65   /* Hide room descriptions         */
#define ACT_COMBINE      66   /* Combine like items (3) etc     */
#define ACT_PROMPT       67   /* Show a prompt                  */
#define ACT_TELNET_GA    68   /* Use Telnet GA sequences        */
#define ACT_COLOR        69   /* CoLoR!                         */
#define ACT_AUTOSPLIT    70   /* Automatically split gold       */
#define ACT_FULL_ANSI    71   /* Use full ANSI controls         */
#define ACT_MAPPER       72   /* Display the automapper         */
#define ACT_JUSTIFY      73   /* Justify text so it's prettier  */
#define ACT_AUTODIG      74   /* Automatically dig in builder   */
#define ACT_AUTOBRIEF    75   /* Hide room descrs on speedwalk  */
#define ACT_GHOST        76   /* You're dead! A ghost!          */
#define ACT_NO_COOLDOWN  77   /* suppress showing cooldown msgs */
#define ACT_SHUNTER      78   /* vamp/wolf 'hunter' super race  */
#define ACT_WHITELIST    79   /* utilize login whitelist        */

/*
 * Channel bits.
 */
#define CHANNEL_NONE       0
#define CHANNEL_HERMIT     1 /* All channels are off */
#define CHANNEL_AUCTION    2
#define CHANNEL_GOSSIP     3
#define CHANNEL_MUSIC      4
#define CHANNEL_IMMTALK    5
#define CHANNEL_NEWBIE     6
#define CHANNEL_QUESTION   7
#define CHANNEL_SHOUT      8
#define CHANNEL_YELL       9
#define CHANNEL_FLAME      10
#define CHANNEL_ZZZ        11
#define CHANNEL_RACE       12
#define CHANNEL_CLAN       13
#define CHANNEL_NOTIFY     14
#define CHANNEL_INFO       15
#define CHANNEL_LOG        16
#define CHANNEL_CREATOR    17
#define CHANNEL_ALLCLAN    18
#define CHANNEL_ALLRACE    19
#define CHANNEL_BEEP       20
#define CHANNEL_FAMILY     21
#define CHANNEL_DIPLOMAT   22
#define CHANNEL_CRUSADE    23
#define CHANNEL_REMORTTALK 24
#define CHANNEL_HOWL       25
#define CHANNEL_ADEPT      26
#define CHANNEL_OOC        27
#define CHANNEL_QUEST      28
#define CHANNEL_GAME       29

/* 
 * Monitor channels - for imms to select what mud-based info they receive
 */
#define MONITOR_NONE         0
#define MONITOR_CONNECT      1
#define MONITOR_AREA_UPDATE  2
#define MONITOR_AREA_BUGS    3
#define MONITOR_AREA_SAVING  4
#define MONITOR_GEN_IMM      5
#define MONITOR_GEN_MORT     6
#define MONITOR_COMBAT       7
#define MONITOR_HUNTING      8
#define MONITOR_BUILD        9
#define MONITOR_CLAN         10
#define MONITOR_OBJ          11
#define MONITOR_MOB          12
#define MONITOR_ROOM         13
#define MONITOR_MAGIC        14
#define MONITOR_BAD          15
#define MONITOR_DEBUG        16
#define MONITOR_IMC          17
#define MONITOR_SYSTEM       18
#define MONITOR_HELPS        19
#define MONITOR_LOG          20
#define MONITOR_TYPO         21
#define MONITOR_IDEA         22
#define MONITOR_BUG          23

/*
 * Hunt flags for mobs
 */
#define HUNT_WORLD	0x00000001  /* Search the whole world  */
#define HUNT_OPENDOOR	0x00000002  /* Can open obstructung doors */
#define HUNT_UNLOCKDOOR	0x00000004  /* Can unlock obstructing doors  */
#define HUNT_PICKDOOR	0x00000008  /* Can pick obstructing doors */
#define HUNT_INFORM	0x00000010  /* Yells while hunting     */
#define HUNT_CR		0x00000020  /* Is preforming a CR      */
#define HUNT_MERC	0x00000040  /* Is gonna assassinate someone  */
#define HUNT_ALL	0x0000001E  /* can hunt through anything    */

/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (Start of section ... start here)                     *
 *                                                                         *
 ***************************************************************************/


/********************* Define Flags for hunting *************************/
#define ACT_HUNT_CHAR		1  /* Hunting a character     */
#define ACT_HUNT_OBJ		2  /* Hunting an object       */
#define ACT_HUNT_INFORM		4  /* Mob will gossip when hunting  */
#define ACT_HUNT_CR		8  /* Mob is doing a corpse retrival   */
#define ACT_HUNT_MOVE	       16   /* Just walking somewhere      */

#define NO_VNUM		       -1   /* For ch->move_to thingy      */

/************************************************************************/

/** Define what is calling the trigger handler for objects ******/
#define TRIGGER_OPEN		1
              /*
               * Opening a container      
               */
#define TRIGGER_CLOSE		2
              /*
               * Closing a container   
               */
#define TRIGGER_EAT		3
              /*
               * Eating some food      
               */
#define TRIGGER_C_DRINK		4
              /*
               * Drinking from container  
               */
#define TRIGGER_F_DRINK		5
              /*
               * Drinking from fountain   
               */
#define TRIGGER_WEAR		6
              /*
               * Wearing an object     
               */
#define TRIGGER_GET		7
              /*
               * Picking up an object  
               */
#define TRIGGER_DROP		8
              /*
               * Dropping an object    
               */
#define TRIGGER_EXAMINE		9
              /*
               * Examining an object   
               */
/** Define what actions the triggered object can do *************/
#define ACTION_TRANSPORT	1
              /*
               * Transports victim to room   
               */
#define ACTION_RESTORE		2
              /*
               * Restores victim    
               */
#define ACTION_SLAY		3
              /*
               * Kills victim    
               */
#define ACTION_TRANSFORM	4
              /*
               * Loads mob(s) in its place   
               */
/****************************************************************/

/* BitVector flags for room-affect spells.  These are used in the
   ROOM_AFFECT_DATA structure, and passes to the major handling
   functions.  They MUST be used whenever a room-affect spell is
   being cast (from within code) */

#define ROOM_BV_NONE		0
#define ROOM_BV_SILENCE		1  /* Like silence room flag  */
#define ROOM_BV_SAFE		2  /* Like safe room flag     */
#define ROOM_BV_ENCAPS		4  /* Magically blocks exits   */
#define ROOM_BV_SHADE		8  /* Room is darkened      */
#define ROOM_BV_HEAL_REGEN    16 /* room heals hits quicker     */
#define ROOM_BV_HEAL_STEAL    32 /* room takes hits instead of giving */
#define ROOM_BV_MANA_REGEN    64 /* room heals mana quicker     */
#define ROOM_BV_MANA_STEAL    128   /* room saps mana              */
#define ROOM_BV_FIRE_RUNE     256   /* room does fire damage       */
#define ROOM_BV_FIRE_TRAP     512   /* room is fire trapped        */
#define ROOM_BV_DAMAGE_TRAP   1024  /* room is physical damage trapped */
#define ROOM_BV_SHOCK_RUNE    2048  /* room is shock runed         */
#define ROOM_BV_SHOCK_TRAP    4096  /* room is shock trapped       */
#define ROOM_BV_SPELL_ON_ENTER 8192 /* room spell cast on entrance */
#define ROOM_BV_SPELL_ALWAYS   16384   /* room casts spell continuously */
#define ROOM_BV_HOLD           32768   /* room lets you in, but not out..recall works */
#define ROOM_BV_POISON_RUNE    65536   /* room gives poison to entering ch */
#define ROOM_BV_SOUL_NET	131072   /* makes soul instead of corpse */

/* OLC modes */
#define BUILD_MODE_NONE                0   /* not doing anything   */
#define BUILD_MODE_REDIT               1   /* editing rooms        */
#define BUILD_MODE_OEDIT               2   /* editing objects      */
#define BUILD_MODE_MEDIT               3   /* editing mobiles      */

#define NO_USE			   -999  /* this table entry can */
                  /*
                   * NOT be used, except
                   */
                  /*
                   * by a Creator
                   */


/*
 * New bits to determine what skills a mob can do in combat -S-
 */

#define MOB_NONE          BIT_1
#define MOB_REFLEXES      BIT_2
#define MOB_SLEIGHT       BIT_3
#define MOB_CRUSHING      BIT_4
#define MOB_PUNCH         BIT_5
#define MOB_HEADBUTT      BIT_6
#define MOB_KNEE          BIT_7
#define MOB_DISARM        BIT_8
#define MOB_TRIP          BIT_9
#define MOB_NODISARM      BIT_10
#define MOB_NOTRIP        BIT_11
#define MOB_DODGE         BIT_12
#define MOB_PARRY         BIT_13
#define MOB_MARTIAL       BIT_14
#define MOB_ENHANCED      BIT_15
#define MOB_DUALWIELD     BIT_16
#define MOB_DIRT          BIT_17
#define MOB_PROWESS       BIT_18
#define MOB_QUICKSTRIKE   BIT_19
#define MOB_CHARGE        BIT_20

/*
 * New bits to determine what spells a mob will cast in combat -S-
 * These are for offensive spells.
 * Remember spec_funs may still cast as well! - but spells cast 
 *  this way WILL reduce the mob's mana <g>
 */

#define CAST_NONE			     1
#define CAST_MAGIC_MISSILE		     2
#define CAST_SHOCKING_GRASP		     4
#define CAST_BURNING_HANDS		     8
#define CAST_COLOR_SPRAY		    16
#define CAST_FIREBALL			    32
#define CAST_HELLSPAWN			    64
#define CAST_ACID_BLAST			   128
#define CAST_CHAIN_LIGHTNING		   256
#define CAST_FLARE			  1024
#define CAST_FLAMESTRIKE		  2048
#define CAST_EARTHQUAKE			  4096
#define CAST_MIND_FLAIL			  8192
#define CAST_PLANERGY			 16384
#define CAST_PHOBIA			 32768
#define CAST_MIND_BOLT			 65536
#define CAST_STATIC			131072
#define CAST_EGO_WHIP			262144
#define CAST_BLOODY_TEARS		524288
#define CAST_MINDFLAME		       1048576
#define CAST_SUFFOCATE		       2097152
#define CAST_NERVE_FIRE		       4194304
#define CAST_LIGHTNING_BOLT	       8388608
#define CAST_HEAT_ARMOR			16777216
#define CAST_LAVA_BURST			33554432

/*
 * New bits to determine the defensive spells available to
 * mobs.  May be used in fights (cure light, heal, etc)
 *  will deduct mana from the mob when cast.
 */

#define DEF_NONE		1
#define DEF_CURE_LIGHT		2
#define DEF_CURE_SERIOUS	4
#define DEF_CURE_CRITIC		8
#define DEF_CURE_HEAL		16
#define DEF_SHIELD_FIRE		32
#define DEF_SHIELD_ICE		64
#define DEF_SHIELD_SHOCK	128

/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 */
#define AFF_BLIND                     1
#define AFF_INVISIBLE                 2
#define AFF_DETECT_EVIL               4
#define AFF_DETECT_INVIS              8
#define AFF_DETECT_MAGIC             16
#define AFF_DETECT_HIDDEN            32
#define AFF_CLOAK_REFLECTION         64
#define AFF_SANCTUARY               128
#define AFF_FAERIE_FIRE             256
#define AFF_INFRARED                512
#define AFF_CURSE                  1024
#define AFF_CLOAK_FLAMING          2048
#define AFF_POISON                 4096
#define AFF_PROTECT                8192
#define AFF_CLOAK_ABSORPTION      16384
#define AFF_SNEAK                 32768
#define AFF_HIDE                  65536
#define AFF_SLEEP                131072
#define AFF_CHARM                262144
#define AFF_FLYING               524288
#define AFF_PASS_DOOR           1048576
#define AFF_ANTI_MAGIC          2097152   /* -S- no magic hurts ch */
#define AFF_BLASTED              BIT_23
/* available 
#define AFF_                     BIT_24
*/
#define AFF_CONFUSED           BIT_25
#define AFF_VAMP_HEALING       BIT_26  /* DO NOT USE IN OLC! */
#define AFF_HOLD			         BIT_27
#define AFF_PARALYSIS	         BIT_28
#define AFF_CLOAK_ADEPT		     BIT_29
#define AFF_CLOAK_REGEN		     BIT_30

/* VAMP_HEALING is used to show that a vamp is currently healing after death*/

/*
 * Sex.
 * Used in #MOBILES.
 */
#define SEX_NEUTRAL                   0
#define SEX_MALE                      1
#define SEX_FEMALE                    2


/*
 * Class defines. I hate magic numbers! --Kline
 */
#define CLS_MAG 0
#define CLS_CLE 1
#define CLS_THI 2
#define CLS_WAR 3
#define CLS_PSI 4

#define CLS_SOR 0
#define CLS_MON 1
#define CLS_ASS 2
#define CLS_KNI 3
#define CLS_NEC 4

/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT                    1
#define ITEM_SCROLL                   2
#define ITEM_WAND                     3
#define ITEM_STAFF                    4
#define ITEM_WEAPON                   5
#define ITEM_BEACON                   6   /* for portals, etc */
#define ITEM_PORTAL		      7
#define ITEM_TREASURE                 8
#define ITEM_ARMOR                    9
#define ITEM_POTION                  10
#define ITEM_CLUTCH                  11   /* for clutch location only! */
#define ITEM_FURNITURE               12
#define ITEM_TRASH                   13
#define ITEM_TRIGGER		     14
#define ITEM_CONTAINER               15
#define ITEM_QUEST		     16
#define ITEM_DRINK_CON               17
#define ITEM_KEY                     18
#define ITEM_FOOD                    19
#define ITEM_MONEY                   20
#define ITEM_STAKE		     21
#define ITEM_BOAT                    22
#define ITEM_CORPSE_NPC              23
#define ITEM_CORPSE_PC               24
#define ITEM_FOUNTAIN                25
#define ITEM_PILL                    26
#define ITEM_BOARD                   27
#define ITEM_SOUL		     28
#define ITEM_PIECE		     29
#define ITEM_SPELL_MATRIX	     30
#define ITEM_ENCHANTMENT	     31
#define ITEM_WRAPPED_PRESENT         32
#define ITEM_CHARM                   33
#define ITEM_ANVIL                   34
#define ITEM_COOKING_FIRE            35
#define ITEM_ALCHEMY_LAB             36
#define ITEM_FORGE                   37
#define ITEM_LOOM                    38

/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_EXTRA_NONE                 0
#define ITEM_EXTRA_GLOW			1
#define ITEM_EXTRA_HUM			2
#define ITEM_EXTRA_NO_DISARM		3
#define ITEM_EXTRA_LOCK			4
#define ITEM_EXTRA_EVIL			5
#define ITEM_EXTRA_INVIS		6
#define ITEM_EXTRA_MAGIC		7
#define ITEM_EXTRA_NO_DROP		8
#define ITEM_EXTRA_BLESS		9
#define ITEM_EXTRA_ANTI_GOOD		10
#define ITEM_EXTRA_ANTI_EVIL		11
#define ITEM_EXTRA_ANTI_NEUTRAL		12
#define ITEM_EXTRA_NO_REMOVE		13
#define ITEM_EXTRA_INVENTORY		14
#define ITEM_EXTRA_NO_SAVE		15 /* For "quest" items :) */
#define ITEM_EXTRA_CLAN_EQ		16 /* only leaders and creators may load */
#define ITEM_EXTRA_TRIG_DESTROY	 	17 /* Trigger object that destroy after use */
#define ITEM_EXTRA_NO_AUCTION		18   /* Can't be auctioned */
#define ITEM_EXTRA_REMORT		19   /* player must have a remort class of higher level than item level */
#define ITEM_EXTRA_ADEPT		20
#define ITEM_EXTRA_RARE			21
#define ITEM_EXTRA_VAMP                 22 /* player must be a vampire */
#define ITEM_EXTRA_NO_LOOT		23
#define ITEM_EXTRA_NO_SAC		24
#define ITEM_EXTRA_UNIQUE		25
#define ITEM_EXTRA_LIFESTEALER	        26
#define ITEM_EXTRA_SILVER		27
#define ITEM_EXTRA_NO_QUEST             28 /* disable for mquest */
#define ITEM_EXTRA_NO_TAKE              29 /* can't get this item */

/* Class-restricted stuff dropped. */
#define ITEM_APPLY_NONE                 1
#define ITEM_APPLY_INFRA                2
#define ITEM_APPLY_INV                  4
#define ITEM_APPLY_DET_INV              8
#define ITEM_APPLY_SANC                16
#define ITEM_APPLY_SNEAK               32
#define ITEM_APPLY_HIDE                64
#define ITEM_APPLY_PROT               128
#define ITEM_APPLY_ENHANCED           256
#define ITEM_APPLY_DET_MAG            512
#define ITEM_APPLY_DET_HID           1024
#define ITEM_APPLY_DET_EVIL          2048
#define ITEM_APPLY_PASS_DOOR         4096
#define ITEM_APPLY_DET_POISON        8192
#define ITEM_APPLY_FLY              16384
#define ITEM_APPLY_KNOW_ALIGN       32768
#define ITEM_APPLY_DET_UNDEAD	    65536
#define ITEM_APPLY_HEATED	   131072


/* Class-restricted object stuff
 * So only warriors can use some swords, thieves daggers, etc
 * -- Stephen
 */

#define ITEM_CLASS_SOR          1
#define ITEM_CLASS_KNI          2
#define ITEM_CLASS_ASS          4
#define ITEM_CLASS_MON          8
#define ITEM_CLASS_NEC         16
#define ITEM_CLASS_PSI         32
#define ITEM_CLASS_CON         64
#define ITEM_CLASS_TEM        128
#define ITEM_CLASS_RAN        256

/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_WEAR_NONE          0
#define ITEM_WEAR_HALO          1
#define ITEM_WEAR_AURA          2
#define ITEM_WEAR_HORNS         3
#define ITEM_WEAR_HEAD          4
#define ITEM_WEAR_FACE          5
#define ITEM_WEAR_BEAK          6
#define ITEM_WEAR_EAR           7
#define ITEM_WEAR_NECK          8
#define ITEM_WEAR_WINGS         9
#define ITEM_WEAR_SHOULDERS     10
#define ITEM_WEAR_ARMS          11
#define ITEM_WEAR_WRIST         12
#define ITEM_WEAR_HANDS         13
#define ITEM_WEAR_FINGER        14
#define ITEM_WEAR_CLAWS         15
#define ITEM_WEAR_HOLD_HAND     16
#define ITEM_WEAR_ABOUT         17
#define ITEM_WEAR_WAIST         18
#define ITEM_WEAR_BODY          19
#define ITEM_WEAR_TAIL          20
#define ITEM_WEAR_LEGS          21
#define ITEM_WEAR_FEET          22
#define ITEM_WEAR_HOOVES        23
#define ITEM_TAKE               24
/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
#define APPLY_NONE                    0
#define APPLY_STR                     1
#define APPLY_DEX                     2
#define APPLY_INT                     3
#define APPLY_WIS                     4
#define APPLY_CON                     5
#define APPLY_SEX                     6
#define APPLY_CLASS                   7
#define APPLY_LEVEL                   8
#define APPLY_AGE                     9
#define APPLY_HEIGHT                 10
#define APPLY_WEIGHT                 11
#define APPLY_MANA                   12
#define APPLY_HIT                    13
#define APPLY_MOVE                   14
/* 15 is empty                       */
#define APPLY_EXP                    16
#define APPLY_AC                     17
#define APPLY_HITROLL                18
#define APPLY_DAMROLL                19
#define APPLY_SAVING_PARA            20
#define APPLY_SAVING_ROD             21
#define APPLY_SAVING_PETRI           22
#define APPLY_SAVING_BREATH          23
#define APPLY_SAVING_SPELL           24

/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define MAX_OBJ_VALUE                 10
#define CONT_CLOSEABLE                1
#define CONT_PICKPROOF                2
#define CONT_CLOSED                   4
#define CONT_LOCKED                   8

/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 * The NULL_FILE is held open so that we have a stream handle in reserve,
 *   so players can go ahead and telnet to all the other descriptors.
 * Then we close it whenever we need to open a file (e.g. a save file).
 */
#if defined(macintosh)
#define PLAYER_DIR      "" /* Player files                 */
#define NPC_DIR		""
#define NULL_FILE       "proto.are" /* To reserve one stream        */
#define MOB_DIR         "" /* MOBProg files                */
#endif

#if defined(MSDOS)
#define PLAYER_DIR      "" /* Player files                 */
#define NPC_DIR		""
#define NULL_FILE       "nul" /* To reserve one stream        */
#define MOB_DIR         "" /* MOBProg files                */
#endif

#if defined(unix)
#define PLAYER_DIR      "../player/"   /* Player files                 */
#define NPC_DIR		"../npc/"
#define NULL_FILE       "/dev/null" /* To reserve one stream        */
#endif

#if defined(linux)
#define PLAYER_DIR      "../player/"   /* Player files                 */
#define NPC_DIR		"../npc/"
#define NULL_FILE       "/dev/null" /* To reserve one stream        */
#endif

#define AREA_DIR        "../area/"
#define MAIL_DIR        "../mail/"
#define DATA_DIR        "../data/"
#define BUG_DIR         "../report/"
#define HELP_DIR        "../help/"
#define BOARD_DIR       "../board/"
#define SCRIPT_DIR      "../script/"

#define HELP_MORT      "mrt"              /* Mortal helpfiles                      */
#define HELP_IMM       "imm"              /* Immortal helpfiles                    */
#define HELP_INDEX     "help.index"       /* For 'help' with no arg                */
#define SHELP_INDEX    "shelp_help.index" /* For 'shelp' with no arg               */
#define BHELP_INDEX    "build_help.index" /* For 'help' with no arg while building */

#define BUG_FILE       BUG_DIR  "bugs.txt"   /* For 'bug' and bug( )         */
#define IDEA_FILE      BUG_DIR  "ideas.txt" /* For 'idea'                   */
#define TYPO_FILE      BUG_DIR  "typos.txt" /* For 'typo'                   */
#define SHUTDOWN_FILE  BUG_DIR  "shutdown.txt" /* For 'shutdown'               */

#define AREA_LIST      DATA_DIR "arealst.dat" /* List of areas */
#define NOTE_FILE      DATA_DIR "notes.dat"  /* For 'notes'                  */
#define CLAN_FILE      DATA_DIR "clans.dat" /* stores clan diplomacy data   */
#define CORPSE_FILE    DATA_DIR "corpses.dat"
#define MARKS_FILE     DATA_DIR "marks.dat"
#define BANS_FILE      DATA_DIR "bans.dat"
#define RULERS_FILE    DATA_DIR "rulers.dat"
#define BRANDS_FILE    DATA_DIR "brands.dat"
#define SYSDAT_FILE    DATA_DIR "sysdat.dat"
#define SOCIAL_FILE    DATA_DIR "socials.dat"   /* or whatever fits you */
#define DISABLED_FILE  DATA_DIR "disabled.dat"
#define MUDINFO_FILE   DATA_DIR "mudinfo.dat"
#define PLAYERNUM_FILE DATA_DIR "playernum.dat"

/* Other Stuff - Flar */
#define COPYOVER_FILE  "COPYOVER.TXT" /* Temp data file used for copyover */
#define EXE_FILE       "../src/ack"   /* The one that runs the ACK! */

/* Kline Stuff */
#define VERS_STRING "AckFUSS 4.4.1"

// Some string defaults
#define DEFAULT_PROMPT    "@@g<@@d[@@W%x@@d] [@@e%h@@RH @@l%m@@BM @@r%v@@GV@@d]@@g>@@N"

// MSSP -- Update these to properly report your MUD to automated crawlers
#define MSSP_HOSTNAME     "home.gotr00t.us"                    //MUD hostname
#define MSSP_PORT         3000                                 //MUD port
#define MSSP_CONTACT      "genocaust@gmail.com"                //Admin email
#define MSSP_CREATED      2008                                 //Year MUD opened
#define MSSP_ICON         "http://www.ackmud.net/ack_mssp.gif" //32x32 256 color icon
#define MSSP_IP           "24.162.100.182"                     //MUD IP address
#define MSSP_LANGUAGE     "English"                            //Primary language spoken
#define MSSP_LOCATION     "United States"                      //Full country name per ISO standard
#define MSSP_MIN_AGE      0                                    //Minimum age to play, 0 for none
#define MSSP_NAME         mudnamenocolor                       //MUD name -- just copy from the top of config.h
#define MSSP_WEBSITE      "http://www.ackmud.net"              //MUD website
#define MSSP_FAMILY       "DikuMUD"                            //Most distant ancestor -- leave this alone :P
#define MSSP_GENRE        "Fantasy"                            //Duh...
#define MSSP_GAMEPLAY     "Hack and Slash"                     //Kill lots of mobs!
#define MSSP_GAMESYSTEM   "Custom"                             //Cuz AckFUSS is so badass
#define MSSP_INTERMUD     "IMC2"                               //Included by default
#define MSSP_STATUS       "Closed Beta"                        //Dev game is always closed
#define MSSP_SUBGENRE     "Medieval Fantasy"                   //Duh...
#define MSSP_CLASSES      11                                   //5 mort + 5 remort + 1 adept
#define MSSP_RACES        19                                   //Total count so far
#define MSSP_LEVELS       120                                  //Adept is psuedo-level 120
#define MSSP_HIRE_BUILDER 1                                    //Hiring builders? 1 for yes 0 for no
#define MSSP_HIRE_CODER   1                                    //Hiring coders? 1 for yes 0 for no
