
#ifndef TS_ZONE_H
#define TS_ZONE_H

#define USE_ZONE_COMMAND_LIST_AS_MEMBER


#include <QList>
#include <QTextStream>
#include "objectdata.h"
#include "zonecommandgroup.h"
#include "zonecommandlist.h"
#if !defined( USE_ZONE_COMMAND_LIST_AS_MEMBER )
#include "zoneinitlist.h"
#endif
#include "bit.h"




namespace ts
{

  class Zone : public ObjectData
  {
  public:
    void init();
    void validate();

    Zone();
    explicit Zone( VNumber, const QString& zone_name = "" );
    Zone( const Zone& );

    virtual ~Zone();

    Zone& operator=( const Zone& );

#if defined( USE_ZONE_COMMAND_LIST_AS_MEMBER )
    const ZoneCommandList& commandList() const { return m_initList; }
    void setCommandList( const ZoneCommandList& zcl ) { m_initList = zcl; }
#else
    ZoneCommandList commandList() const { return m_initList.commandList(); }
    void setCommandList( const ZoneCommandList& zcl ) { m_initList.setCommandList( zcl ); }
    ZoneInitList initList() const { return m_initList; }
    void setInitList( const ZoneInitList& zil ) { m_initList = zil; }
#endif

    VNumber minVNumber() const { return vnumber() * 100; }
    VNumber maxVNumber() const { return m_maxVNumber; }
    void setMaxVNumber( VNumber new_value ) { m_maxVNumber = new_value; }

    BitVector flags() const { return m_flags; }
    bool hasFlag( int flag ) const { return Bit::has( m_flags, flag ); }
    void setFlag( int flag ) { Bit::set( &m_flags, flag ); }
    void removeFlag( int flag ) { Bit::remove( &m_flags, flag ); }
    void setFlags( BitVector new_value ) { m_flags = new_value; }

    BitVector newFlags() const { return m_newFlags; }
    bool hasNewFlag( int flag ) const { return Bit::has( m_newFlags, flag ); }
    void setNewFlag( int flag ) { Bit::set( &m_newFlags, flag ); }
    void removeNewFlag( int flag ) { Bit::remove( &m_newFlags, flag ); }
    void setNewFlags( BitVector new_value ) { m_newFlags = new_value; }

    int repopMode() const { return m_repopMode; }
    void setRepopMode( int repop_mode ) { m_repopMode = repop_mode; }

    int repopInterval() const { return m_repopInterval; }
    void setRepopInterval( int new_value ) { m_repopInterval = new_value; }

    void loadHeaderOnly( FILE* );
    void load( FILE* );
    void save( QTextStream& );

    int countMobInits( VNumber ) const;
    int countItemInits( VNumber ) const;
    int setMaxMobInits( VNumber, int );
    int setMaxItemInits( VNumber, int );

    int pwpLevel() const { return m_pwpLevel; }
    void setPwpLevel(int lvl) { m_pwpLevel = lvl; }

  protected:
#if defined( USE_ZONE_COMMAND_LIST_AS_MEMBER )
    ZoneCommandList m_initList;
#else
    ZoneInitList m_initList;
#endif
    VNumber m_maxVNumber;
    BitVector m_flags;
    BitVector m_newFlags;
    int m_repopInterval;
    int m_repopMode;
    int m_pwpLevel;

  };

} // namespace ts

#endif // TS_ZONE_H
