
#include "settings.h"
#include "objectdata.h"


namespace ts
{

  QString Settings::m_zonesDirectory = ".";
  QString Settings::m_roomsDirectory = ".";
  QString Settings::m_itemsDirectory = ".";
  QString Settings::m_mobsDirectory = ".";
  QString Settings::m_notesDirectory = ".";
  QString Settings::m_shopsDirectory = ".";
  QString Settings::m_logsDirectory = ".";

  QString Settings::m_zoneSuffix = "zon";
  QString Settings::m_roomSuffix = "wld";
  QString Settings::m_itemSuffix = "obj";
  QString Settings::m_mobSuffix = "mob";
  QString Settings::m_noteSuffix = "txt";
  QString Settings::m_shopSuffix = "shp";

  QString Settings::m_defaultCommentSeparator = "*!";

  bool Settings::saveEntry( QSettings* pSettings, const QString &key, const QString &value )
  {
    pSettings->setValue( key, value );
    return true;
  }

  bool Settings::loadEntry( const QSettings& settings, const QString &key, QString* pValue, const QString &def_value )
  {
    *pValue = settings.value( key, def_value ).toString();
    return true;
  }

  bool Settings::load()
  {
    qDebug( "Settings: loading preferences." );
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, "ts", "core" );
    bool ok = true;

    ok |= loadEntry( settings, "/directories/rooms", &m_roomsDirectory, "." );
    ok |= loadEntry( settings, "/directories/zones", &m_zonesDirectory, "." );
    ok |= loadEntry( settings, "/directories/items", &m_itemsDirectory, "." );
    ok |= loadEntry( settings, "/directories/mobs", &m_mobsDirectory, "." );
    ok |= loadEntry( settings, "/directories/notes", &m_notesDirectory, "." );
    ok |= loadEntry( settings, "/directories/shops", &m_shopsDirectory, "." );
    ok |= loadEntry( settings, "/directories/logs", &m_logsDirectory, "." );

    ok |= loadEntry( settings, "/misc/commentseparator", &m_defaultCommentSeparator, "*!" );

    qDebug( "Settings: loading preferences... completed!" );
    return ok;
  }

  bool Settings::save()
  {
    qDebug( "Settings: saving preferences." );
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, "ts", "core" );

    bool ok = true;

    ok |= saveEntry( &settings, "/directories/rooms", m_roomsDirectory );
    ok |= saveEntry( &settings, "/directories/zones", m_zonesDirectory );
    ok |= saveEntry( &settings, "/directories/items", m_itemsDirectory );
    ok |= saveEntry( &settings, "/directories/mobs", m_mobsDirectory );
    ok |= saveEntry( &settings, "/directories/notes", m_notesDirectory );
    ok |= saveEntry( &settings, "/directories/shops", m_shopsDirectory );
    ok |= saveEntry( &settings, "/directories/logs", m_logsDirectory );

    ok |= saveEntry( &settings, "/misc/commentseparator", m_defaultCommentSeparator );

    qDebug( "Settings: saving preferences... completed!" );
    return ok;
  }

  QString Settings::fileSuffix( int odt )
  {
    if( odt == ObjectData::Object_Zone )
      return zoneSuffix();
    else if( odt == ObjectData::Object_Room )
      return roomSuffix();
    else if( odt == ObjectData::Object_Item )
      return itemSuffix();
    else if( odt == ObjectData::Object_Mob )
      return mobSuffix();
    else if( odt == ObjectData::Object_Shop )
      return shopSuffix();
    else
      return noteSuffix();
  }

  QString Settings::objectDirectory( int odt )
  {
    if( odt == ObjectData::Object_Zone )
      return zonesDirectory();
    else if( odt == ObjectData::Object_Room )
      return roomsDirectory();
    else if( odt == ObjectData::Object_Item )
      return itemsDirectory();
    else if( odt == ObjectData::Object_Mob )
      return mobsDirectory();
    else if( odt == ObjectData::Object_Shop )
      return shopsDirectory();
    else
      return notesDirectory();
  }

} // namespace ts
