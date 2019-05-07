#ifndef TS_WNDSHOP_H
#define TS_WNDSHOP_H

#include <QDialog>
#include "ui_guishop.h"
#include "area.h"

using namespace ts;


class WndShop : public QDialog, private Ui::GuiShop
{
  Q_OBJECT

public:
  WndShop( Area&, const Shop&, QWidget* );
  virtual ~WndShop();

signals:
  void dataSaved();

protected slots:
  void saveData();
  void restoreData();
  void somethingChanged();
  void saveAndClose();
  void showNextShop();
  void showPrevShop();
  void editSell0();
  void editSell1();
  void editSell2();
  void editSell3();
  void editSell4();
  void editMob();
  void editRoom();

protected:
  void init();
  void closeEvent( QCloseEvent* );
  void loadData();
  void refreshTitle();
  void refreshPanel();

private:
  Area& m_area;
  Shop m_shop;

};

#endif // TS_WNDSHOP_H
