#pragma once
#include <iup.h>
#include "appLng.h"
#include "appCfg.h"

OPEN_C

typedef struct _GUI_MAIN
{
  Ihandle *fset;
  Ihandle *vb;
} GUI_MAIN;

typedef struct _GUI_TEXT
{
  Ihandle *fset;
  Ihandle *tb;
} GUI_TEXT;

void guiText_OnInit( GUI_TEXT *guiText, Icallback OnKAny, Icallback OnValueChanged );

typedef struct _GUI_SPIN
{
  Ihandle *fset;
  Ihandle *tb;
  Ihandle *spin;
} GUI_SPIN;

void guiSpin_OnInit( GUI_SPIN *guiSpin, Icallback OnKAny, Icallback OnValueChanged );

typedef struct _GUI_ORG
{
  GUI_MAIN main;
  GUI_TEXT name;
  GUI_TEXT file;
} GUI_ORG;

int guiName_OnKAny( Ihandle *ih, int c );
int guiFile_OnKAny( Ihandle *ih, int c );
int guiName_OnValueChanged( Ihandle *ih );
int guiFile_OnValueChanged( Ihandle *ih );


typedef struct _GUI_BASE
{
  GUI_MAIN main;
  GUI_MAIN base;
  GUI_TEXT name;
  GUI_SPIN addr;
  GUI_SPIN size;
  GUI_SPIN depth;
} GUI_BASE;

int guiAddr_OnKAny( Ihandle *ih, int c );
int guiSize_OnKAny( Ihandle *ih, int c );
int guiDepth_OnKAny( Ihandle *ih, int c );
int guiAddr_OnValueChanged( Ihandle *ih );
int guiSize_OnValueChanged( Ihandle *ih );
int guiDepth_OnValueChanged( Ihandle *ih );

typedef struct _GUI_PFM
{
  GUI_MAIN main;
  GUI_TEXT name;
  GUI_TEXT file;
  GUI_SPIN memory;
  Ihandle *listEndian;
#ifdef GUI_SHARED
  GUI_BASE base;
#else
  GUI_BASE base[0x10];
#endif
} GUI_PFM;

int guiEndian_OnValueChanged( Ihandle *ih );

typedef struct _GUI_TAR
{
  GUI_MAIN main;
  GUI_TEXT name;
  GUI_TEXT file;
  GUI_TEXT path;
  GUI_TEXT targ;
} GUI_TAR;

typedef struct _GUI_CMP
{
  GUI_MAIN    main;
  Ihandle *listCmp;
} GUI_CMP;

typedef enum _MEVAL
{
  VAL_SCHAR = '\0',
  VAL_UCHAR,
  VAL_SSHRT,
  VAL_USHRT,
  VAL_SLONG,
  VAL_ULONG,
  VAL_SHUGE,
  VAL_UHUGE,
  VAL_SINT,
  VAL_UINT,
  VAL_FPN,
  VAL_DPN,
  VAL_LPN,
  VAL_COUNT
} MEVAL_T;

typedef struct _GUI_VAL
{
  GUI_MAIN     main;
  GUI_MAIN    value;
  Ihandle  *tbValue;
  Ihandle *listType;
} GUI_VAL;

int guiVal_OnKAny( Ihandle *ih, int c );
int guiVal_OnValueChanged( Ihandle *ih );

typedef enum _LIST_T
{
  LIST_FIRST = 0,
  LIST_PREV,
  LIST_NEXT,
  LIST_FINAL,
  LIST_NEW,
  LIST_DEL,
  LIST_MOV,
  LIST_COUNT
} LIST_T;

typedef struct _GUI_CODE
{
  GUI_MAIN     main;
  GUI_MAIN     code;
  GUI_SPIN  spinVal;
  GUI_CMP       cmp;
  GUI_VAL       val;
  Ihandle *listCode;
} GUI_CODE;

typedef struct _GUI_CODES
{
  GUI_MAIN      main;
  GUI_MAIN     codes;
  GUI_CODE      code;
  Ihandle *treeCodes;
  Ihandle   *listMod;
  Ihandle   *listPos;
} GUI_CODES;

typedef struct _GUI_HACK
{
  GUI_MAIN   main;
  GUI_TEXT   name;
  GUI_CODES codes;
  GUI_CODE   code;
  Ihandle *spinID;
} GUI_HACK;

typedef struct _GUI_HACKS
{
  GUI_MAIN   main;
  GUI_MAIN  hacks;
  GUI_HACK   hack;
  GUI_CODES codes;
  Ihandle *treeHacks;
  Ihandle *listPos;
} GUI_HACKS;

typedef enum _QRY_T
{
  QRY_ANY = 0,
  QRY_MUST,
  QRY_NONE,
  QRY_COUNT
} QRY_T;

typedef struct _GUI_QRY
{
  GUI_MAIN     main;
  GUI_CMP       cmp;
  Ihandle *listType;
  Ihandle  *listVal;
  Ihandle  *spinVal;
  uchar q; uchar  c;
#ifdef GUI_SHARED
  GUI_VAL val;
#else
  GUI_VAL val[QRY_COUNT][CMP_COUNT];
#endif
} GUI_QRY;

typedef struct _GUI_RES
{
  GUI_MAIN   main;
  GUI_CODES codes;
  /* Matrix Box */
  Ihandle   *mbMain;
} GUI_RES;

typedef struct _GUI
{
  char     font[20];
  char  fontSize[5];
  Ihandle  *dlgMain;
  Ihandle   *vbMain;
  Ihandle *listMain;
  Ihandle  *listSet;
  GUI_QRY       qry;
  GUI_RES       res;
  GUI_ORG       org;
  GUI_PFM       pfm;
  GUI_TAR       tar;
  GUI_HACKS   hacks;
  GUI_CODES  *codes;
  GUI_BASE    *base;
  GUI_VAL      *val;
  GUI_CMP      *cmp;
} GUI;

GUI* appGetGui( void );
void execHacksId( pid_t pid );

Ihandle* meMkList( Icallback func, ... );
#define guiGetListPosFset( ih, pos ) IupGetChild( IupGetChild( ih, 0 ), pos )
#define guiGetListPosLbl(  ih, pos ) IupGetChild( guiGetListPosFset(ih, pos) , 0 )
#define guiGetListPos_Fset( vbox, pos ) IupGetChild( vbox, pos )
#define guiGetListPos_Lbl(  vbox, pos ) IupGetChild( guiGetListPos_Fset( vbox, pos ), 0 )

// Used by min 2 initialisers, search & code sections
void  guiCmp_OnInit( void );
void  guiVal_OnInit( void );

// On Language Change should call these
void  guiOrg_OnLang( void );
void  guiPfm_OnLang( void );
void guiBase_OnLang( void );
void  guiTar_OnLang( void );
void  guiPfl_OnLang( void );
void  guiCmp_OnLang( void );
void  guiVal_OnLang( void );

int   guiOrg_OnShow( Ihandle *ih );
int   guiPfm_OnShow( Ihandle *ih );
int  guiBase_OnShow( Ihandle *ih );
int   guiTar_OnShow( Ihandle *ih );
int   guiPfl_OnShow( Ihandle *ih );
int   guiQry_OnShow( Ihandle *ih );

/**
  \brief Loads *.dll, *.so, etc
  \param name 1st part of library filename, version and extension is appended to this
  \param hfunc Pointer to pointer that will receive hack communication objects pointer
  \param cfunc Pointer to pointer that will receive code communication objects pointer
  \return Reference to library loaded
  Loads a library and acquires the objects needed for communicating with it,
  also sets the pointers on the object needed by library to query required info.
 **/
HMODULE meLoadLib( char* name, HACK_FUNC **hfunc, CODE_FUNC **cfunc );
/** \brief Frees *.dll, *.so, etc
  \param lib reference to library to free
  \return NULL or 0
  Frees Library and returns NULL for simple cleaning of variable holding reference
**/
HMODULE meFreeLib( HMODULE lib );

int guiMenu_OnValueChanged( Ihandle *ih );
int meListP_ButtonCB( Ihandle *ih, int button, int pressed, int x, int y, char* status );
int meListP_MotionCB( Ihandle *sbox, int x, int y, char* status );
/** \brief Converts number to text
  \param value Number to convert
  \param dst buffer that will hold text (assumed to be at least _CVTBUFSIZE length)
  \param sigDigits How many digits after decimal point to include
  Unsafe - Assumes there will be enough char elements, will NULL terminate dst
**/
void ftoa( double value, char* dst, int sigDigits );
/** \brief Converts number to text
  \param value Number to convert
  \param dst buffer that will hold text
  \param limit on number of characters in text
  \param sigDigits How many digits after decimal point to include
  Will NULL terminate dst
**/
void ftoa_s( double value, char *dst, size_t length, int sigDigits );

SHUT_C
