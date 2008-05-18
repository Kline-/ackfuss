#ifndef DEC_TYPEDEFS_H
#include "typedefs.h"
#endif
#ifndef DEC_LISTS_H
#include "lists.h"
#endif



struct hash_entry_tp
{
   bool is_free;  /* Ramias:for run-time checks of LINK/UNLINK */
   HASH_ENTRY *next;
   void *reference;
   int key;
};

typedef struct hash_table_tp hash_table;

struct hash_table_tp
{
   int max_hash;
   HASH_ENTRY **table;
};



hash_table *create_hash_table( int max_hash );
void del_hash_entry( hash_table * hash_head, int key );
void add_hash_entry( hash_table * hash_head, int key, void *entry );
void *get_hash_entry( hash_table * hash_head, int key );
void clear_hash_table( hash_table * hash_head );
void delete_hash_table( hash_table * hash_head );
