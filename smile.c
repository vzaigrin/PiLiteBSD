#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define TDEV "/dev/ttyu0"

void usage ( void )  {
  printf( "usage: %s [-f terminal_device]\n", getprogname() );
}


void writecmd( int fd, char *cmd )  {
  int cmdsize;

  cmdsize = strlen ( cmd );
  write (fd, cmd, cmdsize );

  return;
}


int main ( int argc, char** argv )  {
  char *tdev, buf[20];
  int i, ch, tfd;
  struct termios cntrl;

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

  if ( (tfd = open ( tdev, O_RDWR | O_NONBLOCK | O_NOCTTY )) == -1 )  {
    fprintf ( stderr, "Cann't open terminal device %s\n", tdev );
    exit(-1);
  }

  tcgetattr ( tfd, &cntrl ); 
  cntrl.c_cflag |= B9600;
  cntrl.c_lflag &= ~ECHO;
  cntrl.c_iflag &= ~( ICRNL |IXANY ); 
  cntrl.c_iflag |= IGNPAR;
  cntrl.c_cflag &= ~( PARENB | HUPCL); 
  cntrl.c_cflag |= CS8 | CLOCAL; 
  cntrl.c_oflag &= ~OPOST;
  tcsetattr ( tfd, TCSANOW, &cntrl ); 

  writecmd ( tfd, "$$$ALL,OFF\r" );
  writecmd ( tfd, "$$$B1,100\r" );
  for ( i = 2; i < 14; i++ )  {
    sprintf ( buf, "$$$P%d,1,ON\r", i );
    writecmd ( tfd, buf );
    sprintf ( buf, "$$$P%d,9,ON\r", i );
    writecmd ( tfd, buf );
  }
  writecmd ( tfd, "$$$P6,3,ON\r" );
  writecmd ( tfd, "$$$P9,3,ON\r" );
  writecmd ( tfd, "$$$P5,5,ON\r" );
  writecmd ( tfd, "$$$P10,5,ON\r" );
  writecmd ( tfd, "$$$P6,6,ON\r" );
  writecmd ( tfd, "$$$P9,6,ON\r" );
  writecmd ( tfd, "$$$P7,7,ON\r" );
  writecmd ( tfd, "$$$P8,7,ON\r" );
  writecmd ( tfd, "$$$B14,100\r" );

  close(tfd);
  exit(0);
}

