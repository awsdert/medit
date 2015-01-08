#include "guiCmp.h"
GUI_CMP *guiCmp = NULL;
void guiCmp_SetGUI( GUI_CMP *cmp ) { guiCmp = cmp; }
void guiCmp_OnLang( void )
{
  Ihandle *layout = IupGetChild( guiCmp->listCmp, 0 );
  IupSetAttribute( guiCmp->listCmp, IUP_TITLE, appLang->a[ LNG_COMPARISON ].a );
  IupSetAttribute(    guiCmp->listCmp,  "1", appLang->a[ LNG_DUMP ].a );
  IupSetAttribute(    guiCmp->listCmp, "12", appLang->a[ LNG_UNDO ].a );
  IupSetAttribute(    guiCmp->listCmp, "13", appLang->a[ LNG_REDO ].a );
}
void guiCmp_OnInit( void )
{
  Ihandle *layout;
  schar val = CMP_DUMP;
  int i = 0, c;
  Ihandle *child, *kid;
  guiCmp->listCmp = IupHList(NULL);
  IupSetAttribute(    guiCmp->listCmp,  "1", appLang->a[ LNG_DUMP ].a );
  IupSetStrAttribute( guiCmp->listCmp,  "2", "==" );
  IupSetStrAttribute( guiCmp->listCmp,  "3", "!=" );
  IupSetStrAttribute( guiCmp->listCmp,  "4",  ">" );
  IupSetStrAttribute( guiCmp->listCmp,  "5", ">=" );
  IupSetStrAttribute( guiCmp->listCmp,  "6",  "<" );
  IupSetStrAttribute( guiCmp->listCmp,  "7", "<=" );
  IupSetStrAttribute( guiCmp->listCmp,  "8",  "^" );
  IupSetStrAttribute( guiCmp->listCmp,  "9", "!^" );
  IupSetStrAttribute( guiCmp->listCmp, "10",  "~" );
  IupSetStrAttribute( guiCmp->listCmp, "11", "!~" );
  IupSetAttribute(    guiCmp->listCmp, "12", appLang->a[ LNG_UNDO ].a );
  IupSetAttribute(    guiCmp->listCmp, "13", appLang->a[ LNG_REDO ].a );
  layout = IupGetChild( guiCmp->listCmp, 0 );
  for
  (
    c = IupGetChildCount( layout )
    ; i < c; ++i, ++val
  )
  {
    child = IupGetChild( layout,  i );
      kid = IupGetChild( child, 0 );
    IupSetInt( child, IUP_VALUE, val );
  }
  IupSetAttribute( guiCmp->listCmp, IUP_SIZE, "100x30" );
  IupSetAttribute( guiCmp->listCmp, IUP_EXPAND, IUP_HORIZONTAL );
  IupAppend( guiCmp->main.vb, guiCmp->listCmp );
  IupMap( guiCmp->listCmp );
  guiCmp_OnLang();
}
