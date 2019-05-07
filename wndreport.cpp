#include "wndreport.h"
#include <QPushButton>
#include <QTextEdit>
#include <QCloseEvent>
#include <QFileDialog>
#include <QDir>
#include "settings.h"
#include "kreatorsettings.h"


WndReport::WndReport( QWidget* parent )
  : QDialog( parent ), Ui::GuiReport()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndReport::WndReport( QWidget* ) called." );
#endif
  setupUi( this );
  KreatorSettings::instance().loadGuiStatus( "ReportWindow", this );
  connect( mp_pbClose, SIGNAL( clicked() ), this, SLOT( close() ) );
  connect( mp_pbSaveAs, SIGNAL( clicked() ), this, SLOT( saveReportAs() ) );
}

WndReport::~WndReport()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndReport::~WndReport() called." );
#endif
}

void WndReport::setReport( const QString& report )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndReport::setReport( const QString& ) called." );
#endif
  mp_teReport->setPlainText( report );
}

void WndReport::saveReportAs()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndReport::saveReportAs() called." );
#endif
  QString file_selected = QFileDialog::getSaveFileName( this, trUtf8( "Seleziona il nuovo file" ), ts::Settings::notesDirectory(), "*.txt" );

  if( file_selected.isNull() || file_selected.isEmpty() )
    return;
}

void WndReport::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndReport::closeEvent( QCloseEvent* ) called." );
#endif
  e->accept();

  if( e->isAccepted() )
    KreatorSettings::instance().saveGuiStatus( "ReportWindow", this );
}
