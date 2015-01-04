#include "guiPfl.h"
GUI_PFL guiPfl = {{NULL}};
void guiPfl_OnLang( void )
{
  IupSetAttribute( guiPfl.main.fset, IUP_TITLE, appLang->x[ LNG_PROFILE ] );
  IupSetAttribute( guiPfl.name.fset, IUP_TITLE, appLang->x[ LNG_NAME    ] );
  IupSetAttribute( guiPfl.file.fset, IUP_TITLE, appLang->x[ LNG_FILE    ] );
}
extern void guiTar_OnInit( void );
void guiPfl_OnInit( void )
{
  guiTar_OnInit();
#ifdef GUI_SHARED
  guiPfl.main = guiOrg.main;
  guiPfl.name = guiOrg.name;
  guiPfl.file = guiOrg.file;
#else
  guiText_OnInit( &guiPfl.name, (Icallback)guiName_OnKAny, guiName_OnValueChanged );
  guiText_OnInit( &guiPfl.file, (Icallback)guiFile_OnKAny, guiFile_OnValueChanged );
  guiPfl.main.vb = IupVbox( guiPfl.name.fset, guiPfl.file.fset, NULL );
  guiPfl.main.fset = IupFrane( guiPfl.main.vb );
#endif
  guiPfl_OnLang();
}
int guiPfl_OnShow( Ihandle *ih )
{
  srcName = srcPro.name;
  srcFile = srcPro.file;
  tmpName = tmpPro.name;
  tmpFile = tmpPro.file;
  guiText_SendShowMsg( &guiPfl.name, tmpName );
  guiText_SendShowMsg( &guiPfl.file, tmpFile );
  IupSetAttribute( guiPfl.main.fset, "FLOATING", IUP_NO );
  guiPfl_OnLang();
  return IUP_DEFAULT;
}
