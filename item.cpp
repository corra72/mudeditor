#include "item.h"
#include <QFile>
#include "exception.h"
#include "handler.h"

namespace ts
{

  void Item::init()
  {
    int i = 0;
    m_shortDescription = QString( "an " ) + defaultName();
    m_longDescription = QString( "An "  ) + defaultName() + " is here, on the ground.";
    m_actionDescription = "";
    m_flags = 0;
    m_extraFlags = 0;
    m_wearPositions = ITEM_WEAR_POSITION_TAKE;
    m_itemType = ITEM_TYPE_ARMOR;
    for( i = 0; i < ITEM_VALUE_END; i++ )
      m_values[ i ] = 0;
    m_weight = 1;
    m_goldValue = 10;
    m_rentCost = 0;

    if( !m_extraDescriptions.empty() )
      m_extraDescriptions.clear();

    for( i = 0; i < ITEM_AFFECT_POSITION_END; i++ )
      m_affects[ i ].setNull();

  }

  void Item::validate()
  {

  }

  Item::Item()
    : ObjectData( Object_Item )
  {
    init();
  }

  Item::Item( VNumber vnum, const QString& item_name )
    : ObjectData( Object_Item, vnum, item_name )
  {
    init();
  }

  Item::Item( const Item& i )
    : ObjectData( (ObjectData)i )
  {
    int j = 0;
    m_shortDescription = i.m_shortDescription;
    m_longDescription = i.m_longDescription;
    m_actionDescription = i.m_actionDescription;
    m_flags = i.m_flags;
    m_extraFlags = i.m_extraFlags;
    m_wearPositions = i.m_wearPositions;
    m_itemType = i.m_itemType;
    for( j = 0; j < ITEM_VALUE_END; j++ )
      m_values[ j ] = i.m_values[ j ];
    m_weight = i.m_weight;
    m_goldValue = i.m_goldValue;
    m_rentCost = i.m_rentCost;
    m_extraDescriptions = i.m_extraDescriptions;
    for( j = 0; j < ITEM_AFFECT_POSITION_END; j++ )
      m_affects[ j ] = i.m_affects[ j ];

  }

  Item::~Item()
  {
  }

  Item& Item::operator=( const Item& i )
  {
    if( this != &i )
    {
      int j = 0;
      ObjectData::operator=( (ObjectData)i );
      m_shortDescription = i.m_shortDescription;
      m_longDescription = i.m_longDescription;
      m_actionDescription = i.m_actionDescription;
      m_flags = i.m_flags;
      m_extraFlags = i.m_extraFlags;
      m_wearPositions = i.m_wearPositions;
      m_itemType = i.m_itemType;
      for( j = 0; j < ITEM_VALUE_END; j++ )
        m_values[ j ] = i.m_values[ j ];
      m_weight = i.m_weight;
      m_goldValue = i.m_goldValue;
      m_rentCost = i.m_rentCost;
      m_extraDescriptions = i.m_extraDescriptions;
      for( j = 0; j < ITEM_AFFECT_POSITION_END; j++ )
        m_affects[ j ] = i.m_affects[ j ];

    }
    return *this;
  }

  bool Item::hasExtraDescription( const QString& extra_keys ) const
  {
    try
    {
      (void) extraDescription( extra_keys );
      return true;
    }
    catch( XObjectNotFound& )
    {
      return false;
    }
  }

  const ExtraDescription& Item::extraDescription( const QString& extra_keys ) const
  {
    QString sKeysToFind = extra_keys.toLower();
    extra_description_const_iterator it = m_extraDescriptions.begin();

    while( it != m_extraDescriptions.end() )
    {
      if( (*it).keys() == sKeysToFind )
        return *it;
      ++it;
    }

    QString sException = "";
    sException.sprintf( "ExtraDescription '%s' not found.", sKeysToFind.toUtf8().data() );
    throw XObjectNotFound( sException );
  }

  void Item::removeExtraDescription( const QString& extra_keys )
  {
    QString sKeysToFind = extra_keys.toLower();
    extra_description_iterator it = m_extraDescriptions.begin();
    while( it != m_extraDescriptions.end() )
    {
      if( (*it).keys() == sKeysToFind )
      {
        qDebug( "%s removed from %s.", (*it).dumpObject().toUtf8().data(), dumpObject().toUtf8().data() );
        m_extraDescriptions.erase( it );
        return;
      }
      ++it;
    }
  }

  void Item::addExtraDescription( const ExtraDescription& new_extra_description )
  {
    removeExtraDescription( new_extra_description.keys() );
    m_extraDescriptions.append( new_extra_description );
    qDebug( "%s added to %s.", new_extra_description.dumpObject().toUtf8().data(), dumpObject().toUtf8().data() );
  }

  void Item::save( QTextStream& stream )
  {
    int i = 0;
    stream << "#" << m_vnumber << endl << flush;
    stream << m_name << "~" << endl << flush;
    stream << m_shortDescription << "~" << endl << flush;
    stream << m_longDescription << "~" << endl << flush;
	if(!m_actionDescription.isEmpty())
		stream << m_actionDescription << endl << "~" << endl << flush;
	else
		stream << "~" << endl << flush;
    stream << m_itemType << " " << flush;
   // Utils::writeNewFlags( stream, m_flags, m_extraFlags);
    
    Utils::saveBitVector( stream, m_flags );
    stream << " " << flush;
    
    Utils::saveBitVector( stream, m_wearPositions );
    stream << endl << flush;
    for( i = 0; i < ITEM_VALUE_END; i++ )
      stream << m_values[ i ] << " " << flush;
    stream << endl << flush;
    stream << m_weight << " " << flush;
    stream << m_goldValue << " " << flush;
    stream << m_rentCost << endl << flush;

    /* Save Extra Descriptions */
    extra_description_iterator itex = m_extraDescriptions.begin();
    while( itex != m_extraDescriptions.end() )
    {
      (*itex).save( stream );
      ++itex;
    }

    /* Save Affects */
    for( i = 0; i < ITEM_AFFECT_POSITION_END; i++ )
      m_affects[ i ].save( stream );
    
    if( m_extraFlags > 0 )
    {
      stream << "F" << endl << flush;
      stream << m_extraFlags << endl << flush;
    }

    qDebug( "%s saving completed.", dumpObject().toUtf8().data() );
  }

  void Item::load( FILE* pFile )
  {
    setName( Utils::readString( pFile ) );

    QString sErrorMessage = "";
    sErrorMessage = dumpObject();

    setShortDescription( Utils::readString( pFile ) );
    setLongDescription( Utils::readString( pFile ) );
    setActionDescription( Utils::readString( pFile ) );

    long iTmp;
    iTmp = Utils::readNumber( pFile, sErrorMessage + " (ItemType)" );
    if( iTmp <= ITEM_TYPE_NONE || iTmp >= ITEM_TYPE_END )
    {
      qWarning( "%s has an invalid item type of %ld (fixed).", sErrorMessage.toUtf8().data(), iTmp );
      m_itemType = ITEM_TYPE_OTHER;
    }
    else
      m_itemType = iTmp;

    iTmp = Utils::readNewFlags( pFile, &m_extraFlags);
    m_flags = iTmp;
    m_wearPositions = Utils::readNumber( pFile, sErrorMessage + " (WearPositions)" );

    for( int i = ITEM_VALUE_0; i < ITEM_VALUE_END; i++ )
      m_values[ i ] = Utils::readNumber( pFile, sErrorMessage + " (Value" + QString::number( i ) + ')' );

    m_weight = Utils::readNumber( pFile, sErrorMessage + " (Weight)" );
    m_goldValue = Utils::readNumber( pFile, sErrorMessage + " (GoldValue)" );
    m_rentCost = Utils::readNumber( pFile, sErrorMessage + " (RentCost)" );

    validate();

    int iAffectId = ITEM_AFFECT_POSITION_0;
    long lPos = 0;
    bool bDone = false;
    char szBuffer[ 161 ];

    while( !bDone && ( ( lPos = ftell( pFile ) ), fgets( szBuffer, sizeof( szBuffer ), pFile ) ) )
    {
      switch( *szBuffer )
      {
      case 'A':
        {
          ItemAffect ia;
          ia.load( pFile, m_vnumber );
          setAffect( iAffectId, ia );
          iAffectId++;
        }
        break;

      case 'E':
        {
          ExtraDescription ed;
          ed.load( pFile );
          addExtraDescription( ed );
        }
        break;
      
       case 'F':
         {
           m_extraFlags = Utils::readNumber( pFile, sErrorMessage + " (Extraflags2)" );
         }
         break;

      case '#':
        fseek( pFile, lPos, SEEK_SET );
        bDone = true;
        break;
      }
    }

    qDebug( "%s loading completed.", dumpObject().toUtf8().data() );
  }

  QString Item::valueName( int value_id )
  {
    return Handler::itemValueName( itemType(), value_id );
  }

  QString Item::valueToString( int value_id )
  {
    return Handler::itemValueToString( itemType(), value_id, *this );
  }

} // namespace ts

