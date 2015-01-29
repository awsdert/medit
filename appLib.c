#include "_guiMain.h"
extern void guiPfm_OnDefPath ( char *path );
HACK_COM *hCOM = NULL;
HLIB lib = NULL;
void appLoadLib( char* name )
{
  if( lib )
    libShut( lib );
  if ( !name || !name[0] )
  {
quit:
    lib  = NULL;
    hCOM = NULL;
    memset( appSession.lib, 0, NAME_MAX );
    return;
  }
  memcpy_s( appSession.lib, NAME_MAX, name, strlen( name ) );
  char path[ PATH_MAX ] = {0};
  strcpy_s( path, PATH_MAX, ipGetUsrDir() );
  strcat_s( path, PATH_MAX, DIR_SEP ".medit" DIR_SEP "data" );
  guiPfm_OnDefPath( path );
  strcat_s( path, PATH_MAX, DIR_SEP );
  strcat_s( path, PATH_MAX, name );
  if ( _access( path, 0 ) != 0 )
    goto quit;
  lib = libOpen( path );
  if ( !lib )
  {
    IupMessage( "Failure to load Library", name );
    goto quit;
  }
  _GETHACKCOM gethackcom = (_GETHACKCOM)libAddr( lib, "GetHackCOM");
  if ( !gethackcom )
  {
    IupMessage( "Failure to load hack COM", name );
    libShut( lib );
    goto quit;
  }
  hCOM = gethackcom();
  hCOM->GetBase     = GetBase;
  hCOM->ResizeHacks = hacksReSize;
  hCOM->ResizeCodes = codesReSize;
}
