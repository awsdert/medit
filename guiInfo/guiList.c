#include "../_guiMain.h"

Ihandle* meMkList( Icallback func, ... )
{
  Ihandle *hbox = IupHbox( NULL ),
          *sbox = IupScrollBox( hbox ),
         *frame = NULL, *ih = NULL;
  char   *title = NULL;
  char const *DLGFGCOLOR = IupGetAttribute( NULL, "DLGFGCOLOR" );
  char const *DLGBGCOLOR = IupGetAttribute( NULL, "DLGBGCOLOR" );
  va_list   va;
  va_start( va, func );
  title = va_arg( va, char* );
  while ( title )
  {
    ih = IupLabel( title );
    IupSetAttribute( ih, IUP_EXPAND, IUP_YES );
    IupSetAttribute( ih, IUP_ALIGNMENT, "ACENTER:ACENTER" );
    frame = IupFrame( ih );
    IupSetAttribute( frame, IUP_ACTIVE,      IUP_NO );
    IupSetAttribute( frame, IUP_BORDER,     IUP_YES );
    IupSetAttribute( ih,    IUP_FGCOLOR, DLGFGCOLOR );
    IupSetAttribute( frame, IUP_FGCOLOR, DLGFGCOLOR );
    IupSetAttribute( ih,    IUP_BGCOLOR, DLGBGCOLOR );
    IupSetAttribute( frame, IUP_BGCOLOR, DLGBGCOLOR );
    IupSetAttribute( frame, IUP_SCROLLBAR,   IUP_NO );
    IupSetAttribute( frame, IUP_SIZE,         "40x" );
    IupSetCallback(  frame, IUP_BUTTON_CB,     NULL );
    IupAppend( hbox, frame );
    title = va_arg( va, char* );
  }
  va_end( va );
  IupSetCallback( sbox, IUP_BUTTON_CB, (Icallback)meListP_ButtonCB );
  IupSetCallback( sbox, IUP_MOTION_CB, (Icallback)meListP_MotionCB );
  IupSetAttribute( sbox, IUP_SIZE, "100x25" );
  IupSetAttribute( hbox, IUP_GAP, "10" );
  IupSetAttribute( hbox, IUP_EXPAND, IUP_YES );
  IupSetAttribute( sbox, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetCallback(  sbox, "VALUECHANGED", func );
  return sbox;
}

int meListP_ButtonCB( Ihandle *sbox, int button, int pressed, int x, int y, char* status )
{
  Ihandle *hbox = IupGetChild( sbox, 0 ), *zero = IupGetChild( hbox, 0 ),
    *ih = NULL, *frame = NULL, *lbl;
  char attr[20] = {0}, *tok = NULL, *tmp = NULL;
  int w = 0, h = 0, px = IupGetInt( sbox, IUP_POSX ), gap = 0, kid = 0;
  Icallback func = NULL;
  int i = 0, c = IupGetChildCount( hbox );
  div_t res = {0};
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
    px += x;
    gap = IupGetInt( hbox, IUP_GAP );
    for ( ; i < c; ++i )
    {
      ih = IupGetChild( hbox, i );
      strcpy_s( attr, 20, IupGetAttribute( ih, IUP_RASTERSIZE ) );
      tmp = strtok_s( attr, "x", &tok );
      w += atoi( tmp );
      tmp = strtok_s( NULL, "x", &tok );
      h += atoi( tmp );
      if ( px < w )
        break;
      tok = NULL;
      w += gap;
      if ( px < w )
      {
        ih = NULL;
        break;
      }
    }
    if ( ih )
    {
      func = IupGetCallback( sbox, "VALUECHANGED" );
      IupSetInt( sbox, IUP_VALUE, i );
      IupSetAttribute( sbox, "VALUE_TITLE", IupGetAttribute( ih, IUP_TITLE ) );
      IupSetAttribute(sbox, "_IUP_DRAG_SB", NULL);
      for ( i = 0; i < c; ++i )
      {
        frame = IupGetChild( hbox, i );
        lbl   = IupGetChild( frame, 0 );
        IupSetAttribute( frame, "SUNKEN", IUP_NO );
        IupRedraw( frame, 0 );
      }
      lbl = IupGetChild( ih, 0 );
      IupSetAttribute( ih, "SUNKEN", IUP_YES );
      if ( func )
        func( ih );
      IupRedraw( ih, 0 );
      IupFlush();
    }
  }
  return IUP_DEFAULT;
}
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

