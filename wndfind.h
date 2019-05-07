#ifndef TS_WNDFIND_H
#define TS_WNDFIND_H

#include <QDialog>
#include "ui_guifind.h"
#include "area.h"
#include "myqt.h"


using namespace ts;

class WndFind : public QDialog, private Ui::GuiFind
{
  Q_OBJECT

public:
  WndFind( const Area&, QWidget* parent );
  ~WndFind();

signals:
  void openEditWindow( int, VNumber );

protected slots:
  void find();
  void showObject( QTreeWidgetItem*, int );
  void somethingChanged();

protected:
  void init();
  int findInRooms();
  int findInMobs();
  int findInItems();
  void addRoom( const QString& where, VNumber, const QString& room_name );
  void addMob( const QString& where, VNumber, const QString& mob_name );
  void addItem( const QString& where, VNumber, const QString& item_name );
  void closeEvent( QCloseEvent* );

private:
  Area m_area;
  QString m_keywords;
  MyQt::ButtonGroup* mp_bgFilters;

};

#endif // TS_WNDSEARCH_H

