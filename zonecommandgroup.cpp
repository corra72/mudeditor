
#include "zonecommandgroup.h"
#include "exception.h"

namespace ts
{
  void ZoneCommandGroup::init()
  {
    m_parent = ZoneCommand();
    if( !m_sons.empty() )
      m_sons.clear();
  }

  ZoneCommandGroup::ZoneCommandGroup()
  {
    init();
  }

  ZoneCommandGroup::ZoneCommandGroup( const ZoneCommandGroup& zcg )
  {
    m_parent = zcg.m_parent;
    m_sons = zcg.m_sons;
  }

  ZoneCommandGroup::~ZoneCommandGroup()
  {

  }

  ZoneCommandGroup& ZoneCommandGroup::operator=( const ZoneCommandGroup& zcg )
  {
    m_parent = zcg.m_parent;
    m_sons = zcg.m_sons;
    return *this;
  }

  void ZoneCommandGroup::addSon( const ZoneCommand& zc )
  {
    removeSon( zc.id() );
    m_sons.append( zc );
    qDebug( "%s added to parent %s.", qPrintable( zc.dumpObject() ), qPrintable( m_parent.dumpObject() ) );
  }

  void ZoneCommandGroup::removeSon( VNumber son_id )
  {
    if( son_id <= 0 )
      return;

    sons_iterator it = m_sons.begin();
    while( it != m_sons.end() )
    {
      if( son_id == (*it).id() )
      {
        qDebug( "%s removed from parent %s.", qPrintable( (*it).dumpObject() ), qPrintable( m_parent.dumpObject() ) );
        m_sons.erase( it );
        return;
      }
      ++it;
    }
  }

  const ZoneCommand& ZoneCommandGroup::son( VNumber son_id ) const
  {
    sons_const_iterator it = m_sons.begin();
    while( it != m_sons.end() )
    {
      if( son_id == (*it).id() )
        return *it;
      ++it;
    }

    QString sException = "";
    sException.sprintf( "ZoneCommand #%ld not found as son of %s.", son_id, m_parent.dumpObject().toUtf8().data() );
    throw XObjectNotFound( sException );
  }

  bool ZoneCommandGroup::hasSon( VNumber son_id ) const
  {
    try
    {
      (void) son( son_id );
      return true;
    }
    catch( XObjectNotFound& )
    {
      return false;
    }
  }

  int ZoneCommandGroup::countItems( VNumber item_vnum ) const
  {
    int iCount = 0;

    if( m_parent.isItemLoad() && m_parent.argument( ZoneCommand::Argument1 ) == item_vnum )
      iCount++;

    if( !m_sons.empty() )
    {
      sons_const_iterator it = m_sons.begin();
      while( it != m_sons.end() )
      {
        if( ((*it).isItemPut() || (*it).isMobEquip() || (*it).isMobGive() )
          && m_parent.argument( ZoneCommand::Argument1 ) == item_vnum )
          iCount++;
        ++it;
      }
    }
    return iCount;
  }

  int ZoneCommandGroup::countMobs( VNumber mob_vnum ) const
  {
    if( ( m_parent.isMobLoad() || m_parent.isMobFollower() ) && m_parent.argument( ZoneCommand::Argument1 ) == mob_vnum )
      return 1;
    else
      return 0;
  }

  int ZoneCommandGroup::setMaxMobs( VNumber mob_vnum, int max_mobs )
  {
    if( ( m_parent.isMobLoad() || m_parent.isMobFollower() ) && m_parent.argument( ZoneCommand::Argument1 ) == mob_vnum )
    {
      if( max_mobs > 0 )
        m_parent.setArgument( ZoneCommand::Argument2, max_mobs );
      return 1;
    }
    else
      return 0;
  }

  int ZoneCommandGroup::setMaxItems( VNumber item_vnum, int max_items )
  {
    int iCount = 0;

    if( m_parent.isItemLoad() && m_parent.argument( ZoneCommand::Argument1 ) == item_vnum )
    {
      if( max_items > 0 )
        m_parent.setArgument( ZoneCommand::Argument2, max_items );
      iCount++;
    }

    if( !m_sons.empty() )
    {
      sons_iterator it = m_sons.begin();
      while( it != m_sons.end() )
      {
        if( ((*it).isItemPut() || (*it).isMobEquip() || (*it).isMobGive() )
          && m_parent.argument( ZoneCommand::Argument1 ) == item_vnum )
        {
          if( max_items > 0 )
            (*it).setArgument( ZoneCommand::Argument2, max_items );
          iCount++;
        }
        ++it;
      }
    }
    return iCount;
  }

} // namespace ts
