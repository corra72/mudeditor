
#include "extradescription.h"
#include "utils.h"


namespace ts
{

  ExtraDescription::ExtraDescription()
    : m_keys( "" ),
      m_description( "" )
  {
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created (default).", dumpObject().toUtf8().data() );
#endif
  }

  ExtraDescription::ExtraDescription( const QString& extra_keys, const QString& extra_desc )
    : m_keys( extra_keys.toLower().simplified() ),
      m_description( extra_desc )
  {
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created.", dumpObject().toUtf8().data() );
#endif
  }

  ExtraDescription::ExtraDescription( const ExtraDescription& ed )
  {
    m_keys = ed.m_keys;
    m_description = ed.m_description;
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created (by copy).", dumpObject().toUtf8().data() );
#endif
  }

  ExtraDescription::~ExtraDescription()
  {
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s deleted.", dumpObject().toUtf8().data() );
#endif
  }

  ExtraDescription& ExtraDescription::operator=( const ExtraDescription& ed )
  {
    if( this != &ed )
    {
#if defined( TS_DEBUG_PANIC )
      qDebug( "%s overwritten by %s.", dumpObject().toUtf8().data(), ed.dumpObject().toUtf8().data() );
#endif
      m_keys = ed.m_keys;
      m_description = ed.m_description;
    }
    return *this;
  }

  QString ExtraDescription::dumpObject() const
  {
    QString sDump = "";
    sDump.sprintf( "ExtraDescription '%s'", m_keys.toUtf8().data() );
    return sDump;
  }

#if defined( USE_QT_FILE_IO )
  void ExtraDescription::load( QTextStream& stream )
  {
    setKeys( Utils::readString( stream ) );
    setDescription( Utils::readString( stream ) );
    qDebug( "%s loading completed.", dumpObject().toUtf8().data() );
  }
#else
  void ExtraDescription::load( FILE* pFile )
  {
    setKeys( Utils::readString( pFile ) );
    setDescription( Utils::readString( pFile ) );
    qDebug( "%s loading completed.", dumpObject().toUtf8().data() );
  }
#endif

  void ExtraDescription::save( QTextStream& stream )
  {
    stream << "E" << endl;
    stream << m_keys << "~" << endl;
    stream << m_description << endl << "~" << endl << flush;
    qDebug( "%s saving completed.", dumpObject().toUtf8().data() );
  }

} // namespace ts
