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
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
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

#include "h/globals.h"

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_HUNT_H
#include "h/hunt.h"
#endif

#ifndef DEC_MACROS_H
#include "h/macros.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_QUEST_H
#include "h/quest.h"
#endif

#ifndef DEC_SPEC_FUN_H
#include "h/spec_fun.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

extern CHAR_DATA *quest_mob;
extern CHAR_DATA *quest_target;
extern OBJ_DATA *quest_object;
extern int quest_wait;
extern int quest_timer;
extern short quest_personality;
extern bool auto_quest;
extern bool quest;

/* 17 messages, organised by blocks for each personality
   indented messages are for when the target mob gets killed  */
struct qmessage_type
{
   char *const message1;
   char *const message2;
};

const struct qmessage_type qmessages[4][17] = {
   {
    {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""},
    {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""}, {"", ""},
    {"", ""}
    },
   {
    {"@@aOh my! My %s @@ahas been stolen from me, and I am too young to find it!", ""},
    {"@@aWould someone please search for my %s@@a? I'm sure that it will be much too hard for me to find.", ""},
    {"@@aWhat will I do without my %s@@a?", ""},
    {"@@aCan't anybody find my %s @@afor me?", ""},
    {"@@aHelp me! My %s @@ais still missing!", ""},
    {"@@aKeep searching for my %s@@a, and i'll ask my mom if she knows who stole it!", ""},
    {"@@aDoes anyone know who stole my %s@@a?", ""},
    {"@@aMy mom says %s@@a stole my %s@@a. I know it is too hard for me to get back. Oh my, what will I do?",
     "@@aYeay!! It looks like whoever stole my %s @@ais now dead!! Thank you very much!"},
    {"@@aPlease, can you go kill %s@@a? I need my %s@@a!! I can't sleep without it!!",
     "@@aWell, thank you for killing the nasty thief, but can you please return my %s @@ato me?"},
    {"@@aMy mom says %s@@a stole my %s@@a. I know it is too hard for me to get back. Oh my, what will I do?",
     "@@aYeay!! It looks like whoever stole my %s @@ais now dead!! Thank you very much!"},
    {"@@aIt's time for my nap now, and %s still has my %s@@a!! Can anyone please get it back for me?",
     "@@aPlease, time is running out! Return my %s @@ato me!"},
    {"@@a%s@@a is a real meanie for stealing my %s@@a! Can you pretty pretty pretty please get it back for me?",
     "@@aPlease, time is running out! Return my %s @@ato me!"},
    {"@@aIt's time for my nap now, and %s still has my %s@@a!! Can anyone please get it back for me?",
     "@@aPlease, time is running out! Return my %s @@ato me!"},
    {"@@aOh my, I'll never get up to watch cartoons tomorrow now!  %s @@ais still holding my %s@@a ransom, and I need it for my nap!",
     "@@aHow can I sleep without my %s@@a?"},
    {"@@aI give up! %s @@acan keep my %s @@afor all I care! I didn't want to take a nap, anyway!",
     "@@aI give up! I never want to see my %s @@aagain!"},
    {"@@aMommy, can I watch cartoons now, instead of taking a nap?? PLEASE??", ""},
    {"@@aOh THANK YOU, %s@@a!! Now that I have my %s @@aagain, I can go sleepy sleep!", ""}

    },


   {
    {"Hmm, I seem to have lost my %s@@l. Oh well, didn't really need it much, anyway.", ""},
    {"I wonder where I could find another %s@@l? Anyone have any ideas?", ""},
    {"Where can my %s @@lhave gone?", ""},
    {"I guess no one really cares, anyway, but I think I might need a %s @@llater.", ""},
    {"I guess I should try and find my %s@@l, but I really don't feel like it.", ""},
    {"If anyone has an extra %s@@l, I might be willing to reward them for it.", ""},
    {"Doesn't anyone out there know where to find a %s@@l?", ""},
    {"Hmm, maybe %s@@l knew something I didn't, and thought it was a good idea to steal my %s@@l. Maybe he could use it, I know I can't.",
     "I guess my %s @@ldidn't help him much, since he is now dead!  I do miss it though."},
    {"Hmm, maybe it IS worth something.  Could someone go kill %s@@l and get my %s@@l back for me?",
     "I guess my %s @@ldidn't help him much, since he is now dead!  I do miss it though."},
    {"I would pay a lot if someone would kill %s @@land get my %s@@l back. I don't really know where it went.",
     "Even though it's not worth very much, my %s @@lis kind of important to me. Oh, well, guess I will never see it again."},
    {"Hmm, maybe it IS worth something.  Could someone go kill %s@@l and get my %s@@l back for me?",
     "I guess my %s @@ldidn't help him much, since he is now dead!  I do miss it though."},
    {"I would pay a lot if someone would kill %s @@land get my %s@@l back. I don't really know where it went.",
     "Even though it's not worth very much, my %s @@lis kind of important to me. Oh, well, guess I will never see it again."},
    {"Oh well, since no one will help me, I guess %s @@lcan keep my %s@@l.",
     "It must be cursed, since everyone who has it is dead. I don't think I want my %s @@lafter all!"},
    {"Oh well, since no one will help me, I guess %s @@lcan keep my %s@@l.",
     "It must be cursed, since everyone who has it is dead. I don't think I want my %s @@lafter all!"},
    {"I give up! %s @@lcan keep my %s @@lfor all I care!",
     "I give up! I never want to see my %s @@lagain!"},
    {"Well, I will stop asking now, but don't ever ask ME for any favors, ok?", ""},
    {"Well, looks like %s @@lhas recovered my %s @@lfor me. Not sure I want it anymore, but thanks anyway.", ""}

    },

/*
{
	{ "BANZAI! My %s @@lhas been stolen from me!  Will someone brave recover it?", 	"" },
	{ "Oh! Has no one found my %s @@lfor me yet?", 							"" },
	{ "Where can my %s @@lhave gone?", 									"" },
	{ "Can't anybody find my %s @@lfor me?",  							"" },
	{ "Help me! My %s @@lhas not yet been recovered!", 						"" },
	{ "Keep searching for my %s@@l, and i'll find out who stole it!", 			"" },
	{ "Were there no witnesses to the theft of my %s?@@l", 					"" },
	{ "It was %s @@lwho stole my %s @@lfrom me!  Someone help me!",
		"It looks like whoever stole my %s @@lis now dead!!" 					},
	{ "Please, time is running out! Recover my %s @@lfrom %s @@lfor me NOW!",
		"Please, time is running out! Return my %s @@lto me!" 				},
	{ "Please, time is running out! Recover my %s @@lfrom %s @@lfor me NOW!",
		"Please, time is running out! Return my %s @@lto me!" 				},
	{ "Please, time is running out! Recover my %s @@lfrom %s @@lfor me NOW!",
		"Please, time is running out! Return my %s @@lto me!" 				},
	{ "Please, time is running out! Recover my %s @@lfrom %s @@lfor me NOW!",
		"Please, time is running out! Return my %s @@lto me!" 				},
	{ "I give up! %s @@lcan keep my %s @@lfor all I care!",
		"I give up! I never want to see my %s @@lagain!" 					},
	{ "I give up! %s @@lcan keep my %s @@lfor all I care!",
		"I give up! I never want to see my %s @@lagain!"					},
	{ "I give up! %s @@lcan keep my %s @@lfor all I care!",
		"I give up! I never want to see my %s @@lagain!"					},
	{ "Shoot! Just forget about recovering ANYTHING for me, ok?" , 				"" },
	{ "At Last! %s @@lhas recovered %s @@lfor me!", 						"" }

    },  */

   {
    {"@@eMuuaahhhaaahaaaa! Some puny mortal has stolen my %s@@e!  I shall seek revenge!!", ""},
    {"@@eI shall send many minions to seek my %s@@e! All that steal from me shall die!!", ""},
    {"@@eSO, you have defeated my servants.  I shall still regain my %s@@e!!", ""},
    {"@@eI am prepared to reward well anyone that aids the return of my %s@@e. Are any of you puny mortals willing to attempt my challenge?", ""},
    {"@@eIf you are worthy, I will grant many favors upon anyone that returns my %s@@e.", ""},
    {"@@mMethlok@@e, By the dark powers, I command you to seek my %s@@e! Now, if any of you worthless mortals wish to attempt to return it, I shall grant you many powers!", ""},
    {"@@eI sense that @@mMethlok@@e is nearing the witless thief who stole my %s@@e. Now, my vengence shall be sweet!", ""},
    {"@@eAhhh, my servant has returned, and informs me that %s @@estole my %s@@e. They shall be incinerated by the powers that I command!!!",
     "@@mMethlok@@e has informed me that the weakling that stole my %s @@lhas met his maker!!"},
    {"@@eAre none of you powerful enough to kill %s @@eand regain my %s@@e? Bah!! Mortals are useless, except as side dishes!!",
     "@@eThough my taste for blood has been satiated, my %s @@estill evades my grasp!"},
    {"@@eAre none of you powerful enough to kill %s @@e and regain my %s @@e? Bah!! Mortals are useless, except as side dishes!!",
     "@@eThough my taste for blood has been satiated, my %s @@estill evades my grasp!"},
    {"@@eAre none of you powerful enough to kill %s @@e and regain my %s @@e? Bah!! Mortals are useless, except as side dishes!!",
     "@@eThough my taste for blood has been satiated, my %s @@estill evades my grasp!"},
    {"@@eI should have known that a powerless, puny mortal could never be the servant of my vengence against %s@@e, or regain my %s@@e!!",
     "@@eI shall rain death upon all of you for refusing to return my %s@@e!!!"},
    {"@@eI should have known that a powerless, puny mortal could never be the servant of my vengence against %s@@e, or regain my %s@@e!!",
     "@@eI shall rain death upon all of you for refusing to return my %s@@e!!!"},
    {"@@eI shall slay your brothers and poison your fields for refusing to seek %s @@eand return my %s@@e!!!!",
     "@@eThough my vengeance has been served, I shall drink your souls for your failure to return my %s@@e!!!"},
    {"@@eI shall slay your brothers and poison your fields for refusing to seek %s @@eand return my %s@@e!!!!",
     "@@eThough my vengeance has been served, I shall drink your souls for your failure to return my %s@@e!!!"},
    {"@@eDeath and great suffering shall be your punishment for failing me!!!?", ""},
    {"@@eWell done.  It seems that %s @@eat least has a modicum of strength, unlike you worthless idiots who failed to return my %s@@e! My curse shall lie upon you for the rest of your short days!", ""}

    }

};


DO_FUN(do_iquest)
{
   char buf[MAX_STRING_LENGTH];
   char new_long_desc[MAX_STRING_LENGTH];

   if( argument[0] == '\0' )  /* Display status */
   {
      if( !quest )
      {
         send_to_char( "There is no quest currently running.\r\n", ch );
         if( auto_quest )
            send_to_char( "Quests are currently running automatically.\r\n", ch );
         if( quest_wait > 0 )
         {
            snprintf( buf, MSL, "The next quest may occur in %d minutes.\r\n", quest_wait );
            send_to_char( buf, ch );
         }
         return;
      }
      else
         send_to_char( "There is currently a quest running ", ch );

      if( auto_quest )
         send_to_char( "(Automatically)", ch );

      send_to_char( "\r\nQuest Details:\r\n\r\n", ch );
      if( quest_mob )
      {

         snprintf( buf, MSL, "The questing mobile is: %s [In Room %d]\r\n", quest_mob->get_name(), quest_mob->in_room->vnum );
         send_to_char( buf, ch );
      }
      else
      {
         send_to_char( "The questing mobile is dead!\r\n", ch );
      }
      if( quest_target )
      {
         snprintf( buf, MSL, "Target Mobile is: %s [In Room %d]\r\n", quest_target->get_name(), quest_target->in_room->vnum );
         send_to_char( buf, ch );
      }
      else
         send_to_char( "The target mobile is dead!\r\n", ch );

      snprintf( buf, MSL, "Target Object is: %s.\r\n", quest_object->short_descr );
      send_to_char( buf, ch );

      snprintf( buf, MSL, "Quest Object is worth: %d QP, %d Prac, %d to %d Exp, %s\r\n",
               quest_object->value[0], quest_object->value[1],
               static_cast<int>((exp_mob_base(quest_mob->level) * sysdata.killperlev) * 0.02),
               static_cast<int>((exp_mob_base(quest_mob->level) * sysdata.killperlev) * 0.04),
               cost_to_money(quest_object->value[2]) );
      send_to_char( buf, ch );


      snprintf( buf, MSL, "The Quest has been running for %d/15 minutes.\r\n", quest_timer );
      send_to_char( buf, ch );

      return;
   }
   if( !strcmp( argument, "stop" ) )
   {
      if( quest )
      {
         snprintf( buf, MSL,
                  "@@NThe quest has been stopped by an @@mImmortal@@N. Please speak up if you have already gotten the item." );
         do_echo( ch, buf );
         clear_quest(  );
      }
      return;
   }

   if( !strcmp( argument, "start" ) )
   {
      DESCRIPTOR_DATA *d;
      int a = 80;
      int b = 0;
      short player_count = 0, average_level = 0, total_levels = 0;

      /*
       * generate a new quest!
       */
      if( quest )
      {
         send_to_char( "There is already a quest running...\r\n", ch );
         return;
      }

      if( auto_quest )
      {
         auto_quest = FALSE;
         send_to_char( "Automatic Quests now OFF.\r\n", ch );
      }

      /*
       * Work out levels of currently playing folks
       */
      for( d = first_desc; d; d = d->next )
      {
         if( ( d->connected != CON_PLAYING ) || ( IS_IMMORTAL( d->character ) ) )
            continue;
         player_count += 1;
         total_levels += d->character->level;
      }
      average_level = ( ( ( total_levels == 0 ) ? 30 : total_levels ) / ( ( player_count == 0 ) ? 1 : player_count ) );
      a = average_level - 20;
      b = average_level + 20;
      quest_mob = get_quest_giver( a, b );
      if( quest_mob == NULL )
      {
         send_to_char( "Failed to find a quest mob\r\n", ch );
         return;
      }
      b = UMIN( b, 110 );
      a = UMAX( 20, a );

      quest_target = get_quest_target( a, b );
      if( ( quest_target == NULL ) || ( quest_target == quest_mob ) )
      {
         send_to_char( "Failed to find a quest target\r\n", ch );
         return;
      }
      quest_object = load_quest_object( quest_target );
      if( quest_object == NULL )
      {
         send_to_char( "An invalid quest object was encountered.  Check log files.\r\n", ch );
         quest = FALSE;
         return;
      }
      /*
       * Set values on quest item for Qp, Pracs, Exp, Gold
       */
      quest_object->value[0] = UMAX( 1, ( quest_target->level / 30 ) );
      quest_object->value[1] = UMAX( 1, ( quest_target->level / 25 ) );
      quest_object->value[2] = ( quest_target->level * 20 );
      quest_object->value[3] = average_level;

      if( number_percent() < 10 )
       quest_object->value[0] *= 2;
      if( number_percent() < 10 )
       quest_object->value[1] *= 2;
      if( number_percent() < 10 )
       quest_object->value[2] *= 2;

      quest_timer = 0;
      quest = TRUE;
      new_long_desc[0] = '\0';

      quest_mob->long_descr_orig = quest_mob->long_descr;
      snprintf( new_long_desc, MSL, "%s @@Nsays have you found my %s ?", quest_mob->get_name(), quest_object->short_descr );
      quest_mob->long_descr = new_long_desc;
      quest_mob->act.set(ACT_NO_SUMMON );
      quest_mob->act.set(ACT_NO_VISIT );
      quest_mob->act.set(ACT_NO_BLOOD );

      new_long_desc[0] = '\0';

      quest_target->long_descr_orig = quest_target->long_descr;
      snprintf( new_long_desc, MSL, "%s @@Nsays I stole the %s !!!", quest_target->get_name(), quest_object->short_descr );
      quest_target->long_descr = new_long_desc;

      quest_target->act.set(ACT_NO_SUMMON );
      quest_target->act.set(ACT_NO_VISIT );
      quest_target->act.set(ACT_NO_BLOOD );

      send_to_char( "QUEST STARTED!\r\n\r\n", ch );

      snprintf( buf, MSL, "The questing mobile is: %s [In Room %d]\r\n", quest_mob->get_name(), quest_mob->in_room->vnum );
      send_to_char( buf, ch );

      snprintf( buf, MSL, "Target Mobile is: %s [In Room %d]\r\n", quest_target->get_name(), quest_target->in_room->vnum );
      send_to_char( buf, ch );

      snprintf( buf, MSL, "Target Object is: %s.\r\n", quest_object->short_descr );
      send_to_char( buf, ch );

      snprintf( buf, MSL, "Quest Object is worth: %d QP, %d Prac, %d to %d Exp, %s\r\n",
               quest_object->value[0], quest_object->value[1],
               static_cast<int>((exp_mob_base(quest_mob->level) * sysdata.killperlev) * 0.02),
               static_cast<int>((exp_mob_base(quest_mob->level) * sysdata.killperlev) * 0.04),
               cost_to_money(quest_object->value[2]) );
      send_to_char( buf, ch );

      return;
   }
   if( !str_cmp( argument, "auto" ) )
   {
      send_to_char( "AutoQuest now initiated!\r\n", ch );
      auto_quest = TRUE;
      return;
   }


   return;
}



/*
 * get_quest_target : This attempts to pick a random mobile to hold the quest
 * item for the player (questor).  Various checks are made to ensure that the
 * questor has a chance of killing the mobile, etc.
 * Returns NULL if it didn't get a mobile this time.
 */

CHAR_DATA *get_quest_target( int min_level, int max_level )
{
   CHAR_DATA *target = NULL;
   list<CHAR_DATA *>::iterator li;
   int min_index = 0;   /* the minimum number of times to go through the list */

   if( max_level > 140 )
      max_level = 140;
   min_index = number_range(0,mob_index_list.size()-1);

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      target = *li;
      if( !IS_NPC( target ) )
         continue;
      min_index--;

      if( min_index > 0 )
         continue;


      /*
       * Check if mobile is suitable for the quest -
       * Ignore mobs that are likely to be in well known places, such
       * as train/prac mobs, healers, etc
       */

      if( ( target->level < min_level )
          || ( target->level > max_level )
          || ( IS_VAMP( target ) )
          || ( target->in_room->area->flags.test(AFLAG_NOSHOW) )
          || ( target->act.test(ACT_SENTINEL) )
          || ( target->act.test(ACT_PET) )
          || ( !str_cmp( rev_spec_lookup( target->npcdata->spec_fun ), "spec_stephen" ) ) )
         continue;

      /*
       * Lastly, some random choice
       */
      if( number_percent(  ) < 2 )
         break;
   }

   if( !IS_NPC(target) )
    target = NULL;

   return target;
}

/*
 * load_quest_object : This takes a pointer to OBJ_INDEX_DATA and places the
 * object onto the target.
 */

OBJ_DATA *load_quest_object( CHAR_DATA * target )
{
   OBJ_INDEX_DATA *pObj;
   OBJ_DATA *object;
   int foo;

   foo = number_range( OBJ_VNUM_QUEST_MIN, OBJ_VNUM_QUEST_MAX );

   pObj = get_obj_index( foo );

   if( pObj == NULL )
   {
      bug( "load_quest_object : Invalid object vnum %d.", foo );
      return NULL;
   }

   object = create_object( pObj, 1 );
   obj_to_char( object, target );

   return object;
}

CHAR_DATA *get_quest_giver( int min_level, int max_level )
{
   CHAR_DATA *target = NULL;
   list<CHAR_DATA *>::iterator li;
   int min_index = 0;

   min_index = number_range(0,mob_index_list.size()-1);

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      target = *li;
      if( !IS_NPC( target ) )
         continue;
      min_index--;

      if( min_index > 0 )
         continue;


      /*
       * Check if mobile is suitable for the quest -
       * Ignore mobs that are likely to be in well known places, such
       * as train/prac mobs, healers, etc
       */

      if( ( target->level < min_level )
          || ( target->level > max_level )
          || ( IS_VAMP( target ) )
          || ( target->in_room->area->flags.test(AFLAG_NOSHOW) )
          || ( target->act.test(ACT_SENTINEL) )
          || ( target->act.test(ACT_PET) )
          || ( !str_cmp( rev_spec_lookup( target->npcdata->spec_fun ), "spec_stephen" ) )
          || ( !str_cmp( rev_spec_lookup( target->npcdata->spec_fun ), "spec_tax_man" ) ) )

         continue;
      {
         if( number_percent(  ) < 2 )
            break;
      }

   }

   if( !IS_NPC(target) )
    target = NULL;

   return target;
}

/*
 * quest_inform : Makes the questing mobile give out information to the
 * players on the mud.  Starts off real simple, and gets more helpful as
 * time runs out :P
 */

void quest_inform( void )
{
   char buf[MAX_STRING_LENGTH];

   /*
    * Work out what the mob should tell the players....
    */
   /*
    * Add random element to each case so quests look different each time?
    */
   if( quest_timer < 7 )
   {
      snprintf( buf, MSL, qmessages[quest_personality][quest_timer].message1, quest_object->short_descr );
   }
   else
   {
      if( quest_target )
         snprintf( buf, MSL, qmessages[quest_personality][quest_timer].message1,
                  quest_target->get_name(), quest_object->short_descr );
      else
         snprintf( buf, MSL, qmessages[quest_personality][quest_timer].message2, quest_object->short_descr );
   }



   quest_timer++;
   if( quest_mob && quest_timer < 16 )
      do_crusade( quest_mob, buf );
   if( quest_timer == 1 )
   {
      snprintf( buf, MSL, "%s is crusading for %s ", quest_mob->get_name(), quest_object->short_descr );
      info( buf, 5 );
   }
   if( !quest_mob )
   {
      clear_quest(  );
   }
   return;
}

void quest_complete( CHAR_DATA * ch )
{
   char buf[MAX_STRING_LENGTH];

   snprintf( buf, MSL, qmessages[quest_personality][16].message1, ch->get_name(), quest_object->short_descr );
   do_crusade( quest_mob, buf );
   clear_quest(  );
   return;
}


void quest_cancel(  )
{
   if( quest_mob )
      do_crusade( quest_mob, "Shoot! Just forget about recovering ANYTHING for me, ok?" );

   clear_quest(  );
   return;
}

void clear_quest(  )
{
   /*
    * Clear ALL values, ready for next quest
    */

   quest = FALSE;
   extract_obj( quest_object );
   if( quest_mob )
   {
      quest_mob->long_descr = quest_mob->long_descr_orig;
      quest_mob->long_descr_orig.clear();
   }
   if( quest_target )
   {
      quest_target->long_descr = quest_target->long_descr_orig;
      quest_target->long_descr_orig.clear();
   }


   quest_mob = NULL;
   quest_target = NULL;
   quest_object = NULL;
   quest_timer = 0;
   quest_wait = 2 + number_range( 1, 4 );
   quest_personality = 0;

   return;
}

void generate_auto_quest(  )
{
   DESCRIPTOR_DATA *d;
   int hunt_flags = 0;
   char new_long_desc[MAX_STRING_LENGTH];
   short loop_counter = 0;

   int a = 140;
   int b = 0;
   short player_count = 0, average_level = 0, total_levels = 0;

   /*
    * generate a new quest!
    */
   if( quest )
   {
      return;
   }

   /*
    * Work out levels of currently playing folks
    */
   if( first_desc && first_desc->connected == CON_PLAYING )
   {
      for( d = first_desc; d; d = d->next )
      {
         if( d->connected != CON_PLAYING )
            continue;
         if( IS_IMMORTAL(d->character) ) /* Imms shouldn't count against the quest level. --Kline */
            continue;
         player_count++;
         total_levels += d->character->level;
      }
      player_count = UMAX( 1, player_count );
      if( total_levels > 0 ) /* If we don't have any players on, don't want a div by 0 error. --Kline */
       average_level = ( total_levels / player_count );
      else
       average_level = 1;

      quest_mob = NULL;
      quest_target = NULL;

      if( average_level < 20 )
      {
         a = number_range( 5, 25 );
         b = number_range( 30, 45 );
         hunt_flags = HUNT_WORLD | HUNT_OPENDOOR;
         quest_personality = 1;
      }
      else if( average_level < 65 )
      {
         a = number_range( 40, 55 );
         b = number_range( 60, 84 );
         hunt_flags = HUNT_WORLD | HUNT_OPENDOOR | HUNT_PICKDOOR;
         quest_personality = 2;
      }
      else
      {
         a = number_range( 100, 110 );
         b = number_range( 115, 140 );
         hunt_flags = HUNT_WORLD | HUNT_OPENDOOR | HUNT_PICKDOOR | HUNT_UNLOCKDOOR;
         quest_personality = 3;
      }

      while( ( quest_mob == NULL ) && ( loop_counter < 500 ) )
      {
         loop_counter++;
         quest_mob = get_quest_giver( a, b );
         if( ( quest_mob == NULL )
             || ( ( h_find_dir( get_room_index( ROOM_VNUM_TEMPLE ), quest_mob->in_room, hunt_flags ) < 0 ) ) )
            quest_mob = NULL;
      }

      if( quest_mob == NULL )
      {
         quest = FALSE;
         quest_wait = number_range( 1, 3 );
         return;
      }
      loop_counter = 0;
      while( ( quest_target == NULL ) && ( loop_counter < 500 ) )
      {
         loop_counter++;
         quest_target = get_quest_target( a, b );
         if( ( quest_target == NULL )
             || ( ( h_find_dir( get_room_index( ROOM_VNUM_TEMPLE ), quest_target->in_room, hunt_flags ) < 0 ) )
             || ( quest_target == quest_mob ) )
            quest_target = NULL;
      }



      if( quest_target == NULL )
      {
         quest = FALSE;
         quest_wait = number_range( 1, 3 );
         return;
      }
      quest_object = load_quest_object( quest_target );
      if( quest_object == NULL )
      {
         quest = FALSE;
         quest_wait = number_range( 1, 3 );
         return;
      }
      /*
       * Set values on quest item for Qp, Pracs, Exp, Gold
       */
      quest_object->value[0] = UMAX( 1, ( quest_target->level / 20 ) );
      quest_object->value[1] = UMAX( 1, ( quest_target->level / 18 ) );
      quest_object->value[2] = ( quest_target->level * 20 );
      quest_object->value[3] = average_level;

      if( number_percent() < 10 )
       quest_object->value[0] *= 2;
      if( number_percent() < 10 )
       quest_object->value[1] *= 2;
      if( number_percent() < 10 )
       quest_object->value[2] *= 2;

      quest_timer = 0;
      quest = TRUE;
      new_long_desc[0] = '\0';

      quest_mob->long_descr_orig = quest_mob->long_descr;
      snprintf( new_long_desc, MSL, "%s @@Nsays have you found my %s ?", quest_mob->get_name(), quest_object->short_descr );
      quest_mob->long_descr = new_long_desc;
      quest_mob->act.set(ACT_NO_SUMMON );
      quest_mob->act.set(ACT_NO_VISIT );
      quest_mob->act.set(ACT_NO_BLOOD );


      new_long_desc[0] = '\0';

      quest_target->long_descr_orig = quest_target->long_descr;
      snprintf( new_long_desc, MSL, "%s @@Nsays I stole the %s !!!", quest_target->get_name(), quest_object->short_descr );
      quest_target->long_descr = new_long_desc;

      quest_target->act.set(ACT_NO_SUMMON );
      quest_target->act.set(ACT_NO_VISIT );
      quest_target->act.set(ACT_NO_BLOOD );

      return;
   }
}

void crusade_reward( CHAR_DATA *ch )
{
 char buf[MSL];
 int reward = 0;

 ch->pcdata->records->crusade++;

 reward = quest_object->value[0];
 snprintf( buf, MSL, "You receive %d quest points!\r\n", reward );
 send_to_char( buf, ch );
 ch->pcdata->quest_points += reward;
 ch->pcdata->records->qp_tot += reward;
 if( ch->pcdata->quest_points > ch->pcdata->records->qp )
 {
  send_to_char("@@yYou've broken your quest point record!@@N\r\n",ch);
  ch->pcdata->records->qp = ch->pcdata->quest_points;
 }

 reward = quest_object->value[1];
 snprintf( buf, MSL, "You receive %d practices!\r\n", reward );
 send_to_char( buf, ch );
 ch->pcdata->practice += reward;

 reward = (exp_mob_base(quest_mob->level) * sysdata.killperlev);
 reward = number_range(static_cast<int>(reward * 0.02),static_cast<int>(reward * 0.04));
 snprintf( buf, MSL, "You receive %d experience points!\r\n", reward );
 send_to_char( buf, ch );
 ch->exp += reward;

 reward = quest_object->value[2];
 snprintf( buf, MSL, "You receive %s!\r\n", cost_to_money( reward ) );
 send_to_char( buf, ch );
 join_money( round_money( reward, TRUE ), ch->money );

 return;
}
