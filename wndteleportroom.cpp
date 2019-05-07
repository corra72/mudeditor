#include "wndteleportroom.h"
#include <QLineEdit>
#include <QToolButton>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include "validator.h"
#include "config.h"
#include "constantname.h"
#include "selectobject.h"
#include "guiutils.h"


WndTeleportRoom::WndTeleportRoom( const Area& ref_area, Room& ref_room, QWidget* parent )
  : QDialog( parent ), Ui::GuiTeleportRoom(),
    m_area( ref_area ),
    m_room( ref_room )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::WndTeleportRoom( const Area&, Room&, QWidget* ) called." );
#endif
  init();
  loadData();
}

WndTeleportRoom::~WndTeleportRoom()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::~WndTeleportRoom() called." );
#endif
}

void WndTeleportRoom::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::init() called." );
#endif
  setupUi( this );

  mp_tbFlags->setIcon( TS::GetEditIcon() );
  mp_tbRoom->setIcon( TS::GetEditIcon() );

  mp_comboSector->clear();
  for( int i = 0; i < ROOM_SECTOR_END; i++ )
    mp_comboSector->insertItem( i, ConstantName::roomSector( i ).toLower() );

  mp_leToRoom->setValidator( Validator::vnumber() );

  connect( mp_leTeleportTime, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leTeleportCount, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leToRoom, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboSector, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );

  connect( mp_tbRoom, SIGNAL( clicked() ), this, SLOT( editToRoom() ) );
  connect( mp_tbFlags, SIGNAL( clicked() ), this, SLOT( editFlags() ) );

  connect( mp_pbOk, SIGNAL( clicked() ), this, SLOT( saveAndClose() ) );
  connect( mp_pbSave, SIGNAL( clicked() ), this, SLOT( saveData() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );

}

void WndTeleportRoom::refreshPanel()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::refreshPanel() called." );
#endif

  mp_leTeleportTime->setText( QString::number( m_room.teleportTime() ) );
  mp_leTeleportCount->setText( QString::number( m_room.teleportCount() ) );
  mp_comboSector->setCurrentIndex( m_room.teleportSectorType() );
  mp_leToRoom->setText( QString::number( m_room.teleportToRoom() ) );
}

void WndTeleportRoom::refreshTitle()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::refreshTitle() called." );
#endif
  QString sTitle = "";
  sTitle.sprintf( "Opzioni Teleport - Stanza #%ld - %s", m_room.vnumber(), m_room.name().toUtf8().data() );
  if( mp_pbSave->isEnabled() )
    sTitle.append( QString( " [modificate]" ) );
  setWindowTitle( sTitle );
}

void WndTeleportRoom::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::somethingChanged() called." );
#endif
  mp_pbSave->setEnabled( true );
  refreshTitle();
}

void WndTeleportRoom::loadData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::loadData() called." );
#endif
  refreshPanel();
  refreshFlags();
  mp_pbSave->setEnabled( false );
  refreshTitle();
}

void WndTeleportRoom::saveData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::saveData() called." );
#endif
  m_room.setTeleportTime( mp_leTeleportTime->text().toInt() );
  m_room.setTeleportCount( mp_leTeleportCount->text().toInt() );
  m_room.setTeleportSectorType( mp_comboSector->currentIndex() );
  m_room.setTeleportToRoom( mp_leToRoom->text().toLong() );
  mp_pbSave->setEnabled( false );
  refreshTitle();
  emit( dataSaved() );
}

void WndTeleportRoom::saveAndClose()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::saveAndClose() called." );
#endif
  if( mp_pbSave->isEnabled() )
    saveData();
  close();
}

void WndTeleportRoom::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::closeEvent( QCloseEvent* ) called." );
#endif

  if( mp_pbSave->isEnabled() )
  {
    switch( QMessageBox::question( this, TS::MESSAGE_BOX_QUESTION, TS::MESSAGE_ASK_SAVE,
              trUtf8( "Si" ), trUtf8( "No" ), trUtf8( "Annulla" ), 1, 2 ) )
    {
    case 0:
      saveData();
      e->accept();
      break;
    case 1:
      e->accept();
      break;
    default:
      e->ignore();
      break;
    }
  }
  else
    e->accept();
}

void WndTeleportRoom::refreshFlags()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::refreshFlags() called." );
#endif
  mp_leFlags->setText( Utils::bitvector2string( m_room.teleportFlags(), Eleuconf::getAllFlagsCaption(Eleuconf::teleportFlags) ).toLower() );
}

void WndTeleportRoom::editFlags()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::editFlags( int ) called." );
#endif
  BitVector vFlags = m_room.teleportFlags();
  BitVector vRetFlags = SelectObject::roomTeleportFlags( vFlags, this );
  if( vRetFlags == vFlags )
    return;
  m_room.setTeleportFlags( vRetFlags );
  refreshFlags();
  somethingChanged();
}

void WndTeleportRoom::editToRoom()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndTeleportRoom::editToRoom() called." );
#endif
  VNumber def_vnum = mp_leToRoom->text().toLong();
  VNumber vnum_selected = SelectObject::room( m_area, def_vnum, this );

  if( vnum_selected == def_vnum )
    return;

  mp_leToRoom->setText( QString::number( vnum_selected ) );
  somethingChanged();
}
