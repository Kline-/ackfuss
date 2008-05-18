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

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
/* For forks etc. */
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define DEC_STRFUNS_H		1

#ifndef  DEC_ACK_H
#include "ack.h"
#include "tables.h"
#endif




char *learnt_name args( ( int learnt ) );
int my_strlen args( ( const char *text ) );
char *get_moon_phase_name args( ( void ) );
char *get_tribe_standing_name args( ( int standing ) );
bool is_number args( ( char *arg ) );
bool is_name args( ( const char *str, char *namelist ) );
bool list_in_list args( ( char *first_list, char *second_list ) );
int number_argument args( ( char *argument, char *arg ) );
char *one_argument args( ( char *argument, char *arg_first ) );
char *two_args args( ( char *argument, char *arg_first, char *arg_second ) );
char *str_mod args( ( char *mod_string, char *argument ) );
void rand_arg args( ( char *argument, char *output ) );
char *space_pad args( ( const char *str, sh_int final_size ) );
void safe_strcat args( ( int max_len, char *dest, char *source ) );
char *center_text args( ( char *text, int width ) );
char *item_type_name args( ( OBJ_DATA * obj ) );
char *affect_loc_name args( ( int location ) );
char *affect_bit_name args( ( int vector ) );
char *raffect_bit_name args( ( int vector ) );

char *extra_bit_name args( ( int extra_flags ) );
char *race_name args( ( CHAR_DATA * ch ) );
char *short_race_name args( ( CHAR_DATA * ch ) );
char *get_adept_name args( ( CHAR_DATA * ch ) );
bool str_cmp args( ( const char *astr, const char *bstr ) );
bool str_prefix args( ( const char *astr, const char *bstr ) );
bool str_infix args( ( const char *astr, const char *bstr ) );
bool str_suffix args( ( const char *astr, const char *bstr ) );
char *capitalize args( ( const char *str ) );
void smash_tilde args( ( char *str ) );
char *strip_out args( ( const char *orig, const char *strip ) );
char *strip_color args( ( const char *orig, const char *strip ) );
int nocol_strlen( const char *text );
int ccode_len( const char *text, sh_int desired );

void pre_parse( char *list, char *victimname, char *containername, char *things );
char *one_word( char *argument, char *arg_first );
