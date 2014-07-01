#include "lang.h"

static LANG _lang =
{{
  // Main list
  "Target",
  "Profile",
  "Search",
  "Results",
  "Editor",
  "Hacks",
  "About",
  // Search Menu
  "Dump",
  "Undo",
  "Redo",
  // Other
  "Name",
  "Type",
  "ID",
  "Memory",
  "Range",
  "Code",
  "Comparison",
  "Value",
  "Always",
  "True",
  "False"
}};

LANG* meGetLang( void ) { return &_lang; }
char const* meGetText( LANG_T text ) { return _lang.x[ text ]; }
