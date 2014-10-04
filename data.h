#pragma once
#ifndef _DATA_H
#define _DATA_H

#include "_hack.h"


typedef struct _MEFILE
{
  char name[NAME_LEN];
  char file[NAME_LEN];
} MEFILE;

typedef struct _MEMORY
{
  char name[NAME_LEN];
  BLOCK tmem;
} MEMORY;

typedef struct _TARGET
{
  MEFILE file;
  char   name[NAME_LEN];
  END_T  endian;
  MEMORY list[16];
} TARGET;

typedef MEFILE PROFILE;

typedef struct _MECONFIG
{
  char  *name;
  char  *file;
  TARGET  tar;
  MEMORY *mem;
  PROFILE pro;
  HACK  *hack;
  CODE  *code;
} MECONFIG;

typedef struct _MECFG
{
  char lang[7];
  HACKINFO info;
  char    _hname[HACKS_COUNT ][NAME_LEN];
  char    _bname[BLOCKS_COUNT][NAME_LEN];
  HACK    _hack;
  MEMORY   _mem;
  MECONFIG  src;
  MECONFIG  dst;
} MECFG;

#endif // _DATA_H
