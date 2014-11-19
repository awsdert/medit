#pragma once
#include "_guiMain.h"
#include <ipipe/main.h>

OPEN_C

#define CAST_DMP( T ) \
  tNum = *((T*)&tar.buff[i]); \
  dNum = *((T*)&dmp.buff[i])
#define CAST_CMP( T ) \
  mNum = *((T*)&mecM->buff[c]); \
  nNum = *((T*)&mecN->buff[c])
#define COMP( CMP_T ) \
  rBl = (tNum CMP_T dNum)
#define COMPARE( CMP_T ) \
  if ( mecM->used[c] ) rBl = ( rBl &&  (tNum CMP_T mNum)); \
  if ( mecN->used[c] ) rBl = ( rBl && !(tNum CMP_T nNum))



typedef struct _MEPIPE
{
  Ipipe pipe;
  uchar buff[BUFSIZ];
} MEPIPE;

typedef union _FPN
{
    fpn val;
  uchar buf[ sizeof( fpn ) ];
} FPN;
typedef union _DPN
{
    dpn val;
  uchar buf[ sizeof( dpn ) ];
} DPN;
typedef union _LPN
{
    lpn val;
  uchar buf[ sizeof( lpn ) ];
} LPN;

typedef struct _ME_LPN
{
  uchar used[CMP_COUNT];
    LPN buff[CMP_COUNT];
} ME_LPN;

typedef struct _ME_ULINT
{
  uchar used[CMP_COUNT];
  ulint buff[CMP_COUNT];
} ME_ULINT;

typedef struct _ME_LINT
{
  uchar used[CMP_COUNT];
   lint buff[CMP_COUNT];
} ME_LINT;

int guiQryMenu_OnClick( Ihandle *ih, int button, int pressed, int x, int y, char* status );
void* bQry( pid_t pid, size_t size );
void fQry( uchar    used,
          MEPIPE   *mepI,
          MEPIPE   *mepO,
          MEPIPE   *mepP,
          Ipipe     prev,
          schar    bytes,
          ME_LPN   *mecM,
          ME_LPN   *mecN );
void iQry( uchar    used,
          MEPIPE   *mepI,
          MEPIPE   *mepO,
          MEPIPE   *mepP,
          Ipipe     prev,
          schar    bytes,
          ME_LINT  *mecM,
          ME_LINT  *mecN );
void uQry( uchar    used,
          MEPIPE   *mepI,
          MEPIPE   *mepO,
          MEPIPE   *mepP,
          Ipipe     prev,
          schar    bytes,
          ME_ULINT *mecM,
          ME_ULINT *mecN );

SHUT_C
