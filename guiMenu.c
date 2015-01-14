#include "guiMain.h"
Ihandle *guiActionVB = NULL;
Ihandle *guiMenu = NULL;
Ihandle *guiData = NULL;
extern CODES srcHackCodes;
extern CODES tmpHackCodes;
extern CODES resCodes;
int  guiData_OnValueChanged ( Ihandle *ih );
int  guiMenu_OnValueChanged ( Ihandle *ih );
#define guiData_Load  1
#define guiData_Apply 2
#define guiData_Reset 3
#define guiData_Save  4
#define guiData_Add   5
#define guiData_Ins   6
#define guiData_Shu   7
#define guiData_ShuNB 8
#define guiData_Shd   9
#define guiData_ShdNB 10
#define guiData_Shl   11
#define guiData_Shr   12
#define guiData_Rem   13
#define guiData_Count guiData_Save
#define guiList_Count guiData_Rem
#ifndef EMULATOR
#define guiMenu_Org   1
#define guiMenu_Pfm   2
#define guiMenu_Tar   3
#define guiMenu_Pro   4
#define guiMenu_Qry   5
#define guiMenu_Res   6
#define guiMenu_Med   7
#define guiMenu_Hacks 8
#define guiMenu_Hack  9
#define guiMenu_Codes 10
#define guiMenu_Code  11
#define guiMenu_About 12
#else
#define guiMenu_Pro   1
#define guiMenu_Qry   2
#define guiMenu_Res   3
#define guiMenu_Med   4
#define guiMenu_Hacks 5
#define guiMenu_Hack  6
#define guiMenu_Codes 7
#define guiMenu_Code  8
#define guiMenu_About 9
#endif
#define guiMenu_Count guiMenu_About
void guiMenu_OnLang ( void )
{
  GUI_CODES *codes = guiCodes;
  CODES *srcC = srcCodes, *tmpC = tmpCodes;
  int i = IupGetInt ( guiMenu, IUP_VALUE );
  IupSetAttribute ( guiData, MSTR ( guiData_Load ), appLang->a[ LNG_LOAD         ].a );
  IupSetAttribute ( guiData, MSTR ( guiData_Apply ), appLang->a[ LNG_APPLY        ].a );
  IupSetAttribute ( guiData, MSTR ( guiData_Reset ), appLang->a[ LNG_RESET        ].a );
  IupSetAttribute ( guiData, MSTR ( guiData_Save ), appLang->a[ LNG_SAVE         ].a );
  if ( i == guiMenu_Hacks || i == guiMenu_Codes )
  {
    IupSetAttribute ( guiData, MSTR ( guiData_Add ), appLang->a[ LNG_ADD          ].a );
    IupSetAttribute ( guiData, MSTR ( guiData_Ins ), appLang->a[ LNG_INSERT       ].a );
    IupSetAttribute ( guiData, MSTR ( guiData_Shu ), appLang->a[ LNG_SHIFT_UP     ].a );
    IupSetAttribute ( guiData, MSTR ( guiData_ShuNB ), appLang->a[ LNG_SHIFT_UP__NB_     ].a );
    IupSetAttribute ( guiData, MSTR ( guiData_Shd ), appLang->a[ LNG_SHIFT_DOWN   ].a );
    IupSetAttribute ( guiData, MSTR ( guiData_ShdNB ), appLang->a[ LNG_SHIFT_DOWN__NB_   ].a );
    IupSetAttribute ( guiData, MSTR ( guiData_Shl ), appLang->a[ LNG_SHIFT_LEFT   ].a );
    IupSetAttribute ( guiData, MSTR ( guiData_Shr ), appLang->a[ LNG_SHIFT_RIGHT  ].a );
    IupSetAttribute ( guiData, MSTR ( guiData_Rem ), appLang->a[ LNG_REMOVE       ].a );
  }
  else
  {
    IupSetAttribute ( guiData, MSTR ( guiData_Add ), NULL );
  }
#ifndef EMULATOR
  IupSetAttribute ( guiMenu, MSTR ( guiMenu_Org ), appLang->a[ LNG_ORGANISATION ].a );
  IupSetAttribute ( guiMenu, MSTR ( guiMenu_Pfm ), appLang->a[ LNG_PLATFORM     ].a );
  IupSetAttribute ( guiMenu, MSTR ( guiMenu_Tar ), appLang->a[ LNG_TARGET       ].a );
#endif
  IupSetAttribute ( guiMenu, MSTR ( guiMenu_Pro ), appLang->a[ LNG_PROFILE      ].a );
  IupSetAttribute ( guiMenu, MSTR ( guiMenu_Qry ), appLang->a[ LNG_SEARCH       ].a );
  IupSetAttribute ( guiMenu, MSTR ( guiMenu_Res ), appLang->a[ LNG_RESULTS      ].a );
  IupSetAttribute ( guiMenu, MSTR ( guiMenu_Med ), appLang->a[ LNG_EDITOR       ].a );
  IupSetAttribute ( guiMenu, MSTR ( guiMenu_Hacks ), appLang->a[ LNG_HACKS        ].a );
  IupSetAttribute ( guiMenu, MSTR ( guiMenu_Hack ), appLang->a[ LNG_HACK         ].a );
  IupSetAttribute ( guiMenu, MSTR ( guiMenu_Codes ), appLang->a[ LNG_CODES        ].a );
  IupSetAttribute ( guiMenu, MSTR ( guiMenu_Code ), appLang->a[ LNG_CODE         ].a );
  IupSetAttribute ( guiMenu, MSTR ( guiMenu_About ), appLang->a[ LNG_ABOUT        ].a );
}
extern void guiHacks_OnInit ( void );
void guiMenu_OnInit ( void )
{
  guiHacks_OnInit();
  guiMenu = IupHList ( NULL );
  guiData = IupHList ( NULL );
  guiActionVB = IupHbox ( guiMenu, guiData, NULL );
  IupInsert ( guiDlg.vb, guiOrg.main.fset, guiActionVB );
  IupMap ( guiActionVB );
  IupSetAttribute ( guiActionVB, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute ( guiMenu, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute ( guiData, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetCallback ( guiMenu, "VALUECHANGED_CB", guiMenu_OnValueChanged );
  IupSetCallback ( guiData, "VALUECHANGED_CB", guiData_OnValueChanged );
  guiMenu_OnLang();
  IupSetInt ( guiMenu, IUP_VALUE, 1 );
  IupSetInt ( guiData, IUP_VALUE, 0 );
}

int  guiMenu_OnValueChanged ( Ihandle *ih )
{
  Ihandle *kid = IupGetChild ( ih, 0 ), *fset[] =
  {
#ifndef EMULATOR
    guiOrg.main.fset,
    guiPfm.main.fset,
    guiTar.main.fset,
#endif
    guiPro.main.fset,
    guiQry.main.fset,
    guiRes.main.fset,
    NULL,
    guiHacks.main.fset,
    guiHack.main.fset,
    guiCodes->main.fset,
    guiCode->main.fset,
    NULL
  };
  Icallback cb[] =
  {
#ifndef EMULATOR
    guiOrg_OnShow,
    guiPfm_OnShow,
    guiTar_OnShow,
#endif
    guiPro_OnShow,
    guiQry_OnShow,
    guiRes_OnShow,
    NULL,
    guiHacks_OnShow,
    guiHack_OnShow,
    guiCodes_OnShow,
    guiCode_OnShow,
    NULL
  };
  char  *title = IupGetAttribute ( kid, IUP_TITLE );
  int i = 1, c = IupGetChildCount ( guiDlg.vb );
  // Hide all groups first
  for ( ; i < c; ++i )
  {
    kid = IupGetChild ( guiDlg.vb, i );
    IupSetAttribute ( kid, "FLOATING", IUP_YES );
    IupHide ( kid );
  }
  for ( i = 0, c = IupGetChildCount ( guiOrg.main.vb ); i < c; ++i )
  {
    kid = IupGetChild ( guiOrg.main.vb, i );
    IupSetAttribute ( kid, "FLOATING", IUP_YES );
    IupHide ( kid );
  }
  // Find and show appropriate group
  i = IupGetInt ( ih, IUP_VALUE ) - 1;
  if ( i >= 0 && fset[i] && cb[i] )
  {
    IupShow ( fset[i] );
    cb[i] ( fset[i] );
  }
  guiMenu_OnLang();
  // Force Update GUI
  IupRefresh ( guiDlg.vb );
  IupRedraw ( guiDlg.vb, 1 );
  IupFlush();
  return IUP_DEFAULT;
}
void guiOpen ( uchar saveFile )
{
  int fd, op = _O_BINARY | _O_RDWR | (saveFile ? _O_CREAT : 0);
  FILE *file;
  char path[ PATH_MAX ] = {0};
  char opath[ PATH_MAX ] = {0};
  void *buff = NULL;
  if ( !appMethods.OnDefPath || !appMethods.OnDefExt )
  {
    return;
  }
  strcpy_s ( path, PATH_MAX, ipGetUsrDir() );
  strcat_s ( path, PATH_MAX, DIR_SEP ".medit" DIR_SEP "data" );
  strcpy_s ( opath, PATH_MAX, path );
  appMethods.OnDefPath ( opath );
  if ( saveFile )
  {
    appMethods.OnApply();
  }
  appMethods.OnDefPath ( path );
  if ( IupGetInt ( guiMenu, IUP_VALUE ) >= guiMenu_Hacks )
  {
    op = _O_TEXT | _O_RDWR | (saveFile ? _O_CREAT : 0);
    strcat_s ( path, PATH_MAX, "*." );
  }
  else if ( saveFile )
  {
    strcat_s ( path, PATH_MAX, "." );
  }
  appMethods.OnDefExt ( path );
  appMethods.OnDefExt ( opath );
  if ( _access( opath, 0 ) == 0 && strcmpi( opath, path ) != 0 )
    rename( opath, path );
  if ( IupGetInt ( guiMenu, IUP_VALUE ) >= guiMenu_Hacks )
  {
    IupGetFile ( path );
  }
  ipFdOpen ( &fd, path, op, IP_D_RW, IP_A_RW );
  if ( !fd )
    return;
  file = ipFdOpenFile ( fd, "rw+" );
  if ( !file )
  {
    return;
  }
  if ( saveFile )
  {
    appMethods.OnSave ( fd, file );
  }
  else
  {
    appMethods.OnLoad ( fd, file );
    appMethods.OnReset();
  }
  ipShut ( file );
  ipFdShut ( fd );
}
int  guiData_OnValueChanged ( Ihandle *ih )
{
  switch ( IupGetInt ( ih, IUP_VALUE ) )
  {
  // Load
  case guiData_Load:
    guiOpen ( 0 );
    break;
  case guiData_Reset:
    appMethods.OnReset();
    break;
  // Save
  case guiData_Save:
    guiOpen ( 1 );
    break;
  case guiData_Apply:
    appMethods.OnApply();
    break;
  // Modify List
  case guiData_Add:
    appMethods.OnAdd ( 0 );
    break;
  case guiData_Ins:
    appMethods.OnAdd ( 1 );
    break;
  case guiData_Shu:
    appMethods.OnMov ( 1, 1 );
    break;
  case guiData_ShuNB:
    appMethods.OnMov ( 0, 1 );
    break;
  case guiData_Shd:
    appMethods.OnMov ( 1, -1 );
    break;
  case guiData_ShdNB:
    appMethods.OnMov ( 0, -1 );
    break;
  case guiData_Shl:
    appMethods.OnMov ( -1, 0 );
    break;
  case guiData_Shr:
    appMethods.OnMov ( 1, 0 );
    break;
  case guiData_Rem:
    appMethods.OnRem();
    break;
  default:
    return IUP_DEFAULT;
  }
  IupSetInt ( ih, IUP_VALUE, 0 );
  return IUP_DEFAULT;
}
