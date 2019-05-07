#include "wndsettings.h"
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QDir>
#include <QStyleFactory>
#include "validator.h"
#include "config.h"
#include "settings.h"
#include "utils.h"
#include "guiutils.h"
#include "kreatorsettings.h"

using namespace ts;


WndSettings::WndSettings( QWidget* parent )
  : QDialog( parent ), Ui::GuiSettings()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::WndSettings( QWidget* ) called." );
#endif
  init();
  loadData();
}

WndSettings::~WndSettings()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::~WndSettings() called." );
#endif
}

void WndSettings::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::init() called." );
#endif

  setupUi( this );

  TS::SetIOAreaIcon( mp_tbRoomsDir );
  TS::SetIOAreaIcon( mp_tbItemsDir );
  TS::SetIOAreaIcon( mp_tbMobsDir );
  TS::SetIOAreaIcon( mp_tbShopsDir );
  TS::SetIOAreaIcon( mp_tbZonesDir );
  TS::SetIOAreaIcon( mp_tbNotesDir );
  TS::SetIOAreaIcon( mp_tbLogsDir );

  mp_comboTheme->addItems(QStyleFactory::keys());

  setWindowTitle( trUtf8( "Preferenze - TS Kreator" ) );

  connect( mp_pbSave, SIGNAL( clicked() ), this, SLOT( saveData() ) );
  connect( mp_pbOk, SIGNAL( clicked() ), this, SLOT( saveAndClose() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );

  connect( mp_tbRoomsDir, SIGNAL( clicked() ), this, SLOT( editRoomsDir() ) );
  connect( mp_tbItemsDir, SIGNAL( clicked() ), this, SLOT( editItemsDir() ) );
  connect( mp_tbMobsDir, SIGNAL( clicked() ), this, SLOT( editMobsDir() ) );
  connect( mp_tbShopsDir, SIGNAL( clicked() ), this, SLOT( editShopsDir() ) );
  connect( mp_tbZonesDir, SIGNAL( clicked() ), this, SLOT( editZonesDir() ) );
  connect( mp_tbNotesDir, SIGNAL( clicked() ), this, SLOT( editNotesDir() ) );
  connect( mp_tbLogsDir, SIGNAL( clicked() ), this, SLOT( editLogsDir() ) );

  connect( mp_cbApplyAlways, SIGNAL( stateChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_cbCheckAreasAtStartup, SIGNAL( stateChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_cbAssignZoneToRoom, SIGNAL( stateChanged( int ) ), this, SLOT( somethingChanged() ) );

  connect( mp_leRoomsDir, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leItemsDir, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leMobsDir, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leShopsDir, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leZonesDir, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leNotesDir, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leLogsDir, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );

  connect( mp_comboTheme, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
}

void WndSettings::refreshPanel()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::refreshPanel() called." );
#endif
/*
  mp_leRoomsDir->setText( QDir::convertSeparators( Settings::roomsDirectory() ) );
  mp_leItemsDir->setText( QDir::convertSeparators( Settings::itemsDirectory() ) );
  mp_leMobsDir->setText( QDir::convertSeparators( Settings::mobsDirectory() ) );
  mp_leShopsDir->setText( QDir::convertSeparators( Settings::shopsDirectory() ) );
  mp_leZonesDir->setText( QDir::convertSeparators( Settings::zonesDirectory() ) );
  mp_leNotesDir->setText( QDir::convertSeparators( Settings::notesDirectory() ) );
  mp_leLogsDir->setText( QDir::convertSeparators( Settings::logsDirectory() ) );
*/
  mp_leRoomsDir->setText( QDir::toNativeSeparators( Settings::roomsDirectory() ) );
  mp_leItemsDir->setText( QDir::toNativeSeparators( Settings::itemsDirectory() ) );
  mp_leMobsDir->setText( QDir::toNativeSeparators( Settings::mobsDirectory() ) );
  mp_leShopsDir->setText( QDir::toNativeSeparators( Settings::shopsDirectory() ) );
  mp_leZonesDir->setText( QDir::toNativeSeparators( Settings::zonesDirectory() ) );
  mp_leNotesDir->setText( QDir::toNativeSeparators( Settings::notesDirectory() ) );
  mp_leLogsDir->setText( QDir::toNativeSeparators( Settings::logsDirectory() ) );

  mp_cbApplyAlways->setChecked( KreatorSettings::instance().applyWithoutAsking() );
  mp_cbCheckAreasAtStartup->setChecked( KreatorSettings::instance().checkAreasAtStartup() );
  mp_cbAssignZoneToRoom->setChecked( KreatorSettings::instance().assignZoneToRooms() );

  mp_comboTheme->setCurrentIndex( KreatorSettings::instance().kreatorTheme() );
}

void WndSettings::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::somethingChanged() called." );
#endif
  mp_pbSave->setEnabled( true );
}

void WndSettings::loadData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::loadData() called." );
#endif
  refreshPanel();
  mp_pbSave->setEnabled( false );
}

void WndSettings::saveData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::saveData() called." );
#endif

  Settings::setRoomsDirectory( Utils::createPath( mp_leRoomsDir->text().simplified() ) );
  Settings::setItemsDirectory( Utils::createPath( mp_leItemsDir->text().simplified() ) );
  Settings::setMobsDirectory( Utils::createPath( mp_leMobsDir->text().simplified() ) );
  Settings::setZonesDirectory( Utils::createPath( mp_leZonesDir->text().simplified() ) );
  Settings::setShopsDirectory( Utils::createPath( mp_leShopsDir->text().simplified() ) );
  Settings::setNotesDirectory( Utils::createPath( mp_leNotesDir->text().simplified() ) );
  Settings::setLogsDirectory( Utils::createPath( mp_leLogsDir->text().simplified() ) );
  Settings::save();

  KreatorSettings::instance().setApplyWithoutAsking( mp_cbApplyAlways->isChecked() );
  KreatorSettings::instance().setCheckAreasAtStartup( mp_cbCheckAreasAtStartup->isChecked() );
  KreatorSettings::instance().setAssignZoneToRooms( mp_cbAssignZoneToRoom->isChecked() );
  KreatorSettings::instance().setKreatorTheme( mp_comboTheme->currentIndex() );
  KreatorSettings::instance().save();

  refreshPanel();
  QApplication::setStyle(QStyleFactory::create(QStyleFactory::keys().at(KreatorSettings::instance().kreatorTheme())));
  mp_pbSave->setEnabled( false );
}

void WndSettings::saveAndClose()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::saveAndClose() called." );
#endif
  if( mp_pbSave->isEnabled() )
    saveData();
  close();
}

void WndSettings::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::closeEvent( QCloseEvent* ) called." );
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

void WndSettings::editRoomsDir()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::editRoomsDir() called." );
#endif
  selectDir( mp_leRoomsDir, trUtf8( "Seleziona la directory per le stanze" ) );
}

void WndSettings::editZonesDir()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::editZonesDir() called." );
#endif
  selectDir( mp_leZonesDir, trUtf8( "Seleziona la directory per le zone" ) );
}

void WndSettings::editMobsDir()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::editMobsDir() called." );
#endif
  selectDir( mp_leMobsDir, trUtf8( "Seleziona la directory per i mobs" ) );
}

void WndSettings::editItemsDir()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::editItemsDir() called." );
#endif
  selectDir( mp_leItemsDir, trUtf8( "Seleziona la directory per gli oggetti" ) );
}

void WndSettings::editShopsDir()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::editShopsDir() called." );
#endif
  selectDir( mp_leShopsDir, trUtf8( "Seleziona la directory per i negozi" ) );
}

void WndSettings::editNotesDir()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::editNotesDir() called." );
#endif
  selectDir( mp_leNotesDir, trUtf8( "Seleziona la directory per gli appunti" ) );
}

void WndSettings::editLogsDir()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::editLogsDir() called." );
#endif
  selectDir( mp_leLogsDir, trUtf8( "Seleziona la directory per i log" ) );
}

void WndSettings::selectDir( QLineEdit* ple, const QString& msg )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndSettings::selectDir( QLineEdit*, const QString&, const QString& ) called." );
#endif
  QString sDir = QFileDialog::getExistingDirectory( this, ple->text(), msg );
  if( !sDir.isNull() || !sDir.isEmpty() )
    ple->setText( Utils::createPath( sDir ) );
}
