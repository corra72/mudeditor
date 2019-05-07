#include "shop.h"
#include <QTextStream>
#include "utils.h"


namespace ts
{

  void Shop::init()
  {
    int i;
    for( i = 0; i < SHOP_MAX_ITEMS; i++ )
    {
      m_itemsToSell[ i ] = -1;
      m_itemTypesToBuy[ i ] = 0;
    }

    m_keeper = VNumberInvalid;
    m_assistant = VNumberInvalid;
    m_room = VNumberInvalid;

    m_time[ SHOP_TIME_OPEN_1  ] = 9;
    m_time[ SHOP_TIME_CLOSE_1 ] = 13;
    m_time[ SHOP_TIME_OPEN_2  ] = 15;
    m_time[ SHOP_TIME_CLOSE_2 ] = 19;

    m_profitSell = 1.1;
    m_profitBuy = 0.3;

    for( i = 0; i < SHOP_REACTION_END; i++ )
      m_reactions[ i ] = 2;

    resetAnswersToDefault();
  }

  void Shop::resetAnswersToDefault()
  {
    m_answers[ SHOP_ANSWER_NO_SUCH_ITEM_1 ] = QString( "%s Mi spiace ma non vendo quel tipo di oggetto." );
    m_answers[ SHOP_ANSWER_NO_SUCH_ITEM_2 ] = QString( "%s Non mi sembra che lei abbia nulla del genere." );
    m_answers[ SHOP_ANSWER_DO_NOT_BUY ] = QString( "%s Mi dispiace ma non tratto questo tipo di oggetti." );
    m_answers[ SHOP_ANSWER_MISSING_CASH_1 ] = QString( "%s Mi dispiace ma non ho abbastanza denaro al momento." );
    m_answers[ SHOP_ANSWER_MISSING_CASH_2 ] = QString( "%s Forse dovrebbe orientarsi su qualcosa di meno costoso." );
    m_answers[ SHOP_ANSWER_SELL ] = QString( "%s Per sole %d monete, lei ha fatto un ottimo affare." );
    m_answers[ SHOP_ANSWER_BUY ] = QString( "%s Ecco a lei %d monete." );
  }

  void Shop::validate()
  {}

  Shop::Shop()
    : ObjectData( Object_Shop )
  {
    init();
  }

  Shop::Shop( VNumber vnum )
    : ObjectData( Object_Shop, vnum )
  {
    init();
  }

  Shop::Shop( const Shop& s )
    : ObjectData( (ObjectData)s )
  {
    int i;
    for( i = 0; i < SHOP_MAX_ITEMS; i++ )
    {
      m_itemsToSell[ i ] = s.m_itemsToSell[ i ];
      m_itemTypesToBuy[ i ] = s.m_itemTypesToBuy[ i ];
    }
    m_keeper = s.m_keeper;
    m_assistant = s.m_assistant;
    m_room = s.m_room;
    for( i = 0; i < SHOP_TIME_END; i++ )
      m_time[ i ] = s.m_time[ i ];
    m_profitSell = s.m_profitSell;
    m_profitBuy = s.m_profitBuy;
    for( i = 0; i < SHOP_REACTION_END; i++ )
      m_reactions[ i ] = s.m_reactions[ i ];
    for( i = 0; i < SHOP_ANSWER_END; i++ )
      m_answers[ i ] = s.m_answers[ i ];
  }

  Shop::~Shop()
  {

  }

  Shop& Shop::operator=( const Shop& s )
  {
    if( this != &s )
    {
      ObjectData::operator=( (ObjectData)s );
      int i;
      for( i = 0; i < SHOP_MAX_ITEMS; i++ )
      {
        m_itemsToSell[ i ] = s.m_itemsToSell[ i ];
        m_itemTypesToBuy[ i ] = s.m_itemTypesToBuy[ i ];
      }
      m_keeper = s.m_keeper;
      m_assistant = s.m_assistant;
      m_room = s.m_room;
      for( i = 0; i < SHOP_TIME_END; i++ )
        m_time[ i ] = s.m_time[ i ];
      m_profitSell = s.m_profitSell;
      m_profitBuy = s.m_profitBuy;
      for( i = 0; i < SHOP_REACTION_END; i++ )
        m_reactions[ i ] = s.m_reactions[ i ];
      for( i = 0; i < SHOP_ANSWER_END; i++ )
        m_answers[ i ] = s.m_answers[ i ];
    }
    return *this;
  }

  void Shop::setAnswer( int id, const QString& txt )
  {
    if( id < 0 || id >= SHOP_ANSWER_END )
      return;
    m_answers[ id ] = Utils::removeTilde( txt.simplified() );
  }

  void Shop::setReaction( int id, int new_value )
  {
    if( id < 0 || id >= SHOP_REACTION_END )
      return;
    m_reactions[ id ] = new_value;
  }

  void Shop::load( FILE* pFile )
  {
    int i;
    for( i = 0; i < SHOP_MAX_ITEMS; i++ )
      fscanf( pFile, "%ld \n", &m_itemsToSell[ i ] );

	fscanf( pFile, "%f \n", &m_profitBuy );
	fscanf( pFile, "%f \n", &m_profitSell );
	for( i = 0; i < SHOP_MAX_ITEMS; i++ )
	  fscanf( pFile, "%d \n", &m_itemTypesToBuy[ i ] );

    for( i = 0; i < SHOP_ANSWER_END; i++ )
      setAnswer( i, Utils::readString( pFile ) );

    for( i = 0; i < SHOP_REACTION_END; i++ )
      fscanf( pFile, "%d \n", &m_reactions[ i ] );

    fscanf( pFile, "%ld \n", &m_keeper );
    fscanf( pFile, "%ld \n", &m_assistant );
    fscanf( pFile, "%ld \n", &m_room );

    for( i = 0; i < SHOP_TIME_END; i++ )
      fscanf( pFile, "%d \n", &m_time[ i ] );

    qDebug( "%s loading completed.", qPrintable( dumpObject() ) );
  }

  /*

*** SHOP ARCHETYPE

#vnum~
<item sold vnum 1>
<item sold vnum 2>
<item sold vnum 3>
<item sold vnum 4>
<item sold vnum 5>
<multiple for selling items>
<multiple for buying items>
<item type bought 1>
<item type bought 2>
<item type bought 3>
<item type bought 4>
<item type bought 5>
<message 1>
<message 2>
<message 3>
<message 4>
<message 5>
<message 6>
<message 7>
<temper 1>
<temper 2>
<shopkeeper vnum>
0
<room vnum>
<open 1>
<close 1>
<open 2>
<close 2>

*/

  void Shop::save( QTextStream& stream )
  {
    stream << "#" << m_vnumber << "~" << endl << flush;
    int i;
    for( i = 0; i < SHOP_MAX_ITEMS; i++ )
      stream << m_itemsToSell[ i ] << " " << endl;

    stream << m_profitBuy << " " << endl;
    stream << m_profitSell << " " << endl;

	for( i = 0; i < SHOP_MAX_ITEMS; i++ )
      stream << m_itemTypesToBuy[ i ] << " " << endl;

    for( i = 0; i < SHOP_ANSWER_END; i++ )
      stream << m_answers[ i ] << "~" << endl;

    for( i = 0; i < SHOP_REACTION_END; i++ )
      stream << m_reactions[ i ] << " " << endl;

    stream << m_keeper << " " << endl;
    stream << m_assistant << " " << endl;
    stream << m_room << " " << endl;

    for( i = 0; i < SHOP_TIME_END; i++ )
      stream << m_time[ i ] << " " << endl;

    stream << flush;

    qDebug( "%s loading completed.", qPrintable( dumpObject() ) );
  }

} // namespace ts
