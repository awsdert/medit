#include "../guiMain.h"
void guiCmp_OnLang( void )
{
  GUI *gui = appGetGui();
  LANG const *lang = appGetLang();
  Ihandle *hbox = IupGetChild( gui->cmp->listCmp, 0 );
  IupSetAttribute( gui->cmp->listCmp,                 IUP_TITLE, lang->x[ LNG_COMPARISON ] );
  IupSetAttribute( guiGetListPos_Lbl( hbox, CMP_DUMP + 1 ), IUP_TITLE, lang->x[ LNG_DUMP ] );
  IupSetAttribute( guiGetListPos_Lbl( hbox, CMP_UNDO - 1 ), IUP_TITLE, lang->x[ LNG_UNDO ] );
  IupSetAttribute( guiGetListPos_Lbl( hbox, CMP_REDO - 1 ), IUP_TITLE, lang->x[ LNG_REDO ] );
}
void guiCmp_OnInit( void )
{
  GUI *gui = appGetGui();
  Ihandle *hbox;
  schar val = CMP_DUMP;
  int i = 0, c;
  Ihandle *child, *kid;
  gui->cmp->listCmp = meMkList( NULL, "D",
    "==", "!=", ">",  ">=", "<",  "<=",
    "&&", "!&&", "|",  "!|", "^",  "!^", "~",  "!~",
    "U", "R", NULL );
  hbox = IupGetChild( gui->cmp->listCmp, 0 );
  for
  (
    c = IupGetChildCount( hbox )
    ; i < c; ++i, ++val
  )
  {
    child = IupGetChild( hbox,  i );
      kid = IupGetChild( child, 0 );
    IupSetInt( child, IUP_VALUE, val );
  }
  IupSetAttribute( gui->cmp->listCmp, IUP_SIZE, "100x30" );
  IupSetAttribute( gui->cmp->listCmp, IUP_EXPAND, IUP_HORIZONTAL );
  IupAppend( gui->org.main.vb, gui->cmp->listCmp );
  IupMap( gui->cmp->listCmp );
  guiCmp_OnLang();
}
