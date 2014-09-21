#pragma once
#include <float.h>
#include "c.h"

OPEN_C

#define U1 uint_least8_t
#define U1_MAX UINT_LEAST8_MAX
#define S1 int_least8_t
#define S1_MAX INT_LEAST8_MAX
#define S1_MIN INT_LEAST8_MIN

#define U2 uint_least16_t
#define U2_MAX UINT_LEAST16_MAX
#define S2 int_least16_t
#define S2_MAX INT_LEAST16_MAX
#define S2_MIN INT_LEAST16_MIN

#define U4 uint_least32_t
#define U4_MAX UINT_LEAST32_MAX
#define S4 int_least32_t
#define S4_MAX INT_LEAST32_MAX
#define S4_MIN INT_LEAST32_MIN

#define U8 uint_least64_t
#define U8_MAX UINT_LEAST64_MAX
#define S8 int_least64_t
#define S8_MAX INT_LEAST64_MAX
#define S8_MIN INT_LEAST64_MIN

#define UP uintptr_t
#define UP_MAX UINTPTR_MAX
#define SP intptr_t
#define SP_MAX INTPTR_MAX
#define SP_MIN INTPTR_MIN

#define UI unsigned int
#define UI_MAX UINT_MAX
#define SI   signed int
#define SI_MAX INT_MAX
#define SI_MIN INT_MIN

#define UZ size_t
#define UZ_MAX SIZE_MAX
#define SZ ssize_t
#define SZ_MAX SSIZE_MAX
#define SZ_MIN SSIZE_MIN

#define RF float
#define RF_MAX FLT_MAX
#define RF_MIN FLT_MIN

#define RD double
#define RD_MAX DBL_MAX
#define RD_MIN DBL_MIN

#define RL long double
#define RL_MAX LDBL_MAX
#define RL_MIN LDBL_MIN

#define JK_A      0x1000000000000000LL
#define JK_B      0x2000000000000000LL
#define JK_C      0x4000000000000000LL
#define JK_D      0x8000000000000000LL
#define JK_E      0x0100000000000000LL
#define JK_F      0x0200000000000000LL
#define JK_G      0x0400000000000000LL
#define JK_H      0x0800000000000000LL
#define JK_I      0x0010000000000000LL
#define JK_J      0x0020000000000000LL
#define JK_K      0x0040000000000000LL
#define JK_L      0x0080000000000000LL
#define JK_M      0x0001000000000000LL
#define JK_N      0x0002000000000000LL
#define JK_O      0x0004000000000000LL
#define JK_P      0x0008000000000000LL
#define JK_Q      0x0000100000000000LL
#define JK_R      0x0000200000000000LL
#define JK_S      0x0000400000000000LL
#define JK_T      0x0000800000000000LL
#define JK_U      0x0000010000000000LL
#define JK_V      0x0000020000000000LL
#define JK_W      0x0000040000000000LL
#define JK_X      0x0000080000000000LL
#define JK_Y      0x0000001000000000LL
#define JK_Z      0x0000002000000000LL
#define JK_0      0x0000004000000000LL
#define JK_1      0x0000008000000000LL
#define JK_2      0x0000000100000000LL
#define JK_3      0x0000000200000000LL
#define JK_4      0x0000000400000000LL
#define JK_5      0x0000000800000000LL
#define JK_6      0x0000000010000000LL
#define JK_7      0x0000000020000000LL
#define JK_8      0x0000000040000000LL
#define JK_9      0x0000000080000000LL
#define JK_LCTRL  0x0000000001000000LL
#define JK_LALT   0x0000000002000000LL
#define JK_LSHIFT 0x0000000004000000LL
#define JK_LCMD   0x0000000008000000LL
#define JK_BSLASH 0x0000000000100000LL
#define JK_PLUS   0x0000000000200000LL
#define JK_MINUS  0x0000000000400000LL
#define JK_FSLASH 0x0000000000800000LL
#define JK_RCTRL  0x0000000000010000LL
#define JK_RALT   0x0000000000020000LL
#define JK_RSHIFT 0x0000000000040000LL
#define JK_RCMD   0x0000000000080000LL

// XInput / Playstation Controller Input
#define JK_LT     0x0000000000000001LL
#define JK_L2     0x0000000000000001LL
#define JK_RT     0x0000000000000002LL
#define JK_R2     0x0000000000000002LL
#define JK_LB     0x0000000000000004LL
#define JK_L1     0x0000000000000004LL
#define JK_RB     0x0000000000000008LL
#define JK_R1     0x0000000000000008LL

#define JK_X_Y    0x0000000000000010LL
#define JK_P_T    0x0000000000000010LL
#define JK_X_B    0x0000000000000020LL
#define JK_P_C    0x0000000000000020LL
#define JK_X_A    0x0000000000000040LL
#define JK_P_X    0x0000000000000040LL
#define JK_X_X    0x0000000000000080LL
#define JK_P_S    0x0000000000000080LL

#define JK_BACK   0x0000000000000100LL
#define JK_SELECT 0x0000000000000100LL
#define JK_LS     0x0000000000000200LL
#define JK_L3     0x0000000000000200LL
#define JK_RS     0x0000000000000400LL
#define JK_R3     0x0000000000000400LL
#define JK_START  0x0000000000000800LL

// Hatch Switch: Up, Right, Down, Left
#define JK_HS_U   0x0000000000001000LL
#define JK_HS_R   0x0000000000002000LL
#define JK_HS_D   0x0000000000004000LL
#define JK_HS_L   0x0000000000008000LL

typedef union _RLN
{
  U8 u;
  S8 s;
  RL r;
} RLN;

typedef union _RLD
{
  U8 u;
  S8 s;
  RD r;
} RLD;

typedef union _RLF
{
  U8 u;
  S8 s;
  RF r;
} RLF;

typedef enum   _MECMP
{
  /* ==, != */
  CMP_EQ = '\0',
  CMP_NE,
  /* >, >= */
  CMP_MT, CMP_ME,
  /* <, <= */
  CMP_LT, CMP_LE,
  /* &, !& */
  CMP_IA, CMP_NA,
  /* |, !| */
  CMP_IO, CMP_NO,
  /* ^, !^ */
  CMP_IX, CMP_NX,
  CMP_COUNT,
  CMP_DUMP,
  CMP_UNDO,
  CMP_REDO
} MECMP_T;

typedef enum _CODE_T
{
  CODE_W = 0,
  CODE_INC,
  CODE_DEC,
  CODE_CMP,
  CODE_JOKER,
  CODE_CPY,
  CODE_MASTER,
  CODE_COUNT
} CODE_T;

typedef enum _ENDIAN_TYPE
{
  ENDIAN_LITTLE_BIG = 0,
  ENDIAN_LITTLE,
  ENDIAN_BIG,
  ENDIAN_COUNT
} ENDIAN_TYPE;

typedef struct _BASE
{
  U1 endian : 4;
  U8 addr : 64;
  U8 size : 64;
} BASE;

#define BASES_LIMIT 0xf
typedef struct _BASES
{
  U1 c : 4;
  BASE a[ BASES_LIMIT ];
} BASES;

typedef struct _HACK
{
  U1       use : 1;
  U1 radioList : 1;
  U1     depth : 4;
  // ID (codelist file usage only)
  U2        id : 16;
  // Current Index
  U2        ci : 16;
  // Parent Index
  U2        pi : 16;
  // 1st Child Index
  U2        fi : 16;
  // Next Sibling Index
  U2        ni : 16;
} HACK;

#define HACKS_LIMIT 0xffu
typedef struct _HACKS
{
  U1   c : 8;
  HACK a[HACKS_LIMIT];
} HACKS;

typedef enum _DATA_TYPE
{
  DATA_TYPE_UI = 0,
  DATA_TYPE_SI,
  DATA_TYPE_RF,
  DATA_TYPE_COUNT
} DATA_TYPE;

#define CODE_LIMIT 20u
typedef struct _CODE
{
  U1 type : 4;
  U1 addrSize : 4;
  U1 dataSize : 4;
  U1 dataType : 4;
  U1 info;
  U1 loop;
  U8 addr[2];
  RLN a[CODE_LIMIT];
} CODE;

#define CODES_LIMIT 30u
typedef struct _CODES
{
  U1   c : 8;
  CODE a[ CODES_LIMIT ];
} CODES;

/*
  This is merely for %TMP%\PROC_ID\info.bin, it ensures a consistent data
  format for the GUI to write and the injected functions to read
*/
typedef struct _HACKINFO
{
  BASES bases;
  HACKS hacks;
  CODES codes;
} HACKINFO;

SHUT_C
