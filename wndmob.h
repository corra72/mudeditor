#ifndef TS_WNDMOB_H
#define TS_WNDMOB_H

#include <QDialog>
#include "ui_guimob.h"
#include "area.h"

using namespace ts;


class WndMob : public QDialog, private Ui::GuiMob
{
  Q_OBJECT

public:
  WndMob( Area&, const Mob&, QWidget* );
  virtual ~WndMob();

signals:
  void dataSaved();

protected slots:
  void saveData();
  void restoreData();
  void somethingChanged();
  void saveAndClose();
  void showNextMob();
  void showPrevMob();
  void typeSelected( int );
  void editActs();
  void editNewActs();
  void editAffects();
  void editAffects2();
  void editResistances();
  void editImmunities();
  void editSusceptibilities();
//  void editInits();

protected:
  void init();
  void closeEvent( QCloseEvent* );
  void loadData();
  void refreshTitle();
  void refreshPanel();
  void refreshActs();
  void refreshNewActs();
  void refreshAffects();
  void refreshAffects2();
  void refreshResistances();
  void refreshImmunities();
  void refreshSusceptibilities();
  void refreshAverageHpAndXP();

private:
  Area& m_area;
  Mob m_mob;

};

#endif // TS_WNDMOB_H
