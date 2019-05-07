#ifndef TS_WNDTELEPORTROOM_H
#define TS_WNDTELEPORTROOM_H

#include <QDialog>
#include "ui_guiteleportroom.h"
#include "area.h"

using namespace ts;


class WndTeleportRoom : public QDialog, private Ui::GuiTeleportRoom
{
  Q_OBJECT

public:
  WndTeleportRoom( const Area&, Room&, QWidget* );
  virtual ~WndTeleportRoom();

signals:
  void dataSaved();

protected slots:
  void saveData();
  void somethingChanged();
  void saveAndClose();
  void refreshFlags();
  void editFlags();
  void editToRoom();

protected:
  void init();
  void closeEvent( QCloseEvent* );
  void loadData();
  void refreshTitle();
  void refreshPanel();

private:
  const Area& m_area;
  Room& m_room;

};

#endif // TS_WNDTELEPORTROOM_H
