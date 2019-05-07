
#ifndef TS_ZONECOMMANDLIST_H
#define TS_ZONECOMMANDLIST_H

#include <QList>
#include <QTextStream>
#include "zonecommand.h"
#include "zonecommandgroup.h"


namespace ts
{
  class Exit;

  class ZoneCommandList
  {
    friend class ZoneInitList;

  public:
    ZoneCommandList();

    ZoneCommandList( const ZoneCommandList& );

    virtual ~ZoneCommandList();

    ZoneCommandList& operator=( const ZoneCommandList& );

    typedef QList<ZoneCommand>::const_iterator const_iterator;
    typedef QList<ZoneCommand>::iterator iterator;

    const_iterator begin() const { return m_commands.begin(); }
    const_iterator end() const { return m_commands.end(); }
    iterator begin() { return m_commands.begin(); }
    iterator end() { return m_commands.end(); }
    iterator erase( iterator itc ) { return m_commands.erase( itc ); }

    void addCommand( const ZoneCommand& );
    void addCommand( VNumber );
    void addComment( VNumber, const QString& );
    void removeCommand( VNumber );
    const ZoneCommand& command( VNumber ) const;
    bool hasCommand( VNumber ) const;
    VNumber topId() const;
    int count() const { return m_commands.size(); }
    void resetCommandsId();

    void load( FILE* );
    void save( QTextStream& ) const;

    int countMobs( VNumber ) const;
    int countItems( VNumber ) const;
    int setMaxMobs( VNumber, int );
    int setMaxItems( VNumber, int );

    void reset() { m_commands.clear(); }

    void addCommandGroup( ZoneCommandGroup );

    void trimEmptyCommands();
    int removeDoorCommands( VNumber* pStartLine = 0 );

    ZoneCommand door( const Exit& ) const;
    
    QList<ZoneCommand> cList() {return m_commands; }

  protected:
    void appendCommand( const ZoneCommand& );
    void appendComment( const QString& );
    void appendCommandGroup( ZoneCommandGroup );

  private:
    QList<ZoneCommand> m_commands;

  };
}

#endif // TS_ZONECOMMANDLIST_H
