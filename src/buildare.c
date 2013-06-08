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
/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
 ***************************************************************************/
#include "h/includes.h"
#include "h/list.h"

#ifndef DEC_AREASAVE_H
#include "h/areasave.h"
#endif

#ifndef DEC_BUILD_H
#include "h/build.h"
#endif

#ifndef DEC_BUILDARE_H
#include "h/buildare.h"
#endif

#ifndef DEC_BUILDTAB_H
#include "h/buildtab.h"
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

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

/* Variables declared in db.c, which we need */
extern char *string_hash[MAX_KEY_HASH];
extern char *string_space;
extern char *top_string;
extern char str_empty[1];
extern AREA_DATA *area_used[MAX_AREAS];
extern long int nAllocString;
extern long int sAllocString;
extern int nAllocPerm;
extern int sAllocPerm;
extern int fBootDb;

bool build_canread( AREA_DATA * Area, CHAR_DATA * ch, int showerror )
{
    if ( ch->gTrust() >= MAX_LEVEL - 1 )
        return true;

    if ( Area->can_read != NULL )
        if ( is_name( "all", Area->can_read )
                || is_name( ch->name.c_str(), Area->can_read ) || ( is_name( "gods", Area->can_read ) && IS_IMMORTAL( ch ) ) )
            return true;

    if ( showerror == AREA_SHOWERROR )
        send_to_char( "You are not allowed to use this area.\r\n", ch );

    return false;
}

bool build_canwrite( AREA_DATA * Area, CHAR_DATA * ch, int showerror )
{
    if ( ch->gTrust() >= MAX_LEVEL - 1 )
        return true;

    if ( Area->can_write != NULL )
        if ( is_name( "all", Area->can_write )
                || is_name( ch->name.c_str(), Area->can_write ) || ( is_name( "gods", Area->can_write ) && IS_IMMORTAL( ch ) ) )
            return true;

    if ( showerror == AREA_SHOWERROR )
        send_to_char( "You are not allowed to edit this area.\r\n", ch );

    return false;
}

void build_save_area_list( void )
{
    list<AREA_DATA *>::iterator li;
    AREA_DATA *pArea;
    FILE *fpArea;
    char buf[MSL];

    snprintf( buf, MSL, "%s.new", AREA_LIST );
    fpArea = file_open( buf, "w" );

    if ( fpArea == NULL )
    {
        bug( "Could not open new AREA_LIST for saving.", 0 );
        file_close( fpArea );
        return;
    }

    for ( li = area_list.begin(); li != area_list.end(); li++ )
    {
        pArea = *li;
        fprintf( fpArea, "%s\n", pArea->filename );
    }

    fprintf( fpArea, "$\r\n" );

    file_close( fpArea );

    /*
     * Save backup
     */
    snprintf( buf, MSL, "%s.old", AREA_LIST );
    rename( AREA_LIST, buf );
    snprintf( buf, MSL, "%s.new", AREA_LIST );
    rename( buf, AREA_LIST );
}

bool compare_vnum( const AREA_DATA *a1, const AREA_DATA *a2 ) { return a1->min_vnum < a2->min_vnum; }

DO_FUN(build_makearea)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *pRoomIndex;
    int vnum = 0;
    int svnum = 0;
    int envnum = 0;
    int mvnum = 0;
    int a = 0;
    int iHash;
    int rooms;

    list<AREA_DATA *>::iterator li;
    BUILD_DATA_LIST *pList;
    AREA_DATA *pArea = NULL;
    FILE *fpArea;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );


    strcpy( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        send_to_char( "\r\nSyntax: makearea filename numrooms [system]\r\n", ch );
        return;
    }
    rooms = atoi( arg2 );
    if ( rooms < 3 || rooms > 300 )
    {

        send_to_char( "rooms between 3 and 300..:)\r\n", ch );
        snprintf( buf, MSL, "%d\r\n", rooms );
        send_to_char( buf, ch );
        return;

    }


    for ( li = area_list.begin(); li != area_list.end(); li++ )
    {
        pArea = *li;
        svnum = pArea->min_vnum;

        a = svnum - envnum - 1;
        /*  snprintf(buf,MSL,"dif %d for rooms %d, %d-%d\r\n",a,rooms,pArea->min_vnum,pArea->max_vnum);
            send_to_char(buf,ch);
                snprintf(buf,MSL,"%s\r\n",pArea->name);
                send_to_char(buf,ch); */
        if ( str_cmp( arg3, "system" ) ? ( svnum > MAX_SYS_VNUM && envnum > MAX_SYS_VNUM ) && ( rooms <= a && buf != NULL ) : ( rooms <= a && buf != NULL ) )
        {
            send_to_char( "found one!\r\n", ch );
            vnum = envnum + 1;
            mvnum = vnum + rooms - 1;
            snprintf( buf, MSL, "\r\n Your start vnum :%d and ending vnum: %d :\r\n", vnum, mvnum );
            send_to_char( buf, ch );
            break;
        }

        envnum = pArea->max_vnum;
    }


    if ( vnum == mvnum )
    {
        send_to_char( "Can't create a room with 0 rooms.\r\n", ch );
        return;
    }

    if ( get_room_index( vnum ) != NULL )
    {
        send_to_char( "There is already a room with that vnum.\r\n", ch );
        return;
    }

    strncat(arg1, ".are", MIL); /* Add file extension */

    snprintf( buf, MSL, "%s%s", AREA_DIR, arg1 );

    fpArea = file_open( buf, "r" );
    if ( fpArea != NULL )
    {
        send_to_char( "There is already a file with that name.\r\n", ch );
        file_close( fpArea );
        return;
    }

    fpArea = file_open( buf, "w" );
    if ( fpArea == NULL )
    {
        send_to_char( "Invalid filename, would not be able to save.\r\n", ch );
        return;
    }
    file_close( fpArea );

    /*
     * Find a unique area number
     */
    for ( a = 0; a < MAX_AREAS; a++ )
        if ( area_used[a] == NULL )
            break;
    if ( a == MAX_AREAS )
    {
        send_to_char( "Maximum number of areas already.\r\n", ch );
        return;
    }


    pArea = new AREA_DATA;
    pArea->area_num = a;
    pArea->modified = true;
    pArea->filename = str_dup( arg1 );
    pArea->min_vnum = vnum;
    pArea->max_vnum = mvnum;
    pArea->owner = str_dup( ch->name.c_str() );
    pArea->can_read = str_dup( ch->name.c_str() );
    pArea->can_write = str_dup( ch->name.c_str() );
    pArea->flags.set(AFLAG_NO_SHOW);   /* don't list on 'areas' -S- */

    area_used[pArea->area_num] = pArea;

    area_list.sort(compare_vnum);

    /*
     * Now add it to area.lst
     */
    build_save_area_list(  );

    /*
     * Now add room
     */
    pRoomIndex = new ROOM_INDEX_DATA;
    pRoomIndex->area = pArea;
    pRoomIndex->vnum = vnum;
    pRoomIndex->sector_type = SECT_INSIDE;

    /*
     * Add room to hash table
     */


    iHash = vnum % MAX_KEY_HASH;
    SING_TOPLINK( pRoomIndex, room_index_hash[iHash], next );

    /*
     * Add room into area list.
     */
    pList = new BUILD_DATA_LIST;
    pList->data = pRoomIndex;
    LINK( pList, pArea->first_area_room, pArea->last_area_room, next, prev );

    send_to_char( "Ok.\r\n", ch );
}

DO_FUN(build_setarea)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char name[MAX_INPUT_LENGTH];
    char buffer[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *argn, *oldperm;
    AREA_DATA *pArea;
    int num;

    smash_tilde( argument );
    argument = one_argument( argument, arg1 );

    if ( ( str_cmp( arg1, "title" ) ) && ( str_cmp( arg1, "level_label" ) ) && ( str_cmp( arg1, "message" ) ) )
        argument = one_argument( argument, arg2 );
    else
        strcpy( arg2, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
        ch->send( "Syntax: setarea <arguments> \r\n\r\n" );
        ch->send( "Arguments being one of:\r\n" );
        ch->send( "      owner       <name>\r\n" );
        ch->send( "      read        [-]<name>\r\n" );
        ch->send( "      write       [-]<name>\r\n" );
        ch->send( "      title       <string>\r\n" );
        ch->send( "      min         <min_vnum>\r\n" );
        ch->send( "      max         <max_vnum>\r\n" );
        ch->send( "      min_level   <level>\r\n" );
        ch->send( "      max_level   <level>\r\n" );
        ch->send( "      level_label <label>\r\n" );
        ch->send( "      keyword     <keyword>\r\n" );
        ch->send( "      repop_rate  ticks( lower is faster )\r\n" );
        ch->send( "      message     <message/off> \r\n" );
        ch->send( "      flag        <name>\r\n" );
        return;
    }

    pArea = ch->in_room->area;

    area_modified( pArea );

    /*
     * Set something.
     */
    if ( !str_cmp( arg1, "read" ) )
    {
        num = 1;
        argn = arg2;
        if ( argn[0] == '-' )
        {
            num = 0;
            argn++;
        }

        if ( num == 1 )
        {
            if ( pArea->can_read == NULL )
            {
                pArea->can_read = str_dup( argn );
                return;
            }
            if ( !is_name( argn, pArea->can_read ) )
            {
                snprintf( buffer, MSL, "%s %s", pArea->can_read, argn );
                free_string( pArea->can_read );
                pArea->can_read = str_dup( buffer );
            }
        }
        else
        {

            if ( is_name( argn, pArea->can_read ) )
            {
                buffer[0] = '\0';

                strcpy( buf2, pArea->can_read );
                oldperm = buf2;

                oldperm = one_argument( oldperm, name );
                while ( name[0] != '\0' )
                {
                    if ( str_cmp( name, argn ) )  /* i.e. not the same */
                    {
                        strncat( buffer, name, MIL );
                        strncat( buffer, " ", MIL );
                    }
                    oldperm = one_argument( oldperm, name );
                }
                free_string( pArea->can_read );
                pArea->can_read = str_dup( buffer );
            }
        }
        return;
    }

    if ( !str_cmp( arg1, "write" ) )
    {
        num = 1;
        argn = arg2;
        if ( argn[0] == '-' )
        {
            num = 0;
            argn++;
        }

        if ( num == 1 )
        {
            if ( pArea->can_write == NULL )
            {
                pArea->can_write = str_dup( argn );
                return;
            }
            if ( !is_name( argn, pArea->can_write ) )
            {
                snprintf( buffer, MSL, "%s %s", pArea->can_write, argn );
                free_string( pArea->can_write );
                pArea->can_write = str_dup( buffer );
            }
        }
        else if ( is_name( argn, pArea->can_write ) )
        {
            buffer[0] = '\0';

            strcpy( buf2, pArea->can_write );
            oldperm = buf2;

            oldperm = one_argument( oldperm, name );
            while ( name[0] != '\0' )
            {
                if ( str_cmp( name, argn ) )  /* i.e. not the same */
                {
                    strncat( buffer, name, MIL );
                    strncat( buffer, " ", MIL );
                }
                oldperm = one_argument( oldperm, name );
            }
            free_string( pArea->can_write );
            pArea->can_write = str_dup( buffer );
        }
        return;
    }

    if ( !str_cmp( arg1, "owner" ) )
    {
        build_strdup( &pArea->owner, arg2, TRUE, FALSE, ch );
        return;
    }

    if ( !str_cmp( arg1, "title" ) )
    {
        build_strdup( &pArea->name, argument, TRUE, FALSE, ch );
        return;
    }
    if ( !str_cmp( arg1, "level_label" ) )
    {
        build_strdup( &pArea->level_label, argument, TRUE, FALSE, ch );
        return;
    }
    if ( !str_cmp( arg1, "keyword" ) )
    {
        build_strdup( &pArea->keyword, arg2, TRUE, FALSE, ch );
        return;
    }
    if ( !str_cmp( arg1, "message" ) )
    {
        build_strdup( &pArea->reset_msg, arg2, TRUE, FALSE, ch );
        return;
    }

    if ( !str_cmp( arg1, "flag" ) )
    {
        num = table_lookup( tab_area_flags, arg2 );

        if( num == 0 )
        {
            snprintf( buffer, MSL, "Values for area flags are:\r\n" );
            table_printout( tab_area_flags, buffer + strlen( buffer ) );
            send_to_char( buffer, ch );
            return;
        }

        pArea->flags.flip(num);
        return;
    }

    if ( !str_cmp( arg1, "repop_rate" ) )
    {
        if ( is_number( arg2 ) )
        {
            pArea->reset_rate = atoi( arg2 );
            return;
        }
    }

    if ( !str_cmp( arg1, "min" ) )
    {
        if ( is_number( arg2 ) )
        {
            pArea->min_vnum = atoi( arg2 );
            return;
        }
    }
    if ( !str_cmp( arg1, "min_level" ) )
    {
        if ( is_number( arg2 ) )
        {
            pArea->min_level = atoi( arg2 );
            return;
        }
    }
    if ( !str_cmp( arg1, "max_level" ) )
    {
        if ( is_number( arg2 ) )
        {
            pArea->max_level = atoi( arg2 );
            return;
        }
    }

    if ( !str_cmp( arg1, "max" ) )
    {
        if ( is_number( arg2 ) )
        {
            pArea->max_vnum = atoi( arg2 );
            return;
        }
    }
    /*
     * Generate usage message.
     */
    build_setarea( ch, "" );
    return;
}

DO_FUN(build_findarea)
{
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    AREA_DATA *pArea;
    list<AREA_DATA *>::iterator li;
    ROOM_INDEX_DATA *pRoomIndex = NULL;
    int nMatch;
    bool fAll;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
        send_to_char( "Find what area?\r\n", ch );
        return;
    }


    buf1[0] = '\0';
    fAll = !str_cmp( arg, "all" );
    found = FALSE;
    nMatch = 0;

    for ( li = area_list.begin(); li != area_list.end(); li++ )
    {
        pArea = *li;
        nMatch++;
        if ( ( fAll || is_name( arg, pArea->name ) ) && build_canread( pArea, ch, 0 ) )
        {
            found = TRUE;
            if ( pArea->first_area_room != NULL )
                pRoomIndex = (ROOM_INDEX_DATA *)pArea->first_area_room->data;
            snprintf( buf, MSL, "[%5d] %s\r\n", pArea->first_area_room != NULL ? pRoomIndex->vnum : 0, pArea->name );
            strncat( buf1, buf, MSL - 1 );
        }
    }

    if ( !found )
    {
        send_to_char( "No area like that.\r\n", ch );
        return;
    }

    send_to_char( buf1, ch );
    return;
}


DO_FUN(build_showarea)
{
    AREA_DATA *pArea;
    char buf[MAX_STRING_LENGTH];
    char buffer[MAX_STRING_LENGTH];

    pArea = ch->in_room->area;

    if ( !build_canread( pArea, ch, 1 ) )
        return;

    buf[0] = '\0';

    snprintf( buffer, MSL, "\r\nTitle: %s\r\n", pArea->name );
    strncat( buf, buffer, MSL - 1 );
    snprintf( buffer, MSL, "Keyword: %s\r\n", pArea->keyword );
    strncat( buf, buffer, MSL - 1 );
    snprintf( buffer, MSL, "Level Label: %s\r\n", pArea->level_label );
    strncat( buf, buffer, MSL - 1 );
    snprintf( buffer, MSL, "Repop Rate: %i\r\n", pArea->reset_rate );
    strncat( buf, buffer, MSL - 1 );
    snprintf( buffer, MSL, "Reset Message: %s\r\n", pArea->reset_msg );
    strncat( buf, buffer, MSL - 1 );

    if ( ch->gTrust() >= MAX_LEVEL - 1 )
    {
        snprintf( buffer, MSL, "Filename: %s\r\n", pArea->filename );
        strncat( buf, buffer, MSL - 1 );
    }

    snprintf( buffer, MSL, "Owner: %s\r\nCan Read: %s\r\nCan Write: %s\r\n", pArea->owner, pArea->can_read, pArea->can_write );
    strncat( buf, buffer, MSL - 1 );

    snprintf( buffer, MSL, "Min Vnum: %5d    Max Vnum: %5d\r\n", pArea->min_vnum, pArea->max_vnum );
    strncat( buf, buffer, MSL - 1 );
    snprintf( buffer, MSL, "Min Level: %5d    Max Level: %5d \r\n", pArea->min_level, pArea->max_level );
    strncat( buf, buffer, MSL - 1 );

    if ( pArea->flags.test(AFLAG_PAYAREA) )
        strncat( buf, "This is a pay area.\r\n", MSL - 1 );
    if ( pArea->flags.test(AFLAG_NO_TELEPORT) )
        strncat( buf, "You cannot teleport into here.\r\n", MSL - 1 );
    if ( pArea->flags.test(AFLAG_BUILDING) )
        strncat( buf, "Area currently being built.\r\n", MSL - 1 );
    if ( pArea->flags.test(AFLAG_NO_SHOW) )
        strncat( buf, "Area title will not be shown on area list.\r\n", MSL - 1 );
    else
        strncat( buf, "Area title will show on area list.\r\n", MSL - 1 );
    if ( pArea->flags.test(AFLAG_NO_ROOM_AFF) )
        strncat( buf, "Bad Room Affect spells are not allowed.\r\n", MSL - 1 );
    else
        strncat( buf, "Bad Room Affect spells may be used.\r\n", MSL - 1 );

    send_to_char( buf, ch );
    return;
}

DO_FUN(build_arealist)
{
    /*
     * -S- : Show list of areas, vnum range and owners.
     */
    char buf[MAX_STRING_LENGTH];
    char msg[MAX_STRING_LENGTH];
    AREA_DATA *pArea;
    list<AREA_DATA *>::iterator li;
    short stop_counter = 0;

    buf[0] = '\0';
    snprintf( msg, MSL, "%s", "Areas of " mudnamecolor ":\r\n" );
    for ( li = area_list.begin(); li != area_list.end(); li++ )
    {
        pArea = *li;
        snprintf( buf, MSL, "%12s [%8d to %8d] %s\r\n", capitalize( pArea->owner ), pArea->min_vnum, pArea->max_vnum, pArea->name );
        stop_counter++;
        if ( stop_counter > 40 )
        {
            strncat( msg, buf, MSL - 1 );
            stop_counter = 0;
            send_to_char( msg, ch );
            msg[0] = '\0';
        }
        else
        {
            strncat( msg, buf, MSL - 1 );
        }
    }

    send_to_char( msg, ch );
    return;
}
