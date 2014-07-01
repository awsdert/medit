#pragma once
#ifndef _MELANG_H
#define _MELANG_H

#include "types.h"
#include <iup.h>

typedef enum _LANG_T
{
  LANG_TARGET = 0,
  LANG_PROFILE,
  LANG_SEARCH,
  LANG_RESULTS,
  LANG_EDITOR,
  LANG_HACKS,
  LANG_ABOUT,
  LANG_DUMP,
  LANG_UNDO,
  LANG_REDO,
  LANG_NAME,
  LANG_TYPE,
  LANG_ID,
  LANG_MEMORY,
  LANG_RANGE,
  LANG_CODE,
  LANG_COMPARISON,
  LANG_VALUE,
  LANG_ALWAYS,
  LANG_TRUE,
  LANG_FALSE,
  LANG_COUNT
} LANG_T;

typedef struct _LANG
{
  char x[ LANG_COUNT ][12];
} LANG;
LANG*       meGetLang();
char const* meGetText( LANG_T text );
char const* IupGetCwd( void );
char const* IupGetAppName( void );

#endif // _MELANG_H
