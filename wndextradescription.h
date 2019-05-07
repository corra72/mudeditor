#ifndef TS_WNDEXTRADESCRIPTION_H
#define TS_WNDEXTRADESCRIPTION_H

#include "ui_guiextradescription.h"
#include "extradescription.h"
#include "syntax.h"

using namespace ts;


class WndExtraDescription : public QDialog, private Ui::GuiExtraDescription
{
  Q_OBJECT

public:
  WndExtraDescription( const ExtraDescription&, QWidget* );
  virtual ~WndExtraDescription();

  bool okPressed() { return m_okPressed; }
  bool keysChanged() const { return m_extraDescription.keys() != m_oldExtraDescription.keys(); }
  bool descriptionChanged() const { return m_extraDescription.description() != m_oldExtraDescription.description(); }
  const ExtraDescription& extraDescriptionSelected() const { return m_extraDescription; }

protected slots:
  void saveData();
  void restoreData();
  void somethingChanged();
  void saveAndClose();

protected:
  void init();
  void closeEvent( QCloseEvent* );
  void loadData();
  void refreshTitle();
  void refreshPanel();

private:
  ExtraDescription m_extraDescription;
  ExtraDescription m_oldExtraDescription;
  bool m_okPressed;
  Syntax *highLighter;

};

#endif // TS_WNDEXTRADESCRIPTION_H
