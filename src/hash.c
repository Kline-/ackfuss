/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#include "h/globals.h"

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HASH_H
#include "h/hash.h"
#endif

/*
 * Deals with generic hashing tables.
 */


void *get_hash_entry( hash_table * hash_head, int key )
{
    int a;
    HASH_ENTRY *rvalue;
    HASH_ENTRY **this_hash_table;

    a = key % hash_head->max_hash;
    this_hash_table = hash_head->table;


    for ( rvalue = this_hash_table[a]; rvalue != NULL; rvalue = rvalue->next )
        if ( rvalue->key == key )
            break;

    if ( rvalue != NULL )
        return rvalue->reference;
    else
        return NULL;
}

void add_hash_entry( hash_table * hash_head, int key, void *entry )
{
    int a;
    HASH_ENTRY *temp;
    HASH_ENTRY **this_hash_table;

    a = key % hash_head->max_hash;
    this_hash_table = hash_head->table;

    temp = new HASH_ENTRY;
    temp->key = key;
    temp->reference = entry;
    temp->next = this_hash_table[a];

    this_hash_table[a] = temp;
}

void del_hash_entry( hash_table * hash_head, int key )
{
    int a;
    HASH_ENTRY *prev_entry;
    HASH_ENTRY *search_entry;
    HASH_ENTRY **this_hash_table;

    a = key % hash_head->max_hash;
    this_hash_table = hash_head->table;

    prev_entry = NULL;
    for ( search_entry = this_hash_table[a]; search_entry != NULL; search_entry = search_entry->next )
    {
        if ( search_entry->key == key )
            break;
        prev_entry = search_entry;
    }

    if ( search_entry != NULL )
    {
        if ( prev_entry == NULL )
            this_hash_table[a] = NULL;
        else
            prev_entry->next = search_entry->next;
        delete search_entry;
    }
}

hash_table *create_hash_table( int max_hash )
{
    int a;
    hash_table *hash_head;
    HASH_ENTRY **this_hash_table;

    this_hash_table = (HASH_ENTRY **)getmem( sizeof( HASH_ENTRY * ) * max_hash );
    hash_head = (struct hash_table_tp *)getmem( sizeof( struct hash_table_tp ) );

    hash_head->max_hash = max_hash;
    hash_head->table = this_hash_table;

    for ( a = 0; a < max_hash; a++ )
        this_hash_table[a] = NULL;

    return hash_head;

}

void clear_hash_table( hash_table * hash_head )
{
    int a;
    HASH_ENTRY *entry, *next_entry;
    HASH_ENTRY **this_hash_table;

    this_hash_table = hash_head->table;

    for ( a = 0; a < hash_head->max_hash; a++ )
    {
        if ( this_hash_table[a] != NULL )
        {
            for ( entry = this_hash_table[a]; entry != NULL; entry = next_entry )
            {
                next_entry = entry->next;
                delete entry;
            }
            this_hash_table[a] = NULL;
        }
    }
}

void delete_hash_table( hash_table * hash_head )
{

    int a;
    HASH_ENTRY *entry, *next_entry;
    HASH_ENTRY **this_hash_table;
    this_hash_table = hash_head->table;

    for ( a = 0; a < hash_head->max_hash; a++ )
    {
        if ( this_hash_table[a] != NULL )
        {
            for ( entry = this_hash_table[a]; entry != NULL; entry = next_entry )
            {
                next_entry = entry->next;
                delete entry;
            }
            this_hash_table[a] = NULL;
        }
    }
    dispose( this_hash_table, sizeof( HASH_ENTRY * ) * hash_head->max_hash );
    dispose( hash_head, sizeof( *hash_head ) );
}
