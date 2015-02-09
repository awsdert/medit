#pragma once
#include <float.h>
#include <ipipe/c.h>

OPEN_C

#ifdef _MSC_VER
#define snprintf sprintf_s
#endif

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

typedef union _RLN
{
  ulint u[ ( sizeof( lpn ) / sizeof( lint ) ) + ( ( sizeof( lpn ) % sizeof(
             lint ) ) ? 1 : 0 ) ];
  lint s[ ( sizeof( lpn ) / sizeof( lint ) ) + ( ( sizeof( lpn ) % sizeof(
            lint ) ) ? 1 : 0 ) ];
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

typedef struct _BASE
{
  ulint addr;
  ulint size;
  uchar depth;
} BASE;

#define BASES_COUNT 0x10
#define BASES_LAST  0xf
typedef struct _BASES
{
  uchar c;
  BASE  a[ BASES_COUNT ];
} BASES;

typedef ushort hack_t;
#define HACK_T_MAX USHRT_MAX
/**
  \brief Holds relative information about a hack
  \param use 1 = Use during hook, 0 = Do NOT use during hook
  \param irl If immeadiate children are not folders: 1 = radio, 0 = check
  \param cid Codelist ID
  \param coi Codelist Owner ID
  \param id Index
  \param ui List Index
  \param oi Owner Index
  \param fi First Child Index
  \param ni Next Sibling Index
  \param pi Prev Sibling Index
**/
typedef struct struct_HACK
{
  uchar  use : 1;
  // Is Radio List
  uchar  irl : 1;
  // ID (codelist file usage only)
  ushort cid;
  // Owner ID (codelist file usage only)
  ushort coi;
  // Current Index
  hack_t id;
  // UI Index
  hack_t ui;
  // Owner Index
  hack_t oi;
  // 1st Child Index
  hack_t fi;
  // Next Sibling Index
  hack_t ni;
  // Prev Sibling Index
  hack_t pi;
} HACK;
typedef struct VHACKS_
{
  hack_t count;
  hack_t total;
  size_t bytes;
  hack_t index;
} VHACKS;

typedef struct HACKS_
{
  VHACKS size;
  HACK   a[1];
} HACKS;

#define DIV_SIZE( T1, T2 ) (sizeof(T1) / sizeof(T2))
#define MOD_SIZE( T1, T2 ) (sizeof(T1) % sizeof(T2))
#define NODES_NEEDED( T1, T2 ) (DIV_SIZE(T1,T2) + (MOD_SIZE(T1,T2) ? 1 : 0))

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
  ulint _ulint[  NODES_NEEDED( lpn,  lint ) ];
  lint _lint[   NODES_NEEDED( lpn,  lint ) ];
  ulong _ulong[  NODES_NEEDED( lpn,  long ) ];
  long _long[   NODES_NEEDED( lpn,  long ) ];
  uint _uint[   NODES_NEEDED( lpn,   int ) ];
  int _int[    NODES_NEEDED( lpn,   int ) ];
  ushort _ushort[ NODES_NEEDED( lpn, short ) ];
  short _short[  NODES_NEEDED( lpn, short ) ];
  uchar _uchar[  sizeof( lpn ) ];
  schar _schar[  sizeof( lpn ) ];
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

#define CODEV_COUNT 20
#define CODEV_LAST  19
typedef struct _CODE
{
  uchar type : 4;
  uchar size : 4;
  // 'i', 'u', 'f', 'x', 'o', 'B' (binary), 'b' (boolean)
  char  format;
  ulint addr[3];
  NUM   a[CODEV_COUNT];
  ulong loop;
  uchar info;
  uchar _ci;
  uchar _ni;
  uchar _fi;
  uchar _pi;
  uchar _tm : 4;
} CODE;

#define CODES_COUNT 30
#define CODES_LAST  29
typedef struct VCODES_
{
  uchar  count;
  uchar  total;
  size_t bytes;
  uchar  index;
} VCODES;

typedef struct CODES_
{
  VCODES size;
  CODE   a[1];
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
