
#ifndef TS_ROOM_H
#define TS_ROOM_H

#include <QList>
#include <QTextStream>
#include <stdio.h>
#include "objectdata.h"
#include "extradescription.h"
#include "exit.h"
#include "bit.h"


namespace ts
{

  class Room : public ObjectData
  {
  public:
    void init();
    void validate();

    Room();
    explicit Room( VNumber, const QString& room_name = "" );
    Room( const Room& );

    virtual ~Room();

    Room& operator=( const Room& );

    const QString& description() const { return m_description; }
    VNumber zone() const { return m_zone; }
    BitVector flags() const { return m_flags; }
    int sectorType() const { return m_sectorType; }
    int realSectorType() const { return m_sectorType != ROOM_SECTOR_TELEPORT ? m_sectorType : m_teleportSectorType; }
    int tunnelLimit() const { return m_tunnelLimit; }
    int teleportTime() const { return m_teleportTime; }
    int teleportToRoom() const { return m_teleportToRoom; }
    BitVector teleportFlags() const { return m_teleportFlags; }
    int teleportCount() const { return m_teleportCount; }
    int teleportSectorType() const { return m_teleportSectorType; }
    int riverDir() const { return m_riverDir; }
    int riverSpeed() const { return m_riverDir; }

    void setDescription( const QString& new_description ) { m_description = Utils::removeTilde( Utils::stripSpaceAtEnd( new_description ) ); }
    void setZone( VNumber new_zone_vnumber ) { m_zone = new_zone_vnumber; }
    void setFlags( BitVector new_flags ) { m_flags = new_flags; }
    void setSectorType( int new_sector_type ) { m_sectorType = new_sector_type; }
    void setTunnelLimit( int new_tunnel_limit ) { m_tunnelLimit = new_tunnel_limit; }
    void setTeleportTime( int new_teleport_time ) { m_teleportTime = new_teleport_time; }
    void setTeleportToRoom( VNumber new_teleport_to_room ) { m_teleportToRoom = new_teleport_to_room; }
    void setTeleportFlags( BitVector new_teleport_flags ) { m_teleportFlags = new_teleport_flags; }
    void setTeleportCount( int new_teleport_count ) { m_teleportCount = new_teleport_count; }
    void setTeleportSectorType( int new_teleport_sector_type ) { m_teleportSectorType = new_teleport_sector_type; }
    void setRiverDir( int new_river_dir ) { m_riverDir = new_river_dir; }
    void setRiverSpeed( int new_river_speed ) { m_riverSpeed = new_river_speed; }

    bool isSectorType( int st ) const { return m_sectorType == st; }
    bool isRealSectorType( int st ) const { return st == realSectorType(); }
    bool hasFlag( int flag ) const { return Bit::has( m_flags, flag ); }
    bool isTeleport() const { return isSectorType( ROOM_SECTOR_TELEPORT ); }
    bool hasTeleportFlag( int flag ) const { return Bit::has( m_teleportFlags, flag ); }
    bool isRiver() const { return isRealSectorType( ROOM_SECTOR_WATER_NO_SWIM ) || isRealSectorType( ROOM_SECTOR_UNDERWATER ); }
    bool isDeathRoom() const { return hasFlag( ROOM_FLAG_DEATH ); }

    typedef QList<ExtraDescription>::const_iterator extra_description_const_iterator;
    typedef QList<ExtraDescription>::iterator extra_description_iterator;
    const QList<ExtraDescription>& extraDescriptions() const { return m_extraDescriptions; }
    bool hasExtraDescription( const QString& extra_keys ) const;
    const ExtraDescription& extraDescription( const QString& extra_keys ) const;
    void removeExtraDescription( const QString& extra_keys );
    void addExtraDescription( const ExtraDescription& );

    typedef QList<Exit>::const_iterator exits_const_iterator;
    typedef QList<Exit>::iterator exits_iterator;
    const QList<Exit>& exits() const { return m_exits; }
    QList<Exit>& exits() { return m_exits; } // useful for change vnumbers
    bool hasExit( const QString& exit_name ) const;
    bool hasExit( int ) const;
    const Exit& exit( const QString& exit_name ) const;
    const Exit& exit( int ) const;
    void removeExit( const QString& exit_name );
    void addExit( Exit );

    void load( FILE* );
    void loadExit( FILE*, const QString& );
    void loadExtraDescription( FILE* );
    void save( QTextStream& );

  protected:
    QString m_description;
    VNumber m_zone;
    BitVector m_flags;
    int m_sectorType;
    int m_tunnelLimit;
    /* teleport */
    int m_teleportTime;
    VNumber m_teleportToRoom;
    BitVector m_teleportFlags;
    int m_teleportCount;
    int m_teleportSectorType;
    /* river */
    int m_riverDir;
    int m_riverSpeed;
    /* lists */
    QList<ExtraDescription> m_extraDescriptions;
    QList<Exit> m_exits;

  };

} // namespace ts

#endif // TS_ROOM_H

