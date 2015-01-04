#include "_guiMain.h"
HLIB appLoadLib( char* name, HACK_LIB_COM **hfunc, CODE_LIB_COM **cfunc )
{
  HMODULE lib = NULL;
  char szFile[ PATH_MAX ] = {0};
  HACKLIBCOM gethacks = NULL;
  CODELIBCOM getcodes = NULL;
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
  gethacks = (HACKLIBCOM)GetProcAddress( lib, "GetHackFuncs");
  getcodes = (CODELIBCOM)GetProcAddress( lib, "GetCodeFuncs");
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
  (*cfunc)->GetBase = GetBase;
  return lib;
}
HLIB appFreeLib( HLIB lib )
{
  FreeLibrary( lib );
  return NULL;
}
