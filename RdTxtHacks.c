#include "guiMain.h"
long
_RdTxtHacks( HACK_FUNC *hfunc, HACKS *hacks, CODE_FUNC *cfunc, CODES *codes, Ipipe *file, CODELIST *cl, long hid, long pid, long cdepth, long  mode )
{
  DWORD len = 0;
  usv i = 0;
  long cid = 0, depth = 0, gid = 0, j = 0, k = 0;
  HACK *hack = &hacks->buff[hid];
  if ( !mode )
    len = ipRdLine( file, cl->x[i], cl->cols );
  for
  (
    ; len; memset( cl->x[i], 0, cl->cols ),
    len = ipRdLine( file, cl->x[i], cl->cols )
  )
  {
    if ( cl->x[i][0] == '"' )
      mode = ( mode == 1 ) ? 2 : 0;
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
        i = cfunc->txt2raw( &codes->buff[cid], cl, i ), ++cid );
      cl->rows = 30;
      if ( hid == 0 )
      {
        _RdTxtHacks( hfunc, hacks, cfunc, codes, file, cl, 1, 0, 0, 2 );
        break;
      }
      else if ( depth == cdepth )
      {
        ++hid;
        hack->pid = pid;
      }
      else if ( depth < cdepth )
      {
        if ( depth >= 0 )
          hack->pid = -depth;
        break;
      }
      else
      {
        hack->pid = hid - 1;
        hid = _RdTxtHacks( hfunc, hacks, cfunc, codes, file, cl, hid + 1, hid - 1, depth, 2 );
      }
      /* Finish reading hack name/header */
      hack = &hacks->buff[ hid ];
      if ( hack->pid < 0 )
      {
        if ( -hack->pid != cdepth )
          break;
        hack->pid = pid;
      }
      mode = 0;
    }
    /* Retrieve hack information */
    if ( !mode )
    {
      gid = hfunc->txt2raw( hack, file );
      if ( hack->pid < 0 )
        break;
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
          hack->name[j] = cl->x[i][k];
          ++j; ++k;
        }
      }
      while ( k < cl->cols && cl->x[i][k] );
      hack->name[ NAME_LAST ] = 0;
      IupMessage( "Hack Load", hacks->buff[i].name );
      mode = 1;
      i = 0;
    }
    /* Retrieve codelist */
    else
    {
      ++i;
      if ( i == cl->rows )
        break;
    }
  }
  return hid;
}
void
RdTxtHacks( HACK_FUNC *hfunc, HACKS *hacks, CODE_FUNC *cfunc, CODES *codes, Ipipe *file )
{
  CODELIST cl = {0};
  cl.rows = 30;
  cl.cols = 50;
  _RdTxtHacks( hfunc, hacks, cfunc, codes, file, &cl, 0, 0, 0, 0 );
}
