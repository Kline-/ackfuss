/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_BITMASK_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* bitmask.c */
bool remove_bit P_((BITMASK *mask, int bit));
bool set_bit P_((BITMASK *mask, int bit));
bool is_set P_((BITMASK *mask, int bit));
int *serialize_bitmask P_((BITMASK *mask));
bool free_bitmask P_((BITMASK *pBmask));
void load_bitmask P_((BITMASK *pBmask, FILE *fp));
char *save_bitmask P_((BITMASK *pBmask));
void bv_to_bm P_((int list, BITMASK *mask));
char *debug_bitmask P_((BITMASK *pBmask));
void bm_test P_((void));

#undef P_
