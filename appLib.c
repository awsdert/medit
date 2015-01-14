#include "_guiMain.h"
extern void guiTar_OnDefPath ( char *path, uchar saveFile );
HLIB appLoadLib( char* name, HACK_LIB_COM **hfunc, CODE_LIB_COM **cfunc )
{
  HLIB lib = NULL;
  char path[ PATH_MAX ] = {0};
  HACKLIBCOM gethacks = NULL;
  CODELIBCOM getcodes = NULL;
  if ( !name )
  {
    (*hfunc) = NULL;
    (*cfunc) = NULL;
    return NULL;
  }
  strcpy_s( path, PATH_MAX, ipGetCwd() );
  strcat_s( path, PATH_MAX, DIR_SEP ".medit" DIR_SEP );
  guiTar_OnDefPath( path, 1 );
  strcat_s( path, PATH_MAX, name );
  strcat_s( path, PATH_MAX, "." LIB_EXT );
  lib = libOpen( path );
  if ( !lib )
  {
    IupMessage( "Failure to load Library", name );
    return NULL;
  }
  gethacks = (HACKLIBCOM)libAddr( lib, "GetHackFuncs");
  getcodes = (CODELIBCOM)libAddr( lib, "GetCodeFuncs");
  if ( !gethacks )
  {
    IupMessage( "Failure to load hack functions", name );
    libShut( lib );
    return NULL;
  }
  if ( !getcodes )
  {
    IupMessage( "Failure to load code functions", name );
    libShut( lib );
    return NULL;
  }
  (*hfunc) = gethacks();
  (*cfunc) = getcodes();
  (*cfunc)->GetBase = GetBase;
  return lib;
}
HLIB appFreeLib( HLIB lib )
{
  libShut( lib );
  return NULL;
}
