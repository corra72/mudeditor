
#ifndef TS_ZONECOMMANDGROUP_H
#define TS_ZONECOMMANDGROUP_H

#include <QList>
#include "zonecommand.h"



namespace ts
{

  class ZoneCommandGroup
  {
  public:
    void init();

    ZoneCommandGroup();
    ZoneCommandGroup( const ZoneCommandGroup& );

    virtual ~ZoneCommandGroup();

    ZoneCommandGroup& operator=( const ZoneCommandGroup& );
    // Useful only in MobInits and ItemInits
    bool operator<( const ZoneCommandGroup& zcg ) const { return parent().priority() < zcg.parent().priority(); }
    bool operator>( const ZoneCommandGroup& zcg ) const { return parent().priority() > zcg.parent().priority(); }

    VNumber id() const { return m_parent.id(); }

    const ZoneCommand& parent() const { return m_parent; }
    void setParent( const ZoneCommand& new_value ) { m_parent = new_value; }

    typedef QList<ZoneCommand>::const_iterator sons_const_iterator;
    typedef QList<ZoneCommand>::iterator sons_iterator;
    sons_const_iterator sonsBegin() const { return m_sons.begin(); }
    sons_const_iterator sonsEnd() const { return m_sons.end(); }

    const QList<ZoneCommand>& sons() const { return m_sons; }
    void addSon( const ZoneCommand& );
    void removeSon( VNumber );
    const ZoneCommand& son( VNumber ) const;
    bool hasSon( VNumber ) const;
    int numSons() const { return m_sons.size(); }

    bool isNull() const { return id() == VNumberInvalid; }

    int countItems( VNumber ) const;
    int countMobs( VNumber ) const;
    int setMaxMobs( VNumber, int );
    int setMaxItems( VNumber, int );

  protected:
    ZoneCommand m_parent;
    QList<ZoneCommand> m_sons;

  };

}

#endif // TS_ZONECOMMANDGROUP_H
