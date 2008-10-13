/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

/*
 * Original mquest code kindly donated by Abel@StormHunters. Thanks!
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include "mquest.h"

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_MONEY_H
#include "money.h"
#endif

extern int top_mob_index;
extern int top_obj_index;

/* locals */
void      mquest_info              args( ( CHAR_DATA *ch ) );
void      mquest_complete_message  args( ( CHAR_DATA *ch ) );
void      mquest_calc_rewards      args( ( CHAR_DATA *ch ) );
void      mquest_clear             args( ( CHAR_DATA *ch, bool error ) );
void      generate_killing_quest   args( ( CHAR_DATA *ch ) );
CHAR_DATA *get_quest_kill          args( ( int min_lev, int max_lev, CHAR_DATA *ch ) );
void      generate_retrieval_quest args( ( CHAR_DATA *ch ) );
OBJ_DATA  *get_quest_item          args( ( int min_lev, int max_lev, CHAR_DATA *ch ) );

void do_mquest( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *mob;
 OBJ_DATA *obj;
 OBJ_DATA *obj_next;
 char buf[MSL];
 char arg1[MIL]; /* command */
 char arg2[MIL]; /* for hint */

 argument = one_argument(argument,arg1);
 argument = one_argument(argument,arg2);

 if( IS_NPC(ch) )
  return;
 if( get_psuedo_level(ch) < 20 )
 {
  send_to_char("You must be level 20 to request quests from the questmaster.\n\r",ch);
  return;
 }
 if( arg1[0] == '\0' )
 {
  send_to_char("Mquest what? Syntax: mquest <giveup|request|info|complete|hint>\n\r",ch);
  return;
 }
 if( !str_prefix(arg1,"request") )
 {
  for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   if( IS_NPC(mob) && is_set(mob->pIndexData->act,ACT_QUESTMASTER) )
    break;
  if( mob == NULL || !can_see(ch,mob) )
  {
   send_to_char("You have to be at a questmaster!\n\r",ch);
   return;
  }
  if( ch->pcdata->quest_info->is_questing )
  {
   send_to_char("You are already on a quest! Give up on it first if you'd like a new one.\n\r",ch);
   return;
  }
  if( ch->pcdata->quest_info->wait_time > 0 )
  {
   xprintf(buf,"You must wait %d more minute%s before taking another quest.\n\r",ch->pcdata->quest_info->wait_time,ch->pcdata->quest_info->wait_time > 1 ? "s" : "");
   send_to_char(buf,ch);
   return;
  }
  if( number_percent() < 51 )
  {
   send_to_char("Your next available quest is an @@eitem retrieval quest@@N!\n\r",ch);
   if( number_percent() < 70 )
    ch->pcdata->quest_info->quest_type = QUEST_MULTI_RETRIEVE;
   else
    ch->pcdata->quest_info->quest_type = QUEST_RETRIEVAL;
   generate_retrieval_quest(ch);
   ch->pcdata->quest_info->is_questing = TRUE;
  }
  else
  {
   send_to_char("Your next available quest is a @@ecreature killing@@N quest!\n\r",ch);
   if( number_percent() <= 25 )
    ch->pcdata->quest_info->quest_type = QUEST_MULTI_KILL;
   else if( number_percent() <= 50 )
    ch->pcdata->quest_info->quest_type = QUEST_KILLING;
   else if( number_percent() <= 75 )
    ch->pcdata->quest_info->quest_type = QUEST_KILLING_R;
   else
    ch->pcdata->quest_info->quest_type = QUEST_MULTI_KILL_R;
   generate_killing_quest(ch);
   ch->pcdata->quest_info->is_questing = TRUE;
  }
  mquest_calc_rewards(ch);
  mquest_info(ch);
  do_save(ch,"auto");
  return;
 }
 else if( !str_prefix(arg1,"giveup") )
 {
  CHAR_DATA *mob;
  int pqp = 0, pgold = 0, pexp = 0, mn = 0;
  char *cost;
  char change[MSL], mbuf[MSL];

  change[0] = '\0';
  mbuf[0] = '\0';

  for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   if( IS_NPC(mob) && is_set(mob->pIndexData->act,ACT_QUESTMASTER) )
    break;
  if( mob == NULL || !can_see(ch,mob) )
  {
   send_to_char("You have to be at a questmaster!\n\r",ch);
   return;
  }
  if( !ch->pcdata->quest_info->is_questing )
  {
   send_to_char("You are not currently on a quest!\n\r",ch);
   return;
  }

  pqp = UMAX(1,ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP]/3);
  pgold = UMAX(1,ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD]/4);
  pexp = UMAX(1,ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP]/4);

  if( (ch->pcdata->quest_points - pqp) < 0 )
  {
   send_to_char("You do not have enough quest points to give up on this quest!\n\r",ch);
   return;
  }
  if( (money_value(ch->money) - pgold) < 0 )
  {
   send_to_char("You do not have enough gold to give up on this quest!\n\r",ch);
   return;
  }
  if( (ch->exp - pexp) < 0 )
  {
   send_to_char("You do not have enough experience to give up on this quest!\n\r",ch);
   return;
  }

  ch->pcdata->quest_points -= pqp;
  cost = take_best_coins(ch->money,pgold); /* Gives a string of the coins we need to steal. */
  cost = one_argument(cost,change);        /* First arg is change, skip it. Rest is what we take. */
  for( mn = 0; mn < MAX_CURRENCY; mn++ )
  {
   char m_number[MSL], m_name[MSL];
   cost = one_argument(cost,m_number);
   if( m_number[0] == '\0' )
    break;
   cost = one_argument(cost,m_name);
   if( m_name[0] == '\0' )
    break; 
   if( m_number > 0 )
    xcat(mbuf,"%s %s ",m_number,m_name);
  }
  ch->exp -= pexp;

  xprintf(buf,"Giving up on this quest has cost you %d qp, %d exp, %s.\n\r",pqp,pexp,cost_to_money(money_to_value(ch,mbuf)));
  send_to_char(buf,ch);
  ch->pcdata->records->mquest_f++;
  ch->pcdata->quest_info->wait_time = number_range(10,15); /* higher wait time on giveup */
  xprintf(buf,"You must wait %d minutes before your next quest.\n\r",ch->pcdata->quest_info->wait_time);
  send_to_char(buf,ch);
  mquest_clear(ch,FALSE);
  do_save(ch,"auto");
  return;
 }
 else if( !str_prefix(arg1,"hint") )
 {
  short hint = 0;

  if( !ch->pcdata->quest_info->is_questing )
  {
   send_to_char("You are not currently on a quest!\n\r",ch);
   return;
  }
  if( ch->pcdata->quest_info->quest_type == QUEST_KILLING || ch->pcdata->quest_info->quest_type == QUEST_KILLING_R )
   xprintf(arg2,"1");
  hint = atoi(arg2);
  if( !is_number(arg2) )
  {
   send_to_char("You must select a task number for which you want a hint.\n\r",ch);
   return;
  }
  if( hint < 1 || hint > 5 )
  {
   send_to_char("You must choose the number of a task that you currently have!\n\r",ch);
   return;
  }
  if( ch->pcdata->quest_info->amount[hint-1] == -1 )
  {
   send_to_char("You do not currently have a task with that number.\n\r",ch);
   return;
  }
  if( ch->pcdata->quest_info->amount[hint-1] == 0 )
  {
   send_to_char("You have already completed that task!\n\r",ch);
   return;
  }
  if( ch->pcdata->quest_info->quest_hint[hint-1] )
  {
   send_to_char("You've already got your hint for that task!\n\r",ch);
   return;
  }
  if( ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL || ch->pcdata->quest_info->quest_type == QUEST_KILLING )
  {
   MOB_INDEX_DATA *mob = get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[hint-1]);
   xprintf(buf,"%s can be found somewhere in %s.\n\r",mob->short_descr,mob->area->name);
   send_to_char(buf,ch);
  }
  else if( ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL_R || ch->pcdata->quest_info->quest_type == QUEST_KILLING_R )
  {
   CHAR_DATA *mob;
   for( mob = first_char; mob != NULL; mob = mob->next )
   {
    if( !IS_NPC(mob) )
     continue;
    if( mob->in_room == NULL )
     continue;
    if( mob->race == ch->pcdata->quest_info->quest_mob_vnum[hint-1] )
     break;
   }
   xprintf(buf,"%ss can be found somewhere in %s.\n\r",race_table[mob->race].race_title,mob->in_room->area->name);
   send_to_char(buf,ch);
  }
  else if( ch->pcdata->quest_info->quest_type == QUEST_MULTI_RETRIEVE || ch->pcdata->quest_info->quest_type == QUEST_RETRIEVAL )
  {
   OBJ_INDEX_DATA *ob = get_obj_index(ch->pcdata->quest_info->quest_item_vnum[hint-1]);
   xprintf(buf,"%s can be found somewhere in %s.\n\r",ob->short_descr,ob->area->name);
   send_to_char(buf,ch);
  }
  ch->pcdata->quest_info->quest_hint[hint-1] = TRUE;
  send_to_char("Your quest rewards have been reduced for using a hint.\n\r",ch);
  do_save(ch,"auto");
  return;
 }
 else if( !str_prefix(arg1,"info") )
 {
  if( !ch->pcdata->quest_info->is_questing )
  {
   send_to_char("You are not currently on a quest!\n\r",ch);
   if( ch->pcdata->quest_info->wait_time > 0 )
   {
    xprintf(buf,"You must wait %d more minute%s before taking another quest.\n\r",ch->pcdata->quest_info->wait_time,ch->pcdata->quest_info->wait_time > 1 ? "s" : "");
    send_to_char(buf,ch);
   }
   return;
  }
  if( ch->pcdata->quest_info->quest_type != 0 )
  {
   mquest_info(ch);
   return;
  }
 }
 else if( !str_prefix(arg1,"complete") )
 {
  CHAR_DATA *mob;
  short i = 0, x = 0;

  for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   if( IS_NPC(mob) && is_set(mob->pIndexData->act,ACT_QUESTMASTER) )
    break;
  if( mob == NULL )
  {
   send_to_char("You have to be at a questmaster!\n\r",ch);
   return;
  }
  if( !ch->pcdata->quest_info->is_questing )
  {
   send_to_char("You aren't even on a quest! Request one first.\n\r",ch);
   return;
  }

  switch( ch->pcdata->quest_info->quest_type )
  {
   default:
    send_to_char("Invalid quest type. Notifity an immortal.\n\r",ch);
    return;
   case QUEST_RETRIEVAL:
   case QUEST_MULTI_RETRIEVE:
    for( i = 0; i < QUEST_MAX_DATA; i++) /* take objects */
    {
     if( ch->pcdata->quest_info->quest_item_vnum[i] > -1 )
     {
      x = 0;
      for( obj = ch->first_carry; obj != NULL && ch->pcdata->quest_info->amount[i] > 0; obj = obj_next )
      {
       obj_next = obj->next_in_carry_list;
       if( ch->pcdata->quest_info->quest_item_vnum[i] == obj->pIndexData->vnum && obj->wear_loc == WEAR_NONE && !IS_SET(obj->extra_flags,ITEM_UNIQUE) )
       {
        extract_obj(obj);
        x++;
        ch->pcdata->quest_info->amount[i]--;
        if( ch->pcdata->quest_info->amount[i] == 0 )
        {
         xprintf(buf,"You have now handed in all the %s you needed for your quest.\n\r",get_obj_index(ch->pcdata->quest_info->quest_item_vnum[i])->short_descr);
         send_to_char(buf,ch);
        }
       }
      }
      if( x > 0 && ch->pcdata->quest_info->amount[i] != 0 )
      {
       xprintf(buf,"%s (%d) was handed into the questmaster.\n\r",get_obj_index(ch->pcdata->quest_info->quest_item_vnum[i])->short_descr,x);
       send_to_char(buf,ch);
      }
     }
    }
    x = 0;
    for( i = 0; i < QUEST_MAX_DATA; i++ )
     if( ch->pcdata->quest_info->quest_item_vnum[i] > -1 && ch->pcdata->quest_info->amount[i] > 0 )
      x += ch->pcdata->quest_info->amount[i];
    if( x > 0 )
    {
     mquest_info(ch);
     return;
    }
    ch->pcdata->quest_info->quest_complete = TRUE;
    mquest_complete_message(ch);
    do_save(ch,"auto");
    return;
   case QUEST_KILLING:
   case QUEST_MULTI_KILL:
   case QUEST_KILLING_R:
   case QUEST_MULTI_KILL_R:
    x = 0;
    for( i = 0; i < QUEST_MAX_DATA; i++ )
     if( ch->pcdata->quest_info->quest_mob_vnum[i] > -1 && ch->pcdata->quest_info->amount[i] > 0 )
      x += ch->pcdata->quest_info->amount[i];
    if( x > 0 )
    {
     mquest_info(ch);
     return;
    }
    ch->pcdata->quest_info->quest_complete = TRUE;
    mquest_complete_message(ch);
    do_save(ch,"auto");
    return;
  }
 }
 do_mquest(ch,"");
 return;
}

void do_qstat( CHAR_DATA *ch, char *argument )
{
 CHAR_DATA *victim;
 char buf[MSL];
 short i = 0;

 if( argument[0] == '\0' )
 {
  send_to_char("Qstat whom?\n\r",ch);
  return;
 }
 if( (victim = get_char_world(ch,argument)) == NULL )
 {
  send_to_char("They aren't here.\n\r",ch);
  return;
 }
 if( IS_NPC(victim) )
 {
  send_to_char("Not on mobiles.\n\r",ch);
  return;
 }

 xprintf(buf,"@@y%s@@b's current quest status:@@N\n\r",victim->name);
 send_to_char(buf,ch);

 switch( victim->pcdata->quest_info->quest_type )
 {
  default:
   send_to_char("No active quest.\n\r",ch);
   return;
  case QUEST_RETRIEVAL:
   if( victim->pcdata->quest_info->quest_item_vnum[0] > -1 )
   {
    xprintf(buf,"%s is seeking item %d-%s...\n\r",PERS(victim,ch),victim->pcdata->quest_info->quest_item_vnum[0],get_obj_index(victim->pcdata->quest_info->quest_item_vnum[0])->short_descr);
    send_to_char(buf,ch);

    if( victim->pcdata->quest_info->amount[0] > 0 )
    {
     xprintf(buf,"...and must retrieve %d more of them.\n\r",victim->pcdata->quest_info->amount[0]);
     send_to_char(buf,ch);
    }
   }
   else
    send_to_char("Invalid quest item.\n\r",ch);
   break;
  case QUEST_KILLING:
   if( victim->pcdata->quest_info->quest_mob_vnum[0] > -1 )
   {
    xprintf(buf,"%s is seeking mob %d-%s...\n\r",PERS(victim,ch),victim->pcdata->quest_info->quest_mob_vnum[0],get_mob_index(victim->pcdata->quest_info->quest_mob_vnum[0])->short_descr);
    send_to_char(buf,ch);

    if( victim->pcdata->quest_info->amount[0] > 0 )
    {
     xprintf(buf,"...and must slay %d more of them.\n\r",victim->pcdata->quest_info->amount[0]);
     send_to_char(buf,ch);
    }
   }
   else
    send_to_char("Invalid quest mob.\n\r",ch);
   break;
  case QUEST_MULTI_KILL:
   send_to_char("Kill the following creatures (# of times).\n\r",ch);
   for( i = 0; i < QUEST_MAX_DATA; i++ )
   {
    if( victim->pcdata->quest_info->quest_mob_vnum[i] > -1 )
    {
     xprintf(buf,"Target #%d: %5d-%s (%d)\n\r",i+1,victim->pcdata->quest_info->quest_mob_vnum[i],get_mob_index(victim->pcdata->quest_info->quest_mob_vnum[i])->short_descr,victim->pcdata->quest_info->amount[i]);
     send_to_char(buf,ch);
    }
   }
   break;
  case QUEST_MULTI_RETRIEVE:
   send_to_char("Retrieve the following items (# of times).\n\r",ch);
   for( i = 0; i < QUEST_MAX_DATA; i++ )
   {
    if( victim->pcdata->quest_info->quest_item_vnum[i] > -1 )
    {
     xprintf(buf,"Item #%d: %5d-%s (%d)\n\r",i+1,victim->pcdata->quest_info->quest_item_vnum[i],get_obj_index(victim->pcdata->quest_info->quest_item_vnum[i])->short_descr,victim->pcdata->quest_info->amount[i]);
     send_to_char(buf,ch);
    }
   }
   break;
  case QUEST_KILLING_R:
   if( victim->pcdata->quest_info->quest_mob_vnum[0] > -1 )
   {
    xprintf(buf,"%s is seeking %s mobs...\n\r",PERS(victim,ch),race_table[victim->pcdata->quest_info->quest_mob_vnum[0]].race_title);
    send_to_char(buf,ch);

    if( victim->pcdata->quest_info->amount[0] > 0 )
    {
     xprintf(buf,"...and must slay %d more of them.\n\r",victim->pcdata->quest_info->amount[0]);
     send_to_char(buf,ch);
    }
   }
   else
    send_to_char("Invalid quest race.\n\r",ch);
   break;
  case QUEST_MULTI_KILL_R:
   send_to_char("Kill mobs of the following races (# of times).\n\r",ch);
   for( i = 0; i < QUEST_MAX_DATA; i++ )
   {
    if( victim->pcdata->quest_info->quest_mob_vnum[i] > -1 )
    {
     xprintf(buf,"Target #%d: %s (%d)\n\r",i+1,race_table[victim->pcdata->quest_info->quest_mob_vnum[i]].race_title,victim->pcdata->quest_info->amount[i]);
     send_to_char(buf,ch);
    }
   }
   break;
 }
 xprintf(buf,"QP:   %d\n\rGold: %s\n\rExp:  %d\n\r",victim->pcdata->quest_info->quest_reward[QUEST_REWARD_QP],cost_to_money(victim->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD]),victim->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP]);
 send_to_char(buf,ch);
 return;
}

void mquest_info( CHAR_DATA *ch )
{
 char buf[MSL];
 short i =0 ;

 switch( ch->pcdata->quest_info->quest_type )
 {
  default:
   xprintf(buf,"quest_type is a value of %d, how'd that happen? Tell an imm, thanks!\n\r",ch->pcdata->quest_info->quest_type);
   send_to_char(buf,ch);
   break;
  case QUEST_RETRIEVAL:
   if( ch->pcdata->quest_info->quest_item_vnum[0] > -1 )
   {
    xprintf(buf,"You are seeking the item known as %s@@N.\n\r",get_obj_index(ch->pcdata->quest_info->quest_item_vnum[0])->short_descr);
    send_to_char(buf,ch);

    if( ch->pcdata->quest_info->amount[0] > 0 )
    {
     xprintf(buf,"Your quest contract states that you must retrieve %d more of them.\n\r",ch->pcdata->quest_info->amount[0]);
     send_to_char(buf,ch);
     xprintf(buf,"@@eHint:@@N They can be found in %s@@N!\n\r",get_obj_index(ch->pcdata->quest_info->quest_item_vnum[i])->area->name);
     if( ch->pcdata->quest_info->quest_hint[0] )
      send_to_char(buf,ch);
    }
   }
   else
    send_to_char("Invalid quest item.\n\r",ch);
   break;
  case QUEST_KILLING:
   if( ch->pcdata->quest_info->quest_mob_vnum[0] > -1 )
   {
    xprintf(buf,"You are seeking out the creature known as %s@@N.\n\r",get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[0])->short_descr);
    send_to_char(buf,ch);

    if( ch->pcdata->quest_info->amount[0] > 0 )
    {
     xprintf(buf,"Your quest contract states that you must slay %d more of them.\n\r",ch->pcdata->quest_info->amount[0]);
     send_to_char(buf,ch);
     xprintf(buf,"@@eHint:@@N They can be found in %s@@N!\n\r",get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->area->name);
     if( ch->pcdata->quest_info->quest_hint[0] )
      send_to_char(buf,ch);
    }
    else
     send_to_char("You have fulfilled the kill requirements outlined in your contract.\n\r",ch);
   }
   else
    send_to_char("Invalid quest mob.\n\r",ch);
   break;
  case QUEST_MULTI_KILL:
   send_to_char("You must hunt down and destroy the following creatures (# of times).\n\r",ch);
   for( i = 0; i < QUEST_MAX_DATA; i++ )
   {
    if( ch->pcdata->quest_info->quest_mob_vnum[i] > -1 )
    {
     xprintf(buf,"Target #%d: %s (%d)",i+1,get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->short_descr,ch->pcdata->quest_info->amount[i]);
     send_to_char(buf,ch);
     if( ch->pcdata->quest_info->quest_hint[i] )
      xprintf(buf," [%s]\n\r",get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->area->name);
     else
      xprintf(buf,"\n\r");
     send_to_char(buf,ch);
    }
   }
   break;
  case QUEST_MULTI_RETRIEVE:
   send_to_char("You must retrieve the following items from across the land (# of times).\n\r",ch);
   for( i = 0; i < QUEST_MAX_DATA; i++ )
   {
    if( ch->pcdata->quest_info->quest_item_vnum[i] > -1 )
    {
     xprintf(buf,"Item #%d: %s (%d)",i+1,get_obj_index(ch->pcdata->quest_info->quest_item_vnum[i])->short_descr,ch->pcdata->quest_info->amount[i]);
     send_to_char(buf,ch);
     if( ch->pcdata->quest_info->quest_hint[i] )
      xprintf(buf," [%s]\n\r",get_obj_index(ch->pcdata->quest_info->quest_item_vnum[i])->area->name);
     else
      xprintf(buf,"\n\r");
     send_to_char(buf,ch);
    }
   }
   break;
  case QUEST_KILLING_R:
   if( ch->pcdata->quest_info->quest_mob_vnum[0] > -1 )
   {
    xprintf(buf,"You are seeking out %s creatures.\n\r",race_table[ch->pcdata->quest_info->quest_mob_vnum[0]].race_title);
    send_to_char(buf,ch);

    if( ch->pcdata->quest_info->amount[0] > 0 )
    {
     CHAR_DATA *mob;
     xprintf(buf,"Your quest contract states that you must slay %d more of them.\n\r",ch->pcdata->quest_info->amount[0]);
     send_to_char(buf,ch);
     for( mob = first_char; mob != NULL; mob = mob->next )
     {
      if( !IS_NPC(mob) )
       continue;
      if( mob->in_room == NULL )
       continue;
      if( mob->race == ch->pcdata->quest_info->quest_mob_vnum[0] )
       break;
     }
     if( mob != NULL )
     {
      xprintf(buf,"@@eHint:@@N They can be found in %s@@N!\n\r",mob->in_room->area->name);
      if( ch->pcdata->quest_info->quest_hint[0] )
       send_to_char(buf,ch);
     }
    }
    else
     send_to_char("You have fulfilled the kill requirements outlined in your contract.\n\r",ch);
   }
   else
    send_to_char("Invalid quest mob race.\n\r",ch);
   break;
  case QUEST_MULTI_KILL_R:
   send_to_char("You must hunt down and destroy the following types of creatures (# of times).\n\r",ch);
   for( i = 0; i < QUEST_MAX_DATA; i++ )
   {
    if( ch->pcdata->quest_info->quest_mob_vnum[i] > -1 )
    {
     CHAR_DATA *mob;
     for( mob = first_char; mob != NULL; mob = mob->next )
     {
      if( !IS_NPC(mob) )
       continue;
      if( mob->in_room == NULL )
       continue;
      if( mob->race == ch->pcdata->quest_info->quest_mob_vnum[i] )
       break;
     }
     xprintf(buf,"Target #%d: %s (%d)",i+1,race_table[ch->pcdata->quest_info->quest_mob_vnum[i]].race_title,ch->pcdata->quest_info->amount[i]);
     send_to_char(buf,ch);
     if( ch->pcdata->quest_info->quest_hint[i] && mob != NULL )
      xprintf(buf," [%s]\n\r",mob->in_room->area->name);
     else
      xprintf(buf,"\n\r");
     send_to_char(buf,ch);
    }
    else
     send_to_char("Invalid quest mob race.\n\r",ch);
   }
   break;
 }
 return;
}

void mquest_complete_message( CHAR_DATA *ch )
{
 char buf[MSL];
 short hint = 0, i = 0;
 float tmp = 0;

 if( IS_NPC(ch) )
  return;
 if( ch->pcdata->quest_info->quest_complete )
 {
  for( i = 0; i < QUEST_MAX_DATA; i++ )
   if( ch->pcdata->quest_info->quest_hint[i] )
    hint++;

  if( hint > 0 )
  {
   xprintf(buf,"Quest reward reduced for the use of %d hint%s.\n\r",hint,hint > 1 ? "s" : "");
   send_to_char(buf,ch);
  }

  while( hint > 0 )
  {
   tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP];
   tmp *= 0.90;
   ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP] = (int)tmp;
   tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD];
   tmp *= 0.90;
   ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] = (int)tmp;
   tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP];
   tmp *= 0.90;
   ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = (int)tmp;
   hint--;
  }

  xprintf(buf,"Quest Complete. You have earned %d qp, %d experience, %s!\n\r",ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP],ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP],cost_to_money(ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD]));
  send_to_char(buf,ch);

  ch->pcdata->quest_points += ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP];
  join_money(round_money(ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD],TRUE),ch->money);
  ch->exp += ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP];

  ch->pcdata->records->mquest_c++;
  ch->pcdata->records->qp_tot += ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP];

  if( ch->pcdata->records->mquest_c % 50 == 0 )   /* every time a person completes 50 quests */
  {
   send_to_char("@@eCongratulations! You've succesfully completed 50 questmaster quests!\n\r",ch);
   send_to_char("@@eYou have been rewarded a bonus 25 practices and 40 quest points!\n\r",ch);
   ch->practice += 25;
   ch->pcdata->quest_points += 40;
   ch->pcdata->records->qp_tot += 40;
  }
  if( ch->pcdata->quest_points > ch->pcdata->records->qp )
  {
   send_to_char("@@yYou've broken your quest point record!@@N\n\r",ch);
   ch->pcdata->records->qp = ch->pcdata->quest_points;
  }
  mquest_clear(ch,FALSE);
 }
 else
  send_to_char("You haven't completed the quest's requirements yet.\n\r",ch);
 return;
}

void mquest_calc_rewards( CHAR_DATA *ch )
{
 short clev, tot_amt, tot_tsk;
 int exp;
 float mod = 1.00, tmp = 0;

 if( IS_NPC(ch) )
  return;

 tot_amt = 0;
 tot_tsk = 0;
 clev = get_psuedo_level(ch);
 exp = (exp_mob_base(clev) * sysdata.killperlev);
 exp = number_range((int)(exp * 0.04),(int)(exp * 0.08));

 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP] = number_range((int)(clev / 40),(int)(clev / 20));
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] = number_range((int)(clev * 15),(int)(clev * 30));

 if( is_adept(ch) )
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = number_range((int)(exp_to_level_adept(ch) * 0.04),(int)(exp_to_level_adept(ch) * 0.08));
 else
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = exp;

 /* Add some randomness */
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP] += number_range(-1,1);
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] = number_range((int)(ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] * 0.9),(int)(ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] * 1.1));
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = number_range((int)(ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] * 0.9),(int)(ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] * 1.1));

 for( tot_tsk = 0; tot_tsk < QUEST_MAX_DATA; tot_tsk++ )
 {
  if( ch->pcdata->quest_info->amount[tot_tsk] == -1 )
   break;
  else
   tot_amt += ch->pcdata->quest_info->amount[tot_tsk];
 }

 if( ch->pcdata->quest_info->quest_type == QUEST_MULTI_RETRIEVE )
 {
  if( tot_tsk >= 3 )
   mod += ((tot_tsk - 3) * 0.1);
  else
   mod -= 0.2;
 }
 else if( ch->pcdata->quest_info->quest_type == QUEST_RETRIEVAL )
 {
  if( tot_amt >= 3 )
   mod += ((tot_amt - 3) * 0.1);
  else
   mod -= 0.2;
 }
 else if( ch->pcdata->quest_info->quest_type == QUEST_KILLING || ch->pcdata->quest_info->quest_type == QUEST_KILLING_R )
 {
  if( tot_amt >= 3 )
   mod += ((tot_amt - 4) * 0.1);
  else
   mod -= 0.2;
 }
 else if( ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL || ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL_R )
 {
  if( tot_tsk >= 3 )
   mod += ((tot_tsk - 3 ) * 0.1);
  else
   mod -= 0.2;
  mod += ((tot_amt - 10) * 0.02);
 }
 else
 {
  send_to_char("An error has occured calculating rewards. This should not happen.\n\r",ch);
  ch->pcdata->quest_info->wait_time = 0;
  mquest_clear(ch,TRUE);
  return;
 }

 tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP];
 tmp *= mod;
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP] = (int)tmp;
 tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD];
 tmp *= mod;
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] = (int)tmp;
 tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP];
 tmp *= mod;
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = (int)tmp;
       
 /* Bonus! */
 if( number_percent() < 5 )
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP] += 2;
 if( number_percent() < 5 )
 {
  tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD];
  tmp *= 1.5;
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] = (int)tmp;
 }
 if( number_percent() < 5 )
 {
  tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP];
  tmp *= 1.5;
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = (int)tmp;
 }

 /* No negative rewards */
 if( ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP] < 0 )
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP] = 0;
 if( ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] < 0 )
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] = 0;
 if( ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] < 0 )
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = 0;

 return;
}

void mquest_clear( CHAR_DATA *ch, bool error )
{
 int i = 0;
 char buf[MSL];

 if( IS_NPC(ch) )
  return;

 for( i = 0; i < QUEST_MAX_DATA; i++ )
  ch->pcdata->quest_info->amount[i] = -1;
 ch->pcdata->quest_info->is_questing = FALSE;
 ch->pcdata->quest_info->quest_complete = FALSE;
 for( i = 0; i < QUEST_MAX_DATA; i++ )
  ch->pcdata->quest_info->quest_hint[i] = FALSE;
 for( i = 0; i < QUEST_MAX_DATA; i++ )
  ch->pcdata->quest_info->quest_item_vnum[i] = -1;
 for( i = 0; i < QUEST_MAX_DATA; i++ )
  ch->pcdata->quest_info->quest_mob_vnum[i] = -1;
 for( i = 0; i < QUEST_MAX_REWARD; i++ )
  ch->pcdata->quest_info->quest_reward[i] = 0;
 ch->pcdata->quest_info->quest_type = 0;
 ch->pcdata->quest_info->time_left = 0;
 if( ch->pcdata->quest_info->wait_time == 0 && !error )
 {
  ch->pcdata->quest_info->wait_time = 5;
  xprintf(buf,"You must wait %d minutes before your next quest.\n\r",ch->pcdata->quest_info->wait_time);
  send_to_char(buf,ch);
 }
 do_save(ch,"auto");
 return;
}

void generate_killing_quest( CHAR_DATA *ch )
{
 CHAR_DATA *mob;
 short max, i, x;
 bool redo = FALSE, race = FALSE;
 short min_lev, max_lev;
 short clev;

 if( IS_NPC(ch) )
  return;

 if( ch->pcdata->quest_info->quest_type == QUEST_KILLING_R || ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL_R )
  race = TRUE;

 clev = get_psuedo_level(ch);
 min_lev = (int)(clev * 0.7);
 max_lev = (int)(clev * 1.1);

 if( ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL || ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL_R )
  max = number_range(3,5);
 else
  max = 1;

 if( clev <= 80 && max_lev > 80 )
  max_lev = 80;
 else if( clev < 110 && max_lev > 110 )
  max_lev = 110;
 else if( clev == 120 && min_lev <110 )
  min_lev = 110;

 min_lev = UMAX(0,min_lev);
 max_lev = UMIN(140,max_lev);

 for( i = 0; i < max; i++ )
 {
  mob = NULL;
  mob = get_quest_kill(min_lev,max_lev,ch);

  if( !mob && !race )
  {
   min_lev -= 10;
   max_lev -= 10;
   i--;
   if( min_lev < 1 || max_lev < 1 )
   {
    send_to_char("Mquest error. Unable to find suitible mobs. Aborting.\n\r",ch);
    ch->pcdata->quest_info->wait_time = 0;
    mquest_clear(ch,TRUE);
    return;
   }
   continue;
  }

  x = 0;
  if( !race )
   ch->pcdata->quest_info->quest_mob_vnum[i] = mob->pIndexData->vnum;
  else
  {
   ch->pcdata->quest_info->quest_mob_vnum[i] = number_range(0,MAX_RACE-1);
   if( number_percent() < 30 )
    ch->pcdata->quest_info->amount[i] = 1;
   else
    ch->pcdata->quest_info->amount[i] = (2 + number_range(0,4));
  }
 
  if( !race )
  {
   if( redo ) /* If it just failed the same-area check, don't check again, check vnums. */
   {
    redo = FALSE; /* Prevent crashing due to infinite loop. */

    for( x = 0; x < i; x++ )
     if( (ch->pcdata->quest_info->quest_mob_vnum[i] == ch->pcdata->quest_info->quest_mob_vnum[x]) && (x != i) )
      redo = TRUE; /* Same vnum, don't allow it. Don't redo same-area check though. */
   }
   else
   {
    for( x = 0; x < i; x++ )
     if( !str_cmp(get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->area->name,get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[x])->area->name) && (x != i) )
      redo = TRUE;
   }
   if( !redo )
   {
    if( number_percent() < 30 )
     ch->pcdata->quest_info->amount[i] = 1;
    else
    {
     if( is_set(get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->act,ACT_SENTINEL) )
      ch->pcdata->quest_info->amount[i] = 1;
     else
      ch->pcdata->quest_info->amount[i] = (2 + number_range(0,4));
    }
   }
   else
   { /* Same area detected. Run it again. */
    ch->pcdata->quest_info->quest_mob_vnum[i] = -1;
    ch->pcdata->quest_info->amount[i] = -1;
    i--;
   }
  }
 }
 if( ch->pcdata->quest_info->quest_type == QUEST_KILLING || ch->pcdata->quest_info->quest_type == QUEST_KILLING_R )
  for( i = 1; i < QUEST_MAX_DATA; i++ )
  {
   ch->pcdata->quest_info->quest_mob_vnum[i] = -1;
   ch->pcdata->quest_info->amount[i] = -1;
  }
 return;
}

CHAR_DATA *get_quest_kill( int min_lev, int max_lev, CHAR_DATA *ch )
{
 CHAR_DATA *mob;
 int i = number_range(0,top_mob_index-1);

 for( mob = first_char; mob != NULL; mob  = mob->next )
 {
  i--;

  if( i > 0 )  /* Add some variety to where we start in the ch_list */
   continue;
  if( !IS_NPC(mob) )
   continue;

  if( (get_psuedo_level(mob) < min_lev || get_psuedo_level(mob) > max_lev)
   || (mob->in_room->area->min_level > get_psuedo_level(ch) || mob->in_room->area->max_level < get_psuedo_level(ch))
   || (IS_SET(mob->in_room->area->flags,AREA_NOSHOW) || IS_SET(mob->in_room->area->flags,AREA_NO_QUEST))
   || (!str_cmp(rev_spec_lookup(mob->spec_fun),"spec_cast_adept"))
   || (!str_cmp(rev_spec_lookup(mob->spec_fun),"spec_executioner"))
   || (mob->pIndexData->vnum > mob->in_room->area->max_vnum || mob->pIndexData->vnum < mob->in_room->area->min_vnum)
   || (is_set(mob->in_room->room_flags,RFLAG_SAFE))
   || (IS_AFFECTED(mob,AFF_CHARM))
   || (is_set(mob->act,ACT_PET))
   || (is_set(mob->act,ACT_NO_QUEST)))
    continue;

  if( number_percent() < 5 ) /* Add variety to actually selecting the first ch we find in the ch_list */
   return mob;
 }
 return mob;
}

void generate_retrieval_quest( CHAR_DATA *ch )
{
 OBJ_DATA *obj;
 short max, i, x;
 bool redo=FALSE;
 short min_lev, max_lev;
 short clev;
 
 if( IS_NPC(ch) )
  return;

 clev = get_psuedo_level(ch);
 min_lev = (int)(clev * 0.7);
 max_lev = (int)(clev * 1.1);

 if(ch->pcdata->quest_info->quest_type == QUEST_MULTI_RETRIEVE )
  max = number_range(3,5);
 else
  max = 1;

 if( clev <= 80 && max_lev > 80 )
  max_lev = 80;
 else if( clev < 110 && max_lev > 110 )
  max_lev = 110;
 else if( clev == 120 && min_lev <110 )
  min_lev = 110;

 min_lev = UMAX(0,min_lev);
 max_lev = UMIN(140,max_lev);

 for( i = 0; i < max; i++ )
 {
  obj = NULL;
  obj = get_quest_item(min_lev,max_lev,ch);

  if( !obj )
  {
   min_lev -= 10;
   max_lev -= 10;
   i--;
   if( min_lev < 1 || max_lev < 1 )
   {
    send_to_char("Mquest error. Unable to find suitible objs. Aborting.\n\r",ch);
    ch->pcdata->quest_info->wait_time = 0;
    mquest_clear(ch,TRUE);
    return;
   }
   continue;
  }

  ch->pcdata->quest_info->quest_item_vnum[i] = obj->pIndexData->vnum;

  if( redo ) /* If it just failed the same-area check, don't check again, check vnums. */
  {
   redo = FALSE; /* Prevent crashing due to infinite loop. */

   for( x = 0; x < i; x++ )
    if( (ch->pcdata->quest_info->quest_item_vnum[i] == ch->pcdata->quest_info->quest_item_vnum[x]) && (x != i) )
     redo = TRUE; /* Same vnum, don't allow it. Don't redo same-area check though. */
  }
  else
  {
   for( x = 0; x < i; x++ )
    if( !str_cmp(get_obj_index(ch->pcdata->quest_info->quest_item_vnum[i])->area->name,get_obj_index(ch->pcdata->quest_info->quest_item_vnum[x])->area->name) && (x != i) )
     redo = TRUE;
  }
  if( !redo )
  {
   if( ch->pcdata->quest_info->quest_type == QUEST_MULTI_RETRIEVE )
    ch->pcdata->quest_info->amount[i] = 1;
   else
    ch->pcdata->quest_info->amount[i] = (2 + number_range(0,4));
  }
  else
  { /* Same area detected. Run it again. */
   ch->pcdata->quest_info->quest_item_vnum[i] = -1;
   ch->pcdata->quest_info->amount[i] = -1;
   i--;
  }
 }
 return;
}

OBJ_DATA *get_quest_item( int min_lev, int max_lev, CHAR_DATA *ch )
{
 OBJ_DATA *obj;
 int i = number_range(0,top_obj_index-1);

 for( obj = first_obj; obj != NULL; obj = obj->next )
 {
  i--;

  if( i > 0 ) /* Add some variety to where we start in the obj_list */
   continue;

  if( obj->in_room )
  {
   if( (IS_SET(obj->in_room->area->flags,AREA_NO_QUEST) || IS_SET(obj->in_room->area->flags,AREA_NOSHOW)) /* Hide places we don't want to use. */
    || (obj->in_room->area->min_level > get_psuedo_level(ch) || obj->in_room->area->max_level < get_psuedo_level(ch))
    || (obj->pIndexData->vnum > obj->in_room->area->max_vnum || obj->pIndexData->vnum < obj->in_room->area->min_vnum) /* outside obj's own area */
    || (!IS_SET(obj->wear_flags,ITEM_TAKE)) /* ensure we can pick it up */
    || (IS_SET(obj->extra_flags,ITEM_RARE)) /* don't waste rares */
    || (obj->item_type == ITEM_QUEST) /* no autoquest items */
    || (obj->item_type == ITEM_MONEY)
    || (IS_SET(obj->extra_flags,ITEM_NO_QUEST))
    || (obj->item_type == ITEM_PIECE)
    || (IS_SET(obj->extra_flags,ITEM_UNIQUE))
    || (obj->carried_by != NULL && (!IS_NPC(obj->carried_by)
    || (IS_NPC(obj->carried_by) && obj->carried_by->pIndexData->pShop != NULL)))  /* held by PC or held by NPC that is a shopkeeper */
    || (obj->weight > 15)
    || (obj->level > max_lev)
    || (obj->level < min_lev))
     continue;

   if( number_percent() < 5 ) /* Add variety to actually selecting the first obj we find in the obj_list */
    return obj;
  }
  else if( obj->carried_by && obj->carried_by->in_room )
  {
   if( (IS_SET(obj->carried_by->in_room->area->flags,AREA_NO_QUEST) || IS_SET(obj->carried_by->in_room->area->flags,AREA_NOSHOW)) /* Hide places we don't want to use. */
    || (obj->carried_by->in_room->area->min_level > get_psuedo_level(ch) || obj->carried_by->in_room->area->max_level < get_psuedo_level(ch))
    || (obj->pIndexData->vnum > obj->carried_by->in_room->area->max_vnum || obj->pIndexData->vnum < obj->carried_by->in_room->area->min_vnum) /* outside obj's own area */
    || (!IS_SET(obj->wear_flags,ITEM_TAKE)) /* ensure we can pick it up */
    || (IS_SET(obj->extra_flags,ITEM_RARE)) /* don't waste rares */
    || (obj->item_type == ITEM_QUEST) /* no autoquest items */
    || (obj->item_type == ITEM_MONEY)
    || (IS_SET(obj->extra_flags,ITEM_NO_QUEST))
    || (obj->item_type == ITEM_PIECE)
    || (IS_SET(obj->extra_flags,ITEM_UNIQUE))
    || (obj->carried_by != NULL && (!IS_NPC(obj->carried_by)
    || (IS_NPC(obj->carried_by) && obj->carried_by->pIndexData->pShop != NULL))) /* held by PC or held by NPC that is a shopkeeper */
    || (obj->weight > 15)
    || (obj->level > max_lev)
    || (obj->level < min_lev))
     continue;

   if( number_percent() < 5 ) /* Add variety to actually selecting the first obj we find in the obj_list */
    return obj;
  }
 }
 return obj;
}

char *display_mob_target( CHAR_DATA *ch, CHAR_DATA *victim )
{
 if( IS_NPC(ch) || !IS_NPC(victim) )
  return "";
 if( ch->pcdata->quest_info->is_questing && (ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL || ch->pcdata->quest_info->quest_type == QUEST_KILLING) )
 {
  short i = 0;

  /* Ugly to workaround same-descr mobs with different vnums for repop purposes */
  for( i = 0; i < QUEST_MAX_DATA; i++ )
   if( ch->pcdata->quest_info->quest_mob_vnum[i] > -1
    && ch->pcdata->quest_info->amount[i] > 0
    && (ch->pcdata->quest_info->quest_mob_vnum[i] == victim->pIndexData->vnum
    || (!str_cmp(get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->short_descr,victim->short_descr) 
    && get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->area == victim->in_room->area)) )
     return "@@e[@@yTARGET@@e] @@N";
 }
 if( ch->pcdata->quest_info->is_questing && (ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL_R || ch->pcdata->quest_info->quest_type == QUEST_KILLING_R) )
 {
  short i = 0;

  for( i = 0; i < QUEST_MAX_DATA; i++ )
   if( ch->pcdata->quest_info->quest_mob_vnum[i] > -1
    && ch->pcdata->quest_info->amount[i] > 0
    && ch->pcdata->quest_info->quest_mob_vnum[i] == victim->race
    && (get_psuedo_level(ch) < get_psuedo_level(victim) || (get_psuedo_level(victim) - get_psuedo_level(ch)) >= -10) )
     return "@@e[@@yTARGET@@e] @@N";
 }
 return "";
}

char *display_obj_target( CHAR_DATA *ch, OBJ_DATA *obj )
{
 if( IS_NPC(ch) )
  return "";
 if( ch->pcdata->quest_info->is_questing && (ch->pcdata->quest_info->quest_type == QUEST_MULTI_RETRIEVE || ch->pcdata->quest_info->quest_type == QUEST_RETRIEVAL) )
 {
  short i = 0;

  for( i = 0; i < QUEST_MAX_DATA; i++ )
   if( ch->pcdata->quest_info->quest_item_vnum[i] > -1
    && ch->pcdata->quest_info->quest_item_vnum[i] == obj->pIndexData->vnum
    && ch->pcdata->quest_info->amount[i] > 0 )
     return "@@e[@@yTARGET@@e] @@N";
 }
 return "";
}

void update_mquest_wait_time( CHAR_DATA *ch )
{
 if( IS_NPC(ch) )
  return;
 if( !ch->pcdata->quest_info->is_questing && ch->pcdata->quest_info->wait_time > 0 && ch->desc != NULL )
 {
  ch->pcdata->quest_info->wait_time--;
  if( ch->pcdata->quest_info->wait_time == 0 )
   send_to_char("You can now get another quest from the questmaster!\n\r",ch);
 }
 return;
}

void update_mquest_kill( CHAR_DATA *ch, CHAR_DATA *victim )
{
 short i, tot = 0;

 if( IS_NPC(ch) || !IS_NPC(victim) )
  return;
 if( ch->pcdata->quest_info->is_questing && (ch->pcdata->quest_info->quest_type == QUEST_KILLING || ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL) && !ch->pcdata->quest_info->quest_complete )
 {
  for( i = 0; i < QUEST_MAX_DATA; i++ )
  {
   if( ch->pcdata->quest_info->quest_mob_vnum[i] < 0 )
    continue;
   if( ch->pcdata->quest_info->amount[i] > 0
    && (ch->pcdata->quest_info->quest_mob_vnum[i] == victim->pIndexData->vnum
    || (!str_cmp(get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->short_descr,victim->short_descr)
    && get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->area == victim->in_room->area)) )
   {
    ch->pcdata->quest_info->amount[i]--;
    send_to_char("You have completed a quest kill!\n\r",ch);
    if( ch->pcdata->quest_info->amount[i] == 0 )
     send_to_char("You have completed the kill requirement for this creature!\n\r",ch);
   }
   if( ch->pcdata->quest_info->amount[i] > -1 )
    tot += ch->pcdata->quest_info->amount[i];
  }
  if( tot == 0 ) /* If the total is 0 after checking through the whole loop, then they've completed their quest. */
  {
   ch->pcdata->quest_info->quest_complete = TRUE;
   send_to_char("You have completed your quest! Return to the quest master.\n\r",ch);
  }
 }
 if( ch->pcdata->quest_info->is_questing && (ch->pcdata->quest_info->quest_type == QUEST_KILLING_R || ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL_R) && !ch->pcdata->quest_info->quest_complete )
 {
  for( i = 0; i < QUEST_MAX_DATA; i++ )
  {
   if( ch->pcdata->quest_info->quest_mob_vnum[i] < 0 )
    continue;
   if( ch->pcdata->quest_info->amount[i] > 0 && (ch->pcdata->quest_info->quest_mob_vnum[i] == victim->race) && (get_psuedo_level(ch) < get_psuedo_level(victim) || (get_psuedo_level(victim) - get_psuedo_level(ch)) >= -10) )
   {
    ch->pcdata->quest_info->amount[i]--;
    send_to_char("You have completed a quest kill!\n\r",ch);
    if( ch->pcdata->quest_info->amount[i] == 0 )
     send_to_char("You have completed the kill requirement for this race!\n\r",ch);
   }
   if( ch->pcdata->quest_info->amount[i] > -1 )
    tot += ch->pcdata->quest_info->amount[i];
  }
  if( tot == 0 ) /* If the total is 0 after checking through the whole loop, then they've completed their quest. */
  {
   ch->pcdata->quest_info->quest_complete = TRUE;
   send_to_char("You have completed your quest! Return to the quest master.\n\r",ch);
  }
 }
 return;
}
