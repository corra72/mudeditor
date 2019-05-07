#include "wndchangevnumbers.h"
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QCheckBox>
#include <QProgressBar>
#include <QLabel>
#include <QApplication>
#include "validator.h"
#include "config.h"


using namespace ts;


WndChangeVNumbers::WndChangeVNumbers( Area& area, QWidget* parent )
  : QDialog( parent ), Ui::GuiChangeVNumbers(), m_area( area )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndChangeVNumbers::WndChangeVNumbers( Area&, QWidget* ) called." );
#endif
  init();
}

WndChangeVNumbers::~WndChangeVNumbers()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndChangeVNumbers::~WndChangeVNumbers() called." );
#endif
}

void WndChangeVNumbers::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndChangeVNumbers::init() called." );
#endif

  setupUi( this );
  setModal( true );

  mp_leFrom->setValidator( Validator::vnumber() );
  mp_leFromNew->setValidator( Validator::vnumber() );
  mp_leTo->setValidator( Validator::vnumber() );

  setWindowTitle( trUtf8( "Rinumera VNumbers - TS Kreator" ) );

  connect( mp_pbStart, SIGNAL( clicked() ), this, SLOT( startChangingVNumbers() ) );
  connect( mp_pbClose, SIGNAL( clicked() ), this, SLOT( close() ) );

  connect( mp_leFrom, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leFromNew, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leTo, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_cbRooms, SIGNAL( stateChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_cbMobs, SIGNAL( stateChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_cbItems, SIGNAL( stateChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_cbShops, SIGNAL( stateChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_cbComments, SIGNAL( stateChanged( int ) ), this, SLOT( somethingChanged() ) );

  mp_pbStart->setEnabled( false );
  resetStatus();
}

void WndChangeVNumbers::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndChangeVNumbers::somethingChanged() called." );
#endif
  mp_pbStart->setEnabled( true );
}

void WndChangeVNumbers::setStatus( const QString& msg, VNumber progress )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndChangeVNumbers::setStatus( const QString&, VNumber ) called." );
#endif
  if( progress >= 0 )
    mp_progress->setValue( progress );

  mp_teStatus->append( msg );
  QApplication::processEvents();
}

void WndChangeVNumbers::resetStatus()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndChangeVNumbers::resetStatus() called." );
#endif
  mp_teStatus->setPlainText( trUtf8( "Pronto." ) );
  mp_progress->setEnabled( false );
}

void WndChangeVNumbers::startChangingVNumbers()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndChangeVNumbers::startChangingVNumbers() called." );
#endif

  if( mp_leFrom->text().simplified().isEmpty() )
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
       TS::MESSAGE_WARNING_FIELD_IS_EMPTY.arg( trUtf8( "Dal VNumber" ) ) );
    mp_leFrom->setFocus();
    return;
  }

  if( mp_leTo->text().simplified().isEmpty() )
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
       TS::MESSAGE_WARNING_FIELD_IS_EMPTY.arg( trUtf8( "Al VNumber" ) ) );
    mp_leTo->setFocus();
    return;
  }

  if( mp_leFromNew->text().simplified().isEmpty() )
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
       TS::MESSAGE_WARNING_FIELD_IS_EMPTY.arg( trUtf8( "Nuovo VNumber" ) ) );
    mp_leFromNew->setFocus();
    return;
  }

  VNumber vnumFrom = mp_leFrom->text().toLong();
  VNumber vnumFromNew = mp_leFromNew->text().toLong();
  VNumber vnumTo = mp_leTo->text().toLong();

  if( vnumTo < vnumFrom )
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
                   trUtf8( "Il campo 'Al VNumber' contiene un valore inferiore a quello 'Dal VNumber'." ) );
    mp_leTo->setFocus();
    return;
  }

  QApplication::setOverrideCursor( Qt::WaitCursor );
  resetStatus();
  mp_progress->setEnabled( true );
  mp_progress->setRange( 0, vnumTo - vnumFrom );
  mp_progress->setValue( 0 );
  mp_progress->setTextVisible( true );
  QApplication::processEvents();

  VNumber new_vnumber = vnumFromNew;
  VNumber old_vnumber;
  int iProgress = 0;


  for( old_vnumber = vnumFrom; old_vnumber <= vnumTo; old_vnumber++ )
  {
    int iRif = 0;
    if( mp_cbRooms->isChecked() )
      iRif += m_area.changeRoomVNumber( old_vnumber, new_vnumber, mp_cbComments->isChecked() );
    if( mp_cbMobs->isChecked() )
      iRif += m_area.changeMobVNumber( old_vnumber, new_vnumber, mp_cbComments->isChecked() );
    if( mp_cbItems->isChecked() )
      iRif += m_area.changeItemVNumber( old_vnumber, new_vnumber, mp_cbComments->isChecked() );
    if( mp_cbShops->isChecked() )
      iRif += m_area.changeShopVNumber( old_vnumber, new_vnumber );
    iProgress++;
    setStatus( trUtf8( "Aggiornati %1 riferimenti nel passaggio dal VNumber %2 al %3." ).arg( iRif ).arg( old_vnumber ).arg( new_vnumber ), iProgress );
    new_vnumber++;
  }

  mp_progress->setTextVisible( false );
  mp_progress->setValue( 0 );
  mp_pbStart->setEnabled( false );
  QApplication::restoreOverrideCursor();
  QApplication::processEvents();

  m_area.setRoomsChanged();
  m_area.setZonesChanged();
  m_area.setItemsChanged();
  m_area.setMobsChanged();
  m_area.setShopsChanged();
  emit( vnumbersChanged() );
}
