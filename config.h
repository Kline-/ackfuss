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

#define DEC_CONFIG_H		1

/*
 *  Your mud info here :) Zen
 */

#define mudnamecolor  "@@lYour Mud Name@@N"
#define mudnamenocolor  "Your Mud name"
#define sacgodname  "Cyberlich"
#define goodgodname  "Freyja"
#define neutralgodname  "Zenithar"
#define evilgodname  "Oderus"
#define UPGRADE_REVISION 15
/*
 * String and memory management parameters.
 */
#define MAX_KEY_HASH             2048
#define MAX_STRING_LENGTH        8192
#define MSL			MAX_STRING_LENGTH
#define MAX_INPUT_LENGTH          640
#define MAX_AREAS                 200
#define MAX_VNUM 32767

#define BOOT_DB_ABORT_THRESHOLD			  25
#define RUNNING_ABORT_THRESHOLD			  10
#define ALARM_FREQUENCY               20

/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.
 */

#define MAX_CLAN_EQ		     6   /* Number of clan eq items */
#define MAX_color		    15   /* eg look, prompt, shout */
#define MAX_ANSI		    28   /* eg red, black, etc (was 11) */
#define MAX_ALIASES		     6
#define MAX_IGNORES		     3
#define MAX_RACE                     16
#define MAX_CLAN                     9 /* number of clans */
#define EXP_LEVEL                 1000 /* exp per level */
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

#define MAX_NUM_IMMS    5

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


#define AREA_PAYAREA  	    1
#define AREA_TELEPORT 	    2
#define AREA_BUILDING 	    4
#define AREA_NOSHOW	    8 /* Show area name on 'areas' command? */
#define AREA_NO_ROOM_AFF    16

/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_MARTIAL		     -3  /* for martial arts */
#define TYPE_UNDEFINED               -2
#define TYPE_HIT                     1000



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
#define C_TYPE_IMC      6
#define C_TYPE_IMM      7

#define C_SHOW_NEVER    -1
#define C_SHOW_ALWAYS     0
#define C_SHOW_SKILL      1




/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_CITYGUARD         3060
#define MOB_VNUM_VAMPIRE           3404
#define MOB_VNUM_DOGGY             99
#define MOB_VNUM_ZOMBIE 	  11004
#define MOB_VNUM_STALKER	   32
#define MOB_VNUM_WATERELEM        1028
#define MOB_VNUM_SKELETON         1028
#define MOB_VNUM_FIREELEM         1028
#define MOB_VNUM_TRAINER		1042
#define MOB_VNUM_EARTHELEM		1028
#define MOB_VNUM_IRON_GOLEM		1029
#define MOB_VNUM_SOUL_THIEF		1030
#define MOB_VNUM_HOLY_AVENGER		1031
#define MOB_VNUM_PEGASUS		1037
#define MOB_VNUM_NIGHTMARE		1038
#define MOB_VNUM_ELEM_BEAST		1039
#define MOB_VNUM_INT_DEVOURER		1040
#define MOB_VNUM_SHADOW_HOUND		1041


#define MOB_VNUM_DIAMOND_GOLEM	1032
#define MOB_VNUM_COMBAT_ELEMENTAL 1036 /* this needs to be a new mob ZENFIX */

/*
 * vnums for quest items.
 * One of these will be picked at random and placed onto a random mob
 * (one the player has a chance of killing!).
 * Reserve vnums 40-60 for this.
 * We'll define a range of possible vnums here - easier to expand (:
 */

#define OBJ_VNUM_QUEST_MIN		18181
#define OBJ_VNUM_QUEST_MAX		18192 /* more to follow, up to 18199  */


#define PULSE_PER_SECOND             8
#define PULSE_VIOLENCE            (  2 * PULSE_PER_SECOND )
#define PULSE_MOBILE              (  4 * PULSE_PER_SECOND )
#define PULSE_OBJFUN		  (  4 * PULSE_PER_SECOND )
#define PULSE_TICK                ( 60 * PULSE_PER_SECOND )
#define PULSE_MESSAGE		  ( 12 * PULSE_PER_SECOND )
#define PULSE_ROOMS		  ( 30 * PULSE_PER_SECOND )
#define PULSE_AREA                ( 80 * PULSE_PER_SECOND )
#define PULSE_AUCTION		  ( 30 * PULSE_PER_SECOND )
#define PULSE_RAUCTION            ( 30 * PULSE_PER_SECOND )
#define PULSE_MAUCTION            ( 30 * PULSE_PER_SECOND )


#define VAMPIRE_RECALL		 9001
#define LIQUID_BLOOD		 13
#define WOLF_RECALL          3001

/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */
#define OBJ_VNUM_MONEY_ONE            2
#define OBJ_VNUM_MONEY_SOME           3

#define OBJ_VNUM_CORPSE_NPC          10
#define OBJ_VNUM_CORPSE_PC           11
#define OBJ_VNUM_SEVERED_HEAD        12
#define OBJ_VNUM_TORN_HEART          13
#define OBJ_VNUM_SLICED_ARM          14
#define OBJ_VNUM_SLICED_LEG          15
#define OBJ_VNUM_FINAL_TURD          16

#define OBJ_VNUM_MUSHROOM            20
#define OBJ_VNUM_LIGHT_BALL          21
#define OBJ_VNUM_SPRING              19

#define OBJ_VNUM_BOARD               23
#define OBJ_VNUM_FOOD		     30
#define OBJ_VNUM_FIREBLADE	     31
#define OBJ_VNUM_TOKEN		     32  /* For old chars */
#define OBJ_VNUM_WINDOW		     33
#define OBJ_VNUM_PORTAL		     34
#define OBJ_VNUM_BEACON		     35

#define OBJ_VNUM_SCHOOL_MACE       3700
#define OBJ_VNUM_SCHOOL_DAGGER     3701
#define OBJ_VNUM_SCHOOL_SWORD      3702
#define OBJ_VNUM_SCHOOL_VEST       3703
#define OBJ_VNUM_SCHOOL_SHIELD     3704
#define OBJ_VNUM_SCHOOL_BANNER     3716
#define OBJ_VNUM_SOUL_POTION       37
#define OBJ_VNUM_CAPTURED_SOUL     38
#define OBJ_VNUM_CONFLAGRATION	   1036  /* need a staff here  ZENFIX */
#define OBJ_VNUM_MINO_PASS         10021
/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_LIMBO               2
#define ROOM_VNUM_CHAT             1200
#define ROOM_VNUM_TEMPLE           3001
#define ROOM_VNUM_ALTAR            3054
#define ROOM_VNUM_SCHOOL           3700
#define ROOM_VNUM_DONATION         3206
#define ROOM_VNUM_MORGUE           3300
#define ROOM_VNUM_MORIBUND         3301
#define ROOM_VNUM_BUILDER	   1019  /* Bulder-flagged players only */
#define ROOM_VNUM_BODIES	   30 /* for morphed players */
#define ROOM_VNUM_CLAN		   1022  /* for leaders only */
#define ROOM_VNUM_DM_RECALL       3001
#define ROOM_VNUM_ETHEREAL_PLANE  3850
#define ROOM_VNUM_INT_HEAL		3871
#define ROOM_VNUM_MID_BOTTOM		3001
#define ROOM_VNUM_MID_TOP		3200
#define ROOM_VNUM_JAIL                  2
#define ROOM_VNUM_ARMOR_DONATE    3018
#define ROOM_VNUM_WEAPON_DONATE   3019
#define ROOM_VNUM_MISC_DONATE     3017



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
#define STANCE_WIZARD         11
#define STANCE_MAGI          12




#define C_SHOW_NEVER    -1
#define C_SHOW_ALWAYS     0
#define C_SHOW_SKILL      1

#define SUPER_NONE			0
#define SUPER_VAMP			1
#define SUPER_WOLF			2
#define SUPER_BLESSED			3

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
#define CON_SETTING_STATS		1

/* values used to check a new player has selected all options.... */
#define CHECK_RACE			  1
#define CHECK_CLASS			  2
#define CHECK_SEX			  4
#define CHECK_STATS			  8

/*
 * TO types for act.
 */
#define TO_ROOM             0
#define TO_NOTVICT          1
#define TO_VICT             2
#define TO_CHAR             3


/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_DARK                     1
#define ROOM_REGEN                    2   /* increased regen in this room */
#define ROOM_NO_MOB                   4
#define ROOM_INDOORS                  8
#define ROOM_NO_MAGIC                16
#define ROOM_HOT                     32   /* players lose hp each tick   */
#define ROOM_COLD                    64   /* players lose hp each tick   */
#define ROOM_PK                     128   /* players may freely pk here  */
#define ROOM_QUIET                  256   /* room is quiet               */
#define ROOM_PRIVATE                512
#define ROOM_SAFE                  1024
#define ROOM_SOLITARY              2048
#define ROOM_PET_SHOP              4096
#define ROOM_NO_RECALL             8192
#define ROOM_NO_TELEPORT          16384
#define ROOM_HUNT_MARK	          32768   /* Dont ever set! */
#define ROOM_NOBLOODWALK	  65536
#define ROOM_NO_PORTAL		 131072
#define ROOM_NO_REPOP		BIT_19


/*
 * Directions.
 * Used in #ROOMS.
 */
#define DIR_NORTH                     0
#define DIR_EAST                      1
#define DIR_SOUTH                     2
#define DIR_WEST                      3
#define DIR_UP                        4
#define DIR_DOWN                      5



/*
 * Exit flags.
 * Used in #ROOMS.
 */
#define EX_ISDOOR                     1
#define EX_CLOSED                     2
#define EX_LOCKED                     4
#define EX_CLIMB		      8
#define EX_IMMORTAL		     16
#define EX_PICKPROOF                 32
#define EX_SMASHPROOF		     64
#define EX_PASSPROOF		    128
#define EX_NODETECT		    256



/*
 * Sector types.
 * Used in #ROOMS.
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
#if 0
#define SECT_HERE                    12
#define SECT_UNSEEN                  13
#define SECT_BLOCKED                 14
#define SECT_TOP                     15
#endif

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
 *  Configuration Bits for players
 */

#define CONFIG_AUTOEXIT         BIT_1
#define CONFIG_AUTOLOOT         BIT_2
#define CONFIG_AUTOSAC          BIT_3
#define CONFIG_BLANK            BIT_4
#define CONFIG_BRIEF            BIT_5
#define CONFIG_COMBINE          BIT_6
#define CONFIG_PROMPT           BIT_7
#define CONFIG_TELNET_GA        BIT_8
#define CONFIG_COLOR            BIT_9
#define CONFIG_AUTOSPLIT        BIT_10
#define CONFIG_FULL_ANSI        BIT_11
#define CONFIG_MAPPER           BIT_12
#define CONFIG_JUSTIFY          BIT_13


/*
 * ACT bits for players.
 */
#define PLR_IS_NPC                    1   /* Don't EVER set.      */
#define PLR_BOUGHT_PET                2
#define PLR_CLAN_LEADER               4   /* Clan Leader!! */
/* Available 
#define PLR_AUTOEXIT                 BIT_4   
#define PLR_AUTOLOOT                 BIT_5
#define PLR_AUTOSAC                  BIT_6
#define PLR_BLANK                    BIT_7
#define PLR_BRIEF                    BIT_8
*/

#define PLR_NO_PRAY                 256
/* Available
#define PLR_COMBINE                 BIT_10
#define PLR_PROMPT                 BIT_11
#define PLR_TELNET_GA              BIT_12
*/
#define PLR_HOLYLIGHT              4096
#define PLR_WIZINVIS               8192
#define PLR_BUILDER               16384   /* Is able to use the OLC */
#define PLR_SILENCE               32768

#define PLR_NO_EMOTE              65536
/* avaiable
#define PLR_color		 BIT_18	
*/
#define PLR_NO_TELL              262144
#define PLR_LOG                  524288
#define PLR_DENY                1048576
#define PLR_FREEZE              2097152
#define PLR_THIEF               4194304
#define PLR_KILLER              8388608
#define PLR_NOSUMMON           16777216
#define PLR_NOVISIT            33554432
/* available
#define PLR_QUESTING	        BIT_27
#define PLR_AUTOSPLIT         BIT_28
#define PLR_AUTOASSIST        BIT_29
*/

#define PLR_NOBLOOD	       536870912

#define PLR_INCOG			1073741824

/*
 * Obsolete bits.
 */
#if 0
#define PLR_AUCTION                   4   /* Obsolete     */
#define PLR_CHAT                    256   /* Obsolete     */
#define PLR_NO_SHOUT             131072   /* Obsolete     */
#endif

/* 
 * Player flags
 */
#define PFLAG_PKOK		1
#define PFLAG_AFK			2
#define PFLAG_AMBAS		4
#define PFLAG_VAMP		8
#define PFLAG_CLAN_DIPLOMAT	16
#define PFLAG_CLAN_BOSS		32
#define PFLAG_CLAN_TREASURER	64
#define PFLAG_CLAN_ARMOURER	128
#define PFLAG_CLAN_LEADER	256
#define PFLAG_SUPER_COUNCIL	512
#define PFLAG_WEREWOLF		1024
#define PFLAG_RAGED		2048
#define PFLAG_SHIFTED		4096
#define PFLAG_RULER		BIT_14
#define PFLAG_BLIND_PLAYER	BIT_15
#define PFLAG_TESTER  BIT_16


/*
 * Channel bits.
 */
#define CHANNEL_AUCTION               1
#define CHANNEL_GOSSIP                2
#define CHANNEL_MUSIC                 4
#define CHANNEL_IMMTALK               8
#define CHANNEL_NEWBIE               16
#define CHANNEL_QUESTION             32
#define CHANNEL_SHOUT                64
#define CHANNEL_YELL                128
#define CHANNEL_FLAME               256
#define CHANNEL_ZZZ                 512
#define CHANNEL_RACE               1024
#define CHANNEL_CLAN               2048
#define CHANNEL_NOTIFY             4096
#define CHANNEL_INFO               8192
#define CHANNEL_LOG		  16384
#define CHANNEL_CREATOR		  32768
#define CHANNEL_ALLCLAN		  65536
#define CHANNEL_ALLRACE		 131072
#define CHANNEL_HERMIT		 262144  /* Turns off ALL channels */
#define CHANNEL_BEEP		 524288
#define CHANNEL_FAMILY		1048576
#define CHANNEL_DIPLOMAT	2097152
#define CHANNEL_CRUSADE		4194304
#define CHANNEL_REMORTTALK	8388608
#define CHANNEL_HOWL           BIT_25
#define CHANNEL_ADEPT	        BIT_26
/* #define CHANNEL_MAUCTION        67108864  */
#define CHANNEL_OOC            BIT_27
#define CHANNEL_QUEST          BIT_28
#define CHANNEL_LANG	        BIT_29
#define CHANNEL_GAME		BIT_30

  /*
   * NOTE 32 is the last allowable channel ZEN 
   */

/* Monitor channels - for imms to select what mud-based info they receive */
#define MONITOR_CONNECT		      1
#define MONITOR_AREA_UPDATE	      2
#define MONITOR_AREA_BUGS	      4
#define MONITOR_AREA_SAVING	      8
#define MONITOR_GEN_IMM		     16
#define MONITOR_GEN_MORT	     32
#define MONITOR_COMBAT		     64
#define MONITOR_HUNTING		    128
#define MONITOR_BUILD		    256
#define MONITOR_CLAN		    512
#define MONITOR_OBJ		   1024
#define MONITOR_MOB		   2048
#define MONITOR_ROOM		   4096
#define MONITOR_MAGIC		   8192
#define MONITOR_BAD		  16384
#define MONITOR_DEBUG   BIT_16
#define MONITOR_IMC     BIT_17
#define MONITOR_SYSTEM  BIT_18
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

#define ERROR_PROG        -1
#define IN_FILE_PROG       0
#define ACT_PROG           1
#define SPEECH_PROG        2
#define RAND_PROG          4
#define FIGHT_PROG         8
#define DEATH_PROG        16
#define HITPRCNT_PROG     32
#define ENTRY_PROG        64
#define GREET_PROG       128
#define ALL_GREET_PROG   256
#define GIVE_PROG        512
#define BRIBE_PROG      1024

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





/*
 * ACT bits for mobs.
 * Used in #MOBILES.
 */
#define ACT_IS_NPC                    1   /* Auto set for mobs    */
#define ACT_SENTINEL                  2   /* Stays in one room    */
#define ACT_SCAVENGER                 4   /* Picks up objects     */
#define ACT_REMEMBER                  8   /* remembers target     */
#define ACT_NO_FLEE                  16   /* can't flee from mob  */
#define ACT_AGGRESSIVE               32   /* Attacks PC's         */
#define ACT_STAY_AREA                64   /* Won't leave area     */
#define ACT_WIMPY                   128   /* Flees when hurt      */
#define ACT_PET                     256   /* Auto set for pets    */
#define ACT_TRAIN                   512   /* Can train PC's       */
#define ACT_PRACTICE               1024   /* Can practice PC's    */
#define ACT_MERCENARY              2048   /* Is a mercenary       */
#define ACT_HEAL                   4096   /* Sells spells         */
#define ACT_ADAPT                  8192   /* Adapts weapons       */
#define ACT_UNDEAD                16384   /* Mob is undead  */
#define ACT_BANKER                32768   /* Is a *B*anker :P     */
#define ACT_NO_BODY               65536   /* no body for damage   */
#define ACT_HUNTER		 131072  /* hunts attackers      */
#define ACT_NOMIND		 262144  /* Psi attack no-no     */
#define ACT_POSTMAN		 524288  /* Postmaster     */
#define ACT_REWIELD	 	1048576  /* Uses better weapons  */
#define ACT_RE_EQUIP	 	2097152  /* Uses better equipment */
#define ACT_INTELLIGENT		4194304  /* For shitkicking mobs */
#define ACT_VAMPIRE             8388608   /* Vampire mob    */
#define ACT_BREEDER          16777216  /* Mob can breed        */
#define ACT_SOLO   	       33554432   /* Mob is buffed for solo combat */
#define ACT_WEREWOLF           67108864         /* Werewolf mob         */ /* doesn't do anything in ack4.0 ZENFIX */
#define ACT_MOUNT             134217728   /* Mountable MOB */
#define ACT_NOBLOOD		BIT_29

/* build bits for OLC -S- */
#define ACT_BUILD_NOWT                0   /* not doing anything   */
#define ACT_BUILD_REDIT               1   /* editing rooms        */
#define ACT_BUILD_OEDIT               2   /* editing objects      */
#define ACT_BUILD_MEDIT               3   /* editing mobiles      */

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

#define MOB_NONE		      	     1
#define MOB_SECOND			     2
#define MOB_THIRD			     4
#define MOB_FOURTH			     8
#define MOB_PUNCH			    16
#define MOB_HEADBUTT			    32
#define MOB_KNEE			    64
#define MOB_DISARM			   128
#define MOB_TRIP			   256
#define MOB_NODISARM			   512
#define MOB_NOTRIP			  1024
#define MOB_DODGE			  2048
#define MOB_PARRY			  4096
#define MOB_MARTIAL			  8192
#define MOB_ENHANCED			 16384
#define MOB_DUALWIELD			 32768
#define MOB_DIRT			 65536
#define MOB_FIFTH			131072
#define MOB_SIXTH			262144
#define MOB_CHARGE			524288

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
#define CAST_color_SPRAY		    16
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
#define ITEM_SOUL				 28
#define ITEM_PIECE			 29
#define ITEM_SPELL_MATRIX		30
#define ITEM_ENCHANTMENT		31
#define ITEM_WRAPPED_PRESENT 32
/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW				1
#define ITEM_HUM				2
#define ITEM_DARK				4
#define ITEM_NODISARM				4
#define ITEM_LOCK				8
#define ITEM_EVIL				16
#define ITEM_INVIS			32
#define ITEM_MAGIC			64
#define ITEM_NODROP			128
#define ITEM_BLESS			256
#define ITEM_ANTI_GOOD			512
#define ITEM_ANTI_EVIL			1024
#define ITEM_ANTI_NEUTRAL		2048
#define ITEM_NOREMOVE			4096
#define ITEM_INVENTORY			8192
#define ITEM_NOSAVE			16384 /* For "quest" items :) */
#define ITEM_CLAN_EQ		 	32768 /* only leaders and creators may load */
#define ITEM_TRIG_DESTROY	 	65536 /* Trigger object that destroy after use */
#define ITEM_NO_AUCTION			131072   /* Can't be auctioned */
#define ITEM_REMORT			262144   /* player must have a remort class of higher level than item level */
#define ITEM_ADEPT			524288
#define ITEM_RARE			1048576
#define ITEM_VAMP                       2097152 /* player must be a vampyre */
#define ITEM_NOLOOT			4194304
#define ITEM_NOSAC			8388608
#define ITEM_UNIQUE			16777216
#define ITEM_LIFESTEALER	   BIT_26
#define ITEM_SILVER			BIT_27

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
#define ITEM_WEAR_NONE          BIT_0
#define ITEM_WEAR_HALO          BIT_1
#define ITEM_WEAR_AURA          BIT_2
#define ITEM_WEAR_HORNS         BIT_3
#define ITEM_WEAR_HEAD          BIT_4
#define ITEM_WEAR_FACE          BIT_5
#define ITEM_WEAR_BEAK          BIT_6
#define ITEM_WEAR_EAR           BIT_7
#define ITEM_WEAR_NECK          BIT_8
#define ITEM_WEAR_WINGS         BIT_9
#define ITEM_WEAR_SHOULDERS     BIT_10
#define ITEM_WEAR_ARMS          BIT_11
#define ITEM_WEAR_WRIST         BIT_12
#define ITEM_WEAR_HANDS         BIT_13
#define ITEM_WEAR_FINGER        BIT_14
#define ITEM_WEAR_CLAWS         BIT_15
#define ITEM_WEAR_HOLD_HAND     BIT_16
#define ITEM_WEAR_ABOUT         BIT_17
#define ITEM_WEAR_WAIST         BIT_18
#define ITEM_WEAR_BODY          BIT_19
#define ITEM_WEAR_TAIL          BIT_20
#define ITEM_WEAR_LEGS          BIT_21
#define ITEM_WEAR_FEET          BIT_22
#define ITEM_WEAR_HOOVES        BIT_23
#define ITEM_TAKE               BIT_24
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
#define APPLY_GOLD                   15
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
#define NPC_DIR		"../npcs/"
#define NULL_FILE       "/dev/null" /* To reserve one stream        */
#define MOB_DIR         "MOBProgs/" /* MOBProg files                */
#endif

#if defined(linux)
#define PLAYER_DIR      "../player/"   /* Player files                 */
#define NPC_DIR		"../npcs/"
#define NULL_FILE       "/dev/null" /* To reserve one stream        */
#define MOB_DIR         "MOBProgs/" /* MOBProg files                */
#endif

#define AREA_LIST       "area.lst"  /* List of areas                */

#define MAIL_DIR        "../mail/"
#define DATA_DIR        "../data/"
#define BUG_DIR         "../reports/"
#define BUG_FILE       BUG_DIR  "bugs.txt"   /* For 'bug' and bug( )         */
#define IDEA_FILE      BUG_DIR   "ideas.txt" /* For 'idea'                   */
#define TYPO_FILE      BUG_DIR   "typos.txt" /* For 'typo'                   */
#define NOTE_FILE      DATA_DIR "notes.txt"  /* For 'notes'                  */
/* FIXME: boards.txt in here????? */
#define SHUTDOWN_FILE   BUG_DIR  "shutdown.txt" /* For 'shutdown'               */

#define CLAN_FILE		DATA_DIR "clandata.dat" /* stores clan diplomacy data   */
#define CORPSE_FILE	DATA_DIR "corpses.lst"
#define MARKS_FILE	DATA_DIR "roommarks.lst"
#define BANS_FILE	DATA_DIR "bans.lst"
#define RULERS_FILE	DATA_DIR "rulers.lst"
#define CONTROLS_FILE	DATA_DIR "area_controls.lst"
#define BRANDS_FILE	DATA_DIR "brands.lst"

#define SYSDAT_FILE	DATA_DIR "system.dat"

#if defined(SOE) && !defined(SOETEST) && !defined(SOEBLD)
#define WHO_HTML_FILE   "/home/ftp/pub/webpage/soewholist.html"   /* for ftping who list to html web page :) */
#define WHO_COUNT_FILE  "/home/ftp/pub/webpage/whocount.html"
#endif
#if !defined(WHO_HTML_FILE)
#define WHO_HTML_FILE   DATA_DIR "whofile"   /* for ftping who list to html web page :) */
#define WHO_COUNT_FILE  DATA_DIR "whocountfile"
#endif
/* Other Stuff - Flar */
#define COPYOVER_FILE	"COPYOVER.TXT" /* Temp data file used for copyover */
#define EXE_FILE		"../src/ack"   /* The one that runs the ACK! */
