#include "guiHacks.h"
void execHacksId( pid_t pid )
{
  pid_t buf[3] = {0, 0, pid};
  STARTUPINFO si = { sizeof(si), 0 };
  PROCESS_INFORMATION pi = {0};
  si.lpReserved2 = (BYTE*)buf;
#ifdef _DEBUG
#if _WIN == 32
  CreateProcess( "hackProc-msw-x86-d-vc.exe",
#else
  CreateProcess( "hackProc-msw-x64-d-vc.exe",
#endif
#else
#if _WIN == 32
  CreateProcess( "hackProc-msw-x86-vc.exe",
#else
  CreateProcess( "hackProc-msw-x64-vc.exe",
#endif
#endif
    NULL, NULL, NULL, 0, 0, NULL, NULL, &si, &pi );
}
GUI_HACKS guiHacks = {{NULL}};
// Lists original index or HACK_T_MAX if new
hack_t *tmpIndex = NULL;
// Syncs name with hack
HACKL tmpHacks = {0};
void guiHacks_OnLang( void )
{
  IupSetAttribute( guiHacks.main.fset, IUP_TITLE, appLang->a[ LNG_HACKS ].a );
}
extern void  guiPro_OnInit( void );
extern void guiHack_OnInit( void );
void hacksReSize( HACKL *hl, hack_t **indexList, hack_t count )
{
  count = count ? count : 1;
  hack_t c =  ( hl->hacks ? hl->hacks->c : 0);
  hack_t _c = ( hl->hacks ? hl->hacks->_c : 0);
  size_t s = (count - 1) * sizeof(HACK);
  if ( count <= _c && hl->hacks )
  {
    hl->hacks->c = count;
    if ( count >= c )
      return;
    c = _c - count;
    memset( &hl->names[count], 0, c * NAME_MAX );
    memset( &hl->hacks->a[count], 0, c * sizeof(HACK) );
    if ( indexList )
      memset( &indexList[count], 0, c * sizeof(hack_t) );
    return;
  }
  if ( !hl->hacks )
  {
    hl->hacks = malloc( sizeof(HACKS) );
    hl->hacks->c = 0;
    hl->hacks->i = 0;
  }
  hl->hacks = realloc( hl->hacks, sizeof(HACKS) + s );
  hl->names = realloc( hl->names, count * NAME_MAX );
  hl->hacks->_c = count;
  hl->hacks->s  = s;
  hack_t i = hl->hacks->c;
  c = count - i;
  memset( &hl->names[i], 0, c * NAME_MAX );
  memset( &hl->hacks->a[i], 0, c * sizeof(HACK) );
  if ( indexList )
  {
    *indexList = realloc( *indexList, count * sizeof(hack_t) );
    memset( &(*indexList)[i], 0, c * sizeof(hack_t) );
  }
  hl->hacks->c = count;
}
int guiHacks_OnSelection(Ihandle *ih, int id, int status)
{
  if ( tmpIndex )
    tmpHacks.hacks->i = (id < 0) ? 0 : (hack_t)id;
  return IUP_DEFAULT;
}
void guiHacks_OnInit( void )
{
   guiPro_OnInit();
  guiHack_OnInit();
  guiHacks.ddFormat = IupList( NULL );
  IupSetAttribute( guiHacks.ddFormat, IUP_DROPDOWN, IUP_YES );
  IupSetAttribute( guiHacks.ddFormat, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute( guiHacks.ddFormat, "FLOATING", IUP_YES );
  guiHacks.treeHacks = IupTree();
  IupSetAttribute( guiHacks.treeHacks, "ADDROOT", IUP_NO );
  IupSetAttribute( guiHacks.treeHacks, IUP_EXPAND, IUP_YES );
  IupSetAttribute( guiHacks.treeHacks, "FLOATING", IUP_YES );
  IupSetCallback( guiHacks.treeHacks, "SELECTION_CB", (Icallback)guiHacks_OnSelection );
#ifdef GUI_SHARED
  guiHacks.main      = guiOrg.main;
  IupAppend( guiHacks.main.vb, guiHacks.ddFormat  );
  IupMap( guiHacks.ddFormat  );
  IupAppend( guiHacks.main.vb, guiHacks.treeHacks );
  IupMap( guiHacks.treeHacks );
#else
  guiHacks.main.vb = IupVbox( guiHacks.ddFormat, guiHacks.treeHacks, NULL );
  guiHacks.main.fset = IupFrame( guiHacks.main.vb );
#endif
}
void guiHacks_BuildTree( Ihandle *ih, int i )
{
  HACK *hack = &tmpHacks.hacks->a[i];
  char *name = tmpHacks.names[i].a;
  if ( strcmp( name, appLang->a[ LNG_NEW ].a ) >= 0 )
  {
    char no[5] = {0};
    _itoa_s( hack->_ci, no, 5, 16 );
    strcpy_s( name, NAME_MAX, appLang->a[ LNG_NEW ].a );
    strcat_s( name, NAME_MAX, " (x" );
    strcat_s( name, NAME_MAX, no );
    strcat_s( name, NAME_MAX, ")" );
  }
  if ( hack->_fi > 0 )
  {
    hack->use = 1;
    IupSetStrAttributeId( ih, "ADDBRANCH", i - 1, name );
    IupSetAttributeId( ih, "TOGGLEVISIBLE", i, IUP_NO );
    guiHacks_BuildTree( ih, hack->_fi );
  }
  else
  {
    IupSetStrAttributeId( ih, "ADDLEAF", i - 1, name );
    IupSetAttributeId( ih, "TOGGLEVISIBLE", i, IUP_YES );
    IupSetAttributeId( ih, "TOGGLEVALUE", i, hack->use ? IUP_ON : IUP_OFF );
  }
  if ( hack->_ni > 0 )
    guiHacks_BuildTree( ih, hack->_ni );
}
extern void guiPro_OnDefPath( char *path, uchar saveFile );
void guiHacks_OnDefPath( char *path, uchar saveFile )
{
  guiPro_OnDefPath( path, 1 );
  strcat_s( path, PATH_MAX, DIR_SEP );
}
void guiHacks_OnDefExt( char *path )
{
  strcat_s( path, PATH_MAX, "txt" );
}
void guiHacks_OnApply( void )
{
  hacksReSize( &hCOM->hl, NULL, tmpHacks.hacks->_c );
  memcpy( hCOM->hl.names, tmpHacks.names, tmpHacks.hacks->_c * NAME_MAX );
  memcpy( hCOM->hl.hacks->a, tmpHacks.hacks->a, tmpHacks.hacks->s );
  for ( hack_t i = 0; i < tmpHacks.hacks->_c; ++i )
    tmpIndex[i] = i;
}
hack_t guiHacks_OnAddUpdate( hack_t i )
{
  HACK *next = &tmpHacks.hacks->a[i];
  next->_ci = i;
  if ( next->_fi )
    i = guiHacks_OnAddUpdate( ++next->_fi );
  if ( next->_ni )
    return guiHacks_OnAddUpdate( ++next->_ni );
  return i;
}
void guiHacks_OnAdd( int i )
{
  i = IupGetInt( guiHacks.treeHacks, IUP_VALUE ) + i;
  if ( tmpIndex )
  {
    tmpHacks.hacks->a[0]._fi = 1;
    tmpHacks.hacks->a[0]._ni = 0;
    if ( i < 1 )
      i = tmpHacks.hacks->c;
    hacksReSize( &tmpHacks, &tmpIndex, tmpHacks.hacks->c + 1 );
  }
  else
  {
    i = 0;
    hacksReSize( &tmpHacks, &tmpIndex, 1 );
  }
  int p = (i - 1);
  hack_t c = tmpHacks.hacks->c - 1, n = (hack_t)(i + 1);
  char *name = tmpHacks.names[i].a;
  HACK *hack = &tmpHacks.hacks->a[i];
  HACK *prev = (p >= 0) ? &tmpHacks.hacks->a[p] : NULL;
  HACK *next = (n <= c) ? &tmpHacks.hacks->a[n] : NULL;
  memset( hack, 0, sizeof(HACK) );
  hack->_ci = (hack_t)i;
  if ( prev )
  {
    if ( hack->_ci == prev->_fi )
      hack->_pi = prev->_ci;
    else
    {
      prev->_ni = hack->_ci;
      hack->_pi = prev->_pi;
    }
  }
  if ( next )
  {
    c -= i;
    memmove( &tmpIndex[n], &tmpIndex[i], c * sizeof(hack_t) );
    memmove( name + NAME_MAX, name, c * NAME_MAX );
    memmove( next, hack, c * sizeof(HACK) );
    if ( hack->_pi != next->_pi )
    {
      tmpHacks.hacks->a[hack->_pi]._ni = n;
      hack->_ni = 0;
    }
    else
      hack->_ni = n;
    for ( n = guiHacks_OnAddUpdate( n ) + 1;
      n < tmpHacks.hacks->c; ++n )
    {
      next->_ni = n;
      next = &tmpHacks.hacks->a[n];
      next->_pi = hack->_pi;
      next->_ci = n;
      next->_ni = 0;
    }
  }
  if ( !prev )
    strcpy_s( name, NAME_MAX, appLang->a[ LNG__M_ ].a );
  else
    strcpy_s( name, NAME_MAX, appLang->a[ LNG_NEW ].a );
  guiHacks_OnShow( guiHacks.main.fset );
}
void guiHacks_OnRemUpdate( hack_t i, hack_t c, hack_t r )
{
  HACK *next = &tmpHacks.hacks->a[i];
  next->_ci = i;
  if ( next->_fi )
    guiHacks_OnAddUpdate( next->_fi >= r ? next->_fi - c : next->_fi );
  if ( next->_ni )
    guiHacks_OnAddUpdate( next->_ni >= r ? next->_ni - c : next->_ni );
}
void guiHacks_OnRem( void )
{
  hack_t i = tmpHacks.hacks->i;
  if ( !tmpIndex || i < 1 || tmpHacks.hacks->c == 1 )
    return;
  int p = ((int)i - 1);
  char *name = tmpHacks.names[i].a;
  HACK *hack = &tmpHacks.hacks->a[i];
  HACK *prev = (p >= 0) ? &tmpHacks.hacks->a[p] : NULL;
  hack_t c = tmpHacks.hacks->c - 1, n = hack->_ni ? hack->_ni : prev->_ni;
  HACK *next = (n > 0 && n <= c) ? &tmpHacks.hacks->a[n] : NULL;
  if ( next )
  {
    c -= n;
    /*
    if ( prev->_ni == n )
    {
      prev->_fi = 0;
      prev->_ni = hack->_ci;
    }
    */
    memmove( &tmpIndex[i], &tmpIndex[n], c * sizeof(hack_t) );
    memmove( name, tmpHacks.names[n].a, c * NAME_MAX );
    memmove( hack, next, c * sizeof(HACK) );
    c = tmpHacks.hacks->c;
    hacksReSize( &tmpHacks, &tmpIndex, tmpHacks.hacks->c - (n - i) );
  }
  else
  {
    /*
    if ( prev->_ni == hack->_ci )
      prev->_ni = 0;
    else
      prev->_fi = 0;
    */
    c = tmpHacks.hacks->c;
    hacksReSize( &tmpHacks, &tmpIndex, tmpHacks.hacks->c - 1 );
  }
  guiHacks_OnRemUpdate( 0, c - tmpHacks.hacks->c, i );
  guiHacks_OnShow( guiHacks.main.fset );
}
void guiHacks_OnReset( void )
{
  hacksReSize( &tmpHacks, NULL, ( hCOM && hCOM->hl.hacks ) ? hCOM->hl.hacks->_c : 0 );
  if ( hCOM && tmpIndex )
  {
    memcpy( tmpHacks.names, hCOM->hl.names, hCOM->hl.hacks->_c * NAME_MAX );
    memcpy( tmpHacks.hacks->a, hCOM->hl.hacks->a, hCOM->hl.hacks->s );
    for ( hack_t i = 0; i < tmpHacks.hacks->_c; ++i )
      tmpIndex[i] = i;
  }
}
void guiHacks_OnLoad( int fd, FILE *file )
{
  char path[ PATH_MAX ] = {0};
  strcpy_s( path, PATH_MAX, ipGetUsrDirA() );
  strcat_s( path, PATH_MAX, ".medit" DIR_SEP "temp" DIR_SEP );
  guiHacks_OnDefPath( path, 1 );
  hCOM->OnLoad( file, path );
}
void guiHacks_OnSave( int fd, FILE *file )
{
  char path[ PATH_MAX ] = {0};
  strcpy_s( path, PATH_MAX, ipGetUsrDirA() );
  strcat_s( path, PATH_MAX, ".medit" DIR_SEP "temp" DIR_SEP );
  guiHacks_OnDefPath( path, 1 );
  hCOM->OnSave( file, path );
}
int guiHacks_OnShow( Ihandle *ih )
{
  IupSetAttribute( guiHacks.main.fset, "FLOATING", IUP_NO );
  IupSetAttribute( guiHacks.ddFormat,  "FLOATING", IUP_NO );
  IupSetAttribute( guiHacks.treeHacks, "FLOATING", IUP_NO );
  appMethods.OnDefPath = guiHacks_OnDefPath;
  appMethods.OnDefExt  = guiHacks_OnDefExt;
  appMethods.OnLoad    = guiHacks_OnLoad;
  appMethods.OnReset   = guiHacks_OnReset;
  appMethods.OnSave    = guiHacks_OnSave;
  appMethods.OnApply   = guiHacks_OnApply;
  appMethods.OnAdd     = guiHacks_OnAdd;
  appMethods.OnRem     = guiHacks_OnRem;
  IupSetAttribute( guiHacks.treeHacks, "DELNODE0", "ALL" );
  if ( tmpIndex && tmpHacks.hacks->c )
  {
    guiHacks_BuildTree( guiHacks.treeHacks, 0 );
    IupSetInt( guiHacks.treeHacks, IUP_VALUE, tmpHacks.hacks->i );
  }
  guiHacks_OnLang();
  IupShow( guiHacks.ddFormat  );
  IupShow( guiHacks.treeHacks );
  return IUP_DEFAULT;
}
