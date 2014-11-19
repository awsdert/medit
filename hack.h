#pragma once
#include <float.h>
#include <ipipe/c.h>

OPEN_C

#define JK_CTRL   0x10000000
#define JK_ALT    0x20000000
#define JK_SHIFT  0x40000000
#define JK_CMD    0x80000000
#define JK_GAMEP  0x08000000

// Game Controller Input
#define JK_L2     0x00000001
#define JK_R2     0x00000002
#define JK_L1     0x00000004
#define JK_R1     0x00000008
#define JK_T      0x00000010
#define JK_C      0x00000020
#define JK_X      0x00000040
#define JK_S      0x00000080
#define JK_SELECT 0x00000100
#define JK_L3     0x00000200
#define JK_R3     0x00000400
#define JK_START  0x00000800
// Hatch Switch: Up, Right, Down, Left
#define JK_HS_U   0x00001000
#define JK_HS_R   0x00002000
#define JK_HS_D   0x00004000
#define JK_HS_L   0x00008000

#define DIV_SIZE( T1, T2 ) (sizeof(T1) / sizeof(T2))
#define MOD_SIZE( T1, T2 ) (sizeof(T1) % sizeof(T2))
#define NODES_NEEDED( T1, T2 ) (DIV_SIZE(T1,T2) + (MOD_SIZE(T1,T2) ? 1 : 0))

typedef union _RLN
{
  ulint u[ (sizeof(lpn) / sizeof(lint)) + ((sizeof(lpn) % sizeof(lint)) ? 1 : 0) ];
   lint s[ (sizeof(lpn) / sizeof(lint)) + ((sizeof(lpn) % sizeof(lint)) ? 1 : 0) ];
    lpn r;
} RLN;

typedef union _RLD
{
  ulint u;
   lint s;
    dpn r;
} RLD;

typedef union _RLF
{
  ulong u;
   long s;
  float r;
} RLF;

static uchar const NUM_d_buff_size     = NODES_NEEDED( lpn,   dpn );
static uchar const NUM_f_buff_size     = NODES_NEEDED( lpn,   fpn );
static uchar const NUM_lint_buff_size  = NODES_NEEDED( lpn,  lint );
static uchar const NUM_long_buff_size  = NODES_NEEDED( lpn,  long );
static uchar const NUM_int_buff_size   = NODES_NEEDED( lpn,   int );
static uchar const NUM_short_buff_size = NODES_NEEDED( lpn, short );

typedef union _NUM
{
     lpn l;
     dpn d[       NODES_NEEDED( lpn,   dpn ) ];
     fpn f[       NODES_NEEDED( lpn,   fpn ) ];
   ulint u_lint[  NODES_NEEDED( lpn,  lint ) ];
    lint s_lint[  NODES_NEEDED( lpn,  lint ) ];
   ulong u_long[  NODES_NEEDED( lpn,  long ) ];
    long s_long[  NODES_NEEDED( lpn,  long ) ];
    uint u_int[   NODES_NEEDED( lpn,   int ) ];
     int s_int[   NODES_NEEDED( lpn,   int ) ];
  ushort u_short[ NODES_NEEDED( lpn, short ) ];
   short s_short[ NODES_NEEDED( lpn, short ) ];
   uchar u_char[  sizeof(lpn) ];
   schar s_char[  sizeof(lpn) ];
} NUM;

typedef enum   _MECMP
{
  CMP_DUMP = -1,
  /* ==, != */
  CMP_EQ, CMP_NE,
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
  CMP_UNDO,
  CMP_REDO
} MECMP_T;

typedef enum _CODE_T
{
  CODE_W = 0,
  CODE_CPY,
  CODE_INC,
  CODE_DEC,
  CODE_CMP,
  CODE_JOKER,
  CODE_MASTER,
  CODE_COUNT
} CODE_T;

typedef enum _ENDIAN_TYPE
{
  ENDIAN_SYS = 0,
  ENDIAN_L,
  ENDIAN_LB,
  ENDIAN_B,
  ENDIAN_COUNT
} ENDIAN_TYPE;

typedef struct _BASE
{
  ulint addr;
  ulint size;
} BASE;

#define BASES_COUNT 0x10
#define BASES_LAST  0xf
typedef struct _BASES
{
  uchar c;
  BASE  a[ BASES_COUNT ];
} BASES;

#define HACK_ACTIVE 0x80
#define HACK_RADIOL 0x40
typedef struct _HACK
{
  uchar   use : 1;
  // Is Radio List
  uchar    rl : 1;
  uchar depth : 4;
  // ID (codelist file usage only)
  uchar   id;
  // Current Index
  uchar  _ci;
  // Parent Index
  uchar  _pi;
  // 1st Child Index
  uchar  _fi;
  // Next Sibling Index
  uchar  _ni;
} HACK;

#define HACKS_COUNT 0xff
#define HACKS_LAST  0xfe
typedef struct _HACKS
{
  uchar c : 8;
  HACK  a[HACKS_COUNT];
} HACKS;

typedef enum _DATA_TYPE
{
  DATA_TYPE_UI = 0,
  DATA_TYPE_SI,
  DATA_TYPE_RF,
  DATA_TYPE_COUNT
} DATA_TYPE;

#define CODEV_COUNT 20
#define CODEV_LAST  19
typedef struct _CODE
{
  uchar _ci;
  uchar _ni;
  uchar _fi;
  uchar _pi;
  uchar _tm      : 4;
  uchar type     : 4;
  uchar addrSize : 4;
  uchar dataSize : 4;
  uchar dataType : 4;
  uchar info;
  ulong loop;
  ulint addr[3];
  RLN   a[CODEV_COUNT];
} CODE;

#define CODES_COUNT 30
#define CODES_LAST  29
typedef struct _CODES
{
  uchar c : 8;
  CODE  a[ CODES_COUNT ];
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
