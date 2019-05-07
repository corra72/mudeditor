#ifndef TS_WNDITEMAFFECTS_H
#define TS_WNDITEMAFFECTS_H

#include <QDialog>
#include "ui_guiitemaffects.h"
#include "area.h"

using namespace ts;


class WndItemAffects : public QDialog, private Ui::GuiItemAffects
{
  Q_OBJECT

public:
  WndItemAffects( const Area&, Item&, QWidget* );
  virtual ~WndItemAffects();

signals:
  void dataSaved();

protected slots:
  void saveData();
  void somethingChanged();
  void saveAndClose();
  void editAffect0();
  void editAffect1();
  void editAffect2();
  void editAffect3();
  void editAffect4();

protected:
  void init();
  void closeEvent( QCloseEvent* );
  void loadData();
  void refreshTitle();
  void refreshPanel();
  QLineEdit* getLineEditAffect( int );
  QComboBox* getComboBoxAffect( int );
  void editAffect( int );

private:
  const Area& m_area;
  Item& m_item;

};

#endif // TS_WNDITEMAFFECTS_H
