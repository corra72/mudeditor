
#ifndef TS_ITEM_H
#define TS_ITEM_H

#include <QList>
#include <stdio.h>
#include "objectdata.h"
#include "itemaffect.h"
#include "extradescription.h"
#include "bit.h"
#include "constants.h"
#include "utils.h"

namespace ts
{

  class Item : public ObjectData
  {
  public:
    void init();
    void validate();

    Item();
    explicit Item( VNumber, const QString& item_name = "" );
    Item( const Item& );

    virtual ~Item();

    Item& operator=( const Item& );

    const QString& shortDescription() const { return m_shortDescription; }
    const QString& longDescription() const { return m_longDescription; }
    const QString& actionDescription() const { return m_actionDescription; }
    BitVector flags() const { return m_flags; }
    BitVector extraFlags() const { return m_extraFlags; }
    BitVector wearPositions() const { return m_wearPositions; }
    int itemType() const { return m_itemType; }
    VNumber value( int v ) const { return m_values[ v ]; }
    int weight() const { return m_weight; }
    int goldValue() const { return m_goldValue; }
    int rentCost() const { return m_rentCost; }

    bool hasFlag( int flag ) const { return Bit::has( m_flags, flag ); }
    bool hasExtraFlag( int flag ) const { return Bit::has( m_extraFlags, flag ); }
    bool hasWearPosition( int wp ) const { return Bit::has( m_wearPositions, wp ); }

    bool isContainer() const { return m_itemType == ITEM_TYPE_CONTAINER; }
    VNumber containerKey() const { return value( ITEM_VALUE_2 ); }
    void setContainerKey( VNumber new_value ) { setValue( ITEM_VALUE_2, new_value ); }

    void setShortDescription( const QString& txt ) { m_shortDescription = Utils::removeTilde( txt.simplified() ); }
    void setLongDescription( const QString& txt ) { m_longDescription = Utils::removeTilde( txt.trimmed() ); }
    void setActionDescription( const QString& txt ) { m_actionDescription = Utils::removeTilde( txt.trimmed() ); }
    void setFlags( BitVector new_flags ) { m_flags = new_flags; }
    void setExtraFlags( BitVector new_flags ) { m_extraFlags = new_flags; }
    void setWearPositions( BitVector new_pos ) { m_wearPositions = new_pos; }
    void setItemType( int new_it ) { m_itemType = new_it; }
    void setValue( int v, VNumber new_value ) { m_values[ v ] = new_value; }
    void setWeight( int w ) { m_weight = w; }
    void setGoldValue( int gv ) { m_goldValue = gv; }
    void setRentCost( int rc ) { m_rentCost = rc; }

    typedef QList<ExtraDescription>::const_iterator extra_description_const_iterator;
    typedef QList<ExtraDescription>::iterator extra_description_iterator;
    const QList<ExtraDescription>& extraDescriptions() const { return m_extraDescriptions; }
    bool hasExtraDescription( const QString& extra_keys ) const;
    const ExtraDescription& extraDescription( const QString& extra_keys ) const;
    void removeExtraDescription( const QString& extra_keys );
    void addExtraDescription( const ExtraDescription& );

    bool hasAffect( int id ) const { return isValidAffectId( id ) ? m_affects[ id ].isValid() : false; }
    ItemAffect affect( int id ) const { return isValidAffectId( id ) ? m_affects[ id ] : ItemAffect(); }
    void removeAffect( int id ) { if( isValidAffectId( id ) ) m_affects[ id ].setNull(); }
    void setAffect( int id, const ItemAffect& ia ) { if( isValidAffectId( id ) ) m_affects[ id ] = ia; }

    void load( FILE* );
    void save( QTextStream& );

    QString valueName( int );
    QString valueToString( int );

  private:
    bool isValidAffectId( int id ) const { return id >= 0 && id <= ITEM_AFFECT_POSITION_END; }

  protected:
    QString m_shortDescription;
    QString m_longDescription;
    QString m_actionDescription;
    BitVector m_flags;
    BitVector m_extraFlags;
    BitVector m_wearPositions;
    int m_itemType;
    VNumber m_values[ITEM_VALUE_END];
    int m_weight;
    int m_goldValue;
    int m_rentCost;
    QList<ExtraDescription> m_extraDescriptions;
    ItemAffect m_affects[ ITEM_AFFECT_POSITION_END ];

  };

} // namespace ts

#endif // TS_ITEM_H

