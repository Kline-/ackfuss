/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/

#include "h/globals.h"

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_STRFUNS_H
#include "h/strfuns.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

#define KEY( literal, field, value )  if ( !str_cmp( word, literal ) ) { field  = value; fMatch = TRUE;  break;}

bool exists_social( const char *social )
{
    SOCIAL_DATA *soc;
    list<SOCIAL_DATA *>::iterator li;
    char search[MSL] = {'\0'};
    char tmp[MSL] = {'\0'};
    string str;
    
    if( !isalpha(*social) )
        return false;

    /* Check the cache list before trying to load from file --Kline */
    for( li = social_list.begin(); li != social_list.end(); li++ )
    {
        soc = *li;
        if( soc->name.find(social) != string::npos )
            return true;
    }
    
    snprintf( search, MSL, "find %s%s/ -iname %s\\*.%s -printf '%%f '", SOCIAL_DIR, initial(social), social, SOCIAL_EXT );
    snprintf( tmp, MSL, "%s", _popen(search) );
    str = tmp;

    if( str.empty() )
        return false;
    
    return true;
}

SOCIAL_DATA *load_social( const char *social )
{
    SOCIAL_DATA *ret;
    list<SOCIAL_DATA *>::iterator li;
    FILE *fp;
    char tmp[MSL] = {'\0'};
    char search[MSL] = {'0'};
    const char *word;
    bool fMatch = false;
    
    /* Check the cache list before trying to load from file --Kline */
    for( li = social_list.begin(); li != social_list.end(); li++ )
    {
        ret = *li;
        if( ret->name.find(social) != string::npos )
        {
            snprintf( log_buf, MSL, "load_social: using cached copy of (%s) which expires in (%d)", ret->name.c_str(), ret->cache_time );
            monitor_chan( log_buf, MONITOR_DEBUG );
            ret->cache_time = sysdata.pulse_cache;
            return ret;
        }
    }
    
    ret = new SOCIAL_DATA;
    
    snprintf( search, MSL, "find %s%s/ -iname %s\\*.%s -printf '%%p'", SOCIAL_DIR, initial(social), social, SOCIAL_EXT );
    snprintf( tmp, MSL, "%s", _popen(search) );
    fp = file_open( tmp, "r" );
    
    snprintf( tmp, MSL, "ls -1 %s%s/%s*.%s | cut -d/ -f 4 | cut -d. -f 1", SOCIAL_DIR, initial(social), social, SOCIAL_EXT );
    ret->name = _popen(tmp);
    ret->name.resize(ret->name.length()-1); /* Strip off the appended \n --Kline */

    for ( ;; )
    {
        word = feof(fp) ? "End" : fread_word(fp);
        fMatch = false;
        
        switch ( UPPER(word[0]) )
        {
            case '*':
                fMatch = true;
                fread_to_eol(fp);
                break;
                
            case 'C':
                KEY("CharAuto", ret->char_auto, fread_string( fp ) );
                KEY("CharFound", ret->char_found, fread_string( fp ) );
                KEY("CharNoArg", ret->char_no_arg, fread_string( fp ) );
                break;
                            
            case 'E':
                if ( !str_cmp(word, "End") )
                {
                    file_close(fp);
                    return ret;
                }
                break;
                
            case 'O':
                KEY("OtherAuto", ret->other_auto, fread_string( fp ) );
                KEY("OtherFound", ret->other_found, fread_string( fp ) );
                KEY("OtherNoArg", ret->other_no_arg, fread_string( fp ) );
                break;
                
            case 'V':
                KEY("VictFound", ret->vict_found, fread_string( fp ) );
                break;
        }

    }
    
    if ( !fMatch )
    {
        snprintf( log_buf, (2 * MIL), "Loading in social :%s, no match for ( %s ).", social, word );
        monitor_chan( log_buf, MONITOR_BAD );
        fread_to_eol( fp );
    }
    file_close(fp);
    
    return ret;
}

void cache_check_social( )
{
    SOCIAL_DATA *soc;
    list<SOCIAL_DATA *>::iterator li;
    
    for( li = social_list.begin(); li != social_list.end(); li++ )
    {
        soc = *li;
        if( --soc->cache_time <= 0 )
        {
            delete soc;
            li = social_list.erase(li);
        }
    }

    return;
}