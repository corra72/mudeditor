
#ifndef TS_UTILS_H
#define TS_UTILS_H

#include <QString>
#include <QTextStream>
#include <stdio.h>
#include "types.h"
#include "dice.h"



namespace ts
{

class Utils
{
public:
  static void setEntityReading( const QString& txt ) { m_entityReading = txt; }

  static QString readString( FILE*, const QString &defString = QString("") );
  static long readNumber( FILE*, const QString& error, long defNumber = 0 );
  static long readNumberInLine( FILE* pFile, const QString& error, long defNumber = 0 );
  static void writeNewFlags( QTextStream& stream, BitVector Flags, BitVector MoreFlags);
  static long readNewFlags( FILE *pFile, BitVector *Flags);

  //static VNumber readVNumber( FILE* );
  static Dice readDice( FILE* );

  static void saveBitVector( QTextStream&, BitVector );

  static QString stripSpaceAtEnd( const QString& );
  static QString createPath( const QString& value_dir, const QString& value_file = "", const QString& value_suffix = "" );
  static QString vnumber2string( VNumber, int num_chars = 5 );
  static VNumber string2vnumber( const QString& );
  static int double2int( double );

  static QString bitvector2string(BitVector flags, const QStringList &list, const QString& separator = ", " );

  static QString object2string( VNumber, const QString& );

  static inline double min( double first, double second ) { return ( first < second ? first : second ); }
  static inline double max( double first, double second ) { return ( first > second ? first : second ); }

  static QString removeTilde( const QString& );

protected:
  Utils() {}
  virtual ~Utils() {}

private:
  static QString m_entityReading;
};

} // namespace ts

#endif // TS_UTILS_H
