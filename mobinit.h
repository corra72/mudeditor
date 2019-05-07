
#ifndef TS_MOBINIT_H
#define TS_MOBINIT_H

#include <QList>
#include "zonecommandgroup.h"



namespace ts
{
  class MobInit
  {
  public:
    void init();

    MobInit();
    ~MobInit();

    MobInit( const MobInit& );

    MobInit& operator=( const MobInit& );
    bool operator<( const MobInit& mi ) const { return m_master.parent().priority() < mi.m_master.parent().priority(); }
    bool operator>( const MobInit& mi ) const { return m_master.parent().priority() > mi.m_master.parent().priority(); }

    VNumber id() const { return m_master.id(); }
    ZoneCommandGroup master() const { return m_master; }

    void setMaster( const ZoneCommandGroup& new_value ) { m_master = new_value; }
    void setKill( const ZoneCommand& );

    void addFollower( const ZoneCommandGroup& );
    void removeFollower( VNumber );
    bool hasFollower( VNumber ) const;
    bool hasFollowers() const { return !m_followers.empty(); }
    const ZoneCommandGroup& follower( VNumber ) const;
    typedef QList<ZoneCommandGroup>::const_iterator followers_const_iterator;
    typedef QList<ZoneCommandGroup>::iterator followers_iterator;
    followers_const_iterator followersBegin() const { return m_followers.begin(); }
    followers_const_iterator followersEnd() const { return m_followers.end(); }

    bool isNull() const { return id() == VNumberInvalid; }
    bool isMobLoad() const { return m_master.parent().isMobLoad(); }

    int countMobs( VNumber ) const;
    int countItems( VNumber ) const;

    int setMaxMobs( VNumber vnum, int max) { return m_master.setMaxMobs(vnum, max); }

  private:
    ZoneCommandGroup m_master;
    QList<ZoneCommandGroup> m_followers;

  };

}  // namespace ts

#endif // TS_MOBINIT_H
