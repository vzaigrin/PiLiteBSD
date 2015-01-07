#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define TDEV "/dev/ttyu0"
#define CMDS 512

void usage ( void )  {
  printf( "usage: %s [-f terminal_device] [-c command_file]\n", getprogname() );
}


void writecmd( int fd, char *cmd )  {
  int cmdsize;

  cmdsize = strlen ( cmd );
  write (fd, cmd, cmdsize );

  return;
}


int main ( int argc, char** argv )  {
  char *tdev, *cfile, cmd[CMDS];
  int ch, tfd, cmdf = 0;;
  FILE *cfd;
  struct termios cntrl;

  tdev = TDEV;

  while ( (ch = getopt ( argc, argv, "f:c:" )) != -1 )  {
    switch (ch)  {
      case 'f':  tdev = optarg;
                 break;
      case 'c':  cfile = optarg;
                 cmdf = 1;
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

  if ( cmdf )  {
    if ( (cfd = fopen ( cfile, "r" )) == NULL )  {
      fprintf ( stderr, "Cann't open command file %s\n", cfile );
      exit(-1);
    }
  }   else
    cfd = stdin;

  while ( !feof ( cfd ) )  {
    fgets ( cmd, CMDS, cfd );
    writecmd ( tfd, cmd );
  }

  fclose(cfd);
  close(tfd);
  exit(0);
}
