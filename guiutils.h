
#ifndef TS_GUIUTILS_H
#define TS_GUIUTILS_H


#include <QFont>
#include <QIcon>
class QLineEdit;
class QWidget;
class QAbstractButton;


namespace TS
{
  /* GUI Functions */
  QFont GetFixedFont();
  QIcon GetAddIcon();
  QIcon GetEditIcon();
  QIcon GetCopyIcon();
  QIcon GetRemoveIcon();
  QIcon GetInitIcon();
  void SetKreatorLogo( QWidget* );
  void SetLeftArrow( QAbstractButton* );
  void SetRightArrow( QAbstractButton* );
  void SetIOAreaIcon( QAbstractButton* );

  /* Misc Functions */
  bool SelectSpell( QLineEdit*, QWidget* parent );


} // namespace TS

#endif // TS_GUIUTILS_H
