
#include "objectdata.h"


namespace ts
{

  ObjectData::ObjectData() : m_type( Object_Invalid ),
                             m_vnumber( VNumberInvalid ),
                             m_name( defaultName() )
  {
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created (default).", dumpObject().toUtf8().data().data() );
#endif
  }

  ObjectData::ObjectData( Type type ) : m_type( type ),
                                        m_vnumber( VNumberInvalid ),
                                        m_name( defaultName() )
  {
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created.", dumpObject().toUtf8().data() );
#endif
  }

  ObjectData::ObjectData( Type type, VNumber vnumber, const QString& object_name )
    : m_type( type ),
      m_vnumber( vnumber ),
      m_name( object_name.simplified() )
  {
    if( m_name.isEmpty() )
      m_name = defaultName();

#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created.", dumpObject().toUtf8().data() );
#endif
  }

  ObjectData::ObjectData( const ObjectData& od )
    : m_type( od.m_type ),
      m_vnumber( od.m_vnumber ),
      m_name( od.m_name )
  {
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s created (by copy).", dumpObject().toUtf8().data() );
#endif
  }

  ObjectData::~ObjectData()
  {
#if defined( TS_DEBUG_PANIC )
    qDebug( "%s deleted.", dumpObject().toUtf8().data() );
#endif
  }

  ObjectData& ObjectData::operator=( const ObjectData& od )
  {
    if( this != &od )
    {
#if defined( TS_DEBUG_PANIC )
      qDebug( "%s overwritten by %s.", dumpObject().toUtf8().data(), od.dumpObject().toUtf8().data() );
#endif
      m_type = od.m_type;
      m_vnumber = od.m_vnumber;
      m_name = od.m_name;
    }
    return *this;
  }

  QString ObjectData::typeName( Type type )
  {
    switch( type )
    {
    case Object_Character:
      return "Character";
    case Object_Mob:
      return "Mob";
    case Object_Item:
      return "Item";
    case Object_Room:
      return "Room";
    case Object_Shop:
      return "Shop";
    case Object_Zone:
      return "Zone";
    default:
      return "Invalid Object";
    };
  }

  QString ObjectData::dumpObject() const
  {
    QString sDump = "";
    sDump.sprintf( "%s #%ld '%s'", typeName().toUtf8().data(), m_vnumber, m_name.toUtf8().data() );
    return sDump;
  }

} // namespace ts

