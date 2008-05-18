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

/* Scans vnums in area files and prints out those used in each file */

#define __USE_GNU

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct range_entry hash_entry;

struct range_entry
{
   struct range_entry *next;
   int vnum;
   int offset; /* To begging of list or end */
};

#define MAX_HASH 2048

hash_entry *hash_table[MAX_HASH];


hash_entry *get_hash_entry( int vnum )
{
   int a;
   hash_entry *rvalue;

   a = vnum % MAX_HASH;

   for( rvalue = hash_table[a]; rvalue != NULL; rvalue = rvalue->next )
      if( rvalue->vnum == vnum )
         break;

   return rvalue;
}

void add_hash_entry( hash_entry * entry )
{
   int a;

   a = entry->vnum % MAX_HASH;

   entry->next = hash_table[a];
   hash_table[a] = entry;
}

void del_hash_entry( hash_entry * entry )
{
   int a;
   hash_entry *prev_entry;
   hash_entry *search_entry;

   a = entry->vnum % MAX_HASH;

   prev_entry = NULL;
   for( search_entry = hash_table[a]; search_entry != NULL; search_entry = search_entry->next )
   {
      if( search_entry == entry )
         break;
      prev_entry = search_entry;
   }

   if( search_entry != NULL )
      if( prev_entry == NULL )
         hash_table[a] = NULL;
      else
         prev_entry->next = entry->next;

   free( entry );
}

void setup_hash_table( void )
{
   int a;

   for( a = 0; a < MAX_HASH; a++ )
      hash_table[a] = NULL;

}

void clear_hash_table( void )
{
   int a;
   hash_entry *entry, *next_entry;

   for( a = 0; a < MAX_HASH; a++ )
   {
      if( hash_table[a] != NULL )
      {
         for( entry = hash_table[a]; entry != NULL; entry = next_entry )
         {
            next_entry = entry->next;
            free( entry );
         }
         hash_table[a] = NULL;
      }
   }
}

#define ADD_BLOCK_SIZE 50

int getline( char **buffer, int *size, FILE * file )
{
   char c;
   char *buf;
   char *curpos;
   int a, cursize;

   a = 0;
   cursize = *size;
   buf = *buffer;
   curpos = buf;

   for( ;; )
   {
      c = getc( file );

      if( c == '\n' )
         break;

      if( c == '\r' )
         continue;

      a++;
      if( a == cursize )
      {
         cursize += ADD_BLOCK_SIZE;
         buf = realloc( buf, cursize );
         curpos = buf + a;
      }

      *curpos++ = c;
   }

   *curpos = '\0';

   *buffer = buf;
   *size = cursize;
   return a;
}

void add_vnum( int vnum )
{
   hash_entry *entry;
   hash_entry *range, *temp;
   int start_vnum;
   int end_vnum;
   int cnt;

   /*
    * Adds a vnum, adding to beggining or ends 
    */

   start_vnum = vnum;
   end_vnum = vnum;

   for( ;; )
   {
      cnt = 0;

      if( ( range = get_hash_entry( start_vnum - 1 ) ) != NULL )
      {
         if( range->offset > 0 )
            printf( "ERROR in ranges vnum: %i range start: %i range end: %i",
                    start_vnum, range->vnum, range->vnum + range->offset );
         else
         {
            start_vnum = range->vnum + range->offset;
            if( range->offset != 0 && ( temp = get_hash_entry( start_vnum ) ) != NULL )
               del_hash_entry( temp );
            del_hash_entry( range );
            cnt = 1;
         }
      }

      if( ( range = get_hash_entry( end_vnum + 1 ) ) != NULL )
      {
         if( range->offset < 0 )
            printf( "ERROR in ranges vnum: %i range start: %i range end: %i",
                    end_vnum, range->vnum + range->offset, range->vnum );
         else
         {
            end_vnum = range->vnum + range->offset;
            if( range->offset != 0 && ( temp = get_hash_entry( start_vnum ) ) != NULL )
               del_hash_entry( temp );
            del_hash_entry( range );
            cnt = 1;
         }
      }

      if( !cnt )
         break;
   }

   if( start_vnum == end_vnum )
   {
      entry = malloc( sizeof( hash_entry ) );
      entry->vnum = start_vnum;
      entry->offset = 0;
      add_hash_entry( entry );
   }
   else
   {
      entry = malloc( sizeof( hash_entry ) );
      entry->vnum = start_vnum;
      entry->offset = end_vnum - start_vnum;
      add_hash_entry( entry );

      entry = malloc( sizeof( hash_entry ) );
      entry->vnum = end_vnum;
      entry->offset = start_vnum - end_vnum;
      add_hash_entry( entry );
   }
}


#define BUF_SIZE 255
#define str_cmp(a,b) strcasecmp(a,b)


void collate_vnums( FILE * in_file )
{
   int vnum;
   char *line;
   int line_size;
   int min_vnum = 65000;
   int max_vnum = 0;
   hash_entry *entry;

   line = malloc( BUF_SIZE );
   line_size = BUF_SIZE;

   for( ;; )
   {
      getline( &line, &line_size, in_file );

      if( line[0] != '#' )
         continue;

      vnum = atoi( line + 1 );

      if( vnum == 0 )
         break;

      if( vnum > max_vnum )
         max_vnum = vnum;

      if( vnum < min_vnum )
         min_vnum = vnum;

      add_vnum( vnum );
   }

   if( max_vnum != 0 )
   {
      for( vnum = min_vnum; vnum <= max_vnum; vnum++ )
      {
         if( ( entry = get_hash_entry( vnum ) ) != NULL && entry->offset >= 0 )
         {
            if( entry->offset == 0 )
               printf( "     %i\n", entry->vnum );
            else
               printf( "     %i-%i\n", entry->vnum, entry->vnum + entry->offset );
         }
      }
   }

   clear_hash_table(  );
   free( line );
}

int main(  )
{
   FILE *farea_lst;
   FILE *fcur_area;
   char *sarea_buffer;
   char *sfile_line;
   int sarea_buffer_size;
   int sfile_line_size;

   sarea_buffer = malloc( BUF_SIZE );
   sfile_line = malloc( BUF_SIZE );
   sarea_buffer_size = BUF_SIZE;
   sfile_line_size = BUF_SIZE;

   setup_hash_table(  );
   farea_lst = fopen( "area.lst", "r" );

   for( getline( &sarea_buffer, &sarea_buffer_size, farea_lst );
        sarea_buffer[0] != '$' && !feof( farea_lst ); getline( &sarea_buffer, &sarea_buffer_size, farea_lst ) )
   {
      /*
       * sarea_buffer contains next filename 
       */
      printf( "\nFile: %s\n", sarea_buffer );
      fcur_area = fopen( sarea_buffer, "r" );
      for( ;; )
      {
         getline( &sfile_line, &sfile_line_size, fcur_area );
         if( !str_cmp( sfile_line, "#$" ) )
            break;

         if( sfile_line[0] != '#' )
            continue;

         if( !str_cmp( sfile_line, "#ROOMS" ) )
         {
            printf( "Rooms:\n" );
            collate_vnums( fcur_area );
         }

         if( !str_cmp( sfile_line, "#OBJECTS" ) )
         {
            printf( "Objects:\n" );
            collate_vnums( fcur_area );
         }

         if( !str_cmp( sfile_line, "#MOBILES" ) )
         {
            printf( "Mobiles:\n" );
            collate_vnums( fcur_area );
         }
      }
      fclose( fcur_area );
   }
   fclose( farea_lst );

   free( sarea_buffer );
   free( sfile_line );

   return 1;
}
