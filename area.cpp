#include "area.h"
#include <QFile>
#include <QTextStream>
#include "exception.h"
#include "utils.h"
#include "settings.h"
#include "constantname.h"
#include "eleuconf.h"
#include <qmessagebox.h>

namespace ts
{

  void Area::resetFilePaths()
  {
    m_roomsFilePath = "";
    m_mobsFilePath = "";
    m_itemsFilePath = "";
    m_zonesFilePath = "";
    m_shopsFilePath = "";
  }

  void Area::init()
  {
    resetFilePaths();

    if( !m_rooms.empty() )
      m_rooms.clear();
    if( !m_mobs.empty() )
      m_mobs.clear();
    if( !m_items.empty() )
      m_items.clear();
    if( !m_zones.empty() )
      m_zones.clear();

    m_roomsSorted = true;
    m_zonesSorted = true;
    m_itemsSorted = true;
    m_mobsSorted = true;
    m_shopsSorted = true;
    m_roomsChanged = false;
    m_zonesChanged = false;
    m_itemsChanged = false;
    m_mobsChanged = false;
    m_shopsChanged = false;
  }

  Area::Area()
  {
    m_name = "newarea";
    init();
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created (default).", qPrintable( dumpObject() ) );
#endif
  }

  Area::Area( const QString& new_name )
    : m_name( new_name )
  {
    init();
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created.", qPrintable( dumpObject() ) );
#endif
  }

  Area::Area( const Area& a )
  {
    m_name = a.m_name;
    m_roomsFilePath = a.m_roomsFilePath;
    m_mobsFilePath = a.m_mobsFilePath;
    m_itemsFilePath = a.m_itemsFilePath;
    m_zonesFilePath = a.m_zonesFilePath;
    m_shopsFilePath = a.m_shopsFilePath;
    m_rooms = a.m_rooms;
    m_items = a.m_items;
    m_mobs = a.m_mobs;
    m_zones = a.m_zones;
    m_roomsSorted = a.m_roomsSorted;
    m_zonesSorted = a.m_zonesSorted;
    m_itemsSorted = a.m_itemsSorted;
    m_mobsSorted = a.m_mobsSorted;
    m_shopsSorted = a.m_shopsSorted;
    m_shopsChanged = a.m_shopsChanged;
    m_roomsChanged = a.m_roomsChanged;
    m_zonesChanged = a.m_zonesChanged;
    m_itemsChanged = a.m_itemsChanged;
    m_mobsChanged = a.m_mobsChanged;
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created (by copy).", qPrintable( dumpObject() ) );
#endif
  }

  Area::~Area()
  {
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s deleted.", qPrintable( dumpObject() ) );
#endif
  }

  Area& Area::operator=( const Area& a )
  {
    if( this != &a )
    {
#if defined( TS_DEBUG_PANIC )
      qDebug( "%s overwritten by %s.", qPrintable( dumpObject() ), a.qPrintable( dumpObject() ) );
#endif
      m_name = a.m_name;
      m_roomsFilePath = a.m_roomsFilePath;
      m_mobsFilePath = a.m_mobsFilePath;
      m_itemsFilePath = a.m_itemsFilePath;
      m_zonesFilePath = a.m_zonesFilePath;
      m_shopsFilePath = a.m_shopsFilePath;
      m_rooms = a.m_rooms;
      m_items = a.m_items;
      m_mobs = a.m_mobs;
      m_zones = a.m_zones;
      m_roomsSorted = a.m_roomsSorted;
      m_zonesSorted = a.m_zonesSorted;
      m_itemsSorted = a.m_itemsSorted;
      m_mobsSorted = a.m_mobsSorted;
      m_shopsSorted = a.m_shopsSorted;
      m_shopsChanged = a.m_shopsChanged;
      m_roomsChanged = a.m_roomsChanged;
      m_zonesChanged = a.m_zonesChanged;
      m_itemsChanged = a.m_itemsChanged;
      m_mobsChanged = a.m_mobsChanged;
    }
    return *this;
  }

  QString Area::dumpObject() const
  {
    QString sDump = "";
    sDump.sprintf( "Area '%s'", qPrintable( m_name ) );
    return sDump;
  }

  const Room& Area::room( VNumber vnum ) const
  {
    rooms_const_iterator it = m_rooms.begin();
    while( it != m_rooms.end() )
    {
      if( (*it).vnumber() == vnum )
        return *it;
      ++it;
    }

    QString sException = "";
    sException.sprintf( "Room #%ld not found.", vnum );
    throw XObjectNotFound( sException );
  }

  bool Area::hasRoom( VNumber vnum ) const
  {
    try
    {
      (void) room( vnum );
      return true;
    }
    catch( XObjectNotFound& )
    {
      return false;
    }
  }

  void Area::removeRoom( VNumber vnum )
  {
    rooms_iterator it = m_rooms.begin();
    while( it != m_rooms.end() )
    {
      if( (*it).vnumber() == vnum )
      {
        qDebug( "%s removed from %s.", qPrintable( (*it).dumpObject() ), qPrintable( dumpObject() ) );
        m_rooms.erase( it );
        return;
      }
      ++it;
    }
  }

  void Area::addRoom( const Room& r )
  {
    removeRoom( r.vnumber() );
    m_rooms.append( r );
    m_roomsSorted = false;
    qDebug( "%s added to %s.", qPrintable( r.dumpObject() ), qPrintable( dumpObject() ) );
  }

  void Area::sortRooms()
  {
    if( m_roomsSorted )
      return;
    qSort( m_rooms );
    m_roomsSorted = true;
    qDebug( "%s sorting rooms completed.", qPrintable( dumpObject() ) );
  }

  bool Area::loadRooms()
  {
    if( m_roomsFilePath.isEmpty() )
      m_roomsFilePath = Utils::createPath( Settings::roomsDirectory(), name(), Settings::roomSuffix() );

    VNumber vnum;
    FILE *pFile;

    if( ( pFile = fopen( qPrintable( m_roomsFilePath ), "r" ) ) )
    {
      char szBuffer[ 82 ];

      while( fgets( szBuffer, sizeof( szBuffer ), pFile ) )
      {
        if( *szBuffer == '#' )
        {
          vnum = -1;
          sscanf( szBuffer, "#%ld", &vnum );

          if( vnum >= VNumberInvalid && vnum < VNumberTop )
          {
            Room new_room( vnum );
            new_room.load( pFile );
            addRoom( new_room );
          }
          else
          {
            qWarning( "Invalid Room VNumber in string '%s' reading file '%s'.",
              szBuffer, qPrintable( m_roomsFilePath ) );
          }
        }
      }

      fclose( pFile );
    }
    else
    {
      qWarning( "%s loading rooms not started. Unable to open file '%s'.",
        qPrintable( dumpObject() ), qPrintable( m_roomsFilePath ) );
      return false;
    }

    qDebug( "%s loading %d rooms completed. File '%s' read.",
        qPrintable( dumpObject() ), m_rooms.size(), qPrintable( m_roomsFilePath ) );
    m_roomsSorted = false;
    sortRooms();
    m_roomsChanged = false;
    return true;
  }

  bool Area::saveRooms()
  {
    if( m_rooms.empty() )
    {
      qDebug( "%s saving 0 rooms completed. List is empty.", qPrintable( dumpObject() ) );
      return true;
    }

    m_roomsSorted = false;
    sortRooms();

    QString file_name;

    if( m_roomsFilePath.isEmpty() )
    {
      qWarning( "Unable to save a rooms file with an empty path." );
      return false;
    }

    QFile file( m_roomsFilePath );
    if( !file.open( QIODevice::WriteOnly ) )
    {
      qDebug( "%s saving rooms not possible. Unable to open file '%s'.",
        qPrintable( dumpObject() ), qPrintable( file_name ) );
      return false;
    }

    rooms_iterator it = m_rooms.begin();
    QTextStream stream( &file );
    while( it != m_rooms.end() )
    {
      (*it).save( stream );
      ++it;
    }

    stream << flush;
    file.close();
    qDebug( "%s saving %d rooms completed. File '%s' written.",
        qPrintable( dumpObject() ), m_rooms.size(), qPrintable( file_name ) );
    m_roomsChanged = false;
    return true;
  }

  const Mob& Area::mob( VNumber vnum ) const
  {
    mobs_const_iterator it = m_mobs.begin();
    while( it != m_mobs.end() )
    {
      if( (*it).vnumber() == vnum )
        return *it;
      ++it;
    }

    QString sException = "";
    sException.sprintf( "Mob #%ld not found.", vnum );
    throw XObjectNotFound( sException );
  }

  bool Area::hasMob( VNumber vnum ) const
  {
    try
    {
      (void) mob( vnum );
      return true;
    }
    catch( XObjectNotFound& )
    {
      return false;
    }
  }

  void Area::removeMob( VNumber vnum )
  {
    mobs_iterator it = m_mobs.begin();
    while( it != m_mobs.end() )
    {
      if( (*it).vnumber() == vnum )
      {
        qDebug( "%s removed from %s.", qPrintable( (*it).dumpObject() ), qPrintable( dumpObject() ) );
        m_mobs.erase( it );
        return;
      }
      ++it;
    }
  }

  void Area::addMob( const Mob& m )
  {
    removeMob( m.vnumber() );
    m_mobs.append( m );
    m_mobsSorted = false;
    qDebug( "%s added to %s.", qPrintable( m.dumpObject() ), qPrintable( dumpObject() ) );
  }

  void Area::sortMobs()
  {
    if( m_mobsSorted )
      return;
    qSort( m_mobs );
    m_mobsSorted = true;
    qDebug( "%s sorting mobs completed.", qPrintable( dumpObject() ) );
  }

  bool Area::loadMobs()
  {
    if( m_mobsFilePath.isEmpty() )
      m_mobsFilePath = Utils::createPath( Settings::mobsDirectory(), name(), Settings::mobSuffix() );

    VNumber vnum;
    FILE *pFile;

#if defined( Q_WS_WIN )
    // FIXME: fseek ha problemi con lo \r\n. Bisogna leggere in binario il file.
    if( ( pFile = fopen( qPrintable( m_mobsFilePath ), "rb" ) ) )
#else
    if( ( pFile = fopen( qPrintable( m_mobsFilePath ), "r" ) ) )
#endif
    {
      char szBuffer[ 82 ];

      while( fgets( szBuffer, sizeof( szBuffer ), pFile ) )
      {
        if( *szBuffer == '#' )
        {
          vnum = -1;
          sscanf( szBuffer, "#%ld", &vnum );

          if( vnum >= VNumberInvalid && vnum < VNumberTop )
          {
            Mob new_mob( vnum );
            new_mob.load( pFile );
            addMob( new_mob );
          }
          else
          {
            qWarning( "Invalid Mob VNumber in string '%s' reading file '%s'.",
              szBuffer, qPrintable( m_mobsFilePath ) );
          }
        }
      }

      fclose( pFile );
    }
    else
    {
      qWarning( "%s loading mobs not started. Unable to open file '%s'.",
        qPrintable( dumpObject() ), qPrintable( m_mobsFilePath ) );
      return false;
    }

    qDebug( "%s loading %d mobs completed. File '%s' read.",
        qPrintable( dumpObject() ), m_mobs.size(), qPrintable( m_mobsFilePath ) );
    m_mobsSorted = false;
    sortRooms();
    m_mobsChanged = false;
    return true;
  }

  bool Area::saveMobs()
  {
    if( m_mobs.empty() )
    {
      qDebug( "%s saving 0 mobs completed. List is empty.", qPrintable( dumpObject() ) );
      return true;
    }

    m_mobsSorted = false;
    sortMobs();

    if( m_mobsFilePath.isEmpty() )
    {
      qWarning( "Unable to save a mobs file with an empty path." );
      return false;
    }

    QFile file( m_mobsFilePath );
    if( !file.open( QIODevice::WriteOnly ) )
    {
      qDebug( "%s saving mobs not possible. Unable to open file '%s'.",
        qPrintable( dumpObject() ), qPrintable( m_mobsFilePath ) );
      return false;
    }

    mobs_iterator it = m_mobs.begin();
    QTextStream stream( &file );
    while( it != m_mobs.end() )
    {
      (*it).save( stream );
      ++it;
    }

    stream << flush;
    file.close();
    qDebug( "%s saving %d mobs completed. File '%s' written.",
        qPrintable( dumpObject() ), m_mobs.size(), qPrintable( m_mobsFilePath ) );
    m_mobsChanged = false;
    return true;
  }

  const Item& Area::item( VNumber vnum ) const
  {
    items_const_iterator it = m_items.begin();
    while( it != m_items.end() )
    {
      if( (*it).vnumber() == vnum )
        return *it;
      ++it;
    }

    QString sException = "";
    sException.sprintf( "Item #%ld not found.", vnum );
    throw XObjectNotFound( sException );
  }

  bool Area::hasItem( VNumber vnum ) const
  {
    try
    {
      (void) item( vnum );
      return true;
    }
    catch( XObjectNotFound& )
    {
      return false;
    }
  }

  void Area::removeItem( VNumber vnum )
  {
    items_iterator it = m_items.begin();
    while( it != m_items.end() )
    {
      if( (*it).vnumber() == vnum )
      {
        qDebug( "%s removed from %s.", qPrintable( (*it).dumpObject() ), qPrintable( dumpObject() ) );
        m_items.erase( it );
        return;
      }
      ++it;
    }
  }

  void Area::addItem( const Item& i )
  {
    removeItem( i.vnumber() );
    m_items.append( i );
    m_itemsSorted = false;
    qDebug( "%s added to %s.", qPrintable( i.dumpObject() ), qPrintable( dumpObject() ) );
  }

  void Area::sortItems()
  {
    if( m_itemsSorted )
      return;
    qSort( m_items );
    m_itemsSorted = true;
    qDebug( "%s sorting items completed.", qPrintable( dumpObject() ) );
  }

  bool Area::loadItems()
  {
    if( m_itemsFilePath.isEmpty() )
      m_itemsFilePath = Utils::createPath( Settings::itemsDirectory(), name(), Settings::itemSuffix() );

    VNumber vnum;
    FILE *pFile;

#if defined( Q_WS_WIN )
    // FIXME: fseek ha problemi con lo \r\n. Bisogna leggere in binario il file.
    if( ( pFile = fopen( qPrintable( m_itemsFilePath ), "rb" ) ) )
#else
    if( ( pFile = fopen( qPrintable( m_itemsFilePath ), "r" ) ) )
#endif
    {
      char szBuffer[ 82 ];

      while( fgets( szBuffer, sizeof( szBuffer ), pFile ) )
      {
        if( *szBuffer == '#' )
        {
          vnum = -1;
          sscanf( szBuffer, "#%ld", &vnum );

          if( vnum >= VNumberInvalid && vnum < VNumberTop )
          {
            Item new_item( vnum );
            new_item.load( pFile );
            addItem( new_item );
          }
          else
          {
            qWarning( "Invalid Item VNumber in string '%s' reading file '%s'.",
              szBuffer, qPrintable( m_itemsFilePath ) );
          }
        }
      }

      fclose( pFile );
    }
    else
    {
      qWarning( "%s loading items not started. Unable to open file '%s'.",
        qPrintable( dumpObject() ), qPrintable( m_itemsFilePath ) );
      return false;
    }

    qDebug( "%s loading %d items completed. File '%s' read.",
        qPrintable( dumpObject() ), m_items.size(), qPrintable( m_itemsFilePath ) );
    m_itemsSorted = false;
    sortRooms();
    m_itemsChanged = false;
    return true;
  }

  bool Area::saveItems()
  {
    if( m_items.empty() )
    {
      qDebug( "%s saving 0 items completed. List is empty.", qPrintable( dumpObject() ) );
      return true;
    }

    m_itemsSorted = false;
    sortItems();

    if( m_itemsFilePath.isEmpty() )
    {
      qWarning( "Unable to save a items file with an empty path." );
      return false;
    }

    QFile file( m_itemsFilePath );
    if( !file.open( QIODevice::WriteOnly ) )
    {
      qDebug( "%s saving items not possible. Unable to open file '%s'.",
        qPrintable( dumpObject() ), qPrintable( m_itemsFilePath ) );
      return false;
    }

    items_iterator it = m_items.begin();
    QTextStream stream( &file );
    while( it != m_items.end() )
    {
      (*it).save( stream );
      ++it;
    }

    stream << flush;
    file.close();
    qDebug( "%s saving %d items completed. File '%s' written.",
        qPrintable( dumpObject() ), m_items.size(), qPrintable( m_itemsFilePath ) );
    m_itemsChanged = false;
    return true;
  }

  const Zone& Area::zone( VNumber vnum ) const
  {
    zones_const_iterator it = m_zones.begin();
    while( it != m_zones.end() )
    {
      if( (*it).vnumber() == vnum )
        return *it;
      ++it;
    }

    QString sException = "";
    sException.sprintf( "Zone #%ld not found.", vnum );
    throw XObjectNotFound( sException );
  }

  bool Area::hasZone( VNumber vnum ) const
  {
    try
    {
      (void) zone( vnum );
      return true;
    }
    catch( XObjectNotFound& )
    {
      return false;
    }
  }

  void Area::removeZone( VNumber vnum )
  {
    zones_iterator it = m_zones.begin();
    while( it != m_zones.end() )
    {
      if( (*it).vnumber() == vnum )
      {
        qDebug( "%s removed from %s.", qPrintable( (*it).dumpObject() ), qPrintable( dumpObject() ) );
        m_zones.erase( it );
        return;
      }
      ++it;
    }
  }

  void Area::addZone( const Zone& z )
  {
    removeZone( z.vnumber() );
    m_zones.append( z );
    m_zonesSorted = false;
    qDebug( "%s added to %s.", qPrintable( z.dumpObject() ), qPrintable( dumpObject() ) );
  }

  void Area::sortZones()
  {
    if( m_zonesSorted )
      return;
    qSort( m_zones );
    m_zonesSorted = true;
    qDebug( "%s sorting zones completed.", qPrintable( dumpObject() ) );
  }

  bool Area::loadZones()
  {
    if( m_zonesFilePath.isEmpty() )
      m_zonesFilePath = Utils::createPath( Settings::zonesDirectory(), name(), Settings::zoneSuffix() );

    VNumber vnum;
    FILE *pFile;

    if( ( pFile = fopen( qPrintable( m_zonesFilePath ), "r" ) ) )
    {
      char szBuffer[ 82 ];

      while( fgets( szBuffer, sizeof( szBuffer ), pFile ) )
      {
        if( *szBuffer == '#' )
        {
          vnum = -1;

          sscanf( szBuffer, "#%ld", &vnum );

          if( vnum >= VNumberInvalid && vnum < VNumberTop )
          {
            Zone new_zone( vnum );
            new_zone.load( pFile );

            addZone( new_zone );
          }
          else
          {
            qWarning( "Invalid Zone VNumber in string '%s' reading file '%s'.",
              szBuffer, qPrintable( m_zonesFilePath ) );
          }
        }
      }
      fclose( pFile );
    }
    else
    {
      qWarning( "%s loading zones not started. Unable to open file '%s'.",
        qPrintable( dumpObject() ), qPrintable( m_zonesFilePath ) );
      return false;
    }

    qDebug( "%s loading %d zones completed. File '%s' read.",
        qPrintable( dumpObject() ), m_zones.size(), qPrintable( m_zonesFilePath ) );
    m_zonesSorted = false;
    sortZones();
    m_zonesChanged = false;

    return true;
  }

  bool Area::saveZones()
  {
    if( m_zones.empty() )
    {
      qDebug( "%s saving 0 zones completed. List is empty.", qPrintable( dumpObject() ) );
      return true;
    }

    m_zonesSorted = false;
    sortZones();

    if( m_zonesFilePath.isEmpty() )
    {
      qWarning( "Unable to save a zones file with an empty path." );
      return false;
    }

    QFile file( m_zonesFilePath );
    if( !file.open( QIODevice::WriteOnly ) )
    {
      qDebug( "%s saving zones not possible. Unable to open file '%s'.",
        qPrintable( dumpObject() ), qPrintable( m_zonesFilePath ) );
      return false;
    }

    zones_iterator it = m_zones.begin();
    QTextStream stream( &file );
    while( it != m_zones.end() )
    {
      (*it).save( stream );
      ++it;
    }

    stream << flush;
    file.close();
    qDebug( "%s saving %d zones completed. File '%s' written.",
        qPrintable( dumpObject() ), m_zones.size(), qPrintable( m_zonesFilePath ) );
    m_zonesChanged = false;
    return true;
  }

  static QString GetReactionCommnet( const ZoneCommand& zc )
  {
    QString sComment = "";

    if( zc.isMobHate() )
      sComment += "Odio";
    else if( zc.isMobFear() )
      sComment += "Paura";
    else
      return sComment;

    long arg_1 = zc.argument( ZoneCommand::Argument1 );
    long arg_2 = zc.argument( ZoneCommand::Argument2 );

    switch( arg_1 )
    {
    case MOB_REACTION_GENDER:
      {
        if( arg_2 == CHARACTER_GENDER_MALE )
          sComment += " del genere maschile";
        else if( arg_2 == CHARACTER_GENDER_FEMALE )
          sComment += " del genere femmine";
        else
          sComment += " di tutti";
      }
      break;
    case MOB_REACTION_RACE:
      sComment += " della razza ";
      sComment += ConstantName::characterRace( arg_2 );
      break;
    case MOB_REACTION_CLASS:
      sComment += " della classe ";
      sComment += ConstantName::characterClass( arg_2 );
      break;
    case MOB_REACTION_EVIL:
      sComment += " dell'allineamento <= ";
      sComment += QString::number( arg_2 );
      break;
    case MOB_REACTION_GOOD:
      sComment += " dell'allineamneto >= ";
      sComment += QString::number( arg_2 );
      break;
    case MOB_REACTION_VNUMBER:
      sComment += " del mob ";
      sComment += QString::number( arg_2 );
      break;
    default:
      qWarning( "Field sconosciuto nella reazione: %s", qPrintable( zc.toString(false) ) );
      sComment += " [field sconosciuto] ";
    }

    return sComment;
  }

  QString Area::generateComment( const ZoneCommand& zc ) const
  {
    QString sComment = "";

    if( !zc.isValidType() )
      return sComment;

    char c = zc.type().unicode();
    long arg_1 = zc.argument( ZoneCommand::Argument1 );
    long arg_2 = zc.argument( ZoneCommand::Argument2 );
    long arg_3 = zc.argument( ZoneCommand::Argument3 );

    switch( c )
    {
    case 'M':
      // M <if-flag> <mob vnum> <max #> <room vnum>
      {
          sComment.sprintf( "Carica il mob %ld (%s) nella locazione %ld (%s).",
            arg_1, qPrintable( mobName( arg_1 ) ), arg_3, qPrintable( roomName( arg_3 ) ) );
      }
      break;

    case 'C':
      // C <if-flag> <mob vnum> <max #> <room vnum>
      {
          sComment.sprintf( "Carica il seguace %ld (%s) nella locazione %ld (%s).",
            arg_1, qPrintable( mobName( arg_1 ) ), arg_3, qPrintable( roomName( arg_3 ) ) );
      }
      break;

    case 'K':
      // K <if-flag> <mob vnum> <room vnum>
      sComment.sprintf( "Rimuovi il mob %ld (%s) dalla locazione %ld (%s).",
        arg_1, qPrintable( mobName( arg_1 ) ), arg_2, qPrintable( roomName( arg_2 ) ) );
      break;

    case 'F':
    case 'H':
      // F <if-flag> <field> <operator>
      // H <if-flag> <field> <operator>
      sComment.sprintf( "%s.", qPrintable( GetReactionCommnet( zc ) ) );
      break;

    case 'P':
      // P <if-flag> <loaded object vnum> <max #> <container vnum>
      {
          sComment.sprintf( "Carica l'oggetto %ld (%s) nell'oggetto %ld (%s).",
            arg_1, qPrintable( itemName( arg_1 ) ), arg_3, qPrintable( itemName( arg_3 ) ) );
      }
      break;

    case 'E':
      // E <if-flag> <object vnum> <max #> <equipment position>
      {
          sComment.sprintf( "Indossa %s: %ld (%s).", qPrintable( ConstantName::equipmentPositionName( arg_3 ) ), arg_1, qPrintable( itemName( arg_1 ) ) );

      }
      break;

    case 'G':
      // G <if-flag> <object vnum> <max #>
      {
          sComment.sprintf( "In inventario: %ld (%s).", arg_1, qPrintable( itemName( arg_1 ) ) );

      }
      break;

    case 'O':
      // O <if-flag> <object vnum> <max #> <room vnum> <max room #>
      {
          sComment.sprintf( "Carica l'oggetto %ld (%s) nella locazione %ld (%s).",
            arg_1, qPrintable( itemName( arg_1 ) ), arg_3, qPrintable( roomName( arg_3 ) ) );
      }
      break;

    case 'R':
      // R <if-flag> <object vnum> <room vnum>
      sComment.sprintf( "Rimuovi l'oggetto %ld (%s) dalla locazione %ld (%s).",
        arg_1, qPrintable( itemName( arg_1 ) ), arg_2, qPrintable( roomName( arg_2 ) ) );
      break;

    case 'D':
      // D <if-flag> <room vnum> <exit #> <doorstate>
      sComment.sprintf( "Uscita '%s' della stanza %ld (%s): %s",
        qPrintable( ConstantName::exitDirection( arg_2 ).toLower() ), arg_1,
        qPrintable( roomName( arg_1 ) ), qPrintable( ConstantName::exitStatus( arg_3 ).toLower() ) );
      break;

    default:
      sComment = "";
    }

    return sComment;
  }

  VNumber Area::nextObjectInList( ObjectData::Type ot, VNumber prev_vnum ) const
  {
    if( prev_vnum == VNumberInvalid )
      return prev_vnum;

    if( ot == ObjectData::Object_Room )
    {
      rooms_const_iterator it = m_rooms.begin();
      while( it != m_rooms.end() )
      {
        if( (*it).vnumber() == prev_vnum )
        {
          ++it;
          if( it != m_rooms.end() )
            return (*it).vnumber();
          else
            return VNumberInvalid;
        }
        ++it;
      }
    }
    else if( ot == ObjectData::Object_Mob )
    {
      mobs_const_iterator it = m_mobs.begin();
      while( it != m_mobs.end() )
      {
        if( (*it).vnumber() == prev_vnum )
        {
          ++it;
          if( it != m_mobs.end() )
            return (*it).vnumber();
          else
            return VNumberInvalid;
        }
        ++it;
      }
    }
    else if( ot == ObjectData::Object_Item )
    {
      items_const_iterator it = m_items.begin();
      while( it != m_items.end() )
      {
        if( (*it).vnumber() == prev_vnum )
        {
          ++it;
          if( it != m_items.end() )
            return (*it).vnumber();
          else
            return VNumberInvalid;
        }
        ++it;
      }
    }
    else if( ot == ObjectData::Object_Zone )
    {
      zones_const_iterator it = m_zones.begin();
      while( it != m_zones.end() )
      {
        if( (*it).vnumber() == prev_vnum )
        {
          ++it;
          if( it != m_zones.end() )
            return (*it).vnumber();
          else
            return VNumberInvalid;
        }
        ++it;
      }
    }
    else if( ot == ObjectData::Object_Shop )
    {
      shops_const_iterator it = m_shops.begin();
      while( it != m_shops.end() )
      {
        if( (*it).vnumber() == prev_vnum )
        {
          ++it;
          if( it != m_shops.end() )
            return (*it).vnumber();
          else
            return VNumberInvalid;
        }
        ++it;
      }
    }

    return VNumberInvalid;
  }

  VNumber Area::prevObjectInList( ObjectData::Type ot, VNumber prev_vnum ) const
  {
    if( prev_vnum == VNumberInvalid )
      return prev_vnum;

    if( ot == ObjectData::Object_Room )
    {
      rooms_const_iterator it = m_rooms.begin();
      while( it != m_rooms.end() )
      {
        if( (*it).vnumber() == prev_vnum )
        {
          if( it != m_rooms.begin() )
          {
            --it;
            return (*it).vnumber();
          }
          else
            return VNumberInvalid;
        }
        ++it;
      }
    }
    else if( ot == ObjectData::Object_Mob )
    {
      mobs_const_iterator it = m_mobs.begin();
      while( it != m_mobs.end() )
      {
        if( (*it).vnumber() == prev_vnum )
        {
          if( it != m_mobs.begin() )
          {
            --it;
            return (*it).vnumber();
          }
          else
            return VNumberInvalid;
        }
        ++it;
      }
    }
    else if( ot == ObjectData::Object_Item )
    {
      items_const_iterator it = m_items.begin();
      while( it != m_items.end() )
      {
        if( (*it).vnumber() == prev_vnum )
        {
          if( it != m_items.begin() )
          {
            --it;
            return (*it).vnumber();
          }
          else
            return VNumberInvalid;
        }
        ++it;
      }
    }
    else if( ot == ObjectData::Object_Zone )
    {
      zones_const_iterator it = m_zones.begin();
      while( it != m_zones.end() )
      {
        if( (*it).vnumber() == prev_vnum )
        {
          if( it != m_zones.begin() )
          {
            --it;
            return (*it).vnumber();
          }
          else
            return VNumberInvalid;
        }
        ++it;
      }
    }
    else if( ot == ObjectData::Object_Shop )
    {
      shops_const_iterator it = m_shops.begin();
      while( it != m_shops.end() )
      {
        if( (*it).vnumber() == prev_vnum )
        {
          if( it != m_shops.begin() )
          {
            --it;
            return (*it).vnumber();
          }
          else
            return VNumberInvalid;
        }
        ++it;
      }
    }

    return VNumberInvalid;
  }

  VNumber Area::topRoomsVNumber() const
  {
    VNumber vnumRet = firstVNumber();
    rooms_const_iterator it = m_rooms.begin();
    while( it != m_rooms.end() )
    {
      if( (*it).vnumber() > vnumRet )
        vnumRet = (*it).vnumber();
      ++it;
    }
    return vnumRet;
  }

  VNumber Area::topItemsVNumber() const
  {
    VNumber vnumRet = firstVNumber();
    items_const_iterator it = m_items.begin();
    while( it != m_items.end() )
    {
      if( (*it).vnumber() > vnumRet )
        vnumRet = (*it).vnumber();
      ++it;
    }
    return vnumRet;
  }

  VNumber Area::topMobsVNumber() const
  {
    VNumber vnumRet = firstVNumber();
    mobs_const_iterator it = m_mobs.begin();
    while( it != m_mobs.end() )
    {
      if( (*it).vnumber() > vnumRet )
        vnumRet = (*it).vnumber();
      ++it;
    }
    return vnumRet;
  }

  VNumber Area::topShopsVNumber() const
  {
    VNumber vnumRet = firstVNumber();
    shops_const_iterator it = m_shops.begin();
    while( it != m_shops.end() )
    {
      if( (*it).vnumber() > vnumRet )
        vnumRet = (*it).vnumber();
      ++it;
    }
    return vnumRet;
  }

  VNumber Area::topZonesVNumber() const
  {
    VNumber vnumRet = VNumberInvalid;
    zones_const_iterator it = m_zones.begin();
    while( it != m_zones.end() )
    {
      if( (*it).vnumber() > vnumRet )
        vnumRet = (*it).vnumber();
      ++it;
    }
    return vnumRet;
  }

  VNumber Area::bottomRoomsVNumber() const
  {
    if( m_rooms.empty() )
      return VNumberInvalid;
    VNumber vnumRet = m_rooms.first().vnumber();
    rooms_const_iterator it = m_rooms.begin();
    while( it != m_rooms.end() )
    {
      if( (*it).vnumber() < vnumRet )
        vnumRet = (*it).vnumber();
      ++it;
    }
    return vnumRet;
  }

  VNumber Area::bottomItemsVNumber() const
  {
    if( m_items.empty() )
      return VNumberInvalid;
    VNumber vnumRet = m_items.first().vnumber();
    items_const_iterator it = m_items.begin();
    while( it != m_items.end() )
    {
      if( (*it).vnumber() < vnumRet )
        vnumRet = (*it).vnumber();
      ++it;
    }
    return vnumRet;
  }

  VNumber Area::bottomMobsVNumber() const
  {
    if( m_mobs.empty() )
      return VNumberInvalid;
    VNumber vnumRet = m_mobs.first().vnumber();
    mobs_const_iterator it = m_mobs.begin();
    while( it != m_mobs.end() )
    {
      if( (*it).vnumber() < vnumRet )
        vnumRet = (*it).vnumber();
      ++it;
    }
    return vnumRet;
  }

  VNumber Area::bottomZonesVNumber() const
  {
    if( m_zones.empty() )
      return VNumberInvalid;
    VNumber vnumRet = m_zones.first().vnumber();
    zones_const_iterator it = m_zones.begin();
    while( it != m_zones.end() )
    {
      if( (*it).vnumber() < vnumRet )
        vnumRet = (*it).vnumber();
      ++it;
    }
    return vnumRet;
  }

  VNumber Area::bottomShopsVNumber() const
  {
    if( m_shops.empty() )
      return VNumberInvalid;
    VNumber vnumRet = m_shops.first().vnumber();
    shops_const_iterator it = m_shops.begin();
    while( it != m_shops.end() )
    {
      if( (*it).vnumber() < vnumRet )
        vnumRet = (*it).vnumber();
      ++it;
    }
    return vnumRet;
  }

  int Area::countMobInits( VNumber vnum ) const
  {
    int iCount = 0;
    zones_const_iterator it = m_zones.begin();
    while( it != m_zones.end() )
    {
      iCount += (*it).countMobInits( vnum );
      ++it;
    }
    return iCount;
  }

  int Area::countItemInits( VNumber vnum ) const
  {
    int iCount = 0;
    zones_const_iterator it = m_zones.begin();
    while( it != m_zones.end() )
    {
      iCount += (*it).countItemInits( vnum );
      ++it;
    }
    return iCount;
  }

  int Area::changeRoomVNumber( VNumber old_vnum, VNumber new_vnum, bool in_comment_too )
  {
    int iRif = 0;

    if( hasRoom( old_vnum ) )
    {
      Room r = room( old_vnum );
      r.setVNumber( new_vnum );
      removeRoom( old_vnum );
      addRoom( r );
      iRif++;
    }

    rooms_iterator itr = m_rooms.begin();
    while( itr != m_rooms.end() )
    {
      if( (*itr).teleportToRoom() == old_vnum )
      {
        (*itr).setTeleportToRoom( new_vnum );
        iRif++;
      }

      Room::exits_iterator ite = (*itr).exits().begin();
      while( ite != (*itr).exits().end() )
      {
        if( (*ite).toRoom() == old_vnum )
        {
          (*ite).setToRoom( new_vnum );
          iRif++;
        }
        if( (*ite).fromRoom() == old_vnum )
        {
          (*ite).setFromRoom( new_vnum );
          iRif++;
        }
        ++ite;
      }
      ++itr;
    }

    shops_iterator its = m_shops.begin();
    while( its != m_shops.end() )
    {
      if( (*its).room() == old_vnum )
        (*its).setRoom( new_vnum );
      ++its;
    }

    zones_iterator itz = m_zones.begin();
    while( itz != m_zones.end() )
    {
      ZoneCommandList zcList = (*itz).commandList();
      ZoneCommandList::iterator itc = zcList.begin();
      while( itc != zcList.end() )
      {
        if( (*itc).isMobLoad() || (*itc).isMobFollower() || (*itc).isItemLoad() )
        {
          if( (*itc).argument( ZoneCommand::Argument3 ) == old_vnum )
          {
            (*itc).setArgument( ZoneCommand::Argument3, new_vnum );
            if( in_comment_too )
              (*itc).setComment( generateComment( *itc ) );
            iRif++;
          }
        }
        else if( (*itc).isDoorInit() )
        {
          if( (*itc).argument( ZoneCommand::Argument1 ) == old_vnum )
          {
            (*itc).setArgument( ZoneCommand::Argument1, new_vnum );
            if( in_comment_too )
              (*itc).setComment( generateComment( *itc ) );
            iRif++;
          }
        }
        ++itc;
      }
      (*itz).setCommandList( zcList );
      ++itz;
    }

    return iRif;
  }

  int Area::changeMobVNumber( VNumber old_vnum, VNumber new_vnum, bool in_comment_too )
  {
    int iRif = 0;

    if( hasMob( old_vnum ) )
    {
      Mob m = mob( old_vnum );
      m.setVNumber( new_vnum );
      removeMob( old_vnum );
      addMob( m );
      iRif++;
    }

    shops_iterator its = m_shops.begin();
    while( its != m_shops.end() )
    {
      if( (*its).keeper() == old_vnum )
        (*its).setKeeper( new_vnum );
      ++its;
    }

    zones_iterator itz = m_zones.begin();
    while( itz != m_zones.end() )
    {
      ZoneCommandList zcList = (*itz).commandList();
      ZoneCommandList::iterator itc = zcList.begin();
      while( itc != zcList.end() )
      {
        if( (*itc).isMobLoad() || (*itc).isMobFollower() )
        {
          if( (*itc).argument( ZoneCommand::Argument1 ) == old_vnum )
          {
            (*itc).setArgument( ZoneCommand::Argument1, new_vnum );
            if( in_comment_too )
              (*itc).setComment( generateComment( *itc ) );
            iRif++;
          }
        }
        else if( (*itc).isMobHate() || (*itc).isMobFear() )
        {
          if( (*itc).argument( ZoneCommand::Argument2 ) == old_vnum )
          {
            (*itc).setArgument( ZoneCommand::Argument2, new_vnum );
            if( in_comment_too )
              (*itc).setComment( generateComment( *itc ) );
            iRif++;
          }
        }
        ++itc;
      }
      (*itz).setCommandList( zcList );
      ++itz;
    }

    return iRif;
  }

  int Area::changeItemVNumber( VNumber old_vnum, VNumber new_vnum, bool in_comment_too )
  {
    int iRif = 0;

    if( hasItem( old_vnum ) )
    {
      Item i = item( old_vnum );
      i.setVNumber( new_vnum );
      removeItem( old_vnum );
      addItem( i );
      iRif++;
    }

    shops_iterator its = m_shops.begin();
    while( its != m_shops.end() )
    {
      for( int i = 0; i < SHOP_MAX_ITEMS; i++ )
      {
        if( (*its).itemToSell( i ) == old_vnum )
          (*its).setItemToSell( i, new_vnum );
      }
      ++its;
    }

    rooms_iterator itr = m_rooms.begin();
    while( itr != m_rooms.end() )
    {
      Room::exits_iterator ite = (*itr).exits().begin();
      while( ite != (*itr).exits().end() )
      {
        if( (*ite).doorKey() == old_vnum )
        {
          (*ite).setDoorKey( new_vnum );
          iRif++;
        }
        ++ite;
      }
      ++itr;
    }

    items_iterator iti = m_items.begin();
    while( iti != m_items.end() )
    {
      if( (*iti).isContainer() && (*iti).containerKey() == old_vnum )
      {
        (*iti).setContainerKey( new_vnum );
        iRif++;
      }
      ++iti;
    }

    zones_iterator itz = m_zones.begin();
    while( itz != m_zones.end() )
    {
      ZoneCommandList zcList = (*itz).commandList();
      ZoneCommandList::iterator itc = zcList.begin();
      while( itc != zcList.end() )
      {
        if( (*itc).isItemLoad()
          || (*itc).isMobEquip() || (*itc).isMobGive() )
        {
          if( (*itc).argument( ZoneCommand::Argument1 ) == old_vnum )
          {
            (*itc).setArgument( ZoneCommand::Argument1, new_vnum );
            if( in_comment_too )
              (*itc).setComment( generateComment( *itc ) );
            iRif++;
          }
        }
        else if( (*itc).isItemPut() )
        {
          if( (*itc).argument( ZoneCommand::Argument1 ) == old_vnum )
          {
            (*itc).setArgument( ZoneCommand::Argument1, new_vnum );
            if( in_comment_too )
              (*itc).setComment( generateComment( *itc ) );
            iRif++;
          }

          if( (*itc).argument( ZoneCommand::Argument3 ) == old_vnum )
          {
            (*itc).setArgument( ZoneCommand::Argument3, new_vnum );
            if( in_comment_too )
              (*itc).setComment( generateComment( *itc ) );
            iRif++;
          }
        }
        ++itc;
      }
      (*itz).setCommandList( zcList );
      ++itz;
    }

    return iRif;
  }

  int Area::changeZoneVNumber( VNumber old_vnum, VNumber new_vnum )
  {
    int iRif = 0;

    if( hasZone( old_vnum ) )
    {
      Zone z = zone( old_vnum );
      VNumber diff_vnum = z.maxVNumber() - z.minVNumber();
      z.setVNumber( new_vnum );
      z.setMaxVNumber( z.minVNumber() + diff_vnum );
      removeZone( old_vnum );
      addZone( z );
      iRif++;
    }

    rooms_iterator itr = m_rooms.begin();
    while( itr != m_rooms.end() )
    {
      if( (*itr).zone() == old_vnum )
      {
        (*itr).setZone( new_vnum );
        iRif++;
      }
      ++itr;
    }

    return iRif;
  }

  int Area::changeShopVNumber( VNumber old_vnum, VNumber new_vnum )
  {
    int iRif = 0;
    if( hasShop( old_vnum ) )
    {
      Shop s = shop( old_vnum );
      s.setVNumber( new_vnum );
      removeShop( old_vnum );
      addShop( s );
      iRif++;
    }
    return iRif;
  }

  int Area::removeRoomReferences( VNumber vnum )
  {
    int iRif = 0;

    if( hasRoom( vnum ) )
    {
      Room r = room( vnum );
      iRif += m_rooms.removeAll( r );
    }

    rooms_iterator itr = m_rooms.begin();
    while( itr != m_rooms.end() )
    {
      if( (*itr).teleportToRoom() == vnum )
      {
        (*itr).setTeleportToRoom( (*itr).vnumber() );
        iRif++;
      }

      Room::exits_iterator ite = (*itr).exits().begin();
      while( ite != (*itr).exits().end() )
      {
        if( (*ite).toRoom() == vnum )
        {
          ite = (*itr).exits().erase( ite );
          iRif++;
        }
        else
          ++ite;
      }
      ++itr;
    }

    shops_iterator its = m_shops.begin();
    while( its != m_shops.end() )
    {
      if( (*its).room() == vnum )
      {
        its = m_shops.erase( its );
        iRif++;
      }
      else
        ++its;
    }

    zones_iterator itz = m_zones.begin();
    bool increment_iterator = true;

    while( itz != m_zones.end() )
    {
      ZoneCommandList zcList = (*itz).commandList();
      ZoneCommandList::iterator itc = zcList.begin();
      while( itc != zcList.end() )
      {
        if( (*itc).isMobLoad() || (*itc).isMobFollower() || (*itc).isItemLoad() )
        {
          if( (*itc).argument( ZoneCommand::Argument3 ) == vnum )
          {
            itc = zcList.erase( itc );
            increment_iterator = false;
            iRif++;
          }
        }
        else if( (*itc).isDoorInit() )
        {
          if( (*itc).argument( ZoneCommand::Argument1 ) == vnum )
          {
            itc = zcList.erase( itc );
            increment_iterator = false;
            iRif++;
          }
        }

        if( increment_iterator )
          ++itc;
        increment_iterator = true;
      }

      zcList.resetCommandsId();
      (*itz).setCommandList( zcList );
      ++itz;
    }

    return iRif;
  }

  int Area::removeMobReferences( VNumber vnum )
  {
    int iRif = 0;

    if( hasMob( vnum ) )
    {
      Mob m = mob( vnum );
      iRif += m_mobs.removeAll( m );
    }

    shops_iterator its = m_shops.begin();
    while( its != m_shops.end() )
    {
      if( (*its).keeper() == vnum )
      {
        its = m_shops.erase( its );
        iRif++;
      }
      else
        ++its;
    }

    zones_iterator itz = m_zones.begin();
    bool increment_iterator = true;

    while( itz != m_zones.end() )
    {
      ZoneCommandList zcList = (*itz).commandList();
      ZoneCommandList::iterator itc = zcList.begin();
      while( itc != zcList.end() )
      {
        if( (*itc).isMobLoad() || (*itc).isMobFollower() )
        {
          if( (*itc).argument( ZoneCommand::Argument1 ) == vnum )
          {
            itc = zcList.erase( itc );
            increment_iterator = false;
            iRif++;
          }
        }
        else if( (*itc).isMobHate() || (*itc).isMobFear() )
        {
          if( (*itc).argument( ZoneCommand::Argument1 ) == MOB_REACTION_VNUMBER
            && (*itc).argument( ZoneCommand::Argument2 ) == vnum )
          {
            itc = zcList.erase( itc );
            increment_iterator = false;
            iRif++;
          }
        }

        if( increment_iterator )
          ++itc;
        increment_iterator = true;
      }

      zcList.resetCommandsId();
      (*itz).setCommandList( zcList );
      ++itz;
    }

    return iRif;
  }

  int Area::removeItemReferences( VNumber vnum )
  {
    int iRif = 0;

    if( hasItem( vnum ) )
    {
      Item i = item( vnum );
      iRif += m_items.removeAll( i );
    }

    items_iterator iti = m_items.begin();
    while( iti != m_items.end() )
    {
      if( (*iti).isContainer() && (*iti).containerKey() == vnum )
      {
        (*iti).setContainerKey( VNumberInvalid );
        iRif++;
      }
      ++iti;
    }

    rooms_iterator itr = m_rooms.begin();
    while( itr != m_rooms.end() )
    {
      Room::exits_iterator ite = (*itr).exits().begin();
      while( ite != (*itr).exits().end() )
      {
        if( (*ite).doorKey() == vnum )
        {
          (*ite).setDoorKey( VNumberInvalid );
          iRif++;
        }
        ++ite;
      }
      ++itr;
    }

    shops_iterator its = m_shops.begin();
    while( its != m_shops.end() )
    {
      for( int i = 0; i < SHOP_MAX_ITEMS; i++ )
      {
        if( (*its).itemToSell( i ) == vnum )
          (*its).setItemToSell( i, -1 );
      }
      ++its;
    }

    zones_iterator itz = m_zones.begin();
    bool increment_iterator = true;

    while( itz != m_zones.end() )
    {
      ZoneCommandList zcList = (*itz).commandList();
      ZoneCommandList::iterator itc = zcList.begin();
      while( itc != zcList.end() )
      {
        if( (*itc).isItemLoad()
          || (*itc).isMobEquip() || (*itc).isMobGive() )
        {
          if( (*itc).argument( ZoneCommand::Argument1 ) == vnum )
          {
            itc = zcList.erase( itc );
            increment_iterator = false;
            iRif++;
          }
        }
        else if( (*itc).isItemPut() )
        {
          if( (*itc).argument( ZoneCommand::Argument1 ) == vnum
            || (*itc).argument( ZoneCommand::Argument3 ) == vnum )
          {
            itc = zcList.erase( itc );
            increment_iterator = false;
            iRif++;
          }
        }

        if( increment_iterator )
          ++itc;
        increment_iterator = true;
      }

      zcList.resetCommandsId();
      (*itz).setCommandList( zcList );
      ++itz;
    }

    return iRif;
  }

  const Shop& Area::shop( VNumber vnum ) const
  {
    shops_const_iterator it = m_shops.begin();
    while( it != m_shops.end() )
    {
      if( (*it).vnumber() == vnum )
        return *it;
      ++it;
    }

    QString sException = "";
    sException.sprintf( "Shop #%ld not found.", vnum );
    throw XObjectNotFound( sException );
  }

  bool Area::hasShop( VNumber vnum ) const
  {
    try
    {
      (void) shop( vnum );
      return true;
    }
    catch( XObjectNotFound& )
    {
      return false;
    }
  }

  void Area::removeShop( VNumber vnum )
  {
    shops_iterator it = m_shops.begin();
    while( it != m_shops.end() )
    {
      if( (*it).vnumber() == vnum )
      {
        qDebug( "%s removed from %s.", qPrintable( (*it).dumpObject() ), qPrintable( dumpObject() ) );
        m_shops.erase( it );
        return;
      }
      ++it;
    }
  }

  void Area::addShop( const Shop& s )
  {
    removeShop( s.vnumber() );
    m_shops.append( s );
    m_shopsSorted = false;
    qDebug( "%s added to %s.", qPrintable( s.dumpObject() ), qPrintable( dumpObject() ) );
  }

  void Area::sortShops()
  {
    if( m_shopsSorted )
      return;
    qSort( m_shops );
    m_shopsSorted = true;
    qDebug( "%s sorting shops completed.", qPrintable( dumpObject() ) );
  }

  bool Area::loadShops()
  {
    if( m_shopsFilePath.isEmpty() )
      m_shopsFilePath = Utils::createPath( Settings::shopsDirectory(), name(), Settings::shopSuffix() );

    VNumber vnum;
    FILE *pFile;

    if( ( pFile = fopen( qPrintable( m_shopsFilePath ), "r" ) ) )
    {
      char szBuffer[ 82 ];

      while( fgets( szBuffer, sizeof( szBuffer ), pFile ) )
      {
        if( *szBuffer == '#' )
        {
          vnum = -1;

          sscanf( szBuffer, "#%ld", &vnum );

          if( vnum >= VNumberInvalid && vnum < VNumberTop )
          {
            Shop new_shop( vnum );
            new_shop.load( pFile );
            addShop( new_shop );
          }
          else
          {
            qWarning( "Invalid Shop VNumber in string '%s' reading file '%s'.",
              szBuffer, qPrintable( m_shopsFilePath ) );
          }
        }
      }

      fclose( pFile );
    }
    else
    {
      qWarning( "%s loading shops not started. Unable to open file '%s'.",
        qPrintable( dumpObject() ), qPrintable( m_shopsFilePath ) );
      return false;
    }

    qDebug( "%s loading %d shops completed. File '%s' read.",
        qPrintable( dumpObject() ), m_shops.size(), qPrintable( m_shopsFilePath ) );
    m_shopsSorted = false;
    sortShops();
    m_shopsChanged = false;
    return true;
  }

  bool Area::saveShops()
  {
    if( m_shops.empty() )
    {
      qDebug( "%s saving 0 shops completed. List is empty.", qPrintable( dumpObject() ) );
      return true;
    }

    m_shopsSorted = false;
    sortShops();

    if( m_shopsFilePath.isEmpty() )
    {
      qWarning( "Unable to save a shops file with an empty path." );
      return false;
    }

    QFile file( m_shopsFilePath );
    if( !file.open( QIODevice::WriteOnly ) )
    {
      qDebug( "%s saving shops not possible. Unable to open file '%s'.",
        qPrintable( dumpObject() ), qPrintable( m_shopsFilePath ) );
      return false;
    }

    QTextStream stream( &file );
    shops_iterator it = m_shops.begin();
    while( it != m_shops.end() )
    {
      (*it).save( stream );
      ++it;
    }

    stream << flush;
    file.close();
    qDebug( "%s saving %d shops completed. File '%s' written.",
        qPrintable( dumpObject() ), m_shops.size(), qPrintable( m_shopsFilePath ) );
    m_shopsChanged = false;
    return true;
  }

  QString Area::shopName( VNumber vnum ) const
  {
    if( !hasShop( vnum ) )
      return QString( "Negozio sconosciuto" );

    const Shop& s = shop( vnum );

    QString sRet = "";
    sRet.sprintf( "%s (di %s)", qPrintable( roomName( s.room() ) ), qPrintable( mobName( s.keeper() ) ) );
    return sRet;
  }

  bool Area::openReverseRoomExit( const Exit& ex, bool force_it )
  {
    qDebug( "%s tries to open the reverse exit of %s.", qPrintable( dumpObject() ), qPrintable( ex.dumpObject() ) );

    if( !hasRoom( ex.toRoom() ) )
    {
      qWarning( "%s cannot open the reverse exit of %s, target room not exists.",
        qPrintable( dumpObject() ), qPrintable( ex.dumpObject() ) );
      return false;
    }

    Room r = room( ex.toRoom() );
    Exit new_exit = ex.reversed();

    if( r.hasExit( new_exit.name() ) && !force_it )
    {
      qWarning( "%s cannot open the reverse exit of %s, target exit already exists.",
        qPrintable( dumpObject() ), qPrintable( ex.dumpObject() ) );
      return false;
    }

    qDebug( "The reverse exit %s is created.", qPrintable( new_exit.dumpObject() ) );
    r.addExit( new_exit );
    addRoom( r );
    sortRooms();
    return true;
  }

  VNumber Area::zoneReferenced( VNumber vnum ) const
  {
    if( vnum <= VNumberInvalid || m_zones.empty() )
      return ZoneVNumberInvalid;

    zones_const_iterator it = m_zones.begin();
    while( it != m_zones.end() )
    {
      if( vnum >= (*it).minVNumber() && vnum <= (*it).maxVNumber() )
        return (*it).vnumber();
      ++it;
    }

    return m_zones.first().vnumber();
  }

  int Area::assignZoneToRooms()
  {
    int iCount = 0;
    rooms_iterator it = m_rooms.begin();
    while( it != m_rooms.end() )
    {
      (*it).setZone( zoneReferenced( (*it).vnumber() ) );
      iCount++;
      ++it;
    }
    return iCount;
  }

  int Area::assignStatusToExits()
  {
    int iCount = 0;
    zones_iterator it = m_zones.begin();
    while( it != m_zones.end() )
    {
      ZoneCommandList zcl = (*it).commandList();
      ZoneCommandList::const_iterator zcl_it = zcl.begin();
      while( zcl_it != zcl.end() )
      {
        if( (*zcl_it).isDoorInit() )
        {
          try
          {
            Room r = room( (*zcl_it).argument( ZoneCommand::Argument1 ) );
            if( r.hasExit( (*zcl_it).argument( ZoneCommand::Argument2 ) ) )
            {
              Exit ex = r.exit( (*zcl_it).argument( ZoneCommand::Argument2 ) );
              ex.setStatus( (*zcl_it).argument( ZoneCommand::Argument3 ) );
              r.addExit( ex );
              addRoom( r );
              iCount++;
            }
          }
          catch( XObjectNotFound& )
          {
          }
        }
        ++zcl_it;
      }
      ++it;
    }
    sortRooms();
    return iCount;
  }

  ZoneCommand Area::doorCommand( const Exit& ex ) const
  {
    VNumber vnum_zone = zoneReferenced( ex.fromRoom() );
    if( hasZone( vnum_zone ) )
    {
      ZoneCommandList zcl = zone( vnum_zone ).commandList();
      return zcl.door( ex );
    }

    return ZoneCommand();
  }

  VNumber Area::firstVNumber() const
  {
    VNumber vRet = VNumberInvalid;
    zones_const_iterator it = m_zones.begin();
    while( it != m_zones.end() )
    {
      if( vRet < (*it).minVNumber() )
        vRet = (*it).minVNumber();
      ++it;
    }
    return vRet;
  }


  VNumber Area::firstFreeRoomVNumber(VNumber vn) const
  {
    VNumber vRet = VNumberInvalid;
    VNumber vStart = VNumberInvalid;
    VNumber vEnd = VNumberInvalid;
    zones_const_iterator it = m_zones.begin();

    while( it != m_zones.end() )
    {
      if( vn >= (*it).minVNumber() && vn <= (*it).maxVNumber()) {
        vStart = (*it).minVNumber();
        vEnd = (*it).maxVNumber();
        for (long i = vStart; i < vEnd; i++) {
            if (!this->hasRoom(i)) {
                vRet = (VNumber)i;
            }
        }
      }
      ++it;
    }
    if (vRet == VNumberInvalid) {
        vRet = this->newRoomsVNumber();
    }
    return vRet;
  }

} // namespace ts
