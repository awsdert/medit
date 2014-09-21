#pragma once
#include "hack.h"
#include <TlHelp32.h>
#include <process.h>
#include <Shlwapi.h>
#include <winternl.h>

OPEN_C

typedef struct _SYSF
{
  unsigned char bText;
  HFILE* hFile;
} SYSF;

/*
  Holds common variables for hack functions
*/
typedef struct _HACKDATA
{
  SYSF *hacks;
  SYSF *codes;
  char  folder[MAX_PATH];
  char  file[MAX_PATH];
} HACKDATA;

/* Needed for address retrieval */
S4      hackProc( HACKDATA *data, HACKINFO *info, CODES *codesBuff, S4 hi, U2 ci );
BOOL    EnableDebugPrivileges(void);
void    entryPoint( pid_t pid );
DWORD WINAPI entryThread(LPVOID param);
pid_t   GetProcessIdByNameA( LPCSTR name );
pid_t   GetProcessParentId( pid_t pid );
HMODULE injectModule(HANDLE proc, LPVOID module);
DWORD   main();
BOOL    peInjection(pid_t pid, LPTHREAD_START_ROUTINE callRoutine);
BOOL    Itoa_s( int value, char* dst, size_t dstSize, unsigned char base );


int  Div( int value, int by, int *remainder );
void* __cdecl memset( void *dst, int val, size_t size );
void* __cdecl memcpy( void *dst, void const *src, size_t size );
#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

#define sitoa_s( value, buff, size, radix ) RtlIntegerToChar( value, radix, size, buff )
#define s_getpid  GetCurrentProcessId
#define sstrcat_s( dst, dstSize, src ) StrCatBuffA( dst, src, dstSize ); dst[ dstSize - 1 ] = 0
#define sstrcpy_s( dst, dstSize, src ) StrCpyNA( dst, src, dstSize )
#define GetCurrentProcessParentId() GetProcessParentId( GetCurrentProcessId() )

#define SYSF_SET 0
#define SYSF_CUR 1
#define SYSF_END 2

#define snew( size )      VirtualAlloc( NULL, size, MEM_COMMIT, PAGE_READWRITE )
#define sadj( ptr, size ) VirtualAlloc( ptr,  size, MEM_COMMIT, PAGE_READWRITE )
#define sdel( ptr )       VirtualFree(  ptr,  0,    MEM_RELEASE )

errno_t sfopen_s( SYSF** file, char const *path, char const *mode );
#define sfclose( file ) CloseHandle( file->hFile )
#define sfseek( file, bytes, from ) \
                        SetFilePointer( file->hFile, bytes, NULL, from )
size_t  sfread(  void* buffer, size_t elSize, int elCount, SYSF* file );
size_t  sfwrite( void* buffer, size_t elSize, int elCount, SYSF* file );
#define sfflush( file ) FlushFileBuffers( file->hFile )

SHUT_C
