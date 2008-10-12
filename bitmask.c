/*
 * This is Runter's system for infinite bits. It was made for use
 * in Feltain. I, Runter, require no credit, no
 * help file documentation of your use, and no email confirmation.
 * The only thing I do ask is if you find a bug or have a major change
 * that makes the system more efficient or more intelligent than 
 * let me know about it. Just remember what goes around comes around.
 * We all benifit when we work together.
 *
 * To successfully use this you will use the variable BITMASK
 * "BITMASK *act;"
 * instead of int in your struct declarations for your bitmasks.
 * On this new variable type just pass the variables address to
 * the new functions-- set_bit, remove_bit, is_set.
 * example: if(is_set(&ch->act, ACT_NPC))
 *
 * This system passes normal numbers as the bits so you'll need
 * to redeclare stuff as you are passing it. 
 *
 * if(is_set(&ch->act, 400)) // is the 400th bit set?
 *
 * A good way to change your entire system over would be to redo
 * your definitions of A B C D E... aa bb cc and such from defines
 * to one enum struct.
 * 
 * Another important note is that you need to use the new loading
 * and saving functions provided when saving your bitmasks. It
 * saves them to a single line and loads them from a single line.
 *
 */
/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

/*
 * Minor cleanups, a few small fixes, and a few minor edits.
 * --Kline
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"

bool remove_bit( BITMASK *mask, int bit ) /* Returns FALSE if nothing removed. TRUE if so. */
{
 BM_LIST *pBlist = 0, *last = 0;

 if( !is_set(mask,bit) )
  return FALSE;

 --bit;

 for( pBlist = mask->int_list; pBlist; pBlist = pBlist->next) /* Loop through each bitmask we have allocated. */
 {
  if( pBlist->set == bit / 32 ) /* Is this the correct set? */
  {
   pBlist->tar_mask &= ~(1 << (bit % 32)); /* Remove it. */
   --mask->bits;
   if( pBlist->tar_mask == 0 )
   {
    if( last )
     last->next = pBlist->next;
    else
     mask->int_list = pBlist->next;
    free(pBlist);
    --mask->masks;
   }
   return TRUE; /* We found and removed it above :) */
  }
  last = pBlist;
 }
 return FALSE; /* A bug happened. Wee. Somehow. */
}

bool set_bit( BITMASK *mask, int bit ) /* Returns TRUE if a bit was set. FALSE if already set. */
{
 BM_LIST *pBlist = 0;

 --bit;

 for( pBlist = mask->int_list; pBlist; pBlist = pBlist->next ) /* Loop through each bitmask already allocated. */
 {
  if( pBlist->set == bit / 32 ) /* Is this the correct set? */
  {
   if( pBlist->tar_mask & (1 << (bit % 32)) )
    return FALSE; /* Found it, don't set again. */

   pBlist->tar_mask |= 1 << (bit % 32); /* Set our bit inside the mask */
   ++mask->bits;
   return TRUE; /* We did something, yay! */
  }
 }

 /* Let's allocate a new one! */
 pBlist = (BM_LIST *)malloc(sizeof(BM_LIST));
 ++mask->masks;
 pBlist->next = mask->int_list;
 mask->int_list = pBlist;
 pBlist->tar_mask = 0;
 pBlist->set = (bit / 32);
 pBlist->tar_mask |= 1 << (bit % 32); /* Set our bit on the new mask */
 ++mask->bits;
 return TRUE; /* We did something, yay! */
}

bool is_set( BITMASK *mask, int bit )
{
 BM_LIST *pBlist = 0;

 --bit;

 for( pBlist = mask->int_list; pBlist; pBlist = pBlist->next )
  if( pBlist->set == bit / 32 )
  {
   if( pBlist->tar_mask & 1 << (bit % 32) )
    return TRUE;
   else
    return FALSE;
  }
 return FALSE;
}

/*
 * Let's serialize this and return all bits that are set in a dynamic list.
 * This is useful when you just want to know what to traverse in a bitmask.
 * Good for high level solutions to lists of pointers! This is a high level
 * solution that *does not* maintain an ordered queue for specific BFS or
 * DFS algorithms. This is for algorithms that do not matter.
 */
int *serialize_bitmask( BITMASK *mask )
{
 BM_LIST *pBlist = 0;
 int *ilist = malloc(sizeof(int) * mask->bits +1), i = 0, z = 0;
 ilist[mask->bits] = 0; /* Zero terminates it. 0th bit CAN NOT BE SET. */

 for( pBlist = mask->int_list; pBlist; pBlist = pBlist->next )
 {
  for( z = 0; z < 32; ++z )
  {
   if( i > mask->bits )
   {
    /*
     * more found than we have allocated. Maybe residual.
     * May add a check before setting ilist to make sure it
     * doesn't over run but this should be fine. Just don't
     * bug it here for sure. It may not be an error at this
     * point. If you do decide to error check this better
     * this break should be replaced to let it go through.
     */
    break;
   }

   if( pBlist->tar_mask & 1 << z )
    ilist[i++] = (pBlist->set * 32 + z + 1);
  }
 }
 if( i < (mask->bits + 1) )
 {
  /* Error -- we have less recorded bits than allocated. */
 }
 /*
  * This must be freed somewhere with free(ilist) or it will leak.
  * Using a static buffer here would be a poor idea. Using temporary
  * memory allocation would be a good one. Too bad there isn't an
  * ANSI thing for that in C.
  */
 return ilist;
}

bool free_bitmask( BITMASK *pBmask ) /* Frees a bitmask; safe to call dry. Return TRUE if freed, FALSE otherwise. */
{
 BM_LIST *pBMlist = 0, *next = 0;
 bool found = FALSE;

 for( pBMlist = pBmask->int_list; pBMlist; pBMlist = next )
 {
  next = pBMlist->next;
  free(pBMlist);
  found = TRUE;
 }

 return found;
}

void load_bitmask( BITMASK *pBmask, FILE *fp ) /* #masks #bits #mask #vector #mask #vector ... */
{
 int i = 0;
 BM_LIST *pBMlist = 0;

 pBmask->masks = fread_number(fp);
 pBmask->bits = fread_number(fp);

 for( i = 0; i < pBmask->masks; ++i )
 {
  pBMlist = malloc(sizeof(BM_LIST));

  pBMlist->set = fread_number(fp);
  pBMlist->next = pBmask->int_list;
  pBmask->int_list = pBMlist;
  pBMlist->tar_mask = fread_number(fp);
 }
 fread_to_eol(fp);
}

char *save_bitmask( BITMASK *pBmask ) /* Make this a string so it's easier to pass to existing save routines. --Kline */
{
 BM_LIST *pBMlist = 0;
 static char buf[MSL];
 buf[0] = '\0';

 xprintf(buf,"%ld %ld",pBmask->masks,pBmask->bits);

 for( pBMlist = pBmask->int_list; pBMlist; pBMlist = pBMlist->next )
  xcat(buf," %ld %ld",pBMlist->set,pBMlist->tar_mask);

 return buf;
}

/* Thanks to Marlin@Azereth for this */
void bv_to_bm( int list, BITMASK *mask )
{
 sh_int i;

 for( i = 0; i < 32; i++ )
  if( IS_SET(list,(1 << i)) )
   set_bit(mask,i+1);

 return;
}

void debug_bitmask( BITMASK *pBmask )
{
 int *array = (int *)serialize_bitmask(pBmask);
 int i = 0;

 for(; array[i] != 0; ++i)
 {
  xprintf_2(log_buf,"%d",array[i]); // this is our bit we are accessing.
  monitor_chan(log_buf,MONITOR_DEBUG);
 }
 free(array);
 return;
}

void bm_test( void )
{
 BITMASK *bm = (BITMASK *)malloc(sizeof(BITMASK));
 int i;
 static BITMASK bitmask_zero;
 *bm = bitmask_zero;

 // We're gonna put in a random number of random bits.
 i = number_range(75, 250); // between 75 and 250 bits to set
 while( i-- )
 {
  if( number_percent() > 50 )
   set_bit(bm,i);
 }
 debug_bitmask(bm); // Let's see what the contents are.
 free_bitmask(bm);
}
