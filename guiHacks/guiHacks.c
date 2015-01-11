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
void guiHacks_OnLang ( void )
{
  IupSetAttribute ( guiHacks.main.fset, IUP_TITLE, appLang->a[ LNG_HACKS ].a );
}
extern void  guiPro_OnInit ( void );
extern void guiHack_OnInit ( void );
void hacksReSize ( HACKL *hl, hack_t **indexList, hack_t count )
{
  hack_t  n = count ? count : 1;
  hack_t  c = ( hl->hacks ? hl->hacks->c  : 0 );
  hack_t _c = ( hl->hacks ? hl->hacks->_c : 0 );
  size_t  s = ( n - 1 ) * sizeof ( HACK );
  if ( count <= _c && hl->hacks )
  {
    hl->hacks->c = count;
    if ( hl->hacks->i > count )
    {
      hl->hacks->i = count;
    }
    if ( count >= c )
    {
      return;
    }
    c -= count;
    ( void ) memset ( &hl->names[count], 0, c * NAME_MAX );
    ( void ) memset ( &hl->hacks->a[count], 0, c * sizeof ( HACK ) );
    if ( indexList )
    {
      ( void ) memset ( & ( *indexList ) [count], 0, c * sizeof ( hack_t ) );
    }
    return;
  }
  if ( !hl->hacks )
  {
    hl->hacks = malloc ( sizeof ( HACKS ) );
    hl->hacks->c = 0;
    hl->hacks->i = 0;
  }
  hl->hacks = realloc ( hl->hacks, sizeof ( HACKS ) + s );
  hl->names = realloc ( hl->names, n * NAME_MAX );
  hl->hacks->_c = n;
  hl->hacks->s  = s + sizeof ( HACK );
  hack_t i = hl->hacks->c;
  c = n - i;
  ( void ) memset ( &hl->names[i], 0, c * NAME_MAX );
  ( void ) memset ( &hl->hacks->a[i], 0, c * sizeof ( HACK ) );
  if ( indexList )
  {
    *indexList = realloc ( *indexList, n * sizeof ( hack_t ) );
    ( void ) memset ( & ( *indexList ) [i], 0, c * sizeof ( hack_t ) );
  }
  hl->hacks->c = count;
}
int guiHacks_OnSelection ( Ihandle *ih, int id, int status )
{
  if ( tmpIndex )
  {
    if ( status )
    {
      tmpHacks.hacks->i = ( id < 0 ) ? 0 : ( hack_t ) id;
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
  IupSetAttribute ( guiHacks.ddFormat, "FLOATING", IUP_YES );
  guiHacks.treeHacks = IupTree();
  IupSetAttribute ( guiHacks.treeHacks, "ADDROOT", IUP_NO );
  IupSetAttribute ( guiHacks.treeHacks, IUP_EXPAND, IUP_YES );
  IupSetAttribute ( guiHacks.treeHacks, "FLOATING", IUP_YES );
  IupSetCallback ( guiHacks.treeHacks, "SELECTION_CB", ( Icallback ) guiHacks_OnSelection );
#ifdef GUI_SHARED
  guiHacks.main      = guiOrg.main;
  IupAppend ( guiHacks.main.vb, guiHacks.ddFormat );
  IupMap ( guiHacks.ddFormat );
  IupAppend ( guiHacks.main.vb, guiHacks.treeHacks );
  IupMap ( guiHacks.treeHacks );
#else
  guiHacks.main.vb = IupVbox ( guiHacks.ddFormat, guiHacks.treeHacks, NULL );
  guiHacks.main.fset = IupFrame ( guiHacks.main.vb );
#endif
}
hack_t guiHacks_BuildTree ( Ihandle *ih, hack_t i, hack_t ui )
{
  HACK *hack = &tmpHacks.hacks->a[i];
  char *name = tmpHacks.names[i].a;
  int p = hack->pi ? tmpHacks.hacks->a[hack->pi].ui : tmpHacks.hacks->a[hack->oi].ui;
  hack->ui = ui;
  if ( i == 0 )
  {
    p = -1;
  }
  if ( strcmp ( name, appLang->a[ LNG_NEW ].a ) >= 0 )
  {
    char no[5] = {0};
    _itoa_s ( hack->ci, no, 5, 16 );
    strcpy_s ( name, NAME_MAX, appLang->a[ LNG_NEW ].a );
    strcat_s ( name, NAME_MAX, " (x" );
    strcat_s ( name, NAME_MAX, no );
    strcat_s ( name, NAME_MAX, ")" );
  }
  if ( hack->fi > 0 )
  {
    IupSetStrAttributeId ( ih, hack->pi ? "INSERTBRANCH" : "ADDBRANCH", p, name );
    IupSetAttributeId ( ih, "TOGGLEVISIBLE", ui, IUP_NO );
    ui = guiHacks_BuildTree ( ih, hack->fi, ++ui );
  }
  else
  {
    IupSetStrAttributeId ( ih, hack->pi ? "INSERTLEAF" : "ADDLEAF", p, name );
    IupSetAttributeId ( ih, "TOGGLEVISIBLE", ui, IUP_YES );
    IupSetAttributeId ( ih, "TOGGLEVALUE", ui, hack->use ? IUP_ON : IUP_OFF );
  }
  if ( hack->ni > 0 )
  {
    ui = guiHacks_BuildTree ( ih, hack->ni, ++ui );
  }
  return ui;
}
extern void guiPro_OnDefPath ( char *path, uchar saveFile );
void guiHacks_OnDefPath ( char *path, uchar saveFile )
{
  guiPro_OnDefPath ( path, 1 );
  strcat_s ( path, PATH_MAX, DIR_SEP );
}
void guiHacks_OnDefExt ( char *path )
{
  strcat_s ( path, PATH_MAX, "txt" );
}
void guiHacks_OnApply ( void )
{
  hacksReSize ( &hCOM->hl, NULL, tmpHacks.hacks->_c );
  memcpy ( hCOM->hl.names, tmpHacks.names, tmpHacks.hacks->_c * NAME_MAX );
  memcpy ( hCOM->hl.hacks->a, tmpHacks.hacks->a, tmpHacks.hacks->s );
  for ( hack_t i = 0; i < tmpHacks.hacks->_c; ++i )
  {
    tmpIndex[i] = i;
  }
}
void guiHacks_OnAdd ( uchar insert )
{
  hack_t c = 0, i = 0;
  if ( tmpIndex )
  {
    i = tmpHacks.hacks->i;
    tmpHacks.hacks->a[0].fi = 1;
    tmpHacks.hacks->a[0].ni = 0;
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
  char *name = tmpHacks.names[c].a;
  HACK *hack = &tmpHacks.hacks->a[c];
  HACK *ownr = &tmpHacks.hacks->a[0];
  memset ( hack, 0, sizeof ( HACK ) );
  if ( i && i != c )
  {
    HACK *prev = &tmpHacks.hacks->a[i];
    HACK *next = &tmpHacks.hacks->a[i];
    if ( insert )
    {
      ownr = &tmpHacks.hacks->a[next->oi];
      if ( next->pi )
      {
        tmpHacks.hacks->a[next->pi].ni = c;
      }
      else
      {
        ownr->fi = c;
      }
      hack->ni = next->ci;
      hack->pi = next->pi;
      next->pi = c;
    }
    else
    {
      ownr = &tmpHacks.hacks->a[prev->oi];
      if ( prev->ni )
      {
        tmpHacks.hacks->a[prev->ni].pi = c;
      }
      hack->pi = prev->ci;
      hack->ni = prev->ni;
      prev->ni = c;
    }
  }
  else
  {
    ownr->fi = c;
  }
  hack->ci = c;
  hack->oi = ownr->ci;
  if ( !c )
  {
    strcpy_s ( name, NAME_MAX, appLang->a[ LNG__M_ ].a );
  }
  else
  {
    strcpy_s ( name, NAME_MAX, appLang->a[ LNG_NEW ].a );
  }
  guiHacks_OnShow ( guiHacks.main.fset );
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
  _strset_s ( tmpHacks.names[i].a, NAME_MAX, 0 );
  tmpIndex[i] = 0;
}
void guiHacks_OnRem ( void )
{
  if ( !tmpIndex || tmpHacks.hacks->i < 1 || tmpHacks.hacks->c <= 1 )
  {
    return;
  }
  hack_t i = tmpHacks.hacks->i;
  char *name = tmpHacks.names[i].a;
  HACK *hack = &tmpHacks.hacks->a[i];
  HACK *ownr = &tmpHacks.hacks->a[hack->oi];
  HACK *prev = &tmpHacks.hacks->a[hack->pi];
  HACK *next = &tmpHacks.hacks->a[hack->ni];
  if ( hack->fi )
  {
    guiHacks_OnRemUpdate ( hack->fi );
  }
  if ( hack->pi )
  {
    if ( hack->ni )
    {
      next->pi = prev->ci;
      prev->ni = next->ci;
    }
    else
    {
      prev->ni = 0;
    }
  }
  else if ( hack->ni )
  {
    ownr->fi = next->ci;
    next->pi = 0;
  }
  else
  {
    ownr->fi = 0;
  }
  memset ( hack, 0, sizeof ( HACK ) );
  _strset_s ( name, NAME_MAX, 0 );
  tmpIndex[i] = 0;
  guiHacks_OnShow ( guiHacks.main.fset );
}
void guiHacks_OnMov ( schar x, schar y )
{
  if ( !tmpIndex )
  {
    return;
  }
  hack_t i = tmpHacks.hacks->i;
  if ( !i )
  {
    return;
  }
  HACK *hack = &tmpHacks.hacks->a[i];
  HACK *ownr = &tmpHacks.hacks->a[hack->oi];
  HACK *prev = &tmpHacks.hacks->a[hack->pi];
  HACK *next = &tmpHacks.hacks->a[hack->ni];
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
  guiHacks_OnShow ( guiHacks.main.fset );
}
void guiHacks_OnReset ( void )
{
  hacksReSize ( &tmpHacks, NULL, ( hCOM && hCOM->hl.hacks ) ? hCOM->hl.hacks->_c : 0 );
  if ( hCOM && tmpIndex )
  {
    memcpy ( tmpHacks.names, hCOM->hl.names, hCOM->hl.hacks->_c * NAME_MAX );
    memcpy ( tmpHacks.hacks->a, hCOM->hl.hacks->a, hCOM->hl.hacks->s );
    for ( hack_t i = 0; i < tmpHacks.hacks->_c; ++i )
    {
      tmpIndex[i] = i;
    }
  }
}
void guiHacks_OnLoad ( int fd, FILE *file )
{
  char path[ PATH_MAX ] = {0};
  strcpy_s ( path, PATH_MAX, ipGetUsrDirA() );
  strcat_s ( path, PATH_MAX, ".medit" DIR_SEP "temp" DIR_SEP );
  guiHacks_OnDefPath ( path, 1 );
  hCOM->OnLoad ( file, path );
}
void guiHacks_OnSave ( int fd, FILE *file )
{
  char path[ PATH_MAX ] = {0};
  strcpy_s ( path, PATH_MAX, ipGetUsrDirA() );
  strcat_s ( path, PATH_MAX, ".medit" DIR_SEP "temp" DIR_SEP );
  guiHacks_OnDefPath ( path, 1 );
  hCOM->OnSave ( file, path );
}
int guiHacks_OnShow ( Ihandle *ih )
{
  IupSetAttribute ( guiHacks.main.fset, "FLOATING", IUP_NO );
  IupSetAttribute ( guiHacks.ddFormat,  "FLOATING", IUP_NO );
  IupSetAttribute ( guiHacks.treeHacks, "FLOATING", IUP_NO );
  appMethods.OnDefPath = guiHacks_OnDefPath;
  appMethods.OnDefExt  = guiHacks_OnDefExt;
  appMethods.OnLoad    = guiHacks_OnLoad;
  appMethods.OnReset   = guiHacks_OnReset;
  appMethods.OnSave    = guiHacks_OnSave;
  appMethods.OnApply   = guiHacks_OnApply;
  appMethods.OnAdd     = guiHacks_OnAdd;
  appMethods.OnRem     = guiHacks_OnRem;
  appMethods.OnMov     = guiHacks_OnMov;
  IupSetAttribute ( guiHacks.treeHacks, "DELNODE0", "ALL" );
  if ( tmpIndex && tmpHacks.hacks->c )
  {
    ( void ) guiHacks_BuildTree ( guiHacks.treeHacks, 0, 0 );
    //IupSetAttributeId( guiHacks.treeHacks, "MARKED", 0, IUP_NO );
    IupSetAttributeId ( guiHacks.treeHacks, "MARKED", tmpHacks.hacks->a[tmpHacks.hacks->i].ui, IUP_YES );
  }
  guiHacks_OnLang();
  IupShow ( guiHacks.ddFormat );
  IupShow ( guiHacks.treeHacks );
  return IUP_DEFAULT;
}
