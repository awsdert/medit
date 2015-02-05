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
hack_t *tmpIndex = NULL;
// Syncs name with hack
HACKL tmpHacks = {0};
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
  strncpyi ( path, ipGetUsrDir(), PATH_MAX );
  appendstr ( path, DIR_SEP ".medit" DIR_SEP "data", PATH_MAX );
  guiPfm_OnDefPath ( path );
  appendstr ( path, DIR_SEP "*." LIB_EXT, PATH_MAX );

  if ( i > 0 )
  {
    attr = IupGetAttributeId ( guiHacks.ddFormat, IUP_TITLE, i );
    strncpyi ( name, attr, NAME_MAX );
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
      strncpyi ( name, attr, NAME_MAX * 2 );
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
void hacksReSize ( HACKL *hl, hack_t **indexList, hack_t count )
{
  hack_t i, c, _c, n = count ? count : 1;
  void* check;
  if ( !hl->hacks )
  {
    check = malloc ( sizeof ( HACKS ) );
    if ( !check )
      return;
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
    hl->hacks->_c = 1;
    hl->hacks->s = sizeof ( HACK );

    if ( indexList )
    {
      check = malloc ( sizeof ( hack_t ) );
      if ( !check )
        goto cleanInit;
      ( *indexList ) = check;
      ( *indexList ) [0] = 0;
    }
  }
   c = hl->hacks->c;
  _c = hl->hacks->_c;

  if ( count <= _c )
  {
    if ( count >= c )
    {
      goto updateCount;
    }

    hl->hacks->c = count;
  }
  else
  {
    size_t  s = ( n - 1 ) * sizeof ( HACK );
    check = realloc ( hl->hacks, sizeof ( HACKS ) + s );
    if ( !check )
      return;
    hl->hacks = check;
    check = realloc ( hl->names, n * NAME_MAX );
    if ( !check )
    {
clean:
      s = (hl->hacks->_c - 1) * sizeof(HACK);
      hl->hacks = realloc( hl->hacks, sizeof ( HACKS ) + s );
      hl->names = realloc( hl->names, hl->hacks->_c * NAME_MAX );
      return;
    }
    if ( indexList )
    {
      check = realloc ( ( *indexList ), n * sizeof ( hack_t ) );
      if ( !check )
        goto clean;
      ( *indexList ) = check;
    }
    hl->names = check;
    hl->hacks->_c = n;
    hl->hacks->s  = s + sizeof ( HACK );
  }

  i = hl->hacks->c;
  c = n - i;
  ( void ) memset ( &hl->names[i], 0, c * NAME_MAX );
  ( void ) memset ( &hl->hacks->a[i], 0, c * sizeof ( HACK ) );

  if ( indexList )
  {
    ( void ) memset ( & ( *indexList ) [i], 0, c * sizeof ( hack_t ) );
  }

updateCount:
  hl->hacks->c = count;
}
int guiHacks_OnSelection ( Ihandle *ih, int id, int status )
{
  if ( tmpIndex )
  {
    if ( status != 0 )
    {
      tmpHacks.hacks->i = ( id < 1 ) ? 0 : ( hack_t ) IupGetIntId ( ih, "USERDATA",
                          id );
    }
    else
    {
      tmpHacks.hacks->i = 0;
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
  if ( tmpIndex )
  {
    i = tmpHacks.hacks->i;

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

    for ( c = 1; c < tmpHacks.hacks->c; ++c )
    {
      if ( tmpHacks.names[c].a[0] == 0 )
      {
        break;
      }
    }

    if ( c == tmpHacks.hacks->c )
    {
      hacksReSize ( &tmpHacks, &tmpIndex, c + 1 );
    }
  }
  else
  {
    // Prep initial buffer
    hacksReSize ( &tmpHacks, &tmpIndex, 5 );
    tmpHacks.hacks->c = 1;
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
    strncpyi ( name, appLang->a[ LNG__M_ ].a, NAME_MAX );
  }
  else
  {
    strncpyi ( name, appLang->a[ LNG_NEW ].a, NAME_MAX );
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
  tmpIndex[i] = 0;
}
void guiHacks_OnRem ( void )
{
  hack_t i;
  char *name;
  HACK *hack, *ownr, *prev, *next;
  if ( !tmpIndex || !tmpHacks.hacks->i || tmpHacks.hacks->c < 2 )
  {
    return;
  }

     i = tmpHacks.hacks->i;
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
  tmpIndex[i] = 0;
  //guiHacks_OnShow ( guiHacks.main.fset );
}
void guiHacks_OnMov ( schar x, schar y )
{
  hack_t i;
  HACK *hack, *ownr, *prev, *next;
  if ( !tmpIndex )
  {
    return;
  }

  i = tmpHacks.hacks->i;

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
  mkdir ( path );
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
    return;
  hacksReSize ( &hCOM->hl, NULL, tmpHacks.hacks->_c );
  memcpy ( hCOM->hl.names, tmpHacks.names, tmpHacks.hacks->_c * NAME_MAX );
  memcpy ( hCOM->hl.hacks->a, tmpHacks.hacks->a, tmpHacks.hacks->s );

  for ( ; i < tmpHacks.hacks->_c; ++i )
  {
    tmpIndex[i] = i;
  }
}
void guiHacks_OnReset ( void )
{
  hack_t i = 0;
  if ( !hCOM )
    return;
  hacksReSize ( &tmpHacks, &tmpIndex, hCOM->hl.hacks ? hCOM->hl.hacks->_c : 0 );

  if ( hCOM && tmpIndex )
  {
    memcpy ( tmpHacks.names, hCOM->hl.names, hCOM->hl.hacks->_c * NAME_MAX );
    memcpy ( tmpHacks.hacks->a, hCOM->hl.hacks->a, hCOM->hl.hacks->s );

    for ( ; i < tmpHacks.hacks->_c; ++i )
    {
      tmpIndex[i] = i;
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
  strncpyi ( dir, ipGetUsrDir(), PATH_MAX );
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
  strncpyi ( dir, ipGetUsrDir(), PATH_MAX );
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

  if ( tmpIndex && tmpHacks.hacks->c )
  {
    ( void ) guiHacks_BuildTree ( guiHacks.treeHacks, 0, 0 );
    //IupSetAttributeId( guiHacks.treeHacks, "MARKED", 0, IUP_NO );
    IupSetAttributeId ( guiHacks.treeHacks, "MARKED",
                        tmpHacks.hacks->a[tmpHacks.hacks->i].ui, IUP_YES );
  }

  guiHacks_OnLang();
  IupShow ( guiHacks.fsetFormat );
  IupShow ( guiHacks.treeHacks );
  return IUP_DEFAULT;
}
