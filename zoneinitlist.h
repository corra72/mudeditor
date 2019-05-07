
#ifndef TS_ZONEINITLIST_H
#define TS_ZONEINITLIST_H

#include "mobinit.h"
#include "zonecommandlist.h"


namespace ts
{

  class ZoneInitList
  {
  public:
    ZoneInitList();
    ~ZoneInitList();

    ZoneInitList( const ZoneInitList& );

    ZoneInitList& operator=( const ZoneInitList& );

    void reset();

    void setCommandList( const ZoneCommandList& );
    ZoneCommandList commandList() const;

    typedef QList<MobInit>::const_iterator mobs_const_iterator;
    typedef QList<MobInit>::iterator mobs_iterator;
    mobs_const_iterator mobsConstBegin() const { return m_mobs.constBegin(); }
    mobs_const_iterator mobsConstEnd() const { return m_mobs.constEnd(); }

    MobInit mobAt(int i) { return m_mobs.at(i); }
    int mobSize() { return m_mobs.size(); }

    mobs_iterator mobsBegin() { return m_mobs.begin(); }
    mobs_iterator mobsEnd()  { return m_mobs.end(); }

    typedef QList<ZoneCommandGroup>::const_iterator items_const_iterator;
    typedef QList<ZoneCommandGroup>::iterator items_iterator;
    items_const_iterator itemsBegin() const { return m_items.begin(); }
    items_const_iterator itemsEnd() const { return m_items.end(); }

    items_iterator itemsBegin()  { return m_items.begin(); }
    items_iterator itemsEnd() { return m_items.end(); }

    typedef QList<ZoneCommand>::const_iterator doors_const_iterator;
    typedef QList<ZoneCommand>::iterator doors_iterator;
    doors_const_iterator doorsBegin() const { return m_doors.begin(); }
    doors_const_iterator doorsEnd() const { return m_doors.end(); }

  protected:
    void parseCommand( const ZoneCommand& );
    void flushBuffer();

  private:
    QList<MobInit> m_mobs;
    QList<ZoneCommandGroup> m_items;
    QList<ZoneCommand> m_doors;

    ZoneCommandGroup m_lastMobInit;
    ZoneCommandGroup m_lastItemInit;

  };

}  // namespace TS

#endif // TS_ZONEINITLIST_H
