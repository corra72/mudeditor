
#ifndef TS_RANDOM_H
#define TS_RANDOM_H

#include <QString>


namespace ts
{

  class Random
  {
  public:
    static void init();
    static int number( int from, int to );
    static int dice( int num, int faces, int bonus = 0 );
    static int dice( const QString& sDiceXdYplusZ );
    static inline int d2( int num = 1 ) { return number( num, 2 ); }
    static inline int d4( int num = 1 ) { return number( num, 4 ); }
    static inline int d6( int num = 1 ) { return number( num, 6 ); }
    static inline int d8( int num = 1 ) { return number( num, 8 ); }
    static inline int d10( int num = 1 ) { return number( num, 10 ); }
    static inline int d12( int num = 1 ) { return number( num, 12 ); }
    static inline int d20( int num = 1 ) { return number( num, 20 ); }
    static inline int d100( int num = 1 ) { return number( num, 100 ); }

  protected:
    Random() {};
    virtual ~Random() {};

  private:
    static int double2int( double k );
    static double private_random();

  };

} // namespace ts

#endif // TS_RANDOM_H
