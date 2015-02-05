#include "guiCode.h"
GUI_CODE *guiCode = NULL;
CODE *srcCode;
CODE *tmpCode;
void guiCode_SetGUI( GUI_CODE *code, CODE *src, CODE *tmp )
{
  guiCode = code;
}
void guiCode_OnLang( void )
{
  IupSetAttribute( guiCode->code.fset, IUP_TITLE, appLang->a[ LNG_CODE ].a );
  IupSetAttribute( guiCode->type, "1", appLang->a[ LNG_WRITE      ].a );
  IupSetAttribute( guiCode->type, "2", appLang->a[ LNG_COPY       ].a );
  IupSetAttribute( guiCode->type, "3", appLang->a[ LNG_ADD        ].a );
  IupSetAttribute( guiCode->type, "4", appLang->a[ LNG_REMOVE     ].a );
  IupSetAttribute( guiCode->type, "5", appLang->a[ LNG_COMPARISON ].a );
  IupSetAttribute( guiCode->type, "6", appLang->a[ LNG_JOKER      ].a );
  IupSetStrAttribute( guiCode->type, "7", "(M)" );
}
extern void guiCmp_OnInit( void );
extern void guiVal_OnInit( void );
void guiCode_OnInit( void )
{
  // We don't need this anywhere else so just leave it be
  guiSpin_OnInit( &guiCode->indx, NULL, NULL );
  // Initialise Code UI
  guiCode->type = IupHList( NULL );
  guiCode->cmp.main = guiCode->main;
  guiCmp_SetGUI( &guiCode->cmp );
  guiCmp_OnInit();
  guiCode->val.main = guiCode->main;
  guiVal_SetGUI( &guiCode->val );
  guiVal_OnInit();
  guiCode->code.vb  = IupVbox( guiCode->type, guiCode->indx.fset, NULL );
  IupAppend( guiCode->main.vb, guiCode->code.fset );
  IupMap( guiCode->code.fset );
  guiCode_OnLang();
}
int guiCode_OnShow( Ihandle *ih )
{
  IupSetAttribute( guiData, IUP_VISIBLE, IUP_YES );
  return IUP_DEFAULT;
}
