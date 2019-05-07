#include "room.h"
#include <qstringlist.h>
#include <QTextStream>
#include "utils.h"
#include "exception.h"
#include "constantname.h"

namespace ts
{

  void Room::init()
  {
    m_description = "";
    m_zone = ZoneVNumberInvalid;
    m_flags = 0;
    m_sectorType = ROOM_SECTOR_INSIDE;
    m_tunnelLimit = 3;
    /* teleport */
    m_teleportTime = 10;
    m_teleportToRoom = 0;
    m_teleportFlags = 0;
    m_teleportCount = 0;
    m_teleportSectorType = ROOM_SECTOR_INSIDE;
    /* river */
    m_riverDir = 0;
    m_riverSpeed = 0;
    /* lists */
    if( !m_extraDescriptions.empty() )
      m_extraDescriptions.clear();
    if( !m_exits.empty() )
      m_exits.clear();
  }

  Room::Room()
    : ObjectData( ObjectData::Object_Room )
  {
    init();
  }

  Room::Room( VNumber room_vnumber, const QString& room_name )
    : ObjectData( ObjectData::Object_Room, room_vnumber, room_name )
  {
    init();
  }

  Room::Room( const Room& r )
    : ObjectData( (ObjectData)r )
  {
    m_description = r.m_description;
    m_zone = r.m_zone;
    m_flags = r.m_flags;
    m_sectorType = r.m_sectorType;
    m_tunnelLimit = r.m_tunnelLimit;
    m_teleportTime = r.m_teleportTime;
    m_teleportToRoom = r.m_teleportToRoom;
    m_teleportFlags = r.m_teleportFlags;
    m_teleportCount = r.m_teleportCount;
    m_teleportSectorType = r.m_teleportSectorType;
    m_riverDir = r.m_riverDir;
    m_riverSpeed = r.m_riverSpeed;
    m_extraDescriptions = r.m_extraDescriptions;
    m_exits = r.m_exits;
  }

  Room::~Room()
  {}

  Room& Room::operator=( const Room& r )
  {
    if( this != &r )
    {
      ObjectData::operator=( (ObjectData)r );
      m_description = r.m_description;
      m_zone = r.m_zone;
      m_flags = r.m_flags;
      m_sectorType = r.m_sectorType;
      m_tunnelLimit = r.m_tunnelLimit;
      m_teleportTime = r.m_teleportTime;
      m_teleportToRoom = r.m_teleportToRoom;
      m_teleportFlags = r.m_teleportFlags;
      m_teleportCount = r.m_teleportCount;
      m_teleportSectorType = r.m_teleportSectorType;
      m_riverDir = r.m_riverDir;
      m_riverSpeed = r.m_riverSpeed;
      m_extraDescriptions = r.m_extraDescriptions;
      m_exits = r.m_exits;
    }
    return *this;
  }

  bool Room::hasExtraDescription( const QString& extra_keys ) const
  {
    try
    {
      (void) extraDescription( extra_keys );
      return true;
    }
    catch( XObjectNotFound& )
    {
      return false;
    }
  }

  const ExtraDescription& Room::extraDescription( const QString& extra_keys ) const
  {
    QString sKeysToFind = extra_keys.toLower();
    extra_description_const_iterator it = m_extraDescriptions.begin();

    while( it != m_extraDescriptions.end() )
    {
      if( (*it).keys() == sKeysToFind )
        return *it;
      ++it;
    }

    QString sException = "";
    sException.sprintf( "ExtraDescription '%s' not found.", sKeysToFind.toUtf8().data() );
    throw XObjectNotFound( sException );
  }

  void Room::removeExtraDescription( const QString& extra_keys )
  {
    QString sKeysToFind = extra_keys.toLower();
    extra_description_iterator it = m_extraDescriptions.begin();
    while( it != m_extraDescriptions.end() )
    {
      if( (*it).keys() == sKeysToFind )
      {
        qDebug( "%s removed from %s.", (*it).dumpObject().toUtf8().data(), dumpObject().toUtf8().data() );
        m_extraDescriptions.erase( it );
        return;
      }
      ++it;
    }
  }

  void Room::addExtraDescription( const ExtraDescription& new_extra_description )
  {
    removeExtraDescription( new_extra_description.keys() );
    m_extraDescriptions.append( new_extra_description );
    qDebug( "%s added to %s.", new_extra_description.dumpObject().toUtf8().data(), dumpObject().toUtf8().data() );
  }

  bool Room::hasExit( const QString& exit_name ) const
  {
    try
    {
      (void) exit( exit_name );
      return true;
    }
    catch( XObjectNotFound& )
    {
      return false;
    }
  }

  const Exit& Room::exit( const QString& exit_name ) const
  {
    QString sExitToFind = exit_name.toLower();
    exits_const_iterator it = m_exits.begin();
    while( it != m_exits.end() )
    {
      if( (*it).name() == sExitToFind )
        return *it;
      ++it;
    }

    QString sException = "";
    sException.sprintf( "Exit '%s' not found.", sExitToFind.toUtf8().data() );
    throw XObjectNotFound( sException );
  }

  bool Room::hasExit( int dir ) const
  {
    return hasExit( ConstantName::exitDirection( dir ) );
  }

  const Exit& Room::exit( int dir ) const
  {
    return exit( ConstantName::exitDirection( dir ) );
  }

  void Room::removeExit( const QString& exit_name )
  {
    QString sExitToFind = exit_name.toLower();
    exits_iterator it = m_exits.begin();
    while( it != m_exits.end() )
    {
      if( (*it).name() == sExitToFind )
      {
        qDebug( "%s removed from %s.", (*it).dumpObject().toUtf8().data(), dumpObject().toUtf8().data() );
        m_exits.erase( it );
        return;
      }
      ++it;
    }
  }

  void Room::addExit( Exit new_exit )
  {
    removeExit( new_exit.name() );
    new_exit.setFromRoom( m_vnumber );
    m_exits.append( new_exit );
    qDebug( "%s added to %s.", new_exit.dumpObject().toUtf8().data(), dumpObject().toUtf8().data() );
  }

  void Room::save( QTextStream& stream )
  {
    stream << "#" << m_vnumber << endl;
    stream << m_name << "~" << endl;
    stream << m_description << endl;
    stream << "~" << endl;
    stream << m_zone << " ";
    Utils::saveBitVector( stream, m_flags );
    stream << " " << flush;

    if( isTeleport() )
    {
      stream << "-1 " << m_teleportTime << " " << m_teleportToRoom << " ";
      Utils::saveBitVector( stream, m_teleportFlags );
      stream << " ";
      if( hasTeleportFlag( ROOM_TELEPORT_FLAG_COUNT ) )
        stream << m_teleportCount << " ";
    }

    stream << realSectorType() << flush;

    if( isRiver() )
    {
      stream << " " << m_riverSpeed << " " << m_riverDir << flush;
    }

    if( hasFlag( ROOM_FLAG_TUNNEL ) )
    {
      stream << " " << m_tunnelLimit << flush;
    }

    stream << endl << flush;

    /* Exit */
	qSort(m_exits.begin(), m_exits.end());
    exits_iterator ite = m_exits.begin();
    while( ite != m_exits.end() )
    {
      (*ite).save( stream );
      ++ite;
    }

    /* Extras */
    extra_description_iterator itd = m_extraDescriptions.begin();
    while( itd != m_extraDescriptions.end() )
    {
      (*itd).save( stream );
      ++itd;
    }

    /* Closing fields */
    stream << "S" << endl << flush;

    qDebug( "%s saving completed.", dumpObject().toUtf8().data() );
  }

  void Room::load( FILE* pFile )
  {
    setName( Utils::readString( pFile ) );

    QString sErrorMessage = "";
    sErrorMessage = dumpObject();

    setDescription( Utils::readString( pFile ) );

    m_zone = Utils::readNumber( pFile, sErrorMessage + " (ZoneVNumber)" );
    m_flags = Utils::readNumber( pFile, sErrorMessage + " (Flags)" );

    int iTmp = Utils::readNumber( pFile, sErrorMessage + " (SectorType)" );

    if( iTmp == -1 ) // Room is Teleport
      m_sectorType = ROOM_SECTOR_TELEPORT;
    else if( iTmp < 0 || iTmp >= ROOM_SECTOR_END )
    {
      qWarning( "%s has an invalid sector type %d (fixed).", sErrorMessage.toUtf8().data(), iTmp );
      m_sectorType = ROOM_SECTOR_INSIDE;
    }
    else
      m_sectorType = iTmp;

    if( isTeleport() )
    {
      m_teleportTime = Utils::readNumber( pFile, sErrorMessage + " (TeleportTime)" );
      m_teleportToRoom = Utils::readNumber( pFile, sErrorMessage + " (TeleportToRoom)" );
      m_teleportFlags = Utils::readNumber( pFile, sErrorMessage + " (TeleportFlags)" );
      if( hasTeleportFlag( ROOM_TELEPORT_FLAG_COUNT ) ) {
        m_teleportCount = Utils::readNumber( pFile, sErrorMessage + " (TeleportCount)" );
      }

      iTmp = Utils::readNumberInLine( pFile, sErrorMessage + " (RealSectorType)" );

      if( iTmp < 0 || iTmp >= ROOM_SECTOR_END || iTmp == ROOM_SECTOR_TELEPORT )
      {
        qWarning( "%s has an invalid teleport sector type %d (fixed).", sErrorMessage.toUtf8().data(), iTmp );
        m_teleportSectorType = ROOM_SECTOR_INSIDE;
        if( iTmp == -1L) {
            m_teleportSectorType = m_teleportCount;
            m_teleportCount = 0;
        }
      }
      else
        m_teleportSectorType = iTmp;
    }

    if( isRiver() )
    {
      m_riverSpeed = Utils::readNumberInLine( pFile, sErrorMessage + " (RiverSpeed)" );
      if(m_riverSpeed != -1L) {
          iTmp = Utils::readNumberInLine( pFile, sErrorMessage + " (RiverDir)" );
          if( iTmp >= EXIT_DIRECTION_NORTH && iTmp < EXIT_DIRECTION_END )
            m_riverDir = iTmp;
          else
            m_riverDir = EXIT_DIRECTION_INVALID;
      }
    }

    if( hasFlag( ROOM_FLAG_TUNNEL ) )
    {
      m_tunnelLimit = Utils::readNumber( pFile, sErrorMessage + " (TunnelLimit)" );
    }

    char chk[161];

    while( !feof( pFile ) && fgets( chk, 160, pFile ) )
    {
      switch( *chk )
      {
      case 'D': /* exit field */
        loadExit( pFile, QString( chk ) );
        break;
      case 'E': /* extra description field */
        loadExtraDescription( pFile );
        break;
      case 'S': /* end of room */
        qDebug( "%s loading completed.", dumpObject().toUtf8().data() );
        validate();
        return;
#if defined( TS_DEBUG_PANIC )
      default:
        qDebug( "Skip line: '%s'.", chk );
#endif
      }
    }

    qWarning( "%s loading completed. End of room not found.", dumpObject().toUtf8().data() );
    validate();
  }

  void Room::loadExtraDescription( FILE* pFile )
  {
    ExtraDescription new_extra;
    new_extra.load( pFile );
    addExtraDescription( new_extra );
  }

  void Room::loadExit( FILE* pFile, const QString& head )
  {
    QChar c = head.at(1);
    int eDirection = EXIT_DIRECTION_INVALID;

    if( c.isDigit() )
    {
      int iDir = c.digitValue();
      if( iDir >= EXIT_DIRECTION_NORTH && iDir < EXIT_DIRECTION_END )
      {
        eDirection = iDir;
        Exit new_exit( eDirection, m_vnumber );
        new_exit.load( pFile );
        addExit( new_exit );
      }
      else
      {
        qWarning( "%s has an invalid exit direction in line '%s'.", dumpObject().toUtf8().data(), head.toUtf8().data() );
      }
    }
    else
    {
      QString sTmp = head;
      sTmp.remove( 0, 1 );  // remove 'D'
      QStringList sList = sTmp.split( "~" );
      QString name = sList.count() >= 1 ? sList[ 0 ] : "";
      QString listName = sList.count() >= 2 ? sList[ 1 ] : "";
      QString toName = sList.count() >= 3 ? sList[ 2 ] : "";
      QString fromName = sList.count() >= 4 ? sList[ 3 ] : "";
      QString revDirName = sList.count() >= 5 ? sList[ 4 ] : "";

      Exit new_exit( name, m_vnumber );
      new_exit.setListName( listName );
      new_exit.setToName( toName );
      new_exit.setFromName( fromName );
      new_exit.setRevDirName( revDirName );
      new_exit.load( pFile );
      addExit( new_exit );
    }
  }

  void Room::validate()
  {
    if( m_name.isEmpty() )
      qWarning( "%s has an empty name.", dumpObject().toUtf8().data() );

    if( m_description.isEmpty() )
      qWarning( "%s has an empty description.", dumpObject().toUtf8().data() );

    if( m_zone < 1 && m_zone != VNumberInvalid )
      qDebug( "%s has an invalid zone number #%ld.", dumpObject().toUtf8().data(), m_zone );

    if( hasFlag( ROOM_FLAG_DEATH ) )
      qWarning( "%s is a death trap.", dumpObject().toUtf8().data() );

    if( isTeleport() )
    {
      if( m_teleportTime <= 0 || m_teleportTime > 100 || m_teleportTime % 10 > 0 )
      {
        qWarning( "%s has an invalid teleport time %d, fixed to 10.", dumpObject().toUtf8().data(), m_teleportTime );
        m_teleportTime = 10;
      }

      if( m_teleportToRoom <= 0 )
        qWarning( "%s has an invalid teleport target room #%ld.", dumpObject().toUtf8().data(), m_teleportToRoom );

      if( hasTeleportFlag( ROOM_TELEPORT_FLAG_COUNT ) && m_teleportCount <= 0 )
      {
        qWarning( "%s has an invalid teleport count %d, fixed to 1.", dumpObject().toUtf8().data(), m_teleportCount );
        m_teleportCount = 1;
      }
    }

    if( isRiver() )
    {
      if( m_riverSpeed < 0 || m_riverSpeed % 15 > 0 )
      {
        qWarning( "%s has an invalid river speed %d, fixed to 15.", dumpObject().toUtf8().data(), m_riverSpeed );
        m_riverSpeed = 15;
      }

      if( m_riverDir == EXIT_DIRECTION_INVALID )
      {
        qWarning( "%s has an invalid river direction, fixed to North.", dumpObject().toUtf8().data() );
        m_riverDir = EXIT_DIRECTION_NORTH;
      }
    }

    if( hasFlag( ROOM_FLAG_TUNNEL ) && m_tunnelLimit < 1 )
    {
      qWarning( "%s has an invalid tunnel limit %d, fixed to 3.", dumpObject().toUtf8().data(), m_tunnelLimit );
      m_tunnelLimit = 3;
    }

    if( !m_exits.empty() )
    {
      exits_iterator ite = m_exits.begin();
      while( ite != m_exits.end() )
      {
        if( (*ite).name().isEmpty() )
          qWarning( "%s has an empty exit name.", dumpObject().toUtf8().data() );

        if( (*ite).fromRoom() != m_vnumber )
        {
          qWarning( "%s has an exit of another room, fixed." , dumpObject().toUtf8().data() );
          (*ite).setFromRoom( m_vnumber );
        }

        ++ite;
      }
    }
  }

} // namespace ts
