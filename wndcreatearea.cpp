#include "wndcreatearea.h"
#include <QPushButton>
#include <QLineEdit>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "validator.h"
#include "zone.h"
#include "config.h"


WndCreateArea::WndCreateArea( const QString& file_name, QWidget* parent )
  : QDialog( parent ), Ui::GuiCreateArea()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndCreateArea::WndCreateArea( QWidget* ) called." );
#endif
  setupUi( this );
  mp_leZoneVNumber->setValidator( Validator::unsignedInteger() );
  mp_leFileName->setText( file_name );
  connect( mp_pbCreate, SIGNAL( clicked() ), this, SLOT( createArea() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
}

WndCreateArea::~WndCreateArea()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndReport::~WndReport() called." );
#endif
}

void WndCreateArea::createArea()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndReport::saveReportAs() called." );
#endif
  ts::VNumber vnum_zone = mp_leZoneVNumber->text().toLong();
  QString name_zone = mp_leZoneName->text().simplified();
  ts::Zone new_zone( vnum_zone, name_zone );
  QFile file( mp_leFileName->text() );
  if( file.open( QIODevice::WriteOnly ) )
  {
    QTextStream stream( &file );
    new_zone.save( stream );
    file.close();
  }
  else
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
      TS::MESSAGE_WARNING_CANT_SAVE_ON_FILE.arg( mp_leFileName->text() ) );
    return;
  }

  emit( areaCreated( mp_leFileName->text() ) );
  close();
}
