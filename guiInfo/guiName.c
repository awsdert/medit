#include "../guiMain.h"
int guiName_OnKAny( Ihandle *ih, int c )
{
  // Remove modifiers as we don't care about that
  c ^= 0xF0000000;
  if ( c < ' ' )
    return IUP_DEFAULT;
  else if
  (
    c == '=' || c == '"' || c == '\'' ||
    strlen( appGetCfg()->dst.name ) == NAME_MAX
  )
    return IUP_CLOSE;
  return IUP_DEFAULT;
}
int guiName_OnValueChanged( Ihandle *ih )
{
  /* We don't have to worry about space since the OnKAny handler blocks going
  further than NAME_MAX but no harm in being careful */
  strcpy_s( appGetCfg()->dst.name, NAME_MAX, IupGetAttribute( ih, IUP_VALUE ) );
  return IUP_DEFAULT;
}
