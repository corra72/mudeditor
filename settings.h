
#ifndef TS_SETTINGS_H
#define TS_SETTINGS_H

#include <QString>
#include <QSettings>


namespace ts
{

  class Settings
  {
  public:
    static QString zonesDirectory() { return m_zonesDirectory; }
    static QString roomsDirectory() { return m_roomsDirectory; }
    static QString itemsDirectory() { return m_itemsDirectory; }
    static QString mobsDirectory() { return m_mobsDirectory; }
    static QString notesDirectory() { return m_notesDirectory; }
    static QString shopsDirectory() { return m_shopsDirectory; }
    static QString logsDirectory() { return m_logsDirectory; }
    static QString objectDirectory( int );

    static void setZonesDirectory( const QString& value ) { m_zonesDirectory = value; }
    static void setRoomsDirectory( const QString& value ) { m_roomsDirectory = value; }
    static void setItemsDirectory( const QString& value ) { m_itemsDirectory = value; }
    static void setMobsDirectory( const QString& value ) { m_mobsDirectory = value; }
    static void setNotesDirectory( const QString& value ) { m_notesDirectory = value; }
    static void setShopsDirectory( const QString& value ) { m_shopsDirectory = value; }
    static void setLogsDirectory( const QString& value ) { m_logsDirectory = value; }
    static QString zoneSuffix() { return m_zoneSuffix; }
    static QString roomSuffix() { return m_roomSuffix; }
    static QString itemSuffix() { return m_itemSuffix; }
    static QString mobSuffix() { return m_mobSuffix; }
    static QString noteSuffix() { return m_noteSuffix; }
    static QString shopSuffix() { return m_shopSuffix; }
    static QString fileSuffix( int );

    static QString defaultCommentSeparator() { return m_defaultCommentSeparator; }
    static void setDefaultCommentSeparator( const QString& new_value ) { m_defaultCommentSeparator = new_value; }

    static bool load();
    static bool save();

  protected:
    static bool saveEntry( QSettings*, const QString &key, const QString &value );
    static bool loadEntry( const QSettings&, const QString &key, QString* value, const QString &def_value );

  private:
    static QString m_zonesDirectory;
    static QString m_roomsDirectory;
    static QString m_itemsDirectory;
    static QString m_mobsDirectory;
    static QString m_notesDirectory;
    static QString m_shopsDirectory;
    static QString m_logsDirectory;

    static QString m_zoneSuffix;
    static QString m_roomSuffix;
    static QString m_itemSuffix;
    static QString m_mobSuffix;
    static QString m_noteSuffix;
    static QString m_shopSuffix;

    static QString m_defaultCommentSeparator;

  protected:
    Settings() {}
    virtual ~Settings() {}

  };
}

#endif
