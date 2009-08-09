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

#include "h/globals.h"

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
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

#ifndef DEC_MACROS_H
#include "h/macros.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_SPEC_FUN_H
#include "h/spec_fun.h"
#endif

DO_FUN(do_mquest)
{
 CHAR_DATA *mob = NULL;
 OBJ_DATA *obj;
 OBJ_DATA *obj_next;
 char buf[MSL];
 char arg1[MIL]; /* command */
 char arg2[MIL]; /* for hint */

 argument = one_argument(argument,arg1);
 argument = one_argument(argument,arg2);

 if( IS_NPC(ch) )
  return;
 if( ch->get_level("psuedo") < 20 )
 {
  send_to_char("You must be level 20 to request quests from the questmaster.\r\n",ch);
  return;
 }
 if( arg1[0] == '\0' )
 {
  send_to_char("Mquest what? Syntax: mquest <giveup|request|info|complete|hint>\r\n",ch);
  return;
 }
 if( !str_prefix(arg1,"request") )
 {
  for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   if( IS_NPC(mob) && mob->npcdata->pIndexData->act.test(ACT_QUESTMASTER) )
    break;
  if( mob == NULL || !can_see(ch,mob) )
  {
   send_to_char("You have to be at a questmaster!\r\n",ch);
   return;
  }
  if( ch->pcdata->quest_info->is_questing )
  {
   send_to_char("You are already on a quest! Give up on it first if you'd like a new one.\r\n",ch);
   return;
  }
  if( ch->pcdata->quest_info->wait_time > 0 )
  {
   snprintf(buf,MSL,"You must wait %d more minute%s before taking another quest.\r\n",ch->pcdata->quest_info->wait_time,ch->pcdata->quest_info->wait_time > 1 ? "s" : "");
   send_to_char(buf,ch);
   return;
  }
  if( number_percent() < 51 )
  {
   send_to_char("Your next available quest is an @@eitem retrieval quest@@N!\r\n",ch);
   if( number_percent() < 70 )
    ch->pcdata->quest_info->quest_type = QUEST_MULTI_RETRIEVE;
   else
    ch->pcdata->quest_info->quest_type = QUEST_RETRIEVAL;
   generate_retrieval_quest(ch);
   ch->pcdata->quest_info->is_questing = TRUE;
  }
  else
  {
   send_to_char("Your next available quest is a @@ecreature killing@@N quest!\r\n",ch);
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
  int pqp = 0, pgold = 0, pexp = 0, mn = 0;
  char *cost;
  char change[MSL], mbuf[MSL];

  change[0] = '\0';
  mbuf[0] = '\0';

  for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   if( IS_NPC(mob) && mob->npcdata->pIndexData->act.test(ACT_QUESTMASTER) )
    break;
  if( mob == NULL || !can_see(ch,mob) )
  {
   send_to_char("You have to be at a questmaster!\r\n",ch);
   return;
  }
  if( !ch->pcdata->quest_info->is_questing )
  {
   send_to_char("You are not currently on a quest!\r\n",ch);
   return;
  }

  pqp = UMAX(1,ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP]/3);
  pgold = UMAX(1,ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD]/4);
  pexp = UMAX(1,ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP]/4);

  if( (ch->pcdata->quest_points - pqp) < 0 )
  {
   send_to_char("You do not have enough quest points to give up on this quest!\r\n",ch);
   return;
  }
  if( (money_value(ch->money) - pgold) < 0 )
  {
   send_to_char("You do not have enough gold to give up on this quest!\r\n",ch);
   return;
  }
  if( (ch->exp - pexp) < 0 )
  {
   send_to_char("You do not have enough experience to give up on this quest!\r\n",ch);
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
    snprintf(mbuf,MSL,"%s %s",m_number,m_name);
  }
  ch->exp -= pexp;

  snprintf(buf,MSL,"Giving up on this quest has cost you %d qp, %d exp, %s.\r\n",pqp,pexp,cost_to_money(money_to_value(ch,mbuf)));
  send_to_char(buf,ch);
  ch->pcdata->records->mquest_f++;
  ch->pcdata->quest_info->wait_time = number_range(10,15); /* higher wait time on giveup */
  snprintf(buf,MSL,"You must wait %d minutes before your next quest.\r\n",ch->pcdata->quest_info->wait_time);
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
   send_to_char("You are not currently on a quest!\r\n",ch);
   return;
  }
  if( ch->pcdata->quest_info->quest_type == QUEST_KILLING || ch->pcdata->quest_info->quest_type == QUEST_KILLING_R )
   snprintf(arg2,MIL,"1");
  hint = atoi(arg2);
  if( !is_number(arg2) )
  {
   send_to_char("You must select a task number for which you want a hint.\r\n",ch);
   return;
  }
  if( hint < 1 || hint > 5 )
  {
   send_to_char("You must choose the number of a task that you currently have!\r\n",ch);
   return;
  }
  if( ch->pcdata->quest_info->amount[hint-1] == -1 )
  {
   send_to_char("You do not currently have a task with that number.\r\n",ch);
   return;
  }
  if( ch->pcdata->quest_info->amount[hint-1] == 0 )
  {
   send_to_char("You have already completed that task!\r\n",ch);
   return;
  }
  if( ch->pcdata->quest_info->quest_hint[hint-1] )
  {
   send_to_char("You've already got your hint for that task!\r\n",ch);
   return;
  }
  if( ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL || ch->pcdata->quest_info->quest_type == QUEST_KILLING )
  {
   MOB_INDEX_DATA *imob = get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[hint-1]);
   snprintf(buf,MSL,"%s can be found somewhere in %s.\r\n",imob->short_descr,imob->area->name);
   send_to_char(buf,ch);
  }
  else if( ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL_R || ch->pcdata->quest_info->quest_type == QUEST_KILLING_R )
  {
   list<CHAR_DATA *>::iterator li;
   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
    mob = *li;
    if( !IS_NPC(mob) )
     continue;
    if( mob->in_room == NULL )
     continue;
    if( mob->race == ch->pcdata->quest_info->quest_mob_vnum[hint-1] )
     break;
   }
   snprintf(buf,MSL,"%ss can be found somewhere in %s.\r\n",race_table[mob->race].race_title,mob->in_room->area->name);
   send_to_char(buf,ch);
  }
  else if( ch->pcdata->quest_info->quest_type == QUEST_MULTI_RETRIEVE || ch->pcdata->quest_info->quest_type == QUEST_RETRIEVAL )
  {
   OBJ_INDEX_DATA *ob = get_obj_index(ch->pcdata->quest_info->quest_item_vnum[hint-1]);
   snprintf(buf,MSL,"%s can be found somewhere in %s.\r\n",ob->short_descr,ob->area->name);
   send_to_char(buf,ch);
  }
  ch->pcdata->quest_info->quest_hint[hint-1] = TRUE;
  send_to_char("Your quest rewards have been reduced for using a hint.\r\n",ch);
  do_save(ch,"auto");
  return;
 }
 else if( !str_prefix(arg1,"info") )
 {
  if( !ch->pcdata->quest_info->is_questing )
  {
   send_to_char("You are not currently on a quest!\r\n",ch);
   if( ch->pcdata->quest_info->wait_time > 0 )
   {
    snprintf(buf,MSL,"You must wait %d more minute%s before taking another quest.\r\n",ch->pcdata->quest_info->wait_time,ch->pcdata->quest_info->wait_time > 1 ? "s" : "");
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
  short i = 0, x = 0;

  for( mob = ch->in_room->first_person; mob; mob = mob->next_in_room )
   if( IS_NPC(mob) && mob->npcdata->pIndexData->act.test(ACT_QUESTMASTER) )
    break;
  if( mob == NULL )
  {
   send_to_char("You have to be at a questmaster!\r\n",ch);
   return;
  }
  if( !ch->pcdata->quest_info->is_questing )
  {
   send_to_char("You aren't even on a quest! Request one first.\r\n",ch);
   return;
  }

  switch( ch->pcdata->quest_info->quest_type )
  {
   default:
    send_to_char("Invalid quest type. Notifity an immortal.\r\n",ch);
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
       if( ch->pcdata->quest_info->quest_item_vnum[i] == obj->pIndexData->vnum && obj->wear_loc == WEAR_NONE && !IS_OBJ_STAT(obj,ITEM_EXTRA_UNIQUE) )
       {
        extract_obj(obj);
        x++;
        ch->pcdata->quest_info->amount[i]--;
        if( ch->pcdata->quest_info->amount[i] == 0 )
        {
         snprintf(buf,MSL,"You have now handed in all the %s you needed for your quest.\r\n",get_obj_index(ch->pcdata->quest_info->quest_item_vnum[i])->short_descr);
         send_to_char(buf,ch);
        }
       }
      }
      if( x > 0 && ch->pcdata->quest_info->amount[i] != 0 )
      {
       snprintf(buf,MSL,"%s (%d) was handed into the questmaster.\r\n",get_obj_index(ch->pcdata->quest_info->quest_item_vnum[i])->short_descr,x);
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

DO_FUN(do_qstat)
{
 CHAR_DATA *victim;
 char buf[MSL];
 short i = 0;

 if( argument[0] == '\0' )
 {
  send_to_char("Qstat whom?\r\n",ch);
  return;
 }
 if( (victim = get_char_world(ch,argument)) == NULL )
 {
  send_to_char("They aren't here.\r\n",ch);
  return;
 }
 if( IS_NPC(victim) )
 {
  send_to_char("Not on mobiles.\r\n",ch);
  return;
 }

 snprintf(buf,MSL,"@@y%s@@b's current quest status:@@N\r\n",victim->name.c_str());
 send_to_char(buf,ch);

 switch( victim->pcdata->quest_info->quest_type )
 {
  default:
   send_to_char("No active quest.\r\n",ch);
   return;
  case QUEST_RETRIEVAL:
   if( victim->pcdata->quest_info->quest_item_vnum[0] > -1 )
   {
    snprintf(buf,MSL,"%s is seeking item %d-%s...\r\n",victim->get_name(ch),victim->pcdata->quest_info->quest_item_vnum[0],get_obj_index(victim->pcdata->quest_info->quest_item_vnum[0])->short_descr);
    send_to_char(buf,ch);

    if( victim->pcdata->quest_info->amount[0] > 0 )
    {
     snprintf(buf,MSL,"...and must retrieve %d more of them.\r\n",victim->pcdata->quest_info->amount[0]);
     send_to_char(buf,ch);
    }
   }
   else
    send_to_char("Invalid quest item.\r\n",ch);
   break;
  case QUEST_KILLING:
   if( victim->pcdata->quest_info->quest_mob_vnum[0] > -1 )
   {
    snprintf(buf,MSL,"%s is seeking mob %d-%s...\r\n",victim->get_name(ch),victim->pcdata->quest_info->quest_mob_vnum[0],get_mob_index(victim->pcdata->quest_info->quest_mob_vnum[0])->short_descr);
    send_to_char(buf,ch);

    if( victim->pcdata->quest_info->amount[0] > 0 )
    {
     snprintf(buf,MSL,"...and must slay %d more of them.\r\n",victim->pcdata->quest_info->amount[0]);
     send_to_char(buf,ch);
    }
   }
   else
    send_to_char("Invalid quest mob.\r\n",ch);
   break;
  case QUEST_MULTI_KILL:
   send_to_char("Kill the following creatures (# of times).\r\n",ch);
   for( i = 0; i < QUEST_MAX_DATA; i++ )
   {
    if( victim->pcdata->quest_info->quest_mob_vnum[i] > -1 )
    {
     snprintf(buf,MSL,"Target #%d: %5d-%s (%d)\r\n",i+1,victim->pcdata->quest_info->quest_mob_vnum[i],get_mob_index(victim->pcdata->quest_info->quest_mob_vnum[i])->short_descr,victim->pcdata->quest_info->amount[i]);
     send_to_char(buf,ch);
    }
   }
   break;
  case QUEST_MULTI_RETRIEVE:
   send_to_char("Retrieve the following items (# of times).\r\n",ch);
   for( i = 0; i < QUEST_MAX_DATA; i++ )
   {
    if( victim->pcdata->quest_info->quest_item_vnum[i] > -1 )
    {
     snprintf(buf,MSL,"Item #%d: %5d-%s (%d)\r\n",i+1,victim->pcdata->quest_info->quest_item_vnum[i],get_obj_index(victim->pcdata->quest_info->quest_item_vnum[i])->short_descr,victim->pcdata->quest_info->amount[i]);
     send_to_char(buf,ch);
    }
   }
   break;
  case QUEST_KILLING_R:
   if( victim->pcdata->quest_info->quest_mob_vnum[0] > -1 )
   {
    snprintf(buf,MSL,"%s is seeking %s mobs...\r\n",victim->get_name(ch),race_table[victim->pcdata->quest_info->quest_mob_vnum[0]].race_title);
    send_to_char(buf,ch);

    if( victim->pcdata->quest_info->amount[0] > 0 )
    {
     snprintf(buf,MSL,"...and must slay %d more of them.\r\n",victim->pcdata->quest_info->amount[0]);
     send_to_char(buf,ch);
    }
   }
   else
    send_to_char("Invalid quest race.\r\n",ch);
   break;
  case QUEST_MULTI_KILL_R:
   send_to_char("Kill mobs of the following races (# of times).\r\n",ch);
   for( i = 0; i < QUEST_MAX_DATA; i++ )
   {
    if( victim->pcdata->quest_info->quest_mob_vnum[i] > -1 )
    {
     snprintf(buf,MSL,"Target #%d: %s (%d)\r\n",i+1,race_table[victim->pcdata->quest_info->quest_mob_vnum[i]].race_title,victim->pcdata->quest_info->amount[i]);
     send_to_char(buf,ch);
    }
   }
   break;
 }
 snprintf(buf,MSL,"QP:   %d\r\nGold: %s\r\nExp:  %d\r\n",victim->pcdata->quest_info->quest_reward[QUEST_REWARD_QP],cost_to_money(victim->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD]),victim->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP]);
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
   snprintf(buf,MSL,"quest_type is a value of %d, how'd that happen? Tell an imm, thanks!\r\n",ch->pcdata->quest_info->quest_type);
   send_to_char(buf,ch);
   break;
  case QUEST_RETRIEVAL:
   if( ch->pcdata->quest_info->quest_item_vnum[0] > -1 )
   {
    snprintf(buf,MSL,"You are seeking the item known as %s@@N.\r\n",get_obj_index(ch->pcdata->quest_info->quest_item_vnum[0])->short_descr);
    send_to_char(buf,ch);

    if( ch->pcdata->quest_info->amount[0] > 0 )
    {
     snprintf(buf,MSL,"Your quest contract states that you must retrieve %d more of them.\r\n",ch->pcdata->quest_info->amount[0]);
     send_to_char(buf,ch);
     snprintf(buf,MSL,"@@eHint:@@N They can be found in %s@@N!\r\n",get_obj_index(ch->pcdata->quest_info->quest_item_vnum[i])->area->name);
     if( ch->pcdata->quest_info->quest_hint[0] )
      send_to_char(buf,ch);
    }
   }
   else
    send_to_char("Invalid quest item.\r\n",ch);
   break;
  case QUEST_KILLING:
   if( ch->pcdata->quest_info->quest_mob_vnum[0] > -1 )
   {
    snprintf(buf,MSL,"You are seeking out the creature known as %s@@N.\r\n",get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[0])->short_descr);
    send_to_char(buf,ch);

    if( ch->pcdata->quest_info->amount[0] > 0 )
    {
     snprintf(buf,MSL,"Your quest contract states that you must slay %d more of them.\r\n",ch->pcdata->quest_info->amount[0]);
     send_to_char(buf,ch);
     snprintf(buf,MSL,"@@eHint:@@N They can be found in %s@@N!\r\n",get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->area->name);
     if( ch->pcdata->quest_info->quest_hint[0] )
      send_to_char(buf,ch);
    }
    else
     send_to_char("You have fulfilled the kill requirements outlined in your contract.\r\n",ch);
   }
   else
    send_to_char("Invalid quest mob.\r\n",ch);
   break;
  case QUEST_MULTI_KILL:
   send_to_char("You must hunt down and destroy the following creatures (# of times).\r\n",ch);
   for( i = 0; i < QUEST_MAX_DATA; i++ )
   {
    if( ch->pcdata->quest_info->quest_mob_vnum[i] > -1 )
    {
     snprintf(buf,MSL,"Target #%d: %s (%d)",i+1,get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->short_descr,ch->pcdata->quest_info->amount[i]);
     send_to_char(buf,ch);
     if( ch->pcdata->quest_info->quest_hint[i] )
      snprintf(buf,MSL," [%s]\r\n",get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->area->name);
     else
      snprintf(buf,MSL,"\r\n");
     send_to_char(buf,ch);
    }
   }
   break;
  case QUEST_MULTI_RETRIEVE:
   send_to_char("You must retrieve the following items from across the land (# of times).\r\n",ch);
   for( i = 0; i < QUEST_MAX_DATA; i++ )
   {
    if( ch->pcdata->quest_info->quest_item_vnum[i] > -1 )
    {
     snprintf(buf,MSL,"Item #%d: %s (%d)",i+1,get_obj_index(ch->pcdata->quest_info->quest_item_vnum[i])->short_descr,ch->pcdata->quest_info->amount[i]);
     send_to_char(buf,ch);
     if( ch->pcdata->quest_info->quest_hint[i] )
      snprintf(buf,MSL," [%s]\r\n",get_obj_index(ch->pcdata->quest_info->quest_item_vnum[i])->area->name);
     else
      snprintf(buf,MSL,"\r\n");
     send_to_char(buf,ch);
    }
   }
   break;
  case QUEST_KILLING_R:
   if( ch->pcdata->quest_info->quest_mob_vnum[0] > -1 )
   {
    snprintf(buf,MSL,"You are seeking out %s creatures.\r\n",race_table[ch->pcdata->quest_info->quest_mob_vnum[0]].race_title);
    send_to_char(buf,ch);

    if( ch->pcdata->quest_info->amount[0] > 0 )
    {
     CHAR_DATA *mob = NULL;
     list<CHAR_DATA *>::iterator li;
     snprintf(buf,MSL,"Your quest contract states that you must slay %d more of them.\r\n",ch->pcdata->quest_info->amount[0]);
     send_to_char(buf,ch);
     for( li = char_list.begin(); li != char_list.end(); li++ )
     {
      mob = *li;
      if( !IS_NPC(mob) )
       continue;
      if( mob->in_room == NULL )
       continue;
      if( mob->race == ch->pcdata->quest_info->quest_mob_vnum[0] )
       break;
     }
     if( mob != NULL )
     {
      snprintf(buf,MSL,"@@eHint:@@N They can be found in %s@@N!\r\n",mob->in_room->area->name);
      if( ch->pcdata->quest_info->quest_hint[0] )
       send_to_char(buf,ch);
     }
    }
    else
     send_to_char("You have fulfilled the kill requirements outlined in your contract.\r\n",ch);
   }
   else
    send_to_char("Invalid quest mob race.\r\n",ch);
   break;
  case QUEST_MULTI_KILL_R:
   send_to_char("You must hunt down and destroy the following types of creatures (# of times).\r\n",ch);
   for( i = 0; i < QUEST_MAX_DATA; i++ )
   {
    if( ch->pcdata->quest_info->quest_mob_vnum[i] > -1 )
    {
     CHAR_DATA *mob = NULL;
     list<CHAR_DATA *>::iterator li;
     for( li = char_list.begin(); li != char_list.end(); li++ )
     {
      mob = *li;
      if( !IS_NPC(mob) )
       continue;
      if( mob->in_room == NULL )
       continue;
      if( mob->race == ch->pcdata->quest_info->quest_mob_vnum[i] )
       break;
     }
     snprintf(buf,MSL,"Target #%d: %s (%d)",i+1,race_table[ch->pcdata->quest_info->quest_mob_vnum[i]].race_title,ch->pcdata->quest_info->amount[i]);
     send_to_char(buf,ch);
     if( ch->pcdata->quest_info->quest_hint[i] && mob != NULL )
      snprintf(buf,MSL," [%s]\r\n",mob->in_room->area->name);
     else
      snprintf(buf,MSL,"\r\n");
     send_to_char(buf,ch);
    }
    else
     send_to_char("Invalid quest mob race.\r\n",ch);
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
   snprintf(buf,MSL,"Quest reward reduced for the use of %d hint%s.\r\n",hint,hint > 1 ? "s" : "");
   send_to_char(buf,ch);
  }

  while( hint > 0 )
  {
   tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP];
   tmp *= 0.90;
   ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP] = static_cast<int>(tmp);
   tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD];
   tmp *= 0.90;
   ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] = static_cast<int>(tmp);
   tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP];
   tmp *= 0.90;
   ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = static_cast<int>(tmp);
   hint--;
  }

  snprintf(buf,MSL,"Quest Complete. You have earned %d qp, %d experience, %s!\r\n",ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP],ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP],cost_to_money(ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD]));
  send_to_char(buf,ch);

  ch->pcdata->quest_points += ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP];
  join_money(round_money(ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD],TRUE),ch->money);
  ch->exp += ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP];

  ch->pcdata->records->mquest_c++;
  ch->pcdata->records->qp_tot += ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP];

  if( ch->pcdata->records->mquest_c % 50 == 0 )   /* every time a person completes 50 quests */
  {
   send_to_char("@@eCongratulations! You've succesfully completed 50 questmaster quests!\r\n",ch);
   send_to_char("@@eYou have been rewarded a bonus 25 practices and 40 quest points!\r\n",ch);
   ch->pcdata->practice += 25;
   ch->pcdata->quest_points += 40;
   ch->pcdata->records->qp_tot += 40;
  }
  if( ch->pcdata->quest_points > ch->pcdata->records->qp )
  {
   send_to_char("@@yYou've broken your quest point record!@@N\r\n",ch);
   ch->pcdata->records->qp = ch->pcdata->quest_points;
  }
  mquest_clear(ch,FALSE);
 }
 else
  send_to_char("You haven't completed the quest's requirements yet.\r\n",ch);
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
 clev = ch->get_level("psuedo");
 exp = (exp_mob_base(clev) * sysdata.killperlev);
 exp = number_range(static_cast<int>(exp * 0.04),static_cast<int>(exp * 0.08));

 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP] = number_range(static_cast<int>(clev / 40),static_cast<int>(clev / 20));
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] = number_range(static_cast<int>(clev * 15),static_cast<int>(clev * 30));

 if( IS_ADEPT(ch) )
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = number_range(static_cast<int>(exp_to_level_adept(ch) * 0.04),static_cast<int>(exp_to_level_adept(ch) * 0.08));
 else
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = exp;

 /* Add some randomness */
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP] += number_range(-1,1);
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] = number_range(static_cast<int>(ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] * 0.9),static_cast<int>(ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] * 1.1));
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = number_range(static_cast<int>(ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] * 0.9),static_cast<int>(ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] * 1.1));

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
  send_to_char("An error has occured calculating rewards. This should not happen.\r\n",ch);
  ch->pcdata->quest_info->wait_time = 0;
  mquest_clear(ch,TRUE);
  return;
 }

 tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP];
 tmp *= mod;
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP] = static_cast<int>(tmp);
 tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD];
 tmp *= mod;
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] = static_cast<int>(tmp);
 tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP];
 tmp *= mod;
 ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = static_cast<int>(tmp);
       
 /* Bonus! */
 if( number_percent() < 5 )
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_QP] += 2;
 if( number_percent() < 5 )
 {
  tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD];
  tmp *= 1.5;
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_GOLD] = static_cast<int>(tmp);
 }
 if( number_percent() < 5 )
 {
  tmp = ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP];
  tmp *= 1.5;
  ch->pcdata->quest_info->quest_reward[QUEST_REWARD_EXP] = static_cast<int>(tmp);
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
  snprintf(buf,MSL,"You must wait %d minutes before your next quest.\r\n",ch->pcdata->quest_info->wait_time);
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

 clev = ch->get_level("psuedo");
 min_lev = static_cast<int>(clev * 0.7);
 max_lev = static_cast<int>(clev * 1.1);

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
    send_to_char("Mquest error. Unable to find suitible mobs. Aborting.\r\n",ch);
    ch->pcdata->quest_info->wait_time = 0;
    mquest_clear(ch,TRUE);
    return;
   }
   continue;
  }

  x = 0;
  if( !race )
   ch->pcdata->quest_info->quest_mob_vnum[i] = mob->npcdata->pIndexData->vnum;
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
     if( get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->act.test(ACT_SENTINEL) )
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
 CHAR_DATA *mob = NULL;
 list<CHAR_DATA *>::iterator li;
 int i = number_range(0,mob_index_list.size()-1);

 for( li = char_list.begin(); li != char_list.end(); li++ )
 {
  mob = *li;
  i--;

  if( i > 0 )  /* Add some variety to where we start in the ch_list */
   continue;
  if( !IS_NPC(mob) )
   continue;

  if( (mob->get_level("psuedo") < min_lev || mob->get_level("psuedo") > max_lev)
   || (mob->in_room->area->min_level > ch->get_level("psuedo") || mob->in_room->area->max_level < ch->get_level("psuedo"))
   || (mob->in_room->area->flags.test(AFLAG_NOSHOW) || mob->in_room->area->flags.test(AFLAG_NO_QUEST))
   || (!str_cmp(rev_spec_lookup(mob->npcdata->spec_fun),"spec_cast_adept"))
   || (!str_cmp(rev_spec_lookup(mob->npcdata->spec_fun),"spec_executioner"))
   || (mob->npcdata->pIndexData->vnum > mob->in_room->area->max_vnum || mob->npcdata->pIndexData->vnum < mob->in_room->area->min_vnum)
   || (mob->in_room->room_flags.test(RFLAG_SAFE))
   || (IS_AFFECTED(mob,AFF_CHARM))
   || (mob->act.test(ACT_PET))
   || (mob->act.test(ACT_NO_QUEST)))
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

 clev = ch->get_level("psuedo");
 min_lev = static_cast<int>(clev * 0.7);
 max_lev = static_cast<int>(clev * 1.1);

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
    send_to_char("Mquest error. Unable to find suitible objs. Aborting.\r\n",ch);
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
 OBJ_DATA *obj = NULL;
 int i = number_range(0,obj_index_list.size()-1);
 list<OBJ_DATA *>::iterator li;

 for( li = obj_list.begin(); li != obj_list.end(); li++ )
 {
  obj = *li;
  i--;

  if( i > 0 ) /* Add some variety to where we start in the obj_list */
   continue;

  if( obj->in_room )
  {
   if( (obj->in_room->area->flags.test(AFLAG_NO_QUEST) || obj->in_room->area->flags.test(AFLAG_NOSHOW)) /* Hide places we don't want to use. */
    || (obj->in_room->area->min_level > ch->get_level("psuedo") || obj->in_room->area->max_level < ch->get_level("psuedo"))
    || (obj->pIndexData->vnum > obj->in_room->area->max_vnum || obj->pIndexData->vnum < obj->in_room->area->min_vnum) /* outside obj's own area */
    || (!CAN_WEAR(obj,ITEM_TAKE)) /* ensure we can pick it up */
    || (IS_OBJ_STAT(obj,ITEM_EXTRA_RARE)) /* don't waste rares */
    || (obj->item_type == ITEM_QUEST) /* no autoquest items */
    || (obj->item_type == ITEM_MONEY)
    || (IS_OBJ_STAT(obj,ITEM_EXTRA_NO_QUEST))
    || (obj->item_type == ITEM_PIECE)
    || (IS_OBJ_STAT(obj,ITEM_EXTRA_UNIQUE))
    || (obj->carried_by != NULL && (!IS_NPC(obj->carried_by)
    || (IS_NPC(obj->carried_by) && obj->carried_by->npcdata->pIndexData->pShop != NULL)))  /* held by PC or held by NPC that is a shopkeeper */
    || (obj->weight > 15)
    || (obj->level > max_lev)
    || (obj->level < min_lev))
     continue;

   if( number_percent() < 5 ) /* Add variety to actually selecting the first obj we find in the obj_list */
    return obj;
  }
  else if( obj->carried_by && obj->carried_by->in_room )
  {
   if( (obj->carried_by->in_room->area->flags.test(AFLAG_NO_QUEST) || obj->carried_by->in_room->area->flags.test(AFLAG_NOSHOW)) /* Hide places we don't want to use. */
    || (obj->carried_by->in_room->area->min_level > ch->get_level("psuedo") || obj->carried_by->in_room->area->max_level < ch->get_level("psuedo"))
    || (obj->pIndexData->vnum > obj->carried_by->in_room->area->max_vnum || obj->pIndexData->vnum < obj->carried_by->in_room->area->min_vnum) /* outside obj's own area */
    || (!CAN_WEAR(obj,ITEM_TAKE)) /* ensure we can pick it up */
    || (IS_OBJ_STAT(obj,ITEM_EXTRA_RARE)) /* don't waste rares */
    || (obj->item_type == ITEM_QUEST) /* no autoquest items */
    || (obj->item_type == ITEM_MONEY)
    || (IS_OBJ_STAT(obj,ITEM_EXTRA_NO_QUEST))
    || (obj->item_type == ITEM_PIECE)
    || (IS_OBJ_STAT(obj,ITEM_EXTRA_UNIQUE))
    || (obj->carried_by != NULL && (!IS_NPC(obj->carried_by)
    || (IS_NPC(obj->carried_by) && obj->carried_by->npcdata->pIndexData->pShop != NULL))) /* held by PC or held by NPC that is a shopkeeper */
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
    && (ch->pcdata->quest_info->quest_mob_vnum[i] == victim->npcdata->pIndexData->vnum
    || (!str_cmp(get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->short_descr,victim->npcdata->short_descr) 
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
    && (ch->get_level("psuedo") < victim->get_level("psuedo") || (victim->get_level("psuedo") - ch->get_level("psuedo")) >= -10) )
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
   send_to_char("You can now get another quest from the questmaster!\r\n",ch);
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
    && (ch->pcdata->quest_info->quest_mob_vnum[i] == victim->npcdata->pIndexData->vnum
    || (!str_cmp(get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->short_descr,victim->npcdata->short_descr)
    && get_mob_index(ch->pcdata->quest_info->quest_mob_vnum[i])->area == victim->in_room->area)) )
   {
    ch->pcdata->quest_info->amount[i]--;
    send_to_char("You have completed a quest kill!\r\n",ch);
    if( ch->pcdata->quest_info->amount[i] == 0 )
     send_to_char("You have completed the kill requirement for this creature!\r\n",ch);
   }
   if( ch->pcdata->quest_info->amount[i] > -1 )
    tot += ch->pcdata->quest_info->amount[i];
  }
  if( tot == 0 ) /* If the total is 0 after checking through the whole loop, then they've completed their quest. */
  {
   ch->pcdata->quest_info->quest_complete = TRUE;
   send_to_char("You have completed your quest! Return to the quest master.\r\n",ch);
  }
 }
 if( ch->pcdata->quest_info->is_questing && (ch->pcdata->quest_info->quest_type == QUEST_KILLING_R || ch->pcdata->quest_info->quest_type == QUEST_MULTI_KILL_R) && !ch->pcdata->quest_info->quest_complete )
 {
  for( i = 0; i < QUEST_MAX_DATA; i++ )
  {
   if( ch->pcdata->quest_info->quest_mob_vnum[i] < 0 )
    continue;
   if( ch->pcdata->quest_info->amount[i] > 0 && (ch->pcdata->quest_info->quest_mob_vnum[i] == victim->race) && (ch->get_level("psuedo") < victim->get_level("psuedo") || (victim->get_level("psuedo") - ch->get_level("psuedo")) >= -10) )
   {
    ch->pcdata->quest_info->amount[i]--;
    send_to_char("You have completed a quest kill!\r\n",ch);
    if( ch->pcdata->quest_info->amount[i] == 0 )
     send_to_char("You have completed the kill requirement for this race!\r\n",ch);
   }
   if( ch->pcdata->quest_info->amount[i] > -1 )
    tot += ch->pcdata->quest_info->amount[i];
  }
  if( tot == 0 ) /* If the total is 0 after checking through the whole loop, then they've completed their quest. */
  {
   ch->pcdata->quest_info->quest_complete = TRUE;
   send_to_char("You have completed your quest! Return to the quest master.\r\n",ch);
  }
 }
 return;
}
