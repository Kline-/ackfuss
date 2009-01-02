/* Codebase macros - Change as you might need.
 * Yes, Rogel, you can gloat all you want. You win, this is cleaner, though not by a whole lot.
 */
/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#ifndef __IMC2CFG_H__
#define __IMC2CFG_H__

#if !defined(IMCSTANDALONE)
#define CH_IMCDATA(ch)           ((ch)->pcdata->imcchardata)
#define CH_IMCLEVEL(ch)          ((ch)->level)
#define CH_IMCNAME(ch)           ((ch)->name)
#define CH_IMCSEX(ch)            ((ch)->sex)
#define CH_IMCTITLE(ch)          ((ch)->pcdata->title)
#endif

#if defined(IMCACK)
#define CH_IMCRANK(ch)           (str_cmp(ch->pcdata->who_name,"off") ? ch->pcdata->who_name : IS_IMMORTAL(ch) ? "Imm" : class_table[(ch)->p_class].who_name)
#endif

#endif

#if defined(IMCSTANDALONE)

typedef unsigned char bool;

#if !defined(FALSE)
#define FALSE 0
#endif

#if !defined(TRUE)
#define TRUE 1
#endif

#define CH_IMCDATA(ch)           ((ch)->imcchardata)
#define CH_IMCLEVEL(ch)          ((ch)->level)
#define CH_IMCNAME(ch)           ((ch)->name)
#define CH_IMCSEX(ch)            ((ch)->sex)
#define CH_IMCTITLE(ch)          ( "User" )
#define CH_IMCRANK(ch)           ( "User" )

typedef enum
{
   SEX_NEUTRAL, SEX_MALE, SEX_FEMALE
} genders;

#define CON_PLAYING 1
#define LOWER(c)		((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))

typedef struct user_data CHAR_DATA;
typedef struct conn_data DESCRIPTOR_DATA;

struct user_data
{
   struct imcchar_data *imcchardata;
   char *name;
   int level;
   short sex;
};

struct conn_data
{
   DESCRIPTOR_DATA *next;
   DESCRIPTOR_DATA *prev;
   CHAR_DATA *original;
   CHAR_DATA *character;
   short connected;
};

DESCRIPTOR_DATA *first_descriptor;
DESCRIPTOR_DATA *last_descriptor;

#endif
