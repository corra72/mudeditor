
#ifndef MM_MYQT_H
#define MM_MYQT_H

#include <QButtonGroup>
#include <QAbstractButton>
#include <QMap>
#include <QTreeWidget>

class QMenu;


namespace MyQt
{
  /* My Classes */
  class ButtonGroup : public QButtonGroup
  {
    Q_OBJECT
  public:
    ButtonGroup( QObject *parent = 0 );

    void addButton( QAbstractButton*, int id = -1 );
    QAbstractButton* button( int id ) const { return m_mapButtons.key( id ); }
    int id( QAbstractButton* button ) const { return m_mapButtons.value( button ); }
    int checkedId() const { return m_mapButtons.value( checkedButton(), -1); }
    void setButtonChecked( int id, bool value = true ) { if( button( id ) ) button( id )->setChecked( value ); }
    int maxId() const { return m_maxId; };

  signals:
    void buttonClicked( int );

  private slots:
    void emitSignalWithId( QAbstractButton* );

  private:
    QMap<QAbstractButton*, int> m_mapButtons;
    int m_maxId;

  };

  class TreeWidget : public QTreeWidget
  {
  public:
    TreeWidget( QWidget* parent = 0 ) : QTreeWidget( parent ), mp_menu( 0 ) {}

    void setMenu( QMenu* pMenu ) { mp_menu = pMenu; }

  protected:
    virtual void mousePressEvent( QMouseEvent * );

  private:
    QMenu* mp_menu;

  };

} // namespace MyQt

#endif // MM_MYQT_H
