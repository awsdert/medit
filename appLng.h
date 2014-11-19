#pragma once
#ifndef _MELANG_H
#define _MELANG_H

#include <ipipe/main.h>

typedef enum _LNG_T
{
  LNG_ORGANISATION = 0,
  LNG_PLATFORM,
  LNG_TARGET,
  LNG_PROFILE,
  LNG_SEARCH,
  LNG_RESULTS,
  LNG_EDITOR,
  LNG_HACKS,
  LNG_ABOUT,
  LNG_DUMP,
  LNG_UNDO,
  LNG_REDO,
  LNG_NAME,
  LNG_FILE,
  LNG_ENDIAN,
  LNG_SYSTEM,
  LNG_LITTLE,
  LNG_BIG,
  LNG_PATH,
  LNG_TYPE,
  LNG_ID,
  LNG_MEMORY,
  LNG_RANGE,
  LNG_CODE,
  LNG_WRITE,
  LNG_COPY,
  LNG_ADD,
  LNG_REMOVE,
  LNG_COMPARISON,
  LNG_JOKER,
  LNG_VALUE,
  LNG_ALWAYS,
  LNG_TRUE,
  LNG_FALSE,
  LNG_FIRST,
  LNG_PREVIOUS,
  LNG_NEXT,
  LNG_FINAL,
  LNG_NEW,
  LNG_DELETE,
  LNG_MOVE,
  LNG_APPLY,
  LNG_RESET,
  LNG_LOAD,
  LNG_SAVE,
  LNG_T_COUNT
} LNG_T;
typedef struct _LANG
{
  char x[LNG_T_COUNT][NAME_MAX];
} LANG;
/**
  Default Language is English (GB), used only for making translations
  by another app that will be developed
**/
LANG const* appGetDefaultLang();
LANG const* appGetLang();
char const* appGetText( LNG_T text );

/**
  \brief loads "~/APPDATA/APPNAME/lang/*.applang"
  \param name assumed to be at least 2 characters long for "en" (English GB)
**/
void appLoadLang( char *name );

#endif // _MELANG_H
