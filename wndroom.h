
#ifndef TS_WNDROOM_H
#define TS_WNDROOM_H

#include <QDialog>
#include "ui_guiroom.h"
#include "area.h"

using namespace ts;

class QListWidgetItem;


class WndRoom : public QDialog, Ui::GuiRoom
{
  Q_OBJECT

public:
  WndRoom( Area&, const Room&, QWidget* );
  ~WndRoom();

signals:
  void dataSaved();

public slots:
  void somethingChanged();

protected slots:
  void exitChanged();
  void saveData();
  void saveAndClose();
  void restoreData();
  void refreshExits();
  void refreshExtraDescriptions();
  void sectorSelected( int );
  void selectZone();
  void editTeleport();
  void editFlags();
  void editExtraDescription();
  void editExtraDescription( QListWidgetItem* );
  void removeExtraDescription();
  void addExtraDescription();
  void editExit();
  void editExit( QListWidgetItem* );
  void removeExit();
  void addExit();
  void showNextRoom();
  void showPrevRoom();

protected:
  void init();
  void loadData();
  void refreshView();
  void refreshPanel();
  void refreshTitle();
  void refreshFlags();

  void closeEvent( QCloseEvent * );

private:
  Area& m_area;
  Room m_room;

};


#endif  // TS_WNDROOM_H

