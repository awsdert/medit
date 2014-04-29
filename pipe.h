#pragma once
#ifndef _Ipipe_H
#define _Ipipe_H
#include "lang.h"

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
                   uln   unixPerm,
                   ucn   sharePerm,
                   ucn   action,
                   Ipipe templateFile );
Ipipe IupMkDir(    char const *path,
                   uln  unixPerm,
                   ucn  sharePerm,
                   ucn  action );
Ipipe IupOpenFile( char const *path,
                   uln  unixPerm,
                   ucn  sharePerm );
Ipipe IupOpenProcess( uln perm, ucn setAsChild, uln pid );
uln   IupWrPipe(   Ipipe pipe,
                   void *buff,
                   uln   size );
uln   IupRdPipe(   Ipipe pipe,
                   void *buff,
                   uln   size );
ucn   IupShutPipe( Ipipe pipe );

#endif // _Ipipe_H
