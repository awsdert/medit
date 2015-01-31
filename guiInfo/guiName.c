#include "../guiMain.h"
char *srcName;
char *tmpName;
int guiName_OnKAny( Ihandle *ih, int c )
{
  // Remove modifiers as we don't care about that
  c ^= 0xF0000000;
  srcName = IupGetAttribute( ih, "MEDIT_SRC_NAME" );
  tmpName = IupGetAttribute( ih, "MEDIT_TMP_NAME" );
  if ( c < ' ' )
    return IUP_DEFAULT;
  else if
  (
    c == '=' || c == '"' || c == '\'' ||
    strlen( tmpName ) == NAME_MAX
  )
    return IUP_CLOSE;
  return IUP_DEFAULT;
}
int guiName_OnValueChanged( Ihandle *ih )
{
  /* We don't have to worry about space since the OnKAny handler blocks going
  further than NAME_MAX but no harm in being careful */
  srcName = IupGetAttribute( ih, "MEDIT_SRC_NAME" );
  tmpName = IupGetAttribute( ih, "MEDIT_TMP_NAME" );
  strncpyi( tmpName, IupGetAttribute( ih, IUP_VALUE ), NAME_MAX );
  return IUP_DEFAULT;
}
