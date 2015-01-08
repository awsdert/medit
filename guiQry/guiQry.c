#include "guiQry.h"
GUI_QRY guiQry = {{NULL}};
void guiQry_OnLang( void )
{
#ifndef GUI_SHARED
  int q = 0, c = 0;
#endif
  const int len = NAME_MAX * 2;
  char atrue[NAME_MAX * 2] = {0}, afalse[NAME_MAX * 2] = {0};
  Ihandle *layout = IupGetChild( guiQry.hlQry, 0 );
  strcat_s( atrue,  len, appLang->a[ LNG_ALWAYS ].a );
  strcat_s( atrue,  len, " " );
  strcat_s( atrue,  len, appLang->a[ LNG_TRUE ].a );
  strcat_s( afalse, len, appLang->a[ LNG_ALWAYS ].a );
  strcat_s( afalse, len, " " );
  strcat_s( afalse, len, appLang->a[ LNG_FALSE ].a );
  IupSetAttribute( guiQry.main.fset, IUP_TITLE, appLang->a[ LNG_SEARCH ].a );
  IupSetAttribute( guiQry.hlQry, "1", appLang->a[ LNG_SEARCH ].a );
  IupSetStrAttribute( guiQry.hlQry, "2", atrue  );
  IupSetStrAttribute( guiQry.hlQry, "3", afalse );
#ifndef GUI_SHARED
  do
  {
    do
    {
      guiVal_SetGUI( &guiQry.val[q][c] );
#else
      guiVal_SetGUI( &guiQry.val );
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
extern GUI_CODE *guiCode;
void guiQry_OnInit( void )
{
  int i = 0, j = 2;
  guiQry.hlQry = IupHList(NULL);
  IupSetStrAttribute( guiQry.hlQry, "1", "?" );
  IupSetStrAttribute( guiQry.hlQry, "2", "+" );
  IupSetStrAttribute( guiQry.hlQry, "3", "-" );
  // ???
#ifdef GUI_SHARED
  IupAppend( guiQry.main.vb, guiQry.hlQry );
  IupMap( guiQry.hlQry );
  guiQry.cmp = guiCode->cmp;
  guiQry.val = guiCode->val;
#else
  guiQry.vbMain   = IupVbox(  guiQry.listMain, NULL );
  guiQry.fsetMain = IupFrame( guiQry.vbMain   );
  IupSetAttribute( guiQry.fsetMain, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute( guiQry.fsetMain, "FLOATING", IUP_YES );
  do
  {
    j = 0;
    do
    {
      appGui.val = &guiQry.val[i][j];
      guiVal_OnInit();
      IupAppend( guiQry.vbMain, appGui.val->fsetValue );
      IupMap( appGui.val->fsetValue );
      ++j;
    }
    while ( j < CMP_COUNT );
    ++i;
  }
  while ( i < QRY_COUNT );
  i = 0; j = 2;
#endif
  guiQry_OnLang();
}
extern GUI_VAL *guiVal;
int guiQry_OnShow( Ihandle *ih )
{
  IupSetAttribute( guiQry.main.fset, "FLOATING", IUP_NO );
  IupSetAttribute( guiQry.cmp.main.fset, "FLOATING", IUP_NO );
  IupShow( guiQry.cmp.main.fset  );
  if ( guiQry.q < QRY_COUNT )
  {
#ifndef GUI_SHARED
    guiVal_SetGUI( &guiQry.val[ guiQry.q ][ guiQry.c ] );
#else
    guiVal_SetGUI( &guiQry.val );
#endif
    IupSetAttribute( guiVal->value.fset, "FLOATING", IUP_NO );
    IupShow( guiVal->value.fset );
  }
#ifdef GUI_SHARED
  IupShow( guiOrg.main.fset );
  IupSetAttribute( guiOrg.main.fset, "FLOATING", IUP_NO );
  IupSetAttribute( guiOrg.main.fset, IUP_TITLE,  NULL );
#endif
  return IUP_DEFAULT;
}

int guiQryMenu_OnClick( Ihandle *ph, int button, int pressed, int x, int y, char* status )
{
  int i = 0, c = 0, used = IupGetInt( ph, IUP_VALUE ) - 1;
  //search( used );
  return IUP_DEFAULT;
}
