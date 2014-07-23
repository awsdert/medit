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
long hack( long hid, long cid, long hend, long cend )
{
  HACK *hack = &hacks.buff[hid];
  CODE *code = &codes.buff[cid];
  if ( cend < 0 )
  {
    memset( hname, 0, 9 );
    _itoa_s( hid, hname, 9, 16 );
    memset( cpath, 0, FILENAME_MAX );

    cend = codes.size;
  }
  else if ( !cend )
    cend = codes.size;
  return hid;
}
int main( int argc, char *argv[] )
{
  char
    /** \brief Target Information File **/
    *tinfo = argv[1],
    /** \brief Target Path
    *tpath = argv[2],
    /** \brief Hacklist Path **/
    *hpath = argv[3],
    /** \brief Codelist Path **/
    cpath[ FILENAME_MAX ] = {0},
    hname[ 9 ] = {0};
  Ipipe
    tpipe = {0},
    hpipe = {0},
    cpipe = {0};
  HACKS hacks = {0};
  CODES codes = {0};
  //hack( &tpipe, &hacks, &codes, &cpipe, cpath, 0, 0, 0, -1 );
  return EXIT_SUCCESS;
}
