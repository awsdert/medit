#include "../guiMain.h"
void guiCode_OnLang( GUI_CODE *code )
{
  GUI *gui = appGetGui();
  LANG const *lang = appGetLang();
  Ihandle *hb = IupGetChild( code->listCode, 0 );
  // Change Code UI Language
  IupSetAttribute( code->code.fset, IUP_TITLE, lang->x[ LNG_CODE ] );
  IupSetAttribute( guiGetListPos_Lbl( hb, CODE_W     ), IUP_TITLE, lang->x[ LNG_WRITE      ] );
  IupSetAttribute( guiGetListPos_Lbl( hb, CODE_CPY   ), IUP_TITLE, lang->x[ LNG_COPY       ] );
  IupSetAttribute( guiGetListPos_Lbl( hb, CODE_INC   ), IUP_TITLE, lang->x[ LNG_ADD        ] );
  IupSetAttribute( guiGetListPos_Lbl( hb, CODE_DEC   ), IUP_TITLE, lang->x[ LNG_REMOVE     ] );
  IupSetAttribute( guiGetListPos_Lbl( hb, CODE_CMP   ), IUP_TITLE, lang->x[ LNG_COMPARISON ] );
  IupSetAttribute( guiGetListPos_Lbl( hb, CODE_JOKER ), IUP_TITLE, lang->x[ LNG_JOKER      ] );
}
void guiCode_OnInit( GUI_CODES *codes )
{
  GUI *gui = appGetGui();
  GUI_CODE *code = &codes->code;
  // We don't need this anywhere else so just leave it be
  guiSpin_OnInit( &code->spinVal, NULL, NULL );
  // Initialise Code UI
  code->listCode = meMkList( NULL, "=#", "<>", "++", "--", "?", "!", "(M)", NULL );
  gui->cmp = &code->cmp;
  gui->val = &code->val;
  code->main     = codes->main;
  code->cmp.main = codes->main;
  code->val.main = codes->main;
  code->code.vb  = IupVbox( code->listCode, code->spinVal.fset, NULL );
  IupAppend( code->main.vb, code->code.fset );
  IupMap( code->code.fset );
  guiCmp_OnInit();
  guiVal_OnInit();
  guiCode_OnLang( code );
}
int guiCode_OnShow( Ihandle *ih )
{
  GUI *gui = appGetGui();
  IupSetAttribute( gui->listSet, IUP_VISIBLE, IUP_YES );
/* FIXME: Create and program these events
  IupSetCallback( gui->code.listSet, "_:)_BUTTON_CB", guiCodeApply_OnButton );
  IupSetCallback( gui->code.listSet, "_:(_BUTTON_CB", guiCodeReset_OnButton );
  IupSetCallback( gui->code.listSet, "_/\\_BUTTON_CB", guiCodeLoad_OnButton );
  IupSetCallback( gui->code.listSet, "_\\/_BUTTON_CB", guiCodeSave_OnButton );
*/
  return IUP_DEFAULT;
}
