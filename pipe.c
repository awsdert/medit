#include "pipe.h"
#include <shlobj.h>
void IupGetLDataDir( char *path )
{
  SHGetFolderPathA( NULL, CSIDL_APPDATA, NULL, 0, path  );
}
Ipipe IupMkFile( char const *path, ulv unixPerm, ucv sharePerm, ucv action, Ipipe templateFile )
{
  uiv perm = 0, flags = 0;
  char rpath[ FILENAME_MAX ] = {0};
#ifdef _WIN32
  if ( path[1] != ':' )
#else
  if ( path[0]) != '/' )
#endif
  {
    IupGetLDataDirFromName( rpath, FILENAME_MAX );
  }
  strcat_s( rpath, FILENAME_MAX, path );
  if ( unixPerm & 00444 )
    perm |= READ_CONTROL |
            FILE_READ_DATA  | FILE_READ_EA  | FILE_READ_ATTRIBUTES;
  if ( unixPerm & 00222 )
    perm |= FILE_APPEND_DATA |
            FILE_WRITE_DATA | FILE_WRITE_EA | FILE_WRITE_ATTRIBUTES;
  if ( unixPerm & 00111 )
    perm |= FILE_EXECUTE | FILE_CREATE_PIPE_INSTANCE;
  return CreateFileA( rpath, perm, sharePerm, NULL, action, flags,templateFile );
}

Ipipe IupMkDir( char const *path, ulv unixPerm, ucv sharePerm, ucv action )
{
  uiv perm = 0, flags = 0;
  char rpath[ FILENAME_MAX ] = {0};
#ifdef _WIN32
  if ( path[1] != ':' )
#else
  if ( path[0]) != '/' )
#endif
  {
    IupGetLDataDirFromName( rpath, FILENAME_MAX );
  }
  if ( unixPerm & 00444 )
    perm |= READ_CONTROL |
            FILE_READ_DATA  | FILE_READ_EA  | FILE_READ_ATTRIBUTES |
            FILE_LIST_DIRECTORY | FILE_TRAVERSE;
  if ( unixPerm & 00222 )
    perm |= FILE_APPEND_DATA |
            FILE_WRITE_DATA | FILE_WRITE_EA | FILE_WRITE_ATTRIBUTES |
            FILE_ADD_SUBDIRECTORY | FILE_ADD_FILE | FILE_DELETE_CHILD;
  if ( unixPerm & 00111 )
    perm |= FILE_EXECUTE | FILE_CREATE_PIPE_INSTANCE;
  strcat_s( rpath, FILENAME_MAX, path );
  CreateDirectory( rpath, NULL );
  return CreateFileA( rpath, perm, sharePerm, NULL, action, flags, NULL );
}

Ipipe IupOpenFile( char const *path, ulv unixPerm, ucv sharePerm )
{
  return IupMkFile( path, unixPerm, sharePerm, ACTION_EXISTING, NULL );
}

Ipipe IupOpenProcess( DWORD perm, ucv setAsChild, DWORD pid )
{
  return OpenProcess( perm, setAsChild, pid );
}

DWORD IupWrPipe( Ipipe pipe, void *buff, DWORD size )
{
  DWORD done = 0;
  if ( WriteFile( pipe, buff, size, &done, NULL ) )
    return done;
  return 0;
}
DWORD IupRdPipe( Ipipe pipe, void *buff, DWORD size )
{
  DWORD done = 0;
  if ( ReadFile( pipe, buff, size, &done, NULL ) )
    return done;
  return 0;
}
ucv    IupShutPipe( Ipipe pipe )
{
  return CloseHandle( pipe ) & 0x1;
}
