#include "search.h"

void search( void )
{

}

int meSea_OnShow( Ihandle *ih )
{
  MEGUI *hGui = meGetGui();
  MELANG *lang = meGetLang();
  int i = 0, c = 0;
  Ihandle *child;
  if ( hGui->sea.stype_hbox )
  {
    IupDetach(  hGui->sea.stype_hbox );
    IupDestroy( hGui->sea.stype_hbox );
    hGui->sea.stype_hbox = NULL;
    return IUP_DEFAULT;
  }
  hGui->sea.stype_hbox = IupHbox(
    IupButton( lang->seaMenu.dump, NULL ),
    IupButton( "==",  NULL ),
    IupButton( "!=",  NULL ),
    IupButton( ">",   NULL ),
    IupButton( ">=",  NULL ),
    IupButton( "<",   NULL ),
    IupButton( "<=",  NULL ),
    IupButton( "&&",  NULL ),
    IupButton( "!&&", NULL ),
    IupButton( "|",   NULL ),
    IupButton( "!|",  NULL ),
    IupButton( "^",   NULL ),
    IupButton( "!^",  NULL ),
    IupButton( "~",   NULL ),
    IupButton( "!~",  NULL ),
    IupButton( lang->seaMenu.undo, NULL ),
    IupButton( lang->seaMenu.redo, NULL ) );
  IupSetAttribute( hGui->sea.stype_hbox, "EXPAND", "HORIZONTAL" );
  IupSetAttribute( hGui->sea.stype_hbox, "GAP", "10" );
  for
  (
    c = IupGetChildCount( hGui->sea.stype_hbox );
    i < c; ++i
  )
  {
    child = IupGetChild( hGui->sea.stype_hbox, i );
    IupSetAttribute( child, "SIZE", "40x20" );
    IupSetAttribute( child, "FLAT", "YES" );
    IupSetCallback(  child, "BUTTON_CB", meSeaMenu_OnClick );
  }
  hGui->sea.stype_sbox = IupScrollBox( hGui->sea.stype_hbox );
  IupSetAttribute( hGui->sea.stype_sbox, "SIZE", "300x30" );
  IupSetAttribute( hGui->sea.stype_sbox, "EXPAND", "HORIZONTAL" );
  IupAppend( hGui->main.main_vb, hGui->sea.stype_sbox );
  IupMap( hGui->sea.stype_sbox );
  IupShow( hGui->sea.stype_sbox );
  IupRefresh( hGui->main.main_vb );
  return IUP_DEFAULT;
}
int meSeaMenu_OnClick( Ihandle *ih )
{
  MEGUI *hGui = meGetGui();
  char *text = IupGetAttribute( NULL, "DLGBGCOLOR" );
  int i = 0, c = 0;
  Ihandle *child;
  /* Default to dump */
  ucv   used = 0;
  for
  (
    c = IupGetChildCount( hGui->sea.stype_hbox );
    i < c; ++i
  )
  {
    child = IupGetChild( hGui->sea.stype_hbox, i );
    IupSetAttribute( child, "FLAT", "YES" );
  }
  text = IupGetAttribute( ih, "TITLE" );
  IupSetAttribute( ih, "FLAT", "NO");
  if ( strcmp( text, "==" ) == 0 )
    used = 1;
  else if ( strcmp( text, "!=" ) == 0 )
    used = 2;
  else if ( strcmp( text, ">"  ) == 0 )
    used = 3;
  else if ( strcmp( text, ">=" ) == 0 )
    used = 4;
  else if ( strcmp( text, "<"  ) == 0 )
    used = 5;
  else if ( strcmp( text, "<=" ) == 0 )
    used = 6;
  return IUP_DEFAULT;
}
