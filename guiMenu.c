#include "guiMain.h"
Ihandle *guiActionVB = NULL;
Ihandle *guiMenu = NULL;
Ihandle *guiData = NULL;
extern CODES srcHackCodes;
extern CODES tmpHackCodes;
extern CODES resCodes;
int  guiData_OnValueChanged( Ihandle *ih );
int  guiMenu_OnValueChanged( Ihandle *ih );
#define guiData_Load  1
#define guiData_Apply 2
#define guiData_Reset 3
#define guiData_Save  4
#define guiData_Add   5
#define guiData_Ins   6
#define guiData_Shl   7
#define guiData_Shr   8
#define guiData_Rem   9
#define guiData_Count guiData_Save
#define guiList_Count guiData_Rem
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
#define guiMenu_Count guiMenu_About
void guiMenu_OnLang( void )
{
  GUI_CODES *codes = guiCodes;
  CODES *srcC = srcCodes, *tmpC = tmpCodes;
  int i = IupGetInt( guiMenu, IUP_VALUE );
  IupSetAttribute( guiData, MSTR( guiData_Load  ), appLang->a[ LNG_LOAD         ].a );
  IupSetAttribute( guiData, MSTR( guiData_Apply ), appLang->a[ LNG_APPLY        ].a );
  IupSetAttribute( guiData, MSTR( guiData_Reset ), appLang->a[ LNG_RESET        ].a );
  IupSetAttribute( guiData, MSTR( guiData_Save  ), appLang->a[ LNG_SAVE         ].a );
  if ( i == guiMenu_Hacks || i == guiMenu_Codes )
  {
    IupSetAttribute( guiData, MSTR( guiData_Add ), appLang->a[ LNG_ADD          ].a );
    IupSetAttribute( guiData, MSTR( guiData_Ins ), appLang->a[ LNG_INSERT       ].a );
    IupSetAttribute( guiData, MSTR( guiData_Shl ), appLang->a[ LNG_SHIFT_LEFT   ].a );
    IupSetAttribute( guiData, MSTR( guiData_Shr ), appLang->a[ LNG_SHIFT_RIGHT  ].a );
    IupSetAttribute( guiData, MSTR( guiData_Rem ), appLang->a[ LNG_REMOVE       ].a );
  }
  else
    IupSetAttribute( guiData, MSTR( guiData_Add ), NULL );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Org   ), appLang->a[ LNG_ORGANISATION ].a );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Pfm   ), appLang->a[ LNG_PLATFORM     ].a );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Tar   ), appLang->a[ LNG_TARGET       ].a );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Pro   ), appLang->a[ LNG_PROFILE      ].a );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Qry   ), appLang->a[ LNG_SEARCH       ].a );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Res   ), appLang->a[ LNG_RESULTS      ].a );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Med   ), appLang->a[ LNG_EDITOR       ].a );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Hacks ), appLang->a[ LNG_HACKS        ].a );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Hack  ), appLang->a[ LNG_HACK         ].a );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Codes ), appLang->a[ LNG_CODES        ].a );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Code  ), appLang->a[ LNG_CODE         ].a );
  IupSetAttribute( guiMenu, MSTR( guiMenu_About ), appLang->a[ LNG_ABOUT        ].a );
#ifndef EMULATOR
  guiOrg_OnLang();
  guiPfm_OnLang();
  guiTar_OnLang();
#endif
  guiPro_OnLang();
  guiHacks_OnLang();
  guiHack_OnLang();
  guiCodes_SetGUI( &guiHack.codes, &srcHackCodes, &tmpHackCodes );
  guiCodes_OnLang();
  guiCode_OnLang();
  guiCodes_SetGUI( &guiRes.codes, &resCodes, &resCodes );
  guiCodes_OnLang();
  guiCode_OnLang();
  guiCodes_SetGUI( codes, srcC, tmpC );
}
extern void guiHacks_OnInit( void );
void guiMenu_OnInit( void )
{
  guiHacks_OnInit();
  guiMenu = IupHList( NULL );
  guiData = IupHList( NULL );
  guiActionVB = IupHbox( guiMenu, guiData, NULL );
  IupInsert( guiDlg.vb, guiOrg.main.fset, guiActionVB );
  IupMap( guiActionVB );
  IupSetAttribute( guiActionVB, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute( guiMenu, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetAttribute( guiData, IUP_EXPAND, IUP_HORIZONTAL );
  IupSetCallback( guiMenu, "VALUECHANGED_CB", guiMenu_OnValueChanged );
  IupSetCallback( guiData, "VALUECHANGED_CB", guiData_OnValueChanged );
  guiMenu_OnLang();
  IupSetInt( guiMenu, IUP_VALUE, 1 );
  IupSetInt( guiData, IUP_VALUE, 0 );
}

int  guiMenu_OnValueChanged( Ihandle *ih )
{
  Ihandle *kid = IupGetChild( ih, 0 ), *fset[] = {
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
  Icallback cb[] = {
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
  char  *title = IupGetAttribute( kid, IUP_TITLE );
  int i = 1, c = IupGetChildCount( guiDlg.vb );
  // Hide all groups first
  for ( ; i < c; ++i )
  {
    kid = IupGetChild( guiDlg.vb, i );
    IupSetAttribute( kid, "FLOATING", IUP_YES );
    IupHide( kid );
  }
  for ( i = 0, c = IupGetChildCount( guiOrg.main.vb ); i < c; ++i )
  {
    kid = IupGetChild( guiOrg.main.vb, i );
    IupSetAttribute( kid, "FLOATING", IUP_YES );
    IupHide( kid );
  }
  // Find and show appropriate group
  i = IupGetInt( ih, IUP_VALUE ) - 1;
  if ( i >= 0 && fset[i] && cb[i] )
  {
    IupShow( fset[i] );
    cb[i]( fset[i] );
  }
  guiMenu_OnLang();
  // Force Update GUI
  IupRefresh( guiDlg.vb );
  IupRedraw( guiDlg.vb, 1 );
  IupFlush();
  return IUP_DEFAULT;
}
void guiOpen( uchar saveFile )
{
  int fd, op = _O_BINARY | _O_RDWR;
  FILE *file;
  char path[ PATH_MAX ] = {0};
  void *buff = NULL;
  strcpy_s( path, PATH_MAX, ipGetUsrDirA() );
  strcat_s( path, PATH_MAX, ".medit" DIR_SEP "data" DIR_SEP );
  if ( !appMethods.OnDefPath || !appMethods.OnDefExt )
    return;
  appMethods.OnDefPath( path, saveFile );
  if ( IupGetInt( guiMenu, IUP_VALUE ) >= guiMenu_Hacks )
    op = _O_TEXT | _O_RDWR;
  if ( !saveFile || IupGetInt( guiMenu, IUP_VALUE ) >= guiMenu_Hacks )
    strcat_s( path, PATH_MAX, "*." );
  if ( saveFile )
    op |= _O_CREAT;
  appMethods.OnDefExt( path );
  if ( !saveFile || IupGetInt( guiMenu, IUP_VALUE ) >= guiMenu_Hacks )
    IupGetFile( path );
  ipFdOpenA( &fd, path, op, IP_D_RW, IP_A_RW );
  file = ipFdOpenFileA( fd, "rw+" );
  if ( !file )
    return;
  if ( saveFile )
  {
    appMethods.OnApply();
    appMethods.OnSave( fd, file );
  }
  else
  {
    appMethods.OnLoad( fd, file );
    appMethods.OnReset();
  }
  ipShut( file );
  ipFdShut( fd );
}
int  guiData_OnValueChanged( Ihandle *ih )
{
  switch ( IupGetInt( ih, IUP_VALUE ) )
  {
  // Load
  case guiData_Load: guiOpen( 0 ); break;
  case guiData_Reset: appMethods.OnReset(); break;
  // Save
  case guiData_Save: guiOpen( 1 ); break;
  case guiData_Apply: appMethods.OnApply(); break;
  // Modify List
  case guiData_Add: appMethods.OnAdd( 1 ); break;
  case guiData_Ins: appMethods.OnAdd( 0 ); break;
  case guiData_Rem: appMethods.OnRem(); break;
  default: return IUP_DEFAULT;
  }
  IupSetInt( ih, IUP_VALUE, 0 );
  return IUP_DEFAULT;
}
