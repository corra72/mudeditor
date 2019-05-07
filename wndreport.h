
#ifndef TS_WNDREPORT_H
#define TS_WNDREPORT_H

#include <QDialog>
#include <QString>
#include "ui_guireport.h"


class WndReport : public QDialog, Ui::GuiReport
{
  Q_OBJECT

public:
  WndReport( QWidget* parent );
  ~WndReport();

  void setReport( const QString& );

protected:
  void closeEvent( QCloseEvent* );

protected slots:
  void saveReportAs();

};

#endif  // TS_WNDREPORT_H

