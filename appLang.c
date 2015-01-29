#include "appLng.h"

LANG const _appDefLang =
{{
  // Main list
  {"Organisation"},
  {"Platform"},
  {"Target"},
  {"Profile"},
  {"Search"},
  {"Results"},
  {"Editor"},
  {"Hacks"},
  {"Hack"},
  {"About"},
  // Search Menu
  {"Dump"},
  {"Undo"},
  {"Redo"},
  // Other
  {"Name"},
  {"File"},
  {"Endian"},
  {"System"},
  {"Little"},
  {"PDP"},
  {"Big"},
  {"Path"},
  {"Type"},
  {"ID"},
  {"Memory"},
  {"Address"},
  {"Size"},
  {"Depth"},
  {"Range"},
  {"Codes"},
  {"Code"},
  {"Write"},
  {"Copy"},
  {"Add"},
  {"Remove"},
  {"Comparison"},
  {"Joker"},
  {"Format"},
  {"(M)"},
  {"???"},
  {"Value"},
  {"Always"},
  {"True"},
  {"False"},
  // List
  {"First"},
  {"Previous"},
  {"Next"},
  {"Final"},
  {"New"},
  {"Insert"},
  {"Shift Up"},
  {"Shift Up (NB)"},
  {"Shift Down"},
  {"Shift Down (NB)"},
  {"Shift Left"},
  {"Shift Right"},
  {"Delete"},
  {"Move"},
  // Data <> GUI Actions
  {"Apply"},
  {"Reset"},
  {"Load"},
  {"Save"}
}};
LANG _appLang = {{{{0}}}};
LANG const *appLang = &_appLang;

void appLoadLang ( char *name )
{
  int fd;
  char fname[ PATH_MAX ] = {0};
  int  r = ( name && name[0] ) ? _strcmpi ( name, "en" ) : 0;
  if ( r != 0 )
  {
    memset ( fname, 0, PATH_MAX );
    strncat ( fname, "lang", PATH_MAX );
    strncat ( fname, DIR_SEP, PATH_MAX );
    strncat ( fname, name, PATH_MAX );
    strncat ( fname, ".applang", PATH_MAX );
    ipFdOpen ( fd, fname, IP_O_FILE, IP_D_W );
    ipFdRdBuff ( fd, &_appLang, sizeof ( LANG ) );
    ipFdShut ( fd );
  }
  else
  {
    memset ( name, 0, strlen ( name ) );
    name[0] = 'e';
    name[1] = 'n';
    memcpy ( &_appLang, &_appDefLang, sizeof ( LANG ) );
  }
}
