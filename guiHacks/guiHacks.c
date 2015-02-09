#include "guiHacks.h"
void execHacksId ( pid_t pid )
{
  pid_t buf[3] = {0, 0, pid};
  STARTUPINFO si = { sizeof ( si ), 0 };
  PROCESS_INFORMATION pi = {0};
  si.lpReserved2 = ( BYTE* ) buf;
#ifdef _DEBUG
#if _WIN == 32
  CreateProcess ( "hackProc-msw-x86-d-vc.exe",
#else
  CreateProcess ( "hackProc-msw-x64-d-vc.exe",
#endif
#else
#if _WIN == 32
  CreateProcess ( "hackProc-msw-x86-vc.exe",
#else
  CreateProcess ( "hackProc-msw-x64-vc.exe",
#endif
#endif
                  NULL, NULL, NULL, 0, 0, NULL, NULL, &si, &pi );
}
GUI_HACKS guiHacks = {{NULL}};
// Lists original index or HACK_T_MAX if new
hack_t *tmpHacksIndex = NULL;
// Syncs name with hack
HACKL tmpHacks = {&tmpHacksIndex, NULL};
extern void guiPfm_OnDefPath ( char *path );
extern void guiPro_OnDefPath ( char *path );
void guiHacks_OnLang ( void )
{
  int i = IupGetInt ( guiHacks.ddFormat, IUP_VALUE );
  uchar c = 0;
  char *attr;
  char name[ NAME_MAX * 2 ] = {0};
  char path[ PATH_MAX ] = {0};
  dentI entry;
  intptr_t hDir = 0;
  IupSetAttribute ( guiHacks.main.fset,  IUP_TITLE, appLang->a[ LNG_HACKS  ].a );
  IupSetAttribute ( guiHacks.fsetFormat, IUP_TITLE, appLang->a[ LNG_FORMAT ].a );
  copystri ( path, ipGetUsrDir(), PATH_MAX );
  appendstr ( path, DIR_SEP ".medit" DIR_SEP "data", PATH_MAX );
  guiPfm_OnDefPath ( path );
  appendstr ( path, DIR_SEP "*." LIB_EXT, PATH_MAX );

  if ( i > 0 )
  {
    attr = IupGetAttributeId ( guiHacks.ddFormat, IUP_TITLE, i );
    copystri ( name, attr, NAME_MAX );
  }

  IupSetInt ( guiHacks.ddFormat, IUP_VALUE, 0 );
  IupSetAttribute ( guiHacks.ddFormat, "1", NULL );
  hDir = ipDir1stEntI ( path, &entry );

  if ( hDir > 0 )
  {
    i = 0;

    do
    {
      ++i;
      IupSetStrAttribute( guiHacks.ddFormat, "APPENDITEM", entry.name );
      IupSetStrAttributeId ( guiHacks.ddFormat, "TITLE", i, entry.name );

      if ( name[0] && strcmp ( name, entry.name ) == 0 )
      {
        IupSetInt ( guiHacks.ddFormat, IUP_VALUE, i );
        c = 1;
      }
    }
    while ( ipDirNxtEntI ( hDir, &entry ) == 0 );

    ipDirShutHandle ( hDir );

    if ( !c && i > 0 )
    {
      attr = IupGetAttributeId ( guiHacks.ddFormat, IUP_TITLE, 1 );
      copystri ( name, attr, NAME_MAX * 2 );
      IupSetInt ( guiHacks.ddFormat, IUP_VALUE, 1 );
    }

    c = 1;
  }

  //*
  if ( c )
  {
    appLoadLib ( name );
  }

  //*/
}
extern void  guiPro_OnInit ( void );
extern void guiHack_OnInit ( void );

void* appRealloc( void *src, size_t oldSize, size_t newSize )
{
  void *dst = bnew( newSize );
  if ( !dst )
    return NULL;
  memcpy( dst, src, ( newSize < oldSize ) ? newSize : oldSize );
  free( src );
  return dst;
}
void LIB_EXP hacksReSize ( HACKL *hl, hack_t count )
{
  void* check;
  hack_t c;
  size_t bytes = count * sizeof ( HACK );
  if ( !hl->hacks )
  {
    check = bnew ( sizeof ( HACKS ) );

    if ( !check )
    {
      return;
    }

    hl->hacks = check;
    check = malloc ( NAME_MAX );

    if ( !check )
    {
cleanInit:
      free( hl->hacks );
      hl->hacks = NULL;

      if ( hl->names )
      {
        free( hl->names );
        hl->names = NULL;
      }

      return;
    }

    hl->names = check;
    memset ( hl->hacks, 0, sizeof ( HACKS ) );
    memset ( hl->names, 0, NAME_MAX );
    hl->hacks->size.total = 1;
    hl->hacks->size.bytes = sizeof ( HACK );

    if ( hl->index )
    {
      check = bnew ( sizeof ( hack_t ) );

      if ( !check )
      {
        goto cleanInit;
      }

      ( *hl->index ) = check;
      ( *hl->index ) [0] = 0;
    }
  }
  if ( count < hl->hacks->size.count )
  {
    hl->hacks->size.count = count;
    goto zeroArray;
  }
  else if ( count <= hl->hacks->size.total )
  {
    goto updateCount;
  }
  else
  {
    check = brenew ( hl->hacks, sizeof(VHACKS) + bytes );

    if ( !check )
    {
      return;
    }

    hl->hacks = check;
    check = brenew ( hl->names, count * NAME_MAX );

    if ( !check )
    {
      return;
    }

    hl->names = check;

    if ( hl->index )
    {
      check = brenew ( ( *hl->index ), count * sizeof ( hack_t ) );

      if ( !check )
      {
        return;
      }

      ( *hl->index ) = check;
    }

    hl->hacks->size.total = count;
    hl->hacks->size.bytes = bytes;
zeroArray:
    c = count - hl->hacks->size.count;
    ( void ) memset ( &hl->names[   hl->hacks->size.count], 0, c * NAME_MAX );
    ( void ) memset ( &hl->hacks->a[hl->hacks->size.count], 0, c * sizeof ( HACK ) );

    if ( hl->index )
    {
      ( void ) memset ( & ( *hl->index ) [hl->hacks->size.count], 0, c * sizeof ( hack_t ) );
    }
  }
updateCount:
  hl->hacks->size.count = count;
}
int guiHacks_OnSelection ( Ihandle *ih, int id, int status )
{
  if ( tmpHacksIndex )
  {
    if ( status != 0 )
    {
      tmpHacks.hacks->size.index = ( id < 1 ) ? 0 : ( hack_t ) IupGetIntId ( ih, "USERDATA",
                          id );
    }
    else
    {
      tmpHacks.hacks->size.index = 0;
    }
  }

  return IUP_DEFAULT;
}
void guiHacks_OnInit ( void )
{
  guiPro_OnInit();
  guiHack_OnInit();
  guiHacks.ddFormat = IupList ( NULL );
  IupSetAttribute ( guiHacks.ddFormat, IUP_DROPDOWN, IUP_YES );
  IupSetAttribute ( guiHacks.ddFormat, IUP_EXPAND, IUP_HORIZONTAL );
  guiHacks.fsetFormat = IupFrame ( guiHacks.ddFormat );
  IupSetAttribute ( guiHacks.fsetFormat, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute ( guiHacks.fsetFormat, "FLOATING", IUP_YES );
  guiHacks.treeHacks = IupTree();
  IupSetAttribute ( guiHacks.treeHacks, "ADDROOT", IUP_NO );
  IupSetAttribute ( guiHacks.treeHacks, IUP_EXPAND, IUP_YES );
  IupSetAttribute ( guiHacks.treeHacks, "FLOATING", IUP_YES );
  IupSetCallback ( guiHacks.treeHacks, "SELECTION_CB",
                   ( Icallback ) guiHacks_OnSelection );
#ifdef GUI_SHARED
  guiHacks.main = guiOrg.main;
  IupAppend ( guiHacks.main.vb, guiHacks.fsetFormat );
  IupMap ( guiHacks.fsetFormat );
  IupAppend ( guiHacks.main.vb, guiHacks.treeHacks );
  IupMap ( guiHacks.treeHacks );
#else
  guiHacks.main.vb = IupVbox ( guiHacks.fsetFormat, guiHacks.treeHacks, NULL );
  guiHacks.main.fset = IupFrame ( guiHacks.main.vb );
#endif
  guiHacks_OnLang();
}
hack_t guiHacks_BuildTree ( Ihandle *ih, hack_t i, hack_t ui )
{
  HACK *hack = &tmpHacks.hacks->a[i];
  char *name = tmpHacks.names[i].a;
  int p = hack->pi ? tmpHacks.hacks->a[hack->pi].ui :
          tmpHacks.hacks->a[hack->oi].ui;
  hack->ui = ui;

  if ( i == 0 )
  {
    p = -1;
  }

  if ( strcmp ( name, appLang->a[ LNG_NEW ].a ) == 0 )
  {
    char no[10] = {0};
    snprintf ( no, 10, " (0x%02X)", hack->id );
    appendstr ( name, no, NAME_MAX );
  }

  if ( hack->fi > 0 )
  {
    IupSetStrAttributeId ( ih, hack->pi ? "INSERTBRANCH" : "ADDBRANCH", p, name );
    IupSetAttributeId ( ih, "TOGGLEVISIBLE", ui, IUP_NO );
    IupSetIntId ( ih, "USERDATA", ui, i );
    ui = guiHacks_BuildTree ( ih, hack->fi, ++ui );
  }
  else
  {
    IupSetStrAttributeId ( ih, hack->pi ? "INSERTLEAF" : "ADDLEAF", p, name );
    IupSetAttributeId ( ih, "TOGGLEVISIBLE", ui, IUP_YES );
    IupSetAttributeId ( ih, "TOGGLEVALUE", ui, hack->use ? IUP_ON : IUP_OFF );
    IupSetIntId ( ih, "USERDATA", ui, i );
  }

  if ( hack->ni > 0 )
  {
    ui = guiHacks_BuildTree ( ih, hack->ni, ++ui );
  }

  return ui;
}
void guiHacks_OnAdd ( uchar insert )
{
  hack_t c = 0, i = 0;
  char *name;
  HACK *hack, *ownr, *indx;

  if ( tmpHacksIndex )
  {
    i = tmpHacks.hacks->size.index;

    if ( !i )
    {
      i = tmpHacks.hacks->a[0].fi;

      if ( i )
      {
        while ( tmpHacks.hacks->a[i].ni )
        {
          i = tmpHacks.hacks->a[i].ni;
        }
      }
    }

    for ( c = 1; c < tmpHacks.hacks->size.count; ++c )
    {
      if ( tmpHacks.names[c].a[0] == 0 )
      {
        break;
      }
    }

    if ( c == tmpHacks.hacks->size.count )
    {
      hacksReSize ( &tmpHacks, c + 1 );
    }
  }
  else
  {
    // Prep initial buffer
    hacksReSize ( &tmpHacks, 5 );
    tmpHacks.hacks->size.count = 1;
  }

  name = tmpHacks.names[c].a;
  hack = &tmpHacks.hacks->a[c];
  ownr = &tmpHacks.hacks->a[0];
  memset ( hack, 0, sizeof ( HACK ) );

  if ( i )
  {
    indx = &tmpHacks.hacks->a[i];
    ownr = &tmpHacks.hacks->a[indx->oi];

    if ( insert )
    {
      if ( indx->pi )
      {
        tmpHacks.hacks->a[indx->pi].ni = c;
      }
      else
      {
        ownr->fi = c;
      }

      hack->pi = indx->pi;
      indx->pi = c;
      hack->ni = i;
    }
    else
    {
      if ( indx->ni )
      {
        tmpHacks.hacks->a[indx->ni].pi = c;
      }

      hack->ni = indx->ni;
      indx->ni = c;
      hack->pi = i;
    }
  }
  else
  {
    ownr->fi = c;
  }

  hack->id = c;
  hack->oi = ownr->id;

  if ( !c )
  {
    copystri ( name, appLang->a[ LNG__M_ ].a, NAME_MAX );
  }
  else
  {
    copystri ( name, appLang->a[ LNG_NEW ].a, NAME_MAX );
  }

  //guiHacks_OnShow ( guiHacks.main.fset );
}
void guiHacks_OnRemUpdate ( hack_t i )
{
  HACK *hack = &tmpHacks.hacks->a[i];

  if ( hack->fi )
  {
    guiHacks_OnRemUpdate ( hack->fi );
  }

  if ( hack->ni )
  {
    guiHacks_OnRemUpdate ( hack->ni );
  }

  memset ( hack, 0, sizeof ( HACK ) );
  memset ( tmpHacks.names[i].a, 0, NAME_MAX );
  tmpHacksIndex[i] = 0;
}
void guiHacks_OnRem ( void )
{
  hack_t i;
  char *name;
  HACK *hack, *ownr, *prev, *next;

  if ( !tmpHacksIndex || !tmpHacks.hacks->size.index || tmpHacks.hacks->size.count < 2 )
  {
    return;
  }

  i = tmpHacks.hacks->size.index;
  name = tmpHacks.names[i].a;
  hack = &tmpHacks.hacks->a[i];
  ownr = &tmpHacks.hacks->a[hack->oi];
  prev = &tmpHacks.hacks->a[hack->pi];
  next = &tmpHacks.hacks->a[hack->ni];

  if ( hack->fi )
  {
    guiHacks_OnRemUpdate ( hack->fi );
  }

  if ( hack->pi )
  {
    if ( hack->ni )
    {
      next->pi = hack->pi;
      prev->ni = hack->ni;
    }
    else
    {
      prev->ni = 0;
    }
  }
  else if ( hack->ni )
  {
    ownr->fi = hack->ni;
    next->pi = 0;
  }
  else
  {
    ownr->fi = 0;
  }

  memset ( hack, 0, sizeof ( HACK ) );
  memset( name, 0, NAME_MAX );
  tmpHacksIndex[i] = 0;
  //guiHacks_OnShow ( guiHacks.main.fset );
}
void guiHacks_OnMov ( schar x, schar y )
{
  hack_t i;
  HACK *hack, *ownr, *prev, *next;

  if ( !tmpHacksIndex )
  {
    return;
  }

  i = tmpHacks.hacks->size.index;

  if ( !i )
  {
    return;
  }

  hack = &tmpHacks.hacks->a[i];
  ownr = &tmpHacks.hacks->a[hack->oi];
  prev = &tmpHacks.hacks->a[hack->pi];
  next = &tmpHacks.hacks->a[hack->ni];

  if ( y != 0 )
  {
    if ( y > 0 )
    {
      if ( !hack->pi )
      {
        if ( !hack->oi )
        {
          return;
        }

        ownr->fi = hack->ni;
        hack->ni = hack->oi;
        hack->oi = ownr->oi;

        if ( hack->ni )
        {
          next->pi = 0;
        }

        hack->pi = ownr->pi;
        ownr->pi = i;
      }
      else
      {
        prev->ni = hack->ni;

        if ( hack->ni )
        {
          next->pi = hack->pi;
        }

        if ( prev->fi && x > 0 )
        {
          hack->pi = 0;
          hack->ni = prev->fi;
          hack->oi = prev->id;

          while ( hack->ni )
          {
            hack->pi = hack->ni;
            hack->ni = tmpHacks.hacks->a[hack->ni].ni;
          }
        }
        else
        {
          hack->ni = hack->pi;
          hack->pi = prev->pi;
          prev->pi = i;
        }
      }
    }
    else if ( hack->ni )
    {
      next->pi = hack->pi;

      if ( hack->pi )
      {
        prev->ni = hack->ni;
      }
      else
      {
        ownr->fi = hack->ni;
      }

      if ( next->fi && x > 0 )
      {
        hack->oi = hack->ni;
        hack->ni = next->fi;
        next->fi = i;
        hack->pi = 0;
      }
      else
      {
        hack->pi = hack->ni;
        hack->ni = next->ni;
        next->ni = i;
      }
    }
    else if ( hack->oi )
    {
      if ( hack->pi )
      {
        prev->ni = 0;
      }
      else
      {
        ownr->fi = 0;
      }

      hack->pi = hack->oi;
      hack->oi = ownr->oi;
      hack->ni = ownr->ni;
    }
    else
    {
      return;
    }
  }
  else if ( x != 0 )
  {
    if ( x > 0 )
    {
      if ( !hack->pi )
      {
        return;
      }

      hack->oi = hack->pi;

      if ( hack->ni )
      {
        next->pi = hack->pi;
      }

      hack->pi = 0;
      prev->ni = hack->ni;
      hack->ni = prev->fi;

      while ( hack->ni )
      {
        hack->pi = hack->ni;
        hack->ni = tmpHacks.hacks->a[ hack->ni ].ni;
      }
    }
    else if ( hack->oi )
    {
      if ( !hack->pi )
      {
        ownr->fi = hack->ni;
      }
      else
      {
        prev->ni = hack->ni;
      }

      if ( hack->ni )
      {
        next->pi = hack->pi;
      }

      hack->ni = ownr->ni;
      hack->pi = hack->oi;
      hack->oi = ownr->oi;
    }
    else
    {
      return;
    }
  }
  else
  {
    return;
  }

  if ( hack->pi )
  {
    tmpHacks.hacks->a[hack->pi].ni = i;
  }
  else
  {
    tmpHacks.hacks->a[hack->oi].fi = i;
  }

  if ( hack->ni )
  {
    tmpHacks.hacks->a[hack->ni].pi = i;
  }

  //guiHacks_OnShow ( guiHacks.main.fset );
}
void guiHacks_OnDefPath ( char *path )
{
  guiPro_OnDefPath ( path );
  ipMkDir ( path );
  appendstr ( path, DIR_SEP, PATH_MAX );
}
void guiHacks_OnDefExt ( char *path )
{
  appendstr ( path, "txt", PATH_MAX );
}
void guiHacks_OnApply ( void )
{
  hack_t i = 0;

  if ( !hCOM || !tmpHacks.hacks )
  {
    return;
  }

  hacksReSize ( &hCOM->hl, tmpHacks.hacks->size.total );
  memcpy ( hCOM->hl.names, tmpHacks.names, tmpHacks.hacks->size.total * NAME_MAX );
  memcpy ( hCOM->hl.hacks->a, tmpHacks.hacks->a, tmpHacks.hacks->size.bytes );

  for ( ; i < tmpHacks.hacks->size.total; ++i )
  {
    tmpHacksIndex[i] = i;
  }
}
void guiHacks_OnReset ( void )
{
  hack_t i = 0;

  if ( !hCOM )
  {
    return;
  }

  hacksReSize ( &tmpHacks, hCOM->hl.hacks ? hCOM->hl.hacks->size.total : 0 );

  if ( hCOM && tmpHacksIndex )
  {
    memcpy ( tmpHacks.names, hCOM->hl.names, hCOM->hl.hacks->size.total * NAME_MAX );
    memcpy ( tmpHacks.hacks->a, hCOM->hl.hacks->a, hCOM->hl.hacks->size.bytes );

    for ( ; i < tmpHacks.hacks->size.total; ++i )
    {
      tmpHacksIndex[i] = i;
    }
  }
}
void guiHacks_OnLoad ( char *path )
{
  char dir[ PATH_MAX ] = {0};

  if ( !hCOM )
  {
    return;
  }

  copystri ( dir, ipGetUsrDir(), PATH_MAX );
  appendstr ( dir, DIR_SEP ".medit" DIR_SEP "temp", PATH_MAX );
  guiHacks_OnDefPath ( dir );
  hCOM->OnLoad ( path, dir );
}
void guiHacks_OnSave ( char *path )
{
  char dir[ PATH_MAX ] = {0};

  if ( !hCOM )
  {
    return;
  }

  copystri ( dir, ipGetUsrDir(), PATH_MAX );
  appendstr ( dir, DIR_SEP ".medit" DIR_SEP "temp", PATH_MAX );
  guiHacks_OnDefPath ( dir );
  hCOM->OnSave ( path, dir );
  //ipShut ( file );
}
int guiHacks_OnShow ( Ihandle *ih )
{
  appMethods.OnDefPath = guiHacks_OnDefPath;
  appMethods.OnDefExt  = guiHacks_OnDefExt;
  appMethods.OnLoad    = guiHacks_OnLoad;
  appMethods.OnReset   = guiHacks_OnReset;
  appMethods.OnSave    = guiHacks_OnSave;
  appMethods.OnApply   = guiHacks_OnApply;
  appMethods.OnAdd     = guiHacks_OnAdd;
  appMethods.OnRem     = guiHacks_OnRem;
  appMethods.OnMov     = guiHacks_OnMov;

  if ( !ih )
  {
    return IUP_DEFAULT;
  }

  IupSetAttribute ( guiHacks.main.fset,  "FLOATING", IUP_NO );
  IupSetAttribute ( guiHacks.fsetFormat, "FLOATING", IUP_NO );
  IupSetAttribute ( guiHacks.treeHacks,  "FLOATING", IUP_NO );
  IupSetAttribute ( guiHacks.treeHacks, "DELNODE0", "ALL" );

  if ( tmpHacksIndex && tmpHacks.hacks->size.count )
  {
    ( void ) guiHacks_BuildTree ( guiHacks.treeHacks, 0, 0 );
    IupSetAttributeId ( guiHacks.treeHacks, "MARKED",
                        tmpHacks.hacks->a[tmpHacks.hacks->size.index].ui, IUP_YES );
  }

  guiHacks_OnLang();
  IupShow ( guiHacks.fsetFormat );
  IupShow ( guiHacks.treeHacks );
  return IUP_DEFAULT;
}
