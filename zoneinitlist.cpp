
#include "zoneinitlist.h"
#include "exception.h"


namespace ts
{
  void ZoneInitList::reset()
  {
    if( !m_mobs.empty() )
      m_mobs.clear();
    if( !m_items.empty() )
      m_items.clear();
    if( !m_doors.empty() )
      m_doors.clear();
    m_lastMobInit = ZoneCommandGroup();
    m_lastItemInit = ZoneCommandGroup();
  }

  ZoneInitList::ZoneInitList()
  {
    reset();
  }

  ZoneInitList::~ZoneInitList()
  {
  }

  ZoneInitList::ZoneInitList( const ZoneInitList& zil )
  {
    m_mobs = zil.m_mobs;
    m_items = zil.m_items;
    m_doors = zil.m_doors;
    m_lastMobInit = zil.m_lastMobInit;
    m_lastItemInit = zil.m_lastItemInit;
  }

  ZoneInitList& ZoneInitList::operator=( const ZoneInitList& zil )
  {
    if( this != &zil )
    {
      m_mobs = zil.m_mobs;
      m_items = zil.m_items;
      m_doors = zil.m_doors;
      m_lastMobInit = zil.m_lastMobInit;
      m_lastItemInit = zil.m_lastItemInit;
    }
    return *this;
  }

  void ZoneInitList::setCommandList( const ZoneCommandList& zcl )
  {
    reset();

    ZoneCommandList::const_iterator it = zcl.begin();

    while( it != zcl.end() )
    {
      parseCommand( *it );
      ++it;
    }

    qDebug( "ZoneInitList: sorting Mob Inits..." );
    qSort( m_mobs );
    qDebug( "ZoneInitList: sorting Item Inits..." );
    qSort( m_items );
    qDebug( "ZoneInitList: sorting Door Inits..." );
    qSort( m_doors );
    qDebug( "ZoneInitList: sorting completed." );
  }

  void ZoneInitList::flushBuffer()
  {
    if( !m_lastMobInit.isNull() )
    {
      bool append_mob_as_follower = false;
      if( m_lastMobInit.parent().isMobFollower() )
      {
        if( m_mobs.empty() )
        {
          qWarning( "Error in ZoneInitList: follower found without a master... fixed!" );
          ZoneCommand zc = m_lastMobInit.parent();
          zc.setType( QChar( 'M' ) );
          m_lastMobInit.setParent( zc );
        }
        else
          append_mob_as_follower = true;
      }

      if( !append_mob_as_follower )
      {
        MobInit mi;
        mi.setMaster( m_lastMobInit );
        m_mobs.append( mi );
      }
      else
        m_mobs.last().addFollower( m_lastMobInit );

      m_lastMobInit.init();
    }

    if( !m_lastItemInit.isNull() )
    {
      m_items.append( m_lastItemInit );
      m_lastItemInit.init();
    }
  }

  void ZoneInitList::parseCommand( const ZoneCommand& zc )
  {
    if( zc.isMobLoad() || zc.isMobFollower() )
    {
      flushBuffer();
      m_lastMobInit.setParent( zc );
    }
    else if( zc.isMobFear() || zc.isMobHate() || zc.isMobEquip() || zc.isMobGive() )
    {
      m_lastMobInit.addSon( zc );
    }
    else if( zc.isItemPut() )
    {
      m_lastItemInit.addSon( zc );
    }
    else if( zc.isItemLoad() )
    {
      flushBuffer();
      m_lastItemInit.setParent( zc );
    }
    else if( zc.isDoorInit() )
    {
      flushBuffer();
      m_doors.append( zc );
    }
  }

  ZoneCommandList ZoneInitList::commandList() const
  {
    qDebug( "ZoneInitList::commandList() : creating command list..." );
    ZoneCommandList zcl;
    ZoneCommand cmd_comment;

    zcl.addCommand( 1 );  // Init the list

    zcl.appendComment( "Inizializzazione dei Mobs." );

    mobs_const_iterator itm = m_mobs.begin();
    while( itm != m_mobs.end() )
    {
      if( (*itm).isMobLoad() )
      {
        zcl.appendCommandGroup( (*itm).master() );
        if( (*itm).hasFollowers() )
        {
          MobInit::followers_const_iterator itf = (*itm).followersBegin();
          while( itf != (*itm).followersEnd() )
          {
            zcl.appendCommandGroup( *itf );
            ++itf;
          }
        }
      }
      ++itm;
    }

    zcl.appendComment( "Fine inizializzazione dei Mobs." );
    zcl.appendComment( "" );

    zcl.appendComment( "Inizializzazione degli Oggetti." );

    items_const_iterator iti = m_items.begin();
    while( iti != m_items.end() )
    {
      if( (*iti).parent().isItemLoad() )
        zcl.appendCommandGroup( *iti );
      ++iti;
    }

    zcl.appendComment( "Fine inizializzazione degli Oggetti." );
    zcl.appendComment( "" );
    zcl.appendComment( "Inizializzazione delle Porte." );

    doors_const_iterator itd = m_doors.begin();
    while( itd != m_doors.end() )
    {
      if( (*itd).isDoorInit() )
        zcl.appendCommand( *itd );
      ++itd;
    }

    zcl.appendComment( "Fine inizializzazione delle Porte." );
    //zcl.appendComment( "" );

    zcl.resetCommandsId();
    qDebug( "ZoneInitList::commandList() : command list created." );
    return zcl;
  }

}  // namespace ts
