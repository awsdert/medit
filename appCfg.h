#pragma once
#include "_hack.h"

OPEN_C

typedef struct _CFG_ORG
{
  char name[NAME_MAX];
  char file[NAME_MAX];
} CFG_ORG;

typedef struct _CFG_PFM
{
  char name[NAME_MAX];
  char file[NAME_MAX];
  uchar endian;
} CFG_PFM;

typedef struct _CFG_TAR
{
  char   name[NAME_MAX];
  char   file[NAME_MAX];
  char   targ[NAME_MAX];
  char   path[PATH_MAX];
  int    i;
  char   bname[BASES_COUNT][NAME_MAX];
  BASES  bases;
} CFG_TAR;

typedef CFG_ORG CFG_PRO;

typedef struct ___CFG
{
  char      *name;
  char      *file;
  CFG_ORG     org;
  CFG_PFM     pfm;
  CFG_TAR     tar;
  CFG_PRO     pro;
  HACKL        hl;
  CODES        cl;
  BASE      *base;
  HACK      *hack;
  CODE      *code;
} __CFG;

typedef struct _CFG
{
  /// \brief Contains language to use e.g. en-US
  char lang[7];
  /// \brief Contains Original Index of Hacks
  uchar  hl[ HACKS_COUNT ];
  /// \brief Contains Original Index of Codes
  uchar  cl[ CODES_COUNT ];
  /** \brief Semi-Permanent Data, this is what dst "Resets" to
    Saved to file at app termination and upon initiating the save action,
    can be reset beforehand by reloading the file
  **/
  __CFG src;
  /// \brief Temporary data saved by GUI, lost at app termination
  __CFG dst;
} CFG;

CFG* appGetCfg( void );

char const* appGetCwp( void );
char const* appGetCwd( void );
char const* appGetExe( void );
char const* appGetVer( void );

SHUT_C
