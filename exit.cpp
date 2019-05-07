#include "exit.h"
#include <qtextstream.h>
#include "constantname.h"



namespace ts
{
  void Exit::init( const QString& exit_name, VNumber from_room )
  {
    setName( exit_name );
    m_listName = "";
    m_toName = "";
    m_fromName = "";
    m_revDirName = "";

    m_description = "";
    m_keys = "";

    m_doorKey = -1;
    m_openCommand = EXIT_OPEN_COMMAND_DEFAULT;
    m_flags = 0;

    m_toRoom = VNumberInvalid;
    m_fromRoom = from_room;

    m_status = EXIT_STATUS_OPENED;
  }

  Exit::Exit()
  {
    m_direction = EXIT_DIRECTION_INVALID;
    init( "default", VNumberInvalid );
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created (default).", dumpObject().toUtf8().data() );
#endif
  }

  Exit::Exit( int dir, VNumber from_room )
  {
    init( "default", from_room );
    setDirection( dir );
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created.", dumpObject().toUtf8().data() );
#endif
  }

  Exit::Exit( const QString& exit_name, VNumber from_room )
  {
    m_direction = EXIT_DIRECTION_INVALID;
    init( exit_name, from_room );
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created.", dumpObject().toUtf8().data() );
#endif
  }

  Exit::Exit( const Exit& ex )
  {
    m_direction = ex.m_direction;
    m_name = ex.m_name;
    m_listName = ex.m_listName;
    m_toName = ex.m_toName;
    m_fromName = ex.m_fromName;
    m_revDirName = ex.m_revDirName;

    m_description = ex.m_description;
    m_keys = ex.m_keys;

    m_doorKey = ex.m_doorKey;
    m_openCommand = ex.m_openCommand;
    m_flags = ex.m_flags;

    m_toRoom = ex.m_toRoom;
    m_fromRoom = ex.m_fromRoom;

    m_status = ex.m_status;

#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created (by copy).", dumpObject().toUtf8().data() );
#endif
  }

  Exit::~Exit()
  {
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s deleted.", dumpObject().toUtf8().data() );
#endif
  }

  Exit& Exit::operator=( const Exit& ex )
  {
    if( this != &ex )
    {
#if defined( TS_DEBUG_PANIC )
      qDebug( "%s overwritten by %s.", dumpObject().toUtf8().data(), ex.dumpObject().toUtf8().data() );
#endif
      m_direction = ex.m_direction;
      m_name = ex.m_name;
      m_listName = ex.m_listName;
      m_toName = ex.m_toName;
      m_fromName = ex.m_fromName;
      m_revDirName = ex.m_revDirName;

      m_description = ex.m_description;
      m_keys = ex.m_keys;

      m_doorKey = ex.m_doorKey;
      m_openCommand = ex.m_openCommand;
      m_flags = ex.m_flags;

      m_toRoom = ex.m_toRoom;
      m_fromRoom = ex.m_fromRoom;

      m_status = ex.m_status;
    }
    return *this;
  }

  bool Exit::operator<( const Exit& ex ) const
  {
	  if(this->m_direction < ex.m_direction)
		  return true;
	  else
		  return false;
  }

  void Exit::setDirection( int new_dir )
  {
    m_direction = new_dir;
    if( new_dir >= 0 && new_dir < EXIT_DIRECTION_END )
    {
      setName( ConstantName::exitDirection( new_dir ) );
      setListName( ConstantName::exitDirectionListName( new_dir ) );
      setToName( ConstantName::exitDirectionToName( new_dir ) );
      setFromName( ConstantName::exitDirectionFromName( new_dir ) );
      setRevDirName( ConstantName::exitReverseDirectionName( new_dir ) );
    }
  }

  QString Exit::dumpObject() const
  {
    QString sDump = "";
    sDump.sprintf( "Exit '%s' from #%ld to #%ld", m_name.toUtf8().data(), m_fromRoom, m_toRoom );
    return sDump;
  }

  static int GetOpenCommandToSave( int id )
  {
    switch( id )
    {
    case EXIT_OPEN_COMMAND_PULL:
      return CMD_PULL;
    case EXIT_OPEN_COMMAND_TWIST:
      return CMD_TWIST;
    case EXIT_OPEN_COMMAND_TURN:
      return CMD_TURN;
    case EXIT_OPEN_COMMAND_LIFT:
      return CMD_LIFT;
    case EXIT_OPEN_COMMAND_PUSH:
      return CMD_PUSH;
    case EXIT_OPEN_COMMAND_DIG:
      return CMD_DIG;
    case EXIT_OPEN_COMMAND_CUT:
      return CMD_CUT;
    default:
      return -1;
    }
  }

  static int GetOpenCommandFromLoad( int id )
  {
    switch( id )
    {
    case CMD_PULL:
      return EXIT_OPEN_COMMAND_PULL;
    case CMD_TWIST:
      return EXIT_OPEN_COMMAND_TWIST;
    case CMD_TURN:
      return EXIT_OPEN_COMMAND_TURN;
    case CMD_LIFT:
      return EXIT_OPEN_COMMAND_LIFT;
    case CMD_PUSH:
      return EXIT_OPEN_COMMAND_PUSH;
    case CMD_DIG:
      return EXIT_OPEN_COMMAND_DIG;
    case CMD_CUT:
      return EXIT_OPEN_COMMAND_CUT;
    default:
      return EXIT_OPEN_COMMAND_DEFAULT;
    }
  }

  void Exit::load( FILE* pFile )
  {
    setDescription( Utils::readString( pFile ) );
    if(m_description.startsWith('[') && m_description.endsWith(']'))
        setDescription("");
    m_keys = Utils::readString( pFile ).simplified();
    m_flags = Utils::readNumber( pFile, "Exit DoorFlags" );
    m_doorKey = Utils::readNumber( pFile, "Exit DoorKeyVNumber" );
    m_toRoom = Utils::readNumber( pFile, "Exit ToRoomVNumber" );
    m_openCommand = GetOpenCommandFromLoad( Utils::readNumber( pFile, "Exit OpenCommand" ) );
    qDebug( "%s loading completed.", dumpObject().toUtf8().data() );
  }

  void Exit::save( QTextStream& stream )
  {
    stream << "D";
    if( hasDirection() )
    {
      stream << (int)m_direction << endl << flush;
    }
    else
    {
      stream << " " << m_name << "~" << m_listName << "~" << m_toName;
      stream << "~" << m_fromName << "~" << m_revDirName << "~" << endl << flush;
    }
	if(!m_description.isEmpty())
		stream << m_description << endl;
	stream << "~" << endl << flush;

    stream << m_keys << "~" << endl << flush;
    Utils::saveBitVector( stream, m_flags );
    stream << " " << m_doorKey << " " << m_toRoom << " "
      << GetOpenCommandToSave( m_openCommand ) << endl << flush;
    qDebug( "%s saving completed.", dumpObject().toUtf8().data() );
  }

  int Exit::directionReversed() const
  {
    switch( m_direction )
    {
    case EXIT_DIRECTION_NORTH:
      return EXIT_REVERSE_DIRECTION_NORTH;
    case EXIT_DIRECTION_EAST:
      return EXIT_REVERSE_DIRECTION_EAST;
    case EXIT_DIRECTION_SOUTH:
      return EXIT_REVERSE_DIRECTION_SOUTH;
    case EXIT_DIRECTION_WEST:
      return EXIT_REVERSE_DIRECTION_WEST;
    case EXIT_DIRECTION_UP:
      return EXIT_REVERSE_DIRECTION_UP;
    case EXIT_DIRECTION_DOWN:
      return EXIT_REVERSE_DIRECTION_DOWN;
    default:
      return EXIT_REVERSE_DIRECTION_INVALID;
    }
  }

  Exit Exit::reversed() const
  {
    Exit new_exit( *this );
    new_exit.setFromRoom( toRoom() );
    new_exit.setToRoom( fromRoom() );
    if( !new_exit.hasDirection() )
    {
      new_exit.setName( revDirName() );
      new_exit.setRevDirName( name() );
    }
    else
      new_exit.setDirection( directionReversed() );

    return new_exit;
  }

  ZoneCommand Exit::zoneCommand() const
  {
    ZoneCommand zc;
    if( hasDirection() && hasDoor() && !isOpened() )
    {
      zc.setType( 'D' );
      zc.setArgument( ZoneCommand::Argument1, m_fromRoom );
      zc.setArgument( ZoneCommand::Argument2, m_direction );
      zc.setArgument( ZoneCommand::Argument3, m_status );
    }
    return zc;
  }

} // namespace ts
