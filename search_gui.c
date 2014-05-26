#include "search.h"
int meSea_OnShow( Ihandle *ih )
{
  MEGUI *hGui = meGetGui();
  MELANG *lang = meGetLang();
  char val[T_COUNT + 3] = {
    (char)T_DUMP,
    (char)T_EQ, (char)T_NE,
    (char)T_MT, (char)T_ME,
    (char)T_LT, (char)T_LE,
    (char)T_IA, (char)T_NA,
    (char)T_IO, (char)T_NO,
    (char)T_IX, (char)T_NX,
    (char)T_IR, (char)T_NR,
    (char)T_UNDO, (char)T_REDO
  }, tmp[2] = {0};
  int i = 0, c;
  Ihandle *child, *kid;
  if ( hGui->sea.stype_hbox )
    return IUP_DEFAULT;
  hGui->sea.stype_sbox = meMkList( (Icallback)meSeaMenu_OnClick,
    lang->seaMenu.dump,
    "==", "!=", ">",  ">=", "<",  "<=",
    "&&", "!&&", "|",  "!|", "^",  "!^", "~",  "!~",
    lang->seaMenu.undo, lang->seaMenu.redo );
  hGui->sea.stype_hbox = IupGetChild( hGui->sea.stype_sbox, 0 );

  for
  (
    c = IupGetChildCount( hGui->sea.stype_hbox )
    ; i < c; ++i, ++val[0]
  )
  {
    child = IupGetChild( hGui->sea.stype_hbox, i );
    kid = IupGetChild( child, 0 );
    tmp[0] = val[i] + 1;
    IupSetAttribute( kid, IUP_VALUE, tmp );
  }

  IupAppend( hGui->main.main_vb, hGui->sea.stype_sbox );
  //*
  IupMap( hGui->sea.stype_sbox );
  IupUpdate( hGui->main.main_vb );
  IupRefresh( hGui->main.main_vb );
  return IUP_DEFAULT;
}
int meSeaMenu_OnClick( Ihandle *ih, int button, int pressed, int x, int y, char* status )
{
  MEGUI  *hGui = meGetGui();
  MELANG *hLng = meGetLang();
  char *text;
  int i = 0, c = 0;
  Ihandle *ph = IupGetParent(ih);
  char  used = T_DUMP;
  meList_ButtonCB( ih, button, pressed, x, y, status );
  text = IupGetAttribute( ih, IUP_VALUE );
  used = text[0];
  return IUP_DEFAULT;
}

