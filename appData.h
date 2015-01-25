#pragma once
#include "_hack.h"

OPEN_C

typedef struct _DATA_ORG
{
  char name[NAME_MAX];
  char file[NAME_MAX];
} DATA_ORG;

typedef struct _DATA_PFM
{
  char name[NAME_MAX];
  char file[NAME_MAX];
  char endian;
} DATA_PFM;

typedef struct _BNAME
{
  char a[NAME_MAX];
} BNAME;
typedef struct _DATA_TAR
{
  char name[NAME_MAX];
  char file[NAME_MAX];
  char targ[NAME_MAX];
  char path[PATH_MAX];
  BNAME bname[BASES_COUNT];
  BASES bases;
} DATA_TAR;

typedef DATA_ORG DATA_PRO;

/**
  \param Contains language to use e.g. en-US
**/
typedef struct _SESSION
{
  char lang[7];
  char  org[NAME_MAX];
  char  pfm[NAME_MAX];
  char base[NAME_MAX];
  char  tar[NAME_MAX];
  char  pro[NAME_MAX];
  hack_t hi;
  uchar  ci;
} SESSION;

typedef void (*_METHODS_PATH_FUNC)( char *path );
typedef void (*_METHODS_FILE_FUNC)( int  fd    );
typedef void (*_METHODS_SET_FUNC) ( void );
typedef void (*_METHODS_ADD_FUNC) ( uchar insert );
typedef void (*_METHODS_MOV_FUNC) ( schar x, schar y );

typedef struct _METHODS
{
  _METHODS_PATH_FUNC OnDefPath;
  _METHODS_PATH_FUNC OnDefExt;
  _METHODS_FILE_FUNC OnLoad;
  _METHODS_FILE_FUNC OnSave;
  _METHODS_SET_FUNC  OnApply;
  _METHODS_SET_FUNC  OnReset;
  _METHODS_SET_FUNC  OnRem;
  _METHODS_ADD_FUNC  OnAdd;
  _METHODS_MOV_FUNC  OnMov;
} METHODS;

extern SESSION  appSession;
extern DATA_ORG     srcOrg;
extern DATA_ORG     tmpOrg;
extern DATA_PFM     srcPfm;
extern DATA_PFM     tmpPfm;
extern DATA_PRO     srcPro;
extern DATA_PRO     tmpPro;
extern DATA_TAR     srcTar;
extern DATA_TAR     tmpTar;
extern HACKL      tmpHacks;
extern HACK       *srcHack;
extern HACK       *tmpHack;
extern CODES     *srcCodes;
extern CODES     *tmpCodes;
extern CODES      resCodes;
extern CODE       *srcCode;
extern CODE       *tmpCode;
extern METHODS  appMethods;
extern HACK_COM      *hCOM;

uchar GetBase( char *name );
char const* appGetCwp( void );
char const* appGetCwd( void );
char const* appGetExe( void );
char const* appGetVer( void );

SHUT_C
