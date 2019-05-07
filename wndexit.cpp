#include "wndexit.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
#include "validator.h"
#include "config.h"
#include "constantname.h"
#include "selectobject.h"
#include "guiutils.h"
#include "kreatorsettings.h"


WndExit::WndExit( Area& ref_area, Room& ref_room, const Exit& ex, QWidget* parent )
  : QDialog( parent ), Ui::GuiExit(),
    m_area( ref_area ),
    m_room( ref_room ),
    m_exit( ex ),
    m_originalName( ex.name() ),
    m_doorInit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::WndExit( Area&, Room&, const Exit&, QWidget* ) called." );
#endif
  init();
  loadData();
  refreshPanel();
}

WndExit::~WndExit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::~WndExit() called." );
#endif
}

void WndExit::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::init() called." );
#endif
  setupUi( this );
  mp_teDescription->setFont( TS::GetFixedFont() );
  mp_tbFlags->setIcon( TS::GetEditIcon() );
  mp_tbToRoom->setIcon( TS::GetEditIcon() );
  mp_tbKey->setIcon( TS::GetEditIcon() );

  int i = 0;
  mp_comboDirection->clear();
  for( i = 0; i < EXIT_DIRECTION_END; i++ )
    mp_comboDirection->insertItem( i, ConstantName::exitDirection( i ).toLower() );

  mp_comboOpenCommand->clear();
  for( i = 0; i < EXIT_OPEN_COMMAND_END; i++ )
    mp_comboOpenCommand->insertItem( i, ConstantName::exitOpenCommand( i ).toLower());

  mp_comboInit->clear();
  for( i = 0; i < EXIT_STATUS_END; i++ )
    mp_comboInit->insertItem( i, ConstantName::exitStatus( i ).toLower() );

  mp_leToRoom->setValidator( Validator::vnumber() );
  mp_leKey->setValidator( Validator::vnumber() );

  connect( mp_tbFlags, SIGNAL( clicked() ), this, SLOT( editFlags() ) );
  connect( mp_tbToRoom, SIGNAL( clicked() ), this, SLOT( editToRoom() ) );
  connect( mp_tbKey, SIGNAL( clicked() ), this, SLOT( editKey() ) );

  connect( mp_pbSave, SIGNAL( clicked() ), this, SLOT( saveData() ) );
  connect( mp_pbRestore, SIGNAL( clicked() ), this, SLOT( restoreData() ) );
  connect( mp_pbOk, SIGNAL( clicked() ), this, SLOT( saveAndClose() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
  connect( mp_pbOpenReverseExit, SIGNAL( clicked() ), this, SLOT( openReverseExit() ) );

  connect( mp_comboDirection, SIGNAL( activated( int ) ), this, SLOT( directionChanged( int ) ) );
  connect( mp_comboOpenCommand, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboInit, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leName, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leListName, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leToName, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leFromName, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leRevDirName, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leKeys, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leToRoom, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leKey, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_teDescription, SIGNAL( textChanged() ), this, SLOT( somethingChanged() ) );

}

void WndExit::refreshPanel()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::refreshPanel() called." );
#endif
  refreshNames();
  mp_leKeys->setText( m_exit.keys() );
  refreshFlags();
  refreshToRoom();
  refreshKey();
  mp_comboOpenCommand->setCurrentIndex( m_exit.openCommand() );
  mp_teDescription->setPlainText( m_exit.description() );
}

void WndExit::refreshInit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::refreshInit() called." );
#endif
  VNumber vnum_zone = m_area.zoneReferenced( m_exit.fromRoom() );
  if( vnum_zone != ZoneVNumberInvalid )
  {
    Zone z = m_area.zone( vnum_zone );
    ZoneCommandList zcl = z.commandList();
    m_doorInit = zcl.door( m_exit );
    if( !m_doorInit.isDoorInit() )
      m_doorInit.setId( zcl.topId() );
  }
  else
    m_doorInit = ZoneCommand();

  if( m_doorInit.isDoorInit() )
  {
    mp_comboInit->setCurrentIndex( m_doorInit.argument( ZoneCommand::Argument3 ) );
    mp_comboInit->setEnabled( true );
  }
  else
  {
    mp_comboInit->setCurrentIndex( 0 );
    mp_comboInit->setEnabled( true );
  }
}

void WndExit::refreshToRoom()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::refreshToRoom() called." );
#endif
  mp_leToRoom->setText( QString::number( m_exit.toRoom() ) );
}

void WndExit::refreshKey()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::refreshKey() called." );
#endif
  mp_leKey->setText( QString::number( m_exit.doorKey() ) );
}

void WndExit::refreshFlags()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::refreshFlags() called." );
#endif
  mp_leFlags->setText( Utils::bitvector2string( m_exit.flags(), Eleuconf::getAllFlagsCaption(Eleuconf::exitFlags) ).toLower() );

  bool enableEditDoor = m_exit.hasDoor();
  mp_comboOpenCommand->setEnabled( enableEditDoor );
  mp_comboInit->setEnabled( enableEditDoor );
  mp_labelOpenCommand->setEnabled( enableEditDoor );
  mp_labelInit->setEnabled( enableEditDoor );
  mp_leKeys->setEnabled( enableEditDoor );
  mp_labelKeys->setEnabled( enableEditDoor );
  mp_comboInit->setEnabled( enableEditDoor );
  if( !enableEditDoor )
  {
    mp_comboOpenCommand->setCurrentIndex( 0);
    mp_comboInit->setCurrentIndex( 0 );
    mp_leKeys->setText( "" );
  }
}

void WndExit::refreshNames()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::refreshNames() called." );
#endif

  mp_comboDirection->setCurrentIndex( m_exit.hasDirection() ? m_exit.direction() : EXIT_DIRECTION_END );
  mp_leName->setText( m_exit.name() );
  mp_leListName->setText( m_exit.listName() );
  mp_leToName->setText( m_exit.toName() );
  mp_leFromName->setText( m_exit.fromName() );
  mp_leRevDirName->setText( m_exit.revDirName() );

  bool enableEditName = !m_exit.hasDirection();
  mp_labelName->setEnabled( enableEditName );
  mp_labelListName->setEnabled( enableEditName );
  mp_labelToName->setEnabled( enableEditName );
  mp_labelFromName->setEnabled( enableEditName );
  mp_labelRevDirName->setEnabled( enableEditName );
  mp_leName->setEnabled( enableEditName );
  mp_leListName->setEnabled( enableEditName );
  mp_leToName->setEnabled( enableEditName );
  mp_leFromName->setEnabled( enableEditName );
  mp_leRevDirName->setEnabled( enableEditName );
}

void WndExit::refreshTitle()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::refreshTitle() called." );
#endif
  QString sTitle = "";
  if( m_area.hasRoom( m_exit.toRoom() ) )
    sTitle.sprintf( "Uscita '%s' verso (%ld) %s", m_exit.name().toUtf8().data(), m_exit.toRoom(), m_area.room( m_exit.toRoom() ).name().toUtf8().data() );
  else
    sTitle.sprintf( "Uscita '%s' verso (%ld) Stanza Sconosciuta", m_exit.name().toUtf8().data(), m_exit.toRoom() );

  if( mp_pbSave->isEnabled() )
    sTitle.append( QString( " [modificata]" ) );

  setWindowTitle( sTitle );
}

void WndExit::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::somethingChanged() called." );
#endif
  mp_pbSave->setEnabled( true );
  refreshTitle();
}

void WndExit::restoreData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::restoreData() called." );
#endif
  if( m_room.hasExit( m_originalName ) )
  {
    m_exit = m_room.exit( m_originalName );
    loadData();
  }
  else
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
                   TS::MESSAGE_WARNING_NO_EXIT.arg( m_originalName ) );
}

void WndExit::loadData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::loadData() called." );
#endif
  refreshPanel();
  refreshInit();
  mp_pbSave->setEnabled( false );
  refreshTitle();
}

void WndExit::saveData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::saveData() called." );
#endif

  if( mp_comboDirection->currentIndex() == EXIT_DIRECTION_END )
  {
    m_exit.setName( mp_leName->text() );
    m_exit.setListName( mp_leListName->text() );
    m_exit.setToName( mp_leToName->text() );
    m_exit.setFromName( mp_leFromName->text() );
    m_exit.setRevDirName( mp_leRevDirName->text() );
  }

  if( m_originalName != m_exit.name() )
  {
    if( m_room.hasExit( m_exit.name() ) )
    {
      switch( QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
              trUtf8( "L'uscita e' stata modificata con un nome gia' esistente per questa stanza.\n"
              "Vuoi salvare ugualmente le modifiche? Se scegli la l'opzione di duplicazione verra'\n"
              "lasciata anche la vecchia uscita." ),
              trUtf8( "Si" ), trUtf8( "No" ), trUtf8( "Si, duplicandola" ), 0, 1 ) )
      {
      case 0:
        {
          if( m_room.hasExit( m_originalName ) )
            m_room.removeExit( m_originalName );
        }
        break;
      case 1:
        return;
      case 2:
        break;
      default:
        return;
      }
    }
  }

  m_exit.setKeys( mp_leKeys->text() );
  m_exit.setToRoom( mp_leToRoom->text().toLong() );
  m_exit.setDoorKey( mp_leKey->text().toLong() );
  m_exit.setOpenCommand( mp_comboOpenCommand->currentIndex() );
  m_exit.setStatus( mp_comboInit->currentIndex() );
  m_exit.setDescription( mp_teDescription->toPlainText() );
  m_room.addExit( m_exit );

  VNumber vnum_zone = m_area.zoneReferenced( m_exit.fromRoom() );
  if( vnum_zone != ZoneVNumberInvalid )
  {
    Zone z = m_area.zone( vnum_zone );
    ZoneCommandList zcl = z.commandList();
    if( m_originalName == m_exit.name() )
      zcl.removeCommand( m_doorInit.id() );

    if( m_exit.hasDirection() && m_exit.hasDoor() && !m_exit.isOpened() )
    {
      m_doorInit.setType( 'D' );
      m_doorInit.setArgument( ZoneCommand::Argument1, m_exit.fromRoom() );
      m_doorInit.setArgument( ZoneCommand::Argument2, m_exit.direction() );
      m_doorInit.setArgument( ZoneCommand::Argument3, m_exit.status() );
      m_doorInit.setComment( m_area.generateComment( m_doorInit ) );
      zcl.addCommand( m_doorInit );
    }
    z.setCommandList( zcl );
    m_area.addZone( z );
  }

  if( m_originalName == "nuovauscita" )
    m_room.removeExit( m_originalName );

  m_originalName = m_exit.name();

  mp_pbSave->setEnabled( false );
  refreshTitle();
  emit( dataSaved() );
}

void WndExit::saveAndClose()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::saveAndClose() called." );
#endif
  if( mp_pbSave->isEnabled() )
    saveData();
  close();
}

void WndExit::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::closeEvent( QCloseEvent* ) called." );
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
}

void WndExit::directionChanged( int dir_id )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::directionChanged( int ) called." );
#endif

  if( m_exit.direction() != dir_id )
  {
    if( dir_id == EXIT_DIRECTION_END )
      m_exit.setDirection( EXIT_DIRECTION_INVALID );
    else
      m_exit.setDirection( dir_id );

    refreshNames();
    somethingChanged();
  }
}

void WndExit::editFlags()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::editFlags() called." );
#endif
  BitVector vFlags = m_exit.flags();
  BitVector vRetFlags = SelectObject::exitFlags( vFlags, this );
  if( vRetFlags == vFlags )
    return;
  m_exit.setFlags( vRetFlags );
  refreshFlags();
  somethingChanged();
}

void WndExit::editToRoom()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::editToRoom() called." );
#endif
  VNumber def_vnum = mp_leToRoom->text().toLong();
  VNumber vnum_selected = SelectObject::exitRoom( m_area, def_vnum, this );

  if( vnum_selected == def_vnum )
    return;

  mp_leToRoom->setText( QString::number( vnum_selected ) );
  somethingChanged();
}

void WndExit::editKey()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::editKey() called." );
#endif
  VNumber def_vnum = mp_leKey->text().toLong();
  VNumber vnum_selected = SelectObject::key( m_area, def_vnum, this );

  if( vnum_selected == def_vnum )
    return;

  mp_leKey->setText( QString::number( vnum_selected ) );
  somethingChanged();
}

void WndExit::openReverseExit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExit::openReverseExit() called." );
#endif

  if( mp_pbSave->isEnabled() )
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
      trUtf8( "Sono state rilevate alcune modifiche nell'uscita che non sono state applicate.\n"
              "Prima di aprire l'uscita inversa e' necessario salvare le modifiche." ), trUtf8( "Ok" ) );
    return;
  }

  if( !m_area.hasRoom( m_exit.toRoom() ) )
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
      trUtf8( "Impossibile eseguire l'operazione perche' la stanza dell'uscita inversa non esiste." ) );
    return;
  }

  bool exit_opened = m_area.openReverseRoomExit( m_exit, false );
  if( !exit_opened )
  {
    switch( QMessageBox::question( this, TS::MESSAGE_BOX_QUESTION,
              trUtf8( "Esiste gia' un'uscita inversa, si vuole sovrascriverla?" ),
              trUtf8( "Si" ), trUtf8( "No" ), trUtf8( "Annulla" ), 1, 2 ) )
    {
    case 0:
      exit_opened = m_area.openReverseRoomExit( m_exit, true );
      break;
    default:
      return;
      break;
    }
  }

  if( !exit_opened )
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
      trUtf8( "Non e' stato possibile creare l'uscita inversa." ) );
  else
    QMessageBox::information( this, TS::MESSAGE_BOX_INFORMATION,
      trUtf8( "L'uscita inversa e' stata creata." ) );
}
