#include "_guiMain.h"
uchar getBaseNo( char *name )
{
  CFG *cfg = appGetCfg();
  uchar i = 0;
  do
  {
    if ( _strcmpi( name, cfg->dst.tar.bname[i] ) == 0 )
      break;
    ++i;
  }
  while ( i < BASES_COUNT );
  return i;
}
HMODULE meLoadLib( char* name, HACK_FUNC **hfunc, CODE_FUNC **cfunc )
{
  HMODULE lib = NULL;
  char szFile[ PATH_MAX ] = {0};
  GETHACKFUNCS gethacks = NULL;
  GETCODEFUNCS getcodes = NULL;
  strcpy_s( szFile, PATH_MAX, IupGetAttribute( NULL, "APP_CWD" ) );
  strcat_s( szFile, PATH_MAX, name );
  strcat_s( szFile, PATH_MAX, "-" );
  strcat_s( szFile, PATH_MAX, IupGetAttribute( NULL, "APP_VER" ) );
  strcat_s( szFile, PATH_MAX, ".dll" );
  lib = LoadLibrary( szFile );
  if ( !lib )
  {
    IupMessage( "Failure to load Library", name );
    return NULL;
  }
  gethacks = (GETHACKFUNCS)GetProcAddress( lib, "GetHackFuncs");
  getcodes = (GETCODEFUNCS)GetProcAddress( lib, "GetCodeFuncs");
  if ( !gethacks )
  {
    IupMessage( "Failure to load hack functions", name );
    FreeLibrary( lib );
    return NULL;
  }
  if ( !getcodes )
  {
    IupMessage( "Failure to load code functions", name );
    FreeLibrary( lib );
    return NULL;
  }
  (*hfunc) = gethacks();
  (*cfunc) = getcodes();
  (*cfunc)->getBaseNo = getBaseNo;
  return lib;
}
HMODULE meFreeLib( HMODULE lib )
{
  FreeLibrary( lib );
  return NULL;
}
