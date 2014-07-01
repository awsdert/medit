#pragma once
#ifndef _DATA_H
#define _DATA_H

#include "types.h"

#define NAME_LEN  12
#define NAME_LAST 11

typedef struct _MEFILE
{
  char name[NAME_LEN];
  char file[NAME_LEN];
} MEFILE;

typedef enum _END_T
{
  END_LIT = 0,
  END_LBG,
  END_BIG,
  END_COUNT
} END_T;

typedef struct _MEMORY
{
  char name[NAME_LEN];
  upv  addr;
  upv  size;
} MEMORY;

typedef struct _TARGET
{
  MEFILE file;
  char   name[NAME_LEN];
  END_T  endian;
  MEMORY list[16];
} TARGET;

typedef MEFILE PROFILE;

typedef enum _MOD_T
{
  MOD_WRITE = 0,
  MOD_INC,
  MOD_DEC,
  MOD_CMP,
  MOD_LIST,
  MOD_COUNT
} MOD_T;

typedef struct _MODIFIER
{
  MOD_T type;
  usv   info;
  ucv   size;
  ucv   loop;
  ucv  *buff;
} MODIFIER;

typedef struct _HACK
{
  char     name[ NAME_LEN ];
  int      init;
  int      next;
  MODIFIER list[ 20 ];
} HACK;

typedef struct _HACKS
{
  HACK list[ 1000 ];
} HACKS;

typedef struct _MECFG
{
  char      lang[7];
  TARGET    tar[2];
  MEMORY   *mem[2];
  PROFILE   pro[2];
  HACKS     hacks;
  HACK      _hack;
  HACK     *hack[2];
  MODIFIER *code[2];
} MECFG;

#endif // _DATA_H
