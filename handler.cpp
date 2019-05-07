
#include "handler.h"
#include "constants.h"
#include "utils.h"
#include "constantname.h"
#include "eleuconf.h"


namespace ts
{

  QList<Spell> Handler::m_spells;
  QList<TrapDamageType> Handler::m_trapDamageTypes;

  void Handler::init()
  {
    loadSpells();
    loadTrapDamageTypes();
  }

  Handler::Handler()
  {
  }

  Handler::~Handler()
  {
  }

  /*
  Handler::Handler( const Handler& h )
  {
    m_spells = h.m_spells;
  }

  Handler& Handler::operator=( const Handler& h )
  {
    if( this != &h )
    {
      m_spells = h.m_spells;
    }
    return *this;
  }
  */

  void Handler::cleanUp()
  {
    m_spells.clear();
    m_trapDamageTypes.clear();
  }

  void Handler::addSpell( const QString &name, int id )
  {
    Spell spell( id, name );
    m_spells.append( spell );
  }

  void Handler::loadSpells()
  {
    for(int i = 0; i < Eleuconf::countAspells(); i++)
        addSpell( Eleuconf::getAspellsName(i) + QString(" (")+ QString::number(Eleuconf::getAspellsLevel(i)) + QString(")"), i);
  }

  QString Handler::spellName( int sid )
  {
    QList<Spell>::const_iterator it = m_spells.begin();
    while( it != m_spells.end() )
    {
      if( (*it).id() == sid )
        return (*it).name();
      ++it;
    }
    return QString( "Error" );
  }

  void Handler::addTrapDamageType( const QString &name, int id )
  {
    TrapDamageType tdt( id, name );
    m_trapDamageTypes.append( tdt );
  }

  void Handler::loadTrapDamageTypes()
  {
    addTrapDamageType( "poison", -4 );
    addTrapDamageType( "sleep", -3 );
    addTrapDamageType( "teleport", -2 );
    addTrapDamageType( "fire", 26 );
    addTrapDamageType( "cold", 608 );
    addTrapDamageType( "acid", 67 );
    addTrapDamageType( "energy", 10 );
    addTrapDamageType( "blunt", 701 );
    addTrapDamageType( "pierce", 702 );
    addTrapDamageType( "slash", 703 );
  }

  QString Handler::trapDamageTypeName( int did )
  {
    QList<TrapDamageType>::const_iterator it = m_trapDamageTypes.begin();
    while( it != m_trapDamageTypes.end() )
    {
      if( (*it).id() == did )
        return (*it).name();
      ++it;
    }
    return QString( "Sconosciuto" );
  }

  QString Handler::liquidName( int lid )
  {
    switch( lid )
    {
    case 0: return "Acqua";
    case 1: return "Birra";
    case 2: return "Vino";
    case 3: return "BirraChiara";
    case 4: return "BirraScura";
    case 5: return "Whiskey";
    case 6: return "Limonata";
    case 7: return "Acquavite";
    case 8: return "Vodka";
    case 9: return "Slime";
    case 10: return "Latte";
    case 11: return "Te";
    case 12: return "Caffè";
    case 13: return "Sangue";
    case 14: return "AcquaSalata";
    case 15: return "CocaCola";
    default:
        return "Error";
    }
  }

  QString Handler::itemValueName( int item_type, int value_id )
  {
    if( value_id < 0 || value_id >= ITEM_VALUE_END )
    {
      return "Undefined";
    }

    QString sValue[ ITEM_VALUE_END ];
    for( int i = 0; i < ITEM_VALUE_END; i++ )
      sValue[ i ] = QString( "Value%1" ).arg( i );

    switch( item_type )
    {
    case ITEM_TYPE_LIGHT:
      sValue[ ITEM_VALUE_2 ] = "Durata";
      break;

    case ITEM_TYPE_SCROLL:
    case ITEM_TYPE_POTION:
      sValue[ ITEM_VALUE_0 ] = "Livello";
      sValue[ ITEM_VALUE_1 ] = "Incantesimo #1";
      sValue[ ITEM_VALUE_2 ] = "Incantesimo #2";
      sValue[ ITEM_VALUE_3 ] = "Incantesimo #3";
      break;

    case ITEM_TYPE_WAND:
    case ITEM_TYPE_STAFF:
      sValue[ ITEM_VALUE_0 ] = "Livello";
      sValue[ ITEM_VALUE_1 ] = "Max cariche";
      sValue[ ITEM_VALUE_2 ] = "Cariche rimaste";
      sValue[ ITEM_VALUE_3 ] = "Incantesimo";
      break;

    case ITEM_TYPE_WEAPON:
      sValue[ ITEM_VALUE_1 ] = "Numero dadi";
      sValue[ ITEM_VALUE_2 ] = "Numero facce";
      sValue[ ITEM_VALUE_3 ] = "Tipo di danno";
      break;

    case ITEM_TYPE_FIREWEAPON:
      sValue[ ITEM_VALUE_0 ] = "Forza minima";
      sValue[ ITEM_VALUE_1 ] = "Distanza minima";
      sValue[ ITEM_VALUE_2 ] = "Bonus sul range";
      sValue[ ITEM_VALUE_3 ] = "Tipo di proiettile";
      break;

    case ITEM_TYPE_MISSILE:
      sValue[ ITEM_VALUE_0 ] = "% di rottura";
      sValue[ ITEM_VALUE_1 ] = "Numero dadi (danno)";
      sValue[ ITEM_VALUE_2 ] = "Numero facce (danno)";
      sValue[ ITEM_VALUE_3 ] = "Tipo di proiettile";
      break;

    case ITEM_TYPE_ARMOR:
      sValue[ ITEM_VALUE_0 ] = "AC attuale";
      sValue[ ITEM_VALUE_1 ] = "AC massima";
      break;

    case ITEM_TYPE_TREASURE:
    case ITEM_TYPE_WORN:
    case ITEM_TYPE_OTHER:
    case ITEM_TYPE_TRASH:
    case ITEM_TYPE_NOTE:
    case ITEM_TYPE_PEN:
    case ITEM_TYPE_BOAT:
    case ITEM_TYPE_BOARD:
    case ITEM_TYPE_TREE:
    case ITEM_TYPE_ROCK:
      break;

    case ITEM_TYPE_TRAP:
      sValue[ ITEM_VALUE_0 ] = "Trigger";
      sValue[ ITEM_VALUE_1 ] = "Tipo danno";
      sValue[ ITEM_VALUE_2 ] = "Livello";
      sValue[ ITEM_VALUE_3 ] = "Cariche";
      break;

    case ITEM_TYPE_CONTAINER:
      sValue[ ITEM_VALUE_0 ] = "Capacita'";
      sValue[ ITEM_VALUE_1 ] = "Flags";
      sValue[ ITEM_VALUE_2 ] = "Chiave";
      break;

    case ITEM_TYPE_LIQUID_CONTAINER:
      sValue[ ITEM_VALUE_0 ] = "Drink-unit attuali";
      sValue[ ITEM_VALUE_1 ] = "Drink-unit massime";
      sValue[ ITEM_VALUE_2 ] = "Tipo di liquido";
      sValue[ ITEM_VALUE_3 ] = "Flags";
      break;

    case ITEM_TYPE_KEY:
      sValue[ ITEM_VALUE_0 ] = "One time";
      break;

    case ITEM_TYPE_FOOD:
      sValue[ ITEM_VALUE_0 ] = "Ore di sazieta'";
      sValue[ ITEM_VALUE_2 ] = "Avvelenato";
      break;

    case ITEM_TYPE_MONEY:
      sValue[ ITEM_VALUE_0 ] = "Monete";
      break;

    case ITEM_TYPE_AUDIO:
      sValue[ ITEM_VALUE_0 ] = "Timing";
      break;

//    case ITEM_TYPE_SEAL_CONTAINER:
//      sValue[ ITEM_VALUE_0 ] = "Tipo";
//      sValue[ ITEM_VALUE_1 ] = "Minimo";
//      sValue[ ITEM_VALUE_2 ] = "Massimo";
//            break;

    default:
      qWarning( "Handler::itemValueName() : Invalid ItemType '%d'.", item_type );
    }

    return sValue[ value_id ];
  }

  QString Handler::itemValueToString( int item_type, int value_id, const Item& item )
  {
    if( value_id < 0 || value_id >= ITEM_VALUE_END )
    {
      return "Undefined";
    }

    QString sValue[ ITEM_VALUE_END ];
    for( int i = 0; i < ITEM_VALUE_END; i++ )
      sValue[ i ] = QString::number( item.value( i ) );

    switch( item_type )
    {
    case ITEM_TYPE_LIGHT:
      sValue[ ITEM_VALUE_2 ] = ( item.value( ITEM_VALUE_2 ) < 0 ? "eterna" : sValue[ ITEM_VALUE_2 ] );
      break;

    case ITEM_TYPE_SCROLL:
    case ITEM_TYPE_POTION:
      sValue[ ITEM_VALUE_1 ] = Handler::spellName( item.value( ITEM_VALUE_1 ) );
      sValue[ ITEM_VALUE_2 ] = Handler::spellName( item.value( ITEM_VALUE_2 ) );
      sValue[ ITEM_VALUE_3 ] = Handler::spellName( item.value( ITEM_VALUE_3 ) );
      break;

    case ITEM_TYPE_WAND:
    case ITEM_TYPE_STAFF:
      sValue[ ITEM_VALUE_3 ] = Handler::spellName( item.value( ITEM_VALUE_3 ) );
      break;

    case ITEM_TYPE_WEAPON:
      sValue[ ITEM_VALUE_3 ] = ConstantName::itemWeaponDamageType( item.value( ITEM_VALUE_3 ) );
      break;

    case ITEM_TYPE_FIREWEAPON:
    case ITEM_TYPE_MISSILE:
    case ITEM_TYPE_TREASURE:
    case ITEM_TYPE_ARMOR:
    case ITEM_TYPE_WORN:
    case ITEM_TYPE_OTHER:
    case ITEM_TYPE_TRASH:
    case ITEM_TYPE_NOTE:
    case ITEM_TYPE_MONEY:
    case ITEM_TYPE_PEN:
    case ITEM_TYPE_BOAT:
    case ITEM_TYPE_AUDIO:
    case ITEM_TYPE_BOARD:
    case ITEM_TYPE_TREE:
    case ITEM_TYPE_ROCK:
      break;

    case ITEM_TYPE_TRAP:
      sValue[ ITEM_VALUE_0 ] = Utils::bitvector2string( item.value( ITEM_VALUE_0 ), Eleuconf::getAllFlagsCaption(Eleuconf::trapFlags) ).toLower();
      sValue[ ITEM_VALUE_1 ] = Handler::trapDamageTypeName( item.value( ITEM_VALUE_1 ) );
      break;

    case ITEM_TYPE_CONTAINER:
      sValue[ ITEM_VALUE_1 ] = Utils::bitvector2string( item.value( ITEM_VALUE_1 ), Eleuconf::getAllFlagsCaption(Eleuconf::containerFlags) ).toLower();
      break;

    case ITEM_TYPE_LIQUID_CONTAINER:
      sValue[ ITEM_VALUE_2 ] = Handler::liquidName( item.value( ITEM_VALUE_2 ) ).toLower();
      sValue[ ITEM_VALUE_3 ] = Utils::bitvector2string( item.value( ITEM_VALUE_3 ), Eleuconf::getAllFlagsCaption(Eleuconf::drinkContainerFlags) ).toLower();
      break;

    case ITEM_TYPE_KEY:
      sValue[ ITEM_VALUE_0 ] = ( item.value( ITEM_VALUE_0 ) > 0 ? "SI" : "NO" );
      break;

    case ITEM_TYPE_FOOD:
      sValue[ ITEM_VALUE_3 ] = ( item.value( ITEM_VALUE_3 ) > 0 ? "SI" : "NO" );
      break;

    default:
      qWarning( "Invalid ItemType '%d' in %s.", item_type, qPrintable( item.dumpObject() ) );
    }

    return sValue[ value_id ];

  }

} // namespace ts
