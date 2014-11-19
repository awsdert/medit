#include "../guiMain.h"
ushort _RdTxtHacks(
  HACK_FUNC *hfunc,
  HACKL        *hl,
  CODE_FUNC *cfunc,
  CODES     *codes,
  Ipipe      *file,
  CODELIST     *cl,
  long        cgid,
  ushort       hid,
  uchar        pid,
  ushort    cdepth,
  long        mode,
  ushort         i )
{
  DWORD    len = 0;
  long     cid = 0, gid = -1, j = 0, k = 0;
  ushort depth = cdepth;
  HACKS *hacks = &hl->hacks;
  HACK   *hack = &hacks->a[hid];
  char   *name = hl->names[hid];
  if ( mode == 0 )
    len = ipRdLine( file, cl->x[i], cl->cols );
  else
    len = strlen( cl->x[i] );
  for
  (
    ; len; memset( cl->x[i], 0, cl->cols ),
    len = ipRdLine( file, cl->x[i], cl->cols )
  )
  {
    if ( cl->x[i][0] == '"' )
      mode = ( mode == 1 || mode == 2 ) ? 2 : 0;
    else if ( cl->x[i][0] == '\n' || cl->x[i][0] == '\r' )
    {
      mode = ( mode == 1 ) ? 2 : 0;
      continue;
    }
    /* Convert codelist into codes */
    if ( mode == 2 )
    {
      cl->rows = i;
      for ( i = 0; i < cl->rows;
        i = cfunc->txt2raw( &codes->a[cid], cl, i ), ++cid );
      cl->rows = 30;
      if ( hid == 0 )
      {
        _RdTxtHacks( hfunc, hl, cfunc, codes, file, cl, gid, 1, 0, 0, 2, i );
        break;
      }
      else if ( hack->_pi != pid || depth == cdepth )
        ++hid;
      else if ( depth < cdepth )
      {
        if ( depth > 0 )
          hack->_pi = -depth;
        else if ( cdepth > 0 )
          hack->_pi = -(cdepth - 1);
        else
          hack->_pi = 0;
        break;
      }
      else
      {
        hack->_pi = hid - 1;
        hid = _RdTxtHacks( hfunc, hl, cfunc, codes, file, cl, cgid, hid + 1, hid - 1, depth, 2, i );
        mode = 3;
      }
      /* Finish reading hack name/header */
      hack = &hacks->a[ hid ];
      if ( mode == 3 )
      {
        if ( hack->_pi < 0 )
        {
          if ( -hack->_pi != cdepth )
            break;
          hack->_pi = pid;
        }
        ++hid;
        hack = &hacks->a[ hid ];
      }
      mode = 0;
    }
    /* Retrieve hack information */
    if ( mode == 0 )
    {
      /*
        gid is only for converting back to txt,
        also for weeding out invalid hacks.
        Example for gid not supported:
          return -1 for valid hack,
          return -2 for invalid hack
        Example for gid being supported
          Hacks contain their own gid which is compared against (M) gid
        0 and up are treated as valid IDs by converters so -1 is a good
        way to ensure the user is forced to enter a gid before converters
        requiring one can work
      */
      hack->_pi = pid;
      gid = hfunc->txt2raw( hack, hl, file );
      if ( gid != cgid && hid > 0 )
      {
        mode = 3;
        continue;
      }
      j = 0;
      k = (cl->x[i][0] == '"') ? 1 : 0;
      depth = 0;
      do
      {
        if ( cl->x[i][k] == '"' )
          cl->x[i][k] = 0;
        else if ( cl->x[i][k] == '\\' )
          ++depth;
        else
        {
          name[j] = cl->x[i][k];
          ++j; ++k;
        }
      }
      while ( k < cl->cols && cl->x[i][k] );
      name[ NAME_LAST ] = 0;
      //IupMessage( "Hack Load", hacks->buff[i].name );
      mode = 1;
      i = 0;
    }
    /* Retrieve codelist */
    else if ( mode == 1 )
    {
      ++i;
      if ( i == cl->rows )
        break;
    }
    else continue;
  }
  return hid;
}
void RdTxtHacks(
  HACK_FUNC *hfunc,
  HACKL        *hl,
  CODE_FUNC *cfunc,
  CODES     *codes,
  Ipipe      *file )
{
  CODELIST cl = {0};
  cl.rows = 30;
  cl.cols = 50;
  _RdTxtHacks( hfunc, hl, cfunc, codes, file, &cl, -1, 0, 0, 0, 0, 0 );
}
