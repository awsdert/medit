#include "guiVal.h"
GUI_VAL *guiVal = NULL;
void guiVal_SetGUI( GUI_VAL *val ) { guiVal = val; }
void guiVal_OnLang( void )
{
  IupSetAttribute( guiVal->value.fset, IUP_TITLE, appLang->a[ LNG_VALUE ].a );
  IupSetAttribute( guiVal->hlType,     IUP_TITLE, appLang->a[ LNG_TYPE  ].a );
}
void guiVal_OnInit( void )
{
  Ihandle *layout;
  int i = 0;
  Ihandle *child, *kid;
  guiVal->tbValue  = IupText( NULL );
  guiVal->hlType = IupHList(NULL);
  IupSetStrAttribute( guiVal->hlType,  "1",  "schar" );
  IupSetStrAttribute( guiVal->hlType,  "2",  "uchar" );
  IupSetStrAttribute( guiVal->hlType,  "3",  "short" );
  IupSetStrAttribute( guiVal->hlType,  "4", "ushort" );
  IupSetStrAttribute( guiVal->hlType,  "5",    "int" );
  IupSetStrAttribute( guiVal->hlType,  "6",   "uint" );
  IupSetStrAttribute( guiVal->hlType,  "7",   "long" );
  IupSetStrAttribute( guiVal->hlType,  "8",  "ulong" );
  IupSetStrAttribute( guiVal->hlType,  "9",   "lint" );
  IupSetStrAttribute( guiVal->hlType, "10",  "ulint" );
  IupSetStrAttribute( guiVal->hlType, "11",    "fpn" );
  IupSetStrAttribute( guiVal->hlType, "12",    "dpn" );
  IupSetStrAttribute( guiVal->hlType, "13",    "lpn" );
  layout = IupGetChild( guiVal->hlType, 0 );
  for ( ; i < VAL_COUNT; ++i )
  {
    child = IupGetChild( layout, i );
    kid = IupGetChild( child, 0 );
    IupSetInt( child, IUP_VALUE, i );
  }
  guiVal->value.vb   = IupVbox(  guiVal->tbValue, guiVal->hlType, NULL );
  guiVal->value.fset = IupFrame( guiVal->value.vb );
  IupSetAttribute( guiVal->value.fset, IUP_SIZE, "100x30" );
  IupSetAttribute( guiVal->value.fset, IUP_EXPAND, IUP_HORIZONTAL );
  IupAppend( guiVal->main.vb, guiVal->value.fset );
  IupMap( guiVal->value.fset );
  guiVal_OnLang();
}
