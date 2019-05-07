
#include "zone.h"
#include "exception.h"
#include "utils.h"
#include "constants.h"



namespace ts
{

  void Zone::init()
  {
    m_maxVNumber = VNumberInvalid;
    m_flags = 0;
    m_newFlags = 0;
    m_repopInterval = 20;
    m_repopMode = ZONE_REPOP_MODE_ALWAYS;
    m_initList.reset();
    m_pwpLevel = 0;
  }

  void Zone::validate()
  {

  }

  Zone::Zone()
    : ObjectData( ObjectData::Object_Zone )
  {
    init();
  }

  Zone::Zone( VNumber zone_vnumber, const QString& zone_name )
    : ObjectData( ObjectData::Object_Zone, zone_vnumber, zone_name )
  {
    init();
  }

  Zone::Zone( const Zone& z )
    : ObjectData( (ObjectData)z )
  {
    m_initList = z.m_initList;
    m_maxVNumber = z.m_maxVNumber;
    m_flags = z.m_flags;
    m_newFlags = z.m_newFlags;
    m_repopInterval = z.m_repopInterval;
    m_repopMode = z.m_repopMode;
    m_pwpLevel = z.m_pwpLevel;
  }

  Zone::~Zone()
  {}

  Zone& Zone::operator=( const Zone& z )
  {
    if( this != &z )
    {
      ObjectData::operator=( (ObjectData)z );
      m_initList = z.m_initList;
      m_maxVNumber = z.m_maxVNumber;
      m_flags = z.m_flags;
      m_newFlags = z.m_newFlags;
      m_repopInterval = z.m_repopInterval;
      m_repopMode = z.m_repopMode;
      m_pwpLevel = z.m_pwpLevel;
    }
    return *this;
  }

  void Zone::loadHeaderOnly( FILE* pFile )
  {
    setName( Utils::readString( pFile ) );

    fscanf(pFile, " %ld ", &m_maxVNumber);
    fscanf(pFile, " %d ", &m_repopInterval);
    fscanf(pFile, " %ld ", &m_flags);

    if((signed)m_flags == -1) {
        fscanf(pFile, " %ld ", &m_flags);
        m_newFlags = Utils::readNumberInLine( pFile, QString("(New Flags)") );
        m_pwpLevel = Utils::readNumberInLine( pFile, QString("(Pwp Level)"));
        if(m_pwpLevel == -1)
            m_pwpLevel = 0;
    }

    if( hasFlag( ZONE_FLAG_IF_EMPTY ) )
    {
      m_repopMode = ZONE_REPOP_MODE_IF_EMPTY;
      removeFlag( ZONE_FLAG_IF_EMPTY );
    }
    else if( hasFlag( ZONE_FLAG_ALWAYS ) )
    {
      m_repopMode = ZONE_REPOP_MODE_ALWAYS;
      removeFlag( ZONE_FLAG_ALWAYS );
    }
    else
      m_repopMode = ZONE_REPOP_MODE_NEVER;
      
    m_newFlags = m_flags / 4;

    if( m_newFlags < 0)
        m_newFlags = 0;     // giusto per sicurezza se va sotto 0 gli do 0
  }

  void Zone::load( FILE* pFile )
  {
    loadHeaderOnly( pFile );
    ZoneCommandList cmd_list;
    cmd_list.load( pFile );
    setCommandList( cmd_list );
    validate();
  }

  void Zone::save( QTextStream& stream )
  {
    stream << "#" << m_vnumber << endl << flush;        // numero della zona
    stream << m_name << "~" << endl << flush;           // nome della zona
    stream << m_maxVNumber << " " << m_repopInterval << " ";    // numero più alto zona - tempo di repop
    /*
    if( hasFlag( ZONE_FLAG_IF_EMPTY ) )
      removeFlag( ZONE_FLAG_IF_EMPTY );
    if( hasFlag( ZONE_FLAG_ALWAYS ) )
      removeFlag( ZONE_FLAG_ALWAYS );
    */
    long iTmp = ((long)m_newFlags * 4) + (long)m_repopMode;
    stream << iTmp << endl << endl << flush;
//    stream << m_newFlags << endl << endl << flush;

      
/*      long iTmp = (long)m_repopMode + (long)m_flags;     questo era il vecchio save
      stream << "-1 " << iTmp << " "<< flush;
      Utils::saveBitVector(stream, m_newFlags);
      stream << " " << m_pwpLevel << flush;

    stream << endl << endl << flush; */

    ZoneCommandList cmd_list = commandList();
    cmd_list.trimEmptyCommands();
    cmd_list.save( stream );

    stream << endl <<  "S" << endl << endl << flush;
  }

  int Zone::countMobInits( VNumber vnum ) const
  {
#if defined( USE_ZONE_COMMAND_LIST_AS_MEMBER )
    return m_initList.countMobs( vnum );
#else
    int iCount = 0;
    ZoneInitList::mobs_const_iterator it = m_initList.mobsConstBegin();
    while( it != m_initList.mobsConstEnd() )
    {
      if( (*it).isMobLoad()  )
        iCount += (*it).countMobs( vnum );
      ++it;
    }
    return iCount;
#endif
  }

  int Zone::countItemInits( VNumber vnum ) const
  {
#if defined( USE_ZONE_COMMAND_LIST_AS_MEMBER )
    return m_initList.countItems( vnum );
#else
    int iCount = 0;
    ZoneInitList::items_const_iterator it = m_initList.itemsBegin();
    while( it != m_initList.itemsEnd() )
    {
      iCount += (*it).countItems( vnum );
      ++it;
    }
    return iCount;
#endif
  }

  int Zone::setMaxMobInits( VNumber vnum, int max_mobs )
  {
#if defined( USE_ZONE_COMMAND_LIST_AS_MEMBER )
    return m_initList.setMaxMobs( vnum, max_mobs );
#else
    int iCount = 0;
    ZoneInitList::mobs_iterator it = m_initList.mobsBegin();
    while( it != m_initList.mobsEnd() )
    {
      if( (*it).isMobLoad()  )
        iCount += (*it).setMaxMobs( vnum, max_mobs );
      ++it;
    }
    return iCount;
#endif
  }

  int Zone::setMaxItemInits( VNumber vnum, int max_items )
  {
#if defined( USE_ZONE_COMMAND_LIST_AS_MEMBER )
    return m_initList.setMaxItems( vnum, max_items );
#else
    int iCount = 0;
    ZoneInitList::items_iterator it = m_initList.itemsBegin();
    while( it != m_initList.itemsEnd() )
    {
      iCount += (*it).setMaxItems( vnum, max_items );
      ++it;
    }
    return iCount;
#endif
  }

} // namespace ts

