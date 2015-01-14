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
  LNG_HACK,
  LNG_ABOUT,
  LNG_DUMP,
  LNG_UNDO,
  LNG_REDO,
  LNG_NAME,
  LNG_FILE,
  LNG_ENDIAN,
  LNG_SYSTEM,
  LNG_LITTLE,
  LNG_PDP,
  LNG_BIG,
  LNG_PATH,
  LNG_TYPE,
  LNG_ID,
  LNG_MEMORY,
  LNG_ADDRESS,
  LNG_SIZE,
  LNG_DEPTH,
  LNG_RANGE,
  LNG_CODES,
  LNG_CODE,
  LNG_WRITE,
  LNG_COPY,
  LNG_ADD,
  LNG_REMOVE,
  LNG_COMPARISON,
  LNG_JOKER,
  LNG_FORMAT,
  LNG__M_, // (M)
  LNG_QQQ, // ???
  LNG_VALUE,
  LNG_ALWAYS,
  LNG_TRUE,
  LNG_FALSE,
  LNG_FIRST,
  LNG_PREVIOUS,
  LNG_NEXT,
  LNG_FINAL,
  LNG_NEW,
  LNG_INSERT,
  LNG_SHIFT_UP,
  LNG_SHIFT_UP__NB_,
  LNG_SHIFT_DOWN,
  LNG_SHIFT_DOWN__NB_,
  LNG_SHIFT_LEFT,
  LNG_SHIFT_RIGHT,
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
  NAME a[LNG_T_COUNT];
} LANG;
extern LANG const *appLang;

/**
  \brief loads "~/APPDATA/APPNAME/lang/*.applang"
  \param name assumed to be at least 2 characters long for "en" (English GB)
**/
void appLoadLang( char *name );

#endif // _MELANG_H
