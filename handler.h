
#ifndef TS_HANDLER_H
#define TS_HANDLER_H

#include <QList>
#include <QString>
#include "spell.h"
#include "item.h"



namespace ts
{

  typedef Spell TrapDamageType;

  class Handler
  {
  public:
    static void init();
    static void cleanUp();

    static QString spellName( int );
    static QString trapDamageTypeName( int );
    static QString liquidName( int );
    static QString itemValueName( int item_type, int value_id );
    static QString itemValueToString( int item_type, int value_id, const Item& );

    static const QList<Spell>& spells() { return m_spells; }
    static const QList<TrapDamageType>& trapDamageTypes() { return m_trapDamageTypes; }

  protected:
    Handler();
    ~Handler();
    /*
    Handler( const Handler& );
    Handler& operator=( const Handler& );
    */

    static void loadSpells();
    static void loadTrapDamageTypes();
    static void addSpell( const QString&, int );
    static void addTrapDamageType( const QString&, int );

  private:
    static QList<Spell> m_spells;
    static QList<TrapDamageType> m_trapDamageTypes;

  };


} // namespace ts


#endif // TS_HANDLER_H
