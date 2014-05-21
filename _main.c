#include "_main.h"

Ihandle* meMkList( Icallback func, ... )
{
  Ihandle *box = IupHbox( NULL ),
    *sbox = IupScrollBox( box ),
    *frame = NULL, *lbl = NULL;
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
    lbl = IupLabel( title );
    IupSetAttribute( lbl, IUP_EXPAND, IUP_YES );
    IupSetAttribute( lbl, IUP_ALIGNMENT, "ACENTER:ACENTER" );
    frame = IupFrame( lbl );
    IupSetAttribute( frame, IUP_SIZE, "40x20" );
    IupAppend( box, frame );
    IupSetCallback( lbl, IUP_BUTTON_CB, func );
  }
  while ( 1 );
  va_end(   args );
  IupSetAttribute( box, "SIZE", "100x30" );
  IupSetAttribute( sbox, "SIZE", "100x30" );
  IupSetAttribute( box, IUP_GAP, "10" );
  IupSetAttribute( box, IUP_EXPAND, IUP_YES );
  IupSetAttribute( sbox, IUP_EXPAND, IUP_HORIZONTAL );
  return sbox;
}

int meListCB( Ihandle *ih, int button, int pressed, int x, int y, char* status )
{
  char *attr = NULL, tmp[_CVTBUFSIZE] = {0};
  Ihandle *ph = IupGetParent(ih),
    *box   = IupGetParent(ph),
    *frame = NULL;
  int i = 0, c = IupGetChildCount( box );
  double dx = 0.1f, px = 0.0f;
  if (!pressed)
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
    ph = IupGetParent( ph );
    attr = IupGetAttribute( ph, IUP_POSX );
    if ( attr && attr[0] )
      px = atof( attr );
    attr = IupGetAttribute( ph, IUP_DX );
    if ( attr && attr[0] )
      dx = atof( attr );
    px += (x * dx);
    _fcvt_s( tmp, _CVTBUFSIZE, px, 0, &c, &i );
    //IupMessage( attr, tmp );
    IupSetAttribute( ph, IUP_POSX, tmp );
    IupRedraw( ph, 1 );
    IupFlush();
  }
  return IUP_DEFAULT;
}
