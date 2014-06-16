#include "_main.h"

Ihandle* meMkList( Icallback func, ... )
{
  Ihandle *hbox = IupHbox( NULL ),
    *sbox = IupScrollBox( hbox ),
    *frame = NULL, *ih = NULL;
  va_list args;
  char *title = NULL;
  va_start( args, func );
  do
  {
    title = va_arg( args, char* );
    if ( !title )
      break;
    ih = IupLabel( title );
    IupSetAttribute( ih, IUP_EXPAND, IUP_YES );
    IupSetAttribute( ih, IUP_ALIGNMENT, "ACENTER:ACENTER" );
    frame = IupFrame( NULL );
    IupAppend( frame, ih );
    IupSetAttribute( frame, IUP_ACTIVE, IUP_NO );
    IupSetAttribute( frame, IUP_BORDER, IUP_YES );
    IupSetAttribute( frame, IUP_BGCOLOR, IupGetAttribute( NULL, "DLGBGCOLOR" ));
    IupSetAttribute( frame, IUP_FGCOLOR, IupGetAttribute( NULL, "DLGFGCOLOR" ));
    IupSetAttribute( ih, IUP_BGCOLOR, IupGetAttribute( NULL, "DLGBGCOLOR" ));
    IupSetAttribute( ih, IUP_FGCOLOR, IupGetAttribute( NULL, "DLGFGCOLOR" ));
    IupSetAttribute( frame, IUP_SCROLLBAR, IUP_NO );
    IupSetAttribute( frame, IUP_SIZE, "40x" );
    IupAppend( hbox, frame );
    IupSetCallback( frame, IUP_BUTTON_CB, NULL );
    IupSetCallback( frame, "ME_BUTTON_CB", func );
  }
  while ( 1 );
  va_end(   args );
  IupSetCallback( sbox, IUP_BUTTON_CB, (Icallback)meListP_ButtonCB );
  IupSetCallback( sbox, IUP_MOTION_CB, (Icallback)meListP_MotionCB );
  IupSetAttribute( sbox, IUP_SIZE, "100x25" );
  IupSetAttribute( hbox, IUP_GAP, "10" );
  IupSetAttribute( hbox, IUP_EXPAND, IUP_YES );
  IupSetAttribute( sbox, IUP_EXPAND, IUP_HORIZONTAL );
  return sbox;
}

int meListP_ButtonCB( Ihandle *sbox, int button, int pressed, int x, int y, char* status )
{
  Ihandle *hbox = IupGetChild( sbox, 0 ), *zero = IupGetChild( hbox, 0 ),
    *ih = NULL, *frame = NULL, *lbl;
  char attr[20] = {0}, *tok = NULL, *tmp = NULL;
  int w = 0, h = 0, cx = IupGetInt( hbox, IUP_X ), child = 0;
  Icallback func = NULL;
  int i = 0, c = IupGetChildCount( hbox );
  typedef int (*motionCB)( Ihandle*, int, int, int, int, char* );
  if ( pressed )
  {
    if ( iup_isbutton1( status ) )
    {
      IupSetInt( sbox, "_IUP_START_X", x );
      IupSetInt( sbox, "_IUP_START_Y", y );
      IupSetAttribute(sbox, "_IUP_DRAG_SB", "1" );
    }
  }
  else if ( iup_isbutton1( status ) )
  {
    strcpy_s( attr, 20, IupGetAttribute( zero, IUP_RASTERSIZE ) );
    tmp = strtok_s( attr, "x", &tok );
    w = atoi( tmp ) + IupGetInt( hbox, IUP_GAP );
    tmp = strtok_s( NULL, "x", &tok );
    h = atoi( tmp );
    cx = -cx + x;
    child = ( cx > w ) ? cx / w : 0;
    ih = IupGetChild( hbox, child );
    func = IupGetCallback( ih, "ME_BUTTON_CB" );
    IupSetAttribute(sbox, "_IUP_DRAG_SB", NULL);
    for ( ; i < c; ++i )
    {
      frame = IupGetChild( hbox, i );
      lbl   = IupGetChild( frame, 0 );
      IupSetAttribute( frame, "SUNKEN", IUP_NO );
      IupRedraw( frame, 0 );
    }
    lbl   = IupGetChild( ih, 0 );
    IupSetAttribute( ih, "SUNKEN", IUP_YES );
    if ( func )
      ((motionCB)func)( ih, button, pressed, x, y, status );
    IupRedraw( ih, 0 );
    IupFlush();
  }
  return IUP_DEFAULT;
}
/*
int meListC_MotionCB( Ihandle *ph, int x, int y, char* status )
{
  Ihandle
    *hbox   = IupGetParent( ph ),
    *sbox  = IupGetParent( hbox ),
    *frame = NULL;
  int
    dx = IupGetInt( ph, IUP_DX ),
    dy = IupGetInt( ph, IUP_DY ),
    cx = IupGetInt( ph, IUP_X ),
    cy = IupGetInt( ph, IUP_Y ),
    wx = cx + dx,
    wy = cx + dy,
    lx = (IupGetInt( sbox, "LINEX") || 1),
    ly = (IupGetInt( sbox, "LINEY") || 1),
    px = (IupGetInt( ph, IUP_POSX ) || 1),
    py = (IupGetInt( ph, IUP_POSY ) || 1);
  if ( iup_isbutton1(status) )
    return meListP_MotionCB( sbox, x + cx, y + cy, status );
  return IUP_DEFAULT;
}
*/
int meListP_MotionCB( Ihandle *sbox, int x, int y, char* status )
{
  Ihandle *hbox = IupGetChild( sbox, 0 );
  int sx, sy, dx, dy, px, py;
  if (iup_isbutton1(status) &&
      IupGetAttribute(sbox, "_IUP_DRAG_SB"))
  {
    sx = IupGetInt(sbox, "_IUP_START_X");
    sy = IupGetInt(sbox, "_IUP_START_Y");
    dx = x - sx;
    dy = y - sy;
    px = IupGetInt(sbox, "_IUP_START_POSX") - dx;
    py = IupGetInt(sbox, "_IUP_START_POSY") - dy;
    IupSetInt(sbox, IUP_POSX, px);  /* drag direction is opposite to scrollbar */
    IupSetInt(sbox, IUP_POSY, py);
    IupSetInt( hbox, IUP_X, -px );
    IupSetInt( hbox, IUP_Y, -py );
  }
  IupRefresh( sbox );
  IupFlush();
  return IUP_DEFAULT;
}
