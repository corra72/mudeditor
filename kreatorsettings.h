
#ifndef TS_KREATORSETTINGS_H
#define TS_KREATORSETTINGS_H

#include <QString>
#include <QSettings>
#include "myqt.h"

class QWidget;


class KreatorSettings : protected QSettings
{
public:
  static KreatorSettings& instance();
  static void deleteInstance();

  void saveGuiStatus( const QString&, QWidget* );
  void loadGuiStatus( const QString&, QWidget* );

  void saveButtonGroupStatus( const QString&, MyQt::ButtonGroup* );
  void loadButtonGroupStatus( const QString&, MyQt::ButtonGroup* );

  bool applyWithoutAsking();
  void setApplyWithoutAsking( bool );

  bool checkAreasAtStartup();
  void  setCheckAreasAtStartup( bool );

  bool assignZoneToRooms();
  void  setAssignZoneToRooms( bool );

  int kreatorTheme();
  void  setKreatorTheme( int );

  void save() { instance().sync(); }

  QList<QString> mru();
  void setMru( QList<QString> );

protected:
  KreatorSettings();
  virtual ~KreatorSettings();

private:
  static KreatorSettings* mp_instance;

};

#endif // TS_GUISETTINGS_H

