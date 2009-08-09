/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#define DEC_SPEC_FUN_H

#if __STDC__ || defined(__cplusplus)
#define P_(s) s
#else
#define P_(s) ()
#endif

/* spec_fun.c */
SPEC_FUN *spec_lookup P_((const char *name));
char *rev_spec_lookup P_((SPEC_FUN *func));
void print_spec_lookup P_((char *buf));
bool dragon P_((CHAR_DATA *ch, char *spell_name));
DECLARE_SPEC_FUN(spec_breath_any);
DECLARE_SPEC_FUN(spec_breath_acid);
DECLARE_SPEC_FUN(spec_breath_fire);
DECLARE_SPEC_FUN(spec_breath_frost);
DECLARE_SPEC_FUN(spec_breath_gas);
DECLARE_SPEC_FUN(spec_breath_lightning);
DECLARE_SPEC_FUN(spec_cast_adept);
DECLARE_SPEC_FUN(spec_cast_cleric);
DECLARE_SPEC_FUN(spec_cast_judge);
DECLARE_SPEC_FUN(spec_cast_mage);
DECLARE_SPEC_FUN(spec_cast_undead);
DECLARE_SPEC_FUN(spec_executioner);
DECLARE_SPEC_FUN(spec_mino_guard);
DECLARE_SPEC_FUN(spec_fido);
DECLARE_SPEC_FUN(spec_janitor);
DECLARE_SPEC_FUN(spec_mayor);
DECLARE_SPEC_FUN(spec_poison);
DECLARE_SPEC_FUN(spec_thief);
DECLARE_SPEC_FUN(spec_policeman);
DECLARE_SPEC_FUN(spec_cast_cadaver);
DECLARE_SPEC_FUN(spec_undead);
DECLARE_SPEC_FUN(spec_stephen);
DECLARE_SPEC_FUN(spec_rewield);
DECLARE_SPEC_FUN(spec_sylai_priest);
DECLARE_SPEC_FUN(spec_cast_bigtime);
DECLARE_SPEC_FUN(spec_sage);
DECLARE_SPEC_FUN(spec_vamp_hunter);

#undef P_
