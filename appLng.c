#include "appLng.h"

static LANG const _langDef =
{{
  // Main list
  "Organisation",
  "Platform",
  "Target",
  "Profile",
  "Search",
  "Results",
  "Editor",
  "Hacks",
  "About",
  // Search Menu
  "Dump",
  "Undo",
  "Redo",
  // Other
  "Name",
  "File",
  "Endian",
  "System",
  "Little",
  "Big",
  "Path",
  "Type",
  "ID",
  "Memory",
  "Range",
  "Code",
  "Write",
  "Copy",
  "Add",
  "Remove",
  "Comparison",
  "Joker",
  "Value",
  "Always",
  "True",
  "False",
  // List
  "First",
  "Previous",
  "Next",
  "Final",
  "New",
  "Delete",
  "Move",
  // Data <> GUI Actions
  "Apply",
  "Reset",
  "Load",
  "Save"
}};
static LANG _lang = {{{0}}};
LANG const* appGetLang( void ) { return &_lang; }
LANG const* appGetDefaultLang( void ) { return &_langDef; }
char const* appGetText(  LNG_T text ) { return _lang.x[ text ]; }

void appLoadLang( char *name )
{
  Ipipe pipe = {0};
  char fname[ FILENAME_MAX ] = {0};
  int  r = (name && name[0]) ? _strcmpi( name, "en" ) : 0;
  if ( r != 0 )
  {
    memset(   fname, 0, FILENAME_MAX );
    strcat_s( fname,    FILENAME_MAX, "lang"   );
    strcat_s( fname,    FILENAME_MAX, DIR_SEP  );
    strcat_s( fname,    FILENAME_MAX, name );
    strcat_s( fname,    FILENAME_MAX, ".applang" );
    pipe = ipMkFile( fname, 0666, SHARE_READ, ACTION_OPEN_NEW, NULL );
    ipRdPipe( &pipe, &_lang, sizeof(LANG) );
    ipShutPipe( &pipe );
  }
  else
  {
    memset( name, 0, strlen( name ) );
    name[0] = 'e';
    name[1] = 'n';
    _lang = _langDef;
  }
}
