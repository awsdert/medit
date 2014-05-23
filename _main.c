#include "_main.h"

Ihandle* meMkList( Icallback func, ... )
{
  Ihandle *box = IupHbox( NULL ),
    *sbox = IupScrollBox( box ),
    *frame = NULL, *ih = NULL;
  va_list args;
  char *title = NULL;
  va_start( args, func );
  if ( !func )
    func = (Icallback)meListCB;
  do
  {
    title = va_arg( args, char* );
    if ( !title )
      break;
    ih = IupLabel( title );
    IupSetAttribute( ih, IUP_EXPAND, IUP_YES );
    IupSetAttribute( ih, IUP_ALIGNMENT, "ACENTER:ACENTER" );
    frame = IupFrame( ih );
    IupSetAttribute( frame, IUP_SIZE, "40x20" );
    IupAppend( box, frame );
    IupSetCallback( ih, IUP_BUTTON_CB, func );
  }
  while ( 1 );
  va_end(   args );
  IupSetAttribute( box, IUP_SIZE, "100x30" );
  IupSetAttribute( sbox, IUP_SIZE, "100x30" );
  IupSetAttribute( box, IUP_GAP, "10" );
  IupSetAttribute( box, IUP_EXPAND, IUP_YES );
  IupSetAttribute( sbox, IUP_EXPAND, IUP_HORIZONTAL );
  return sbox;
}

int meListCB( Ihandle *ih, int button, int pressed, int x, int y, char* status )
{
  char *attr = NULL, tmp[_CVTBUFSIZE] = {0}, fpn[ _CVTBUFSIZE ] = {0};
  Ihandle *ph = IupGetParent(ih),
    *box   = IupGetParent( ph ),
    *sbox  = IupGetParent( box ),
    *frame = NULL;
  int i = 0, c = IupGetChildCount( box ), dec, sign;
  double dx = 0.1f, px = 0.0f, nx = 0.0f, lx = 0.01f, xmax = 1.0f, xmin = 0.0f;
  static int prvx = 0, drag = 0;
  if ( !pressed )
  {
    if ( !drag )
    {
      for ( ; i < c; ++i )
      {
        frame = IupGetChild( box, i );
        IupSetAttribute( frame, "SUNKEN", IUP_NO );
        IupRedraw( frame, 0 );
      }
      IupSetAttribute( ph, "SUNKEN", IUP_YES );
      IupRedraw( ph, 0 );
      IupFlush();
    }
    else
    {
      attr = IupGetAttribute( sbox, IUP_POSX );
      if ( attr && attr[0] )
        px = atof( attr );
      attr = IupGetAttribute( sbox, IUP_DX );
      if ( attr && attr[0] )
        dx = atof( attr );
      attr = IupGetAttribute( sbox, "LINEX" );
      if ( attr && attr[0] )
        lx = atof( attr );
      attr = IupGetAttribute( sbox, IUP_XMAX );
      if ( attr && attr[0] )
        xmax = atof( attr );
      attr = IupGetAttribute( sbox, IUP_XMIN );
      if ( attr && attr[0] )
        xmin = atof( attr );
      x = ( prvx < x ) ? (x - prvx) : -(prvx - x);
      nx = x;
      nx = px + ( nx * lx );
      xmax -= dx;
      if ( nx > xmax )
        nx = xmax;
      else if ( nx < xmin )
        nx = xmin;
      px = nx - px;
      ftoa( nx, fpn, DBL_DIG );
      attr = fpn;
      IupSetAttribute( sbox, IUP_POSX, fpn );
      IupRefresh( sbox );
      IupRedraw( sbox, 1 );
      IupFlush();
    }
    drag = 0;
  }
  else if ( !drag )
  {
    prvx = x;
    drag = 1;
  }
  return IUP_DEFAULT;
}
