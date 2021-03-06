#include "_guiMain.h"
extern void guiPfm_OnDefPath ( char *path );
HACK_COM *hCOM = NULL;
HLIB lib = NULL;
void appLoadLib( char* name )
{
  char path[ PATH_MAX ] = {0};
  _GETHACKCOM gethackcom = NULL;
  if( lib )
  {
    libShut( lib );
  }

  if ( !name || !name[0] )
  {
quit:
    lib  = NULL;
    hCOM = NULL;
    memset( appSession.lib, 0, NAME_MAX );
    return;
  }

  copystri( appSession.lib, name, NAME_MAX );
  copystri( path, ipGetUsrDir(), PATH_MAX );
  appendstr( path, DIR_SEP ".medit" DIR_SEP "data", PATH_MAX );
  guiPfm_OnDefPath( path );
  appendstr( path, DIR_SEP, PATH_MAX );
  appendstr( path, name, PATH_MAX );

  if ( _access( path, 0 ) != 0 )
  {
    goto quit;
  }

  lib = libOpen( path );

  if ( !lib )
  {
    IupMessage( "Failure to load Library", name );
    goto quit;
  }

  gethackcom = ( _GETHACKCOM )libAddr( lib, "GetHackCOM" );

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
