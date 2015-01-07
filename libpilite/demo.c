#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "libpilite.h"

#define TDEV "/dev/ttyu0"
#define ON 0
#define OFF 1
#define TOGGLE 2

void usage ( void )  {
  printf( "usage: %s [-f terminal_device]\n", getprogname() );
}


int main ( int argc, char** argv )  {
  char *tdev;
  int ch, tfd;

  tdev = TDEV;

  while ( (ch = getopt ( argc, argv, "f:c:" )) != -1 )  {
    switch (ch)  {
      case 'f':  tdev = optarg;
                 break;
      default:   usage();
    }
  }
  argc -= optind;
  argv += optind;

  if ( (tfd = pilite_init ( tdev )) == -1 )  {
    fprintf ( stderr, "Cann't open terminal device %s\n", tdev );
    exit(-1);
  }

// Clear all
  pilite_clear ( tfd );

// Two horisontal line
  pilite_line ( tfd, 1, 1, 14, 1, ON );
  pilite_line ( tfd, 1, 9, 14, 9, ON );

// Two vertical line
  pilite_line ( tfd, 1, 1, 1, 9, ON );
  pilite_line ( tfd, 14, 1, 14, 9, ON );

// Four points
  pilite_point ( tfd, 1, 1, TOGGLE );
  pilite_point ( tfd, 1, 9, TOGGLE );
  pilite_point ( tfd, 14, 1, TOGGLE );
  pilite_point ( tfd, 14, 9, TOGGLE );

  sleep(2);
// Big filled circle
  pilite_fillcircle ( tfd, 7, 5, 3, ON );

// Small circle and point in center
  pilite_circle ( tfd, 7, 5, 1, OFF );
  pilite_point ( tfd, 7, 5, TOGGLE );

  sleep(2);
// Clear and two circle
  pilite_clear( tfd );
  pilite_circle ( tfd, 5, 5, 4, ON );
  sleep(1);
  pilite_circle ( tfd, 10, 5, 4, ON );

  sleep(2);
// Fill all
  pilite_fill ( tfd );

// Two diagonal line
  pilite_line ( tfd, 1, 1, 14, 9, OFF );
  pilite_line ( tfd, 1, 9, 14, 1, OFF );
  
  sleep(2);
// Clear and two box
  pilite_clear( tfd );
  pilite_fillbox ( tfd, 2, 2, 13, 8, ON );
  pilite_box ( tfd, 4, 4, 11, 6, OFF );

  sleep(4);
// Clear
  pilite_clear( tfd );

  pilite_close ( tfd );
  exit(0);
}

