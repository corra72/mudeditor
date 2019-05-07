#include "guiutils.h"
#include <QApplication>
#include <QLineEdit>
#include <QAbstractButton>
#include <QWidget>
#include <QIcon>
#include <QPushButton>
#include <QToolButton>
#include "selectobject.h"


namespace TS
{
  const bool USE_FLAT_BUTTON_WITH_IMAGE = false;


  QFont GetFixedFont()
  {
    QFont fixFont = QApplication::font();
    fixFont.setFamily( "Courier New" );
    return fixFont;
  }

  QIcon GetEditIcon()
  {
    return QIcon( ":/images/edit.png" );
  }

  QIcon GetAddIcon()
  {
    return QIcon( ":/images/add.png" );
  }

  QIcon GetCopyIcon()
  {
    return QIcon( ":/images/copy.png" );
  }

  QIcon GetRemoveIcon()
  {
    return QIcon( ":/images/remove.png" );
  }

  QIcon GetInitIcon()
  {
      return QIcon( ":/images/init.png" );
  }


  void SetIOAreaIcon( QAbstractButton* pab )
  {
    QToolButton* ptb = qobject_cast< QToolButton* >( pab );
    if( ptb )
    {
      //ptb->setFlat( USE_FLAT_BUTTON_WITH_IMAGE );
      ptb->setText( "" );
      ptb->setIcon( QIcon( ":/images/ioarea.png" ) );
    }
  }

  void SetKreatorLogo( QWidget* pw )
  {
    pw->setWindowIcon( QIcon( ":/images/klogo.png" ) );
  }

  void SetLeftArrow( QAbstractButton* pab )
  {
    QPushButton* ppb = qobject_cast< QPushButton* >( pab );
    if( ppb )
    {
      ppb->setFlat( USE_FLAT_BUTTON_WITH_IMAGE );
      ppb->setText( "" );
      ppb->setIcon( QIcon( ":/images/leftarrow.png" ) );
    }
  }

  void SetRightArrow( QAbstractButton* pab )
  {
    QPushButton* ppb = qobject_cast< QPushButton* >( pab );
    if( ppb )
    {
      ppb->setFlat( USE_FLAT_BUTTON_WITH_IMAGE );
      ppb->setText( "" );
      ppb->setIcon( QIcon( ":/images/rightarrow.png" ) );
    }
  }

  bool SelectSpell( QLineEdit* ple, QWidget* parent )
  {
    int spell_id = SelectObject::spell( ple->text().toInt(), parent );
    if( spell_id == ple->text().toInt() )
      return false;
    ple->setText( QString::number( spell_id ) );
    return true;
  }

} // namespace TS

