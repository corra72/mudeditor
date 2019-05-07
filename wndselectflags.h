
#ifndef TS_WNDSELECTFLAGS_H
#define TS_WNDSELECTFLAGS_H

#include <QDialog>
#include "ui_guiselectflags.h"
#include "types.h"
#include "myqt.h"
#include "eleuconf.h"


using namespace ts;

//class MyQt::ButtonGroup;


class WndSelectFlags : public QDialog, Ui::GuiSelectFlags
{
  Q_OBJECT

public:
  WndSelectFlags( QList<Flags> flags, QWidget* parent);
  WndSelectFlags( BitVector, const QStringList &list, QWidget* parent );
  WndSelectFlags( BitVector bv, QList<Flags> flags, QWidget* parent );
  ~WndSelectFlags();

  bool okPressed() { return m_okPressed; }
  bool bitVectorChanged() const { return m_bv != m_oldbv; }
  BitVector bitVertorSelected() const { return m_bv; }
  QButtonGroup *getFlags() { return mp_bgFlags; }

protected slots:
  void somethingChanged();
  void saveData();
  void saveAndClose();
  void restoreData();

protected:
  void init();
  void loadData();
  void refreshPanel();
  void refreshTitle();

  void closeEvent( QCloseEvent * );

private:
  BitVector m_bv;
  BitVector m_oldbv;
  bool m_okPressed;
  MyQt::ButtonGroup* mp_bgFlags;

};

#endif  // TS_WNDSELECTFLAGS_H

