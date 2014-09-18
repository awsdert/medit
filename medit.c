#include "_hack.h"
/** \brief Recursive Function for hacking
  \param hbeg Begin at Hack ID
  \param cbeg Begin at Code ID
  \param hend Stop at Hack ID
  \param cend Stop at Code ID
**/
HACKS hacks = {0};
char  hname[9] = {0};
CODES codes = {0};
Ipipe cpipe = {0};
char  cpath[FILENAME_MAX] = {0};
void mset( void* ptr, ucv fillWith, uzv size )
{
  ucv *buff = (ucv*)ptr;
  uzv i = 0;
  do
  {
    buff[i] = fillWith;
    ++i;
  }
  while ( i < size );
}
long hack( long hid, long cid, long hend, long cend )
{
  HACK *hack = &hacks.buff[hid];
  CODE *code = &codes.buff[cid];
  if ( cend < 0 )
  {
    mset( hname, 0, 9 );
    _itoa_s( hid, hname, 9, 16 );
    mset( cpath, 0, FILENAME_MAX );
    cend = codes.size;
  }
  else if ( !cend )
    cend = codes.size;
  return hid;
}
char
  /** \brief Target Information File **/
  *tinfo = NULL,
  /** \brief Target Path **/
  *tpath = NULL,
  /** \brief Hacklist Path **/
  *hpath = NULL,
  /** \brief Codelist Path **/
  cpath[ FILENAME_MAX ] = {0},
  hname[ 9 ] = {0};
DWORD main( void )
{
  Ipipe
    tpipe = {0},
    hpipe = {0},
    cpipe = {0};
  HACKS hacks = {0};
  CODES codes = {0};
  if ( !cpath[0] )
  {
    // We must insert ourself first
    return EXIT_FAILURE;
  }
  ipInitPiping();
  tpipe = GetProcessHandle();
  //hack( &tpipe, &hacks, &codes, &cpipe, cpath, 0, 0, 0, -1 );
  return EXIT_SUCCESS;
}
