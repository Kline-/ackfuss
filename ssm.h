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


/* Mostly yanked from ssm.c */

#define intType short int
#define uintType unsigned intType
#define intTypeSize (sizeof( intType ))
#define addrType void *
#define addrTypeSize (sizeof( addrType ))
#define addrSizeMask (sizeof( addrType ) - 1)

typedef struct BE BufEntry;

struct BE
{
   BufEntry *next;
   uintType size; /* size of the chunk (regardless of NULL CHAR) */
   intType usage; /* how many pointers to the string */
   intType ref;   /* for reference checking */
   const char *caller;  /* who allocated this chunk originally */
   char buf[1];   /* chunk starts here */
};

/*
 * This is for the temporary hashing of strings at bootup to speedup
 * comparison/crunching of the string space. The temp_string_hash will
 * be freed after boot_done() is called.
 */
typedef struct TH TempHash;

struct TH
{
   TempHash *next;
   intType len;
   char *str;
};

extern TempHash **temp_string_hash;

/* These are the original Merc vars in db.c */
extern char str_empty[1];
extern char *string_space;
extern char *top_string;
extern long nAllocString;
extern long sAllocString;
extern long nOverFlowString;
extern long sOverFlowString;
extern long hwOverFlow;
extern int numFree;
extern bool Full;

int defrag_heap( void );
char *_str_dup( const char *, const char * );   /* str_dup is now a macro */
void _free_string( char *, const char * );   /* another macro          */
char *_fread_string( FILE *, const char * ); /* fread_string as well   */
char *fread_word_dup( FILE * );  /* Implement later to check words also */
void temp_hash_add( char * );
char *temp_hash_find( const char * );

/*
 * ssm_buf_head points to start of shared space,
 * ssm_buf_free points to next free block
 */
extern BufEntry *ssm_buf_head, *ssm_buf_free;

/* To allocate more memory increase MAX_CHUNKS */
#ifndef MAX_CHUNKS
#define               MAX_CHUNKS      85
#endif
#define               CHUNK_SIZE      0xfff0 /* Don't mess with this */
extern long MAX_STRING;
extern int HEADER_SIZE;
extern long tot;

/*
 * Not sure what is a good value for MAX_FREE 
 * If a dup fails str_dup will not defrag unless the number
 * of numFree >= MAX_FREE. numFree is NOT the current number of free blocks,
 * it is just a counter so defrag doesnt start dragging the game in the
 * case of a lot of failed dups.
 */
#define   MAX_FREE     1000
