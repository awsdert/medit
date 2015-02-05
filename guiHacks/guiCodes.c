#include "guiCodes.h"
GUI_CODES *guiCodes = NULL;
CODES *srcCodes = NULL;
CODES *tmpCodes = NULL;
void codesReSize( CODES **cl, uchar **indexList, uchar count )
{
  uchar i = 0, c  = 0;
  void *check;
  if ( !cl )
  {
    return;
  }

  if ( !( *cl ) )
  {
    check = malloc( sizeof( CODES ) );
    if ( !check )
      return;
    ( *cl ) = check;
    memset( *cl, 0, sizeof( CODES ) );
    ( *cl )->_c = 1;
    ( *cl )->s  = sizeof( CODE );

    if ( indexList )
    {
      check = malloc( 1 );
      if ( !check )
      {
        free( *cl );
        (*cl) = NULL;
        return;
      }
      ( *indexList ) = check;
      ( *indexList )[0] = 0;
    }
  }

  if ( count <= ( *cl )->_c )
  {
    if ( count >= ( *cl )->c )
    {
      goto updateCount;
    }

    ( *cl )->c = count;
  }
  else
  {
    size_t s = ( count - 1 ) * sizeof( CODE );
    check = realloc( *cl, sizeof( CODES ) + s );
    if ( !check )
      return;
    ( *cl ) = check;
    if ( indexList )
    {
      check = realloc( *indexList, count );
      if ( !check )
      {
        s = ( (*cl)->_c - 1 ) * sizeof( CODE );
        ( *cl ) = realloc( *cl, sizeof( CODES ) + s );
        return;
      }
      (*indexList) = check;
    }
    ( *cl )->_c = count;
    ( *cl )->s = s + sizeof( CODE );
  }

  i = ( *cl )->c;
  c = ( *cl )->_c - i;

  if ( c )
  {
    memset( &( ( *cl )->a[i] ), 0, c * sizeof( CODE ) );

    if ( indexList )
    {
      memset( &( *indexList )[i], 0, c );
    }
  }

updateCount:
  ( *cl )->c = count;
}
void guiCodes_SetGUI( GUI_CODES *codes, CODES *src, CODES *tmp )
{
  guiCodes = codes;
  srcCodes = src;
  tmpCodes = tmp;
  guiCode_SetGUI( &codes->code, &src->a[ src->i ], &tmp->a[ tmp->i ] );
}
void guiCodes_OnLang( void )
{
  IupSetStrAttribute( guiCodes->hlPos,  "1",  "|<" );
  IupSetStrAttribute( guiCodes->hlPos,  "2",  "<<" );
  IupSetStrAttribute( guiCodes->hlPos,  "3",  ">>" );
  IupSetStrAttribute( guiCodes->hlPos,  "4",  ">|" );
  IupSetStrAttribute( guiCodes->hlPos,  "5",   "+" );
  IupSetStrAttribute( guiCodes->hlPos,  "6",   "-" );
}
extern void guiCode_OnInit( void );
void guiCodes_OnInit( void )
{
  guiCodes->treeCodes = IupTree();
  guiCodes->hlPos   = IupHList( NULL );
  guiCodes->codes.vb   = IupVbox(  guiCodes->treeCodes, guiCodes->hlPos, NULL );
  guiCodes->codes.fset = IupFrame( guiCodes->codes.vb );
  IupAppend( guiCodes->main.vb, guiCodes->codes.fset );
  IupMap( guiCodes->codes.fset );
  guiCode_OnInit();
  guiCodes_OnLang();
}
uchar guiCodes_BuildTree( Ihandle *ih, uchar i, int p )
{
  char *attr = "APPENDLEAF";
  char const *value;
  uchar j = 0;
  switch ( tmpCodes->a[i].type )
  {
  case CODE_CMP:
    attr = "APPENDBRANCH";
    value = appLang->a[ LNG_COMPARISON ].a;
    break;

  case CODE_JOKER:
    attr = "APPENDBRANCH";
    value = appLang->a[ LNG_JOKER ].a;
    break;

  case CODE_W:
    value = appLang->a[ LNG_WRITE  ].a;
    break;

  case CODE_CPY:
    value = appLang->a[ LNG_COPY   ].a;
    break;

  case CODE_INC:
    value = appLang->a[ LNG_ADD    ].a;
    break;

  case CODE_DEC:
    value = appLang->a[ LNG_REMOVE ].a;
    break;

  case CODE_MASTER:
    value = appLang->a[ LNG__M_    ].a;
    break;

  default:
    value = appLang->a[ LNG_QQQ ].a;
  }

  IupSetStrAttributeId( ih, attr, p, value );

  if ( tmpCodes->a[i].type == CODE_CMP || tmpCodes->a[i].type == CODE_JOKER )
  {
    p = i++;
    j = 0;
    do
    {
      guiCodes_BuildTree( ih, i, p );
      ++j; ++i;
    }
    while ( j < tmpCodes->a[i].loop && j < UCHAR_MAX );
  }
  else
  {
    ++i;
  }

  return i;
}
int guiCodes_OnShow( Ihandle *ih )
{
  uchar i = 0;
  while ( i < tmpCodes->c )
  {
    i = guiCodes_BuildTree( guiCodes->treeCodes, i, -1 );
  }

  return IUP_DEFAULT;
}
