#include "../guiMain.h"
void guiVal_OnLang( void )
{
  GUI *gui = appGetGui();
  LANG const *lang = appGetLang();
  IupSetAttribute( gui->val->value.fset, IUP_TITLE, lang->x[ LNG_VALUE ] );
  IupSetAttribute( gui->val->listType,   IUP_TITLE, lang->x[ LNG_TYPE  ] );
}
void guiVal_OnInit( void )
{
  GUI *gui = appGetGui();
  Ihandle *hbox;
  int i = 0;
  Ihandle *child, *kid;
  gui->val->tbValue  = IupText( NULL );
  gui->val->listType = meMkList( NULL,
    "schar", "uchar",
    "short", "ushort",
    "int",   "uint",
    "long",  "ulong",
    "lint",  "ulint",
    "float", "double", "ldouble", NULL );
  hbox = IupGetChild( gui->val->listType, 0 );
  for ( ; i < VAL_COUNT; ++i )
  {
    child = IupGetChild( hbox, i );
    kid = IupGetChild( child, 0 );
    IupSetInt( child, IUP_VALUE, i );
  }
  gui->val->value.vb = IupVbox( gui->val->tbValue, gui->val->listType, NULL );
  gui->val->value.fset = IupFrame( gui->val->value.vb );
  IupSetAttribute( gui->val->value.fset, IUP_SIZE, "100x30" );
  IupSetAttribute( gui->val->value.fset, IUP_EXPAND, IUP_HORIZONTAL );
  IupAppend( gui->val->main.vb, gui->val->value.fset );
  IupMap( gui->val->value.fset );
  guiVal_OnLang();
}
