
#include "mobinit.h"
#include "exception.h"


namespace ts
{

  void MobInit::init()
  {
    m_master = ZoneCommandGroup();
    if( !m_followers.empty() )
      m_followers.clear();
  }

  MobInit::MobInit()
  {
    init();
  }

  MobInit::~MobInit()
  {

  }

  MobInit::MobInit( const MobInit& mi )
  {
    m_master = mi.m_master;
    m_followers = mi.m_followers;
  }

  MobInit& MobInit::operator=( const MobInit& mi )
  {
    if( this != &mi )
    {
      m_master = mi.m_master;
      m_followers = mi.m_followers;
    }
    return *this;
  }

  void MobInit::addFollower( const ZoneCommandGroup& zcg )
  {
    removeFollower( zcg.id() );
    m_followers.append( zcg );
  }

  void MobInit::removeFollower( VNumber zcg_id )
  {
    followers_iterator it = m_followers.begin();
    while( it != m_followers.end() )
    {
      if( (*it).id() == zcg_id )
      {
        m_followers.erase( it );
        return;
      }
      ++it;
    }
  }

  const ZoneCommandGroup& MobInit::follower( VNumber zcg_id ) const
  {
    followers_const_iterator it = m_followers.begin();
    while( it != m_followers.end() )
    {
      if( (*it).id() == zcg_id )
        return *it;
      ++it;
    }

    QString sException = "";
    sException.sprintf( "FollowerInit #%ld not found.", zcg_id );
    throw XObjectNotFound( sException );
  }

  bool MobInit::hasFollower( VNumber zcg_id ) const
  {
    try
    {
      (void) follower( zcg_id );
      return true;
    }
    catch( XObjectNotFound& )
    {
      return false;
    }
  }

  void MobInit::setKill( const ZoneCommand& zc )
  {
    ZoneCommandGroup zcg;
    zcg.setParent( zc );
    setMaster( zcg );
  }

  int MobInit::countMobs( VNumber mob_vnum ) const
  {
    int iCount = m_master.countMobs( mob_vnum );

    if( !m_followers.empty() )
    {
      followers_const_iterator it = m_followers.begin();
      while( it != m_followers.end() )
      {
        iCount += (*it).countMobs( mob_vnum );
        ++it;
      }
    }
    return iCount;
  }

  int MobInit::countItems( VNumber item_vnum ) const
  {
    int iCount = m_master.countItems( item_vnum );

    if( !m_followers.empty() )
    {
      followers_const_iterator it = m_followers.begin();
      while( it != m_followers.end() )
      {
        iCount += (*it).countItems( item_vnum );
        ++it;
      }
    }
    return iCount;
  }

}  // namespace ts
