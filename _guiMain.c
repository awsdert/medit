#include "_guiMain.h"
ucv getRamNo( char *name ) { return 0; }
HMODULE meLoadLib( char* name, HACK_FUNC **hfunc, CODE_FUNC **cfunc )
{
  HMODULE lib = NULL;
  char szFile[ FILENAME_MAX ] = {0};
  GETHACKFUNCS gethacks = NULL;
  GETCODEFUNCS getcodes = NULL;
  strcpy_s( szFile, FILENAME_MAX, IupGetAttribute( NULL, "APP_CWD" ) );
  strcat_s( szFile, FILENAME_MAX, name );
  strcat_s( szFile, FILENAME_MAX, "-" );
  strcat_s( szFile, FILENAME_MAX, IupGetAttribute( NULL, "APP_VER" ) );
  strcat_s( szFile, FILENAME_MAX, ".dll" );
  lib = LoadLibrary( szFile );
  if ( !lib )
  {
    IupMessage( "Failure to load Library", name );
    return NULL;
  }
  gethacks = (GETHACKFUNCS)GetProcAddress( lib, "GetHackFuncs");
  getcodes = (GETCODEFUNCS)GetProcAddress( lib, "GetCodeFuncs");
  if ( !gethacks )
  {
    IupMessage( "Failure to load hack functions", name );
    FreeLibrary( lib );
    return NULL;
  }
  if ( !getcodes )
  {
    IupMessage( "Failure to load code functions", name );
    FreeLibrary( lib );
    return NULL;
  }
  (*hfunc) = gethacks();
  (*cfunc) = getcodes();
  (*cfunc)->getRamNo = getRamNo;
  return lib;
}
HMODULE meFreeLib( HMODULE lib )
{
  FreeLibrary( lib );
  return NULL;
}
Ihandle* meMkList( Icallback func, ... )
{
  Ihandle *hbox = IupHbox( NULL ),
    *sbox = IupScrollBox( hbox ),
    *frame = NULL, *ih = NULL;
  va_list args;
  char *title = NULL;
  va_start( args, func );
  title = va_arg( args, char* );
  do
  {
    ih = IupLabel( title );
    IupSetAttribute( ih, IUP_EXPAND, IUP_YES );
    IupSetAttribute( ih, IUP_ALIGNMENT, "ACENTER:ACENTER" );
    frame = IupFrame( NULL );
    IupAppend( frame, ih );
    IupSetAttribute( frame, IUP_ACTIVE, IUP_NO );
    IupSetAttribute( frame, IUP_BORDER, IUP_YES );
    IupSetAttribute( frame, IUP_BGCOLOR, IupGetAttribute( NULL, "DLGBGCOLOR" ));
    IupSetAttribute( frame, IUP_FGCOLOR, IupGetAttribute( NULL, "DLGFGCOLOR" ));
    IupSetAttribute( ih, IUP_BGCOLOR, IupGetAttribute( NULL, "DLGBGCOLOR" ));
    IupSetAttribute( ih, IUP_FGCOLOR, IupGetAttribute( NULL, "DLGFGCOLOR" ));
    IupSetAttribute( frame, IUP_SCROLLBAR, IUP_NO );
    IupSetAttribute( frame, IUP_SIZE, "40x" );
    IupAppend( hbox, frame );
    IupSetCallback( frame, IUP_BUTTON_CB, NULL );
    IupSetCallback( frame, "_ME_BUTTON_CB", func );
    title = va_arg( args, char* );
  }
  while ( title );
  va_end(   args );
  IupSetCallback( sbox, IUP_BUTTON_CB, (Icallback)meListP_ButtonCB );
  IupSetCallback( sbox, IUP_MOTION_CB, (Icallback)meListP_MotionCB );
  IupSetAttribute( sbox, IUP_SIZE, "100x25" );
  IupSetAttribute( hbox, IUP_GAP, "10" );
  IupSetAttribute( hbox, IUP_EXPAND, IUP_YES );
  IupSetAttribute( sbox, IUP_EXPAND, IUP_HORIZONTAL );
  return sbox;
}

int meListP_ButtonCB( Ihandle *sbox, int button, int pressed, int x, int y, char* status )
{
  Ihandle *hbox = IupGetChild( sbox, 0 ), *zero = IupGetChild( hbox, 0 ),
    *ih = NULL, *frame = NULL, *lbl;
  char attr[20] = {0}, *tok = NULL, *tmp = NULL;
  int w = 0, h = 0, px = IupGetInt( sbox, IUP_POSX ), gap = 0, kid = 0;
  Icallback func = NULL;
  int i = 0, c = IupGetChildCount( hbox );
  div_t res = {0};
  typedef int (*motionCB)( Ihandle*, int, int, int, int, char* );
  if ( pressed )
  {
    if ( iup_isbutton1( status ) )
    {
      IupSetInt( sbox, "_IUP_START_X", x );
      IupSetInt( sbox, "_IUP_START_Y", y );
      IupSetAttribute(sbox, "_IUP_DRAG_SB", "1" );
    }
  }
  else if ( iup_isbutton1( status ) )
  {
    px += x;
    gap = IupGetInt( hbox, IUP_GAP );
    do
    {
      ih = IupGetChild( hbox, i );
      strcpy_s( attr, 20, IupGetAttribute( ih, IUP_RASTERSIZE ) );
      tmp = strtok_s( attr, "x", &tok );
      w += atoi( tmp );
      tmp = strtok_s( NULL, "x", &tok );
      h += atoi( tmp );
      if ( px < w )
        break;
      tok = NULL;
      w += gap;
      if ( px < w )
      {
        ih = NULL;
        break;
      }
      ++i;
    }
    while ( i < c );
    if ( ih )
    {
      func = IupGetCallback( ih, "_ME_BUTTON_CB" );
      IupSetAttribute(sbox, "_IUP_DRAG_SB", NULL);
      for ( i = 0; i < c; ++i )
      {
        frame = IupGetChild( hbox, i );
        lbl   = IupGetChild( frame, 0 );
        IupSetAttribute( frame, "SUNKEN", IUP_NO );
        IupRedraw( frame, 0 );
      }
      lbl   = IupGetChild( ih, 0 );
      IupSetAttribute( ih, "SUNKEN", IUP_YES );
      if ( func )
        ((motionCB)func)( ih, button, pressed, x, y, status );
      IupRedraw( ih, 0 );
      IupFlush();
    }
  }
  return IUP_DEFAULT;
}
int meListP_MotionCB( Ihandle *sbox, int x, int y, char* status )
{
  Ihandle *hbox = IupGetChild( sbox, 0 );
  int sx, sy, dx, dy, px, py;
  if (iup_isbutton1(status) &&
      IupGetAttribute(sbox, "_IUP_DRAG_SB"))
  {
    sx = IupGetInt(sbox, "_IUP_START_X");
    sy = IupGetInt(sbox, "_IUP_START_Y");
    dx = x - sx;
    dy = y - sy;
    px = IupGetInt(sbox, "_IUP_START_POSX") - dx;
    py = IupGetInt(sbox, "_IUP_START_POSY") - dy;
    IupSetInt(sbox, IUP_POSX, px);  /* drag direction is opposite to scrollbar */
    IupSetInt(sbox, IUP_POSY, py);
    IupSetInt( hbox, IUP_X, -px );
    IupSetInt( hbox, IUP_Y, -py );
  }
  IupRefresh( sbox );
  IupFlush();
  return IUP_DEFAULT;
}
int meMenu_ButtonCB( Ihandle *ih, int button, int pressed, int x, int y, char* status )
{
  Ihandle *hLbl = IupGetChild( ih, 0 );
  char   *title = IupGetAttribute( hLbl, IUP_TITLE );
  MEGUI   *gui = meGetGui();
  int i = 1, c = IupGetChildCount( gui->main.main_vbox );
  for ( ; i < c; ++i )
    IupHide( IupGetChild( gui->main.main_vbox, i ) );
  if ( strcmp( meGetText( LANG_SEARCH ), title ) == 0 )
  {
    IupShow( gui->sea.main_fset );
    IupShow( gui->cmp.main_fset );
    IupShow( gui->val.main_fset );
  }
  IupRedraw( gui->main.main_vbox, 1 );
  IupFlush();
  return IUP_DEFAULT;
}
void execHacksId( DWORD pid )
{
  DWORD buf[3] = {0, 0, pid};
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
