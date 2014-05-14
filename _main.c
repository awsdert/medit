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
    func = meListCB;
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

int meListCB( Ihandle *ih )
{
  Ihandle *ph = IupGetParent(ih),
    *box   = IupGetParent(ph),
    *frame = NULL;
  int i = 0, c = IupGetChildCount( box );
  for ( ; i < c; ++i )
  {
    frame = IupGetChild( box, i );
    IupSetAttribute( frame, "SUNKEN", IUP_NO );
    IupRedraw( frame, 0 );
  }
  IupSetAttribute( ph, "SUNKEN", IUP_YES );
  IupRedraw( ph, 0 );
  IupFlush();
  return IUP_DEFAULT;
}
