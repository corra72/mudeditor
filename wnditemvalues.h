#ifndef TS_WNDITEMVALUES_H
#define TS_WNDITEMVALUES_H

#include <QDialog>
#include "ui_guiitemvalues.h"
#include "area.h"

using namespace ts;


class WndItemValues : public QDialog, private Ui::GuiItemValues
{
  Q_OBJECT

public:
  WndItemValues( const Area&, Item&, QWidget* );
  virtual ~WndItemValues();

signals:
  void dataSaved();

protected slots:
  void saveData();
  void somethingChanged();
  void saveAndClose();
  void editValue0();
  void editValue1();
  void editValue2();
  void editValue3();

protected:
  void init();
  void closeEvent( QCloseEvent* );
  void loadData();
  void refreshTitle();
  void refreshPanel();
  void enableValueButton( int, bool );

private:
  const Area& m_area;
  Item& m_item;

};

#endif // TS_WNDITEMVALUES_H
