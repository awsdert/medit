#include "../guiMain.h"
int guiFile_OnKAny( Ihandle *ih, int c )
{
  // Remove modifiers as we don't care about that
  c ^= 0xF0000000;
  if ( c < ' ' )
    return IUP_DEFAULT;
  else if ( c < '0' || strlen( appGetCfg()->dst.file ) == NAME_MAX )
    return IUP_CLOSE;
  else if ( c < '9' )
    return IUP_DEFAULT;
  else if ( c < 'A' )
    return IUP_CLOSE;
  else if ( c < 'Z' )
    return IUP_DEFAULT;
  else if ( c < 'a' )
    return IUP_CLOSE;
  else if ( c < 'z' )
    return IUP_DEFAULT;
  return IUP_CLOSE;
}
int guiFile_OnValueChanged( Ihandle *ih )
{
  strcpy_s( appGetCfg()->dst.file, NAME_MAX, IupGetAttribute( ih, IUP_VALUE ) );
  return IUP_DEFAULT;
}
