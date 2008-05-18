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
 *    _/_/_/_/      _/          _/  _/             Version #: 4.3.1        *
 *   _/      _/      _/_/_/     _/    _/     _/                            *
 *                                                                         *
 *                        http://ackmud.nuc.net/                           *
 *                        zenithar@ackmud.nuc.net                          *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/
/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#include <sys/types.h>
#include <sys/time.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#if defined(__CYGWIN__)
#include <crypt.h>
#endif
/* For child forking and stuff */
#include <sys/wait.h>
#include <unistd.h>  /* for execl */
#include "globals.h"
#include "cursor.h"

/*
 * Socket and TCP/IP stuff.
 */
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
const char echo_off_str[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const char echo_on_str[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const char go_ahead_str[] = { IAC, GA, '\0' };

void copyover_recover args( ( void ) );

/*
 * Global variables.
 */
DESCRIPTOR_DATA *d_next;   /* Next descriptor in loop      */
FILE *fpReserve;  /* Reserved file handle         */
bool god;   /* All new chars are gods!      */
bool merc_down;   /* Shutdown                     */
bool wizlock;  /* Game is wizlocked            */
bool deathmatch;  /* Deathmatch happening?        */
char str_boot_time[MAX_INPUT_LENGTH];
time_t current_time; /* Time of this pulse           */

/* port and control moved from local to global for HOTreboot - Flar */
int port;
int control;

/* -S- Mod: Some Globals for auctioning an item */
OBJ_DATA *auction_item; /* Item being sold      */
CHAR_DATA *auction_owner;  /* Item's owner         */
CHAR_DATA *auction_bidder; /* Last bidder for item    */
int *auction_bid; /* Latest price offered    */
int *auction_reserve;   /* Reserve Price     */
int *auction_stage;  /* start, 1st, 2nd, gone   */
bool auction_flop;   /* Update called externally?    */

/* -S- Mod: Globals to handle questing */
bool quest; /* Is there a quest running?    */
bool auto_quest;  /* Quests start on their own?   */
CHAR_DATA *quest_mob;   /* Mob which started quest      */
CHAR_DATA *quest_target;   /* Target of the quest     */
OBJ_DATA *quest_object; /* Object to recover    */
int quest_timer;  /* Time left to get object */
int quest_wait = 0;  /* Min time until next quest  */
sh_int quest_personality;  /* mob's crusade personality :) */

/* Zen mod: Diplomatics globals */

POL_DATA politics_data;

COUNCIL_DATA super_councils[MAX_SUPER];   /* for supernatural council meetings  */

/* Llolth added for consider */
char hr[MAX_STRING_LENGTH];
char dr[MAX_STRING_LENGTH];

/*
 * OS-dependent local functions.
 */
void game_loop( int control );
int init_socket( int port );
void new_descriptor( int control );
bool read_from_descriptor( DESCRIPTOR_DATA * d );
bool write_to_descriptor( int desc, char *txt, int length );
void init_descriptor( DESCRIPTOR_DATA * dnew, int desc );

/*
 * Other local functions (OS-independent).
 */
bool check_parse_name args( ( char *name ) );
bool check_reconnect args( ( DESCRIPTOR_DATA * d, char *name, bool fConn ) );
bool check_playing args( ( DESCRIPTOR_DATA * d, char *name ) );
int main args( ( int argc, char **argv ) );
void nanny args( ( DESCRIPTOR_DATA * d, char *argument ) );
bool process_output args( ( DESCRIPTOR_DATA * d, bool fPrompt ) );
void read_from_buffer args( ( DESCRIPTOR_DATA * d ) );
void stop_idling args( ( CHAR_DATA * ch ) );
void bust_a_prompt args( ( DESCRIPTOR_DATA * d ) );
void free_desc args( ( DESCRIPTOR_DATA * d ) );

int global_port;

int main( int argc, char **argv )
{
   struct timeval now_time;
   bool fCopyOver = FALSE; /* HOTreboot??? Well is it...is it???? - Flar */
   extern int abort_threshold;
#ifdef IMC
   int imcsocket = -1;
#endif

   /*
    * Init time.
    */
   gettimeofday( &now_time, NULL );
   current_time = ( time_t ) now_time.tv_sec;
   strcpy( str_boot_time, ctime( &current_time ) );

   /*
    * Reserve one channel for our use.
    */
   if( ( fpReserve = fopen( NULL_FILE, "r" ) ) == NULL )
   {
      perror( NULL_FILE );
      exit( 1 );
   }

   /*
    * Get the port number.
    */
   port = 1234;
   if( argc > 1 )
   {
      if( !is_number( argv[1] ) )
      {
         fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
         exit( 1 );
      }
      else if( ( port = atoi( argv[1] ) ) <= 1024 )
      {
         fprintf( stderr, "Port number must be above 1024.\n" );
         exit( 1 );
      }
   }
/* Check for HOTreboot parameter - Flar */
   if( argv[2] && argv[2][0] )
   {
      fCopyOver = TRUE;
      control = atoi( argv[3] );
#ifdef IMC
      imcsocket = atoi( argv[4] );
#endif
   }

   else
      fCopyOver = FALSE;

   rename( "../log/comlog.old", "../log/comlog.crash" );
   rename( "../log/comlog.txt", "../log/comlog.old" );

   /*
    * Run the game.
    */
   if( !fCopyOver )  /* We already have the port if Copyovered. */
   {
      control = init_socket( port );
   }
   global_port = port;
   if( fCopyOver )
      abort_threshold = BOOT_DB_ABORT_THRESHOLD;
   boot_db(  );
#ifndef WIN32
   init_alarm_handler(  );
#endif
   sprintf( log_buf, "ACK! MUD is ready on port %d.", port );
   log_string( log_buf );
#ifdef IMC
   imc_startup( FALSE, imcsocket, fCopyOver );
#endif
   if( fCopyOver )
   {
      extern bool disable_timer_abort;
      disable_timer_abort = TRUE;
      copyover_recover(  );
      disable_timer_abort = FALSE;
   }
   game_loop( control );
   close( control );
#ifdef IMC
   imc_shutdown( FALSE );
#endif

   /*
    * That's all, folks.
    */
   log_string( "Normal termination of game." );
   exit( 0 );
   return 0;
}

int init_socket( int port )
{
   static struct sockaddr_in sa_zero;
   struct sockaddr_in sa;
   int x = 1;
   int fd;

   if( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
   {
      perror( "Init_socket: socket" );
      exit( 1 );
   }

   if( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, ( char * )&x, sizeof( x ) ) < 0 )
   {
      perror( "Init_socket: SO_REUSEADDR" );
      close( fd );
      exit( 1 );
   }

#if defined(SO_DONTLINGER) && !defined(SYSV)
   {
      struct linger ld;

      ld.l_onoff = 1;
      ld.l_linger = 1000;

      if( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER, ( char * )&ld, sizeof( ld ) ) < 0 )
      {
         perror( "Init_socket: SO_DONTLINGER" );
         close( fd );
         exit( 1 );
      }
   }
#endif
   sa = sa_zero;
   sa.sin_family = AF_INET;
   sa.sin_port = htons( port );

   if( bind( fd, ( struct sockaddr * )&sa, sizeof( sa ) ) < 0 )
   {
      perror( "Init_socket: bind" );
      close( fd );
      exit( 1 );
   }

   if( listen( fd, 3 ) < 0 )
   {
      perror( "Init_socket: listen" );
      close( fd );
      exit( 1 );
   }

   return fd;
}

int cur_hour = 0;
int max_players = 0;
int cur_players = 0;

/* + */
int reopen_flag;
void reopen_socket( int sig )
{
   reopen_flag = 1;
   signal( SIGUSR1, reopen_socket );
}

/* + */

void game_loop( int control )
{
   static struct timeval null_time;
   struct timeval last_time;

   signal( SIGPIPE, SIG_IGN );
/*+*/
   /*
    * On a SIGUSR1, open and close the control socket (anti-port-locking
    * *  thing) -- Spectrum
    */
   reopen_flag = 0;
   signal( SIGUSR1, reopen_socket );

/*+*/

   gettimeofday( &last_time, NULL );
   current_time = ( time_t ) last_time.tv_sec;

   /*
    * Main loop 
    */
   while( !merc_down )
   {
      fd_set in_set;
      fd_set out_set;
      fd_set exc_set;
      DESCRIPTOR_DATA *d;
      int maxdesc;

      /*
       * handle reopening the control socket
       * * don't worry about locking here, we assume that SIGUSR1's are
       * * relatively rare
       */
      if( reopen_flag )
      {
         log_string( "SIGUSR1 received, reopening control socket" );
         close( control );
         control = init_socket( global_port );
         reopen_flag = 0;
      }

      /*
       * Poll all active descriptors.
       */
      FD_ZERO( &in_set );
      FD_ZERO( &out_set );
      FD_ZERO( &exc_set );
      FD_SET( control, &in_set );
      maxdesc = control;

      for( d = first_desc; d; d = d->next )
      {
         if( ( d->flags && DESC_FLAG_PASSTHROUGH ) == 0 )
         {
            maxdesc = UMAX( maxdesc, d->descriptor );
            FD_SET( d->descriptor, &in_set );
            FD_SET( d->descriptor, &out_set );
            FD_SET( d->descriptor, &exc_set );
         }
         else
         {
            /*
             * Check to see if child process has terminated 
             */
            if( waitpid( d->childpid, NULL, WNOHANG ) != 0 )
            {
               /*
                * Terminated or error 
                */
               d->childpid = 0;
               REMOVE_BIT( d->flags, DESC_FLAG_PASSTHROUGH );
            }
         }
      }

      if( select( maxdesc + 1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
      {
         perror( "Game_loop: select: poll" );
         exit( 1 );
      }

      /*
       * New connection?
       */
      if( FD_ISSET( control, &in_set ) )
         new_descriptor( control );

      /*
       * Kick out the freaky folks.
       */
      for( d = first_desc; d != NULL; d = d_next )
      {
         d_next = d->next;
         if( FD_ISSET( d->descriptor, &exc_set ) )
         {
            FD_CLR( d->descriptor, &in_set );
            FD_CLR( d->descriptor, &out_set );
            if( d->character )
               save_char_obj( d->character );
            d->outtop = 0;
            close_socket( d );
         }
      }

      /*
       * Process input.
       */
      for( d = first_desc; d != NULL; d = d_next )
      {
         d_next = d->next;
         d->fcommand = FALSE;

         if( FD_ISSET( d->descriptor, &in_set ) )
         {
            if( d->character != NULL )
               d->character->timer = 0;
            if( !read_from_descriptor( d ) )
            {
               FD_CLR( d->descriptor, &out_set );
               if( d->character != NULL )
                  save_char_obj( d->character );
               d->outtop = 0;
               close_socket( d );
               continue;
            }
         }

         if( d->character != NULL && d->character->wait > 0 )
         {
            --d->character->wait;
            continue;
         }

         read_from_buffer( d );
         if( d->incomm[0] != '\0' )
         {
            d->fcommand = TRUE;
            stop_idling( d->character );
            d->timeout = current_time + 180; /* spec: stop idling */

            if( d->connected == CON_PLAYING )
               if( d->showstr_point )
                  show_string( d, d->incomm );
               else
                  interpret( d->character, d->incomm );
            else
               nanny( d, d->incomm );

            d->incomm[0] = '\0';
         }
      }

#ifdef IMC
      imc_loop(  );
#endif

      /*
       * Autonomous game motion.
       */
      update_handler(  );
      waitpid( 0, 0, WNOHANG );

      /*
       * Output.
       */
      for( d = first_desc; d != NULL; d = d_next )
      {
         d_next = d->next;

         /*
          * spec: disconnect people idling on login 
          */
         if( d->connected < 0 && d->timeout < current_time )
         {
            write_to_descriptor( d->descriptor, "Login timeout (180s)\n\r", 0 );
            close_socket( d );
            continue;
         }

         if( ( d->fcommand || d->outtop > 0 ) && FD_ISSET( d->descriptor, &out_set ) )
         {
            if( !process_output( d, TRUE ) )
            {
               if( d->character != NULL )
                  save_char_obj( d->character );
               d->outtop = 0;
               close_socket( d );
            }
         }
      }

      /*
       * Synchronize to a clock.
       * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
       * Careful here of signed versus unsigned arithmetic.
       */
      {
         struct timeval now_time;
         struct tm *now_bd_time;
         FILE *out_file;
         long secDelta;
         long usecDelta;

         long temp_time;

         gettimeofday( &now_time, NULL );
         temp_time = ( time_t ) now_time.tv_sec;
         now_bd_time = localtime( &temp_time );

         if( now_bd_time->tm_hour != cur_hour )
         {
            cur_hour = now_bd_time->tm_hour;
            out_file = fopen( "players.num", "a" );
            fprintf( out_file, "%i %i %i\n", now_bd_time->tm_mday, cur_hour, max_players );
            fclose( out_file );
            max_players = cur_players;
         }

         usecDelta = ( ( int )last_time.tv_usec ) - ( ( int )now_time.tv_usec ) + 1000000 / PULSE_PER_SECOND;
         secDelta = ( ( int )last_time.tv_sec ) - ( ( int )now_time.tv_sec );
         while( usecDelta < 0 )
         {
            usecDelta += 1000000;
            secDelta -= 1;
         }

         while( usecDelta >= 1000000 )
         {
            usecDelta -= 1000000;
            secDelta += 1;
         }

         if( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
         {
            struct timeval stall_time;

            stall_time.tv_usec = usecDelta;
            stall_time.tv_sec = secDelta;
            if( select( 0, NULL, NULL, NULL, &stall_time ) < 0 && errno != EINTR )
            {
               perror( "Game_loop: select: stall" );
               exit( 1 );
            }
         }
      }

      gettimeofday( &last_time, NULL );
      current_time = ( time_t ) last_time.tv_sec;
   }

   return;
}

void free_desc( DESCRIPTOR_DATA * d )
{
   DESCRIPTOR_DATA *sd;

   d->snoop_by = NULL;
   for( sd = first_desc; sd; sd = sd->next )
      if( sd->snoop_by == d )
         sd->snoop_by = NULL;
   if( d->original )
      do_return( d->character, "" );
   if( d->character )
      free_char( d->character );
   free_string( d->host );
   close( d->descriptor );
   if( d->showstr_head )
      qdispose( d->showstr_head, strlen( d->showstr_head ) + 1 );
   if( d->outbuf )
      dispose( d->outbuf, d->outsize );
}

void new_descriptor( int control )
{
   static DESCRIPTOR_DATA d_zero;
   char buf[MSL];
   DESCRIPTOR_DATA *dnew;
   /*
    * BAN_DATA *pban;   
    */
   struct sockaddr_in sock;
   size_t desc, size;

   size = sizeof( sock );
   getsockname( control, ( struct sockaddr * )&sock, &size );
   if( ( desc = accept( control, ( struct sockaddr * )&sock, &size ) ) < 0 )
   {
      perror( "New_descriptor: accept" );
      return;
   }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

   if( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
   {
      perror( "New_descriptor: fcntl: FNDELAY" );
      return;
   }

   /*
    * Cons a new descriptor.
    */
   GET_FREE( dnew, desc_free );
   *dnew = d_zero;
   init_descriptor( dnew, desc );   /* Not sure is this right? */
   dnew->descriptor = desc;
   dnew->connected = CON_GET_NAME;
   dnew->showstr_head = NULL;
   dnew->showstr_point = NULL;
   dnew->outsize = 2000;
   dnew->flags = 0;
   dnew->childpid = 0;

   size = sizeof( sock );
   if( getpeername( desc, ( struct sockaddr * )&sock, &size ) < 0 )
   {
      perror( "New_descriptor: getpeername" );
      dnew->host = str_dup( "(unknown)" );
   }
   else
   {
      /*
       * Would be nice to use inet_ntoa here but it takes a struct arg,
       * which ain't very compatible between gcc and system libraries.
       */
      int addr;

      addr = ntohl( sock.sin_addr.s_addr );
      sprintf( buf, "%d.%d.%d.%d", ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF, ( addr >> 8 ) & 0xFF, ( addr ) & 0xFF );
      sprintf( log_buf, "Sock.sinaddr:  %s (%d)", buf, ntohs( sock.sin_port ) );
      log_string( log_buf );
      sprintf( log_buf, "Connection formed from %s.", buf );
      monitor_chan( log_buf, MONITOR_CONNECT );

      dnew->remote_port = ntohs( sock.sin_port );

      /*
       * From unused to prevent possible ns lockup 
       * from = gethostbyaddr( (char*) &sock.sin_addr,
       * sizeof(sock.sin_addr), AF_INET ); 
       */

      dnew->host = str_dup( buf );
   }

   /*
    * Swiftest: I added the following to ban sites.  I don't
    * endorse banning of sites, but Copper has few descriptors now
    * and some people from certain sites keep abusing access by
    * using automated 'autodialers' and leaving connections hanging.
    *
    * Furey: added suffix check by request of Nickel of HiddenWorlds.
    *
    * Stephen: As we use IP address now, want to use prefix check,
    * so we can ban whole domains....
    */
   /*
    * MOVED TO LOWER FOR NEWBIEBAN     
    * 
    * for ( pban = first_ban; pban != NULL; pban = pban->next )
    * {
    * if ( !str_prefix( pban->name, dnew->host ) && !( pban->newbie ) )
    * {
    * char buf[MAX_STRING_LENGTH];
    * sprintf( buf, "Denying access to banned site %s", dnew->host );
    * monitor_chan( buf, MONITOR_CONNECT );
    * write_to_descriptor( desc,
    * "Your site has been banned from this Mud.  BYE BYE!\n\r", 0 );
    * free_desc(dnew);
    * PUT_FREE(dnew, desc_free);
    * return;
    * }
    * }
    * 
    * 
    */
   /*
    * Init descriptor data.
    */
   LINK( dnew, first_desc, last_desc, next, prev );

   /*
    * spec: set initial login timeout 
    */
   dnew->timeout = current_time + 180;

   /*
    * Send the greeting.
    */
   {
      char buf[MAX_STRING_LENGTH];
      HELP_DATA *pHelp;
      extern HELP_DATA *first_help;

      sprintf( buf, "greeting%d", 0 /* number_range( 0, 4 ) */  );

      for( pHelp = first_help; pHelp != NULL; pHelp = pHelp->next )
         if( !str_cmp( pHelp->keyword, buf ) )
         {
            if( pHelp->text[0] == '.' )
               write_to_buffer( dnew, pHelp->text + 1, 0 );
            else
               write_to_buffer( dnew, pHelp->text, 0 );
            break;   /* so no more found through multiple copies */
         }
   }

   cur_players++;
   if( cur_players > max_players )
      max_players = cur_players;

   return;
}

void init_descriptor( DESCRIPTOR_DATA * dnew, int desc )
{
   static DESCRIPTOR_DATA d_zero;
   *dnew = d_zero;
   dnew->descriptor = desc;
   dnew->connected = CON_GET_NAME;
   dnew->showstr_head = NULL;
   dnew->showstr_point = NULL;
   dnew->outsize = 2000;
   dnew->outbuf = getmem( dnew->outsize );
   dnew->flags = 0;
   dnew->childpid = 0;

}

void close_socket( DESCRIPTOR_DATA * dclose )
{
   CHAR_DATA *ch;

   if( dclose->outtop > 0 )
      process_output( dclose, FALSE );

   if( dclose->snoop_by != NULL )
   {
      write_to_buffer( dclose->snoop_by, "Your victim has left the game.\n\r", 0 );
   }

   {
      DESCRIPTOR_DATA *d;

      for( d = first_desc; d != NULL; d = d->next )
      {
         if( d->snoop_by == dclose )
            d->snoop_by = NULL;
      }
   }

   if( dclose->original )
   {
      if( dclose->character )
         do_return( dclose->character, "" );
      else
      {
         bug( "Close_socket: original without ch", 0 );
         dclose->character = dclose->original;
         dclose->original = NULL;
      }
   }

   if( ( ch = dclose->character ) != NULL )
   {
      sprintf( log_buf, "Closing link to %s.", ch->name );
      log_string( log_buf );
      monitor_chan( log_buf, MONITOR_CONNECT );
      if( dclose->connected == CON_PLAYING )
      {
         act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM );
         ch->desc = NULL;
      }
      else
      {
         free_char( dclose->character );
      }
/*      stop_fighting( ch, TRUE );
      save_char_obj( ch );
      extract_char( ch, TRUE );  */
   }

   if( d_next == dclose )
      d_next = d_next->next;

   UNLINK( dclose, first_desc, last_desc, next, prev );
   close( dclose->descriptor );
   free_string( dclose->host );
   if( dclose->outbuf )
      dispose( dclose->outbuf, dclose->outsize );
   if( dclose->showstr_head )
      qdispose( dclose->showstr_head, strlen( dclose->showstr_head ) + 1 );
   PUT_FREE( dclose, desc_free );

   cur_players--;
   return;
}

bool read_from_descriptor( DESCRIPTOR_DATA * d )
{
   int iStart;

   /*
    * Hold horses if pending command already. 
    */
   if( d->incomm[0] != '\0' )
      return TRUE;

   /*
    * Check for overflow. 
    */
   iStart = strlen( d->inbuf );
   if( iStart >= sizeof( d->inbuf ) - 10 )
   {
      sprintf( log_buf, "%s input overflow!", d->host );
      log_string( log_buf );
      sprintf( log_buf, "input overflow by %s (%s)", ( d->character == NULL ) ? "[login]" : d->character->name, d->host );
      monitor_chan( log_buf, MONITOR_CONNECT );
      write_to_descriptor( d->descriptor, "\n\r SPAMMING IS RUDE, BYE BYE! \n\r", 0 );
      return FALSE;
   }

   /*
    * Snarf input. 
    */
   for( ;; )
   {
      int nRead;

      nRead = read( d->descriptor, d->inbuf + iStart, sizeof( d->inbuf ) - 10 - iStart );
      if( nRead > 0 )
      {
         iStart += nRead;
         if( d->inbuf[iStart - 1] == '\n' || d->inbuf[iStart - 1] == '\r' )
            break;
      }
      else if( nRead == 0 )
      {
         log_string( "EOF encountered on read." );
         return FALSE;
      }
      else if( errno == EWOULDBLOCK )
         break;
      else
      {
         perror( "Read_from_descriptor" );
         return FALSE;
      }
   }
   d->inbuf[iStart] = '\0';
   return TRUE;
}

/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA * d )
{
   int i, j, k;

   /*
    * Hold horses if pending command already.
    */
   if( d->incomm[0] != '\0' )
      return;

   /*
    * Look for at least one new line.
    */
   for( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
   {
      if( d->inbuf[i] == '\0' )
         return;
   }

   /*
    * Canonical input processing.
    */
   for( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
   {
      if( k >= MAX_INPUT_LENGTH - 2 )
      {
         write_to_descriptor( d->descriptor, "Line too long.\n\r", 0 );

         /*
          * skip the rest of the line 
          */
         for( ; d->inbuf[i] != '\0'; i++ )
         {
            if( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
               break;
         }
         d->inbuf[i] = '\n';
         d->inbuf[i + 1] = '\0';
         break;
      }

      if( d->inbuf[i] == '\b' && k > 0 )
         --k;
      else if( isascii( d->inbuf[i] ) && isprint( d->inbuf[i] ) )
         d->incomm[k++] = d->inbuf[i];
   }

   /*
    * Finish off the line.
    */
   if( k == 0 )
      d->incomm[k++] = ' ';
   d->incomm[k] = '\0';

   /*
    * Deal with bozos with #repeat 1000 ...
    */
   if( k > 1 || d->incomm[0] == '!' )
   {
      if( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
      {
         d->repeat = 0;
      }
      else
      {
         if( ++d->repeat >= 30 )
         {
            if( d->connected == CON_PLAYING )
            {
               sprintf( log_buf, "%s input spamming!", d->character->name );
               log_string( log_buf );
               monitor_chan( log_buf, MONITOR_CONNECT );
            }
            write_to_descriptor( d->descriptor, "\n\r***** SHUT UP!! *****\n\r", 0 );
            close_socket( d );
            /*
             * old way: strcpy( d->incomm, "quit" ); 
             */
         }
      }
   }

   /*
    * Do '!' substitution.
    */
   if( d->incomm[0] == '!' )
      strcpy( d->incomm, d->inlast );
   else
      strcpy( d->inlast, d->incomm );

   /*
    * Shift the input buffer.
    */
   while( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
      i++;
   for( j = 0; ( d->inbuf[j] = d->inbuf[i + j] ) != '\0'; j++ )
      ;
   return;
}



/*
 * Low level output function.
 */
bool process_output( DESCRIPTOR_DATA * d, bool fPrompt )
{
   extern bool merc_down;

   /*
    * Bust a prompt.
    */
   if( fPrompt && !merc_down && d->connected == CON_PLAYING )
   {
      if( d->showstr_point )
         write_to_buffer( d, "[Please type (c)ontinue, (r)efresh, (b)ack, (h)elp, (q)uit, or RETURN]:  ", 0 );
      else
      {
         CHAR_DATA *ch;

         ch = d->original ? d->original : d->character;
         if( IS_SET( ch->config, CONFIG_BLANK ) )
            write_to_buffer( d, "\n\r", 2 );
         if( ch->hunting || ch->hunt_obj )
            char_hunt( ch );
         bust_a_prompt( d );
         if( IS_SET( ch->config, CONFIG_TELNET_GA ) )
            write_to_buffer( d, go_ahead_str, 0 );
      }
   }
   /*
    * Short-circuit if nothing to write.
    */
   if( d->outtop == 0 )
      return TRUE;
   /*
    * Snoop-o-rama.
    */
   if( d->snoop_by != NULL )
   {
      char foo[MAX_STRING_LENGTH];
      CHAR_DATA *snoop_ch;

      snoop_ch = d->original != NULL ? d->original : d->character;
      if( snoop_ch != NULL )
         sprintf( foo, "[SNOOP:%s] ", snoop_ch->name );
      write_to_buffer( d->snoop_by, foo, 0 );
      write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
   }
   /*
    * OS-dependent output.
    */
   if( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
   {
      d->outtop = 0;
      return FALSE;
   }
   else
   {
      d->outtop = 0;
      return TRUE;
   }
}

/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
void bust_a_prompt( DESCRIPTOR_DATA * d )
{
   char buf[MAX_STRING_LENGTH];
   char buf2[MAX_STRING_LENGTH];
   const char *str;
   const char *i = " ";
   char *point;
   CHAR_DATA *ch;
   CHAR_DATA *victim;
   int check = -1;

   ch = d->character;

   /*
    * if editing a note, show a REAL simple prompt ;P 
    */
   if( ch->position == POS_WRITING && !IS_NPC( ch ) )
   {
      write_to_buffer( d, ">", 0 );
      return;
   }

   /*
    * If editing, show an 'info-prompt' -S- 
    */
   if( ch->position == POS_BUILDING && !IS_NPC( ch ) )
   {
      ROOM_INDEX_DATA *room;
      OBJ_INDEX_DATA *obj;
      MOB_INDEX_DATA *mob;
      char msg[MAX_STRING_LENGTH];  /* Mode */
      char msg2[MAX_STRING_LENGTH]; /* what */
      char msg3[MAX_STRING_LENGTH]; /* all */

      if( ch->act_build == ACT_BUILD_NOWT )
      {
         sprintf( msg, "Mode:  None" );
         sprintf( msg2, "Use Redit, Medit or Oedit to select mode." );
      }

      if( ch->act_build == ACT_BUILD_REDIT )
      {
         sprintf( msg, "Mode: Redit" );
         if( ch->build_vnum == -1 )
            sprintf( msg2, "No vnum is set." );
         else
         {
            room = get_room_index( ch->build_vnum );
            if( room != NULL )
               sprintf( msg2, "[%5d]: %s", ch->build_vnum, room->name );
         }
      }

      if( ch->act_build == ACT_BUILD_OEDIT )
      {
         sprintf( msg, "Mode: Oedit" );
         if( ch->build_vnum == -1 )
            sprintf( msg2, "No vnum set." );
         else
         {
            obj = get_obj_index( ch->build_vnum );
            if( obj != NULL )
               sprintf( msg2, "[%5d]: %s", ch->build_vnum, obj->short_descr );
         }
      }


      if( ch->act_build == ACT_BUILD_MEDIT )
      {
         sprintf( msg, "Mode: Medit" );
         if( ch->build_vnum == -1 )
            sprintf( msg2, "No vnum set." );
         else
         {
            mob = get_mob_index( ch->build_vnum );
            if( mob != NULL )
               sprintf( msg2, "[%5d]: %s", ch->build_vnum, mob->short_descr );
         }
      }
      sprintf( msg3, "< %s %s >", msg, msg2 );
      write_to_buffer( d, msg3, 0 );
      return;
   }

   /*
    * set default prompt to ">" -Uni 
    */
   if( !IS_SET( ch->config, CONFIG_PROMPT ) )
   {
      sprintf( buf2, "\r" );
      if( ch->hit < ch->max_hit )
      {
         check = 1;
         sprintf( buf, "<%dhp", ch->hit );
         strcat( buf2, buf );
      }
      if( ch->mana < ch->max_mana )
      {
         if( check == 1 )
            sprintf( buf, " %dmn", ch->mana );
         else
            sprintf( buf, "<%dmn", ch->mana );
         check = 1;
         strcat( buf2, buf );
      }
      if( ch->move < ch->max_move )
      {
         if( check == 1 )
            sprintf( buf, " %dmv", ch->move );
         else
            sprintf( buf, "<%dmv", ch->move );
         strcat( buf2, buf );
         check = 1;
      }
      if( check == 1 )
         send_to_char( buf2, ch );

      send_to_char( "> ", ch );
      return;
   }

   if( ch->prompt == NULL || ch->prompt[0] == '\0' )
   {
      send_to_char( "@@N> ", ch );
      return;
   }

   point = buf;
   str = d->original != NULL ? d->original->prompt : d->character->prompt;
   sprintf( buf2, "%s", "@@N" );
   i = buf2;
   while( ( *point = *i ) != '\0' )
      ++point, ++i;
   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch ( *str )
      {
         default:
            i = "> ";
            break;
         case 'w':
         {
            sh_int cl_index = -1;
            int cost;
            bool remort = FALSE, adept = FALSE;
            ++str;
            switch ( *str )
            {
               default:
                  sprintf( buf2, "%s", "BadClass" );
                  i = buf2;
                  break;
               case 'm':
               case 'M':
               {
                  ++str;
                  if( ( *str == 'a' ) || ( *str == 'A' ) )
                  {
                     cl_index = 0;
                     break;
                  }
                  else if( ( *str == 'o' ) || ( *str == 'O' ) )
                  {
                     cl_index = 4;
                     remort = TRUE;
                     break;
                  }
                  else
                  {
                     sprintf( buf2, "%s", "BadClass" );
                     i = buf2;
                     break;
                  }
               }

               case 'c':
               case 'C':
                  ++str;
                  cl_index = 1;
                  break;
               case 't':
               case 'T':
                  ++str;
                  cl_index = 2;
                  break;
               case 'w':
               case 'W':
                  ++str;
                  cl_index = 3;
                  break;
               case 'p':
               case 'P':
                  ++str;
                  cl_index = 4;
                  break;
               case 's':
               case 'S':
                  ++str;
                  cl_index = 0;
                  remort = TRUE;
                  break;
               case 'a':
               case 'A':
               {
                  ++str;
                  if( ( *str == 's' ) || ( *str == 'S' ) )
                  {
                     cl_index = 1;
                     remort = TRUE;
                     break;
                  }
                  else if( ( *str == 'd' ) || ( *str == 'D' ) )
                  {
                     cl_index = 0;
                     adept = TRUE;
                     break;
                  }
                  else
                  {
                     sprintf( buf2, "%s", "BadClass" );
                     i = buf2;
                     break;
                  }
               }

               case 'k':
               case 'K':
                  ++str;
                  cl_index = 2;
                  remort = TRUE;
                  break;
               case 'n':
               case 'N':
                  ++str;
                  cl_index = 3;
                  remort = TRUE;
                  break;

            }
            if( cl_index < 0 )
               break;
            if( adept == TRUE )
               cost = exp_to_level_adept( ch );
            else if( remort == TRUE )
               cost = exp_to_level( ch, cl_index, 5 );
            else
               cost = exp_to_level( ch, cl_index, ( ch->pcdata )->index[cl_index] );
            sprintf( buf2, "%d", UMAX( 0, cost - ch->exp ) );
            i = buf2;
            break;
         }
         case 'W':
         {
            sh_int cl_index = -1;
            int cost;
            bool remort = FALSE, adept = FALSE;
            ++str;
            switch ( *str )
            {
               default:
                  sprintf( buf2, "%s", "BadClass" );
                  i = buf2;
                  break;
               case 'm':
               case 'M':
               {
                  ++str;
                  if( ( *str == 'a' ) || ( *str == 'A' ) )
                  {
                     cl_index = 0;
                     break;
                  }
                  else if( ( *str == 'o' ) || ( *str == 'O' ) )
                  {
                     cl_index = 4;
                     remort = TRUE;
                     break;
                  }
                  else
                  {
                     sprintf( buf2, "%s", "BadClass" );
                     i = buf2;
                     break;
                  }
               }

               case 'c':
               case 'C':
                  ++str;
                  cl_index = 1;
                  break;
               case 't':
               case 'T':
                  ++str;
                  cl_index = 2;
                  break;
               case 'w':
               case 'W':
                  ++str;
                  cl_index = 3;
                  break;
               case 'p':
               case 'P':
                  ++str;
                  cl_index = 4;
                  break;
               case 's':
               case 'S':
                  ++str;
                  cl_index = 0;
                  remort = TRUE;
                  break;
               case 'a':
               case 'A':
               {
                  ++str;
                  if( ( *str == 's' ) || ( *str == 'S' ) )
                  {
                     cl_index = 1;
                     remort = TRUE;
                     break;
                  }
                  else if( ( *str == 'd' ) || ( *str == 'D' ) )
                  {
                     cl_index = 0;
                     adept = TRUE;
                     break;
                  }
                  else
                  {
                     sprintf( buf2, "%s", "BadClass" );
                     i = buf2;
                     break;
                  }
               }

               case 'k':
               case 'K':
                  ++str;
                  cl_index = 2;
                  remort = TRUE;
                  break;
               case 'n':
               case 'N':
                  ++str;
                  cl_index = 3;
                  remort = TRUE;
                  break;

            }
            if( cl_index < 0 )
               break;
            if( adept == TRUE )
               cost = exp_to_level_adept( ch );
            else if( remort == TRUE )
               cost = exp_to_level( ch, cl_index, 5 );
            else
               cost = exp_to_level( ch, cl_index, ( ch->pcdata )->index[cl_index] );
            sprintf( buf2, "%d", UMAX( 0, cost ) );
            i = buf2;
            break;
         }
         case 'h':
            sprintf( buf2, "%d", ch->hit );
            i = buf2;
            break;
         case '!':
            sprintf( buf2, "%d", GET_HITROLL( ch ) );
            i = buf2;
            break;
         case '+':
            sprintf( buf2, "%d", GET_DAMROLL( ch ) );
            i = buf2;
            break;
         case '*':
            sprintf( buf2, "%d", GET_AC( ch ) );
            i = buf2;
            break;
         case 'H':
            sprintf( buf2, "%d", ch->max_hit );
            i = buf2;
            break;
         case 'm':
            sprintf( buf2, "%d", ch->mana );
            i = buf2;
            break;
         case 'M':
            sprintf( buf2, "%d", ch->max_mana );
            i = buf2;
            break;
         case 'v':
            sprintf( buf2, "%d", ch->move );
            i = buf2;
            break;
         case 'V':
            sprintf( buf2, "%d", ch->max_move );
            i = buf2;
            break;
         case 'x':
            sprintf( buf2, "%d", ch->exp );
            i = buf2;
            break;
         case 'g':
            sprintf( buf2, "%d", ch->gold );
            i = buf2;
            break;
         case 'a':
            if( ch->level < 5 )
               sprintf( buf2, "%d", ch->alignment );
            else
               sprintf( buf2, "%s", IS_GOOD( ch ) ? "good" : IS_EVIL( ch ) ? "evil" : "neutral" );
            i = buf2;
            break;
         case 'r':
            if( ch->in_room != NULL )
               sprintf( buf2, "%s", ch->in_room->name );
            else
               sprintf( buf2, " " );
            i = buf2;
            break;
         case 'R':
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%d", ch->in_room->vnum );
            else
               sprintf( buf2, " " );
            i = buf2;
            break;
         case 's':
            if( !IS_NPC( ch ) )
               sprintf( buf2, "%s", stance_app[ch->stance].name );
            else
               sprintf( buf2, " " );
            i = buf2;
            break;

         case 'z':
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               sprintf( buf2, "%s", ch->in_room->area->name );
            else
               sprintf( buf2, " " );
            i = buf2;
            break;
         case '%':
            sprintf( buf2, "%%" );
            i = buf2;
            break;
         case 'b':
            if( !IS_NPC( ch ) && IS_VAMP( ch ) )
               sprintf( buf2, "@@e%d@@N", ch->pcdata->bloodlust );
            else
               sprintf( buf2, " " );
            i = buf2;
            break;
         case 'B':
            if( !IS_NPC( ch ) && IS_VAMP( ch ) )
               sprintf( buf2, "@@e%d@@N", ch->pcdata->bloodlust_max );
            else
               sprintf( buf2, " " );
            i = buf2;
            break;
         case 'c':
            if( !IS_NPC( ch ) )
               sprintf( buf2, "\n\r" );
            else
               sprintf( buf2, " " );
            i = buf2;
            break;

         case 'i':
            if( IS_NPC( ch ) )
               break;
            if( IS_IMMORTAL( ch ) )
               sprintf( buf2, "INVIS: %d", IS_SET( ch->act, PLR_WIZINVIS ) ? ch->invis : 0 );
            else
            {
               if( ( IS_AFFECTED( ch, AFF_INVISIBLE ) )
                   || ( IS_AFFECTED( ch, AFF_HIDE ) )
                   || ( item_has_apply( ch, ITEM_APPLY_INV ) ) || ( item_has_apply( ch, ITEM_APPLY_HIDE ) ) )
               {
                  sprintf( buf2, "%s", "INVIS" );
               }
               else
               {
                  sprintf( buf2, "%s", "VIS" );
               }
            }
            i = buf2;
            break;

         case 't':
            if( !IS_NPC( ch ) )
               sprintf( buf2, "%d %s", ( time_info.hour % 12 == 0 ) ? 12 : time_info.hour % 12,
                        time_info.hour >= 12 ? "pm" : "am" );
            else
               sprintf( buf2, " " );
            i = buf2;
            break;
         case 'F':
            if( !IS_NPC( ch ) && IS_WOLF( ch ) )
            {
               if( IS_RAGED( ch ) )
                  sprintf( buf2, "%s", "GAROU" );
               else if( IS_SHIFTED( ch ) )
                  sprintf( buf2, "%s", "LUPIS" );
               else
                  sprintf( buf2, "%s", "HUMANUS" );
            }

            else
               sprintf( buf2, " " );
            i = buf2;
            break;

         case 'p':
            if( !IS_NPC( ch ) )
            {
               if( weather_info.moon_loc == MOON_DOWN )
                  sprintf( buf2, "%s", "DOWN" );
               else if( weather_info.moon_loc == MOON_PEAK )
                  sprintf( buf2, "PEAK:%s", get_moon_phase_name(  ) );
               else
                  sprintf( buf2, "LOW:%s", get_moon_phase_name(  ) );
            }
            else
               sprintf( buf2, " " );
            i = buf2;
            break;


      }
      ++str;
      while( ( *point = *i ) != '\0' )
         ++point, ++i;
   }


   /*
    * possible HUGE string bug here 
    */

   /*
    * Also have part of battle prompt showing tank status -S- 
    */
   if( ch->fighting != NULL ) /* only way to do it, sorry */
   {
      /*
       * We'll just show a percentage for people in the group 
       */
      CHAR_DATA *tank;
      char buf[200];

      buf[0] = '\0';

      /*
       * See who the target (ch->fighting) is fighting (->fighting) 
       */
      tank = ch->fighting->fighting;
      if( tank != NULL )
      {
         int percent;
         char wound[100];


         if( tank->max_hit > 0 )
            percent = tank->hit * 100 / tank->max_hit;
         else
            percent = -1;

         if( percent >= 95 )
            sprintf( wound, "@@rExcellent@@N" );
         else if( percent >= 85 )
            sprintf( wound, "@@GScratches@@N" );
         else if( percent >= 70 )
            sprintf( wound, "@@ySmall wounds@@N" );
         else if( percent >= 55 )
            sprintf( wound, "@@bLarger wounds@@N" );
         else if( percent >= 45 )
            sprintf( wound, "@@mBleeding freely@@N" );
         else if( percent >= 30 )
            sprintf( wound, "@@RLeaking guts@@N" );
         else if( percent >= 15 )
            sprintf( wound, "@@eA MESS!@@N" );
         else
            sprintf( wound, "@@2@@W@@fALMOST DEAD!!!@@N" );
         if( !IS_NPC( ch ) && IS_SET( ch->pcdata->pflags, PFLAG_BLIND_PLAYER ) )
            sprintf( buf, "@@W%s %s @@N ", tank == ch ? "YOU" : "Tank", wound );
         else
            sprintf( buf, "@@a[@@W%s@@a:%s@@a]@@N ", tank == ch ? "YOU" : "Tank", wound );
         write_to_buffer( d, buf, 0 );
      }
   }


   /*
    * battle prompt 
    */
   if( ( victim = ch->fighting ) != NULL )

   {
      int percent;
      /*
       * int percent2; 
       */
      char wound[100];
      char buf[MAX_STRING_LENGTH];

      if( victim->max_hit > 0 )
         percent = victim->hit * 100 / victim->max_hit;
      else
         percent = -1;

      if( percent >= 95 )
         sprintf( wound, "@@rExcellent@@N" );
      else if( percent >= 85 )
         sprintf( wound, "@@GScratches@@N" );
      else if( percent >= 70 )
         sprintf( wound, "@@ySmall wounds@@N" );
      else if( percent >= 55 )
         sprintf( wound, "@@bLarger wounds@@N" );
      else if( percent >= 45 )
         sprintf( wound, "@@mBleeding freely@@N" );
      else if( percent >= 30 )
         sprintf( wound, "@@RLeaking guts@@N" );
      else if( percent >= 15 )
         sprintf( wound, "@@eA MESS!@@N" );
      else
         sprintf( wound, "@@2@@WALMOST DEAD!!!@@N" );
      if( !IS_NPC( ch ) && IS_SET( ch->pcdata->pflags, PFLAG_BLIND_PLAYER ) )
         sprintf( buf, "@@WVictim %s @@N\n\r", wound );
      else
         sprintf( buf, "@@a[@@WVictim@@a:%s@@a]@@N\n\r", wound );

      /*
       * buf[0] = UPPER(buf[0]); 
       */
      write_to_buffer( d, buf, 0 );
      /*
       * No newline, to keep both prompts on one line... 
       */
   }


   /*
    * MAG Mod should be point-buf + 1, not point-buf (for end null char.) 
    */

   write_to_buffer( d, buf, point - buf );
   return;
}

/*
 * Append onto an output buffer.
 */

#define color_MARGIN 20

void write_to_buffer( DESCRIPTOR_DATA * d, const char *txt, int length )
{
   if( d == NULL )
      return;

   if( d->outbuf == NULL )
   {
      bugf( "write_to_buffer with NULL outbuf, string=%s", txt );
      return;
   }

   /*
    * Find length in case caller didn't.
    */
   if( length <= 0 )
      length = strlen( txt );

   /*
    * Initial \n\r if needed.
    */
   if( d->outtop == 0 && !d->fcommand )
   {
      d->outbuf[0] = '\n';
      d->outbuf[1] = '\r';
      d->outtop = 2;
   }

   /*
    * Expand the buffer as needed.
    */
   while( d->outtop + length + 1 + color_MARGIN >= d->outsize )
   {
      char *outbuf;

      outbuf = getmem( 2 * d->outsize );
      strncpy( outbuf, d->outbuf, d->outtop );
      dispose( d->outbuf, d->outsize );
      d->outbuf = outbuf;
      d->outsize *= 2;
   }

   /*
    * Copy.
    */

   /*
    * ONLY COPY length number of Bytes, otherwise things can go WRONG 
    */
   /*
    * MAG mod to strncpy 
    */
   /*
    * Old was  strcpy( d->outbuf + d->outtop, txt ); 
    */


   /*
    * Before Mod2: strncpy( d->outbuf + d->outtop, txt, length ); 
    */

   /*
    * MAG Mod2: Convert color strings, while copying. Also expand buffer as neede. 
    */
   {
      char c;
      char lookup;
      char *dest;
      int count = length;
      CHAR_DATA *ch;
      char *colstr;
      int collen, cnt;

      dest = d->outbuf + d->outtop;

      for( ; count > 0; )
      {
         c = *( txt++ );
         if( c != '@' )
         {
            *( dest++ ) = c;
            count--;
            continue;
         }
         else
         {
            if( *txt != '@' )
            {
               *( dest++ ) = c;
               count--;
               continue;
            }

            txt++;   /* txt now points at color code. */
            c = *( txt++ );   /* c is color code. */
            length = length - 3;
            count = count - 3;

            if( c == '@' )
            {
               length++;
               *( dest++ ) = c;
               continue;
            }

            ch = d->original != NULL ? d->original : d->character;
            if( ch != NULL && !IS_SET( ch->config, CONFIG_COLOR ) )
               continue;
/* set to default highlight or dim set by player */
            lookup = c;
            if( ch != NULL && !IS_NPC( ch ) )   /* shouldn't happen, but... */
            {
               if( lookup == '!' )
                  lookup = ch->pcdata->hicol;
               else if( lookup == '.' )
                  lookup = ch->pcdata->dimcol;
            }

            for( cnt = 0; cnt < MAX_ANSI; cnt++ )
               if( ansi_table[cnt].letter == lookup )
                  break;

            if( cnt == MAX_ANSI )
            {
               colstr = ansi_table[10].value;
               collen = ansi_table[10].stlen;
            }
            else
            {
               colstr = ansi_table[cnt].value;
               collen = ansi_table[cnt].stlen;
            }


            while( d->outtop + length + collen + 1 >= d->outsize )
            {
               char *outbuf;

               outbuf = getmem( 2 * d->outsize );
               strncpy( outbuf, d->outbuf, d->outtop + length - count );
               dispose( d->outbuf, d->outsize );
               d->outbuf = outbuf;
               d->outsize *= 2;
            }

            dest = d->outbuf + d->outtop + length - count;
            strncpy( dest, colstr, collen );
            dest += collen;

            length = length + collen;
         }
      }
   }

   /*
    * Make sure we have a \0 at the end 
    */
   *( d->outbuf + d->outtop + length ) = '\0';

   d->outtop += length;
   return;
}



/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor( int desc, char *txt, int length )
{
   int iStart;
   int nWrite;
   int nBlock;

   if( length <= 0 )
      length = strlen( txt );

   for( iStart = 0; iStart < length; iStart += nWrite )
   {
      nBlock = UMIN( length - iStart, 4096 );
      if( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
      {
         perror( "Write_to_descriptor" );
         return FALSE;
      }
   }
   return TRUE;
}

void show_menu_to( DESCRIPTOR_DATA * d )
{
   CHAR_DATA *ch = d->character;
   char buf[MAX_STRING_LENGTH];
   char menu[MAX_STRING_LENGTH];

   sprintf( menu, "\n\rCharacter Creation Menu.\n\r\n\r" );
   strcat( menu, "Options:\n\r" );

   sprintf( buf, "        1. Set Gender.       Currently:%s\n\r",
            !IS_SET( d->check, CHECK_SEX ) ? "Not Set." :
            ch->sex == SEX_NEUTRAL ? "Neutral." : ch->sex == SEX_MALE ? "Male." : "Female." );
   strcat( menu, buf );

   sprintf( buf, "        2. Set Race.         Currently:%s\n\r",
            !IS_SET( d->check, CHECK_RACE ) ? "Not Set." : race_table[ch->race].race_title );
   strcat( menu, buf );

   strcat( menu, "        3. Roll Attributes.  Currently:" );
   if( IS_SET( d->check, CHECK_STATS ) )
      sprintf( buf, "\n\r        Max_Str:%d.  Max_Int:%d.  Max_Wis:%d.\n\r        Max_Dex:%d.  Max_Con:%d.\n\r",
               ch->pcdata->max_str, ch->pcdata->max_int, ch->pcdata->max_wis, ch->pcdata->max_dex, ch->pcdata->max_con );
   else
      sprintf( buf, "Not Set.\n\r" );


   strcat( menu, buf );

   strcat( menu, "        4. Set Class Order.  Currently:" );
   if( IS_SET( d->check, CHECK_CLASS ) )
   {
      int fubar;
      sprintf( buf, "\n\r        " );
      for( fubar = 0; fubar < MAX_CLASS; fubar++ )
      {
         strcat( menu, class_table[ch->pcdata->order[fubar]].who_name );
         strcat( menu, ". " );
      }
      strcat( menu, "\n\r" );
   }
   else
      strcat( menu, "Not Set.\n\r" );

   strcat( menu, "        5. Exit Creation Process.\n\r" );

   strcat( menu, "\n\rPlease Select 1-5: " );
   write_to_buffer( d, menu, 0 );
   return;
}

void show_smenu_to( DESCRIPTOR_DATA * d )
{
   CHAR_DATA *ch = d->character;
   char buf[MAX_STRING_LENGTH];
   char menu[MAX_STRING_LENGTH];

   sprintf( menu, "\n\rCharacter Creation: Gender.\n\r\n\r" );

   strcat( menu, "Please Select:\n\r" );
   strcat( menu, "              M : Male.\n\r" );
   strcat( menu, "              F : Female.\n\r" );
   strcat( menu, "              N : Neutral.\n\r\n\r" );

   if( IS_SET( d->check, CHECK_SEX ) )
      sprintf( buf, "Current Choice: %s\n\r",
               ch->sex == SEX_NEUTRAL ? "Neutral." : ch->sex == SEX_MALE ? "Male." : "Female." );
   else
      sprintf( buf, "No Current Selection.\n\r" );

   strcat( menu, buf );
   strcat( menu, "\n\rPlease Select M/F/N: " );

   write_to_buffer( d, menu, 0 );
   return;
}

void show_rmenu_to( DESCRIPTOR_DATA * d )
{
   char menu[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   int iRace;

   sprintf( menu, "\n\rCharacter Creation: Race.\n\r\n\r" );
   strcat( menu, "Notes: a) Race determines abilities in different classes.\n\r" );
   strcat( menu, "       b) Each Race will soon have seperate hometowns.\n\r" );
   strcat( menu, "       c) Race determines your attributes.\n\r\n\r" );
   strcat( menu, "Abr   Name             Class Abilities       Str Int Wis Dex Con\n\r" );
   strcat( menu, "---   ----             ---------------       --- --- --- --- ---\n\r" );

   for( iRace = 0; iRace < MAX_RACE; iRace++ )
   {
      if( race_table[iRace].player_allowed == FALSE )
         continue;
      sprintf( buf, "%3s   %-10s     %s", race_table[iRace].race_name,
               race_table[iRace].race_title, race_table[iRace].comment );
      strcat( menu, buf );

      sprintf( buf, "  %-2d  %-2d  %-2d  %-2d  %-2d\n\r",
               race_table[iRace].race_str,
               race_table[iRace].race_int,
               race_table[iRace].race_wis, race_table[iRace].race_dex, race_table[iRace].race_con );
      strcat( menu, buf );
   }

   strcat( menu, "\n\rPlease Select Your Race (Abr), or type the full race name for race info: " );
   write_to_buffer( d, menu, 0 );
   return;
}

void show_amenu_to( DESCRIPTOR_DATA * d )
{
   CHAR_DATA *ch = d->character;
   char buf[MAX_STRING_LENGTH];
   char menu[MAX_STRING_LENGTH];

   /*
    * Make the 'rolls', set ch->max_*, and display 
    */
   if( !IS_SET( d->check, CHECK_RACE ) )
   {
      sprintf( menu, "\n\rYou must select a race first.\n\r" );
      write_to_buffer( d, menu, 0 );
      d->connected = CON_MENU;
      show_menu_to( d );
      return;
   }

   for( ;; )
   {
      ch->pcdata->max_str = UMIN( 22, ( race_table[ch->race].race_str + number_range( -3, 3 ) ) );
      ch->pcdata->max_str = UMIN( 22, ch->pcdata->max_str );
      ch->pcdata->max_int = UMIN( 22, ( race_table[ch->race].race_int + number_range( -3, 3 ) ) );
      ch->pcdata->max_int = UMIN( 22, ch->pcdata->max_int );
      ch->pcdata->max_dex = UMIN( 22, ( race_table[ch->race].race_dex + number_range( -3, 3 ) ) );
      ch->pcdata->max_dex = UMIN( 22, ch->pcdata->max_dex );
      ch->pcdata->max_con = UMIN( 22, ( race_table[ch->race].race_con + number_range( -3, 3 ) ) );
      ch->pcdata->max_con = UMIN( 22, ch->pcdata->max_con );
      ch->pcdata->max_wis = UMIN( 22, ( race_table[ch->race].race_wis + number_range( -3, 3 ) ) );
      ch->pcdata->max_wis = UMIN( 22, ch->pcdata->max_wis );

      if( ( ch->pcdata->max_str + ch->pcdata->max_int + ch->pcdata->max_wis
            + ch->pcdata->max_dex + ch->pcdata->max_con ) < 91 )
         break;   /* Prevent 'super' characters! */
   }

   sprintf( menu, "\n\rCharacter Creation: Attributes.\n\r\n\r" );
   strcat( menu, "This option rolls a new set of MAX attributes.\n\r" );
   strcat( menu, "There is no way to increase your MAX attributes, so choose wisely!\n\r" );
   strcat( menu, "Current Attributes:  (Just Rolled)\n\r" );

   sprintf( buf, "Max_Str:%d.  Max_Int:%d.  Max_Wis:%d.  Max_Dex:%d.  Max_Con:%d.\n\r",
            ch->pcdata->max_str, ch->pcdata->max_int, ch->pcdata->max_wis, ch->pcdata->max_dex, ch->pcdata->max_con );
   strcat( menu, buf );
   strcat( menu, "\n\rPlease Select: (A)ccept, return to menu, (H)help stats, or (R)eroll: " );

   write_to_buffer( d, menu, 0 );
   return;
}

void show_ahelp_menu_to( DESCRIPTOR_DATA * d )
{

   char menu[MAX_STRING_LENGTH];
   sprintf( menu, "%s", "" );
   strcat( menu, "Str affects items you can wear and weight you can carry, and your hitroll and damroll.\n\r" );
   strcat( menu, "Int affects your mana gain, how many Npcs you can control effectively, and spell success.\n\r" );
   strcat( menu, "Wis affects how many practices you get, your mana, and your saving against spells.\n\r" );
   strcat( menu, "Dex affects your ac, how many items you can carry, and your ability to dodge.\n\r" );
   strcat( menu, "Con affects how many hitpoints you gain per level.\n\r" );
   strcat( menu, "\n\rPlease Select: (A)ccept, return to menu, (H)help stats, or (R)eroll: " );

   write_to_buffer( d, menu, 0 );
   return;
}

void show_cmenu_to( DESCRIPTOR_DATA * d )
{
   char menu[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   int iClass;

   sprintf( menu, "Character Creation: Class Order.\n\r\n\r" );
   strcat( menu, "This option allows you to select the order of your classes.\n\r" );
   strcat( menu, "Being a MultiClass Mud, this order is very important, as it\n\r" );
   strcat( menu, "will determine how easily you progress in each class, and\n\r" );
   strcat( menu, "how well you can use the skills/spells of each class.\n\r" );
   strcat( menu, "There are five classes.  Please list, in order of best to\n\r" );
   strcat( menu, "worst, the order your classes will be.\n\r" );
   strcat( menu, "(The 1st you pick will be your prime class, gaining a +1 bonus.\n\r" );
   strcat( menu, "For example, psi mag cle thi war.\n\r" );
   strcat( menu, "Abr    Prime Atr    Name\n\r" );
   strcat( menu, "---    ---------    ----\n\r" );

   for( iClass = 0; iClass < MAX_CLASS; iClass++ )
   {
      sprintf( buf, "%3s    %3s    %-10s\n\r", class_table[iClass].who_name, class_table[iClass].attr,
               class_table[iClass].class_name );
      strcat( menu, buf );
   }
   strcat( menu, "\n\rOrder: " );
   write_to_buffer( d, menu, 0 );
   return;
}




/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA * d, char *argument )
{
   char buf[MAX_STRING_LENGTH];
   char msg[MAX_STRING_LENGTH];
   CHAR_DATA *ch;
   char *pwdnew;
   char *p;
   int iClass;
   int lines;
   bool fOld;
   int cnt;
   NOTE_DATA *pnote;
   int notes;

   while( isspace( *argument ) )
      argument++;

   ch = d->character;

   if( d->connected == CON_GET_NAME )
   {
      if( argument[0] == '\0' )
      {
         close_socket( d );
         return;
      }

      argument[0] = UPPER( argument[0] );

      sprintf( buf, "%s provided as name from login from site %s.", argument, d->host );
      monitor_chan( buf, MONITOR_CONNECT );

      if( !check_parse_name( argument ) )
      {
         sprintf( buf, "Illegal name %s from site %s.", argument, d->host );
         monitor_chan( buf, MONITOR_CONNECT );
         write_to_buffer( d, "Illegal name, try another.\n\rName: ", 0 );
         return;
      }

      fOld = load_char_obj( d, argument, FALSE );
      ch = d->character;



      if( IS_SET( ch->act, PLR_DENY ) )
      {
         sprintf( log_buf, "Denying access to %s@%s.", argument, d->host );
         log_string( log_buf );
         monitor_chan( log_buf, MONITOR_CONNECT );
         write_to_buffer( d, "You are denied access.\n\r", 0 );
         close_socket( d );
         return;
      }
/* TEMP FIX ZEN */
      if( IS_SET( ch->config, CONFIG_JUSTIFY ) )
         REMOVE_BIT( ch->config, CONFIG_JUSTIFY );
      if( check_reconnect( d, argument, FALSE ) )
      {
         fOld = TRUE;
      }
      else
      {
         if( wizlock && !IS_HERO( ch ) && !ch->wizbit && !is_name( argument, sysdata.playtesters ) )
         {
            write_to_buffer( d, "\n\r             " mudnamenocolor " IS CURRENTLY WIZLOCKED.\n\r", 0 );
            write_to_buffer( d, "Please Try Connecting Again In A Little While, When Any Problems\n\r", 0 );
            write_to_buffer( d, "       We Are Working On Have Been Solved.  Thank You.\n\r", 0 );
            close_socket( d );
            return;
         }
         if( deathmatch && !IS_HERO( ch ) && !ch->wizbit )
         {
            write_to_buffer( d, "\n\r             " mudnamenocolor " IS CURRENTLY WIZLOCKED.\n\r", 0 );
            write_to_buffer( d, "Sorry, The Players Connected At This Time Are Currently Participating\n\r", 0 );
            write_to_buffer( d, "     In A DEATHMATCH Murder-Fest.  Please try later!\n\r", 0 );
            write_to_buffer( d, "  Deathmatches are usually held on Thursdays and Sundays.  They\n\r", 0 );
            write_to_buffer( d, " normally last about 30 minutes.  Please be patient!\n\r", 0 );
            close_socket( d );
            return;
         }
         if( check_playing( d, ch->name ) )
            return;
      }



      if( fOld )
      {
         BAN_DATA *pban;

         for( pban = first_ban; pban != NULL; pban = pban->next )
         {
            if( !str_prefix( pban->name, d->host ) && ( pban->newbie == FALSE ) )
            {
               char buf[MAX_STRING_LENGTH];
               sprintf( buf, "Denying access to banned site %s", d->host );
               monitor_chan( buf, MONITOR_CONNECT );
               write_to_descriptor( d->descriptor, "Your site has been banned from this Mud.  BYE BYE!\n\r", 0 );
               d->connected = CON_QUITTING;
               close_socket( d );
               return;
            }
         }

         /*
          * Old player 
          */
         write_to_buffer( d, "Password: ", 0 );
         write_to_buffer( d, echo_off_str, 0 );
         d->connected = CON_GET_OLD_PASSWORD;
         return;
      }
      else
      {
         BAN_DATA *pban;
         /*
          * New player 
          */
         /*
          * New characters with same name fix by Salem's Lot 
          */
         if( check_playing( d, ch->name ) )
            return;

         for( pban = first_ban; pban != NULL; pban = pban->next )
         {
            if( !str_prefix( pban->name, d->host ) )

            {
               char buf[MAX_STRING_LENGTH];
               sprintf( buf, "Denying access to banned site %s", d->host );
               monitor_chan( buf, MONITOR_CONNECT );
               write_to_descriptor( d->descriptor, "Your site has been banned from this Mud.  BYE BYE!\n\r", 0 );
               d->connected = CON_QUITTING;
               close_socket( d );
               return;
            }
         }


         sprintf( buf, "Did I get that right, %s (Y/N)? ", argument );
         write_to_buffer( d, buf, 0 );
         d->connected = CON_CONFIRM_NEW_NAME;
         return;
      }
   }

   if( d->connected == CON_GET_OLD_PASSWORD )
   {
      write_to_buffer( d, "\n\r", 2 );
      if( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
      {
         write_to_buffer( d, "Wrong password.\n\r", 0 );
         sprintf( buf, "FAILED LOGIN for %s from site %s.", ch->name, d->host );
         monitor_chan( buf, MONITOR_CONNECT );
         ch->pcdata->failures++;
         save_char_obj( ch );
         close_socket( d );
         return;
      }

      write_to_buffer( d, echo_on_str, 0 );

      if( check_reconnect( d, ch->name, TRUE ) )
         return;

      if( check_playing( d, ch->name ) )
         return;

      sprintf( log_buf, "%s has connected.", ch->name );
      monitor_chan( log_buf, MONITOR_CONNECT );

      sprintf( log_buf, "Site Name: %s.", d->host );
      monitor_chan( log_buf, MONITOR_CONNECT );

      log_string( log_buf );
      lines = ch->pcdata->pagelen;
      ch->pcdata->pagelen = 20;

      if( ch->lvl[ch->class] == -1 )
         ch->lvl[ch->class] = ch->level;

      if( IS_HERO( ch ) )
      {
         DL_LIST *brands;
         sh_int numbrands;
         char msgbuf[MSL];
         for( brands = first_brand, numbrands = 0; brands; brands = brands->next, numbrands++ );
         do_help( ch, "imotd" );
         sprintf( msgbuf, "There are currently %d outstanding brands.\n\r%s",
                  numbrands,
                  ( ( numbrands < 50 ) ?
                    "" :
                    "@@eWarning: Process these brands immediately using immbrand list, immbrand read, and immbrand remove to avoid disk overflow!!@@N\n\r" ) );
         send_to_char( msgbuf, ch );

      }
      else
      {
         do_help( ch, "motd" );
      }
      ch->pcdata->pagelen = lines;
      d->connected = CON_READ_MOTD;
   }

   if( d->connected == CON_CONFIRM_NEW_NAME )
   {
      switch ( *argument )
      {
         case 'y':
         case 'Y':
            sprintf( buf, "New character.\n\rGive me a password for %s: %s", ch->name, echo_off_str );
            write_to_buffer( d, buf, 0 );
            d->connected = CON_GET_NEW_PASSWORD;
            return;

         case 'n':
         case 'N':
            write_to_buffer( d, "Ok, what IS it, then? ", 0 );
            free_char( d->character );
            d->character = NULL;
            d->connected = CON_GET_NAME;
            return;

         default:
            write_to_buffer( d, "Please type Yes or No? ", 0 );
            return;
      }
      return;
   }

   if( d->connected == CON_GET_NEW_PASSWORD )
   {
      write_to_buffer( d, "\n\r", 2 );

      if( strlen( argument ) < 5 )
      {
         write_to_buffer( d, "Password must be at least five characters long.\n\rPassword: ", 0 );
         return;
      }

      pwdnew = crypt( argument, ch->name );
      for( p = pwdnew; *p != '\0'; p++ )
      {
         if( *p == '~' )
         {
            write_to_buffer( d, "New password not acceptable, try again.\n\rPassword: ", 0 );
            return;
         }
      }

      free_string( ch->pcdata->pwd );
      ch->pcdata->pwd = str_dup( pwdnew );
      write_to_buffer( d, "Please retype password: ", 0 );
      d->connected = CON_CONFIRM_NEW_PASSWORD;
      return;
   }

   if( d->connected == CON_CONFIRM_NEW_PASSWORD )
   {
      write_to_buffer( d, "\n\r", 2 );

      if( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
      {
         write_to_buffer( d, "Passwords don't match.\n\rRetype password: ", 0 );
         d->connected = CON_GET_NEW_PASSWORD;
         return;
      }
      write_to_buffer( d, echo_on_str, 0 );
      show_menu_to( d );
      d->connected = CON_MENU;
      return;
   }

   if( d->connected == CON_MENU )
   {
      int number;

      if( !is_number( argument ) )
      {
         write_to_buffer( d, "\n\rPlease Enter A Number.\n\r", 0 );
         show_menu_to( d );
         return;
      }
      number = atoi( argument );
      if( number < 1 && number > 5 )
      {
         write_to_buffer( d, "\n\rPlease Enter A Number Between 1 And 5.\n\r", 0 );
         show_menu_to( d );
         return;
      }

      switch ( number )
      {
         case 1:
            d->connected = CON_GET_NEW_SEX;
            show_smenu_to( d );
            break;
         case 2:
            d->connected = CON_GET_RACE;
            show_rmenu_to( d );
            break;
         case 3:
            d->connected = CON_GET_STATS;
            show_amenu_to( d );
            break;
         case 4:
            d->connected = CON_GET_NEW_CLASS;
            show_cmenu_to( d );
            break;
         case 5:
            if( !IS_SET( d->check, CHECK_SEX ) || !IS_SET( d->check, CHECK_CLASS )
                || !IS_SET( d->check, CHECK_STATS ) || !IS_SET( d->check, CHECK_RACE ) )
            {
               write_to_buffer( d, "ALL Options Must Be Selected First.\n\r", 0 );
               show_menu_to( d );
               return;
            }
            sprintf( log_buf, "%s@%s new player.", ch->name, d->host );
            log_string( log_buf );
            monitor_chan( log_buf, MONITOR_CONNECT );
            write_to_buffer( d, "\n\r", 2 );
            ch->pcdata->pagelen = 20;

            /*
             * prompt already set?
             * ch->prompt = str_dup("<%hhp %mm %vmv> ");
             * old crash bug was b/c of prompts somehow 
             */

            do_help( ch, "newun" );
            d->connected = CON_READ_MOTD;
            /*
             * Display motd, and all other malarky 
             */
            break;
      }
      return;
   }

   if( d->connected == CON_GET_STATS )
   {
      switch ( argument[0] )
      {
         case 'A':
         case 'a':
            if( !IS_SET( d->check, CHECK_STATS ) )
               SET_BIT( d->check, CHECK_STATS );
            d->connected = CON_MENU;
            show_menu_to( d );
            break;
         case 'R':
         case 'r':
            show_amenu_to( d );
            break;
         case 'H':
         case 'h':
            show_ahelp_menu_to( d );
            break;
         default:
            write_to_buffer( d, "Enter A or R, or H for stat help:", 0 );
            break;
      }
      return;
   }


   if( d->connected == CON_GET_NEW_SEX )
   {
      switch ( argument[0] )
      {
         case 'm':
         case 'M':
            ch->sex = SEX_MALE;
            ch->login_sex = SEX_MALE;
            break;
         case 'f':
         case 'F':
            ch->sex = SEX_FEMALE;
            ch->login_sex = SEX_FEMALE;
            break;
         case 'n':
         case 'N':
            ch->sex = SEX_NEUTRAL;
            ch->login_sex = SEX_NEUTRAL;
            break;
         default:
            write_to_buffer( d, "That's not a sex.\n\rWhat IS your sex? ", 0 );
            show_smenu_to( d );
            return;
      }
      write_to_buffer( d, "\n\r\n\r", 0 );
      if( !IS_SET( d->check, CHECK_SEX ) )
         SET_BIT( d->check, CHECK_SEX );
      d->connected = CON_MENU;
      show_menu_to( d );
      return;
   }

   if( d->connected == CON_GET_NEW_CLASS )
   {
      sh_int classes[MAX_CLASS];
      sh_int parity[MAX_CLASS];  /* Nowt to do with parity really */
      sh_int index[MAX_CLASS];
      char arg[MAX_STRING_LENGTH];
      int cnt;
      int foo;
      bool ok = TRUE;

      /*
       * Parity set to 1 for each class found. 
       */
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         parity[cnt] = -1;

      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      {
         argument = one_argument( argument, arg );
         if( arg[0] == '\0' )
         {
            ok = FALSE;
            break;
         }
         for( foo = 0; foo < MAX_CLASS; foo++ )
            if( !str_cmp( arg, class_table[foo].who_name ) )
            {
               classes[cnt] = foo;
               index[foo] = cnt;
               parity[foo] = 1;
               break;
            }
         if( foo == MAX_CLASS )
         {
            ok = FALSE;
            break;
         }
      }

      /*
       * If 5 unique classes given, parity[cnt] == 1 
       */
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
         if( parity[cnt] == -1 ) /* Then a class was missed */
            ok = FALSE;

      if( !ok )
      {
         write_to_buffer( d,
                          "Invalid Order... Please Try Again. You must list each class, by abbreviation, such as CLE WAR MAG THI PSI.\n\r",
                          0 );
         show_cmenu_to( d );
         return;
      }

      /*
       * Copy classes across to pcdata 
       */
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
      {
         ch->pcdata->order[cnt] = classes[cnt];
         ch->pcdata->index[cnt] = index[cnt];
      }

      d->connected = CON_MENU;
      if( !IS_SET( d->check, CHECK_CLASS ) )
         SET_BIT( d->check, CHECK_CLASS );
      show_menu_to( d );
      return;
   }

   if( d->connected == CON_GET_RACE )
   {
      for( iClass = 0; iClass < MAX_RACE; iClass++ )
      {
         if( ( !str_cmp( argument, race_table[iClass].race_name ) ) && ( race_table[iClass].player_allowed == TRUE ) )
         {
            ch->race = iClass;
            break;
         }
/* curtesy of Sylvana@LCN */
         else if( !str_cmp( argument, race_table[iClass].race_title ) )
         {
            do_help( ch, argument );
         }
      }

      if( ( iClass == MAX_RACE ) || ( race_table[iClass].player_allowed == FALSE ) )
      {
         write_to_buffer( d, "Invalid Choice.\n\r", 0 );
         show_rmenu_to( d );
         return;
      }
      if( IS_SET( d->check, CHECK_STATS ) )
         REMOVE_BIT( d->check, CHECK_STATS );


      if( !IS_SET( d->check, CHECK_RACE ) )
         SET_BIT( d->check, CHECK_RACE );
      show_menu_to( d );
      d->connected = CON_MENU;
      return;
   }

   if( d->connected == CON_READ_MOTD )
   {
      /*
       * Prime level idea dropped.  Give ch 1 level in their best class 
       */
      if( ch->level == 0 )
      {
         ch->class = ch->pcdata->order[0];
         ch->lvl[ch->class] = 1;
      }

      LINK( ch, first_char, last_char, next, prev );
      d->connected = CON_PLAYING;


      if( IS_SET( ch->config, CONFIG_FULL_ANSI ) )
      {
         char scrollbuf[MSL];
         sprintf( scrollbuf, "%s%s%s%i;%ir%s%%i;%iH",
                  CRS_RESET, CRS_CLS, CRS_CMD, 0, ch->pcdata->term_rows - 12, CRS_CMD, ch->pcdata->term_rows - 13 );
         send_to_char( scrollbuf, ch );
      }
      send_to_char( "\n\rWelcome to " mudnamecolor ".  May your visit here be ... mutated.\n\r", ch );

      if( ch->level == 0 )
      {
         /*
          * OBJ_DATA *obj; unused 
          */

         switch ( class_table[ch->class].attr_prime )
         {
            case APPLY_STR:
               ch->pcdata->max_str++;
               break;
            case APPLY_INT:
               ch->pcdata->max_int++;
               break;
            case APPLY_WIS:
               ch->pcdata->max_wis++;
               break;
            case APPLY_DEX:
               ch->pcdata->max_dex++;
               break;
            case APPLY_CON:
               ch->pcdata->max_con++;
               break;
         }

         ch->level = 1;

         /*
          * FIXME: this temp fix for m/c stuff 
          */
         /*
          * All Races get 5 classes now.. 
          */

         ch->lvl[ch->class] = 1;
         for( cnt = 0; cnt < MAX_CLASS; cnt++ )
            if( cnt != ch->class )
               ch->lvl[cnt] = 0;

         ch->exp = 0;
         ch->hit = ch->max_hit;
         ch->mana = ch->max_mana;
         ch->move = ch->max_move;
         ch->pcdata->mana_from_gain = ch->max_mana;
         ch->pcdata->hp_from_gain = ch->max_hit;
         ch->pcdata->move_from_gain = ch->max_move;

         ch->pcdata->clan = 0;   /* no clan */
         sprintf( buf, " needs a new title!" );
         set_title( ch, buf );

         {
            char race_skill[MSL];
            char *race_skill_list;
            race_skill_list = race_table[ch->race].skill1;
            for( ;; )
            {
               race_skill_list = one_argument( race_skill_list, race_skill );
               if( skill_lookup( race_skill ) < 0 )
                  break;
               ch->pcdata->learned[skill_lookup( race_skill )] = 101;
            }
         }
         ch->deaf = 0;
         char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
         /*
          * Needed in case newbie drops an item. 
          */

         /*
          * This seems to be causing a prob on meson right now.. byebye!
          * 
          * obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_BANNER), 0 );
          * obj_to_char( obj, ch );
          * equip_char( ch, obj, WEAR_LIGHT );
          * 
          * obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_VEST), 0 );
          * obj_to_char( obj, ch );
          * equip_char( ch, obj, WEAR_BODY );
          * 
          * obj = create_object( get_obj_index(OBJ_VNUM_SCHOOL_SHIELD), 0 );
          * obj_to_char( obj, ch );
          * equip_char( ch, obj, WEAR_SHIELD );
          * 
          * obj = create_object( get_obj_index(class_table[ch->class].weapon),
          * 0 );
          * obj_to_char( obj, ch );
          * equip_char( ch, obj, WEAR_WIELD );
          */

      }
      else if( ch->in_room != NULL )
      {
         char_to_room( ch, ch->in_room );
      }
      else if( IS_IMMORTAL( ch ) )
      {
         char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
      }
      else
      {
         char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
      }

      /*
       * check for login failures, then clear count. 
       */
      if( ch->pcdata->failures != 0 && ch->level != 1 )
      {
         sprintf( msg, "WARNING:  There have been %d failed login attempts.\n\r", ch->pcdata->failures );
         send_to_char( msg, ch );
         ch->pcdata->failures = 0;
      }

      if( ch->level > 1 )
      {
         sprintf( msg, "\n\rLast successful login from: %s\n\r\n\r", ch->pcdata->host );
         send_to_char( msg, ch );
         if( strcmp( d->host, ch->pcdata->host ) )
         {
            sprintf( msg, "%s connected from %s ( last login was from %s ) !", ch->name, d->host, ch->pcdata->host );
            log_string( msg );
            monitor_chan( msg, MONITOR_CONNECT );
            if( ( ch->level > 80 ) )
            {
               sprintf( msg, "WARNING!!! %s logged in with level %d.\n\r", ch->name, ch->level );
               log_string( msg );
            }

         }
         if( ch->pcdata->host != NULL )
            free_string( ch->pcdata->host );
         ch->pcdata->host = str_dup( d->host );
      }

      /*
       * Check for new notes waiting at the post office 
       */
      notes = 0;

      for( pnote = first_note; pnote; pnote = pnote->next )
         if( is_note_to( ch, pnote ) && str_cmp( ch->name, pnote->sender ) && pnote->date_stamp > ch->last_note )
            notes++;

      if( notes == 1 )
         send_to_char( "\n\rYou have one new letter waiting.\n\r", ch );
      else if( notes > 1 )
      {
         sprintf( buf, "\n\rYou have %d new letters waiting.\n\r", notes );
         send_to_char( buf, ch );
      }



      act( "$n enters " mudnamecolor ".", ch, NULL, NULL, TO_ROOM );

      sprintf( buf, "%s has entered the game.", ch->name );
      monitor_chan( buf, MONITOR_CONNECT );

      if( ( number_range( 0, 99 ) < ( ch->balance / 10000000 ) ) && ( ch->balance > ( get_psuedo_level( ch ) * 100000 ) ) )
      {
         int loss;
         int new_balance;
         loss = number_range( ch->balance * .3, ch->balance * .6 );
         new_balance = UMAX( ch->balance - loss, get_psuedo_level( ch ) * 100000 );
         ch->balance = UMIN( ch->balance, new_balance );
      }


      if( !IS_NPC( ch ) )
      {
         if( !str_cmp( ch->pcdata->room_enter, "" ) )
         {
            switch ( ch->race )
            {

               case 0:
                  ch->pcdata->room_enter = str_dup( "saunters arrogantly in from" );
                  ch->pcdata->room_exit = str_dup( "walks" );
                  break;

               case 1:
                  ch->pcdata->room_enter = str_dup( "sneaks in from" );
                  ch->pcdata->room_exit = str_dup( "disappears" );
                  break;

               case 2:
                  ch->pcdata->room_enter = str_dup( "barges into you from" );
                  ch->pcdata->room_exit = str_dup( "charges" );
                  break;


               case 3:
                  ch->pcdata->room_enter = str_dup( "quietly glides in from" );
                  ch->pcdata->room_exit = str_dup( "glides quietly" );
                  break;

               case 4:
                  ch->pcdata->room_enter = str_dup( "appears in from" );
                  ch->pcdata->room_exit = str_dup( "fades away" );
                  break;

               case 5:
                  ch->pcdata->room_enter = str_dup( "stomps angrily in from" );
                  ch->pcdata->room_exit = str_dup( "angrily stomps" );
                  break;
               case 6:
                  ch->pcdata->room_enter = str_dup( "skitters in from" );
                  ch->pcdata->room_exit = str_dup( "skitters" );
                  break;
               case 7:
                  ch->pcdata->room_enter = str_dup( "slithers in from" );
                  ch->pcdata->room_exit = str_dup( "slithers" );
                  break;
               case 8:
                  ch->pcdata->room_enter = str_dup( "flys at you from" );
                  ch->pcdata->room_exit = str_dup( "flys away" );
                  break;
               case 9:
                  ch->pcdata->room_enter = str_dup( "gallops in from" );
                  ch->pcdata->room_exit = str_dup( "gallops" );
                  break;
               case 10:
                  ch->pcdata->room_enter = str_dup( "marches in from" );
                  ch->pcdata->room_exit = str_dup( "marches" );
                  break;
               case 11:
                  ch->pcdata->room_enter = str_dup( "flitters in from" );
                  ch->pcdata->room_exit = str_dup( "flitters" );
                  break;
               case 12:
                  ch->pcdata->room_enter = str_dup( "boars you down from" );
                  ch->pcdata->room_exit = str_dup( "rushes" );
                  break;
               case 13:
                  ch->pcdata->room_enter = str_dup( "flys at you from" );
                  ch->pcdata->room_exit = str_dup( "flys away" );
                  break;
               default:
                  ch->pcdata->room_enter = str_dup( "moves in from" );
                  ch->pcdata->room_exit = str_dup( "moves" );
                  break;


            }
         }
      }

      if( IS_VAMP( ch ) && !IS_NPC( ch ) && ( ch->pcdata->recall_vnum == 3001 ) )
         ch->pcdata->recall_vnum = 9001;

      if( ch->pcdata->hp_from_gain < 0 )
         reset_gain_stats( ch );
/*       ch->affected_by = 0;   */


      ch->is_quitting = FALSE;
      d->connected = CON_SETTING_STATS;
      {
         OBJ_DATA *wear_object;
         AFFECT_DATA *this_aff;
         AFFECT_DATA *this_aff_next;

         ch->max_mana = ch->pcdata->mana_from_gain;
         ch->max_hit = ch->pcdata->hp_from_gain;
         ch->max_move = ch->pcdata->move_from_gain;
         ch->saving_throw = get_psuedo_level( ch ) / 10;
         ch->hitroll = 0;
         ch->damroll = 0;
         ch->armor = 100;
         if( ch->login_sex != -1 )
            ch->sex = ch->login_sex;
         ch->affected_by = 0;


         for( wear_object = ch->first_carry; wear_object != NULL; wear_object = wear_object->next_in_carry_list )
         {
            if( wear_object->wear_loc > WEAR_NONE )
               equip_char( ch, wear_object, wear_object->wear_loc );
         }
/* add spells saved to stats  */

         for( this_aff = ch->first_saved_aff; this_aff != NULL; this_aff = this_aff_next )
         {
            this_aff_next = this_aff->next;
            UNLINK( this_aff, ch->first_saved_aff, ch->last_saved_aff, next, prev );


            if( this_aff->type == skill_lookup( "fireshield" ) )
               do_cast( ch, "fireshield" );
            else if( this_aff->type == skill_lookup( "iceshield" ) )
               do_cast( ch, "iceshield" );
            else if( this_aff->type == skill_lookup( "shockshield" ) )
               do_cast( ch, "shockshield" );
            else if( this_aff->type == skill_lookup( "shadowshield" ) )
               do_cast( ch, "shadowshield" );
            else if( this_aff->type == skill_lookup( "thoughtshield" ) )
               do_cast( ch, "thoughtshield" );
            else
               affect_to_char( ch, this_aff );
            PUT_FREE( this_aff, affect_free );
         }
      }
      d->connected = CON_PLAYING;

      do_look( ch, "auto" );


      return;
   }

   return;
}



/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
   /*
    * Reserved words.
    */
   if( is_name( name, "all auto everymob localmobs immortal zen self someone" ) )
      return FALSE;

   /*
    * Length restrictions.
    */
   if( strlen( name ) < 3 )
      return FALSE;

   if( strlen( name ) > 12 )
      return FALSE;

   /*
    * Alphanumerics only.
    * Lock out IllIll twits.
    */
   {
      char *pc;
      bool fIll;

      fIll = TRUE;
      for( pc = name; *pc != '\0'; pc++ )
      {
         if( !isalpha( *pc ) )
            return FALSE;
         if( LOWER( *pc ) != 'i' && LOWER( *pc ) != 'l' )
            fIll = FALSE;
      }

      if( fIll )
         return FALSE;
   }

   /*
    * Prevent players from naming themselves after mobs.
    */
   {
      extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
      MOB_INDEX_DATA *pMobIndex;
      int iHash;

      for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
      {
         for( pMobIndex = mob_index_hash[iHash]; pMobIndex != NULL; pMobIndex = pMobIndex->next )
         {
            if( is_name( name, pMobIndex->player_name ) )
               return FALSE;
         }
      }
   }

   return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA * d, char *name, bool fConn )
{
   CHAR_DATA *ch;
   OBJ_DATA *obj;

   for( ch = first_char; ch != NULL; ch = ch->next )
   {
      if( !IS_NPC( ch ) && ( !fConn || ch->desc == NULL ) && !str_cmp( d->character->name, ch->name ) )
      {
         if( fConn == FALSE )
         {
            free_string( d->character->pcdata->pwd );
            d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
         }
         else
         {
            free_char( d->character );
            d->character = ch;
            ch->desc = d;
            ch->timer = 0;
            send_to_char( "Reconnecting.\n\r", ch );
            act( "$n reconnects.", ch, NULL, NULL, TO_ROOM );
            sprintf( log_buf, "%s@%s reconnected.", ch->name, d->host );
            log_string( log_buf );
            monitor_chan( log_buf, MONITOR_CONNECT );
            d->connected = CON_PLAYING;

            /*
             * Contributed by Gene Choi
             */
            if( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) != NULL
                && obj->item_type == ITEM_LIGHT && obj->value[2] != 0 && ch->in_room != NULL )
               ++ch->in_room->light;
         }
         return TRUE;
      }
   }

   return FALSE;
}



/*
 * Check if already playing.
 */
bool check_playing( DESCRIPTOR_DATA * d, char *name )
{
   DESCRIPTOR_DATA *dold;
   char buf[MAX_STRING_LENGTH];

   for( dold = first_desc; dold; dold = dold->next )
   {
      if( dold != d
          && dold->character != NULL
          && dold->connected != CON_GET_NAME
          && dold->connected != CON_GET_OLD_PASSWORD
          && !str_cmp( name, dold->original ? dold->original->name : dold->character->name ) )
      {
         sprintf( buf, "Player from site %s tried to login as %s (already playing) !", d->host, name );
         monitor_chan( buf, MONITOR_CONNECT );
/* Not sure if we want to do this..players can cheat and try to log back in as themselves to end a fight Zen
	    dold->character->position = POS_STANDING;
          if ( dold->character->fighting != NULL )
            dold->character->fighting = NULL;
*/
         d->connected = CON_RECONNECTING;
         do_quit( dold->character, "" );
         write_to_buffer( d, "Already playing. If you were not fighting or dead, you were disconnected\n\rName: ", 0 );
         d->connected = CON_GET_NAME;
         if( d->character != NULL )
         {
            free_char( d->character );
            d->character = NULL;
         }
         return TRUE;
      }
   }

   return FALSE;
}



void stop_idling( CHAR_DATA * ch )
{
   if( ch == NULL
       || ch->desc == NULL
       || ch->desc->connected != CON_PLAYING || ch->was_in_room == NULL || ch->in_room != get_room_index( ROOM_VNUM_LIMBO ) )
      return;

   ch->timer = 0;
   char_from_room( ch );
   char_to_room( ch, ch->was_in_room );
   ch->was_in_room = NULL;
   act( "$n has returned from the void.", ch, NULL, NULL, TO_ROOM );
   return;
}


/*
 * Write to one char.
 */
void send_to_char( const char *txt, CHAR_DATA * ch )
{
   if( ch == NULL )
      return;
   if( txt == NULL || ch->desc == NULL )
      return;
   /*
    * Large leak fixed here.. -- Altrag 
    */
   if( ch->desc->showstr_head != NULL )
   {
      char *ssh;

      ssh = qgetmem( strlen( ch->desc->showstr_head ) + strlen( txt ) + 1 );
      strcpy( ssh, ch->desc->showstr_head );
      strcat( ssh, txt );
      if( ch->desc->showstr_point )
         ch->desc->showstr_point += ( ssh - ch->desc->showstr_head );
      else
         ch->desc->showstr_point = ssh;
      qdispose( ch->desc->showstr_head, strlen( ch->desc->showstr_head ) + 1 );
      ch->desc->showstr_head = ssh;
   }
   else
   {
      ch->desc->showstr_head = qgetmem( strlen( txt ) + 1 );
      strcpy( ch->desc->showstr_head, txt );
      ch->desc->showstr_point = ch->desc->showstr_head;
   }
   if( ch->desc->showstr_point == ch->desc->showstr_head )
      show_string( ch->desc, "" );
   return;
}

/* The heart of the pager.  Thanks to N'Atas-Ha, ThePrincedom
   for porting this SillyMud code for MERC 2.0 and laying down the groundwork.
   Thanks to Blackstar, hopper.cs.uiowa.edu 4000 for which
   the improvements to the pager was modeled from.  - Kahn */
/* Leak fixes.. alloc_mem'd stuff shouldnt be free_string'd. -- Altrag */
/* Spec: buffer overflow fixes, internal buffer sizes increased */

void show_string( struct descriptor_data *d, char *input )
{
   char buffer[MAX_STRING_LENGTH * 2];
   char buf[MAX_INPUT_LENGTH];
   register char *scan, *chk;
   int lines = 0, toggle = 1;
   int space;

   one_argument( input, buf );

   switch ( UPPER( buf[0] ) )
   {
      case '\0':
      case 'C':  /* show next page of text */
         lines = 0;
         break;

      case 'R':  /* refresh current page of text */
         lines = -1 - ( d->character->pcdata->pagelen );
         break;

      case 'B':  /* scroll back a page of text */
         lines = -( 2 * d->character->pcdata->pagelen );
         break;

      case 'H':  /* Show some help */
         write_to_buffer( d, "C, or Return = continue, R = redraw this page,\n\r", 0 );
         write_to_buffer( d, "B = back one page, H = this help, Q or other keys = exit.\n\r\n\r", 0 );
         lines = -1 - ( d->character->pcdata->pagelen );
         break;

      default:   /*otherwise, stop the text viewing */
         if( d->showstr_head )
         {
            qdispose( d->showstr_head, strlen( d->showstr_head ) + 1 );
            d->showstr_head = 0;
         }
         d->showstr_point = 0;
         return;

   }

   /*
    * do any backing up necessary 
    */
   if( lines < 0 )
   {
      for( scan = d->showstr_point; scan > d->showstr_head; scan-- )
         if( ( *scan == '\n' ) || ( *scan == '\r' ) )
         {
            toggle = -toggle;
            if( toggle < 0 )
               if( !( ++lines ) )
                  break;
         }
      d->showstr_point = scan;
   }

   /*
    * show a chunk 
    */
   lines = 0;
   toggle = 1;

   space = MAX_STRING_LENGTH * 2 - 100;
   for( scan = buffer;; scan++, d->showstr_point++ )
   {
      space--;
      if( ( ( *scan = *d->showstr_point ) == '\n' || *scan == '\r' ) && ( toggle = -toggle ) < 0 && space > 0 )
         lines++;
      else
         if( !*scan || ( d->character && !IS_NPC( d->character ) && lines >= d->character->pcdata->pagelen ) || space <= 0 )
      {

         *scan = '\0';
         write_to_buffer( d, buffer, strlen( buffer ) );

         /*
          * See if this is the end (or near the end) of the string 
          */
         for( chk = d->showstr_point; isspace( *chk ); chk++ );
         if( !*chk )
         {
            if( d->showstr_head )
            {
               qdispose( d->showstr_head, strlen( d->showstr_head ) + 1 );
               d->showstr_head = 0;
            }
            d->showstr_point = 0;
         }
         return;
      }
   }

   return;
}

/*
 * The primary output interface for formatted output.
 */
void act( const char *format, CHAR_DATA * ch, const void *arg1, const void *arg2, int type )
{
   static char *const he_she[] = { "it", "he", "she" };
   static char *const him_her[] = { "it", "him", "her" };
   static char *const his_her[] = { "its", "his", "her" };

   char buf[MAX_STRING_LENGTH];
   char fname[MAX_INPUT_LENGTH];
   char tmp_str[MSL];
   CHAR_DATA *to;
   CHAR_DATA *vch = ( CHAR_DATA * ) arg2;
   OBJ_DATA *obj1 = ( OBJ_DATA * ) arg1;
   OBJ_DATA *obj2 = ( OBJ_DATA * ) arg2;
   const char *str;
   const char *i = "";
   char *point;
   bool do_crlf = TRUE;
   bool can_see_message = TRUE;

   /*
    * Discard null and zero-length messages.
    */
   if( format == NULL || format[0] == '\0' )
      return;

   if( ch == NULL )

      return;

   if( ( ch->is_free != FALSE ) || ( ch->in_room == NULL ) )
   {
      bugf( "bad ch, string=%s", format );
      return;
   }

   to = ch->in_room->first_person;
   if( type == TO_VICT )
   {
      if( vch == NULL )
      {
         bugf( "Act: null vch with TO_VICT, string=%s", format );
         return;
      }
      to = vch->in_room->first_person;
   }

   for( ; to != NULL; to = to->next_in_room )
   {
      if( ( to->desc == NULL && ( IS_NPC( to ) && !( to->pIndexData->progtypes & ACT_PROG ) ) ) || !IS_AWAKE( to ) )
         continue;

      if( type == TO_CHAR && to != ch )
         continue;
      if( type == TO_VICT && ( to != vch || to == ch ) )
         continue;
      if( type == TO_ROOM && to == ch )
         continue;
      if( type == TO_NOTVICT && ( to == ch || to == vch ) )
         continue;

      /*
       * Check for Bash and vannevar *sigh* 
       */
      if( !IS_NPC( ch ) && !IS_NPC( ch ) && !str_cmp( ch->name, "bash" ) && !str_cmp( to->name, "vannevar" ) )
         continue;
      if( !IS_NPC( ch ) && !IS_NPC( ch ) && !str_cmp( ch->name, "vannevar" ) && !str_cmp( to->name, "bash" ) )
         continue;


      point = buf;
      str = format;
      while( *str != '\0' )
      {
         if( *str != '$' )
         {
            *point++ = *str++;
            continue;
         }
         ++str;

         if( arg2 == NULL && *str >= 'M' && *str <= 'N' )
         {
            bugf( "Act: missing arg2 for code $%c, string=%s", *str, format );
            i = " !!!!! ";
         }

         else if( arg2 == NULL && *str >= 'S' && *str <= 'T' )
         {
            bugf( "Act: missing arg2 for code $%c, string=%s", *str, format );
            i = " !!!!! ";
         }
         else if( arg2 == NULL && *str == 'E' )
         {
            bugf( "Act: missing arg2 for code $%c, string=%s", *str, format );
            i = " !!!!! ";
         }
         else
         {
            switch ( *str )
            {
               default:
                  bugf( "Act: bad code $%c, string=%s", *str, format );
                  i = " !!!!! ";
                  break;
                  /*
                   * Thx alex for 't' idea 
                   */
               case 'L':
                  can_see_message = TRUE;
                  if( IS_IMMORTAL( to ) )
                  {
                     if( IS_SET( ch->act, PLR_WIZINVIS ) && ch->invis > get_trust( to )
                         /*
                          * || ( IS_SET( ch->act, PLR_INCOG )
                          * && ch->incog > get_trust( to )
                          */  )
                        can_see_message = FALSE;
                  }
                  else
                  {
                     if( IS_SET( ch->act, PLR_WIZINVIS ) && get_trust( to ) < ch->invis )
                        can_see_message = FALSE;
                     if( ( IS_AFFECTED( ch, AFF_SNEAK ) || item_has_apply( ch, ITEM_APPLY_SNEAK ) )
                         && ( ( get_psuedo_level( ch ) - 20 + number_range( 1, 30 ) ) > get_psuedo_level( to ) ) )
                        can_see_message = FALSE;
                  }
                  break;
               case 't':
                  i = ( char * )arg1;
                  break;
               case 'T':
                  i = ( char * )arg2;
                  break;
               case 'n':
                  i = PERS( ch, to );
                  break;
               case 'N':
                  i = PERS( vch, to );
                  break;
               case 'e':
                  i = he_she[URANGE( 0, ch->sex, 2 )];
                  break;
               case 'E':
                  i = he_she[URANGE( 0, vch->sex, 2 )];
                  break;
               case 'm':
                  i = him_her[URANGE( 0, ch->sex, 2 )];
                  break;
               case 'M':
                  i = him_her[URANGE( 0, vch->sex, 2 )];
                  break;
               case 's':
                  i = his_her[URANGE( 0, ch->sex, 2 )];
                  break;
               case 'S':
                  i = his_her[URANGE( 0, vch->sex, 2 )];
                  break;
               case 'k':
                  one_argument( ch->name, tmp_str );
                  i = ( char * )tmp_str;
                  break;
               case 'K':
                  one_argument( vch->name, tmp_str );
                  i = ( char * )tmp_str;
                  break;

               case 'p':
                  if( obj1 )
                  {
                     i = can_see_obj( to, obj1 ) ? obj1->short_descr : "something";
                  }
                  break;

               case 'P':
                  if( obj2 )
                  {
                     i = can_see_obj( to, obj2 ) ? obj2->short_descr : "something";
                  }
                  break;

               case 'd':
                  if( arg2 == NULL || ( ( char * )arg2 )[0] == '\0' )
                  {
                     i = "door";
                  }
                  else
                  {
                     one_argument( ( char * )arg2, fname );
                     i = fname;
                  }
                  break;
            }
            if( i[0] == '^' )
            {
               ++i;
            }

         }
         ++str;
         while( ( *point = *i ) != '\0' )
            ++point, ++i;
      }

      if( do_crlf )
      {
         *point++ = '\n';
         *point++ = '\r';
      }
      buf[0] = UPPER( buf[0] );
      *point = '\0';
      if( to->desc && can_see_message )
         write_to_buffer( to->desc, buf, point - buf );
      if( MOBtrigger )
         mprog_act_trigger( buf, to, ch, obj1, vch );
      /*
       * Added by Kahn 
       */
   }

   MOBtrigger = TRUE;
   return;
}

void do_finger( CHAR_DATA * ch, char *argument )
{
   CHAR_DATA *victim;
   char name[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   bool found = FALSE;
   DESCRIPTOR_DATA d;
   DESCRIPTOR_DATA *this_d;


   argument = one_argument( argument, name );

   for( this_d = first_desc; this_d != NULL; this_d = this_d->next )
   {
      if( ( this_d->connected > 0 ) && !str_cmp( this_d->character->name, name ) )
      {
         do_whois( ch, name );
         return;
      }
   }

   found = load_char_obj( &d, name, TRUE );

   if( !found )
   {
      sprintf( buf, "No pFile found for '%s'.\n\r", capitalize( name ) );
      send_to_char( buf, ch );
      return;
   }

   victim = d.character;
   d.character = NULL;
   victim->desc = NULL;

   sprintf( buf, "Name: %s.\n\r", capitalize( victim->name ) );
   send_to_char( buf, ch );

   sprintf( buf, "Last Login was from: %s.\n\r", victim->pcdata->host );
   send_to_char( buf, ch );

   sprintf( buf, "pFile was last saved at: %s.", victim->pcdata->lastlogin );
   send_to_char( buf, ch );

   free_char( victim );

   return;
}

void send_to_descrips( const char *message )
{
   return;
}

/* Here it is boys and girls the HOT reboot function and all its nifty  * little parts!! - Flar
 */
void do_hotreboo( CHAR_DATA * ch, char *argument )
{
   send_to_char( "If you want to do a @@R@@fHOT@@Breboot@@N....spell it out.\n\r", ch );
   return;
}

extern int port, control;  /* db.c */

void do_hotreboot( CHAR_DATA * ch, char *argument )
{
   FILE *fp;
   DESCRIPTOR_DATA *d, *d_next;
   char buf[100], buf2[100], buf3[100];

   fp = fopen( COPYOVER_FILE, "w" );

   if( !fp )
   {
      send_to_char( "Copyover file not writeable, aborted.\n\r", ch );
      log_f( "Could not write to copyover file: %s", COPYOVER_FILE );
      perror( "do_copyover:fopen" );
      return;
   }


   if( auction_item != NULL )
      do_auction( ch, "stop" );

   sprintf( buf,
            "\n\r**** HOTreboot by An Immortal - Please remain ONLINE ****\n\r*********** We will be back in 30 seconds!! *************%s\n\n\r",
            "" );

   /*
    * For each PLAYING descriptor( non-negative ), save its state 
    */
   for( d = first_desc; d; d = d_next )
   {
      CHAR_DATA *och = CH( d );
      d_next = d->next; /* We delete from the list , so need to save this */

      if( !d->character || d->connected < 0 )   /* drop those logging on */
      {
         write_to_descriptor( d->descriptor, "\n\r@Sorry, ACK! MUD rebooting. Come back in a few minutes.\n\r", 0 );
         close_socket( d );   /* throw'em out */
      }
      else
      {
         fprintf( fp, "%d %s %s\n", d->descriptor, och->name, d->host );
         if( och->level == 1 )
         {

            write_to_descriptor( d->descriptor,
                                 "Since you are level one, and level one characters do not save....you have been advanced!\n\r",
                                 0 );
            och->level = 2;
            och->lvl[och->class] = 2;
         }
         save_char_obj( och );
         write_to_descriptor( d->descriptor, buf, 0 );
      }
   }

   fprintf( fp, "-1\n" );
   fclose( fp );

   /*
    * Close reserve and other always-open files and release other resources 
    */

   fclose( fpReserve );

#ifdef IMC
   imc_hotboot(  );
#endif

   /*
    * exec - descriptors are inherited 
    */

   sprintf( buf, "%d", port );
   sprintf( buf2, "%d", control );
#ifdef IMC
   if( this_imcmud )
      snprintf( buf3, 100, "%d", this_imcmud->desc );
   else
      strncpy( buf3, "-1", 100 );
#else
   strncpy( buf3, "-1", 100 );
#endif

   /*
    * spec: handle profiling cleanly here 
    */
#ifdef PROF
   if( !fork(  ) )   /* dump profile info */
      exit( 0 );
   signal( SIGPROF, SIG_IGN );
#endif

   execl( EXE_FILE, "ACK! MUD", buf, "HOTreboot", buf2, buf3, ( char * )NULL );

   /*
    * Failed - sucessful exec will not return 
    */

   perror( "do_copyover: execl" );
   send_to_char( "HOTreboot FAILED! Something is wrong in the shell!\n\r", ch );
}

/* Recover from a copyover - load players */
void copyover_recover(  )
{
   DESCRIPTOR_DATA *d;
   FILE *fp;
   char name[100];
   char host[MSL];
   int desc;
   bool fOld;
   extern bool disable_timer_abort;
   log_f( "Copyover recovery initiated" );
   disable_timer_abort = TRUE;
   fp = fopen( COPYOVER_FILE, "r" );

   if( !fp )   /* there are some descriptors open which will hang forever then ? */
   {
      perror( "copyover_recover:fopen" );
      log_f( "Copyover file not found. Exitting.\n\r" );
      exit( 1 );
   }

   unlink( COPYOVER_FILE );   /* In case something crashes - doesn't prevent reading   */

   for( ;; )
   {
      fscanf( fp, "%d %s %s\n", &desc, name, host );
      if( desc == -1 )
         break;

      /*
       * Write something, and check if it goes error-free 
       */
      if( !write_to_descriptor( desc, "\n\rRestoring from HOTreboot...\n\r", 0 ) )
      {
         close( desc ); /* nope */
         continue;
      }

      GET_FREE( d, desc_free );
      init_descriptor( d, desc );   /* set up various stuff */

      d->host = str_dup( host );
      d->next = NULL;
      d->prev = NULL;

      d->connected = CON_COPYOVER_RECOVER;   /* -15, so close_socket frees the char */
      LINK( d, first_desc, last_desc, next, prev );

      /*
       * Now, find the pfile 
       */

      fOld = load_char_obj( d, name, FALSE );

      if( !fOld ) /* Player file not found?! */
      {
         write_to_descriptor( desc, "\n\rSomehow, your character was lost in the HOTreboot. Sorry, you must relog in.\n\r",
                              0 );
         close_socket( d );
      }
      else  /* ok! */
      {
         CHAR_DATA *this_char;


         write_to_descriptor( desc, "\n\rCopyover recovery complete.\n\r", 0 );

         /*
          * Just In Case 
          */
         if( !d->character->in_room )
            d->character->in_room = get_room_index( ROOM_VNUM_LIMBO );

         /*
          * Insert in the char_list 
          */
         d->character->next = NULL;
         d->character->prev = NULL;
         this_char = d->character;

         LINK( this_char, first_char, last_char, next, prev );

         char_to_room( d->character, d->character->in_room );
         if( d->character->position == POS_RIDING )
            d->character->position = POS_STANDING;
         do_look( d->character, "" );
         act( "$n's atoms materialize and reform.", d->character, NULL, NULL, TO_ROOM );
         /*
          * d->connected = CON_PLAYING;  
          */

         if( this_char->pcdata->hp_from_gain < 0 )
            reset_gain_stats( this_char );
/*       this_char->affected_by = 0;   */


         this_char->is_quitting = FALSE;
         d->connected = CON_SETTING_STATS;
         {
            OBJ_DATA *wear_object;
            AFFECT_DATA *this_aff;
            AFFECT_DATA *this_aff_next;

            this_char->max_mana = this_char->pcdata->mana_from_gain;
            this_char->max_hit = this_char->pcdata->hp_from_gain;
            this_char->max_move = this_char->pcdata->move_from_gain;
            this_char->saving_throw = get_psuedo_level( this_char ) / 10;
            this_char->hitroll = 0;
            this_char->damroll = 0;
            this_char->armor = 100;
            if( this_char->login_sex != -1 )
               this_char->sex = this_char->login_sex;
            this_char->affected_by = 0;



            for( wear_object = this_char->first_carry; wear_object != NULL; wear_object = wear_object->next_in_carry_list )
            {
               if( wear_object->wear_loc > WEAR_NONE )
                  equip_char( this_char, wear_object, wear_object->wear_loc );
            }
/* add spells saved to stats  */

            for( this_aff = this_char->first_saved_aff; this_aff != NULL; this_aff = this_aff_next )
            {
               this_aff_next = this_aff->next;
               UNLINK( this_aff, this_char->first_saved_aff, this_char->last_saved_aff, next, prev );


               if( this_aff->type == skill_lookup( "fireshield" ) )
                  do_cast( this_char, "fireshield" );
               else if( this_aff->type == skill_lookup( "iceshield" ) )
                  do_cast( this_char, "iceshield" );
               else if( this_aff->type == skill_lookup( "shockshield" ) )
                  do_cast( this_char, "shockshield" );
               else if( this_aff->type == skill_lookup( "shadowshield" ) )
                  do_cast( this_char, "shadowshield" );
               else if( this_aff->type == skill_lookup( "thoughtshield" ) )
                  do_cast( this_char, "thoughtshield" );
               else
                  affect_to_char( this_char, this_aff );
               PUT_FREE( this_aff, affect_free );
            }
         }
         d->connected = CON_PLAYING;


      }

   }

   fclose( fp );
   disable_timer_abort = FALSE;
}


void hang( const char *str )
{
   bug( str, 0 );
   kill( getpid(  ), SIGQUIT );
}
