#pragma once
#ifndef _TYPES_H
#define _TYPES_H

#include <limits.h>
#include <float.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#ifdef __cplusplus
#define C_EXT extern "C"
#define C_OPEN C_EXT {
#define C_SHUT }
#else
#define C_EXT
#define C_OPEN
#define C_SHUT
#endif

C_OPEN

#ifndef HUGE
#ifdef _MSC_VER
/** \brief min 64bit **/
#define HUGE __int64
#define H( NUM ) NUM##I64
#else
/** \brief min 64bit **/
#define HUGE long long
#define H( NUM ) NUM##LL
#endif
/** \brief min 64bit **/
typedef HUGE huge;
#endif

/** \brief min 64bit **/
typedef unsigned  HUGE uhv;
/** \brief min 64bit **/
typedef   signed  HUGE shv;
/** \brief min 32bit **/
typedef unsigned  long ulv;
/** \brief min 32bit **/
typedef   signed  long slv;
/** \brief min 16bit, normally 32bit **/
typedef unsigned   int uiv;
/** \brief min 16bit, normally 32bit **/
typedef   signed   int siv;
/** \brief min 16bit **/
typedef unsigned short usv;
/** \brief min 16bit **/
typedef   signed short ssv;
/** \brief min 6bit, normally 8bit **/
typedef unsigned  char ucv;
/** \brief min 6bit, normally 8bit **/
typedef   signed  char scv;

typedef        SSIZE_T szv;
typedef         SIZE_T uzv;
typedef        INT_PTR spv;
typedef       UINT_PTR upv;

/** \brief min 32bit **/
typedef          float fpv;
/** \brief min 64bit **/
typedef         double dpv;
/** \brief min 64bit, normally 80bit **/
typedef    long double lpv;

C_SHUT

#endif // _ME_TYPES_H
