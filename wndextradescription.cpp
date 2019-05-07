#include "wndextradescription.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include "config.h"
#include "utils.h"
#include "guiutils.h"



WndExtraDescription::WndExtraDescription( const ExtraDescription& extra_desc, QWidget* parent )
  : QDialog( parent ), GuiExtraDescription(),
    m_extraDescription( extra_desc ),
    m_oldExtraDescription( extra_desc ),
    m_okPressed( false )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExtraDescription::WndExtraDescription( const ExtraDescription&, QWidget* ) called." );
#endif
  init();
  loadData();
}

WndExtraDescription::~WndExtraDescription()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExtraDescription::~WndExtraDescription() called." );
#endif
}

void WndExtraDescription::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExtraDescription::init() called." );
#endif

  setupUi( this );

  highLighter = new Syntax(mp_teDescription->document());

  mp_teDescription->setFont( TS::GetFixedFont() );

  connect( mp_pbSave, SIGNAL( clicked() ), this, SLOT( saveData() ) );
  connect( mp_pbRestore, SIGNAL( clicked() ), this, SLOT( restoreData() ) );
  connect( mp_pbOk, SIGNAL( clicked() ), this, SLOT( saveAndClose() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );

  connect( mp_leKeys, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_teDescription, SIGNAL( textChanged() ), this, SLOT( somethingChanged() ) );
}

void WndExtraDescription::refreshPanel()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExtraDescription::refreshPanel() called." );
#endif
  mp_leKeys->setText( m_extraDescription.keys() );
  mp_teDescription->setPlainText( m_extraDescription.description() );
}

void WndExtraDescription::refreshTitle()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExtraDescription::refreshTitle() called." );
#endif
  QString sTitle = "";
  sTitle.sprintf( "Descrizione Extra '%s'", qPrintable( m_extraDescription.keys() ) );
  if( mp_pbSave->isEnabled() )
    sTitle.append( QString( " [modificata]" ) );
  setWindowTitle( sTitle );
}

void WndExtraDescription::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExtraDescription::somethingChanged() called." );
#endif
  mp_pbSave->setEnabled( true );
  refreshTitle();
}

void WndExtraDescription::restoreData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExtraDescription::restoreData() called." );
#endif
  m_extraDescription = m_oldExtraDescription;
  loadData();
}

void WndExtraDescription::loadData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExtraDescription::loadData() called." );
#endif
  refreshPanel();
  mp_pbSave->setEnabled( false );
  refreshTitle();
}

void WndExtraDescription::saveData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExtraDescription::saveData() called." );
#endif
  m_extraDescription.setKeys( mp_leKeys->text() );
  m_extraDescription.setDescription( mp_teDescription->toPlainText() );
  mp_pbSave->setEnabled( false );
  refreshTitle();
}

void WndExtraDescription::saveAndClose()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExtraDescription::saveAndClose() called." );
#endif
  if( mp_pbSave->isEnabled() )
    saveData();
  m_okPressed = true;
  close();
}

void WndExtraDescription::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndExtraDescription::closeEvent( QCloseEvent* ) called." );
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
