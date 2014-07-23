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

typedef struct _MECFG
{
  char      lang[7];
  TARGET    tar[2];
  MEMORY   *mem[2];
  PROFILE   pro[2];
  HACKS     hacks;
  HACK      _hack;
  HACK     *hack[2];
  CODE     *code[2];
} MECFG;

#endif // _DATA_H
