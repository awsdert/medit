#include "hackProc.h"
errno_t sfopen_s( SYSF** file, char const *path, char const *mode )
{
  int i = 0;
  DWORD perm = 0, open = CREATE_NEW, share = FILE_SHARE_READ | FILE_SHARE_WRITE,
        attr = FILE_ATTRIBUTE_NORMAL;
  *file = ( SYSF* )snew( sizeof( SYSF ) );

  while ( mode[i] )
  {
    switch ( mode[i] )
    {
    case 'r':
      perm |= FILE_READ_DATA;
      break;

    case 'w':
      perm |= FILE_WRITE_DATA;
      break;

    case 'a':
      perm |= FILE_APPEND_DATA;
      break;

    case '+':
      open = OPEN_EXISTING;
      break;

    case 't':
      ( *file )->bText = 1;
    }
  }

  ( *file )->hFile = CreateFileA( path, perm, share, NULL, open, attr, NULL );

  if ( !( ( *file )->hFile ) )
  {
    sdel( *file );
    *file = NULL;
    return ERROR_SEVERITY_ERROR;
  }

  return ERROR_SEVERITY_SUCCESS;
}
