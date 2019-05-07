
#ifndef TS_WNDZONE_H
#define TS_WNDZONE_H

#include <QDialog>
#include "ui_guizone.h"
#include "area.h"

using namespace ts;

class QMenu;
class QAction;


class WndZone : public QDialog, Ui::GuiZone
{
  Q_OBJECT

public:
  WndZone( Area&, const Zone&, QWidget* );
  ~WndZone();

signals:
  void dataSaved();

protected slots:
  void saveData();
  void restoreData();
  void somethingChanged();
  void saveAndClose();
  void addNewEmptyLine();
  void removeCommandLine();
  void editCommandLine();
  void lineSelected();
  void refreshView();
  void editFlags();
  void cutCommandLine();
  void copyCommandLine();
  void pasteCommandLine();
  void updateMaxInits();

protected:
  void init();
  void closeEvent( QCloseEvent* );
  void loadData();
  void refreshPanel();
  void refreshTitle();
  void refreshFlags();
  void keyPressEvent( QKeyEvent* );
  QTreeWidgetItem* selectedItem();

private:
  Area& m_area;
  Zone m_zone;
  ZoneCommandList m_zoneCommands;
  long m_lastLineSelected;
  ZoneCommandGroup m_commandGroupCopied;
  QMenu* mp_popupMenu;
  QAction* mp_actPaste;
  QAction* mp_actUpdateMaxInits;
  MyQt::ButtonGroup* mp_bgFilters;

};

#endif // TS_WNDZONE_H

