#include "search.h"
Ihandle *hSeaEdit = NULL;
Ihandle *hSeaText = NULL;
Ihandle *hSeaMenu = NULL;

void search( void )
{

}

int meSea_OnMk( Ihandle *ih )
{
  MELANG *lang = meGetLang();
  hSeaMenu = IupList( NULL );
  IupSetAttribute( hSeaMenu, "EXPAND", "HORIZONTAL"   );
  IupSetAttribute( hSeaMenu, "VISIBLECOLUMNS", "3"    );
  IupSetAttribute( hSeaMenu, "SIZE", "100x20"         );
  IupSetAttribute( hSeaMenu, "1",  lang->seaMenu.dump );
  IupSetAttribute( hSeaMenu, "2",  "=="  );
  IupSetAttribute( hSeaMenu, "3",  "!="  );
  IupSetAttribute( hSeaMenu, "4",  ">"   );
  IupSetAttribute( hSeaMenu, "5",  ">="  );
  IupSetAttribute( hSeaMenu, "6",  "<"   );
  IupSetAttribute( hSeaMenu, "7",  "<="  );
  IupSetAttribute( hSeaMenu, "8",  "&"   );
  IupSetAttribute( hSeaMenu, "9",  "!&"  );
  IupSetAttribute( hSeaMenu, "10", "|"   );
  IupSetAttribute( hSeaMenu, "11", "!|"  );
  IupSetAttribute( hSeaMenu, "10", "^"   );
  IupSetAttribute( hSeaMenu, "11", "!^"  );
  IupSetAttribute( hSeaMenu, "12", "~"   );
  IupSetAttribute( hSeaMenu, "13", "!~"  );
  IupSetAttribute( hSeaMenu, "14", lang->seaMenu.undo );
  IupSetAttribute( hSeaMenu, "15", lang->seaMenu.redo );
  return 0;
}
int meSeaMenu_OnClick( Ihandle *ih )
{
  char *text = IupGetAttribute( ih, "TEXT" );
  /* Default to dump */
  ucv   used = 0;
  if ( strcmp( text, "==" ) == 0 )
    used = 1;
  else if ( strcmp( text, "!=" ) == 0 )
    used = 2;
  else if ( strcmp( text, ">"  ) == 0 )
    used = 3;
  else if ( strcmp( text, ">=" ) == 0 )
    used = 4;
  else if ( strcmp( text, "<"  ) == 0 )
    used = 5;
  else if ( strcmp( text, "<=" ) == 0 )
    used = 6;
  return 0;
}
