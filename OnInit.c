#include "_main.h"
int meCmp_OnInit( Ihandle *ih )
{
  MEGUI *gui = meGetGui();
  Ihandle *hbox;
  LANG *lang = meGetLang();
  char val[CMP_COUNT + 3] = {
    (char)CMP_DUMP,
    (char)CMP_EQ, (char)CMP_NE,
    (char)CMP_MT, (char)CMP_ME,
    (char)CMP_LT, (char)CMP_LE,
    (char)CMP_IA, (char)CMP_NA,
    (char)CMP_IO, (char)CMP_NO,
    (char)CMP_IX, (char)CMP_NX,
    (char)CMP_IR, (char)CMP_NR,
    (char)CMP_UNDO, (char)CMP_REDO
  };
  int i = 0, c;
  Ihandle *child, *kid;
  gui->cmp.main_list = meMkList( NULL,
    lang->x[ LANG_DUMP ],
    "==", "!=", ">",  ">=", "<",  "<=",
    "&&", "!&&", "|",  "!|", "^",  "!^", "~",  "!~",
    lang->x[ LANG_UNDO ], lang->x[ LANG_REDO ], NULL );
  hbox = IupGetChild( gui->cmp.main_list, 0 );
  for
  (
    c = IupGetChildCount( hbox )
    ; i < c; ++i, ++val[0]
  )
  {
    child = IupGetChild( hbox, i );
    kid = IupGetChild( child, 0 );
    IupSetInt( child, IUP_VALUE, val[i] + 1 );
  }
  IupAppend( gui->cmp.main_vbox, gui->cmp.main_list );
  IupMap( gui->cmp.main_list );
  IupFlush();
  return IUP_DEFAULT;
}
int meVal_OnInit( Ihandle *ih )
{
  MEGUI *gui = meGetGui();
  Ihandle *hbox;
  int i = 0;
  Ihandle *child, *kid;
  gui->val.type_list = meMkList( NULL,
    "signed char",      "unsigned char",
    "signed short",     "unsigned short",
    "signed long",      "unsigned long",
    "signed long long", "unsigned long long",
    "signed int",       "unsigned int",
    "float",  "double", " long double", NULL );
  hbox = IupGetChild( gui->val.type_list, 0 );
  for ( ; i < VAL_COUNT; ++i )
  {
    child = IupGetChild( hbox, i );
    kid = IupGetChild( child, 0 );
    IupSetInt( child, IUP_VALUE, i );
  }
  IupAppend( gui->val.main_vbox, gui->val.type_list );
  IupMap( gui->val.type_list );
  IupFlush();
  return IUP_DEFAULT;
}
int meSea_OnInit( Ihandle *ih )
{
  MEGUI *gui = meGetGui();
  LANG *lang = meGetLang();
  Ihandle *hbox;
  const int len = NAME_LEN * 2;
  char atrue[NAME_LEN * 2] = {0}, afalse[NAME_LEN * 2] = {0};
  int i = 2, j = 0;
  strcat_s( atrue, len, lang->x[ LANG_ALWAYS ] );
  strcat_s( atrue, len, " " );
  strcat_s( atrue, len, lang->x[ LANG_TRUE ] );
  strcat_s( afalse, len, lang->x[ LANG_ALWAYS ] );
  strcat_s( afalse, len, " " );
  strcat_s( afalse, len, lang->x[ LANG_FALSE ] );
  gui->sea.main_list =
    meMkList( NULL, lang->x[ LANG_SEARCH ], atrue, afalse, NULL );
  hbox = IupGetChild( gui->sea.main_list, 0 );
  for ( ; i > 0; --i, ++j )
    IupSetInt( IupGetChild( hbox, i ), IUP_VALUE, j );
  IupAppend( gui->sea.main_vbox, gui->sea.main_list );
  IupMap( gui->sea.main_list );
  IupFlush();
  return IUP_DEFAULT;
}
