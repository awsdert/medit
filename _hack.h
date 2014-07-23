#pragma once
#ifndef __HACK_H
#define __HACK_H
#include <Ipipe.h>
C_OPEN

typedef enum _END_T
{
  END_LIT = 0,
  END_LBG,
  END_BIG,
  END_COUNT
} END_T;

typedef enum _REG_T
{
  REG_US = 0,
  REG_EU,
  REG_UK,
  REG_AUS,
  REG_DE,
  REG_JP,
  /* REG_COUNT Limit is 63 */
  REG_COUNT
} REG_T;

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
  /* ~, !~ */
  CMP_IR, CMP_NR,
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
  CODE_MASTER,
  CODE_COUNT
} CODE_T;
/** \brief Object holding info about target memory
  \param addr [1] is 32bit address, [0] extends it to 64bit
  \param size [1] is 32bit size, [0] extends it to 64bit
  \param endian Byte Order of memory
**/
typedef struct _BLOCK
{
  ulv   addr[2];
  ulv   size[2];
  END_T endian;
} BLOCK;
#define BLOCKS_COUNT 16
typedef struct _BLOCKS
{
  BLOCK buff[ BLOCKS_COUNT ];
} BLOCKS;

typedef struct _CODE CODE;
/** \brief Object holding usable information about codes read
  \param first Used only by comparison/joker codes
  \param next Used for fast traversal of Hacks codelist
  \param type How the code data should be treated
  \param tmem ID of object holding Target Memory Information
  \param info Comparison to be performed, Number of values in write/inc/dec codes
  \param loop How many iterations a code has (0 is treated as 1), Not used by Comparison/Joker codes
  \param sval Is Value signed
  \param ptr Is Value pointer
  \param addr [1] is 32bit address, [0] extends to 64bit address
  \param size How many bytes a value uses
  \param buff Buffer/Array of values
**/
struct _CODE
{
  CODE  *first;
  CODE  *next;
  CODE_T type;
  ucv    tmem;
  ulv    info;
  ulv    loop;
  ucv    sval;
  ucv    ptr;
  ulv    addr[2];
  ucv    size;
  void  *buff;
};
/** \brief Maximum no of codes allowed in codelist buffer
  Used for fast file load/save of buffer
**/
#define CODES_COUNT 20
typedef struct _CODES
{
  ucv  size;
  CODE buff[CODES_COUNT];
} CODES;

#define NAME_LEN  12
#define NAME_LAST 11
#define HACK_ACTIVE 0x80
#define HACK_RADIOL 0x40
typedef struct _HACK HACK;
/** \brief Object holding info about a hack
  \param first Used only by folder types
  \param next Used for fast traversal of hacklist
  \param id Hack ID, used for making text version of hack
  \param pid Parent Hack ID, used for making text version of hack
  \param info Is hack active, Is hack a radiolist
  \param name Brief description of hack
  \param size Number of Codes in buffer
  \param buff Buffer/Array of Codes
**/
struct _HACK
{
  HACK *first;
  HACK *next;
  long  id;
  long  pid;
  ucv   info;
  char  name[ NAME_LEN ];
  ucv   size;
};

/** \brief Maximum no of codes allowed in codelist buffer
  Used for fast file load/save of buffer
**/
#define HACKS_COUNT 256
/** \brief Object holding buffer for hacklist
  \param size Number of hacks
  \param buff Buffer/Array for hacklist
**/
typedef struct _HACKS
{
  usv  size;
  HACK buff[HACKS_COUNT];
} HACKS;

/** \brief Object through which DLL and EXE can communicate with each other
  \param txt2raw pointer DLL must fill when it is attached
  \param raw2txt pointer DLL must fill when it is attached
**/

typedef struct _HACK_FUNC
{
  long (*txt2raw)( HACK *raw, Ipipe *pipe );
  void (*raw2txt)( HACK *raw, Ipipe *pipe, long gid );
} HACK_FUNC;

typedef struct _CODELIST
{
  ucv rows;
  ucv cols;
  char x[30][50];
} CODELIST;

/** \brief Object through which DLL and EXE can communicate with each other
  \param getRamNo pointer EXE must fill after acquiring object pointer
  \param txt2raw pointer DLL must fill when it is attached
  \param raw2txt pointer DLL must fill when it is attached
**/
typedef struct _CODE_FUNC
{
  ucv  (*getRamNo)( char* id );
  ucv  (*txt2raw)( CODE *raw, CODELIST *cl, ucv line );
  void (*raw2txt)( CODE *raw, Ipipe *pipe );
} CODE_FUNC;

typedef HACK_FUNC* (*GETHACKFUNCS)( void );
typedef CODE_FUNC* (*GETCODEFUNCS)( void );

C_SHUT

#endif
