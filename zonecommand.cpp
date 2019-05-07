
#include "zonecommand.h"
#include "settings.h"


namespace ts
{

  void ZoneCommand::initArguments()
  {
    setArgument( Argument0, 0 );

    if( isMobLoad() || isMobFollower() )
    {
      m_numArgs = 4;
    }
    else if( isItemPut() || isMobEquip() )
    {
      setArgument( Argument0, 1 );
      m_numArgs = 4;
    }
    else if( isMobGive() || isMobFear() || isMobHate() )
    {
      setArgument( Argument0, 1 );
      m_numArgs = 3;
    }
    else if( isDoorInit() )
    {
      m_numArgs = 4;
    }
    else if( isOnlyComment() || isZoneEnd() )
    {
      m_numArgs = 0;
    }
    else
      m_numArgs = 5;

    for( int i = 1; i < LastArgument; i++ )
    {
      if( i < m_numArgs )
      {
        if( argument( (Argument)i ) < 0 )
          setArgument( (Argument)i, 0 );
      }
      else
        setArgument( (Argument)i, -1 );
    }
  }

  void ZoneCommand::init()
  {
    for( int i = 0; i < LastArgument; i++ )
      setArgument( (Argument)i, 0 );

    initArguments();

    m_comment = "";
    m_commentSeparator = Settings::defaultCommentSeparator() ;
  }

  void ZoneCommand::validate()
  {

  }

  ZoneCommand::ZoneCommand()
    : m_type( '*' ),
      m_numArgs( 0 ),
      m_id( VNumberInvalid ),
      m_comment( "" ),
      m_commentSeparator( Settings::defaultCommentSeparator() )
  {
    init();
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created (default).", dumpObject().toUtf8().data() );
#endif
  }

  ZoneCommand::ZoneCommand( const ZoneCommand& zc )
  {
    m_type = zc.type();
    init();
    for( int i = 0; i < LastArgument; i++ )
      setArgument( (Argument)i, zc.argument( (Argument)i ) );
    m_numArgs = zc.m_numArgs;
    m_id = zc.m_id;
    m_comment = zc.m_comment;
    m_commentSeparator = zc.m_commentSeparator;
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created (by copy).", dumpObject().toUtf8().data() );
#endif
  }

  ZoneCommand::~ZoneCommand()
  {
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s deleted.", dumpObject().toUtf8().data() );
#endif
  }

  ZoneCommand& ZoneCommand::operator=( const ZoneCommand& zc )
  {
    if( this != &zc )
    {
#if defined( TS_DEBUG_PANIC )
      qDebug( "%s overwritten by %s.", dumpObject().toUtf8().data(), zc.dumpObject().toUtf8().data() );
#endif
      m_type = zc.type();
      for( int i = 0; i < LastArgument; i++ )
        setArgument( (Argument)i, zc.argument( (Argument)i ) );
      m_numArgs = zc.m_numArgs;
      m_id = zc.m_id;
      m_comment = zc.m_comment;
      m_commentSeparator = zc.m_commentSeparator;
    }
    return *this;
  }

  QString ZoneCommand::dumpObject() const
  {
    QString sDump = "";
    sDump.sprintf( "ZoneCommand #%ld '%c", m_id, (char)m_type.unicode() );
    for( int i = 0; i < LastArgument; i++ )
      if( argument( (Argument)i ) >= 0 )
        sDump += QString( " %1" ).arg( QString::number( argument( (Argument)i ) ) );
    sDump += QString( "'" );
    return sDump;
  }

// M <if-flag> <mob vnum> <max #> <room vnum>
// C <if-flag> <mob vnum> <max #> <room vnum>
// G <if-flag> <object vnum> <max #>
// E <if-flag> <object vnum> <max #> <equipment position>
// H <if-flag> <field> <operator>
// F <if-flag> <field> <operator>
// O <if-flag> <object vnum> <max #> <room vnum> <max room #>
// P <if-flag> <loaded object vnum> <max #> <container vnum>
// R <if-flag> <object vnum> <room vnum>
// K <if-flag> <mob vnum> <room vnum>
// D <if-flag> <room vnum> <exit #> <doorstate>

  void ZoneCommand::loadComment( const QString& sLine )
  {
    if( sLine.isNull() || sLine.isEmpty() )
    {
      qDebug( "ZoneCommand LoadComment: empty line." );
      setComment( "" );
      return;
    }

    int iIndex = sLine.indexOf( QChar( ';' ) );
    if( iIndex < 0 )
    {
      iIndex = sLine.indexOf( QChar( '*' ) );
    }
    else
      m_commentSeparator = QString( ";" );

    if( iIndex < 0 )
    {
      qDebug( "ZoneCommand LoadComment: no comment found." );
      setComment( "" );
      return;
    }
    else
      m_commentSeparator = Settings::defaultCommentSeparator();


    QString sComment = sLine.mid( iIndex, sLine.length() );
    sComment = sComment.simplified();

    if( sComment.isNull() || sComment.isEmpty() )
    {
      qDebug( "ZoneCommand LoadComment: no comment found after separator." );
      setComment( "" );
      return;
    }

    if( sComment.startsWith( Settings::defaultCommentSeparator() ) )
    {
      sComment.remove( 0, Settings::defaultCommentSeparator().length() );
    }
    else if( sComment.startsWith( QString( ";" ) ) || sComment.startsWith( QString( "*" ) ) )
    {
      sComment.remove( 0, 1 );
      if( sComment.startsWith( QString( "!" ) ) )
        sComment.remove( 0, 1 );
    }

    setComment( sComment );
  }

  void ZoneCommand::load( FILE* pFile )
  {
    char szBuffer[ 255 ];

    *szBuffer = '\0';

    if( !fgets( szBuffer, 254, pFile ) )
    {
      qDebug( "ZoneCommand Load: error reading file." );
      return;
    }

    QString sLine = QString( szBuffer ).simplified();

    if( sLine.isEmpty() )
    {
      qDebug( "ZoneCommand Load: skip empty line." );
      return;
    }

    setType( sLine.at( 0 ) );

    if( isZoneEnd() )
    {
      qDebug( "ZoneCommand Load: end of zone found." );
      return;
    }

    if( !isValidType() )
    {
      qWarning( "ZoneCommand Load: invalid command on line '%s'.", sLine.toUtf8().data() );
      return;
    }

    if( isOnlyComment() )
    {
      loadComment( sLine );
      return;
    }

    sLine.remove( 0, 1 );
    QTextStream ts( &sLine, QIODevice::ReadOnly );
    for( int i = 0; i < m_numArgs; i++ )
    {
      if( !ts.atEnd() )
      {
        ts >> mp_args[ i ];
      }
      else
      {
        qDebug( "ZoneCommand Load: invalid number of arguments found in line '%s'.", sLine.toUtf8().data() );
        return;
      }
    }

    loadComment( sLine );

    qDebug( "%s loading completed.", dumpObject().toUtf8().data() );
  }

  void ZoneCommand::save( QTextStream& stream ) const
  {
    if( !isValidType() )
      return;

    stream << toString( true ) << endl << flush;

    qDebug( "%s saving completed.", dumpObject().toUtf8().data() );
  }

  QString ZoneCommand::toString( bool append_comment ) const
  {
    QString sCommand = "";
    if( isOnlyComment() )
    {
      if( !append_comment )
        return sCommand;

      if( !m_comment.isEmpty() )
        sCommand += m_commentSeparator + QString( " " ) + m_comment;
      else
        sCommand = "";
      return sCommand;
    }

    sCommand = QString( m_type ) + QString( " " );

    sCommand += QString::number( argument( Argument0 ) ) + QString( " " );


    for( int i = (int)Argument1; i < m_numArgs; i++ )
      sCommand += QString::number( argument( (Argument)i ) ) + QString( " " );

    if( !m_comment.isEmpty() && append_comment )
      sCommand += QString( " " ) + m_commentSeparator + QString( " " ) + m_comment;

    return sCommand;
  }

  int ZoneCommand::priority() const
  {
    if( isMobLoad() )
      return 2000000 + argument( Argument1 );
    else if( isMobFollower() )
      return 3000000 + argument( Argument1 );
    else if( isMobEquip() )
      return 4000000 + argument( Argument3 );
    else if( isMobGive() )
      return 5000000 + argument( Argument1 );
    else if( isMobHate() )
      return 6000000;
    else if( isMobFear() )
      return 7000000;
    else if( isItemLoad() )
      return 9000000 + argument( Argument1 );
    else if( isItemPut() )
      return 10000000 + argument( Argument3 );
    else if( isDoorInit() )
      return 11000000 + argument( Argument1 );
    else
      return 50000000;
  }

} // namespace ts




