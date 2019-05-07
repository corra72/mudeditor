
#include "myqt.h"
#include <QMouseEvent>
#include <QMenu>

namespace MyQt
{

  ButtonGroup::ButtonGroup( QObject *parent )
    : QButtonGroup( parent )
  {
    m_maxId = 0;
    setExclusive( false );

    connect( this, SIGNAL( buttonClicked( QAbstractButton* ) ),
      this, SLOT( emitSignalWithId( QAbstractButton* ) ) );
  }

  void ButtonGroup::addButton( QAbstractButton *button, int id )
  {
    QButtonGroup::addButton( button );
    if( id != -1 )
    {
      m_mapButtons[ button ] = id;
      if( id >= m_maxId )
        m_maxId = id;
    }
  }

  void ButtonGroup::emitSignalWithId( QAbstractButton *button )
  {
    emit buttonClicked( id( button ) );
  }


  /*****************/

  void TreeWidget::mousePressEvent( QMouseEvent* e )
  {
    if( e->button() == Qt::RightButton )
    {
      if( mp_menu )
        mp_menu->popup( e->globalPos() );
    }

    QTreeWidget::mousePressEvent( e );
  }

} // namespace MyQt
