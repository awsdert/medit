#include "../guiMain.h"
void guiCodes_OnInit( GUI_CODES* codes )
{
  GUI *gui = appGetGui();
  codes->treeCodes = IupTree();
  codes->listPos   = meMkList( NULL,
    "|<", "<<", ">>", ">|", "+", "-", "<>", ":)", ":(", "/\\", "\\/", NULL );
  codes->codes.vb   = IupVbox(  codes->treeCodes, codes->listPos, NULL );
  codes->codes.fset = IupFrame( codes->codes.vb );
  IupAppend( codes->main.vb, codes->codes.fset );
  IupMap( codes->codes.fset );
}
