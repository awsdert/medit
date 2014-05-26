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
  //IupSetCallback( sbox, IUP_BUTTON_CB, (Icallback)meListSbox_ButtonCB );
  //IupSetCallback( sbox, IUP_MOTION_CB, (Icallback)meList_MotionCB );
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
    if ( button != IUP_BUTTON1 )
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
    //*
    else
      IupSetAttribute(sbox, "_IUP_DRAG_SB", NULL);
    //*/
  }
  //*
  else if ( button == IUP_BUTTON1 )
  {
    IupSetInt(sbox, "_IUP_START_X", x);
    //IupSetInt(sbox, "_IUP_START_Y", y);
    IupSetInt(sbox, "_IUP_START_POSX", IupGetInt(sbox, "POSX"));
    //IupSetInt(sbox, "_IUP_START_POSY", IupGetInt(sbox, "POSY"));
    IupSetAttribute(sbox, "_IUP_DRAG_SB", "1");
  }
  //*/
  return IUP_DEFAULT;
}
int meListSbox_ButtonCB( Ihandle *sbox, int button, int pressed, int x, int y, char* status )
{
  if ( button == IUP_BUTTON1 )
  {
    if ( !pressed )
      IupSetAttribute(sbox, "_IUP_DRAG_SB", NULL);
    else if ( !IupGetAttribute( sbox, "_IUP_DRAG_SB" ) )
    {
      IupSetInt(sbox, "_IUP_START_X", x);
      IupSetInt(sbox, "_IUP_START_Y", y);
      IupSetFloat(sbox, "_IUP_START_POSX", IupGetFloat(sbox, "POSX"));
      IupSetFloat(sbox, "_IUP_START_POSY", IupGetFloat(sbox, "POSY"));
      IupSetAttribute(sbox, "_IUP_DRAG_SB", "1");
    }
  }
  return IUP_DEFAULT;
}
int meList_ScrollCB( Ihandle *ih, int what, float posx, float posy )
{
  IupSetFloat( ih, "_POSX", posx );
  IupSetFloat( ih, "_POSY", posy );
  return IUP_DEFAULT;
}

int meList_MotionCB( Ihandle *sbox, int x, int y, char* status )
{
  /*
  Ihandle *ph = IupGetParent( ih ),
    *hbox = IupGetParent( ph ),
    *sbox = IupGetParent( hbox );
  //*/
  if (iup_isbutton1(status) &&
      IupGetAttribute(sbox, "_IUP_DRAG_SB"))
  {
    int start_x = IupGetInt(sbox, "_IUP_START_X");
    int start_y = IupGetInt(sbox, "_IUP_START_Y");
    int dx = x - start_x;
    int dy = y - start_y;
    int posx = IupGetInt(sbox, "_IUP_START_POSX");
    int posy = IupGetInt(sbox, "_IUP_START_POSY");
    IupSetInt(sbox, "POSX", posx-dx);  /* drag direction is opposite to scrollbar */
    IupSetInt(sbox, "POSY", posy-dy);
    meList_ScrollCB(sbox, 0, IupGetFloat(sbox, "POSX"), IupGetFloat(sbox, "POSY"));
  }
  return IUP_DEFAULT;
}
