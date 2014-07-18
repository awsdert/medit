#pragma once
#ifndef _Ipipe_H
#define _Ipipe_H
#include "types.h"

#ifndef DIR_SEP
#if defined( _WIN32 ) || defined( _WIN64 ) || defined( _WINDOWS_ )
#define DIR_SEP "\\"
#else
#define DIR_SEP "/"
#endif
#endif

typedef struct _Ipipe
{
  void* pipe;
  ucv   eof;
} Ipipe;
#define SHARE_DELETE 0x4
#define SHARE_WRITE  0x2
#define SHARE_READ   0x1

#define FPOS_SOF FILE_BEGIN
#define FPOS_CUR FILE_CURRENT
#define FPOS_EOF FILE_END

#define ACTION_NEW       0x1
#define ACTION_OVERWRITE 0x2
#define ACTION_EXISTING  0x3
#define ACTION_OPEN_NEW  0x4
#define ACTION_TRUNCATE  0x5

Ipipe IupMkFile(   char const *path,
                   ulv         unixPerm,
                   ucv         sharePerm,
                   ucv         action,
                   Ipipe      *templateFile );
Ipipe IupMkDir(    char const *path,
                   ulv  unixPerm,
                   ucv  sharePerm,
                   ucv  action );
Ipipe IupOpenFile( char const *path,
                   ulv  unixPerm,
                   ucv  sharePerm );
ucv   IupEof( Ipipe *pipe );
Ipipe IupOpenProcess( ulv perm, ucv setAsChild, ulv pid );
void  IupGetLDataDir( char *path );
void  IupGetLDataDirFromName( char *path, DWORD size );
ucv   IupSkPipe(   Ipipe *pipe,
                   int   offset,
                   int   pos );
DWORD IupWrPipe(   Ipipe *pipe,
                   void *buff,
                   DWORD size );
DWORD IupWrLine(   Ipipe *pipe,
                   char *buff,
                   DWORD size,
                   char const *nl );
DWORD IupRdPipe(   Ipipe *pipe,
                   void *buff,
                   DWORD size );
DWORD IupRdLine(   Ipipe *pipe,
                   char *buff,
                   DWORD size );
ucv   IupShutPipe( Ipipe *pipe );

#endif // _Ipipe_H
