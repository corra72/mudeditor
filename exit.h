
#ifndef TS_EXIT_H
#define TS_EXIT_H

#include <QString>
#include <QTextStream>
#include <stdio.h>
#include "types.h"
#include "utils.h"
#include "constants.h"
#include "bit.h"
#include "zonecommand.h"


namespace ts
{

  class Exit
  {
  public:
    void init( const QString& exit_name, VNumber from_room );

    Exit();
    Exit( int, VNumber from_room );
    Exit( const QString& exit_name, VNumber from_room );
    Exit( const Exit& );

    ~Exit();

    Exit& operator=( const Exit& );
	bool operator<( const Exit& ) const;

    int direction() const { return m_direction; }
    QString name() const { return m_name; }
    QString listName() const { return m_listName; }
    QString toName() const { return m_toName; }
    QString fromName() const { return m_fromName; }
    QString revDirName() const { return m_revDirName; }
    QString description() const { return m_description; }
    QString keys() const { return m_keys; }
    VNumber doorKey() const { return m_doorKey; }
    int openCommand() const { return m_openCommand; }
    BitVector flags() const { return m_flags; }
    VNumber toRoom() const { return m_toRoom; }
    VNumber fromRoom() const { return m_fromRoom; }
    bool hasFlag( int flag ) const { return Bit::has( m_flags, flag ); }
    bool hasKeyHole() const { return m_doorKey != -1; }
    bool hasDoor() const { return Bit::has( m_flags, EXIT_FLAG_ISDOOR ); }
    bool isFake() const { return m_toRoom == 0; }
    bool hasDirection() const { return (m_direction >= EXIT_DIRECTION_NORTH && m_direction <= EXIT_DIRECTION_DOWN); }
    int status() const { return m_status; }
    bool isOpened() const { return m_status == EXIT_STATUS_OPENED; }
    bool isClosed() const { return m_status == EXIT_STATUS_CLOSED; }
    bool isLocked() const { return m_status == EXIT_STATUS_LOCKED; }

    QString dumpObject() const;

    void setDirection( int );
    void setName( const QString& new_txt ) { m_name = Utils::removeTilde( new_txt.trimmed().toLower() ); }
    void setListName( const QString& new_txt ) { m_listName = Utils::removeTilde( new_txt.simplified() ); }
    void setToName( const QString& new_txt ) { m_toName = Utils::removeTilde( new_txt.simplified() ); }
    void setFromName( const QString& new_txt ) { m_fromName = Utils::removeTilde( new_txt.simplified() ); }
    void setRevDirName( const QString& new_txt ) { m_revDirName = Utils::removeTilde( new_txt.simplified() ); }
    void setDescription( const QString& new_txt ) { m_description = Utils::removeTilde( Utils::stripSpaceAtEnd( new_txt ) ); }
    void setKeys( const QString& new_txt ) { m_keys = Utils::removeTilde( new_txt.simplified() ); }
    void setDoorKey( VNumber new_vnum ) { m_doorKey = new_vnum; }
    void setOpenCommand( int new_cmd ) { m_openCommand = new_cmd; }
    void setFlags( BitVector new_flags ) { m_flags = new_flags; }
    void setToRoom( VNumber new_vnum ) { m_toRoom = new_vnum; }
    void setFromRoom( VNumber new_vnum ) { m_fromRoom = new_vnum; }
    void setStatus( int new_value ) { m_status = new_value; }

    void load( FILE* );
    void save( QTextStream& );

    int directionReversed() const;
    Exit reversed() const;
    ZoneCommand zoneCommand() const;

  protected:
    int m_direction;
    QString m_name;
    QString m_listName;
    QString m_toName;
    QString m_fromName;
    QString m_revDirName;

    QString m_description;
    QString m_keys;

    VNumber m_doorKey;
    int m_openCommand;
    BitVector m_flags;

    VNumber m_toRoom;
    VNumber m_fromRoom;

    int m_status;

  };

} // namespace ts


#endif // TS_EXIT_H

