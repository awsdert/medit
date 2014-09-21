#pragma once
/*
  We will NOT live in the past, C99/C++ compatible code should be used,
  any compiler supporting this will support pragma once from above as well
*/
#ifdef __cplusplus
#define EXT_C extern "C"
#define OPEN_C EXT_C {
#define SHUT_C }
#else
#define EXT_C
#define OPEN_C
#define SHUT_C
#endif // __cplusplus

#ifdef _WIN32_C_LIB
#define _WIN_C_LIB 32
#define _WIN 32
#elif defined( _WIN64_C_LIB )
#define _WIN_C_LIB 64
#define _WIN 64
#elif defined( __WIN64 ) || defined( __WIN64__ ) || defined( _WIN64 )
#define _WIN 64
#elif defined( __WIN32 ) || defined( __WIN32__ ) || defined( _WIN32 ) || \
  defined( __WINDOWS ) || defined( __WINDOWS__ ) || defined( _WINDOWS )
#define _WIN 32
#elif defined( _X86_ )
#define _UNIX 32
#else
#define _UNIX 64
#endif

#ifdef _WIN
#include <windef.h>
#include <Windows.h>
#include <windowsx.h>
#else
#include <linux/limits.h>
#include <pstdint.h>
#endif

OPEN_C

#ifdef _WIN
#define PATH_MAX MAX_PATH
#define CHAR_BITS 8
#define stdint_least_defined
#define stdint_fast_defined

typedef DWORD pid_t;
#define PID_MAX MAXDWORD
#define PID_MIN 0

typedef UINT8 uint8_t;
typedef  INT8  int8_t;
#define UINT8_MAX MAXBYTE
#define  INT8_MAX MAXCHAR
#define  INT8_MIN MINCHAR
typedef UINT8 uint_least8_t;
typedef  INT8  int_least8_t;
#define UINT_LEAST8_MAX UINT8_MAX
#define  INT_LEAST8_MAX  INT8_MAX
#define  INT_LEAST8_MIN  INT8_MIN
typedef UINT8 uint_fast8_t;
typedef  INT8  int_fast8_t;
#define UINT_FAST8_MAX UINT8_MAX
#define  INT_FAST8_MAX  INT8_MAX
#define  INT_FAST8_MIN  INT8_MIN

typedef UINT16 uint16_t;
typedef  INT16  int16_t;
#define UINT16_MAX USHRT_MAX
#define  INT16_MAX SHRT_MAX
#define  INT16_MIN SHRT_MIN
typedef UINT16 uint_least16_t;
typedef  INT16  int_least16_t;
#define UINT_LEAST16_MAX UINT16_MAX
#define  INT_LEAST16_MAX  INT16_MAX
#define  INT_LEAST16_MIN  INT16_MIN
typedef UINT16 uint_fast16_t;
typedef  INT16  int_fast16_t;
#define UINT_FAST16_MAX UINT16_MAX
#define  INT_FAST16_MAX  INT16_MAX
#define  INT_FAST16_MIN  INT16_MIN

typedef UINT32 uint32_t;
typedef  INT32  int32_t;
#define UINT32_MAX UINT_MAX
#define  INT32_MAX  INT_MAX
#define  INT32_MIN  INT_MIN
typedef UINT32 uint_least32_t;
typedef  INT32  int_least32_t;
#define UINT_LEAST32_MAX UINT32_MAX
#define  INT_LEAST32_MAX  INT32_MAX
#define  INT_LEAST32_MIN  INT32_MIN
typedef UINT32 uint_fast32_t;
typedef  INT32  int_fast32_t;
#define UINT_FAST32_MAX UINT32_MAX
#define  INT_FAST32_MAX  INT32_MAX
#define  INT_FAST32_MIN  INT32_MIN

typedef UINT64 uint64_t;
typedef  INT64  int64_t;
#define UINT64_MAX ULLONG_MAX
#define  INT64_MAX  LLONG_MAX
#define  INT64_MIN  LLONG_MIN
typedef UINT64 uint_least64_t;
typedef  INT64  int_least64_t;
#define UINT_LEAST64_MAX UINT64_MAX
#define  INT_LEAST64_MAX  INT64_MAX
#define  INT_LEAST64_MIN  INT64_MIN
typedef UINT64 uint_fast64_t;
typedef  INT64  int_fast64_t;
#define UINT_FAST64_MAX UINT64_MAX
#define  INT_FAST64_MAX  INT64_MAX
#define  INT_FAST64_MIN  INT64_MIN

typedef uint64_t uintmax_t;
#define UINTMAX_MAX UINT64_MAX
typedef  int64_t  intmax_t;
#define  INTMAX_MAX  INT64_MAX
#define  INTMAX_MIN  INT64_MIN
/*
#ifndef _UINTPTR_T_DEFINED
typedef UINT_PTR uintptr_t;
#define _UINTPTR_T_DEFINED
#endif
#ifndef _INTPTR_T_DEFINED
typedef  INT_PTR  intptr_t;
#define _INTPTR_T_DEFINED
#endif
#ifndef UINTPTR_MAX
//*/
#define UINTPTR_MAX MAXUINT_PTR
/*
#endif
#ifndef INTPTR_MAX
//*/
#define  INTPTR_MAX MAXINT_PTR
#define  INTPTR_MIN MININT_PTR
/*
#endif
//*/

#ifndef SIZE_MAX
typedef SIZE_T   size_t;
#define SIZE_MAX   MAXSIZE_T
#endif

#ifndef SSIZE_MAX
typedef SSIZE_T  ssize_t;
#define SSIZE_MAX  MAXSSIZE_T
#define SSIZE_MIN  MINSSIZE_T
#endif

#else
#define CHAR_BITS CHAR_BIT
#define DWORD    unsigned long
#define HANDLE   void*
#define HMODULE  void*
#define HFILE    void*
#define MAX_PATH 8192
#endif

SHUT_C
