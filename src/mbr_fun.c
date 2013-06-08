/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
#include "h/includes.h"
#include "h/list.h"

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

const string char_data::gChannelColor( const string& input )
{
    int i = 0;

    if ( IS_NPC( this ) || input.empty() )
        return string();

    if ( !act.test( ACT_COLOR ) )
        return string();

    if ( input == "normal" )
        return "\x1b[0m";

    for ( i = 0; i < MAX_COLOR; i++ )
        if ( input == color_table[i].name )
            break;

    return ansi_table[pcdata->color[i]].value;
}

const int char_data::gTrust() const
{
    CHAR_DATA *ch = NULL;

    if ( desc != NULL && desc->original != NULL )
        ch = desc->original;

    if ( !IS_NPC( ch ) && ch->act.test(ACT_AMBASSADOR) )
        return ( LEVEL_HERO + 1 );

    if ( ch->trust != 0 )
        return ch->trust;

    if ( IS_NPC( ch ) && ch->level >= LEVEL_HERO )
        return LEVEL_HERO - 1;
    else
        return ch->level;

}

bool char_data::check_cooldown( const char *skill )
{
    int sn = skill_lookup(skill);

    if ( sn < 0 )
        return false;

    if ( skill_table[sn].cooldown <= COOLDOWN_NONE )
        return false;

    if ( cooldown[skill_table[sn].cooldown] > 0 )
    {
        switch ( skill_table[sn].cooldown )
        {
            case COOLDOWN_OFF: send("@@eYour offensive abilities are on cooldown right now.\r\n"); break;
            case COOLDOWN_DEF: send("@@lYour defensive abilities are on cooldown right now.\r\n"); break;
        }
        return true;
    }

    return false;
}

bool char_data::check_cooldown( int pos )
{
    if ( pos <= COOLDOWN_NONE )
        return false;

    if ( cooldown[pos] > 0 )
    {
        switch ( pos )
        {
            case COOLDOWN_OFF: send("@@eYour offensive abilities are on cooldown right now.\r\n"); break;
            case COOLDOWN_DEF: send("@@lYour defensive abilities are on cooldown right now.\r\n"); break;
        }
        return true;
    }

    return false;
}

void char_data::gain_exp( int gain )
{
    if ( IS_NPC(this) && !act.test(ACT_INTELLIGENT) )
        return;
    if ( IS_IMMORTAL(this) )
        return;

    if ( (exp + gain) >= MAX_EXP )
        exp = MAX_EXP;
    else
        exp += gain;

    if ( exp < 0 )
        exp = 0;

    return;
}

short char_data::get_level( const char *what )
{
    short i = 0, max = 0;;

    if ( what == '\0' )
        return level;

    if ( !str_prefix(what, "maxmortal") )
    {
        for ( i = 0; i < MAX_CLASS; i++ )
            if ( lvl[i] > max )
                max = lvl[i];
        return max;
    }
    if ( !str_prefix(what, "maxremortal") )
    {
        for ( i = 0; i < MAX_CLASS; i++ )
            if ( lvl2[i] > max )
                max = lvl2[i];
        return max;
    }
    if ( !str_prefix(what, "mortal") )
        return (lvl[CLS_MAG] + lvl[CLS_CLE] + lvl[CLS_THI] + lvl[CLS_WAR] + lvl[CLS_PSI]);
    if ( !str_prefix(what, "remortal") )
        return (lvl2[CLS_SOR] + lvl2[CLS_MON] + lvl2[CLS_ASS] + lvl2[CLS_KNI] + lvl2[CLS_NEC]);
    if ( !str_prefix(what, "psuedo") )
    {
        if ( IS_NPC(this) )
            return get_level();
        else
        {
            if ( (get_level("remortal") / 4) > 0 )
                return (get_level() + (get_level("remortal") / 4));
            else
                return get_level();
        }
    }

    if ( !str_prefix(what, "adept") )
        return IS_NPC(this) ? get_level() / 7 : pcdata->adept_level;
    if ( !str_prefix(what, "mage") )
        return lvl[CLS_MAG];
    if ( !str_prefix(what, "cleric") )
        return lvl[CLS_CLE];
    if ( !str_prefix(what, "thief") )
        return lvl[CLS_THI];
    if ( !str_prefix(what, "warrior") )
        return lvl[CLS_WAR];
    if ( !str_prefix(what, "psionicist") )
        return lvl[CLS_PSI];
    if ( !str_prefix(what, "sorcerer") )
        return lvl2[CLS_SOR];
    if ( !str_prefix(what, "monk") )
        return lvl2[CLS_MON];
    if ( !str_prefix(what, "assassin") )
        return lvl2[CLS_ASS];
    if ( !str_prefix(what, "knight") )
        return lvl2[CLS_KNI];
    if ( !str_prefix(what, "necromancer") )
        return lvl2[CLS_NEC];

    snprintf(log_buf, (2 * MIL), "char_data::get_level(): Received invalid request for '%s'.", what);
    monitor_chan(log_buf, MONITOR_DEBUG);
    return level;
}

const char *char_data::get_name( CHAR_DATA *looker )
{
    if ( looker != NULL )
    {
        if ( can_see(looker, this) )
        {
            if ( IS_NPC(this) )
                return CSTR( short_descr );
            else
            {
                if ( IS_WOLF(this) && (IS_SHIFTED(this) || IS_RAGED(this)) )
                    return "A Large @@bWolf@@N";
                else
                    return name.c_str();
            }
        }
        else
        {
            if ( IS_IMMORTAL(this) )
                return "A Mystical Being";
            else
                return "Someone";
        }
    }
    else
    {
        if ( IS_NPC(this) )
            return CSTR( short_descr );
        else
            return name.c_str();
    }

    return "Unknown!";
}

const char *char_data::get_title( )
{
    if ( IS_NPC(this) )
        return "";
    else
        return pcdata->title;
}

const char *char_data::get_whoname( )
{
    char buf[MSL] = {'\0'};
    short s1, s2, s3, s4, s5;
    const char *output;

    if ( IS_NPC(this) )
        return "";

    if ( IS_IMMORTAL(this) )
    {
        if ( strcmp(pcdata->who_name,"off") )
            return pcdata->who_name;

        switch ( get_level() )
        {
            case MAX_LEVEL - 0: return "@@l~* CREATOR *~@@N ";
            case MAX_LEVEL - 1: return "@@B-* SUPREME *-@@N ";
            case MAX_LEVEL - 2: return "@@a-=MAJOR GOD=-@@N ";
            case MAX_LEVEL - 3: return "@@a--MINOR GOD--@@N ";
            case MAX_LEVEL - 4: return "@@c - IMMORTAL -@@N ";
        }
    }

    if ( IS_ADEPT(this) )
    {
        if ( strcmp(pcdata->who_name,"off") )
            return pcdata->who_name;

        switch ( get_level("adept") )
        {
            case 1:  return "@@W    Mystic    @@N";
            case 2:  return "@@a   Templar    @@N";
            case 3:  return "@@l Illusionist  @@N";
            case 4:  return "@@e   Crusader   @@N";
            case 5:  return "@@d   Warlock    @@N";
            case 6:  return "@@a   Paladin    @@N";
            case 7:  return "@@r    Ranger    @@N";
            case 8:  return "@@c  Gladiator   @@N";
            case 9:  return "@@l    Shogun    @@N";
            case 10: return "@@e    Shamen    @@N";
            case 11: return "@@r    Druid     @@N";
            case 12: return "@@b  Conjurer    @@N";
            case 13: return "@@l Elementalist @@N";
            case 14: return "@@m  Runemaster  @@N";
            case 15: return "@@d Shadowmaster @@N";
            case 16: return "@@b Beastmaster  @@N";
            case 17: return "@@R   Warlord    @@N";
            case 18: return "@@e  Dragonlord  @@N";
            case 19: return "@@d  Demonlord   @@N";
            case 20: return "@@m  Realm Lord  @@N";
        }
    }

    if ( IS_REMORT(this) )
    {
        s1 = get_level("sor"); s2 = get_level("mon"); s3 = get_level("ass"); s4 = get_level("kni"); s5 = get_level("nec");
        snprintf( buf, MSL, "@@m%2d %2d %2d %2d %2d@@N", s1 <= 0 ? 0 : s1, s2 <= 0 ? 0 : s2, s3 <= 0 ? 0 : s3, s4 <= 0 ? 0 : s4, s5 <= 0 ? 0 : s5 );
        output = str_dup( buf );

        return output;
    }
    else
    {
        s1 = get_level("mag"); s2 = get_level("cle"); s3 = get_level("thi"); s4 = get_level("war"); s5 = get_level("psi");
        snprintf( buf, MSL, "@@b%2d %2d %2d %2d %2d@@N", s1 <= 0 ? 0 : s1, s2 <= 0 ? 0 : s2, s3 <= 0 ? 0 : s3, s4 <= 0 ? 0 : s4, s5 <= 0 ? 0 : s5 );
        output = str_dup( buf );

        return output;
    }

    return "";
}

void char_data::rsend( string txt, ... )
{
    DESCRIPTOR_DATA *d;

    if ( this->in_room == NULL )
        return;

    for ( d = first_desc; d; d = d->next )
    {
        if ( d->connected == CON_PLAYING && d->character->in_room == this->in_room )
            d->character->send(txt);
    }

    return;
}

void char_data::send( string txt, ... )
{
    char buf[MSL];
    va_list args;

    if ( this == NULL || desc == NULL )
        return;

    if ( txt.empty() )
        return;

    va_start(args, txt);
    vsprintf(buf, txt.c_str(), args);
    va_end(args);

    if ( desc->showstr_head != NULL )
    {
        char *ssh;

        ssh = (char *)qgetmem( strlen( desc->showstr_head ) + strlen( buf ) + 1 );
        strcpy( ssh, desc->showstr_head );
        strcat( ssh, buf );

        if ( desc->showstr_point )
            desc->showstr_point += ( ssh - desc->showstr_head );
        else
            desc->showstr_point = ssh;

        free(desc->showstr_head);
        desc->showstr_head = ssh;
    }
    else
    {
        desc->showstr_head = (char *)malloc( strlen( buf ) + 1);
        strcpy( desc->showstr_head, buf );
        desc->showstr_point = desc->showstr_head;
    }
    if ( desc->showstr_point == desc->showstr_head )
        show_string( desc, "" );
    return;
}

/* Use for passing any strings to escape vsprintf formatting, such as lists of objects with %s, %d, etc in them.
   Value of the bool doesn't matter, just something to differentiate the func for overloading. --Kline */
void char_data::send( bool raw, string txt )
{
    if ( this == NULL || desc == NULL )
        return;

    if ( txt.empty() )
        return;

    if ( desc->showstr_head != NULL )
    {
        char *ssh;

        ssh = (char *)qgetmem( strlen( desc->showstr_head ) + txt.length() + 1 );
        strcpy( ssh, desc->showstr_head );
        strcat( ssh, txt.c_str() );

        if ( desc->showstr_point )
            desc->showstr_point += ( ssh - desc->showstr_head );
        else
            desc->showstr_point = ssh;

        free(desc->showstr_head);
        desc->showstr_head = ssh;
    }
    else
    {
        desc->showstr_head = (char *)malloc( txt.length() + 1);
        strcpy( desc->showstr_head, txt.c_str() );
        desc->showstr_point = desc->showstr_head;
    }
    if ( desc->showstr_point == desc->showstr_head )
        show_string( desc, "" );

    return;
}

void char_data::set_cooldown( const char *skill )
{
    int sn = skill_lookup(skill);

    if ( sn < 0 )
        return;

    if ( skill_table[sn].cooldown <= COOLDOWN_NONE )
        return;

    cooldown[skill_table[sn].cooldown] += skill_table[sn].beats;

    return;
}

void char_data::set_cooldown( int pos, float duration )
{
    if ( pos <= COOLDOWN_NONE )
        return;

    cooldown[pos] += duration;

    return;
}

void char_data::set_title( const char *title )
{
    if ( IS_NPC(this) )
        return;

    free_string(pcdata->title);
    pcdata->title = str_dup(title);

    return;
}
