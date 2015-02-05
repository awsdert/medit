#include "../guiMain.h"
char *srcFile;
char *tmpFile;
int guiFile_OnKAny( Ihandle *ih, int c )
{
  // Remove modifiers as we don't care about that
  c ^= 0xF0000000;
  srcFile = IupGetAttribute( ih, "MEDIT_SRC_FILE" );
  tmpFile = IupGetAttribute( ih, "MEDIT_TMP_FILE" );

  if ( c < ' ' )
  {
    return IUP_DEFAULT;
  }
  else if ( c < '0' || strlen( tmpFile ) == NAME_MAX )
  {
    return IUP_CLOSE;
  }
  else if ( c < '9' )
  {
    return IUP_DEFAULT;
  }
  else if ( c < 'A' )
  {
    return IUP_CLOSE;
  }
  else if ( c < 'Z' )
  {
    return IUP_DEFAULT;
  }
  else if ( c < 'a' )
  {
    return IUP_CLOSE;
  }
  else if ( c < 'z' )
  {
    return IUP_DEFAULT;
  }

  return IUP_CLOSE;
}
int guiFile_OnValueChanged( Ihandle *ih )
{
  srcFile = IupGetAttribute( ih, "MEDIT_SRC_FILE" );
  tmpFile = IupGetAttribute( ih, "MEDIT_TMP_FILE" );
  strncpyi( tmpFile, IupGetAttribute( ih, IUP_VALUE ), NAME_MAX );
  return IUP_DEFAULT;
}
