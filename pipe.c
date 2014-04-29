#include "pipe.h"

Ipipe IupMkFile( char const *path, uln unixPerm, ucn sharePerm, ucn action, Ipipe templateFile )
{
  uin perm = 0, flags = 0;
  if ( unixPerm & 00444 )
    perm |= READ_CONTROL |
            FILE_READ_DATA  | FILE_READ_EA  | FILE_READ_ATTRIBUTES;
  if ( unixPerm & 00222 )
    perm |= FILE_APPEND_DATA |
            FILE_WRITE_DATA | FILE_WRITE_EA | FILE_WRITE_ATTRIBUTES;
  if ( unixPerm & 00111 )
    perm |= FILE_EXECUTE | FILE_CREATE_PIPE_INSTANCE;
  return CreateFile( path, perm, sharePerm, NULL, action, flags,templateFile );
}

Ipipe IupMkDir( char const *path, uln unixPerm, ucn sharePerm, ucn action )
{
  uin perm = 0, flags = 0;
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
  return CreateFile( path, perm, sharePerm, NULL, action, flags, NULL );
}

Ipipe IupOpenFile( char const *path, uln unixPerm, ucn sharePerm )
{
  return IupMkFile( path, unixPerm, sharePerm, ACTION_EXISTING, NULL );
}

Ipipe IupOpenProcess( DWORD perm, ucn setAsChild, DWORD pid )
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
ucn    IupShutPipe( Ipipe pipe )
{
  return CloseHandle( pipe ) & 0x1;
}
