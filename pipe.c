#include "pipe.h"
#include <shlobj.h>
void IupGetLDataDir( char *path )
{
  SHGetFolderPathA( NULL, CSIDL_APPDATA, NULL, 0, path  );
}
Ipipe IupMkFile( char const *path, ulv unixPerm, ucv sharePerm, ucv action, Ipipe *templateFile )
{
  uiv perm = 0, flags = 0;
  char rpath[ FILENAME_MAX ] = {0};
  Ipipe pipe = {0};
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
  pipe.pipe = CreateFileA( rpath, perm, sharePerm, NULL, action, flags, templateFile->pipe );
  return pipe;
}
#define _PP_TXT( VARIABLE ) #VARIABLE
#define PP_TXT( VARIABLE ) _PP_TXT( VARIABLE )
char _IpipeAppExe[] = PP_TXT(WORKSPACE_NAME) "-" PP_TXT( TARGET_NAME );

void IupGetLDataDirFromName( char *path, DWORD size )
{
  IupGetLDataDir( path );
  strcat_s( path, size, DIR_SEP );
  strcat_s( path, size, _IpipeAppExe );
  strcat_s( path, size, DIR_SEP );
}

Ipipe IupMkDir( char const *path, ulv unixPerm, ucv sharePerm, ucv action )
{
  uiv perm = 0, flags = 0;
  char rpath[ FILENAME_MAX ] = {0};
  Ipipe pipe = {0};
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
  pipe.pipe = CreateFileA( rpath, perm, sharePerm, NULL, action, flags, NULL );
  return pipe;
}

Ipipe IupOpenFile( char const *path, ulv unixPerm, ucv sharePerm )
{
  return IupMkFile( path, unixPerm, sharePerm, ACTION_EXISTING, NULL );
}

Ipipe IupOpenProcess( DWORD perm, ucv setAsChild, DWORD pid )
{
  Ipipe pipe = { OpenProcess( perm, setAsChild, pid ), 0 };
  return pipe;
}

ucv IupEof( Ipipe *pipe )
{
  return pipe->eof;
}
ucv IupSkPipe( Ipipe *pipe, int offset, int pos )
{
  LARGE_INTEGER li = {0};
  li.QuadPart = offset;
  if ( SetFilePointerEx( pipe->pipe, li, NULL, pos ) )
    return 1;
  return 0;
}
#ifdef HUGE
ucv IupSkHPipe( Ipipe *pipe, huge offset, int pos )
{
  int add = (offset < 0) ? -INT_MAX : INT_MAX;
  ucv stop = 0;
  LARGE_INTEGER li = {0};
  SetFilePointerEx( pipe, li, NULL, pos );
  li.QuadPart = add;
  do
  {
    stop = (add < 0) ? (offset >= add) : (offset <= add);
    if ( stop )
    {
      li.QuadPart = offset & add;
      if ( SetFilePointerEx( pipe, li, NULL, FPOS_CUR ) )
        return 1;
      return 0;
    }
    if ( !SetFilePointerEx( pipe, li, NULL, FPOS_CUR ) )
      return 0;
    if ( add < 0 )
      offset += INT_MAX;
    else
      offset -= INT_MAX;
  }
  while ( offset );
  return 0;
}
#endif

DWORD IupWrPipe( Ipipe *pipe, void *buff, DWORD size )
{
  DWORD done = 0;
  if ( WriteFile( pipe, buff, size, &done, NULL ) )
    return done;
  return 0;
}
DWORD IupWrLine( Ipipe *pipe, char *buff, DWORD size, char const *nl )
{
  DWORD nldone = 0, done = 0;
  if ( !size )
    size = strlen(buff);
  if ( !buff[size - 1] )
    --size;
  if ( WriteFile( pipe, buff, size, &done, NULL ) )
  {
    if ( WriteFile( pipe, nl, strlen( nl ), &nldone, NULL ) )
      return done + nldone;
    return done;
  }
  return 0;
}
DWORD IupRdPipe( Ipipe *pipe, void *buff, DWORD size )
{
  DWORD done = 0, i = 0;
  char* data = (char*)buff;
  ReadFile( pipe->pipe, buff, size, &done, NULL );
  pipe->eof = ( GetLastError() == ERROR_HANDLE_EOF ) ? 1 : 0;
  for ( i = done; i < size; ++i ) data[i] = 0;
  return done;
}
DWORD IupRdLine( Ipipe *pipe, char* buff, DWORD size )
{
  DWORD done = 0, i = 0;
  ReadFile( pipe->pipe, buff, size, &done, NULL );
  pipe->eof = ( GetLastError() == ERROR_HANDLE_EOF ) ? 1 : 0;
  for ( ; i < done; ++i )
  {
    if ( buff[i] == '\r' )
    {
      ++i;
      if ( buff[i] == '\n' )
        ++i;
      break;
    }
    else if ( buff[i] == '\n' )
    {
      ++i;
      break;
    }
  }
  if ( i < done )
    /* Ensure our pointer is back at line break */
    IupSkPipe( pipe, -((int)(done - i)), SEEK_CUR );
  /* Shorten length returned */
  done = i;
  /* Force EOL with NULL */
  for ( ; i < size; ++i ) buff[i] = 0;
    return done;
}
ucv    IupShutPipe( Ipipe *pipe )
{
  return CloseHandle( pipe ) & 0x1;
}
