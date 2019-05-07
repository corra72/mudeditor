#ifndef TS_WNDCHANGEVNUMBERS_H
#define TS_WNDCHANGEVNUMBERS_H

#include <QDialog>
#include "ui_guichangevnumbers.h"
#include "area.h"



class WndChangeVNumbers : public QDialog, private Ui::GuiChangeVNumbers
{
  Q_OBJECT

public:
  WndChangeVNumbers( ts::Area&, QWidget* parent );
  virtual ~WndChangeVNumbers();

signals:
  void vnumbersChanged();

protected slots:
  void startChangingVNumbers();
  void somethingChanged();

protected:
  void init();
  void setStatus( const QString&, ts::VNumber progress = -1 );
  void resetStatus();

private:
  ts::Area& m_area;

};

#endif // TS_WNDCHANGEVNUMBERS_H

