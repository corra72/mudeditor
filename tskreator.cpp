#include "tskreator.h"
#include "validator.h"
#include "settings.h"
#include "handler.h"
#include "eleuconf.h"
#include "kreatorsettings.h"

using namespace ts;


TsKreator::TsKreator( int& argc, char** argv )
  : QApplication( argc, argv )
{
#if defined( KREATOR_DEBUG )
  qDebug( "TsKreator::TsKreator( int& argc, char** argv ) called." );
#endif
  init();
}

TsKreator::~TsKreator()
{
#if defined( KREATOR_DEBUG )
  qDebug( "TsKreator::~TsKreator() called." );
#endif
}

void TsKreator::closeTsKreator()
{
#if defined( KREATOR_DEBUG )
  qDebug( "TsKreator::closeTsKreator() called." );
#endif
  cleanUp();
  quit();
}

void TsKreator::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "TsKreator::init() called." );
#endif
  Eleuconf::readAll();
  ts::Settings::load();
  Handler::init();
  KreatorSettings::instance();
}

void TsKreator::cleanUp()
{
#if defined( KREATOR_DEBUG )
  qDebug( "TsKreator::cleanUp() called." );
#endif
/*
  QWidgetList *pWList = QApplication::allWidgets();
  QWidgetListIt it( *pWList );
  QWidget * w;
  while( (w=it.current()) != 0 )
  {
    ++it;
    w->close();
  }
  delete pWList;
*/
  Validator::cleanUp();
  Handler::cleanUp();
  KreatorSettings::deleteInstance();
}
