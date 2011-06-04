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
    char search[MSL] = {'\0'};
    char tmp[MSL] = {'\0'};
    string str;
    
    if( !isalpha(*social) )
        return false;

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
    FILE *fp;
    char tmp[MSL] = {'\0'};
    char search[MSL] = {'0'};
    const char *word;
    bool fMatch = false;
    int spos, epos = 0;
    
    ret = new SOCIAL_DATA;
    spos = strlen(SOCIAL_DIR)+3;
    epos = spos + strlen(SOCIAL_EXT)+2;
    
    snprintf( search, MSL, "find %s%s/ -iname %s\\*.%s -printf '%%p'", SOCIAL_DIR, initial(social), social, SOCIAL_EXT );
    snprintf( tmp, MSL, "%s", _popen(search) );
    fp = file_open( tmp, "r" );
    
    snprintf( tmp, MSL, "ls -1 %s%s/%s*.%s | cut -c %d-%d", SOCIAL_DIR, initial(social), social, SOCIAL_EXT, spos, epos );
    ret->name = _popen(tmp);

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