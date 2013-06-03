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

#ifndef DEC_HELP_H
#include "h/help.h"
#endif

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_STRFUNS_H
#include "h/strfuns.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

DO_FUN(do_help)
{
    HELP_DATA *hlp = NULL;
    char buf[MSL];
    string str;
    char *farg = argument;
    bool found = false;
    bool shelp = false;

    farg = one_argument(farg, buf);

    if ( argument[0] != '\0' )
    {
        if ( !str_prefix(buf, "-find") )
        {
            send_to_char(find_helps(farg, IS_IMMORTAL(ch) ? true : false), ch);
            return;
        }
        if ( !str_prefix(buf, "-search") )
        {
            send_to_char(grep_helps(farg, IS_IMMORTAL(ch) ? true : false), ch);
            return;
        }
    }

    argument = strlower(argument);
    smash_replace(argument, " ", "_");
    smash_replace(argument, ":", "_");

    if ( !str_prefix("shelp_", argument) )
        shelp = true;

    if ( argument[0] == '\0' ) /* special case for the main help index */
    {
        if( exists_help(HELP_INDEX) )
        {
            hlp = load_help(HELP_INDEX);
            ch->send(hlp->description);
            return;
        }
    }
    else if ( !str_cmp(argument, BHELP_INDEX) ) /* special case for the builder */
    {
        if( exists_help(BHELP_INDEX) )
        {
            hlp = load_help(BHELP_INDEX);
            ch->send(hlp->description);
            return;
        }
    }
    else if ( !str_cmp(argument, SHELP_INDEX) ) /* special case for shelp */
    {
        if( exists_help(SHELP_INDEX) )
        {
            hlp = load_help(SHELP_INDEX);
            ch->send(hlp->description);
            return;
        }
    }
    else
    {
        str = argument;
        str += ".";
        str += IS_IMMORTAL(ch) ? HELP_IMM : HELP_MORT;

        if( exists_help(str.c_str()) )
        {
            hlp = load_help(str.c_str());
            if ( !IS_IMMORTAL(ch) && hlp->imm )
                found = false;
            else
                found = true;
            if( found )
                ch->send(hlp->description);
        }
    }

    if ( !IS_IMMORTAL(ch) && !found )
    {
        if ( !shelp )
        {
            ch->send("No help on that word.\r\n");
            snprintf(log_buf, MIL, "Missing help: %s attempted by %s.", argument, ch->name.c_str());
            monitor_chan(log_buf, MONITOR_HELPS);
        }
        else
        {
            ch->send("No sHelp for that skill/spell.\r\n");
            snprintf(log_buf, MIL, "Missing sHelp: %s attempted by %s.", argument, ch->name.c_str());
            monitor_chan(log_buf, MONITOR_HELPS);
        }
        return;
    }

    if ( IS_IMMORTAL(ch) )
    {
        str = argument;
        str += ".";
        str += HELP_MORT;

        if( exists_help(str.c_str()) )
        {
            if( found )
                ch->send("\r\n");
            hlp = load_help(str.c_str());
            found = true;
            ch->send(hlp->description);
        }

        if ( !found )
        {
            if ( !shelp )
            {
                ch->send("No help on that word.\r\n");
                snprintf(log_buf, MIL, "Missing help: %s attempted by %s.", argument, ch->name.c_str());
                monitor_chan(log_buf, MONITOR_HELPS);
            }
            else
            {
                ch->send("No sHelp for that skill/spell.\r\n");
                snprintf(log_buf, MIL, "Missing sHelp: %s attempted by %s.", argument, ch->name.c_str());
                monitor_chan(log_buf, MONITOR_HELPS);
            }
            return;
        }
    }

    return;
}

DO_FUN(do_shelp)
{
    /*
     * Like help, except for spells and skills.
     */
    int sn;
    char buf[MSL];
    buf[0] = '\0';

    if ( argument[0] == '\0' )
    {
        do_help( ch, SHELP_INDEX );
        return;
    }

    if ( ( sn = skill_lookup( argument ) ) < 0 )
    {
        snprintf( buf, MSL, "No sHelp found for argument:%s\r\n", argument );
        send_to_char( buf, ch );
        return;
    }
    snprintf( buf, MSL, "shelp_%s", skill_table[sn].name );
    do_help(ch, buf);

    return;
}

bool exists_help( const char *help )
{
    HELP_DATA *hlp;
    list<HELP_DATA *>::iterator li;
    char search[MSL] = {'\0'};
    char tmp[MSL] = {'\0'};
    string str1, str2;

    if( !isalpha(*help) )
        return false;

    /* Check the cache list before trying to load from file --Kline */
    for( li = help_list.begin(); li != help_list.end(); li++ )
    {
        hlp = *li;
        if( hlp->name.find(help) != string::npos )
            return true;
    }

    snprintf( search, MSL, "find %s%s/ -iname %s -printf '%%f '", HELP_DIR, initial(help), help );
    snprintf( tmp, MSL, "%s", _popen(search) );
    str1 = tmp;

    if( str1.empty() )
        return false;
    if( str1.empty() )
    {
        /* Search for plural */
        snprintf( search, MSL, "find %s%s/ -iname %ss -printf '%%f '", HELP_DIR, initial(help), help );
        snprintf( tmp, MSL, "%s", _popen(search) );
        str2 = tmp;
        if( str2.empty() )
            return false;
    }

    return true;
}

HELP_DATA *load_help( const char *help )
{
    HELP_DATA *ret;
    list<HELP_DATA *>::iterator li;
    FILE *fp;
    char tmp[MSL] = {'\0'};
    char search[MSL] = {'0'};

    /* Check the cache list before trying to load from file --Kline */
    for( li = help_list.begin(); li != help_list.end(); li++ )
    {
        ret = *li;
        if( ret->name.find(help) != string::npos )
        {
            snprintf( log_buf, MIL, "load_help: using cached copy of (%s) which expires in (%d)", ret->name.c_str(), ret->cache_time );
            monitor_chan( log_buf, MONITOR_DEBUG );
            ret->cache_time = sysdata.pulse_cache;
            return ret;
        }
    }

    ret = new HELP_DATA;

    snprintf( search, MSL, "find %s%s/ -iname %s -printf '%%p'", HELP_DIR, initial(help), help );
    snprintf( tmp, MSL, "%s", _popen(search) );
    fp = file_open( tmp, "r" );

    snprintf( tmp, MSL, "ls -1 %s%s/%s | cut -d/ -f4", HELP_DIR, initial(help), help );
    ret->name = _popen(tmp);
    ret->name.resize(ret->name.length()-1);  /* Strip off the appended \n --Kline */
    if( ret->name.find(HELP_MORT) != string::npos )
        ret->imm = false;
    else
        ret->imm = true;

    ret->description = "\r\n";
    ret->description += fread_to_eof(fp);

    file_close(fp);

    return ret;
}

void cache_check_help( )
{
    HELP_DATA *hlp;
    list<HELP_DATA *>::iterator li;

    for( li = help_list.begin(); li != help_list.end(); li++ )
    {
        hlp = *li;
        if( --hlp->cache_time <= 0 )
        {
            snprintf( log_buf, MIL, "cache_check_help: removing file (%s) from cache, time expired", hlp->name.c_str() );
            monitor_chan( log_buf, MONITOR_DEBUG );
            delete hlp;
            li = help_list.erase(li);
        }
    }

    return;
}

int count_helps( void )
{
    char buf[MSL];
    char tmp[MSL];

    snprintf(tmp, MSL, "ls -1 -R %s/*/*.* | wc -l", HELP_DIR);
    snprintf(buf, MSL, "%s", _popen(tmp));

    return atoi(buf);
}

char *find_helps( const char *search, bool imm )
{
    static char ret[MSL];
    char tmp[MSL] = {'\0'};
    char split[MSL] = {'\0'};
    string str;
    size_t pos;

    if ( !isalpha(*search) )
        return "You can only search for letters.\r\n";

    if ( strlen(search) < 2 )
        return "Searches must be at least two letters.\r\n";

    if ( imm )
        snprintf(split, MSL, "find %s -maxdepth 2 -iname \\*%s\\* -printf '%%f '", HELP_DIR, search);
    else
        snprintf(split, MSL, "find %s -maxdepth 2 -iname \\*%s\\*.%s -printf '%%f '", HELP_DIR, search, HELP_MORT);
    snprintf(tmp, MSL, "%s", _popen(split));

    str = tmp;

    if ( str.empty() )
        return "Nothing found.\r\n";

    snprintf(tmp, MSL, ".%s", HELP_MORT);
    while ( (pos = str.find(tmp)) < str.size() )
        str.replace(pos, strlen(tmp), "");

    if ( imm )
    {
        snprintf(tmp, MSL, ".%s", HELP_IMM);
        while ( (pos = str.find(tmp)) < str.size() )
            str.replace(pos, strlen(tmp), "");
    }

    snprintf(ret, MSL, "Found the following possible matches:\r\n%s\r\n", str.c_str());
    return ret;
}

char *grep_helps( const char *search, bool imm )
{
    static char ret[MSL];
    char tmp[MSL] = {'\0'};
    char split[MSL] = {'\0'};
    string str;
    size_t pos;

    if ( !isalpha(*search) )
        return "You can only search for letters.\r\n";

    if ( strlen(search) < 2 )
        return "Searches must be at least two letters.\r\n";

    if ( imm )
        snprintf(split, MSL, "grep -i -l -R '%s' %s*/* | cut -c %d-", search, HELP_DIR, static_cast<int>(strlen(HELP_DIR) + 3));
    else
        snprintf(split, MSL, "grep -i -l -R '%s' %s*/*.%s | cut -c %d-", search, HELP_DIR, HELP_MORT, static_cast<int>(strlen(HELP_DIR) + 3));
    snprintf(tmp, MSL, "%s", _popen(split));

    str = tmp;

    if ( str.empty() )
        return "Nothing found.\r\n";

    snprintf(tmp, MSL, ".%s", HELP_MORT);
    while ( (pos = str.find(tmp)) < str.size() )
        str.replace(pos, strlen(tmp), "");

    if ( imm )
    {
        snprintf(tmp, MSL, ".%s", HELP_IMM);
        while ( (pos = str.find(tmp)) < str.size() )
            str.replace(pos, strlen(tmp), "");
    }

    snprintf(ret, MSL, "Found the following possible matches:\r\n%s", str.c_str());
    return ret;
}
