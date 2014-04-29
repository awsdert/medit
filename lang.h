#ifndef _MELANG_H
#define _MELANG_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <iup.h>

#ifndef _HUGE_DEFINED
#define _HUGE_DEFINED
#define HUGE long long
typedef HUGE huge;
#endif

typedef unsigned  HUGE uhn;
typedef   signed  HUGE shn;
typedef unsigned  long uln;
typedef   signed  long sln;
typedef unsigned   int uin;
typedef   signed   int sin;
typedef unsigned short usn;
typedef   signed short ssn;
typedef unsigned  char ucn;
typedef   signed  char scn;
typedef          float fpn;
typedef         double dpn;
typedef    long double lpn;


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

#endif // _MELANG_H
