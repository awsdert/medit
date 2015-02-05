#include "../_guiMain.h"
#ifdef HLIST
typedef int ( *_iupListAction )( Ihandle *ih, char *text, int item, int state );
int _iupHList_OnAction( Ihandle *ih, char *text, int item, int state );
int _iupHList_OnButton( Ihandle *ih, int button, int pressed, int x, int y,
                        char* status );
int _iupHList_OnMotion( Ihandle *sbox, int x, int y, char* status );
int _iupHList_OnIdle( Ihandle *ih );
Ihandle* _iupHList_NewItem( char *title )
{
  Ihandle *lbl = IupLabel( title ), *fset = IupFrame( lbl );
  return fset;
}
Ihandle* IupHList( void )
{
  Ihandle *layout = IupHbox( NULL ),
           *ih = IupScrollBox( layout );
  IupSetInt( ih, IUP_VALUE, 0 );
  IupSetAttribute( ih, IUP_SIZE, "100x25" );
  IupSetAttribute( ih, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetInt( layout, IUP_GAP, 10 );
  IupSetAttribute( layout, IUP_EXPAND, IUP_YES );
  IupSetCallback( ih, "_IUP_BUTTON_CB", ( Icallback )_iupHList_OnButton );
  IupSetCallback( ih, "_IUP_MOTION_CB", ( Icallback )_iupHList_OnMotion );
  IupSetCallback( ih, "_IUP_IDLE_ACTION", _iupHList_OnIdle );
  return ih;
}
int _iupHList_OnAction( Ihandle *ih, char *text, int item, int state )
{
  Ihandle *layout = IupGetChild( ih, 0 ),
           *fset = IupGetChild( layout, item );
  _iupListAction func = ( _iupListAction )IupGetCallback( ih, IUP_ACTION );
  IupSetAttribute( fset, "SUNKEN", ( state == 0 ) ? IUP_NO : IUP_YES );

  if ( IupGetAttribute( ih, "AUTOREDRAW" ) == IUP_YES )
  {
    IupRedraw( fset, 1 );
    IupFlush();
  }

  return IUP_DEFAULT;
}
int _iupHList_OnButton( Ihandle *ih, int button, int pressed, int x, int y,
                        char* status )
{
  Ihandle *layout = IupGetChild( ih, 0 ), *zero = IupGetChild( layout, 0 ),
           *lbl = NULL, *fset = NULL;
  char attr[20] = {0}, *tok = NULL, *tmp = NULL;
  int i = 0, c = IupGetChildCount( layout ), px = 0, py = 0, cx = 0, cy = 0;
  div_t res = {0};

  if ( pressed )
  {
    if ( iup_isbutton1( status ) )
    {
      IupSetInt( ih, "_IUP_START_X", x );
      IupSetInt( ih, "_IUP_START_Y", y );
      IupSetAttribute( ih, "_IUP_DRAG_SB", "1" );
    }
  }
  else if ( iup_isbutton1( status ) )
  {
    px += x;
    strncpyi( attr, 20, IupGetAttribute( ih, "CLIENTOFFSET" ) );
    tmp = strtok_s( attr, "x", &tok );
    cx = atoi( tmp );
    tmp = strtok_s( attr, "x", &tok );
    cy = atoi( tmp );

    for ( ; i < c; ++i )
    {
      fset = IupGetChild( layout, i );
      strncpyi( attr, 20, IupGetAttribute( fset, "POSITION" ) );
      tmp = strtok_s( attr, "x", &tok );
      px = cx + atoi( tmp );
      tmp = strtok_s( NULL, "x", &tok );
      py = cy + atoi( tmp );

      if ( x < px && y < py )
      {
        break;
      }
    }

    if ( i < c )
    {
      IupSetInt( ih, IUP_VALUE, i + 1 );
      IupSetAttribute( ih, "_IUP_DRAG_SB", NULL );
      _iupHList_OnValueChanged( ih );
    }
  }

  return IUP_DEFAULT;
}
int _iupHList_OnMotion( Ihandle *ih, int x, int y, char* status )
{
  Ihandle *layout = IupGetChild( ih, 0 );
  int sx, sy, dx, dy, px, py;

  if ( iup_isbutton1( status ) &&
       IupGetAttribute( ih, "_IUP_DRAG_SB" ) )
  {
    sx = IupGetInt( ih, "_IUP_START_X" );
    sy = IupGetInt( ih, "_IUP_START_Y" );
    dx = x - sx;
    dy = y - sy;
    px = IupGetInt( ih, "_IUP_START_POSX" ) - dx;
    py = IupGetInt( ih, "_IUP_START_POSY" ) - dy;
    IupSetInt( ih, IUP_POSX, px ); /* drag direction is opposite to scrollbar */
    IupSetInt( ih, IUP_POSY, py );
    IupSetInt( layout, IUP_X, -px );
    IupSetInt( layout, IUP_Y, -py );
  }

  IupRefresh( ih );
  IupFlush();
  return IUP_DEFAULT;
}
int _iupHList_OnIdle( Ihandle *ih )
{
  int prvValue = IupGetInt( ih, "_IUP_PRV_VALUE" ), prvCount = IupGetInt( ih,
                 "_IUP_PRV_COUNT" );
  Ihandle *fset, *layout = IupGetChild( ih, 0 );
  int i = prvCount + 1, curValue = IupGetInt( ih, IUP_VALUE ), curCount = 0;
  char *iText = IupGetAttribute( ih, "APPENDITEM" );

  if ( iText || ( iText = IupGetAttributeId( ih, "INSERTITEM", i ) ) )
  {
    IupAppend( layout, _iupHList_NewItem( iText ) );
    IupSetAttributeId( ih, NULL, i, iText );
    IupSetAttribute( ih, "APPENDITEM", NULL );
    curCount = ++prvCount;
  }
  else
  {
    curCount = prvCount;

    for ( i = prvCount; i > 0; --i )
    {
      iText = IupGetAttributeId( ih, "INSERTITEM", i );

      if ( iText )
      {
        IupInsert( layout, IupGetChild( layout, i - 1 ),
                   _iupHList_NewItem( iText ) );
        ++prvCount;
      }
    }

    for ( i = 1; i <= prvCount; ++i )
    {
      if ( !IupGetAttributeId( ih, NULL, i ) )
      {
        break;
      }

      IupSetAttributeId( ih, NULL, i,
                         IupGetAttribute( IupGetChild( IupGetChild( layout, i - 1 ), 0 ), IUP_TITLE ) );
    }

    curCount = prvCount - i;

    for ( ; i <= prvCount; ++i )
    {
      fset = IupGetChild( layout, i - 1 );
      IupUnmap( fset );
      IupDestroy( fset );
      IupSetAttributeId( ih, NULL, i, NULL );
    }
  }

  if ( prvValue != curValue || prvCount != curCount )
  {
    _iupHList_OnValueChanged( ih );
  }

  prvValue = curValue;
  prvCount = curCount;
  return IUP_DEFAULT;
}
int _iupHList_OnValueChanged( Ihandle *ih )
{
  Icallback  func = IupGetCallback( ih, "VALUECHANGED" );
  Ihandle *layout = IupGetChild( ih, 0 ), *fset = NULL;
  int i = 0, c = IupGetChildCount( layout ), v = IupGetInt( ih, IUP_VALUE );

  for ( i = 0; i < c; ++i )
  {
    _iupHList_OnAction( ih, IupGetAttributeId( ih, NULL, i ), i,
                        ( i == v ) ? 1 : 0 );
  }

  if ( func )
  {
    func( ih );
  }

  return IUP_DEFAULT;
}
#endif
