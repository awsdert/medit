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
  IupSetAttribute( guiHacks.main.fset, IUP_TITLE, appLang->x[ LNG_HACKS ] );
}
extern void  guiPfl_OnInit( void );
extern void guiHack_OnInit( void );
hack_t* hacksReSize( HACKL *hl, hack_t *indexList, hack_t count )
{
  HACKS *hacks = hl->hacks;
  hack_t c = hacks->c;
  if ( count <= hacks->_c )
  {
    hacks->c = count;
    if ( count >= c )
      return indexList;
    c = hacks->_c - count;
    memset( hl->names[count], 0, c * NAME_MAX );
    memset( &hacks->a[count], 0, c * sizeof(HACK) );
    if ( indexList )
      memset( &indexList[count], 0, c * sizeof(hack_t) );
    return indexList;
  }
  hacks->_c = count;
  hacks->s = count * sizeof(HACK);
  hacks = realloc( hacks, hacks->s + ( 3 * sizeof(hack_t) ) + sizeof(size_t) );
  hl->names = realloc( hl->names, count * NAME_MAX );
  hl->hacks = hacks;
  c = count - hacks->c;
  memset( hl->names[hacks->c], 0, c * NAME_MAX );
  memset( &hacks->a[hacks->c], 0, c * sizeof(HACK) );
  if ( indexList )
  {
    indexList = realloc( indexList, count * sizeof(hack_t) );
    memset( &indexList[hacks->c], 0, c * sizeof(hack_t) );
  }
  hacks->c = count;
  return indexList;
}
void guiHacks_OnInit( void )
{
   guiPfl_OnInit();
  guiHack_OnInit();
  guiHacks.ddFormat = IupList( NULL );
  IupSetAttribute( guiHacks.ddFormat, IUP_DROPDOWN, IUP_YES );
  IupSetAttribute( guiHacks.ddFormat, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute( guiHacks.ddFormat, "FLOATING", IUP_YES );
  guiHacks.treeHacks = IupTree();
  IupSetAttribute( guiHacks.treeHacks, IUP_EXPAND, IUP_YES );
  IupSetAttribute( guiHacks.treeHacks, "FLOATING", IUP_YES );
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
void guiHacks_BuildTree( Ihandle *ih, hack_t i )
{
  HACK *hack = &tmpHacks.hacks->a[i];
  int p = i ? hack->_pi : -1;
  if ( hack->_fi )
  {
    hack->use = 1;
    IupSetAttributeId( ih, "ADDBRANCH", p, tmpHacks.names[i] );
    IupSetAttributeId( ih, "TOGGLEVISIBLE", i, IUP_NO );
    for ( i = hack->_fi; i; i = tmpHacks.hacks->a[i]._ni )
      guiHacks_BuildTree( ih, i );
  }
  else
  {
    IupSetAttributeId( ih, "ADDLEAF", p, tmpHacks.names[i] );
    IupSetAttributeId( ih, "TOGGLEVISIBLE", i, IUP_YES );
    IupSetAttributeId( ih, "TOGGLEVALUE", i, hack->use ? IUP_ON : IUP_OFF );
  }
}
void guiHacks_OnDefPath( char *path )
{
  guiPfl_OnDefPath( path );
  strcat_s( path, PATH_MAX, DIR_SEP );
  strcat_s( path, PATH_MAX, srcPro.file );
}
void guiHacks_OnDefExt( char *path )
{
  strcat_s( path, PATH_MAX, ".txt" );
}
void guiHacks_OnApply( void )
{
  hacksReSize( &hCOM->hl, NULL, tmpHacks.hacks->_c );
  memcpy( hCOM->hl.names, tmpHacks.names, tmpHacks.hacks->_c * NAME_MAX );
  memcpy( hCOM->hl.hacks->a, tmpHacks.hacks->a, tmpHacks.hacks->s );
  for ( hack_t i = 0; i < tmpHacks.hacks->_c; ++i )
    tmpIndex[i] = i;
}
void guiHacks_OnReset( void )
{
  hacksReSize( &tmpHacks, NULL, hCOM->hl.hacks->_c );
  memcpy( tmpHacks.names, hCOM->hl.names, hCOM->hl.hacks->_c * NAME_MAX );
  memcpy( tmpHacks.hacks->a, hCOM->hl.hacks->a, hCOM->hl.hacks->s );
  for ( hack_t i = 0; i < tmpHacks.hacks->_c; ++i )
    tmpIndex[i] = i;
}
void guiHacks_OnLoad( Ihandle *file )
{
  char path[ PATH_MAX ] = {0};
  strcat_s( path, PATH_MAX, appGetCwp() );
  guiHacks_OnDefPath( path );
  hCOM->OnLoad( file, path );
}
void guiHacks_OnSave( Ihandle *file )
{
  char path[ PATH_MAX ] = {0};
  strcat_s( path, PATH_MAX, appGetCwp() );
  guiHacks_OnDefPath( path );
  hCOM->OnSave( file, path );
}
int guiHacks_OnShow( Ihandle *ih )
{
  IupSetAttribute( guiHacks.main.fset, "FLOATING", IUP_NO );
  IupSetAttribute( guiHacks.ddFormat,  "FLOATING", IUP_NO );
  IupSetAttribute( guiHacks.treeHacks, "FLOATING", IUP_NO );
  if ( tmpHacks.hacks )
    guiHacks_BuildTree( guiHacks.treeHacks, 0 );
  guiHacks_OnLang();
  return IUP_DEFAULT;
}
