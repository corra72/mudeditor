
#ifndef TS_SHOP_H
#define TS_SHOP_H

#include <stdio.h>
#include "objectdata.h"
#include "constants.h"


namespace ts
{

  class Shop : public ObjectData
  {
  public:
    void init();
    void validate();

    Shop();
    explicit Shop( VNumber );
    Shop( const Shop& );

    virtual ~Shop();

    Shop& operator=( const Shop& );

    VNumber itemToSell( int id ) const { return m_itemsToSell[ id ]; }
    int itemTypeToBuy( int id ) const { return m_itemTypesToBuy[ id ]; }
    VNumber keeper() const { return m_keeper; }
    VNumber assistant() const { return m_assistant; }
    VNumber room() const { return m_room; }
    int time( int id ) const { return m_time[ id ]; }
    float profitSell() const { return m_profitSell; }
    float profitBuy() const { return m_profitBuy; }
    int reaction( int id ) const { return m_reactions[ id ]; }
    const QString& answer( int id ) const { return m_answers[ id ]; }

    void setItemToSell( int id, VNumber new_value ) { m_itemsToSell[ id ] = new_value; }
    void setItemTypeToBuy( int id, int new_value ) { m_itemTypesToBuy[ id ] = new_value; }
    void setKeeper( VNumber new_value ) { m_keeper = new_value; }
    void setAssistant( VNumber new_value ) { m_assistant = new_value; }
    void setRoom( VNumber new_value ) { m_room = new_value; }
    void setTime( int id, int new_value ) { m_time[ id ] = new_value; }
    void setProfitSell( float new_value ) { m_profitSell = new_value; }
    void setProfitBuy( float new_value ) { m_profitBuy = new_value; }
    void setReaction( int, int );
    void setAnswer( int, const QString& );

    void resetAnswersToDefault();

    void load( FILE* );
    void save( QTextStream& );

  protected:
    VNumber m_itemsToSell[ SHOP_MAX_ITEMS ];
    int m_itemTypesToBuy[ SHOP_MAX_ITEMS ]; // ItemTypes
    VNumber m_keeper;
    VNumber m_assistant;
    VNumber m_room;
    int m_time[ SHOP_TIME_END ];
    float m_profitSell;
    float m_profitBuy;
    int m_reactions[ SHOP_REACTION_END ];
    QString m_answers[ SHOP_ANSWER_END ];

  };

} // namespace ts

#endif // TS_SHOP_H
