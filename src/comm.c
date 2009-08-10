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
/***************************************************************************
 * _/_/_/_/  _/    _/  _/_/_/_/ _/_/_/_/ AckFUSS is modified ACK!MUD 4.3.1 *
 * _/        _/    _/  _/       _/       copyright Matt Goff (Kline) 2008  *
 * _/_/      _/    _/  _/_/_/_/ _/_/_/_/                                   *
 * _/        _/    _/        _/       _/ Support for this code is provided *
 * _/        _/_/_/_/  _/_/_/_/ _/_/_/_/ at www.ackmud.net -- check it out!*
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

#include "h/globals.h"

#ifndef DEC_ACT_COMM_H
#include "h/act_comm.h"
#endif

#ifndef DEC_ACT_INFO_H
#include "h/act_info.h"
#endif

#ifndef DEC_ACT_OBJ_H
#include "h/act_obj.h"
#endif

#ifndef DEC_ACT_WIZ_H
#include "h/act_wiz.h"
#endif

#ifndef DEC_COMM_H
#include "h/comm.h"
#endif

#ifndef DEC_CURSOR_H
#include "h/cursor.h"
#endif

#ifndef DEC_DB_H
#include "h/db.h"
#endif

#ifndef DEC_HANDLER_H
#include "h/handler.h"
#endif

#ifndef DEC_HUNT_H
#include "h/hunt.h"
#endif

#ifndef DEC_INTERP_H
#include "h/interp.h"
#endif

#ifndef DEC_MACROS_H
#include "h/macros.h"
#endif

#ifndef DEC_MAGIC_H
#include "h/magic.h"
#endif

#ifndef DEC_MONEY_H
#include "h/money.h"
#endif

#ifndef DEC_MUDINFO_H
#include "h/mudinfo.h"
#endif

#ifndef DEC_SAVE_H
#include "h/save.h"
#endif

#ifndef DEC_SSM_H
#include "h/ssm.h"
#endif

#ifndef DEC_TELOPT_H
#include "h/telopt.h"
#endif

#ifndef DEC_UPDATE_H
#include "h/update.h"
#endif

const char echo_off_str[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const char echo_on_str[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const char go_ahead_str[] = { IAC, GA, '\0' };

/*
 * Global variables.
 */
DESCRIPTOR_DATA *d_next;   /* Next descriptor in loop      */
FILE *fpReserve;  /* Reserved file handle         */
bool god;   /* All new chars are gods!      */
bool merc_down;   /* Shutdown                     */
bool wizlock;  /* Game is wizlocked            */
extern bool deathmatch;  /* Deathmatch happening?        */
extern bool disable_timer_abort;
char str_boot_time[MAX_INPUT_LENGTH];
int int_boot_time;
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
short quest_personality;  /* mob's crusade personality :) */

/* Some debug globals --Kline */
int free_get = 0;
int free_put = 0;

/* Zen mod: Diplomatics globals */

POL_DATA politics_data;

COUNCIL_DATA super_councils[MAX_SUPER];   /* for supernatural council meetings  */

/* Llolth added for consider */
char hr[MAX_STRING_LENGTH];
char dr[MAX_STRING_LENGTH];

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
   int_boot_time = now_time.tv_sec;

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
   }

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
   snprintf( log_buf, (2 * MIL), "%s is ready on port %d.", VERS_STRING, port );
   log_string( log_buf );
   log_string("Last compiled on " __DATE__ " at " __TIME__ ".");
#ifdef IMC
   imc_startup( FALSE, imcsocket, fCopyOver );
#endif
   if( fCopyOver )
   {
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
   clear_lists();
   exit( 0 );
   return 0;
}

int init_socket( int sock_port )
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

#if defined(SO_DONTLINGER)
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
   sa.sin_port = htons( sock_port );

   /* Uncomment the below if your server requires IP_Binding
    * and fill it in with the address they give you --Kline
    */
   /* sa.sin_addr.s_addr = inet_addr("127.0.0.1"); */

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

void game_loop( int game_control )
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
         close( game_control );
         game_control = init_socket( global_port );
         reopen_flag = 0;
      }

      /*
       * Poll all active descriptors.
       */
      FD_ZERO( &in_set );
      FD_ZERO( &out_set );
      FD_ZERO( &exc_set );
      FD_SET( game_control, &in_set );
      maxdesc = game_control;

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
      if( FD_ISSET( game_control, &in_set ) )
         new_descriptor( game_control );

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
            write_to_descriptor( d->descriptor, "Login timeout (180s)\r\n" );
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
            out_file = file_open( PLAYERNUM_FILE, "a" );
            fprintf( out_file, "%i %i %i\n", now_bd_time->tm_mday, cur_hour, max_players );
            file_close( out_file );
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

void *lookup_address( void *input )
{
 struct hostent *from = 0;
 struct hostent ent;
 char buf[16384];
 int err;
 LOOKUP_DATA *ld = static_cast<LOOKUP_DATA *>(input);

 gethostbyaddr_r( ld->buf, sizeof(ld->buf), AF_INET, &ent, buf, 16384, &from, &err);

 if( from && from->h_name )
 {
  free_string(ld->d->host);
  ld->d->host = str_dup(from->h_name);
  snprintf( log_buf, (2 * MIL), "Hostname resolved to: %s.", from->h_name );
  monitor_chan( log_buf, MONITOR_CONNECT );
 }
 else
  monitor_chan( "Unable to resolve hostname.", MONITOR_CONNECT );

 delete ld;
 pthread_exit(0);
}

void new_descriptor( int d_control )
{
   static DESCRIPTOR_DATA d_zero;
   char buf[MSL];
   DESCRIPTOR_DATA *dnew;
   /*
    * BAN_DATA *pban;   
    */
   struct sockaddr_in sock;
   size_t desc;
   socklen_t size;

   size = sizeof( sock );
   getsockname( d_control, ( struct sockaddr * )&sock, &size );
   if( ( desc = accept( d_control, ( struct sockaddr * )&sock, &size ) ) < 0 )
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
   dnew = new DESCRIPTOR_DATA;
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
      pthread_t lookup_thread;
      pthread_attr_t attr;

      addr = ntohl( sock.sin_addr.s_addr );
      snprintf( buf, MSL, "%d.%d.%d.%d", ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF, ( addr >> 8 ) & 0xFF, ( addr ) & 0xFF );
      snprintf( log_buf, (2 * MIL), "Sock.sinaddr:  %s (%d)", buf, ntohs( sock.sin_port ) );
      log_string( log_buf );
      snprintf( log_buf, (2 * MIL), "Connection formed from %s.", buf );
      monitor_chan( log_buf, MONITOR_CONNECT );

      dnew->remote_port = ntohs( sock.sin_port );
      dnew->host = str_dup( buf );

      LOOKUP_DATA *ld = new LOOKUP_DATA;
      ld->d = dnew;
      ld->buf = str_dup((char *)&sock.sin_addr);
      pthread_attr_init(&attr);
      pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
      pthread_create(&lookup_thread,&attr,&lookup_address,(void *)ld);
   }

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
      FILE *fp;

      send_telopts(dnew);

      snprintf( buf, MSL, "%s/g/greeting%d.%s", HELP_DIR, number_range(0,MAX_GREETING), HELP_MORT );

      if( (fp = file_open(buf,"r")) != NULL )
       while( fgets(buf,MAX_STRING_LENGTH,fp) )
        write_to_buffer(dnew,buf);
      else
       snprintf(buf,MSL,"Please enter your name:");

      file_close(fp);
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
   dnew->outbuf = (char *)getmem( dnew->outsize );
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
      write_to_buffer( dclose->snoop_by, "Your victim has left the game.\r\n" );
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
      snprintf( log_buf, (2 * MIL), "Closing link to %s.", ch->name.c_str() );
      log_string( log_buf );
      monitor_chan( log_buf, MONITOR_CONNECT );
      if( dclose->connected == CON_PLAYING )
      {
         act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM );
         ch->desc = NULL;
      }
      else
      {
         delete dclose->character;
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
      free(dclose->showstr_head);

   delete dclose;

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
   if( iStart >= (int)sizeof( d->inbuf ) - 10 )
   {
      snprintf( log_buf, (2 * MIL), "%s input overflow!", d->host );
      log_string( log_buf );
      snprintf( log_buf, (2 * MIL), "input overflow by %s (%s)", ( d->character == NULL ) ? "[login]" : d->character->name.c_str(), d->host );
      monitor_chan( log_buf, MONITOR_CONNECT );
      write_to_descriptor( d->descriptor, "\r\n SPAMMING IS RUDE, BYE BYE! \r\n" );
      return FALSE;
   }

   /*
    * Snarf input. 
    */
   for( ;; )
   {
      char tmp[MSL];
      int nRead;

      nRead = read( d->descriptor, tmp, sizeof( tmp ) - 10 - iStart );
      if( nRead > 0 )
      {
         iStart += telopt_handler(d,tmp,nRead,(d->inbuf + iStart));
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
         write_to_descriptor( d->descriptor, "Line too long.\r\n" );

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
               snprintf( log_buf, (2 * MIL), "%s input spamming!", d->character->name.c_str() );
               log_string( log_buf );
               monitor_chan( log_buf, MONITOR_CONNECT );
            }
            write_to_descriptor( d->descriptor, "\r\n***** SHUT UP!! *****\r\n" );
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

   /*
    * Bust a prompt.
    */
   if( fPrompt && !merc_down && d->connected == CON_PLAYING )
   {
      if( d->showstr_point )
         write_to_buffer( d, "[Please type (c)ontinue, (r)efresh, (b)ack, (h)elp, (q)uit, or RETURN]:  " );
      else
      {
         CHAR_DATA *ch;

         ch = d->original ? d->original : d->character;
         if( !IS_NPC(ch) && ch->act.test(ACT_BLANK) && (ch->pcdata->movement <= MAX_MOVE_DISPLAY || !ch->act.test(ACT_AUTOBRIEF)) )
            write_to_buffer( d, "\r\n", 2 );
         if( ch->hunting || ch->hunt_obj )
            char_hunt( ch );
         if( !IS_NPC(ch) && (ch->pcdata->movement <= MAX_MOVE_DISPLAY || !ch->act.test(ACT_AUTOBRIEF)) )
          bust_a_prompt( d );
         if( ch->act.test(ACT_TELNET_GA) )
            write_to_buffer( d, go_ahead_str );
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
         snprintf( foo, MSL, "[SNOOP:%s] ", snoop_ch->name.c_str() );
      write_to_buffer( d->snoop_by, foo );
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
   char buf[MSL];
   char buf2[MSL];
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
      write_to_buffer( d, ">" );
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
      char msg[MSL];  /* Mode */
      char msg2[MSL]; /* what */
      char msg3[MSL]; /* all */

      if( ch->pcdata->build_mode == BUILD_MODE_NONE )
      {
         snprintf( msg, MSL, "Mode:  None" );
         snprintf( msg2, MSL, "Use Redit, Medit or Oedit to select mode." );
      }

      if( ch->pcdata->build_mode == BUILD_MODE_REDIT )
      {
         snprintf( msg, MSL, "Mode: Redit" );
         if( ch->pcdata->build_vnum < 1 )
            snprintf( msg2, MSL, "No vnum is set." );
         else
         {
            room = get_room_index( ch->pcdata->build_vnum );
            if( room != NULL )
               snprintf( msg2, MSL, "[%5d]: %s", ch->pcdata->build_vnum, room->name );
         }
      }

      if( ch->pcdata->build_mode == BUILD_MODE_OEDIT )
      {
         snprintf( msg, MSL, "Mode: Oedit" );
         if( ch->pcdata->build_vnum < 1 )
            snprintf( msg2, MSL, "No vnum set." );
         else
         {
            obj = get_obj_index( ch->pcdata->build_vnum );
            if( obj != NULL )
               snprintf( msg2, MSL, "[%5d]: %s", ch->pcdata->build_vnum, obj->short_descr );
         }
      }


      if( ch->pcdata->build_mode == BUILD_MODE_MEDIT )
      {
         snprintf( msg, MSL, "Mode: Medit" );
         if( ch->pcdata->build_vnum < 1 )
            snprintf( msg2, MSL, "No vnum set." );
         else
         {
            mob = get_mob_index( ch->pcdata->build_vnum );
            if( mob != NULL )
               snprintf( msg2, MSL, "[%5d]: %s", ch->pcdata->build_vnum, mob->short_descr );
         }
      }
      snprintf( msg3, MSL, "< %s %s >", msg, msg2 );
      write_to_buffer( d, msg3 );
      return;
   }

   /*
    * set default prompt to ">" -Uni 
    */
   if( !ch->act.test(ACT_PROMPT) )
   {
      snprintf( buf2, MSL, "\r" );
      if( ch->hit < ch->max_hit )
      {
         check = 1;
         snprintf( buf, MSL, "@@W<@@e%d@@RH", ch->hit );
         strncat( buf2, buf, MSL-1 );
      }
      if( ch->mana < ch->max_mana )
      {
         if( check == 1 )
            snprintf( buf, MSL, " @@l%d@@BM", ch->mana );
         else
            snprintf( buf, MSL, "@@W<@@l%d@@BMn", ch->mana );
         check = 1;
         strncat( buf2, buf, MSL-1 );
      }
      if( ch->move < ch->max_move )
      {
         if( check == 1 )
            snprintf( buf, MSL, " @@r%d@@GV", ch->move );
         else
            snprintf( buf, MSL, "@@W<@@r%d@@GV", ch->move );
         strncat( buf2, buf, MSL-1 );
         check = 1;
      }
      if( check == 1 )
         send_to_char( buf2, ch );

      send_to_char( "@@W>@@N ", ch );
      return;
   }

   if( ch->prompt.empty() )
   {
      send_to_char( "@@N> ", ch );
      return;
   }

   point = buf;
   str = d->original != NULL ? d->original->prompt.c_str() : d->character->prompt.c_str();
   snprintf( buf2, MSL, "%s", "@@N" );
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
         case 'O':
            if( ch->cooldown[COOLDOWN_OFF] > 0 )
             snprintf( buf2, MSL, "%0.2f", ch->cooldown[COOLDOWN_OFF] );
            else
             snprintf( buf2, MSL, "N/A" );
            break;
         case 'D':
            if( ch->cooldown[COOLDOWN_DEF] > 0 )
             snprintf( buf2, MSL, "%0.2f", ch->cooldown[COOLDOWN_DEF] );
            else
             snprintf( buf2, MSL, "N/A" );
            break;
         case 'C':
            if( IS_CASTING(ch) )
             snprintf( buf2, MSL, "%0.2f", ch->casting->time );
            else
             snprintf( buf2, MSL, "N/A" );
            i = buf2;
            break;
         case 'w':
         {
            short cl_index = -1;
            int cost;
            bool remort = FALSE, adept = FALSE;
            ++str;
            switch ( *str )
            {
               default:
                  snprintf( buf2, MSL, "%s", "BadClass" );
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
                     snprintf( buf2, MSL, "%s", "BadClass" );
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
                     snprintf( buf2, MSL, "%s", "BadClass" );
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
               cost = exp_to_level( ch, cl_index, ch->pcdata->order[cl_index] );
            snprintf( buf2, MSL, "%d", UMAX( 0, cost - ch->exp ) );
            i = buf2;
            break;
         }
         case '!':
            snprintf( buf2, MSL, "%d", GET_HITROLL( ch ) );
            i = buf2;
            break;
         case '+':
            snprintf( buf2, MSL, "%d", GET_DAMROLL( ch ) );
            i = buf2;
            break;
         case '*':
            snprintf( buf2, MSL, "%d", GET_AC( ch ) );
            i = buf2;
            break;
         case 'h':
            snprintf( buf2, MSL, "%d", ch->hit );
            i = buf2;
            break;
         case 'H':
            snprintf( buf2, MSL, "%d", ch->max_hit );
            i = buf2;
            break;
         case 'I':
            snprintf( buf2, MSL, ".0%f%%", (float)((ch->hit / ch->max_hit) * 100) );
            i = buf2;
            break;
         case 'm':
            snprintf( buf2, MSL, "%d", ch->mana );
            i = buf2;
            break;
         case 'M':
            snprintf( buf2, MSL, "%d", ch->max_mana );
            i = buf2;
            break;
         case 'J':
            snprintf( buf2, MSL, "%.0f%%", (float)((ch->mana / ch->max_mana) * 100) );
            i = buf2;
            break;
         case 'v':
            snprintf( buf2, MSL, "%d", ch->move );
            i = buf2;
            break;
         case 'V':
            snprintf( buf2, MSL, "%d", ch->max_move );
            i = buf2;
            break;
         case 'K':
            snprintf( buf2, MSL, "%.0f%%", (float)((ch->move / ch->max_move) * 100) );
            i = buf2;
            break;
         case 'x':
            snprintf( buf2, MSL, "%d", ch->exp );
            i = buf2;
            break;
         case 'g':
            snprintf( buf2, MSL, "%d", money_value(ch->money) );
            i = buf2;
            break;
         case 'a':
            if( ch->level < 5 )
               snprintf( buf2, MSL, "%d", ch->alignment );
            else
               snprintf( buf2, MSL, "%s", IS_GOOD( ch ) ? "good" : IS_EVIL( ch ) ? "evil" : "neutral" );
            i = buf2;
            break;
         case 'r':
            if( ch->in_room != NULL )
               snprintf( buf2, MSL, "%s", ch->in_room->name );
            else
               snprintf( buf2, MSL, " " );
            i = buf2;
            break;
         case 'R':
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               snprintf( buf2, MSL, "%d", ch->in_room->vnum );
            else
               snprintf( buf2, MSL, " " );
            i = buf2;
            break;
         case 's':
            if( !IS_NPC( ch ) )
               snprintf( buf2, MSL, "%s", stance_app[ch->stance].name );
            else
               snprintf( buf2, MSL, " " );
            i = buf2;
            break;
         case 'z':
            if( IS_IMMORTAL( ch ) && ch->in_room != NULL )
               snprintf( buf2, MSL, "%s", ch->in_room->area->name );
            else
               snprintf( buf2, MSL, " " );
            i = buf2;
            break;
         case '%':
            snprintf( buf2, MSL, "%%" );
            i = buf2;
            break;
         case 'b':
            if( !IS_NPC( ch ) && IS_VAMP( ch ) )
               snprintf( buf2, MSL, "@@e%d@@N", ch->pcdata->super->energy );
            else
               snprintf( buf2, MSL, " " );
            i = buf2;
            break;
         case 'B':
            if( !IS_NPC( ch ) && IS_VAMP( ch ) )
               snprintf( buf2, MSL, "@@e%d@@N", ch->pcdata->super->energy_max );
            else
               snprintf( buf2, MSL, " " );
            i = buf2;
            break;
         case 'c':
            if( !IS_NPC( ch ) )
               snprintf( buf2, MSL, "\r\n" );
            else
               snprintf( buf2, MSL, " " );
            i = buf2;
            break;
         case 'i':
            if( IS_NPC( ch ) )
               break;
            if( IS_IMMORTAL( ch ) )
               snprintf( buf2, MSL, "INVIS: %d", ch->act.test(ACT_WIZINVIS) ? ch->pcdata->invis : 0 );
            else
            {
               if( ( IS_AFFECTED( ch, AFF_INVISIBLE ) )
                   || ( IS_AFFECTED( ch, AFF_HIDE ) )
                   || ( item_has_apply( ch, ITEM_APPLY_INV ) ) || ( item_has_apply( ch, ITEM_APPLY_HIDE ) ) )
               {
                  snprintf( buf2, MSL, "%s", "INVIS" );
               }
               else
               {
                  snprintf( buf2, MSL, "%s", "VIS" );
               }
            }
            i = buf2;
            break;
         case 't':
            if( !IS_NPC( ch ) )
               snprintf( buf2, MSL, "%d %s", ( time_info.hour % 12 == 0 ) ? 12 : time_info.hour % 12,
                        time_info.hour >= 12 ? "pm" : "am" );
            else
               snprintf( buf2, MSL, " " );
            i = buf2;
            break;
         case 'F':
            if( !IS_NPC( ch ) && IS_WOLF( ch ) )
            {
               if( IS_RAGED( ch ) )
                  snprintf( buf2, MSL, "%s", "GAROU" );
               else if( IS_SHIFTED( ch ) )
                  snprintf( buf2, MSL, "%s", "LUPIS" );
               else
                  snprintf( buf2, MSL, "%s", "HUMANUS" );
            }

            else
               snprintf( buf2, MSL, " " );
            i = buf2;
            break;
         case 'p':
            if( !IS_NPC( ch ) )
            {
               if( weather_info.moon_loc == MOON_DOWN )
                  snprintf( buf2, MSL, "%s", "DOWN" );
               else if( weather_info.moon_loc == MOON_PEAK )
                  snprintf( buf2, MSL, "PEAK:%s", get_moon_phase_name(  ) );
               else
                  snprintf( buf2, MSL, "LOW:%s", get_moon_phase_name(  ) );
            }
            else
               snprintf( buf2, MSL, " " );
            i = buf2;
            break;
         case 'Q':
            if( !IS_NPC(ch) )
            {
               if( ch->pcdata->quest_info->wait_time > 0 )
                  snprintf( buf2, MSL, "%2d", ch->pcdata->quest_info->wait_time );
               else
                  snprintf( buf2, MSL, "OK" );
            }
            else
               snprintf( buf2, MSL, "Mquest Error.\r\n" );
            i = buf2; 
            break;


      }
      ++str;
      while( ( *point = *i ) != '\0' )
         ++point, ++i;
   }

   /*
    * Also have part of battle prompt showing tank status -S- 
    */
   if( ch->fighting != NULL ) /* only way to do it, sorry */
   {
      /*
       * We'll just show a percentage for people in the group 
       */
      CHAR_DATA *tank;
      char cbuf[200];

      cbuf[0] = '\0';

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
            snprintf( wound, 100, "@@rExcellent@@N" );
         else if( percent >= 85 )
            snprintf( wound, 100, "@@GScratches@@N" );
         else if( percent >= 70 )
            snprintf( wound, 100, "@@ySmall wounds@@N" );
         else if( percent >= 55 )
            snprintf( wound, 100, "@@bLarger wounds@@N" );
         else if( percent >= 45 )
            snprintf( wound, 100, "@@mBleeding freely@@N" );
         else if( percent >= 30 )
            snprintf( wound, 100, "@@RLeaking guts@@N" );
         else if( percent >= 15 )
            snprintf( wound, 100, "@@eA MESS!@@N" );
         else
            snprintf( wound, 100, "@@2@@W@@fALMOST DEAD!!!@@N" );
         if( ch->act.test(ACT_BLIND_PLAYER) )
            snprintf( cbuf, 200, "@@W%s %s @@N ", tank == ch ? "YOU" : "Tank", wound );
         else
            snprintf( cbuf, 200, "@@a[@@W%s@@a:%s@@a]@@N ", tank == ch ? "YOU" : "Tank", wound );
         write_to_buffer( d, cbuf );
      }
   }


   /*
    * battle prompt 
    */
   if( ( victim = ch->fighting ) != NULL )

   {
      int percent;
      char wound[100];
      char cbuf[200];

      cbuf[0] = '\0';

      if( victim->max_hit > 0 )
         percent = victim->hit * 100 / victim->max_hit;
      else
         percent = -1;

      if( percent >= 95 )
         snprintf( wound, 100, "@@rExcellent@@N" );
      else if( percent >= 85 )
         snprintf( wound, 100, "@@GScratches@@N" );
      else if( percent >= 70 )
         snprintf( wound, 100, "@@ySmall wounds@@N" );
      else if( percent >= 55 )
         snprintf( wound, 100, "@@bLarger wounds@@N" );
      else if( percent >= 45 )
         snprintf( wound, 100, "@@mBleeding freely@@N" );
      else if( percent >= 30 )
         snprintf( wound, 100, "@@RLeaking guts@@N" );
      else if( percent >= 15 )
         snprintf( wound, 100, "@@eA MESS!@@N" );
      else
         snprintf( wound, 100, "@@2@@WALMOST DEAD!!!@@N" );
      if( ch->act.test(ACT_BLIND_PLAYER) )
         snprintf( cbuf, 200, "@@WVictim %s @@N\r\n", wound );
      else
         snprintf( cbuf, 200, "@@a[@@WVictim@@a:%s@@a]@@N\r\n", wound );

      write_to_buffer( d, cbuf );
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
    * Initial \r\n if needed.
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

      outbuf = (char *)getmem( 2 * d->outsize );
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
            if( ch != NULL && !ch->act.test(ACT_COLOR) )
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

            if( ch != NULL && IS_GHOST(ch) )
            {
             switch( number_range(1,3) )
             {
              case 1: lookup = 'W';  break; /* white      */
              case 2: lookup = 'd';  break; /* dark grey  */
              case 3: lookup = 'g';  break; /* light grey */
             }
            }

            for( cnt = 0; cnt < MAX_ANSI; cnt++ )
               if( ansi_table[cnt].letter == lookup )
                  break;

            if( lookup == 'z' ) /* Random foreground color */
             cnt = number_range(0,16);
            if( lookup == 'q' ) /* Random background color */
             cnt = number_range(20,27);

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

               outbuf = (char *)getmem( 2 * d->outsize );
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
/*
 *
 * Added block checking to prevent random booting of the descriptor. Thanks go
 * out to Rustry for his suggestions. -Orion
 */
bool write_to_descriptor( int desc, char *txt, int length )
{
 int iStart = 0;
 int nWrite = 0;
 int nBlock = 0;
 int iErr = 0;

 if( length <= 0 )
  length = strlen( txt );

 for( iStart = 0; iStart < length; iStart += nWrite )
 {
  nBlock = UMIN( length - iStart, 4096 );
  nWrite = send( desc, txt + iStart, nBlock, 0 );

  if( nWrite == -1 )
  {
   iErr = errno;

   if( iErr == EWOULDBLOCK )
   {
    /*
     * This is a SPAMMY little bug error. I would suggest
     * not using it, but I've included it in case. -Orion
     *
     * perror( "Write_to_descriptor: Send is blocking" );
     */
    nWrite = 0;
    continue;
   }
   else
   {
    perror( "Write_to_descriptor" );
    return false;
   }
  }
 }
 return true;
}

void show_stotal_to( DESCRIPTOR_DATA * d )
{
 CHAR_DATA *ch = d->character;
 char buf[MSL];
 
 snprintf( buf, MSL, "Str: [%d/%d]  Int: [%d/%d]  Wis: [%d/%d]  Dex: [%d/%d]  Con: [%d/%d]  Total: [%d/%d]",
  ch->pcdata->max_str, race_table[ch->race].race_str, ch->pcdata->max_int, race_table[ch->race].race_int, ch->pcdata->max_wis,
  race_table[ch->race].race_wis, ch->pcdata->max_dex, race_table[ch->race].race_dex, ch->pcdata->max_con, race_table[ch->race].race_con,
 (ch->pcdata->max_str + ch->pcdata->max_int + ch->pcdata->max_wis + ch->pcdata->max_dex + ch->pcdata->max_con), MAX_TOT_STATS );

 write_to_buffer(d,buf);

 return;
}

void show_menu_to( DESCRIPTOR_DATA * d )
{
   CHAR_DATA *ch = d->character;
   char buf[MAX_STRING_LENGTH];
   char menu[MAX_STRING_LENGTH];

   snprintf( menu, MSL, "\r\nCharacter Creation Menu.\r\n\r\n" );
   strncat( menu, "Options:\r\n", MSL );

   snprintf( buf, MSL, "        1. Set Gender       Currently:%s\r\n",
            !IS_SET( d->check, CHECK_SEX ) ? "Not Set" :
            ch->sex == SEX_NEUTRAL ? "Neutral." : ch->sex == SEX_MALE ? "Male." : "Female." );
   strncat( menu, buf, MSL-1 );

   snprintf( buf, MSL, "        2. Set Race         Currently:%s\r\n",
            !IS_SET( d->check, CHECK_RACE ) ? "Not Set" : race_table[ch->race].race_title );
   strncat( menu, buf, MSL-1 );

   strncat( menu, "        3. Roll Attributes  Currently:", MSL );
   if( IS_SET( d->check, CHECK_STATS ) )
      snprintf( buf, MSL, "\r\n        Str: [%d]  Int: [%d]  Wis: [%d]\r\n        Dex: [%d]  Con: [%d]\r\n",
               ch->pcdata->max_str, ch->pcdata->max_int, ch->pcdata->max_wis, ch->pcdata->max_dex, ch->pcdata->max_con );
   else
      snprintf( buf, MSL, "Not Set\r\n" );


   strncat( menu, buf, MSL-1 );

   strncat( menu, "        4. Set Class Order  Currently:", MSL );
   if( IS_SET( d->check, CHECK_CLASS ) )
   {
      short i;
      snprintf( buf, MSL, "\r\n        " );
      for( i = 0; i < MAX_CLASS; i++ )
      {
         snprintf( buf, MSL, "%s ", class_table[ch->pcdata->order[i]].who_name );
         strncat( menu, buf, MSL-1 );
      }
      strncat( menu, "\r\n", MSL );
   }
   else
      strncat( menu, "Not Set\r\n", MSL );

   strncat( menu, "        5. Exit Creation Process\r\n", MSL );

   strncat( menu, "\r\nPlease Select 1-5: ", MSL );
   write_to_buffer( d, menu );
   return;
}

void show_smenu_to( DESCRIPTOR_DATA * d )
{
   CHAR_DATA *ch = d->character;
   char buf[MAX_STRING_LENGTH];
   char menu[MAX_STRING_LENGTH];

   snprintf( menu, MSL, "\r\nCharacter Creation: Gender.\r\n\r\n" );

   strncat( menu, "Please Select:\r\n", MSL );
   strncat( menu, "              M : Male\r\n", MSL );
   strncat( menu, "              F : Female\r\n", MSL );
   strncat( menu, "              N : Neutral\r\n\r\n", MSL );

   if( IS_SET( d->check, CHECK_SEX ) )
      snprintf( buf, MSL, "Current Choice: %s\r\n",
               ch->sex == SEX_NEUTRAL ? "Neutral" : ch->sex == SEX_MALE ? "Male" : "Female" );
   else
      snprintf( buf, MSL, "No Current Selection.\r\n" );

   strncat( menu, buf, MSL-1 );
   strncat( menu, "\r\nPlease Select M/F/N: ", MSL );

   write_to_buffer( d, menu );
   return;
}

void show_rmenu_to( DESCRIPTOR_DATA * d )
{
   char menu[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   int iRace;

   snprintf( menu, MSL, "\r\nCharacter Creation: Race.\r\n\r\n" );
   strncat( menu, "Notes: a) Race determines abilities in different classes.\r\n", MSL );
   strncat( menu, "       b) Each Race will soon have seperate hometowns.\r\n", MSL );
   strncat( menu, "       c) Race determines your attributes.\r\n\r\n", MSL );
   strncat( menu, "Abr   Name             Class Abilities       Str Int Wis Dex Con\r\n", MSL );
   strncat( menu, "---   ----             ---------------       --- --- --- --- ---\r\n", MSL );

   for( iRace = 0; iRace < MAX_RACE; iRace++ )
   {
      if( race_table[iRace].player_allowed == FALSE )
         continue;
      snprintf( buf, MSL, "%3s   %-10s     %s", race_table[iRace].race_name,
               race_table[iRace].race_title, race_table[iRace].comment );
      strncat( menu, buf, MSL-1 );

      snprintf( buf, MSL, "  %-2d  %-2d  %-2d  %-2d  %-2d\r\n",
               race_table[iRace].race_str,
               race_table[iRace].race_int,
               race_table[iRace].race_wis, race_table[iRace].race_dex, race_table[iRace].race_con );
      strncat( menu, buf, MSL-1 );
   }

   strncat( menu, "\r\nPlease Select Your Race (Abr), or type the full race name for race info: ", MSL );
   write_to_buffer( d, menu );
   return;
}

void show_amenu_to( DESCRIPTOR_DATA * d )
{
   CHAR_DATA *ch = d->character;
   char menu[MAX_STRING_LENGTH];

   /*
    * Make the 'rolls', set ch->max_*, and display
    */
   if( !IS_SET( d->check, CHECK_RACE ) )
   {
      snprintf( menu, MSL, "\r\nYou must select a race first.\r\n" );
      write_to_buffer( d, menu );
      d->connected = CON_MENU;
      show_menu_to( d );
      return;
   }

   ch->pcdata->max_str = number_range(3,(race_table[ch->race].race_str-5));
   ch->pcdata->max_int = number_range(3,(race_table[ch->race].race_int-5));
   ch->pcdata->max_wis = number_range(3,(race_table[ch->race].race_wis-5));
   ch->pcdata->max_dex = number_range(3,(race_table[ch->race].race_dex-5));
   ch->pcdata->max_con = number_range(3,(race_table[ch->race].race_con-5));

   snprintf( menu, MSL, "\r\nCharacter Creation: Attributes.\r\n\r\n" );
   strncat( menu, "There is no way to increase your MAX attributes, so choose wisely!\r\n", MSL );
   strncat( menu, "Current Attributes:\r\n", MSL );
   write_to_buffer(d,menu);

   show_stotal_to(d);

   write_to_buffer( d, "\r\nPlease Select: (A)ccept, return to menu, (H)elp stats, +(stat), or -(stat): ");

   return;
}

void show_ahelp_menu_to( DESCRIPTOR_DATA * d )
{

   char menu[MAX_STRING_LENGTH];
   snprintf( menu, MSL, "%s", "" );
   strncat( menu, "Str affects items you can wear and weight you can carry, and your hitroll and damroll.\r\n", MSL );
   strncat( menu, "Int affects your mana gain, how many Npcs you can control effectively, and spell success.\r\n", MSL );
   strncat( menu, "Wis affects how many practices you get, your mana, and your saving against spells.\r\n", MSL );
   strncat( menu, "Dex affects your ac, how many items you can carry, and your ability to dodge.\r\n", MSL );
   strncat( menu, "Con affects how many hitpoints you gain per level.\r\n", MSL );
   strncat( menu, "\r\nPlease Select: (A)ccept, return to menu, (H)elp stats, +(stat), or -(stat): ", MSL );

   write_to_buffer( d, menu );
   return;
}

void show_cmenu_to( DESCRIPTOR_DATA * d )
{
   char menu[MAX_STRING_LENGTH];
   char buf[MAX_STRING_LENGTH];
   int iClass;

   snprintf( menu, MSL, "Character Creation: Class Order.\r\n\r\n" );
   strncat( menu, "This option allows you to select the order of your classes.\r\n", MSL );
   strncat( menu, "Being a MultiClass Mud, this order is very important, as it\r\n", MSL );
   strncat( menu, "will determine how easily you progress in each class, and\r\n", MSL );
   strncat( menu, "how well you can use the skills/spells of each class.\r\n", MSL );
   strncat( menu, "There are five classes.  Please list, in order of best to\r\n", MSL );
   strncat( menu, "worst, the order your classes will be.\r\n", MSL );
   strncat( menu, "(The 1st you pick will be your prime class, gaining a +1 bonus.\r\n", MSL );
   strncat( menu, "For example, psi mag cle thi war.\r\n", MSL );
   strncat( menu, "Abr    Prime Atr    Name\r\n", MSL );
   strncat( menu, "---    ---------    ----\r\n", MSL );

   for( iClass = 0; iClass < MAX_CLASS; iClass++ )
   {
      snprintf( buf, MSL, "%3s    %3s    %-10s\r\n", class_table[iClass].who_name, class_table[iClass].attr,
               class_table[iClass].class_name );
      strncat( menu, buf, MSL-1 );
   }
   strncat( menu, "\r\nOrder: ", MSL );
   write_to_buffer( d, menu );
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

      snprintf( buf, MSL, "%s provided as name from login from site %s.", argument, d->host );
      monitor_chan( buf, MONITOR_CONNECT );

      if( !check_parse_name( argument ) )
      {
         snprintf( buf, MSL, "Illegal name %s from site %s.", argument, d->host );
         monitor_chan( buf, MONITOR_CONNECT );
         write_to_buffer( d, "Illegal name, try another.\r\nName: " );
         return;
      }

      fOld = load_char_obj( d, argument, FALSE );
      ch = d->character;
      char_list.remove(d->character); // ctor adds to list automatically, we need to exempt still-logging-in folks and re-add later --Kline

      if( ch->act.test(ACT_DENY) )
      {
         snprintf( log_buf, (2 * MIL), "Denying access to %s@%s.", argument, d->host );
         log_string( log_buf );
         monitor_chan( log_buf, MONITOR_CONNECT );
         write_to_buffer( d, "You are denied access.\r\n" );
         close_socket( d );
         return;
      }
      if( ch->act.test(ACT_WHITELIST) )
      {
       bool found = false;

       for( short i = 0; i < MAX_HOSTS; i++ )
        if( ch->pcdata->whitelist[i] != str_empty && !str_prefix(ch->pcdata->whitelist[i],d->host) )
         found = true;

       if( !found )
       {
        snprintf(log_buf,(2 * MIL),"Whitelist prohibited login %s@%s.",argument,d->host);
        log_string(log_buf);
        monitor_chan(log_buf,MONITOR_CONNECT);
        write_to_buffer(d,"This is not an approved connection domain for this character.\r\n");
        close_socket(d);
        return;
       }
      }
/* TEMP FIX ZEN */
      if( ch->act.test(ACT_JUSTIFY) )
         ch->act.reset(ACT_JUSTIFY);

      if( check_reconnect( d, FALSE ) )
      {
         fOld = TRUE;
      }
      else
      {
         if( wizlock && !IS_HERO( ch ) && !ch->wizbit && !is_name( argument, sysdata.playtesters ) )
         {
            write_to_buffer( d, "\r\n             " mudnamenocolor " IS CURRENTLY WIZLOCKED.\r\n" );
            write_to_buffer( d, "Please Try Connecting Again In A Little While, When Any Problems\r\n" );
            write_to_buffer( d, "       We Are Working On Have Been Solved.  Thank You.\r\n" );
            close_socket( d );
            return;
         }
         if( deathmatch && !IS_HERO( ch ) && !ch->wizbit )
         {
            write_to_buffer( d, "\r\n             " mudnamenocolor " IS CURRENTLY WIZLOCKED.\r\n" );
            write_to_buffer( d, "Sorry, The Players Connected At This Time Are Currently Participating\r\n" );
            write_to_buffer( d, "     In A DEATHMATCH Murder-Fest.  Please try later!\r\n" );
            write_to_buffer( d, "  Deathmatches are usually held on Thursdays and Sundays.  They\r\n" );
            write_to_buffer( d, " normally last about 30 minutes.  Please be patient!\r\n" );
            close_socket( d );
            return;
         }
         if( check_playing( d, ch->name ) )
            return;
      }

      if( fOld )
      {
         BAN_DATA *pban;
         list<BAN_DATA *>::iterator li;

         for( li = ban_list.begin(); li != ban_list.end(); li++ )
         {
            pban = *li;
            if( !str_prefix( pban->name, d->host ) && ( pban->newbie == FALSE ) )
            {
               snprintf( buf, MSL, "Denying access to banned site %s", d->host );
               monitor_chan( buf, MONITOR_CONNECT );
               write_to_descriptor( d->descriptor, "Your site has been banned from this Mud.  BYE BYE!\r\n" );
               d->connected = CON_QUITTING;
               close_socket( d );
               return;
            }
         }

         /*
          * Old player 
          */
         write_to_buffer( d, "Password: " );
         write_to_buffer( d, echo_off_str );
         d->connected = CON_GET_OLD_PASSWORD;
         return;
      }
      else
      {
         BAN_DATA *pban;
         list<BAN_DATA *>::iterator li;
         /*
          * New player
          */
         /*
          * New characters with same name fix by Salem's Lot
          */
         if( check_playing( d, ch->name ) )
            return;

         for( li = ban_list.begin(); li != ban_list.end(); li++ )
         {
            pban = *li;
            if( !str_prefix( pban->name, d->host ) )

            {
               snprintf( buf, MSL, "Denying access to banned site %s", d->host );
               monitor_chan( buf, MONITOR_CONNECT );
               write_to_descriptor( d->descriptor, "Your site has been banned from this Mud.  BYE BYE!\r\n" );
               d->connected = CON_QUITTING;
               close_socket( d );
               return;
            }
         }

         snprintf( buf, MSL, "Did I get that right, %s (Y/N)? ", argument );
         write_to_buffer( d, buf );
         d->connected = CON_CONFIRM_NEW_NAME;
         return;
      }
   }

   if( d->connected == CON_GET_OLD_PASSWORD )
   {
      write_to_buffer( d, "\r\n", 2 );
      if( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
      {
         write_to_buffer( d, "Wrong password.\r\n" );
         snprintf( buf, MSL, "FAILED LOGIN for %s from site %s.", ch->name.c_str(), d->host );
         monitor_chan( buf, MONITOR_CONNECT );
         ch->pcdata->failures++;
         save_char_obj( ch );
         close_socket( d );
         return;
      }

      write_to_buffer( d, echo_on_str );

      if( check_reconnect( d, TRUE ) )
         return;

      if( check_playing( d, ch->name ) )
         return;

      snprintf( log_buf, (2 * MIL), "%s has connected.", ch->name.c_str() );
      monitor_chan( log_buf, MONITOR_CONNECT );

      snprintf( log_buf, (2 * MIL), "Site Name: %s.", d->host );
      monitor_chan( log_buf, MONITOR_CONNECT );

      log_string( log_buf );
      lines = ch->pcdata->pagelen;
      ch->pcdata->pagelen = 20;

      if( ch->lvl[ch->p_class] == -1 )
         ch->lvl[ch->p_class] = ch->level;

      if( IS_HERO( ch ) )
      {
         DL_LIST *brands;
         short numbrands;
         char msgbuf[MSL];
         for( brands = first_brand, numbrands = 0; brands; brands = brands->next, numbrands++ );
         do_help( ch, "imotd" );
         snprintf( msgbuf, MSL, "There are currently %d outstanding brands.\r\n%s",
                  numbrands,
                  ( ( numbrands < 50 ) ?
                    "" :
                    "@@eWarning: Process these brands immediately using immbrand list, immbrand read, and immbrand remove to avoid disk overflow!!@@N\r\n" ) );
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
            snprintf( buf, MSL, "New character.\r\nGive me a password for %s: %s", ch->name.c_str(), echo_off_str );
            write_to_buffer( d, buf );
            d->connected = CON_GET_NEW_PASSWORD;
            return;

         case 'n':
         case 'N':
            write_to_buffer( d, "Ok, what IS it, then? " );
            delete d->character;
            d->character = NULL;
            d->connected = CON_GET_NAME;
            return;

         default:
            write_to_buffer( d, "Please type Yes or No? " );
            return;
      }
      return;
   }

   if( d->connected == CON_GET_NEW_PASSWORD )
   {
      write_to_buffer( d, "\r\n", 2 );

      if( strlen( argument ) < 5 )
      {
         write_to_buffer( d, "Password must be at least five characters long.\r\nPassword: " );
         return;
      }

      pwdnew = crypt( argument, ch->name.c_str() );
      for( p = pwdnew; *p != '\0'; p++ )
      {
         if( *p == '~' )
         {
            write_to_buffer( d, "New password not acceptable, try again.\r\nPassword: " );
            return;
         }
      }

      free_string( ch->pcdata->pwd );
      ch->pcdata->pwd = str_dup( pwdnew );
      write_to_buffer( d, "Please retype password: " );
      d->connected = CON_CONFIRM_NEW_PASSWORD;
      return;
   }

   if( d->connected == CON_CONFIRM_NEW_PASSWORD )
   {
      write_to_buffer( d, "\r\n", 2 );

      if( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
      {
         write_to_buffer( d, "Passwords don't match.\r\nRetype password: " );
         d->connected = CON_GET_NEW_PASSWORD;
         return;
      }
      write_to_buffer( d, echo_on_str );
      show_menu_to( d );
      d->connected = CON_MENU;
      return;
   }

   if( d->connected == CON_MENU )
   {
      int number;

      if( !is_number( argument ) )
      {
         write_to_buffer( d, "\r\nPlease Enter A Number.\r\n" );
         show_menu_to( d );
         return;
      }
      number = atoi( argument );
      if( number < 1 && number > 5 )
      {
         write_to_buffer( d, "\r\nPlease Enter A Number Between 1 And 5.\r\n" );
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
               write_to_buffer( d, "ALL Options Must Be Selected First.\r\n" );
               show_menu_to( d );
               return;
            }
            snprintf( log_buf, (2 * MIL), "%s@%s new player.", ch->name.c_str(), d->host );
            log_string( log_buf );
            monitor_chan( log_buf, MONITOR_CONNECT );
            write_to_buffer( d, "\r\n", 2 );
            ch->pcdata->pagelen = 20;

            do_save(ch,"auto");
            do_help( ch, "newun" );
            mudinfo.total_pfiles++;
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
      int total = (ch->pcdata->max_str + ch->pcdata->max_int + ch->pcdata->max_wis + ch->pcdata->max_dex + ch->pcdata->max_con);

      switch ( argument[0] )
      {
         case '-':
          argument++;
          if( !str_prefix(argument,"str") )
          {
           if( ch->pcdata->max_str <= 3 )
            write_to_buffer(d,"You must have at least 3 Str.");
           else
           {
            ch->pcdata->max_str--;
            show_stotal_to(d);
           }
          }
          if( !str_prefix(argument,"int") )
          {
           if( ch->pcdata->max_int <= 3 )
            write_to_buffer(d,"You must have at least 3 Int.");
           else
           {
            ch->pcdata->max_int--;
            show_stotal_to(d);
           }
          }
          if( !str_prefix(argument,"wis") )
          {
           if( ch->pcdata->max_wis <= 3 )
            write_to_buffer(d,"You must have at least 3 Wis.");
           else
           {
            ch->pcdata->max_wis--;
            show_stotal_to(d);
           }
          }
          if( !str_prefix(argument,"dex") )
          {
           if( ch->pcdata->max_dex <= 3 )
            write_to_buffer(d,"You must have at least 3 Dex.");
           else
           {
            ch->pcdata->max_dex--;
            show_stotal_to(d);
           }
          }
          if( !str_prefix(argument,"con") )
          {
           if( ch->pcdata->max_con <= 3 )
            write_to_buffer(d,"You must have at least 3 Con.");
           else
           {
            ch->pcdata->max_con--;
            show_stotal_to(d);
           }
          }
          break;
         case '+':
          argument++;
          if( !str_prefix(argument,"str") )
          {
           if( ch->pcdata->max_str >= race_table[ch->race].race_str )
            write_to_buffer(d,"Your Str is already at max.");
           else if( total >= MAX_TOT_STATS )
           {
            snprintf(buf,MSL,"You already have %d total stat points allocated.",MAX_TOT_STATS);
            write_to_buffer(d,buf);
           }
           else
           {
            ch->pcdata->max_str++;
            show_stotal_to(d);
           }
          }
          if( !str_prefix(argument,"int") )
          {
           if( ch->pcdata->max_int >= race_table[ch->race].race_int )
            write_to_buffer(d,"Your Int is already at max.");
           else if( total >= MAX_TOT_STATS )
           {
            snprintf(buf,MSL,"You already have %d total stat points allocated.",MAX_TOT_STATS);
            write_to_buffer(d,buf);
           }
           else
           {
            ch->pcdata->max_int++;
            show_stotal_to(d);
           }
          }
          if( !str_prefix(argument,"wis") )
          {
           if( ch->pcdata->max_wis >= race_table[ch->race].race_wis )
            write_to_buffer(d,"Your Wis is already at max.");
           else if( total >= MAX_TOT_STATS )
           {
            snprintf(buf,MSL,"You already have %d total stat points allocated.",MAX_TOT_STATS);
            write_to_buffer(d,buf);
           }
           else
           {
            ch->pcdata->max_wis++;
            show_stotal_to(d);
           }
          }
          if( !str_prefix(argument,"dex") )
          {
           if( ch->pcdata->max_dex >= race_table[ch->race].race_dex )
            write_to_buffer(d,"Your Dex is already at max.");
           else if( total >= MAX_TOT_STATS )
           {
            snprintf(buf,MSL,"You already have %d total stat points allocated.",MAX_TOT_STATS);
            write_to_buffer(d,buf);
           }
           else
           {
            ch->pcdata->max_dex++;
            show_stotal_to(d);
           }
          }
          if( !str_prefix(argument,"con") )
          {
           if( ch->pcdata->max_con >= race_table[ch->race].race_con )
            write_to_buffer(d,"Your Con is already at max.");
           else if( total >= MAX_TOT_STATS )
           {
            snprintf(buf,MSL,"You already have %d total stat points allocated.",MAX_TOT_STATS);
            write_to_buffer(d,buf);
           }
           else
           {
            ch->pcdata->max_con++;
            show_stotal_to(d);
           }
          }
          break;
         case 'A':
         case 'a':
          if( total < MAX_TOT_STATS )
           write_to_buffer(d,"Please finish allocating your stats first.");
          else
          {
           if( !IS_SET( d->check, CHECK_STATS ) )
            SET_BIT( d->check, CHECK_STATS );
           d->connected = CON_MENU;
           show_menu_to( d );
          }
          break;
         case 'H':
         case 'h':
            show_ahelp_menu_to( d );
            break;
         default:
            write_to_buffer( d, "n\rPlease Select: (A)ccept, return to menu, (H)elp stats, +(stat), or -(stat): " );
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
            ch->pcdata->login_sex = SEX_MALE;
            break;
         case 'f':
         case 'F':
            ch->sex = SEX_FEMALE;
            ch->pcdata->login_sex = SEX_FEMALE;
            break;
         case 'n':
         case 'N':
            ch->sex = SEX_NEUTRAL;
            ch->pcdata->login_sex = SEX_NEUTRAL;
            break;
         default:
            write_to_buffer( d, "That's not a sex.\r\nWhat IS your sex? " );
            show_smenu_to( d );
            return;
      }
      write_to_buffer( d, "\r\n\r\n" );
      if( !IS_SET( d->check, CHECK_SEX ) )
         SET_BIT( d->check, CHECK_SEX );
      d->connected = CON_MENU;
      show_menu_to( d );
      return;
   }

   if( d->connected == CON_GET_NEW_CLASS )
   {
      short classes[MAX_CLASS];
      short parity[MAX_CLASS];  /* Nowt to do with parity really */
      char arg[MAX_STRING_LENGTH];
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
         write_to_buffer( d, "Invalid Order... Please Try Again. You must list each class, by abbreviation, such as CLE WAR MAG THI PSI.\r\n" );
         show_cmenu_to( d );
         return;
      }

      /*
       * Copy classes across to pcdata 
       */
      for( cnt = 0; cnt < MAX_CLASS; cnt++ )
       ch->pcdata->order[cnt] = classes[cnt];

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
         write_to_buffer( d, "Invalid Choice.\r\n" );
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
      list<NOTE_DATA *>::iterator li;
      /*
       * Prime level idea dropped.  Give ch 1 level in their best class 
       */
      if( ch->level == 0 )
      {
         ch->p_class = ch->pcdata->order[0];
         ch->lvl[ch->p_class] = 1;
      }

      d->connected = CON_PLAYING;


      if( ch->act.test(ACT_FULL_ANSI) )
      {
         char scrollbuf[MSL];
         snprintf( scrollbuf, MSL, "%s%s%s%i;%ir%s%%i;%iH",
                  CRS_RESET, CRS_CLS, CRS_CMD, 0, ch->pcdata->term_rows - 12, CRS_CMD, ch->pcdata->term_rows - 13 );
         send_to_char( scrollbuf, ch );
      }
      send_to_char( "\r\nWelcome to " mudnamecolor ".  May your visit here be ... mutated.\r\n", ch );

      if( ch->level == 0 )
      {
         /*
          * OBJ_DATA *obj; unused 
          */

         switch ( class_table[ch->p_class].attr_prime )
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

         ch->lvl[ch->p_class] = 1;
         for( cnt = 0; cnt < MAX_CLASS; cnt++ )
            if( cnt != ch->p_class )
               ch->lvl[cnt] = 0;

         if( mudinfo.total_pfiles == 1 ) /* First user, grand full admin. --Kline */
         {
          ch->level = MAX_LEVEL;
          ch->lvl[ch->p_class] = MAX_LEVEL;;
          ch->trust = MAX_LEVEL;
          send_to_char("@@e@@f*** AS THE FIRST LOGIN, YOU HAVE BEEN ADVANCED TO MAX LEVEL ***@@N\r\n",ch);
          do_save(ch,"auto");            /* Lets write the pfile and mudinfo to ensure    */
          save_mudinfo();                /* if we crash that no one else logs in first to */
         }                               /* steal power. --Kline                          */

         ch->exp = 0;
         ch->hit = ch->max_hit;
         ch->mana = ch->max_mana;
         ch->move = ch->max_move;
         ch->pcdata->mana_from_gain = ch->max_mana;
         ch->pcdata->hp_from_gain = ch->max_hit;
         ch->pcdata->move_from_gain = ch->max_move;

         ch->clan = 0;   /* no clan */
         ch->set_title("@@N needs a new title!");

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
         char_to_room( ch, get_room_index( ROOM_VNUM_SCHOOL ) );
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
         snprintf( msg, MSL, "WARNING:  There have been %d failed login attempts.\r\n", ch->pcdata->failures );
         send_to_char( msg, ch );
         ch->pcdata->failures = 0;
      }

      if( ch->level > 1 )
      {
         do_logins(ch);

         for( short i = 0; i < MAX_HOSTS; i++ )
         {
          if( strcmp( d->host, ch->pcdata->host[i] ) )
          {
             snprintf( msg, MSL, "%s connected from %s ( last login was from %s ) !", ch->name.c_str(), d->host, ch->pcdata->host[0] );
             log_string( msg );
             monitor_chan( msg, MONITOR_CONNECT );
             if( ( ch->level > 80 ) )
             {
                snprintf( msg, MSL, "WARNING!!! %s logged in with level %d.\r\n", ch->name.c_str(), ch->level );
                log_string( msg );
             }

          }
         }

         for( short i = 0; i < MAX_HOSTS; i++ )
         {
          if( !str_cmp(ch->pcdata->host[i],"Unknown!") )
          {
           free_string(ch->pcdata->host[i]);
           ch->pcdata->host[i] = str_dup(d->host);
           break;
          }
          if( i == (MAX_HOSTS -1 ) )
          {
           free_string(ch->pcdata->host[(i-1)]);
           ch->pcdata->host[(i-1)] = str_dup(ch->pcdata->host[i]);
          }
         }
      }

      /*
       * Check for new notes waiting at the post office
       */
      notes = 0;

      for( li = note_list.begin(); li != note_list.end(); li++ )
      {
         pnote = *li;
         if( is_note_to( ch, pnote ) && str_cmp( ch->name, pnote->sender ) && pnote->date_stamp > ch->pcdata->last_note )
            notes++;
      }

      if( notes == 1 )
         send_to_char( "\r\nYou have one new letter waiting.\r\n", ch );
      else if( notes > 1 )
      {
         snprintf( buf, MSL, "\r\nYou have %d new letters waiting.\r\n", notes );
         send_to_char( buf, ch );
      }



      act( "$n enters " mudnamecolor ".", ch, NULL, NULL, TO_ROOM );
      char_list.push_back(ch); /* we removed them earlier in the function, now put them back that login is done --Kline */

      snprintf( buf, MSL, "%s has entered the game.", ch->name.c_str() );
      monitor_chan( buf, MONITOR_CONNECT );

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

      if( IS_VAMP( ch ) && !IS_NPC( ch ) && ( ch->pcdata->recall_vnum == ROOM_VNUM_TEMPLE ) )
         ch->pcdata->recall_vnum = 9001;

      if( ch->pcdata->hp_from_gain < 0 )
         reset_gain_stats( ch );
/*       ch->affected_by = 0;   */


      ch->is_quitting = false;
      d->connected = CON_SETTING_STATS;
      {
         OBJ_DATA *wear_object;
         AFFECT_DATA *this_aff;
         AFFECT_DATA *this_aff_next;

         ch->max_mana = ch->pcdata->mana_from_gain;
         ch->max_hit = ch->pcdata->hp_from_gain;
         ch->max_move = ch->pcdata->move_from_gain;
         ch->saving_throw = ch->get_level("psuedo") / 10;
         ch->hitroll = 0;
         ch->damroll = 0;
         ch->armor = 100;
         if( ch->pcdata->login_sex != -1 )
            ch->sex = ch->pcdata->login_sex;
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
            delete this_aff;
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
bool check_reconnect( DESCRIPTOR_DATA * d, bool fConn )
{
   CHAR_DATA *ch;
   OBJ_DATA *obj;
   list<CHAR_DATA *>::iterator li;

   for( li = char_list.begin(); li != char_list.end(); li++ )
   {
      ch = *li;
      if( !IS_NPC( ch ) && ( !fConn || ch->desc == NULL ) && d->character->name == ch->name )
      {
         if( fConn == FALSE )
         {
            free_string( d->character->pcdata->pwd );
            d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
         }
         else
         {
            delete d->character;
            d->character = ch;
            ch->desc = d;
            ch->timer = 0;
            send_to_char( "Reconnecting.\r\n", ch );
            act( "$n reconnects.", ch, NULL, NULL, TO_ROOM );
            snprintf( log_buf, (2 * MIL), "%s@%s reconnected.", ch->name.c_str(), d->host );
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
bool check_playing( DESCRIPTOR_DATA * d, string name )
{
   DESCRIPTOR_DATA *dold;
   char buf[MAX_STRING_LENGTH];

   for( dold = first_desc; dold; dold = dold->next )
   {
      if( dold != d
          && dold->character != NULL
          && dold->connected != CON_GET_NAME
          && dold->connected != CON_GET_OLD_PASSWORD
          && name == ( dold->original ? dold->original->name : dold->character->name ) )
      {
         snprintf( buf, MSL, "Player from site %s tried to login as %s (already playing) !", d->host, name.c_str() );
         monitor_chan( buf, MONITOR_CONNECT );
/* Not sure if we want to do this..players can cheat and try to log back in as themselves to end a fight Zen
	    dold->character->position = POS_STANDING;
          if ( dold->character->fighting != NULL )
            dold->character->fighting = NULL;
*/
         d->connected = CON_RECONNECTING;
         do_quit( dold->character, "" );
         write_to_buffer( d, "Already playing. If you were not fighting or dead, you were disconnected\r\nName: " );
         d->connected = CON_GET_NAME;
         if( d->character != NULL )
         {
            delete d->character;
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
void send_to_char( string txt, CHAR_DATA *ch )
{
 if( ch == NULL )
  return;
 if( txt.empty() || ch->desc == NULL )
  return;

 if( ch->desc->showstr_head != NULL )
 {
  char *ssh;

  ssh = (char *)qgetmem( strlen( ch->desc->showstr_head ) + txt.length() + 1 );
  strcpy( ssh, ch->desc->showstr_head );
  strcat( ssh, txt.c_str() );

  if( ch->desc->showstr_point )
   ch->desc->showstr_point += ( ssh - ch->desc->showstr_head );
  else
   ch->desc->showstr_point = ssh;

  free(ch->desc->showstr_head);
  ch->desc->showstr_head = ssh;
 }
 else
 {
  ch->desc->showstr_head = (char *)malloc( txt.length() + 1);
  strcpy( ch->desc->showstr_head, txt.c_str() );
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
         write_to_buffer( d, "C, or Return = continue, R = redraw this page,\r\n" );
         write_to_buffer( d, "B = back one page, H = this help, Q or other keys = exit.\r\n\r\n" );
         lines = -1 - ( d->character->pcdata->pagelen );
         break;

      default:   /*otherwise, stop the text viewing */
         if( d->showstr_head )
         {
            free(d->showstr_head);
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
               free(d->showstr_head);
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

   char buf[MSL];
   char fname[MSL];
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

   if( ch->in_room == NULL )
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
      if( IS_NPC( to ) || !IS_AWAKE( to ) )
         continue;
      if( type == TO_CHAR && to != ch )
         continue;
      if( type == TO_VICT && ( to != vch || to == ch ) )
         continue;
      if( type == TO_ROOM && to == ch )
         continue;
      if( type == TO_NOTVICT && ( to == ch || to == vch ) )
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
                     if( !IS_NPC(ch) && ch->act.test(ACT_WIZINVIS) && ch->pcdata->invis > get_trust( to ) )
                        can_see_message = FALSE;
                  }
                  else
                  {
                     if( !IS_NPC(ch) && ch->act.test(ACT_WIZINVIS) && get_trust( to ) < ch->pcdata->invis )
                        can_see_message = FALSE;
                     if( ( IS_AFFECTED( ch, AFF_SNEAK ) || item_has_apply( ch, ITEM_APPLY_SNEAK ) )
                         && ( ( ch->get_level("psuedo") - 20 + number_range( 1, 30 ) ) > to->get_level("psuedo") ) )
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
                  i = ch->get_name(to);
                  break;
               case 'N':
                  i = vch->get_name(to);
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
                  one_argument( const_cast<char *>(ch->name.c_str()), tmp_str );
                  i = ( char * )tmp_str;
                  break;
               case 'K':
                  one_argument( const_cast<char *>(ch->name.c_str()), tmp_str );
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
      /*
       * Added by Kahn
       */
   }

   return;
}

DO_FUN(do_finger)
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
      snprintf( buf, MSL, "No pFile found for '%s'.\r\n", capitalize( name ) );
      send_to_char( buf, ch );
      return;
   }

   victim = d.character;
   d.character = NULL;
   victim->desc = NULL;

   snprintf( buf, MSL, "Name: %s.\r\n", capitalize( victim->name.c_str() ) );
   send_to_char( buf, ch );

   snprintf( buf, MSL, "Last Login was from: %s.\r\n", victim->pcdata->host[0] );
   send_to_char( buf, ch );

   snprintf( buf, MSL, "pFile was last saved at: %s.", victim->pcdata->lastlogin );
   send_to_char( buf, ch );

   delete victim;

   return;
}

void send_to_descrips( const char *message )
{
   return;
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
   disable_timer_abort = TRUE;
   log_f( "Copyover recovery initiated" );
   fp = fopen( COPYOVER_FILE, "r" );

   if( !fp )   /* there are some descriptors open which will hang forever then ? */
   {
      perror( "copyover_recover:file_open" );
      log_f( "Copyover file not found. Exitting.\r\n" );
      exit( 1 );
   }

   unlink( COPYOVER_FILE );   /* In case something crashes - doesn't prevent reading   */

   for( ;; )
   {
      if( fscanf( fp, "%d %s %s\n", &desc, name, host ) < 1 )
       bugf("Didn't read anything from COPYOVER_FILE, this is bad!");
      if( desc == -1 )
         break;

      /*
       * Write something, and check if it goes error-free 
       */
      if( !write_to_descriptor( desc, "\r\nRestoring from HOTreboot...\r\n" ) )
      {
         close( desc ); /* nope */
         continue;
      }

      d = new DESCRIPTOR_DATA;
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
         write_to_descriptor( desc, "\r\nSomehow, your character was lost in the HOTreboot. Sorry, you must relog in.\r\n" );
         close_socket( d );
      }
      else  /* ok! */
      {
         CHAR_DATA *this_char;


         write_to_descriptor( desc, "\r\nCopyover recovery complete.\r\n" );

         /*
          * Just In Case 
          */
         if( !d->character->in_room )
            d->character->in_room = get_room_index( ROOM_VNUM_LIMBO );

         /*
          * Insert in the char_list 
          */
         this_char = d->character;

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


         this_char->is_quitting = false;
         d->connected = CON_SETTING_STATS;
         {
            OBJ_DATA *wear_object;
            AFFECT_DATA *this_aff;
            AFFECT_DATA *this_aff_next;

            this_char->max_mana = this_char->pcdata->mana_from_gain;
            this_char->max_hit = this_char->pcdata->hp_from_gain;
            this_char->max_move = this_char->pcdata->move_from_gain;
            this_char->saving_throw = this_char->get_level("psuedo") / 10;
            this_char->hitroll = 0;
            this_char->damroll = 0;
            this_char->armor = 100;
            if( this_char->pcdata->login_sex != -1 )
               this_char->sex = this_char->pcdata->login_sex;
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
               delete this_aff;
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
