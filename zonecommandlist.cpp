
#include "zonecommandlist.h"
#include "types.h"
#include "exception.h"
#include "exit.h"


namespace ts
{

  ZoneCommandList::ZoneCommandList()
  {

  }

  ZoneCommandList::ZoneCommandList( const ZoneCommandList& zcl )
  {
    m_commands = zcl.m_commands;
  }

  ZoneCommandList::~ZoneCommandList()
  {}

  ZoneCommandList& ZoneCommandList::operator=( const ZoneCommandList& zcl )
  {
    if( this != &zcl )
    {
      m_commands = zcl.m_commands;
    }
    return *this;
  }

  VNumber ZoneCommandList::topId() const
  {
    const_iterator it = m_commands.begin();
    VNumber iTop = 0;
    while( it != m_commands.end() )
    {
      if( iTop < (*it).id() )
        iTop = (*it).id();
      ++it;
    }
    return iTop;
  }

  void ZoneCommandList::resetCommandsId()
  {
    VNumber iId = 1;
    iterator it = m_commands.begin();
    while( it != m_commands.end() )
    {
      (*it).setId( iId );
      iId++;
      ++it;
    }
  }

  void ZoneCommandList::addCommand( const ZoneCommand& zc )
  {
    if( zc.id() <= VNumberInvalid )
    {
      m_commands.prepend( zc );
    }
    else if( zc.id() <= topId() )
    {
      iterator it = m_commands.begin();
      while( it != m_commands.end() )
      {
        if( (*it).id() >= zc.id()  )
        {
          m_commands.insert( it, zc );
          break;
        }
        ++it;
      }
    }
    else
    {
      m_commands.append( zc );
    }

    resetCommandsId();
  }

  void ZoneCommandList::addCommand( VNumber cid )
  {
    ZoneCommand zc;
    zc.setId( cid );

    addCommand( zc );
  }

  void ZoneCommandList::removeCommand( VNumber cid )
  {
    if( cid <= VNumberInvalid )
      return;

    iterator it = m_commands.begin();
    while( it != m_commands.end() )
    {
      if( (*it).id() == cid )
      {
        qDebug( "%s removed.", qPrintable( (*it).dumpObject() ) );
        m_commands.erase( it );
        resetCommandsId();
        return;
      }
      ++it;
    }
  }

  const ZoneCommand& ZoneCommandList::command( VNumber cid ) const
  {
    const_iterator it = m_commands.begin();
    while( it != m_commands.end() )
    {
      if( (*it).id() == cid )
        return *it;
      ++it;
    }

    QString sException = "";
    sException.sprintf( "ZoneCommand #%ld not found.", cid );
    throw XObjectNotFound( sException );
  }

  bool ZoneCommandList::hasCommand( VNumber cid ) const
  {
    try
    {
      (void) command( cid );
      return true;
    }
    catch( XObjectNotFound& )
    {
      return false;
    }
  }

  void ZoneCommandList::load( FILE* pFile )
  {
    int iNumLines = 0;

    while( !feof( pFile ) )
    {
      iNumLines++;
      ZoneCommand zc;
      zc.setId( iNumLines );
      zc.load( pFile );

      if( zc.isValidType() )
      {
        addCommand( zc );
        qDebug( "%s load.", qPrintable( zc.dumpObject() ) );
      }
      else if( zc.isZoneEnd() )
      {
        qDebug( "Loading commands completed." );
        return;
      }
      else
      {
        // skip
      }
    }

    qWarning( "Loading commands completed. End of zone not found." );
  }

  void ZoneCommandList::save( QTextStream& stream ) const
  {
    const_iterator it = m_commands.begin();
    while( it != m_commands.end() )
    {
      (*it).save( stream );
      ++it;
    }
  }

  void ZoneCommandList::trimEmptyCommands()
  {
    while( !m_commands.empty() )
    {
      ZoneCommand zc1 = m_commands.first();
      if( zc1.isEmptyCommand() )
        m_commands.removeFirst();
      else
        break;
    }

    while( !m_commands.empty() )
    {
      ZoneCommand zc1 = m_commands.last();
      if( zc1.isEmptyCommand() )
        m_commands.removeLast();
      else
        break;
    }
  }

  void ZoneCommandList::addComment( VNumber zcmd_id, const QString& zcmd_comment )
  {
    ZoneCommand zcmd;
    zcmd.setId( zcmd_id );
    zcmd.setComment( zcmd_comment );
    addCommand( zcmd );
  }

  void ZoneCommandList::appendCommand( const ZoneCommand& zcmd )
  {
    m_commands.append( zcmd );
  }

  void ZoneCommandList::appendComment( const QString& zcmd_comment )
  {
    ZoneCommand zcmd;
    zcmd.setComment( zcmd_comment );
    m_commands.append( zcmd );
  }

  void ZoneCommandList::appendCommandGroup( ZoneCommandGroup zcg )
  {
    m_commands.append( zcg.parent() );
    ZoneCommandGroup::sons_const_iterator its = zcg.sonsBegin();
    while( its != zcg.sonsEnd() )
    {
      m_commands.append( *its );
      ++its;
    }
    appendComment( "" );
  }

  int ZoneCommandList::countItems( VNumber item_vnum ) const
  {
    int iCount = 0;
    const_iterator it = m_commands.begin();
    while( it != m_commands.end() )
    {
      if( ( (*it).isItemLoad() || (*it).isItemPut() || (*it).isMobEquip() || (*it).isMobGive() )
        && (*it).argument( ZoneCommand::Argument1 ) == item_vnum )
        iCount++;
      ++it;
    }
    return iCount;
  }

  int ZoneCommandList::countMobs( VNumber mob_vnum ) const
  {
    int iCount = 0;
    const_iterator it = m_commands.begin();
    while( it != m_commands.end() )
    {
      if( ( (*it).isMobLoad() || (*it).isMobFollower() ) && (*it).argument( ZoneCommand::Argument1 ) == mob_vnum )
        iCount++;
      ++it;
    }
    return iCount;
  }

  int ZoneCommandList::setMaxMobs(  VNumber mob_vnum, int max_mobs )
  {
    int iCount = 0;
    iterator it = m_commands.begin();
    while( it != m_commands.end() )
    {
      if( ( (*it).isMobLoad() || (*it).isMobFollower() ) && (*it).argument( ZoneCommand::Argument1 ) == mob_vnum )
      {
         if( max_mobs > 0 )
          (*it).setArgument( ZoneCommand::Argument2, max_mobs );
        iCount++;
      }
      ++it;
    }
    return iCount;
  }

  int ZoneCommandList::setMaxItems( VNumber item_vnum, int max_items )
  {
    int iCount = 0;
    iterator it = m_commands.begin();
    while( it != m_commands.end() )
    {
      if( ( (*it).isItemLoad() || (*it).isItemPut() || (*it).isMobEquip() || (*it).isMobGive() )
        && (*it).argument( ZoneCommand::Argument1 ) == item_vnum )
      {
        if( max_items > 0 )
          (*it).setArgument( ZoneCommand::Argument2, max_items );
        iCount++;
      }
      ++it;
    }
    return iCount;
  }

  void ZoneCommandList::addCommandGroup( ZoneCommandGroup zcg )
  {
    if( zcg.id() <= 0 )
    {
      ZoneCommandGroup::sons_const_iterator its = zcg.sonsBegin();
      while( its != zcg.sonsEnd() )
      {
        m_commands.prepend( *its );
        ++its;
      }
      m_commands.prepend( zcg.parent() );
      resetCommandsId();
    }
    else if( zcg.id() > topId() )
    {
      m_commands.append( zcg.parent() );
      ZoneCommandGroup::sons_const_iterator its = zcg.sonsBegin();
      while( its != zcg.sonsEnd() )
      {
        m_commands.append( *its );
        ++its;
      }
      resetCommandsId();
    }
    else
    {
      int parent_id = zcg.parent().id();
      addCommand( zcg.parent() );
      ZoneCommandGroup::sons_const_iterator its = zcg.sonsBegin();
      while( its != zcg.sonsEnd() )
      {
        ZoneCommand zc = *its;
        zc.setId( ++parent_id );
        addCommand( zc );
        ++its;
      }
    }
  }

  int ZoneCommandList::removeDoorCommands( VNumber* pStartLine )
  {
    int iCount = 0;
    bool is_first = true;
    resetCommandsId();
    iterator it = m_commands.begin();
    while( it != m_commands.end() )
    {
      if( (*it).isDoorInit() )
      {
        if( is_first && pStartLine )
        {
          *pStartLine = (*it).id();
          is_first = false;
        }

        it = m_commands.erase( it );
        iCount++;
      }
      else
        ++it;
    }
    return iCount;
  }

  ZoneCommand ZoneCommandList::door( const Exit& ex ) const
  {
    const_iterator it = m_commands.begin();
    while( it != m_commands.end() )
    {
      if( (*it).isDoorInit() && ex.fromRoom() == (*it).argument( ZoneCommand::Argument1 )
        && ex.direction() == (*it).argument( ZoneCommand::Argument2 ) )
        return (*it);
      ++it;
    }
    return ZoneCommand();
  }

} // namespace ts

