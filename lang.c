#include "lang.h"

static MELANG fLang =
{
  // Main list
  {
    "Target",
    "Profile",
    "Search",
    "Results",
    "Editor",
    "Hacks",
    "About"
  },
  // Search Menu
  {
    "Dump",
    "Undo",
    "Redo"
  }
};

MELANG* meGetLang( void ) { return &fLang; }
