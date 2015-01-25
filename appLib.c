#include "_guiMain.h"
extern void guiPfm_OnDefPath ( char *path );
HACK_COM *hCOM = NULL;
HLIB lib = NULL;
void appLoadLib( char* name )
{
  char path[ PATH_MAX ] = {0};
  //char ext[5] = "." LIB_EXT;
  _GETHACKCOM gethackcom = NULL;
  if( lib )
    libShut( lib );
  lib  = NULL;
  hCOM = NULL;
  if ( !name )
  {
    return;
  }
  strcpy_s( path, PATH_MAX, ipGetUsrDir() );
  strcat_s( path, PATH_MAX, DIR_SEP ".medit" DIR_SEP "data" );
  guiPfm_OnDefPath( path );
  strcat_s( path, PATH_MAX, DIR_SEP );
  strcat_s( path, PATH_MAX, name );
  /*
  if ( strcmpi( path, ext ) < 0 )
    strcat_s( path, PATH_MAX, "." LIB_EXT );
  */
  lib = libOpen( path );
  if ( !lib )
  {
    IupMessage( "Failure to load Library", name );
    return;
  }
  gethackcom = (_GETHACKCOM)libAddr( lib, "GetHackCOM");
  if ( !gethackcom )
  {
    IupMessage( "Failure to load hack COM", name );
    libShut( lib );
    return;
  }
  hCOM = gethackcom();
  hCOM->GetBase     = GetBase;
  hCOM->ResizeHacks = hacksReSize;
  hCOM->ResizeCodes = codesReSize;
}
void appFreeLib( void )
{
  if( lib )
    libShut( lib );
  lib = NULL;
}
