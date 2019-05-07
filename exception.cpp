
#include "exception.h"

namespace ts
{
  Exception::Exception()
    : m_type( Unknown ),
      m_description( "" )
  {}

  Exception::Exception( Type new_type, const QString& new_desc )
    : m_type( new_type ),
      m_description( new_desc )
  {
    qDebug( "%s raised.", dumpObject().toUtf8().data() );
  }

  Exception::Exception( const Exception& x )
  {
    m_type = x.m_type;
    m_description = x.m_description;
  }

  Exception::~Exception()
  {

  }

  Exception& Exception::operator=( const Exception& x )
  {
    m_type = x.m_type;
    m_description = x.m_description;
    return *this;
  }

  QString Exception::dumpObject() const
  {
    QString sType;

    switch( m_type )
    {
    case Runtime:
      sType = "Runtime";
      break;
    default:
      sType = "Unknown";
    }

    QString sRet = "";
    sRet.sprintf( "Exception (%s) '%s'", sType.toUtf8().data(), m_description.toUtf8().data() );
    return sRet;
  }

} // namespace ts
