#include "kreatorsettings.h"
#include <QWidget>
#include <QPoint>
#include "settings.h"
#include "bit.h"

using namespace ts;


KreatorSettings* KreatorSettings::mp_instance = NULL;


KreatorSettings::KreatorSettings()
  : QSettings( QSettings::IniFormat, QSettings::UserScope, "ts", "kreator" )
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::KreatorSettings() called." );
#endif
}

KreatorSettings::~KreatorSettings()
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::~KreatorSettings() called." );
#endif
}

KreatorSettings& KreatorSettings::instance()
{
  if( !mp_instance )
    mp_instance = new KreatorSettings;

  return *mp_instance;
}

void KreatorSettings::deleteInstance()
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::deleteInstance() called." );
#endif
  if( mp_instance )
  {
    delete mp_instance;
    mp_instance = NULL;
  }
}

void KreatorSettings::saveGuiStatus( const QString& widget_name, QWidget* w )
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::saveGuiStatus( const QString&, QWidget* ) called." );
#endif
  instance().beginGroup( widget_name.toLower() );
  instance().setValue( "size", w->size() );
  instance().setValue( "position", w->pos() );
  instance().endGroup();
}

void KreatorSettings::loadGuiStatus( const QString& widget_name, QWidget* w )
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::loadGuiStatus( const QString&, QWidget* ) called." );
#endif
  instance().beginGroup( widget_name.toLower() );
  w->resize( instance().value( "size", w->size() ).toSize() );
  w->move( instance().value( "position", w->pos() ).toPoint() );
  instance().endGroup();
}

void KreatorSettings::saveButtonGroupStatus( const QString& group_name, MyQt::ButtonGroup* pbg )
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::saveButtonGroupStatus( const QString&, MyQt::ButtonGroup* ) called." );
#endif
  BitVector bv = 0;
  QAbstractButton* pButton = NULL;
  for( int i = 0; i <= pbg->maxId(); i++ )
  {
    pButton = pbg->button( i );
    if( !pButton )
      break;

    if( pButton->isChecked() )
      Bit::set( &bv, i );
  }
  instance().beginGroup( group_name.toLower() );
  instance().setValue( "status", QString::number( bv ) );
  instance().endGroup();
}

void KreatorSettings::loadButtonGroupStatus( const QString& group_name, MyQt::ButtonGroup* pbg )
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::loadButtonGroupStatus( const QString&, MyQt::ButtonGroup* ) called." );
#endif
  instance().beginGroup( group_name.toLower() );
  long bv = instance().value( "status", -1 ).toInt();
  instance().endGroup();
  if( bv < 0 )
    return;
  QAbstractButton* pButton = NULL;
  for( int i = 0; i <= pbg->maxId(); i++ )
  {
    pButton = pbg->button( i );
    if( !pButton )
      break;

    pButton->setChecked( Bit::has( bv, i ) );
  }
}

bool KreatorSettings::applyWithoutAsking()
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::applyWithoutAsking() called." );
#endif
  return instance().value( "/settings/applynoask", false ).toBool();
}

void KreatorSettings::setApplyWithoutAsking( bool enable )
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::setApplyWithoutAsking( bool ) called." );
#endif
  instance().setValue( "/settings/applynoask", enable );
}

bool KreatorSettings::checkAreasAtStartup()
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::checkAreasAtStartup() called." );
#endif
  return instance().value( "/settings/checkareasatstartup", false ).toBool();
}

void KreatorSettings::setCheckAreasAtStartup( bool enable )
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::setCheckAreasAtStartup( bool ) called." );
#endif
  instance().setValue( "/settings/checkareasatstartup", enable );
}

bool KreatorSettings::assignZoneToRooms()
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::assignZoneToRooms() called." );
#endif
  return instance().value( "/settings/assignzonetorooms", true ).toBool();
}

void KreatorSettings::setAssignZoneToRooms( bool enable )
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::setAssignZoneToRooms bool ) called." );
#endif
  instance().setValue( "/settings/assignzonetorooms", enable );
}

int KreatorSettings::kreatorTheme()
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::kreatorTheme() called." );
#endif
  return instance().value( "/settings/theme", -1 ).toInt();
}

void KreatorSettings::setKreatorTheme( int theme )
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::setKreatorTheme( int ) called." );
#endif
  instance().setValue( "/settings/theme", theme );
}


QList<QString> KreatorSettings::mru()
{
#if defined( KREATOR_DEBUG )
	qDebug( "KreatorSettings::mru() called." );
#endif
  QList<QString> list;
  int size = instance().beginReadArray( "/settings/mru" );
  for (int i = 0; i < size; ++i) {
    instance().setArrayIndex(i);
    QString tmp;
	tmp = instance().value("mru").toString();
	list.append(tmp);
  }
  instance().endArray();
  return list;
}


void KreatorSettings::setMru( QList<QString> m_mru )
{
#if defined( KREATOR_DEBUG )
  qDebug( "KreatorSettings::setMru( QList<QString> ) called." );
#endif
  instance().beginWriteArray( "/settings/mru" );
  for (int i = 0; i < m_mru.size(); ++i) {
    instance().setArrayIndex(i);
	instance().setValue("mru", m_mru.at(i));
  }
  instance().endArray();
}
