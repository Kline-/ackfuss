/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_HASH_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

class hash_entry_tp
{
 public:
  hash_entry_tp();
  ~hash_entry_tp();
  bool is_free;
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


/* hash.c */
hash_table *create_hash_table P_((int max_hash));
void del_hash_entry P_((hash_table * hash_head, int key));
void add_hash_entry P_((hash_table * hash_head, int key, void *entry));
void *get_hash_entry P_((hash_table * hash_head, int key));
void clear_hash_table P_((hash_table * hash_head));
void delete_hash_table P_((hash_table * hash_head));

#undef P_
