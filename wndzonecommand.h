
#ifndef TS_WNDZONECOMMAND_H
#define TS_WNDZONECOMMAND_H

#include <QDialog>
#include "ui_guizonecommand.h"
#include "area.h"

using namespace ts;


class WndZoneCommand : public QDialog, private Ui::GuiZoneCommand
{
  Q_OBJECT

public:
  WndZoneCommand( const Area&, ZoneCommandList&, const ZoneCommand&, QWidget* parent );
  virtual ~WndZoneCommand();

signals:
  void dataSaved();

protected slots:
  void saveData();
  void restoreData();
  void somethingChanged();
  void saveAndClose();
  void typeSelected( int );
  void generateComment();
  void editArgument1();
  void editArgument2();
  void editArgument3();
  void editArgument4();

protected:
  void init();
  void closeEvent( QCloseEvent* );
  void loadData();
  void refreshTitle();
  void refreshPanel();
  void editArgument( int );

private:
  const Area& m_area;
  ZoneCommandList& m_zoneCommandList;
  ZoneCommand m_command;
  int m_prevTypeId;

};

#endif // TS_WNDZONECOMMAND_H
