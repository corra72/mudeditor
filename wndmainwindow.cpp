#include "wndmainwindow.h"
#include <QStyleFactory>
#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QDir>
#include <QHeaderView>
#include <QTextStream>
#include <QToolBar>
#include <QTimer>
#include <QStatusBar>
#include "wndarea.h"
#include "config.h"
#include "wndsettings.h"
#include "guiversion.h"
#include "version.h"
#include "guiutils.h"
#include "settings.h"
#include "wndcreatearea.h"
#include "constantname.h"
#include "kreatorsettings.h"
#include "eleuconf.h"

using namespace ts;


WndMainWindow::WndMainWindow( QWidget* parent )
  : QMainWindow( parent ), Ui::GuiMainWindow()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::WndMainWindow( QWidget* parent, const char* name ) called." );
#endif
  init();
}

WndMainWindow::~WndMainWindow()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::~WndMainWindow() called." );
#endif
}

void WndMainWindow::closeMainWindow()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::closeMainWindow() called." );
#endif
  close();
}

void WndMainWindow::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::closeEvent( QCloseEvent* ) called." );
#endif
  e->accept();

  if( e->isAccepted() )
  {
    KreatorSettings::instance().saveGuiStatus( "MainWindow", this );
    emit( windowClosed() );
  }
}

#define COLUMN_VNUMBER    0
#define COLUMN_NAME       1
#define COLUMN_PATH       2
#define COLUMN_MINMAX     3
#define COLUMN_REPOP_TIME 4
#define COLUMN_REPOP_MODE 5
#define COLUMN_FLAGS      6
#define COLUNM_MAX        7


void WndMainWindow::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::init() called." );
#endif
  setupUi( this );
  (void) statusBar();
  setWindowTitle( "TS Kreator" );
  TS::SetKreatorLogo( this );
  KreatorSettings::instance().loadGuiStatus( "MainWindow", this );

  if(KreatorSettings::instance().kreatorTheme() != -1) {
    QApplication::setStyle(QStyleFactory::create(QStyleFactory::keys().at(KreatorSettings::instance().kreatorTheme())));
  } else {
#ifdef Q_WS_WIN32
      if(QStyleFactory::keys().contains("windowsxp", Qt::CaseInsensitive))
          QApplication::setStyle(QStyleFactory::create("windowsxp"));
      else
          QApplication::setStyle(QStyleFactory::create("windows"));
#elif defined( Q_WS_MAC )
      //da testare se funziona su os x
      QApplication::setStyle(QStyleFactory::create("macintosh"));
#elif defined( Q_WS_X11 )
      QApplication::setStyle(QStyleFactory::create("plastique"));
#endif
  }

  QString ColumnName[ COLUNM_MAX ];
  ColumnName[ COLUMN_VNUMBER    ] = trUtf8( "# Zona" );
  ColumnName[ COLUMN_NAME       ] = trUtf8( "Nome" );
  ColumnName[ COLUMN_PATH       ] = trUtf8( "Path" );
  ColumnName[ COLUMN_MINMAX     ] = trUtf8( "VNumbers" );
  ColumnName[ COLUMN_REPOP_TIME ] = trUtf8( "Intervallo" );
  ColumnName[ COLUMN_REPOP_MODE ] = trUtf8( "Repop" );
  ColumnName[ COLUMN_FLAGS      ] = trUtf8( "Flags" );

  mp_twObjectList->setColumnCount( COLUNM_MAX );
  QStringList labels;
  for( int i = 0; i < COLUNM_MAX; i++ )
    labels << ColumnName[ i ];
  mp_twObjectList->setHeaderLabels( labels );
  mp_twObjectList->hideColumn( COLUMN_PATH );

  QHeaderView* pHeader = mp_twObjectList->header();
  pHeader->resizeSection( COLUMN_VNUMBER, 80 );
  pHeader->resizeSection( COLUMN_NAME, 270 );
  pHeader->setStretchLastSection( true );
  pHeader->setSortIndicator( COLUMN_VNUMBER, Qt::AscendingOrder );
  pHeader->setSortIndicatorShown( true );
  // pHeader->setClickable( true );
  pHeader->setSectionsClickable( true );

  mp_popupFile = menuBar()->addMenu( trUtf8( "File" ) );
  mp_barFile = addToolBar( trUtf8( "File" ) );
  mp_barFile->setIconSize( QSize( 22, 22 ) );

  mp_actRefreshAreas = new QAction( QIcon(":/images/searchareas.png"), trUtf8( "Genera lista aree" ), this );
  mp_actRefreshAreas->setStatusTip( trUtf8( "Genera la lista delle aree" ) );
  connect( mp_actRefreshAreas, SIGNAL( triggered() ), this, SLOT( loadAreas() ) );
  mp_popupFile->addAction( mp_actRefreshAreas );
  mp_barFile->addAction( mp_actRefreshAreas );

  mp_popupFile->addSeparator();
  mp_barFile->addSeparator();

  mp_actCreateArea = new QAction( QIcon(":/images/newarea.png"), trUtf8( "Nuova area..." ), this );
  mp_actCreateArea->setStatusTip( trUtf8( "Crea una nuova area" ) );
  connect( mp_actCreateArea, SIGNAL( triggered() ), this, SLOT( createArea() ) );
  mp_popupFile->addAction( mp_actCreateArea );
  mp_barFile->addAction( mp_actCreateArea );

  mp_actLoadArea = new QAction( QIcon(":/images/loadarea.png"), trUtf8( "Carica..." ), this );
  mp_actLoadArea->setStatusTip( trUtf8( "Carica un'area" ) );
  connect( mp_actLoadArea, SIGNAL( triggered() ), this, SLOT( loadArea() ) );
  mp_popupFile->addAction( mp_actLoadArea );
  mp_barFile->addAction( mp_actLoadArea );

  mp_mru = mp_popupFile->addMenu( QIcon(":/images/loadarea.png"), trUtf8( "File aperti di recente" ) );
  QList<QString> tmpList = KreatorSettings::instance().mru();
  for(int i = 0; i < tmpList.size(); ++i) {
	  QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(tmpList.at(i)));
	  mp_recentFileActs[i] = new QAction(this);
      mp_recentFileActs[i]->setText(text);
      mp_recentFileActs[i]->setData(tmpList.at(i));
	  mp_recentFileActs[i]->setStatusTip(tmpList.at(i));
	  connect (mp_recentFileActs[i], SIGNAL( triggered() ), this, SLOT( openRecentFile() ) );
	  mp_mru->addAction( mp_recentFileActs[i] );
  }


  mp_popupFile->addSeparator();
  mp_barFile->addSeparator();

  mp_actSettings = new QAction( QIcon(":/images/settings.png"), trUtf8( "Preferenze..." ), this );
  mp_actSettings->setStatusTip( trUtf8( "Apri la finestra delle preferenze" ) );
  connect( mp_actSettings, SIGNAL( triggered() ), this, SLOT( showSettings() ) );
  mp_popupFile->addAction( mp_actSettings );
  mp_barFile->addAction( mp_actSettings );

  mp_popupFile->addSeparator();
  mp_barFile->addSeparator();

  mp_actQuit = new QAction( QIcon(":/images/quit.png"), trUtf8( "Esci" ), this );
  mp_actQuit->setStatusTip( trUtf8( "Esci da TS Kreator" ) );
  connect( mp_actQuit, SIGNAL( triggered() ), this, SLOT( closeMainWindow() ) );
  mp_popupFile->addAction( mp_actQuit );
  mp_barFile->addAction( mp_actQuit );

  mp_popupHelp = menuBar()->addMenu( trUtf8( "?" ) );

  //mp_popupHelp->addAction( trUtf8( "Suggerimenti..." ) );
  //mp_popupHelp->addAction( trUtf8( "Aiuto..." ) );
  //mp_popupHelp->addSeparator();
  mp_popupHelp->addAction( QIcon( ":/images/info.png" ), trUtf8( "Informazioni..." ), this, SLOT( showAbout() ) );

  connect( mp_twObjectList, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ),
    this, SLOT( loadArea( QTreeWidgetItem*, int ) ) );
  connect( mp_twObjectList, SIGNAL( itemSelectionChanged() ), this, SLOT( areaSelectionChanged() ) );
  connect( mp_leFilter, SIGNAL( textEdited( const QString& ) ), this, SLOT( filterList() ) );
}

void WndMainWindow::showMessage( const QString& msg, int timeout )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::showMessage( const QString&, int ) called." );
#endif
  statusBar()->showMessage( msg, timeout );
  emit( messageShowed( msg ) );
  QApplication::processEvents();
}

void WndMainWindow::areaSelectionChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::areaSelectionChanged() called." );
#endif
  QList<QTreeWidgetItem*> wiList = mp_twObjectList->selectedItems();
  if( wiList.empty() )
    return;
  QTreeWidgetItem* item = wiList.first();
  showMessage( trUtf8( "File: " ) + item->text( COLUMN_PATH ) );
}

void WndMainWindow::loadAreas()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::loadAreas() called." );
#endif
  QApplication::processEvents();
  QTimer::singleShot( 500, this, SLOT( refreshAreaView() ) );
}

void WndMainWindow::createItem( const QString& zone_path )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::createItem( const QString ) called." );
#endif

  VNumber vnum;
  FILE *pFile;

  if( ( pFile = fopen( qPrintable( zone_path ), "r" ) ) )
  {
    char szBuffer[ 255 ];

    while( fgets( szBuffer, sizeof( szBuffer ), pFile ) )
    {
      if( *szBuffer == '#' )
      {
        vnum = -1;
        sscanf( szBuffer, "#%ld", &vnum );

        if( vnum >= VNumberInvalid && vnum < VNumberTop )
        {
          Zone new_zone( vnum );
          new_zone.loadHeaderOnly( pFile );

          QTreeWidgetItem *item = new QTreeWidgetItem( mp_twObjectList );
          item->setIcon( COLUMN_VNUMBER, QIcon( ":/images/world.png" ) );
          item->setText( COLUMN_VNUMBER, Utils::vnumber2string( new_zone.vnumber(), 3 ) );
          item->setText( COLUMN_NAME, new_zone.name() );
          item->setText( COLUMN_PATH, zone_path );
          item->setText( COLUMN_MINMAX, Utils::vnumber2string( new_zone.minVNumber() ) + QString( " - " ) + Utils::vnumber2string( new_zone.maxVNumber() ) );
          item->setText( COLUMN_REPOP_TIME, Utils::vnumber2string( new_zone.repopInterval(), 3 ) + QString( " min." ) );
          item->setText( COLUMN_REPOP_MODE, ConstantName::zoneRepopMode( new_zone.repopMode() ).toLower() );
          item->setText( COLUMN_FLAGS, Utils::bitvector2string( new_zone.newFlags(), Eleuconf::getAllFlagsCaption(Eleuconf::zoneFlags) ).toLower() );

          QString msg = "";
          msg.sprintf( "Controllo la Zona #%ld in %s", new_zone.vnumber(), qPrintable( zone_path ) );
          showMessage( msg );
        }
      }
    }
    fclose( pFile );
  }
}

void WndMainWindow::refreshAreaView()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::checkAreas() called." );
#endif

  QString sAreaDir = Settings::zonesDirectory();

  if( sAreaDir.isEmpty() || sAreaDir == "." )
  {
    QMessageBox::information( this, TS::MESSAGE_BOX_INFORMATION,
      trUtf8( "Per favore seleziona le cartelle dove vuoi salvare le aree." ) );
    showSettings();
    return;
  }

  QDir areaDir( sAreaDir );
  if( !areaDir.exists() )
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
      trUtf8( "La cartella delle zone non esiste." ) );
    showSettings();
    return;
  }

  QApplication::setOverrideCursor( Qt::WaitCursor );
  mp_twObjectList->clear();
  showMessage( trUtf8( "Creo la lista delle aree esistenti..." ) );

  QFileInfoList fiList = areaDir.entryInfoList( QDir::Files );

  QList<QFileInfo>::const_iterator it = fiList.begin();
  while( it != fiList.end() )
  {
    if( (*it).suffix() == Settings::zoneSuffix() )
      createItem( (*it).absoluteFilePath() );
    ++it;
  }

  mp_twObjectList->sortItems( COLUMN_VNUMBER, Qt::AscendingOrder );

  QApplication::restoreOverrideCursor();
  showMessage( trUtf8( "Pronto." ) );
}

void WndMainWindow::showSettings()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::showPageSettings() called." );
#endif
  WndSettings *pSettingsDialog = new WndSettings( this );
  pSettingsDialog->exec();
  delete pSettingsDialog;
}

void WndMainWindow::createArea()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::createArea() called." );
#endif
  QString sFileName = QFileDialog::getSaveFileName( this, trUtf8( "Crea un nuovo file" ),
                                     Settings::zonesDirectory(), trUtf8( "Zone File (*.zon)" ) );

  if( sFileName.isNull() || sFileName.isEmpty() )
    return;

  QString zone_suffix = QString( "." ) + Settings::zoneSuffix();
  if( !sFileName.endsWith( zone_suffix ) )
    sFileName.append( zone_suffix );

  WndCreateArea* pwCreateArea = new WndCreateArea( sFileName, this );
  connect( pwCreateArea, SIGNAL( areaCreated( const QString& ) ),
    this, SLOT( loadArea( const QString& ) ) );
  pwCreateArea->show();
  pwCreateArea->setFixedSize( pwCreateArea->size() );
  pwCreateArea->exec();
}

void WndMainWindow::loadArea()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::loadArea() called." );
#endif
  QString sFilter = "";
  sFilter.sprintf( "Area Files (*.%s *.%s *.%s *.%s)", Settings::roomSuffix().toUtf8().data(),
    Settings::mobSuffix().toUtf8().data(), Settings::itemSuffix().toUtf8().data(), Settings::zoneSuffix().toUtf8().data() );

  QList<QString> tmpList = KreatorSettings::instance().mru();
  QString file_selected;
  if(!tmpList.isEmpty()) {
	  QDir tmpDir(tmpList.at(0));
	  file_selected = QFileDialog::getOpenFileName( this, trUtf8( "Seleziona il file da aprire" ),
                            tmpDir.absolutePath(), sFilter );
  } else {
	  file_selected = QFileDialog::getOpenFileName( this, trUtf8( "Seleziona il file da aprire" ),
                            Settings::zonesDirectory(), sFilter );
  }
  if(!file_selected.isEmpty()){
	  tmpList.prepend( file_selected );
	  while(tmpList.size() > 5) {
		  tmpList.removeLast();
	  }
	  KreatorSettings::instance().setMru(tmpList);
	  mp_mru->clear();
	  for(int i = 0; i < tmpList.size(); ++i) {
		  QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(tmpList.at(i)));
		  mp_recentFileActs[i] = new QAction(this);
		  mp_recentFileActs[i]->setText(text);
		  mp_recentFileActs[i]->setData(tmpList.at(i));
		  mp_recentFileActs[i]->setStatusTip(tmpList.at(i));
		  connect (mp_recentFileActs[i], SIGNAL( triggered() ), this, SLOT( openRecentFile() ) );
		  mp_mru->addAction( mp_recentFileActs[i] );
	  }
  }

  loadArea( file_selected );
}

void WndMainWindow::loadArea( const QString& file_selected )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::loadArea( const QString& ) called." );
#endif
  if( file_selected.isNull() || file_selected.isEmpty() )
    return;
qDebug( "loadArea( const QString& ) 1" );
  WndArea* pwArea = new WndArea( this, "WndArea" );
  qDebug( "loadArea( const QString& ) 2" );
  pwArea->show();
  qDebug( "loadArea( const QString& ) 3" );
  pwArea->loadArea( file_selected );
  qDebug( "loadArea( const QString& ) 4" );
}

void WndMainWindow::showAbout()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::showAbout() called." );
#endif

  QString sAbout = "";
  sAbout.sprintf( "TS Kreator %s di Carlo(Otacon/Brandon)\n"
          "basato su LeU Kreator.\n", qPrintable( VERSION ));

  QMessageBox::about( this, TS::MESSAGE_BOX_ABOUT, sAbout );
}

void WndMainWindow::loadArea( QTreeWidgetItem* item, int )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::loadArea( QTreeWidgetItem*, int ) called." );
#endif
  if( !item )
    return;

  WndArea* pwArea = new WndArea( this, "WndArea" );
  pwArea->show();
  pwArea->loadArea( item->text( COLUMN_PATH ) );
}

void WndMainWindow::openRecentFile()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::openRecentFile() called." );
#endif
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        loadArea(action->data().toString());
}

QString WndMainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void WndMainWindow::filterList()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMainWindow::filterList() called." );
#endif
  for( int i = 0; i < mp_twObjectList->topLevelItemCount(); i++)
  {
      if(mp_twObjectList->topLevelItem(i)->text(COLUMN_NAME).contains(mp_leFilter->text(), Qt::CaseInsensitive))
          mp_twObjectList->topLevelItem(i)->setHidden( false );
      else
          mp_twObjectList->topLevelItem(i)->setHidden( true );
  }
}
