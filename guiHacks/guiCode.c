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
  IupSetAttribute( guiCode->code.fset, IUP_TITLE, appLang->x[ LNG_CODE ] );
  IupSetAttribute( guiCode->type, "1", appLang->x[ LNG_WRITE      ] );
  IupSetAttribute( guiCode->type, "2", appLang->x[ LNG_COPY       ] );
  IupSetAttribute( guiCode->type, "3", appLang->x[ LNG_ADD        ] );
  IupSetAttribute( guiCode->type, "4", appLang->x[ LNG_REMOVE     ] );
  IupSetAttribute( guiCode->type, "5", appLang->x[ LNG_COMPARISON ] );
  IupSetAttribute( guiCode->type, "6", appLang->x[ LNG_JOKER      ] );
  IupSetStrAttribute( guiCode->type, "7", "(M)" );
}
extern void guiCmp_OnInit( void );
extern void guiVal_OnInit( void );
void guiCode_OnInit( void )
{
  // We don't need this anywhere else so just leave it be
  guiSpin_OnInit( &guiCode->indx, NULL, NULL );
  // Initialise Code UI
  guiCode->type = IupHList(NULL);
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
