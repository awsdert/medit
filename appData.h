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

typedef struct _SESSION
{
  /// \brief Contains language to use e.g. en-US
  char lang[7];
  char  org[NAME_MAX];
  char  pfm[NAME_MAX];
  char base[NAME_MAX];
  char  tar[NAME_MAX];
  char  pro[NAME_MAX];
  hack_t hi;
  uchar  ci;
} SESSION;

typedef struct _METHODS
{
  void (*OnDefPath)( char *path );
  void  (*OnDefExt)( char *path );
  void    (*OnLoad)( Ipipe *file );
  void   (*OnReset)( void );
  void    (*OnSave)( Ipipe *file );
  void   (*OnApply)( void );
} METHODS;

extern SESSION appSession;
extern DATA_ORG    srcOrg;
extern DATA_ORG    tmpOrg;
extern DATA_PFM    srcPfm;
extern DATA_PFM    tmpPfm;
extern DATA_PRO    srcPro;
extern DATA_PRO    tmpPro;
extern DATA_TAR    srcTar;
extern DATA_TAR    tmpTar;
extern HACKL     tmpHacks;
extern HACK      *srcHack;
extern HACK      *tmpHack;
extern CODES    *srcCodes;
extern CODES    *tmpCodes;
extern CODES     resCodes;
extern CODE      *srcCode;
extern CODE      *tmpCode;
extern METHODS appMethods;
extern CODE_LIB_COM *cCOM;
extern HACK_LIB_COM *hCOM;

uchar GetBase( char *name );
char const* appGetCwp( void );
char const* appGetCwd( void );
char const* appGetExe( void );
char const* appGetVer( void );

SHUT_C
