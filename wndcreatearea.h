
#ifndef TS_WNDCREATEAREA_H
#define TS_WNDCREATEAREA_H

#include <QDialog>
#include <QString>
#include "ui_guicreatearea.h"


class WndCreateArea : public QDialog, Ui::GuiCreateArea
{
  Q_OBJECT

public:
  WndCreateArea( const QString& file_name, QWidget* parent );
  ~WndCreateArea();

signals:
  void areaCreated( const QString& );

protected slots:
  void createArea();

};

#endif  // TS_WNDCREATEAREA_H

