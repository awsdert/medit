#include "guiCodes.h"
GUI_CODES *guiCodes = NULL;
uchar *tmpCodesIndex = NULL;
CODEL  tmpCodes = {&tmpCodesIndex, NULL};
CODEL *refCodes = &tmpCodes;
void codesReSize( CODEL *cl, uchar count )
{
  uchar i = 0, c  = 0;
  void *check;

  if ( !cl )
  {
    return;
  }

  if ( !cl->codes )
  {
    check = malloc( sizeof( CODES ) );

    if ( !check )
    {
      return;
    }

    cl->codes = check;
    memset( cl->codes, 0, sizeof( CODES ) );
    cl->codes->_c = 1;
    cl->codes->s  = sizeof( CODE );

    if ( cl->index )
    {
      check = malloc( 1 );

      if ( !check )
      {
        free( cl->codes );
        cl->codes = NULL;
        return;
      }

      ( *cl->index ) = check;
      ( *cl->index )[0] = 0;
    }
  }

  if ( count <= cl->codes->_c )
  {
    if ( count >= cl->codes->c )
    {
      goto updateCount;
    }

    cl->codes->c = count;
  }
  else
  {
    size_t s = count * sizeof( CODE );
    check = appRealloc( cl->codes, CODES_SIZE + cl->codes->s, CODES_SIZE + s );

    if ( !check )
    {
      return;
    }

    cl->codes = check;

    if ( cl->index )
    {
      check = appRealloc( ( *cl->index ), cl->codes->_c, count );

      if ( !check )
      {
        return;
      }

      ( *cl->index ) = check;
    }

    cl->codes->_c = count;
    cl->codes->s = s;
  }

  i = cl->codes->c;
  c = cl->codes->_c - i;

  if ( c )
  {
    memset( &( cl->codes->a[i] ), 0, c * sizeof( CODE ) );

    if ( cl->index )
    {
      memset( &( *cl->index )[i], 0, c );
    }
  }

updateCount:
  cl->codes->c = count;
}
void guiCodes_SetGUI( GUI_CODES *codes, CODEL *ref )
{
  guiCodes = codes;
  refCodes = ref;
  if ( !ref->codes )
    codesReSize( ref, 1 );
  guiCode_SetGUI( &codes->code, ref->codes ? &ref->codes->a[ ref->codes->i ] : NULL );
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

  switch ( tmpCodes.codes->a[i].type )
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

  if ( tmpCodes.codes->a[i].type == CODE_CMP
       || tmpCodes.codes->a[i].type == CODE_JOKER )
  {
    p = i++;
    j = 0;

    do
    {
      guiCodes_BuildTree( ih, i, p );
      ++j;
      ++i;
    }
    while ( j < tmpCodes.codes->a[i].loop && j < UCHAR_MAX );
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

  while ( i < tmpCodes.codes->c )
  {
    i = guiCodes_BuildTree( guiCodes->treeCodes, i, -1 );
  }

  return IUP_DEFAULT;
}
