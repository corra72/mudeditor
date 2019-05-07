#include "random.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "utils.h"


namespace ts
{

  void Random::init()
  {
    qDebug( "Random Number Generator Initialized." );
    srand( time( NULL ) );
  }

  double Random::private_random()
  {
    double div = 1.0 + RAND_MAX;
    return (rand()/div);
  }

  int Random::number( int from, int to )
  {
    if( from > to )
      return 0;
    else if( from == to )
      return from;
    else
      return (Utils::double2int(private_random()*( to - from )) + from);
  }

  int Random::dice( int num, int faces, int bonus )
  {
    int iRet = bonus;
    for( int i = 0; i < num; i++ )
      iRet += number( 1, faces );
    return iRet;
  }

  int Random::dice( const QString& sDiceXdYplusZ )
  {
    int num = 0, faces = 0, bonus = 0;
    sscanf( sDiceXdYplusZ.toUtf8().data(), "%dd%d+%d", &num, &faces, &bonus );
    return dice( num, faces, bonus );
  }

} // namespace ts
