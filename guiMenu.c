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
#define guiData_Count guiData_Save
#define guiMenu_Org   1
#define guiMenu_Pfm   2
#define guiMenu_Tar   3
#define guiMenu_Pfl   4
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
  IupSetAttribute( guiData, MSTR( guiData_Load  ), appLang->x[ LNG_LOAD         ] );
  IupSetAttribute( guiData, MSTR( guiData_Apply ), appLang->x[ LNG_APPLY        ] );
  IupSetAttribute( guiData, MSTR( guiData_Reset ), appLang->x[ LNG_RESET        ] );
  IupSetAttribute( guiData, MSTR( guiData_Save  ), appLang->x[ LNG_SAVE         ] );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Org   ), appLang->x[ LNG_ORGANISATION ] );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Pfm   ), appLang->x[ LNG_PLATFORM     ] );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Tar   ), appLang->x[ LNG_TARGET       ] );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Pfl   ), appLang->x[ LNG_PROFILE      ] );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Qry   ), appLang->x[ LNG_SEARCH       ] );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Res   ), appLang->x[ LNG_RESULTS      ] );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Med   ), appLang->x[ LNG_EDITOR       ] );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Hacks ), appLang->x[ LNG_HACKS        ] );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Hack  ), appLang->x[ LNG_HACK         ] );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Codes ), appLang->x[ LNG_CODES        ] );
  IupSetAttribute( guiMenu, MSTR( guiMenu_Code  ), appLang->x[ LNG_CODE         ] );
  IupSetAttribute( guiMenu, MSTR( guiMenu_About ), appLang->x[ LNG_ABOUT        ] );
#ifndef EMULATOR
  guiOrg_OnLang();
  guiPfm_OnLang();
  guiTar_OnLang();
#endif
  guiPfl_OnLang();
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
        guiPfl.main.fset,
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
    guiPfl_OnShow,
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
  // Force Update GUI
  IupRefresh( guiDlg.vb );
  IupRedraw( guiDlg.vb, 1 );
  IupFlush();
  return IUP_DEFAULT;
}
void guiOpen( uchar saveFile )
{

  Ipipe file = {0};
  char path[ PATH_MAX ] = {0};
  void *buff = NULL;
  ipGetLDataDirFromName( path, PATH_MAX );
  if ( !appMethods.OnDefPath || !appMethods.OnDefExt )
    return;
  appMethods.OnDefPath( path );
  appMethods.OnDefExt(  path );
  file = ipOpenFile( path, 0666, SHARE_READ );
  if ( !file.pipe )
    return;
  if ( saveFile )
  {
    appMethods.OnApply();
    appMethods.OnSave( &file );
  }
  else
  {
    appMethods.OnLoad( &file );
    appMethods.OnReset();
  }
}
int  guiData_OnValueChanged( Ihandle *ih )
{
  switch ( IupGetInt( ih, IUP_VALUE ) )
  {
  // Load
  case guiData_Load: guiOpen( 0 ); break;
  case guiData_Apply: appMethods.OnApply(); break;
  case guiData_Reset: appMethods.OnReset(); break;
  // Save
  case guiData_Save: guiOpen( 1 ); break;
  }
  return IUP_DEFAULT;
}
