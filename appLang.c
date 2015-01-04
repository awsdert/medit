#include "appLng.h"

static LANG const _appDefLang =
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
  "Hack",
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
  "PDP",
  "Big",
  "Path",
  "Type",
  "ID",
  "Memory",
  "Address",
  "Size",
  "Depth",
  "Range",
  "Codes",
  "Code",
  "Write",
  "Copy",
  "Add",
  "Remove",
  "Comparison",
  "Joker",
  "(M)",
  "???",
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
static LANG _appLang = {{{0}}};
LANG const *appLang = &_appLang;

void appLoadLang( char *name )
{
  Ipipe pipe = {0};
  char fname[ PATH_MAX ] = {0};
  int  r = (name && name[0]) ? _strcmpi( name, "en" ) : 0;
  if ( r != 0 )
  {
    memset(   fname, 0, PATH_MAX );
    strcat_s( fname,    PATH_MAX, "lang"   );
    strcat_s( fname,    PATH_MAX, DIR_SEP  );
    strcat_s( fname,    PATH_MAX, name );
    strcat_s( fname,    PATH_MAX, ".applang" );
    pipe = ipMkFile( fname, 0666, SHARE_READ, ACTION_OPEN_NEW, NULL );
    ipRdPipe( &pipe, &_appLang, sizeof(LANG) );
    ipShutPipe( &pipe );
  }
  else
  {
    memset( name, 0, strlen( name ) );
    name[0] = 'e';
    name[1] = 'n';
    _appLang = _appDefLang;
  }
}
