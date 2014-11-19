#include "../guiQry.h"
void guiQry_OnLang( void )
{
  GUI *gui = appGetGui();
  LANG const *lang = appGetLang();
#ifndef GUI_SHARED
  int q = 0, c = 0;
#endif
  const int len = NAME_MAX * 2;
  char atrue[NAME_MAX * 2] = {0}, afalse[NAME_MAX * 2] = {0};
  Ihandle *hb = IupGetChild( gui->qry.listType, 0 );
  strcat_s( atrue,  len, lang->x[ LNG_ALWAYS ] );
  strcat_s( atrue,  len, " " );
  strcat_s( atrue,  len, lang->x[ LNG_TRUE ] );
  strcat_s( afalse, len, lang->x[ LNG_ALWAYS ] );
  strcat_s( afalse, len, " " );
  strcat_s( afalse, len, lang->x[ LNG_FALSE ] );
  IupSetAttribute( gui->qry.main.fset, IUP_TITLE, lang->x[ LNG_SEARCH ] );
  IupSetAttribute( guiGetListPos_Lbl( hb, QRY_ANY ), IUP_TITLE, lang->x[ LNG_SEARCH ] );
  IupSetAttribute( guiGetListPos_Lbl( hb, QRY_ANY ), IUP_TITLE, atrue  );
  IupSetAttribute( guiGetListPos_Lbl( hb, QRY_ANY ), IUP_TITLE, afalse );
#ifndef GUI_SHARED
  do
  {
    do
    {
      gui->val = &gui->qry.val[q][c];
#else
      gui->val = &gui->qry.val;
#endif
      guiVal_OnLang();
#ifndef GUI_SHARED
    }
    while ( c < CMP_COUNT );
    ++q;
    c = 0;
  }
  while ( q < QRY_COUNT );
#endif
}
void guiQry_OnInit( void )
{
  GUI *gui = appGetGui();
  LANG const *lang = appGetLang();
  Ihandle *hbox;
  int i = 0, j = 2;
  gui->qry.listType =
    meMkList( NULL, "?", "+", "-", NULL );
  hbox = IupGetChild( gui->qry.listType, 0 );
  // ???
#ifdef GUI_SHARED
  IupAppend( gui->qry.main.vb, gui->qry.listType );
  IupMap( gui->qry.listType );
  gui->qry.cmp = gui->hacks.codes.code.cmp;
  gui->qry.val = gui->hacks.codes.code.val;
#else
  gui->qry.vbMain   = IupVbox(  gui->qry.listMain, NULL );
  gui->qry.fsetMain = IupFrame( gui->qry.vbMain   );
  IupSetAttribute( gui->qry.fsetMain, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute( gui->qry.fsetMain, "FLOATING", IUP_YES );
  do
  {
    j = 0;
    do
    {
      gui->val = &gui->qry.val[i][j];
      guiVal_OnInit();
      IupAppend( gui->qry.vbMain, gui->val->fsetValue );
      IupMap( gui->val->fsetValue );
      ++j;
    }
    while ( j < CMP_COUNT );
    ++i;
  }
  while ( i < QRY_COUNT );
  i = 0; j = 2;
#endif
  while ( i < 3 )
  {
    IupSetInt( guiGetListPos_Lbl( hbox, i ), IUP_VALUE, j );
    ++i; --j;
  }
  guiQry_OnLang();
}
int guiQry_OnShow( Ihandle *ih )
{
  GUI *gui = appGetGui();
  IupSetAttribute( gui->qry.main.fset, "FLOATING", IUP_NO );
  IupSetAttribute( gui->qry.cmp.main.fset, "FLOATING", IUP_NO );
  IupShow( gui->qry.cmp.main.fset  );
  if ( gui->qry.q < QRY_COUNT )
  {
#ifndef GUI_SHARED
    gui->val = &gui->qry.val[ gui->qry.q ][ gui->qry.c ];
#else
    gui->val = &gui->qry.val;
#endif
    IupSetAttribute( gui->val->value.fset, "FLOATING", IUP_NO );
    IupShow( gui->val->value.fset );
  }
#ifdef GUI_SHARED
  IupShow( gui->org.main.fset );
  IupSetAttribute( gui->org.main.fset, "FLOATING", IUP_NO );
  IupSetAttribute( gui->org.main.fset, IUP_TITLE,  NULL );
#endif
  return IUP_DEFAULT;
}

int guiQryMenu_OnClick( Ihandle *ph, int button, int pressed, int x, int y, char* status )
{
  GUI  *gui = appGetGui();
  int i = 0, c = 0, used = IupGetInt( ph, IUP_VALUE ) - 1;
  //search( used );
  return IUP_DEFAULT;
}
