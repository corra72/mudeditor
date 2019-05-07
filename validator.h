
#ifndef TS_VALIDATOR_H
#define TS_VALIDATOR_H

#include <QIntValidator>

class Validator
{
public:
  static const QIntValidator* vnumber()
  {
    if( !mp_vnumber )
      mp_vnumber = new QIntValidator( -1, 99999, 0 );
    return mp_vnumber;
  }

  static const QIntValidator* integer()
  {
    if( !mp_integer )
      mp_integer = new QIntValidator( 0 );
    return mp_integer;
  }

  static const QIntValidator* unsignedInteger()
  {
    if( !mp_unsignedInteger )
    {
      mp_unsignedInteger = new QIntValidator( 0 );
      mp_unsignedInteger->setBottom( 0 );
    }
    return mp_integer;
  }

  static void cleanUp()
  {
    qDebug( "Validator::cleanUp() called." );
    if( mp_vnumber )
    {
      delete mp_vnumber;
      mp_vnumber = NULL;
    }

    if( mp_integer )
    {
      delete mp_integer;
      mp_integer = NULL;
    }

    if( mp_unsignedInteger )
    {
      delete mp_unsignedInteger;
      mp_unsignedInteger = NULL;
    }
  }

protected:
  Validator();
  ~Validator() { cleanUp(); }

private:
  static QIntValidator* mp_vnumber;
  static QIntValidator* mp_integer;
  static QIntValidator* mp_unsignedInteger;

};

#endif // TS_VALIDATOR_H
