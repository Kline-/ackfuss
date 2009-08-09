/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_SAVE_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* save.c */
char *cap_nocol P_((const char *str));
char *initial P_((const char *str));
void save_char_obj P_((CHAR_DATA *ch));
void fwrite_char P_((CHAR_DATA *ch, FILE *fp));
void fwrite_obj P_((CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest));
void abort_wrapper P_((void));
bool load_char_obj P_((DESCRIPTOR_DATA *d, char *name, bool system_call));
void fread_char P_((CHAR_DATA *ch, FILE *fp));
void fread_obj P_((CHAR_DATA *ch, FILE *fp));
void fread_corpse P_((FILE *fp));
void fwrite_corpse P_((OBJ_DATA *obj, FILE *fp, int iNest));
void save_corpses P_((void));
void save_marks P_((void));
void save_bans P_((void));

#undef P_
