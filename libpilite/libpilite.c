#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

char *pilite_mode[] = { "ON", "OFF", "TOGGLE" };

int pilite_init ( char *tdev );
int pilite_close ( int tfd );
int pilite_clear ( int tfd );
int pilite_fill ( int tfd );
int pilite_point ( int tfd, int x, int y, int mode );
int pilite_line ( int tfd, int x1, int y1, int x2, int y2, int mode );
int pilite_box ( int tfd, int x1, int y1, int x2, int y2, int mode );
int pilite_fillbox ( int tfd, int x1, int y1, int x2, int y2, int mode );
int pilite_circle ( int tfd, int x, int y, int r, int mode );
int pilite_fillcircle ( int tfd, int x, int y, int r, int mode );


void writecmd( int fd, char *cmd )  {
  int cmdsize;

  cmdsize = strlen ( cmd );
  write (fd, cmd, cmdsize );

  return;
}


int pilite_init ( char *tdev )  {
  int tfd;
  struct termios cntrl;

  if ( (tfd = open ( tdev, O_RDWR | O_NONBLOCK | O_NOCTTY )) == -1 )  {
    return(-1);
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

  return(tfd);
}


int pilite_close ( int tfd )  {
  return(close(tfd));
}


int pilite_clear ( int tfd )  {
  writecmd ( tfd, "$$$ALL,OFF\r" );
  return(0);
}


int pilite_fill ( int tfd )  {
  writecmd ( tfd, "$$$ALL,ON\r" );
  return(0);
}


int pilite_point ( int tfd, int x, int y, int mode )  {
  char point[17];

  if ( (x < 0) || (x > 14) || (y < 0) || (y > 9) )  return(-1);
  if ( (mode < 0 ) || (mode > 2) ) return(-1);

  sprintf ( point, "$$$P%d,%d,%s\r", x, 10-y, pilite_mode[mode] );
  writecmd ( tfd, point );
  
  return(0);
}


int pilite_line ( int tfd, int x1, int y1, int x2, int y2, int mode )  {
  int t, d, incx, incy;
  int xerr=0, yerr=0, dx, dy, x, y;

  if ( (mode < 0 ) || (mode > 2) ) return(-1);

  if ( x1 < 0 )  x1 = 1;
  if ( x1 > 15 )  x1 = 14;
  if ( y1 < 0 )  y1 = 1;
  if ( y1 > 10 )  y1 = 14;

  if ( x2 < 0 )  x2 = 1;
  if ( x2 > 15 )  x2 = 14;
  if ( y2 < 0 )  y2 = 1;
  if ( y2 > 10 )  y2 = 14;

  dx = x2 - x1;
  dy = y2 - y1;

  if ( dx > 0 )  incx = 1;
  else if ( dx == 0 )  incx = 0;
       else  incx = -1;

  if ( dy > 0 )  incy = 1;
  else if ( dy == 0 )  incy = 0;
       else  incy = -1;

  dx = abs(dx);
  dy = abs(dy);

  if ( dx > dy )  d = dx;
  else  d = dy;

  x = x1;
  y = y1;
  for ( t = 0; t <= d+1; t++ )  {
    pilite_point ( tfd, x, y, mode );
    xerr += dx;
    yerr += dy;
    if ( xerr > d )  {
      xerr -= d;
      x += incx;
    }
    if ( yerr > d )  {
      yerr -= d;
      y += incy;
    }
  } 

  return(0);
}


int pilite_box ( int tfd, int x1, int y1, int x2, int y2, int mode )  {

  pilite_line ( tfd, x1, y1, x2, y1, mode );
  pilite_line ( tfd, x1, y1, x1, y2, mode );
  pilite_line ( tfd, x1, y2, x2, y2, mode );
  pilite_line ( tfd, x2, y1, x2, y2, mode );

  return(0);
}


int pilite_fillbox ( int tfd, int x1, int y1, int x2, int y2, int mode )  {
  int i, begin, end;

  begin = x1 < x2 ? x1 : x2;
  end = x1 > x2 ? x1 : x2;

  for ( i = begin; i <= end; i++ )
    pilite_line ( tfd, i, y1, i, y2, mode );

  return(0);
}


int plotcircle ( int tfd, int x, int y, int xc, int yc, int mode )  {
  int sx, sy, ex, ey, x1, y1;
  double ar = 1.0;

  sx = x * ar;
  ex = (x+1) * ar;

  sy = y * ar;
  ey = (y+1) * ar;

  for ( x1 = sx; x1 < ex; x1++ )  {
    pilite_point ( tfd, x1+xc, y+yc, mode );
    pilite_point ( tfd, x1+xc, yc-y, mode );
    pilite_point ( tfd, xc-x1, y+yc, mode );
    pilite_point ( tfd, xc-x1, yc-y, mode );
  }

  for ( y1 = sy; y1 < ey; y1++ )  {
    pilite_point ( tfd, y1+xc, x+yc, mode );
    pilite_point ( tfd, y1+xc, yc-x, mode );
    pilite_point ( tfd, xc-y1, x+yc, mode );
    pilite_point ( tfd, xc-y1, yc-x, mode );
  }

  return(0);
}


int pilite_circle ( int tfd, int xc, int yc, int r, int mode )  {
  int x, y, d;

  y = r;
  d = 3 - 2*r;
  for ( x = 0; x < y; )  {
    plotcircle ( tfd, x, y, xc, yc, mode );
    if ( d < 0 )  d += 4*x + 6;
    else  {
      d += 4*(x-y) + 10;
      y--;
    }
    x++;
  }  
  x = y;
  if ( y )  plotcircle ( tfd, x, y, xc, yc, mode );

  return(0);
}


int pilite_fillcircle ( int tfd, int x, int y, int r, int mode )  {

  while (r)  {
    pilite_circle ( tfd, x, y, r, mode );
    r--;
  }

  return(0);
}
