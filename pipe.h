#pragma once
#ifndef _Ipipe_H
#define _Ipipe_H
#include "lang.h"

#ifndef DIR_SEP
#ifdef _WIN32
#define DIR_SEP "\\"
#else
#define DIR_SEP "/"
#endif
#endif

typedef void* Ipipe;
#define SHARE_DELETE 0x4
#define SHARE_WRITE  0x2
#define SHARE_READ   0x1

#define ACTION_NEW       0x1
#define ACTION_OVERWRITE 0x2
#define ACTION_EXISTING  0x3
#define ACTION_OPEN_NEW  0x4
#define ACTION_TRUNCATE  0x5

Ipipe IupMkFile(   char const *path,
                   ulv   unixPerm,
                   ucv   sharePerm,
                   ucv   action,
                   Ipipe templateFile );
Ipipe IupMkDir(    char const *path,
                   ulv  unixPerm,
                   ucv  sharePerm,
                   ucv  action );
Ipipe IupOpenFile( char const *path,
                   ulv  unixPerm,
                   ucv  sharePerm );
Ipipe IupOpenProcess( ulv perm, ucv setAsChild, ulv pid );
void IupGetLDataDir( char *path );
void IupGetLDataDirFromName( char *path, DWORD size );
ulv   IupWrPipe(   Ipipe pipe,
                   void *buff,
                   ulv   size );
ulv   IupRdPipe(   Ipipe pipe,
                   void *buff,
                   ulv   size );
ucv   IupShutPipe( Ipipe pipe );

#endif // _Ipipe_H
