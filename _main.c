#include "_main.h"

Ihandle* meMkList( Icallback func, ... )
{
  Ihandle *hbox = IupHbox( NULL ),
    *sbox = IupScrollBox( hbox ),
    *frame = NULL, *ih = NULL;
  va_list args;
  char *title = NULL;
  va_start( args, func );
  if ( !func )
    func = (Icallback)meList_ButtonCB;
  do
  {
    title = va_arg( args, char* );
    if ( !title )
      break;
    ih = IupLabel( title );
    IupSetAttribute( ih, IUP_EXPAND, IUP_YES );
    IupSetAttribute( ih, IUP_ALIGNMENT, "ACENTER:ACENTER" );
    frame = IupFrame( ih );
    IupSetAttribute( frame, IUP_SIZE, "40x" );
    IupAppend( hbox, frame );
    IupSetCallback( ih, IUP_BUTTON_CB, func );
  }
  while ( 1 );
  va_end(   args );
  IupSetCallback( sbox, IUP_MOTION_CB, (Icallback)meListSbox_MotionCB );
  IupSetAttribute( sbox, IUP_SIZE, "100x25" );
  IupSetAttribute( hbox, IUP_GAP, "10" );
  IupSetAttribute( hbox, IUP_EXPAND, IUP_YES );
  IupSetAttribute( sbox, IUP_EXPAND, IUP_HORIZONTAL );
  return sbox;
}
int meList_ButtonCB( Ihandle *ih, int button, int pressed, int x, int y, char* status )
{
  char *attr = NULL, tmp[_CVTBUFSIZE] = {0}, fpn[ _CVTBUFSIZE ] = {0};
  Ihandle *ph = IupGetParent(ih),
    *box   = IupGetParent( ph ),
    *sbox  = IupGetParent( box ),
    *frame = NULL;
  int i = 0, c = IupGetChildCount( box ), sx, sy, px, py, dx, dy;
  if ( !pressed )
  {
    if ( !IupGetAttribute(sbox, "_IUP_DRAG_SB" ))
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
      IupSetAttribute(sbox, "_IUP_DRAG_SB", NULL);
  }
  else
  {
    if ( button == IUP_BUTTON1 )
      IupSetAttribute(sbox, "_IUP_DRAG_SB", "1");
    meListSbox_MotionCB( sbox, x, y, status );
  }
  return IUP_DEFAULT;
}
int meListSbox_MotionCB( Ihandle *sbox, int x, int y, char* status )
{
  Ihandle *hbox = IupGetChild( sbox, 0 );
  int start_x, start_y, dx, dy, posx, posy;
  if (iup_isbutton1(status) &&
      IupGetAttribute(sbox, "_IUP_DRAG_SB"))
  {
    start_x = IupGetInt(sbox, "_IUP_START_X");
    start_y = IupGetInt(sbox, "_IUP_START_Y");
    dx = x - start_x;
    dy = y - start_y;
    posx = IupGetInt(sbox, "_IUP_START_POSX") - dx;
    posy = IupGetInt(sbox, "_IUP_START_POSY") - dy;
    IupSetInt(sbox, "POSX", posx);  /* drag direction is opposite to scrollbar */
    IupSetInt(sbox, "POSY", posy);
    if ( posx != 0 )
      posx = -posx;
    if ( posy != 0 )
      posy = -posy;
    IupSetInt( hbox, IUP_X, posx );
    IupSetInt( hbox, IUP_Y, posy );
  }
  IupRefresh( sbox );
  IupFlush();
  return IUP_DEFAULT;
}
