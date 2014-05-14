#ifndef _MELANG_H
#define _MELANG_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <iup.h>

#ifndef HUGE
#ifdef _MSC_VER
#define HUGE __int64
#else
#define HUGE long long
#endif
typedef HUGE huge;
#endif

typedef unsigned  HUGE uhv;
typedef   signed  HUGE shv;
typedef unsigned  long ulv;
typedef   signed  long slv;
typedef unsigned   int uiv;
typedef   signed   int siv;
typedef unsigned short usv;
typedef   signed short ssv;
typedef unsigned  char ucv;
typedef   signed  char scv;

typedef        SSIZE_T szv;
typedef         SIZE_T uzv;
typedef        INT_PTR spv;
typedef       UINT_PTR upv;

typedef          float fpv;
typedef         double dpv;
typedef    long double lpv;


typedef struct _MELANG_MAINLIST
{
  char tar[10];
  char pro[10];
  char sea[10];
  char res[10];
  char edi[10];
  char hac[10];
  char abo[10];
} MELANG_MAINLIST;

typedef struct _MELANG_SEAMENU
{
  char dump[10];
  char undo[10];
  char redo[10];
} MELANG_SEAMENU;

typedef struct _MELANG
{
  MELANG_MAINLIST mainList;
  MELANG_SEAMENU  seaMenu;
} MELANG;

MELANG* meGetLang( void );
char const* IupGetCwd( void );
char const* IupGetAppName( void );

#endif // _MELANG_H
