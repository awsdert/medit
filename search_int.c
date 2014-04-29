#include "search.h"

void qSI(
        ucn    dump,
        ucn    used,
        mePIPE tar,
        mePIPE dmp,
        mePIPE res,
        Ipipe  previous,
        DWORD  size,
        meCOMP mComp,
        meCOMP nComp,
        ucn    bytes,
        ucn    isFloat )
{
  DWORD
    c  = 0,
    cc = 6,
    i  = 0;
  ptrdiff_t
    rNo   = 0,
    addr  = 0,
    addrc = -1;
  ucn
    rBl   = 0;
  shn
    tNum = 0,
    dNum = 0,
    aNum = 0,
    mNum = 0,
    nNum = 0;
  memset( res.buff, 1, size );
  do
  {
    IupRdPipe( tar.pipe, tar.buff, size );
    if ( dump )
      goto write;
    else
    {
      i = 0;
      do
      {
        dmp.buff[i] = 0;
        res.buff[i] = 0;
        ++i;
      }
      while ( i < size );
      IupRdPipe( previous, res.buff, size );
      IupRdPipe( dmp.pipe, dmp.buff, size );
    }
    i = 0;
    do
    {
      rBl = res.buff[i];
      if ( !rBl )
        goto iterate;
      // Set tNum & dNum ready for comparisons
      #ifdef _HUGE_DEFINED
      if ( bytes == sizeof( huge ) )
      {
        CAST_DMP( shn );
      }
      #endif
      else
      if ( bytes == sizeof( long ) )
      {
        CAST_DMP( sln );
      }
      else if ( bytes == sizeof( short ) )
      {
        CAST_DMP( ssn );
      }
      else
      {
        CAST_DMP( scn );
      }
      // Ready to start comparisons
      c = 0;
      switch ( used )
      {
        case 0: COMP( == ); break;
        case 1: COMP( != ); break;
        case 2: COMP( >  ); break;
        case 3: COMP( >= ); break;
        case 4: COMP( <  ); break;
        case 5: COMP( <= ); break;
        case 6: rBl = (( tNum & dNum ) == dNum );
        default: rBl = 0;
      }
      do
      {
        // Set aNum, mNum, nNum
        #ifdef _HUGE_DEFINED
        if ( bytes == sizeof( huge ) )
        {
          CAST_CMP( shn );
        }
        #endif
        else
        if ( bytes == sizeof( long ) )
        {
          CAST_CMP( sln );
        }
        else if ( bytes == sizeof( short ) )
        {
          CAST_CMP( ssn );
        }
        else
        {
          CAST_CMP( scn );
        }
        switch ( c )
        {
        case 0: COMPARE( == ); break;
        case 1: COMPARE( != ); break;
        case 2: COMPARE( >  ); break;
        case 3: COMPARE( >= ); break;
        case 4: COMPARE( <  ); break;
        case 5: COMPARE( <= ); break;
        default: rBl = 0;
        }
      }
      while ( rBl && c < cc );
      // Record result
      res.buff[i] = rBl;
      if ( rBl )
        ++rNo;
iterate:
      ++i;
    }
    while ( i < size );
write:
    IupWrPipe( dmp.pipe, tar.buff, size );
    IupWrPipe( res.pipe, res.buff, size );
  }
  while ( addr < addrc );
}
