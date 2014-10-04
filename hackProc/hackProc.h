#pragma once
#include "../hack.h"
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

/// \brief Holds common variables for hack functions
typedef struct _HACKDATA
{
  SYSF *hacks;
  SYSF *codes;
  char  folder[MAX_PATH];
  char  file[MAX_PATH];
} HACKDATA;

/// \brief Handles actual hacking
S4      hackProc( HACKDATA *data, HACKINFO *info, CODES *codesBuff, S4 hi, U2 ci );
/// \brief If you need to ask then you should give up now
BOOL    EnableDebugPrivileges(void);
/// \brief Injects this process into target process
void    entryPoint( pid_t pid );
/// \brief Forgot
DWORD WINAPI entryThread(LPVOID param);
/// \brief Not sure this even needs to be here any more
pid_t   GetProcessIdByNameA( LPCSTR name );
/// \brief Not sure if needed any more
pid_t   GetProcessParentId( pid_t pid );
/// \brief Dunno
HMODULE injectModule(HANDLE proc, LPVOID module);
// Needed for address retrieval
DWORD   main();
/// \brief Actual injection routine
BOOL    peInjection(pid_t pid, LPTHREAD_START_ROUTINE callRoutine);
/// \brief Needed for acquiring needed files once injected
BOOL    Itoa_s( int value, char* dst, size_t dstSize, unsigned char base );
/// \brief Return both quotient and remainder from a division operation
int  Div( int value, int by, int *remainder );
/// \brief fill size bytes of dst with val
void* __cdecl memset( void *dst, int val, size_t size );
/// \brief fill dst with copy of src stopping at size bytes
void* __cdecl memcpy( void *dst, void const *src, size_t size );
#if _WIN == 32
/// \brief (unsigned long long)num shift left
void  __cdecl _allshl(void);
#endif
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
