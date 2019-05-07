#include "wndroom.h"
#include <QTextEdit>
#include <QPushButton>
#include <QString>
#include <QLineEdit>
#include <QMessageBox>
#include <QListWidget>
#include <QLabel>
#include <QComboBox>
#include <QHeaderView>
#include <QCloseEvent>
#include "validator.h"
#include "selectobject.h"
#include "config.h"
#include "constantname.h"
#include "wndextradescription.h"
#include "wndexit.h"
#include "wndteleportroom.h"
#include "guiutils.h"
#include "kreatorsettings.h"


namespace
{
  QString exitToString( const Exit& ex, const Area& a )
  {
    QString sRet = "";

    if( a.hasRoom( ex.toRoom() ) )
      sRet.sprintf( "%s -> (%ld) %s", ex.name().toUtf8().data(), ex.toRoom(),
             a.room( ex.toRoom() ).name().toUtf8().data() );
    else
      sRet.sprintf( "%s -> (%ld) Stanza Sconosciuta", ex.name().toUtf8().data(), ex.toRoom() );

    return sRet;
  }

  QString getExitName( const QString& txt )
  {
    QString sRet = txt.section( QString( "->" ), 0, 0 );
    return sRet.trimmed();
  }
}

WndRoom::WndRoom( Area& area, const Room& room, QWidget* parent )
  : QDialog( parent ), Ui::GuiRoom(),
    m_area( area ),
    m_room( room )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::WndRoom( const Area&, const Room& ) called." );
#endif
  init();
  loadData();
  mp_leName->setFocus();
}

WndRoom::~WndRoom()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::~WndRoom() called." );
#endif
}

void WndRoom::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::init() called." );
#endif
  setupUi( this );
  KreatorSettings::instance().loadGuiStatus( "RoomWindow", this );

  mp_tbAddExit->setIcon( TS::GetAddIcon() );
  mp_tbEditExit->setIcon( TS::GetEditIcon() );
  mp_tbRemoveExit->setIcon( TS::GetRemoveIcon() );
  mp_tbAddExtraDescription->setIcon( TS::GetAddIcon() );
  mp_tbEditExtraDescription->setIcon( TS::GetEditIcon() );
  mp_tbRemoveExtraDescription->setIcon( TS::GetRemoveIcon() );
  mp_tbZone->setIcon( TS::GetEditIcon() );
  mp_tbFlags->setIcon( TS::GetEditIcon() );
  TS::SetLeftArrow( mp_pbPrevRoom );
  TS::SetRightArrow( mp_pbNextRoom );

  int i;
  mp_comboSector->clear();
  for( i = 0; i < ROOM_SECTOR_END; i++ )
    mp_comboSector->insertItem( i, ConstantName::roomSector( i ).toLower() );

  mp_comboRiverDirection->clear();
  for( i = 0; i < EXIT_DIRECTION_END; i++ )
    mp_comboRiverDirection->insertItem( i, ConstantName::exitDirection( i ).toLower() );

  mp_teDescription->setFont( TS::GetFixedFont() );

  connect( mp_pbSave, SIGNAL( clicked() ), this, SLOT( saveData() ) );
  connect( mp_pbRestore, SIGNAL( clicked() ), this, SLOT( restoreData() ) );
  connect( mp_pbOk, SIGNAL( clicked() ), this, SLOT( saveAndClose() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
  connect( mp_pbPrevRoom, SIGNAL( clicked() ), this, SLOT( showPrevRoom() ) );
  connect( mp_pbNextRoom, SIGNAL( clicked() ), this, SLOT( showNextRoom() ) );

  mp_leVNumber->setValidator( Validator::vnumber() );
  mp_leZone->setValidator( Validator::vnumber() );
  mp_leCharLimit->setValidator( Validator::unsignedInteger() );
  mp_leRiverSpeed->setValidator( Validator::unsignedInteger() );

  connect( mp_leVNumber, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leZone, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leName, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboSector, SIGNAL( activated( int ) ), this, SLOT( sectorSelected( int  ) ) );
  connect( mp_leCharLimit, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboRiverDirection, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leRiverSpeed, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_teDescription, SIGNAL( textChanged() ), this, SLOT( somethingChanged() ) );

  connect( mp_tbZone, SIGNAL( clicked() ), this, SLOT( selectZone() ) );
  connect( mp_pbTeleport, SIGNAL( clicked() ), this, SLOT( editTeleport() ) );
  connect( mp_tbFlags, SIGNAL( clicked() ), this, SLOT( editFlags() ) );
  connect( mp_lwExtraDescriptions, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ),
    this, SLOT( editExtraDescription( QListWidgetItem* ) ) );
  connect( mp_tbEditExtraDescription, SIGNAL( clicked() ), this, SLOT( editExtraDescription() ) );
  connect( mp_tbAddExtraDescription, SIGNAL( clicked() ), this, SLOT( addExtraDescription() ) );
  connect( mp_tbRemoveExtraDescription, SIGNAL( clicked() ), this, SLOT( removeExtraDescription() ) );
  connect( mp_lwExits, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ),
    this, SLOT( editExit( QListWidgetItem* ) ) );
  connect( mp_tbEditExit, SIGNAL( clicked() ), this, SLOT( editExit() ) );
  connect( mp_tbAddExit, SIGNAL( clicked() ), this, SLOT( addExit() ) );
  connect( mp_tbRemoveExit, SIGNAL( clicked() ), this, SLOT( removeExit() ) );
}

void WndRoom::refreshPanel()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::refreshPanel() called." );
#endif
  mp_leVNumber->setText( QString::number( m_room.vnumber() ) );
  mp_leZone->setText( QString::number( m_room.zone() ) );
  mp_comboSector->setCurrentIndex( m_room.sectorType() );
  sectorSelected( m_room.sectorType() );
  mp_leName->setText( m_room.name() );

  mp_leCharLimit->setText( QString::number( m_room.tunnelLimit() ) );
  mp_comboRiverDirection->setCurrentIndex( m_room.riverDir() );
  mp_leRiverSpeed->setText( QString::number( m_room.riverSpeed() ) );

  mp_teDescription->setPlainText( m_room.description() );
}

void WndRoom::refreshExits()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::refreshExits() called." );
#endif
  mp_lwExits->clear();
  Room::exits_const_iterator it = m_room.exits().begin();
  QString sTmp = "";
  while( it != m_room.exits().end() )
  {
    QListWidgetItem* item = new QListWidgetItem( mp_lwExits );
    item->setText( exitToString( (*it), m_area ) );

#if defined( KREATOR_DEBUG )
    qDebug( "WndRoom::refreshExits(): add exit '%s'.", (*it).dumpObject().toUtf8().data() );
#endif
    ++it;
  }
}

void WndRoom::refreshExtraDescriptions()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::refreshExtraDescriptions() called." );
#endif
  mp_lwExtraDescriptions->clear();
  Room::extra_description_const_iterator it = m_room.extraDescriptions().begin();
  while( it != m_room.extraDescriptions().end() )
  {
  	QListWidgetItem* item = new QListWidgetItem( mp_lwExtraDescriptions );
    item->setText( (*it).keys() );

#if defined( KREATOR_DEBUG )
    qDebug( "WndZone::refreshExtraDescriptions(): add extra '%s'.", (*it).dumpObject().toUtf8().data() );
#endif
    ++it;
  }
}

void WndRoom::refreshTitle()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::refreshTitle() called." );
#endif
  QString sTitle = "";
  sTitle.sprintf( "Stanza #%ld - %s", m_room.vnumber(), m_room.name().toUtf8().data() );
  if( mp_pbSave->isEnabled() )
    sTitle.append( QString( " [modificata]" ) );
  setWindowTitle( sTitle );
}

void WndRoom::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::somethingChanged() called." );
#endif
  mp_pbSave->setEnabled( true );
  refreshTitle();
}

void WndRoom::exitChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::exitChanged() called." );
#endif
  refreshExits();
  somethingChanged();
}

void WndRoom::restoreData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::restoreData() called." );
#endif
  if( m_area.hasRoom( m_room.vnumber() ) )
  {
    m_room = m_area.room( m_room.vnumber() );
    loadData();
  }
  else
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
                   TS::MESSAGE_WARNING_NO_VNUMBER.arg( m_room.vnumber() ) );
}

void WndRoom::loadData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::loadData() called." );
#endif
  refreshPanel();
  refreshFlags();
  refreshExits();
  refreshExtraDescriptions();
  mp_pbSave->setEnabled( false );
  refreshTitle();
}

void WndRoom::saveData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::saveData() called." );
#endif

  VNumber old_vnum = m_room.vnumber();
  VNumber new_vnum = mp_leVNumber->text().toLong();
  if( old_vnum != new_vnum )
  {
    if( m_area.hasRoom( new_vnum )
      && QMessageBox::question( this, TS::MESSAGE_BOX_QUESTION,
                        TS::MESSAGE_ASK_OVERWRITE_VNUMBER.arg( new_vnum ),
                        trUtf8( "Si" ), trUtf8( "No" ), QString(), 1, 1 )
      )
      return;

    if( !QMessageBox::question( this, TS::MESSAGE_BOX_QUESTION,
                        TS::MESSAGE_ASK_UPDATE_REFERENCES.arg( old_vnum ).arg( new_vnum ),
                        trUtf8( "Si" ), trUtf8( "No" ), QString(), 0, 1 )
      )
      m_area.changeRoomVNumber( old_vnum, new_vnum, true );

  }

  m_room.setVNumber( new_vnum );
  m_room.setZone( mp_leZone->text().toInt() );
  m_room.setSectorType( mp_comboSector->currentIndex() );
  m_room.setName( mp_leName->text() );

  m_room.setTunnelLimit( mp_leCharLimit->text().toInt() );
  m_room.setRiverDir( mp_comboRiverDirection->currentIndex() );
  m_room.setRiverSpeed( mp_leRiverSpeed->text().toInt() );
  m_room.setDescription( mp_teDescription->toPlainText() );

  m_area.addRoom( m_room );
  m_area.setRoomsChanged();
  m_area.sortRooms();

  mp_pbSave->setEnabled( false );
  refreshTitle();
  emit( dataSaved() );
}

void WndRoom::saveAndClose()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::saveAndClose() called." );
#endif
  if( mp_pbSave->isEnabled() )
    saveData();
  close();
}

void WndRoom::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::closeEvent( QCloseEvent* ) called." );
#endif

  if( mp_pbSave->isEnabled() )
  {
    if( KreatorSettings::instance().applyWithoutAsking() )
    {
      saveData();
      e->accept();
    }
    else
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
  }
  else
    e->accept();

  if( e->isAccepted() )
    KreatorSettings::instance().saveGuiStatus( "RoomWindow", this );

}

void WndRoom::sectorSelected( int iSector )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::sectorSelected( int ) called." );
#endif
  mp_pbTeleport->setEnabled( iSector == ROOM_SECTOR_TELEPORT );
  bool isRiver = ( iSector == ROOM_SECTOR_WATER_NO_SWIM || iSector == ROOM_SECTOR_UNDERWATER );
  mp_labelRiverDirection->setEnabled( isRiver );
  mp_comboRiverDirection->setEnabled( isRiver );
  mp_labelRiverSpeed->setEnabled( isRiver );
  mp_leRiverSpeed->setEnabled( isRiver );

  if( iSector != m_room.sectorType() )
    somethingChanged();
}

void WndRoom::selectZone()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::selectZone() called." );
#endif
  VNumber vnumSelected = SelectObject::zone( m_area, mp_leZone->text().toLong(), this );
  if( vnumSelected == mp_leZone->text().toLong() )
    return;
  mp_leZone->setText( QString::number( vnumSelected ) );
}

void WndRoom::editTeleport()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::editTeleport() called." );
#endif
  WndTeleportRoom *pDialog = new WndTeleportRoom( m_area, m_room, this );
  connect( pDialog, SIGNAL( dataSaved() ), this, SLOT( somethingChanged() ) );
  pDialog->show();
  pDialog->setFixedSize( pDialog->size() );
  pDialog->exec();
  delete pDialog;
}

void WndRoom::refreshFlags()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::refreshFlags() called." );
#endif
  mp_leFlags->setText( Utils::bitvector2string( m_room.flags(), Eleuconf::getAllFlagsCaption(Eleuconf::roomFlags) ).toLower() );
  mp_labelCharLimit->setEnabled( m_room.hasFlag( ROOM_FLAG_TUNNEL ) );
  mp_leCharLimit->setEnabled( m_room.hasFlag( ROOM_FLAG_TUNNEL ) );
}

void WndRoom::editFlags()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::editFlags( int ) called." );
#endif
  BitVector vFlags = m_room.flags();
  BitVector vRetFlags = SelectObject::roomFlags( vFlags, this );
  if( vRetFlags == vFlags )
    return;
  m_room.setFlags( vRetFlags );
  refreshFlags();
  somethingChanged();
}

void WndRoom::showNextRoom()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::showNextRoom() called." );
#endif
  QCloseEvent ce;
  closeEvent( &ce );
  if( !ce.isAccepted() )
    return;

  VNumber next_vnum = m_area.nextObjectInList( ObjectData::Object_Room, m_room.vnumber() );
  if( next_vnum != VNumberInvalid )
  {
    m_room = m_area.room( next_vnum );
    loadData();
    mp_pbPrevRoom->setEnabled( true );
  }
  else
    mp_pbNextRoom->setEnabled( false );
}

void WndRoom::showPrevRoom()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::showPrevRoom() called." );
#endif
  QCloseEvent ce;
  closeEvent( &ce );
  if( !ce.isAccepted() )
    return;

  VNumber prev_vnum = m_area.prevObjectInList( ObjectData::Object_Room, m_room.vnumber() );
  if( prev_vnum != VNumberInvalid )
  {
    m_room = m_area.room( prev_vnum );
    loadData();
    mp_pbNextRoom->setEnabled( true );
  }
  else
    mp_pbPrevRoom->setEnabled( false );
}

void WndRoom::editExtraDescription()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::editExtraDescription() called." );
#endif
  QList<QListWidgetItem*> wiList = mp_lwExtraDescriptions->selectedItems();
  if( wiList.empty() )
    return;

  editExtraDescription( wiList.first() );
}

void WndRoom::editExtraDescription( QListWidgetItem* item )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::editExtraDescription( QListWidgetItem* ) called." );
#endif

  if( !item )
    return;

  if( m_room.hasExtraDescription( item->text() ) )
  {
    ExtraDescription ex = m_room.extraDescription( item->text() );
    WndExtraDescription *pDialog = new WndExtraDescription( ex, this );
    pDialog->show();
    pDialog->setFixedSize( pDialog->size() );
    pDialog->exec();

    if( pDialog->okPressed() && ( pDialog->keysChanged() || pDialog->descriptionChanged() ) )
    {
      if( item->text() == "nuovaextra" )
        m_room.removeExtraDescription( "nuovaextra" );
      m_room.addExtraDescription( pDialog->extraDescriptionSelected() );
      refreshExtraDescriptions();
      somethingChanged();
    }

    delete pDialog;
  }
}

void WndRoom::addExtraDescription()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::addExtraDescription() called." );
#endif
  QString sKey = trUtf8( "nuovaextra" );
  QString sDesc = trUtf8( "Non vedi nulla di particolare qui..." );

  if( m_room.hasExtraDescription( sKey ) )
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
              trUtf8( "Esiste gia' una descrizione con key 'nuovaextra'.\n"
                      "Modificate quella esistente prima di creare una nuova extra." ) );
    return;
  }

  m_room.addExtraDescription( ExtraDescription( sKey, sDesc ) );
  refreshExtraDescriptions();
  somethingChanged();
}

void WndRoom::removeExtraDescription()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::removeExtraDescription() called." );
#endif
  QList<QListWidgetItem*> wiList = mp_lwExtraDescriptions->selectedItems();
  if( wiList.empty() )
    return;

  QString sKey = (wiList.first())->text();

  if( m_room.hasExtraDescription( sKey ) )
  {
    m_room.removeExtraDescription( sKey );
    refreshExtraDescriptions();
    somethingChanged();
  }
}

void WndRoom::editExit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::editExit() called." );
#endif
  QList<QListWidgetItem*> wiList = mp_lwExits->selectedItems();
  if( wiList.empty() )
    return;

  editExit( wiList.first() );
}

void WndRoom::editExit( QListWidgetItem* item )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::editExit( QListWidgetItem* ) called." );
#endif

  if( !item )
    return;

  QString sExitName = getExitName( item->text() );
  qDebug( "Edito l'uscita %s", sExitName.toUtf8().data() );
  if( m_room.hasExit( sExitName ) )
  {
    Exit exit = m_room.exit( sExitName );
    WndExit *pExitDialog = new WndExit( m_area, m_room, exit, this );
    connect( pExitDialog, SIGNAL( dataSaved() ), this, SLOT( exitChanged() ) );
    pExitDialog->show();
    pExitDialog->setFixedSize( pExitDialog->size() );
    pExitDialog->exec();
    delete pExitDialog;
  }
}

void WndRoom::addExit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::addExit() called." );
#endif
  QString sKey = "nuovauscita";

  if( m_room.hasExit( sKey ) )
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
              trUtf8( "Esiste gia' un'uscita con nome 'nuovauscita'.\n"
                      "Modificate quella esistente prima di creare una nuova uscita." ) );
    return;
  }

  m_room.addExit( Exit( sKey, m_room.vnumber() ) );
  refreshExits();
  somethingChanged();
}

void WndRoom::removeExit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndRoom::removeExtraDescription() called." );
#endif
  QList<QListWidgetItem*> wiList = mp_lwExits->selectedItems();
  if( wiList.empty() )
    return;

  QListWidgetItem* item = wiList.first();

  QString sExitName = getExitName( item->text() );

  if( m_room.hasExit( sExitName ) )
  {
    m_room.removeExit( sExitName );
    refreshExits();
    somethingChanged();
  }
}

