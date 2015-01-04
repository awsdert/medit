#pragma once
#include "../hack.h"
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

typedef struct _ME_PIPE
{
  Ipipe pipe;
  uchar buff[BUFSIZ];
} ME_PIPE;

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


void* bQry( pid_t pid, size_t size );
void  fQry( uchar   used,
          ME_PIPE  *mepI,
          ME_PIPE  *mepO,
          ME_PIPE  *mepP,
          Ipipe     prev,
          schar    bytes,
          ME_LPN   *mecM,
          ME_LPN   *mecN );
void  iQry( uchar   used,
          ME_PIPE  *mepI,
          ME_PIPE  *mepO,
          ME_PIPE  *mepP,
          Ipipe     prev,
          schar    bytes,
          ME_LINT  *mecM,
          ME_LINT  *mecN );
void  uQry( uchar   used,
          ME_PIPE  *mepI,
          ME_PIPE  *mepO,
          ME_PIPE  *mepP,
          Ipipe     prev,
          schar    bytes,
          ME_ULINT *mecM,
          ME_ULINT *mecN );

SHUT_C
