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

/******************************************************************************
 *  SSM v2.1.9 (shared string manager)                                        *
 *                                                                            *
 *  Copyright(C) 1996 Melvin Smith (Fusion) for EnvyMUD 2.2                   *
 *                                                                            *
 *  Due to alignment differences on 32 bit and 64 bit machines, memory        *
 *  usage is now virtually identical to standard Merc on 32-bit               *
 *  architecture, but slightly larger on 64-bit. Memory usage is still        *
 *  smaller than SSM 2.0 or earlier. The manager now uses short ints for      *
 *  the count and size of chunks, so to increase MAX_STRING you must          *
 *  increase MAX_CHUNKS instead. String have a max reference count of         *
 *  +32766 and max size of CHUNK_SIZE (0xfff0). Fragmentation is also         *
 *  handled more efficiently by marking failed chunks with -1 to temporarily  *
 *  disable them until a defrag_heap() recycles them. This helps when a       *
 *  4 byte chunk is freed low in the heap, so string_dup() doesn't walk       *
 *  the whole heap every time.                                                *
 *                                                                            *
 *  <msmith@falcon.mercer.peachnet.edu>                                       *
 *                                                                            *
 *  ROM2.4 modifications by Tom Adriaenssen (Jan 1996) -- Wreck               *
 *                                                                            *
 *  <tadriaen@zorro.ruca.ua.ac.be>                                            *
 *                                                                            *
 *  Removed ROM 2.4 modifications as Envy doesnt need *fread_string_eol -Kahn *
 ******************************************************************************/

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "globals.h"
#include <signal.h>
#if !defined( ultrix )
#include <memory.h>
#endif

#include "ssm.h"

TempHash **temp_string_hash;

/* These are the original Merc vars in db.c */
char str_empty[1];
char *string_space;
char *top_string;
long nAllocString;
long sAllocString;
long nOverFlowString;
long sOverFlowString;
long hwOverFlow;
int numFree;
bool Full;
extern bool fBootDb;

/*
 * ssm_buf_head points to start of shared space,
 * ssm_buf_free points to next free block
 */
BufEntry *ssm_buf_head, *ssm_buf_free;

long MAX_STRING = MAX_CHUNKS * CHUNK_SIZE;
int HEADER_SIZE;
long tot = 0;

void init_string_space(  )
{
   BufEntry *walk;
   int i;

   string_space = ( char * )malloc( MAX_STRING );
   if( !string_space )
   {
      bugf( "SSM: Can't allocate %ld bytes shared string space.", MAX_STRING );
      raise( SIGSEGV );
   }

   top_string = string_space + MAX_STRING - 1;
   ssm_buf_head = ( BufEntry * ) string_space;
   HEADER_SIZE = ( int )( ( char * )&ssm_buf_head->buf[0] - ( char * )ssm_buf_head );

   walk = ssm_buf_head;
   for( i = 0;; i++ )
   {
      walk->usage = 0;
      walk->size = CHUNK_SIZE - HEADER_SIZE;
      if( i < MAX_CHUNKS - 1 )
      {
         walk->next = ( BufEntry * ) ( ( char * )walk + CHUNK_SIZE );
         walk = walk->next;
         continue;
      }

      walk->next = 0;
      break;
   }

   ssm_buf_free = ssm_buf_head;
   temp_string_hash = ( TempHash ** ) calloc( sizeof( TempHash * ), MAX_KEY_HASH );
}

int defrag_heap(  )
{
   /*
    * Walk through the shared heap and merge adjacent free blocks.
    * Free blocks are merged in str_free if free->next is free but
    * if the block preceding free is free, it stays unmerged. I would
    * rather not have the heap as a DOUBLE linked list for 2 reasons...
    *  (1) Extra 4 bytes per struct uses more mem
    *  (2) Speed - don't want to bog down str_ functions with heap management
    * The "orphaned" blocks will eventually be either merged or reused.
    * The str_dup function will call defrag if it cant allocate a buf.
    */

   BufEntry *walk, *last_free, *next;
   int merges = 0;
   ssm_buf_free = 0;
   for( walk = ssm_buf_head, last_free = 0; walk; walk = next )
   {
      next = walk->next;
      if( walk->usage > 0 )
      {
         /*
          * this block is in use so set last_free to NULL 
          */
         last_free = 0;
         continue;
      }
      else if( !last_free )
      {
         /*
          * OK found a NEW free block, set last_free and move to next 
          */
         last_free = walk;
         if( !ssm_buf_free )
            ssm_buf_free = walk;

         continue;
      }
      else
      {
         /*
          * previous block free so merge walk into last_free and move on 
          */
         if( ( long )last_free->size + ( long )walk->size <= CHUNK_SIZE )
         {
            merges++;
            last_free->size += walk->size + HEADER_SIZE;
            last_free->next = walk->next;
            last_free->usage = 0;
         }
         else
            last_free = walk;
      }
   }

   if( merges )
      log_f( "SSM: defrag_heap: made %d block merges.", merges );
   else
      log_f( "SSM: defrag_heap: resulted in 0 merges." );

   /*
    * Start count over again 
    */
   numFree = 0;
   return merges;
}


/*
 * Dup a string into shared space. If string exists, the usage count
 * gets incremented and the reference is returned. If the string does
 * not exist in heap, space is allocated and usage is 1.
 * This is a linked list first fit algorithm, so strings can be
 * freed. Upon bootup, there is a seperate hash table constructed in order
 * to do crunching, then the table is destroyed.
 */

/* for alarm_update crosschecking */
int ssm_dup_count;
int ssm_loops;
int ssm_recent_loops;

char *_str_dup( const char *str, const char *caller )
{
   BufEntry *ptr;
   int len;
   int rlen;
   char *str_new;

   ssm_dup_count++;
   ssm_recent_loops = 0;

   if( !str || !*str )
      return &str_empty[0];

   if( str > string_space && str < top_string )
   {
      ptr = ( BufEntry * ) ( str - HEADER_SIZE );
      if( ptr->usage <= 0 )
      {
         bugf( "str_dup: invalid string from %s: %20.20s", caller, str );
         ptr->usage = 0;   /* make it valid again */
      }

      ptr->usage++;
      return ( char * )str;
   }

   rlen = len = ( int )strlen( str ) + 1;

   /*
    * Round up to machine dependant address size.
    * Don't remove this, because when the BufEntry struct is overlaid
    * the struct must be aligned correctly.
    */

   if( ( len + HEADER_SIZE ) & addrSizeMask )
      len += addrTypeSize - ( ( len + HEADER_SIZE ) & addrSizeMask );

 RETRY:
   for( ptr = ssm_buf_free; ptr; ptr = ptr->next )
   {
      ssm_recent_loops++;
      ssm_loops++;
      if( ptr->usage == 0 && ptr->size >= len )
         break;
   }

   if( ptr )
      ptr->usage = 1;

   if( !ptr )
   {
      if( numFree > 1 )
         numFree++;

      if( numFree >= MAX_FREE )
      {
         int merges;
         log_f( "SSM: Attempting to optimize shared string heap." );
         merges = defrag_heap(  );

         /*
          * goto is fine because defrag will return 0 next time 
          */
         if( merges )
            goto RETRY;
      }

      /*
       * A one time toggle just for bugging purposes 
       */
      if( !Full )
      {
         bugf( "SSM: The shared string heap is full!" );
         Full = 1;
      }

      str_new = ( char * )malloc( rlen );
      strcpy( str_new, str );
      sOverFlowString += rlen;
      nOverFlowString++;
      if( sOverFlowString > hwOverFlow )
         hwOverFlow = sOverFlowString;
      return str_new;
   }
   /*
    * If there is at least header size excess break it up 
    */
   else if( ptr->size - len >= ( HEADER_SIZE + 8 ) )
   {
      BufEntry *temp;
      /*
       * WARNING! - DONT REMOVE THE CASTS BELOW! - Fusion 
       */
      temp = ( BufEntry * ) ( ( char * )ptr + HEADER_SIZE + len );
      temp->size = ptr->size - ( len + HEADER_SIZE );
      temp->next = ptr->next;
      temp->usage = 0;
      ptr->size = len;
      ptr->next = temp;

      ssm_buf_free = temp;
   }
   else
   {
      if( ptr != ssm_buf_free )
         ssm_buf_free->usage--;  /* buf_free was skipped */

      /*
       * spec: don't start from the start of the heap again! 
       */
      for( ; ssm_buf_free; ssm_buf_free = ssm_buf_free->next )
      {
         ssm_loops++;
         ssm_recent_loops++;
         if( ssm_buf_free->usage == 0 )
            break;
      }
   }

   str_new = ( char * )&ptr->buf[0];
   strcpy( str_new, str );
   ptr->caller = caller;
   nAllocString++;
   sAllocString += ptr->size + HEADER_SIZE;

   return str_new;
}

/*
 * If string is in shared space, decrement usage, if usage then is 0,
 * free the chunk and attempt to merge with next node. Other
 * strings are freed with standard free.
 * Never call free/delete externally on a shared string.
 */
void _free_string( char *str, const char *caller )
{
   BufEntry *ptr;

   if( !str || str == &str_empty[0] )
      return;

   if( str > string_space && str < top_string )
   {
      ptr = ( BufEntry * ) ( str - HEADER_SIZE );

      if( --ptr->usage > 0 )
         return;
      else if( ptr->usage < 0 )
      {
         bugf( "SSM: free_string: multiple free/invalid from %s: %20.20s", caller, ( char * )&ptr->buf[0] );
         return;
      }

      numFree++;
      sAllocString -= ( ptr->size + HEADER_SIZE );
      nAllocString--;

      if( !ssm_buf_free || ssm_buf_free > ptr )
         ssm_buf_free = ptr;

      if( fBootDb )
      {
         TempHash *ptr;
         TempHash *walk;
         int ihash = strlen( str ) % MAX_KEY_HASH;

         for( ptr = temp_string_hash[ihash]; ptr; ptr = ptr->next )
         {
            if( ptr->str != str )
               continue;
            else if( ptr == temp_string_hash[ihash] )
               temp_string_hash[ihash] = ptr->next;
            else
               for( walk = temp_string_hash[ihash]; walk; walk = walk->next )
               {
                  if( walk->next == ptr )
                  {
                     walk->next = ptr->next;
                     break;
                  }
               }

            free( ptr );
            break;
         }
      }
      return;
   }

   sOverFlowString -= strlen( str ) + 1;
   nOverFlowString--;
   free( str );
}


/*
 * Read and allocate space for a string from a file.
 * This replaces db.c fread_string
 * This is modified version of Furey's fread_string from Merc
 */
char *_fread_string( FILE * fp, const char *caller )
{
   char buf[MAX_STRING_LENGTH * 4];
   char *ptr = buf;
   char c;

   do
   {
      c = getc( fp );
   }
   while( isspace( c ) );

   if( ( *ptr++ = c ) == '~' )
      return &str_empty[0];

   for( ;; )
   {
      switch ( *ptr = getc( fp ) )
      {
         default:
            ptr++;
            break;

         case EOF:
            bugf( "Fread_string: EOF" );
            raise( SIGSEGV );
            break;

         case '\n':
            ptr++;
            *ptr++ = '\r';
            break;

         case '\r':
            break;

         case '~':
            *ptr = '\0';
            if( fBootDb )
            {
               ptr = temp_hash_find( buf );
               if( ptr )
                  return _str_dup( ptr, caller );

               ptr = _str_dup( buf, caller );
               temp_hash_add( ptr );
               return ptr;
            }

            ptr = _str_dup( buf, caller );
            tail_chain(  );
            return ptr;
      }
   }
}


/* 
 * This is a modified version of fread_string:
 * It reads till a '\n' or a '\r' instead of a '~' (like fread_string).
 * ROM uses this function to read in the socials.
 * -- Wreck
 */
char *_fread_string_eol( FILE * fp, const char *caller )
{
   char buf[MAX_STRING_LENGTH * 4];
   char *ptr = buf;
   char c;

   do
   {
      c = getc( fp );
   }
   while( isspace( c ) );

   if( ( *ptr++ = c ) == '\n' )
      return &str_empty[0];

   for( ;; )
   {
      switch ( *ptr = getc( fp ) )
      {
         default:
            ptr++;
            break;

         case EOF:
            bugf( "Fread_string: EOF" );
            raise( SIGSEGV );
            break;

         case '\n':
         case '\r':
            *ptr = '\0';
            if( fBootDb )
            {
               ptr = temp_hash_find( buf );
               if( ptr )
                  return _str_dup( ptr, caller );

               ptr = _str_dup( buf, caller );
               temp_hash_add( ptr );
               return ptr;
            }

            return _str_dup( buf, caller );
      }
   }
}

/*
 * Read string into user supplied buffer.
 * Modified version of Furey's fread_string
 */
void temp_fread_string( FILE * fp, char *buf )
{
   char *ptr = buf;
   char c;

   do
   {
      c = getc( fp );
   }
   while( isspace( c ) );

   if( ( *ptr++ = c ) == '~' )
   {
      *buf = '\0';
      return;
   }

   for( ;; )
   {
      switch ( *ptr = getc( fp ) )
      {
         default:
            ptr++;
            break;

         case EOF:
            bugf( "Fread_string: EOF" );
            raise( SIGSEGV );
            break;

         case '\n':
            ptr++;
            *ptr++ = '\r';
            break;

         case '\r':
            break;

         case '~':
            *ptr = '\0';
            return;
      }
   }
}


/* Lookup the string in the boot-time hash table. */
char *temp_hash_find( const char *str )
{
   TempHash *ptr;
   int len;
   int ihash;

   if( !fBootDb || !*str )
      return 0;

   len = strlen( str );
   ihash = len % MAX_KEY_HASH;

   for( ptr = temp_string_hash[ihash]; ptr; ptr = ptr->next )
   {
      if( *ptr->str != *str )
         continue;
      else if( strcmp( ptr->str, str ) )
         continue;
      else
         return ptr->str;
   }

   return 0;
}


/*
 * Add a reference in the temporary hash table.
 * String is still in the linked list structure but
 * reference is kept here for quick lookup at boot time;
 */
void temp_hash_add( char *str )
{
   int len;
   int ihash;
   TempHash *add;

   if( !fBootDb || !*str || ( str <= string_space && str >= top_string ) )
      return;

   len = strlen( str );
   ihash = len % MAX_KEY_HASH;
   add = ( TempHash * ) malloc( sizeof( TempHash ) );
   add->next = temp_string_hash[ihash];
   temp_string_hash[ihash] = add;
   add->len = len;
   add->str = str;
}

/* Free the temp boot string hash table */
void boot_done( void )
{
   TempHash *ptr, *next;
   int ihash;

   for( ihash = 0; ihash < MAX_KEY_HASH; ihash++ )
   {
      for( ptr = temp_string_hash[ihash]; ptr; ptr = next )
      {
         next = ptr->next;
         free( ptr );
      }
   }

   free( temp_string_hash );
   temp_string_hash = 0;   /* Bug check in case someone accesses later */
}
