
#ifndef TS_AREA_H
#define TS_AREA_H

#include <QList>
#include "room.h"
#include "mob.h"
#include "item.h"
#include "zone.h"
#include "shop.h"



namespace ts
{

  class Area
  {
  public:
    void init();

    Area();
    explicit Area( const QString& );
    Area( const Area& );

    virtual ~Area();

    Area& operator=( const Area& );

    const QString& name() const { return m_name; }
    void setName( const QString& new_name ) { m_name = new_name; }

    QString roomName( VNumber vnum ) const { return hasRoom( vnum ) ? room( vnum ).name() : "stanza sconusciuta"; }
    QString itemName( VNumber vnum ) const { return hasItem( vnum ) ? item( vnum ).shortDescription() : "oggetto sconusciuto"; }
    QString mobName( VNumber vnum ) const { return hasMob( vnum ) ? mob( vnum ).shortDescription() : "mob sconusciuto"; }
    QString zoneName( VNumber vnum ) const { return hasZone( vnum ) ? zone( vnum ).name() : "zona sconusciuta"; }
    QString shopName( VNumber ) const;
    QString generateComment( const ZoneCommand& ) const;
    VNumber nextObjectInList( ObjectData::Type, VNumber ) const;
    VNumber prevObjectInList( ObjectData::Type, VNumber ) const;

    QString dumpObject() const;

    const QString& roomsFilePath() const { return m_roomsFilePath; }
    const QString& mobsFilePath() const { return m_mobsFilePath; }
    const QString& itemsFilePath() const { return m_itemsFilePath; }
    const QString& zonesFilePath() const { return m_zonesFilePath; }
    const QString& shopsFilePath() const { return m_shopsFilePath; }

    void setRoomsFilePath( const QString& new_value ) { m_roomsFilePath = new_value; }
    void setMobsFilePath( const QString& new_value ) { m_mobsFilePath = new_value; }
    void setItemsFilePath( const QString& new_value ) { m_itemsFilePath = new_value; }
    void setZonesFilePath( const QString& new_value ) { m_zonesFilePath = new_value; }
    void setNotesFilePath( const QString& new_value ) { m_notesFilePath = new_value; }
    void setShopsFilePath( const QString& new_value ) { m_shopsFilePath = new_value; }
    void resetFilePaths();

    typedef QList<Room>::const_iterator rooms_const_iterator;
    typedef QList<Room>::iterator rooms_iterator;
    const QList<Room>& rooms() const { return m_rooms; }
    const Room& room( VNumber ) const;
    bool hasRoom( VNumber ) const;
    void removeRoom( VNumber );
    void addRoom( const Room& );
    void sortRooms();
    bool loadRooms();
    bool saveRooms();

    typedef QList<Mob>::const_iterator mobs_const_iterator;
    typedef QList<Mob>::iterator mobs_iterator;
    const QList<Mob>& mobs() const { return m_mobs; }
    const Mob& mob( VNumber ) const;
    bool hasMob( VNumber ) const;
    void removeMob( VNumber );
    void addMob( const Mob& );
    void sortMobs();
    bool loadMobs();
    bool saveMobs();

    typedef QList<Item>::const_iterator items_const_iterator;
    typedef QList<Item>::iterator items_iterator;
    const QList<Item>& items() const { return m_items; }
    const Item& item( VNumber ) const;
    bool hasItem( VNumber ) const;
    void removeItem( VNumber );
    void addItem( const Item& );
    void sortItems();
    bool loadItems();
    bool saveItems();

    typedef QList<Shop>::const_iterator shops_const_iterator;
    typedef QList<Shop>::iterator shops_iterator;
    const QList<Shop>& shops() const { return m_shops; }
    const Shop& shop( VNumber ) const;
    bool hasShop( VNumber ) const;
    void removeShop( VNumber );
    void addShop( const Shop& );
    void sortShops();
    bool loadShops();
    bool saveShops();

    typedef QList<Zone>::const_iterator zones_const_iterator;
    typedef QList<Zone>::iterator zones_iterator;
    const QList<Zone>& zones() const { return m_zones; }
    const Zone& zone( VNumber ) const;
    bool hasZone( VNumber ) const;
    void removeZone( VNumber );
    void addZone( const Zone& );
    void sortZones();
    bool loadZones();
    bool saveZones();

    void setRoomsChanged() { m_roomsChanged = true; }
    void setZonesChanged() { m_zonesChanged = true; }
    void setItemsChanged() { m_itemsChanged = true; }
    void setMobsChanged() { m_mobsChanged = true; }
    void setShopsChanged() { m_shopsChanged = true; }

    bool isChanged() const { return m_shopsChanged || m_roomsChanged || m_zonesChanged || m_itemsChanged || m_mobsChanged; }

    VNumber topRoomsVNumber() const;
    VNumber topItemsVNumber() const;
    VNumber topMobsVNumber() const;
    VNumber topShopsVNumber() const;
    VNumber topZonesVNumber() const;
    VNumber firstVNumber() const;
    VNumber firstFreeRoomVNumber(VNumber vn) const;
    VNumber newRoomsVNumber() const { return m_rooms.empty() ? firstVNumber() : topRoomsVNumber() + 1; }
    VNumber newItemsVNumber() const { return m_items.empty() ? firstVNumber() : topItemsVNumber() + 1; }
    VNumber newMobsVNumber() const { return m_mobs.empty() ? firstVNumber() : topMobsVNumber() + 1; }
    VNumber newShopsVNumber() const { return m_shops.empty() ? firstVNumber() : topShopsVNumber() + 1; }
    VNumber newZonesVNumber() const { return topZonesVNumber() + 1; }
    VNumber bottomRoomsVNumber() const;
    VNumber bottomItemsVNumber() const;
    VNumber bottomMobsVNumber() const;
    VNumber bottomZonesVNumber() const;
    VNumber bottomShopsVNumber() const;

    int countMobInits( VNumber ) const;
    int countItemInits( VNumber ) const;

    int changeRoomVNumber( VNumber old_vnum, VNumber new_vnum, bool in_comment_too );
    int changeMobVNumber( VNumber old_vnum, VNumber new_vnum, bool in_comment_too );
    int changeItemVNumber( VNumber old_vnum, VNumber new_vnum, bool in_comment_too );
    int changeZoneVNumber( VNumber old_vnum, VNumber new_vnum );
    int changeShopVNumber( VNumber old_vnum, VNumber new_vnum );
    int removeRoomReferences( VNumber );
    int removeMobReferences( VNumber );
    int removeItemReferences( VNumber );

    VNumber zoneReferenced( VNumber ) const;
    bool openReverseRoomExit( const Exit&, bool force_it );
    ZoneCommand doorCommand( const Exit& ) const;
    int assignZoneToRooms();
    int assignStatusToExits();


  protected:
    QString m_name;
    QString m_roomsFilePath;
    QString m_mobsFilePath;
    QString m_itemsFilePath;
    QString m_zonesFilePath;
    QString m_notesFilePath;
    QString m_shopsFilePath;
    QList<Room> m_rooms;
    QList<Mob> m_mobs;
    QList<Item> m_items;
    QList<Shop> m_shops;
    QList<Zone> m_zones;
    bool m_roomsSorted;
    bool m_zonesSorted;
    bool m_itemsSorted;
    bool m_mobsSorted;
    bool m_shopsSorted;
    bool m_roomsChanged;
    bool m_zonesChanged;
    bool m_itemsChanged;
    bool m_mobsChanged;
    bool m_shopsChanged;

  };

}  // namespace ts

#endif // TS_AREA_H
