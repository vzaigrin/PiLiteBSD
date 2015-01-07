#ifndef __LIBPILITE_H__
#define __LIBPILITE_H__

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

#endif  /* __LIBPILITE_H__ */
