#include "wndarea.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QApplication>
#include <QProgressDialog>
#include <QTreeWidget>
#include <QGroupBox>
#include <QStackedWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QHeaderView>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QTimer>
#include "wndzone.h"
#include "wndroom.h"
#include "wnditem.h"
#include "wndmob.h"
#include "wndtester.h"
#include "settings.h"
#include "config.h"
#include "handler.h"
#include "guiutils.h"
#include "wndreport.h"
#include "report.h"
#include "wndchangevnumbers.h"
#include "kreatorsettings.h"
#include "wndshop.h"
#include "wndmainwindow.h"
#include "constantname.h"
#include "wndfind.h"
#include "eleuconf.h"
#include "wndmultiplemodify.h"


using namespace ts;


WndArea::WndArea( QWidget* parent, const char* name )
  : QMainWindow( parent ), Ui::GuiArea()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::WndArea( QWidget* parent, const char* name ) called." );
#endif
  setObjectName( name );
  init();
}

WndArea::~WndArea()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::~WndArea() called." );
#endif
}

void WndArea::setAreaFile( const QString& new_file )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::setAreaFile( const QString& ) called." );
#endif
  assignFileNames( new_file );
  refreshTitle();
  refreshFileView();
}

void WndArea::refreshTitle()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::refreshTitle() called." );
#endif
  QString sTitle = "";
  sTitle.sprintf( "Area '%s'", qPrintable( m_area.name() ) );
  if( isChanged() )
    sTitle.append( trUtf8( " [modificata]" ) );
  sTitle.append( trUtf8( " - TS Kreator" ) );
  setWindowTitle( sTitle );
}

void WndArea::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::init() called." );
#endif
  m_currentObjectTypeList = ObjectData::Object_Zone;
  setupUi( this );
  KreatorSettings::instance().loadGuiStatus( "AreaWindow", this );

  refreshTitle();

  mp_pbNew->setIcon( TS::GetAddIcon() );
  mp_pbEdit->setIcon( TS::GetEditIcon() );
  mp_pbRemove->setIcon( TS::GetRemoveIcon() );
  mp_pbClone->setIcon( TS::GetCopyIcon() );
  TS::SetIOAreaIcon( mp_tbChangeFile );

  mp_bgViews = new MyQt::ButtonGroup( this );
  mp_bgViews->setExclusive( true );
  mp_bgViews->addButton( mp_rbViewRooms, 0 );
  mp_bgViews->addButton( mp_rbViewMobs, 1 );
  mp_bgViews->addButton( mp_rbViewItems, 2 );
  mp_bgViews->addButton( mp_rbViewShops, 3 );
  mp_bgViews->addButton( mp_rbViewZones, 4 );

  initMenuArea();
  initMenuActions();
  initMenuReport();
  initMenuUtils();
  menuBar()->addMenu( ((WndMainWindow*)parent())->helpMenu() );

  mp_twObjectList->setAlternatingRowColors( true );
  QHeaderView* pHeader = mp_twObjectList->header();
  pHeader->setStretchLastSection( true );
//  pHeader->setClickable( false );
  pHeader->setSectionsClickable( false );

  connect( mp_bgViews, SIGNAL( buttonClicked( int ) ), this, SLOT( changeView( int ) ) );
  connect( mp_twObjectList, SIGNAL( itemDoubleClicked( QTreeWidgetItem *, int ) ), this, SLOT( showEditDialog( QTreeWidgetItem*, int ) ) );
  connect( mp_pbEdit, SIGNAL( clicked() ), this, SLOT( showEditDialog() ) );
  connect( mp_pbRemove, SIGNAL( clicked() ), this, SLOT( removeObject() ) );
  connect( mp_pbClone, SIGNAL( clicked() ), this, SLOT( cloneObject() ) );
  connect( mp_pbNew, SIGNAL( clicked() ), this, SLOT( createObject() ) );
  connect( mp_tbChangeFile, SIGNAL( clicked() ), this, SLOT( changeFileName() ) );
}

void WndArea::initMenuArea()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::initMenuArea() called." );
#endif
  mp_menuArea = menuBar()->addMenu( trUtf8( "Area" ) );
  mp_barArea = addToolBar( trUtf8( "Area" ) );
  mp_barArea->setIconSize( QSize( 22, 22 ) );

  mp_actSaveArea = new QAction( QIcon(":/images/savearea.png"), trUtf8( "Salva" ), this );
  mp_actSaveArea->setStatusTip( trUtf8( "Salva l'area corrente" ) );
  connect( mp_actSaveArea, SIGNAL( triggered() ), this, SLOT( saveArea() ) );
  mp_menuArea->addAction( mp_actSaveArea );
  mp_barArea->addAction( mp_actSaveArea );

  mp_actSaveAreaAs = new QAction( QIcon(":/images/saveareaas.png"), trUtf8( "Salva con nome..." ), this );
  mp_actSaveAreaAs->setStatusTip( trUtf8( "Salva con nome l'area corrente" ) );
  connect( mp_actSaveAreaAs, SIGNAL( triggered() ), this, SLOT( saveAreaAs() ) );
  mp_menuArea->addAction( mp_actSaveAreaAs );
  mp_barArea->addAction( mp_actSaveAreaAs );

  mp_barArea->addSeparator();
  mp_menuArea->addSeparator();

  mp_actCloseArea = new QAction( QIcon(":/images/closearea.png"), trUtf8( "Chiudi" ), this );
  mp_actCloseArea->setStatusTip( trUtf8( "Chiudi l'area corrente" ) );
  connect( mp_actCloseArea, SIGNAL( triggered() ), this, SLOT( close() ) );
  mp_menuArea->addAction( mp_actCloseArea );
  mp_barArea->addAction( mp_actCloseArea );
}

void WndArea::initMenuActions()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::initMenuActions() called." );
#endif
  mp_menuActions = menuBar()->addMenu( trUtf8( "Azioni" ) );
  mp_barActions = addToolBar( trUtf8( "Azioni" ) );
  mp_barActions->setIconSize( QSize( 22, 22 ) );

  mp_actNew = new QAction( TS::GetAddIcon(), trUtf8( "Crea" ), this );
  mp_actNew->setStatusTip( trUtf8( "Crea una nuova entita'" ) );
  connect( mp_actNew, SIGNAL( triggered() ), this, SLOT( createObject() ) );
  mp_menuActions->addAction( mp_actNew );
  mp_barActions->addAction( mp_actNew );

  mp_actEdit = new QAction( TS::GetEditIcon(), trUtf8( "Modifica" ), this );
  mp_actEdit->setStatusTip( trUtf8( "Modifica l'entita' selezionata" ) );
  connect( mp_actEdit, SIGNAL( triggered() ), this, SLOT( showEditDialog() ) );
  mp_menuActions->addAction( mp_actEdit );
  mp_barActions->addAction( mp_actEdit );

  mp_actClone = new QAction( TS::GetCopyIcon(), trUtf8( "Clona" ), this );
  mp_actClone->setStatusTip( trUtf8( "Clona l'entita' selezionata" ) );
  connect( mp_actClone, SIGNAL( triggered() ), this, SLOT( cloneObject() ) );
  mp_menuActions->addAction( mp_actClone );
  mp_barActions->addAction( mp_actClone );

  mp_actRemove = new QAction( TS::GetRemoveIcon(), trUtf8( "Elimina" ), this );
  mp_actRemove->setStatusTip( trUtf8( "Elimina l'entita' selezionata" ) );
  connect( mp_actRemove, SIGNAL( triggered() ), this, SLOT( removeObject() ) );
  mp_menuActions->addAction( mp_actRemove );
  mp_barActions->addAction( mp_actRemove );
}

void WndArea::initMenuReport()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::initMenuReport() called." );
#endif
  mp_menuReport = menuBar()->addMenu( trUtf8( "Report" ) );
  mp_barReport = addToolBar( trUtf8( "Report" ) );
  mp_barReport->setIconSize( QSize( 22, 22 ) );

  mp_actReportCoins = new QAction( QIcon(":/images/coins.png"), trUtf8( "Monete..." ), this );
  mp_actReportCoins->setStatusTip( trUtf8( "Genera il report delle monete" ) );
  connect( mp_actReportCoins, SIGNAL( triggered() ), this, SLOT( showReportCoins() ) );
  mp_menuReport->addAction( mp_actReportCoins );
  mp_barReport->addAction( mp_actReportCoins );

  mp_actReportTreasures = new QAction( QIcon(":/images/treasure.png"), trUtf8( "Tesori..." ), this );
  mp_actReportTreasures->setStatusTip( trUtf8( "Genera il report dei tesori" ) );
  connect( mp_actReportTreasures, SIGNAL( triggered() ), this, SLOT( showReportTreasures() ) );
  mp_menuReport->addAction( mp_actReportTreasures );
  mp_barReport->addAction( mp_actReportTreasures );

  mp_actReportKeys = new QAction( QIcon(":/images/key.png"), trUtf8( "Chiavi..." ), this );
  mp_actReportKeys->setStatusTip( trUtf8( "Genera il report delle chiavi" ) );
  connect( mp_actReportKeys, SIGNAL( triggered() ), this, SLOT( showReportKeys() ) );
  mp_menuReport->addAction( mp_actReportKeys );
  mp_barReport->addAction( mp_actReportKeys );

  mp_actReportDeathRooms = new QAction( QIcon(":/images/skull.png"), trUtf8( "Stanze della morte..." ), this );
  mp_actReportDeathRooms->setStatusTip( trUtf8( "Genera il report delle locazioni con il flag DEATH" ) );
  connect( mp_actReportDeathRooms, SIGNAL( triggered() ), this, SLOT( showReportDeathRooms() ) );
  mp_menuReport->addAction( mp_actReportDeathRooms );
  mp_barReport->addAction( mp_actReportDeathRooms );

  mp_actReportExits = new QAction( QIcon(":/images/exit.png"), trUtf8( "Uscite esterne..." ), this );
  mp_actReportExits->setStatusTip( trUtf8( "Genera il report delle uscite che portano all'esterno dell'area" ) );
  connect( mp_actReportExits, SIGNAL( triggered() ), this, SLOT( showReportExits() ) );
  mp_menuReport->addAction( mp_actReportExits );
  mp_barReport->addAction( mp_actReportExits );

}

void WndArea::initMenuUtils()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::initMenuUtils() called." );
#endif
  mp_menuUtils = menuBar()->addMenu( trUtf8( "Strumenti" ) );
  mp_barUtils = addToolBar( trUtf8( "Strumenti" ) );
  mp_barUtils->setIconSize( QSize( 22, 22 ) );

  mp_actFind = new QAction( QIcon(":/images/searchareas.png"), trUtf8( "Cerca..." ), this );
  mp_actFind->setStatusTip( trUtf8( "Cerca tra tutte le entita'" ) );
  connect( mp_actFind, SIGNAL( triggered() ), this, SLOT( showFind() ) );
  mp_menuUtils->addAction( mp_actFind );
  mp_barUtils->addAction( mp_actFind );

  mp_actChangeVNumbers = new QAction( QIcon(":/images/calc.png"), trUtf8( "Rinumera..." ), this );
  mp_actChangeVNumbers->setStatusTip( trUtf8( "Rinumera tutte le entita'" ) );
  connect( mp_actChangeVNumbers, SIGNAL( triggered() ), this, SLOT( showChangeVNumbers() ) );
  mp_menuUtils->addAction( mp_actChangeVNumbers );
  mp_barUtils->addAction( mp_actChangeVNumbers );

  mp_actTester = new QAction( QIcon(":/images/tester.png"), trUtf8( "Tester..." ), this );
  connect( mp_actTester, SIGNAL( triggered() ), this, SLOT( showTester() ) );
  mp_menuUtils->addAction( mp_actTester );
  mp_barUtils->addAction( mp_actTester );

  mp_actMultipleModify = new QAction( QIcon(":/images/multiple.png"), trUtf8("Modifiche multiple"), this );
  connect( mp_actMultipleModify, SIGNAL( triggered() ), this, SLOT( showMultipleModify() ));
  mp_menuUtils->addAction( mp_actMultipleModify );
  mp_barUtils->addAction( mp_actMultipleModify );
}

void WndArea::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::closeEvent( QCloseEvent* ) called." );
#endif

  if( isChanged() )
  {
    switch( QMessageBox::question( this, TS::MESSAGE_BOX_QUESTION, TS::MESSAGE_ASK_SAVE,
              trUtf8( "Si" ), trUtf8( "No" ), trUtf8( "Annulla" ), 1, 2 ) )
    {
    case 0:
      saveArea();
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

  if( e->isAccepted() )
  {
    KreatorSettings::instance().saveGuiStatus( "AreaWindow", this );
    QList<QWidget*>::iterator it = m_childs.begin();
    while( it != m_childs.end() )
    {
      if( !(*it)->isHidden() )
        (*it)->close();
      ++it;
    }
  }
}

void WndArea::assignFileNames( const QString& first_file_name )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::assignFileNames( const QString& ) called." );
#endif

  QFileInfo fileInfo( first_file_name );

  QString sDir = fileInfo.absoluteDir().path();
  QString sFileName = fileInfo.baseName();
  QString sFileSuffix = fileInfo.suffix();

  m_area.resetFilePaths();

  if( sFileSuffix == Settings::roomSuffix() )
    m_area.setRoomsFilePath( first_file_name );
  else if( sFileSuffix == Settings::mobSuffix() )
    m_area.setMobsFilePath( first_file_name );
  else if( sFileSuffix == Settings::itemSuffix() )
    m_area.setItemsFilePath( first_file_name );
  else if( sFileSuffix == Settings::zoneSuffix() )
    m_area.setZonesFilePath( first_file_name );
  else if( sFileSuffix == Settings::shopSuffix() )
    m_area.setShopsFilePath( first_file_name );
  else
    qDebug( "Invalid extension in file name '%s'.", qPrintable( first_file_name ) );

  if( m_area.roomsFilePath().isEmpty() )
    m_area.setRoomsFilePath( Utils::createPath( sDir, sFileName, Settings::roomSuffix() ) );

  if( m_area.mobsFilePath().isEmpty() )
    m_area.setMobsFilePath( Utils::createPath( sDir, sFileName, Settings::mobSuffix() ) );

  if( m_area.itemsFilePath().isEmpty() )
    m_area.setItemsFilePath( Utils::createPath( sDir, sFileName, Settings::itemSuffix() ) );

  if( m_area.zonesFilePath().isEmpty() )
    m_area.setZonesFilePath( Utils::createPath( sDir, sFileName, Settings::zoneSuffix() ) );

  if( m_area.shopsFilePath().isEmpty() )
    m_area.setShopsFilePath( Utils::createPath( sDir, sFileName, Settings::shopSuffix() ) );

  m_area.setName( sFileName );
}

void WndArea::loadArea( const QString& file_selected )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::loadArea() called." );
#endif

  setAreaFile( file_selected );

  QApplication::setOverrideCursor( Qt::WaitCursor );
  QApplication::processEvents();

  QProgressDialog progressDialog( trUtf8( "Caricamento area in corso..." ),
                    trUtf8( "Interrompi" ), 0, 6, this );

  progressDialog.setValue( 0 );
  progressDialog.show();
  QApplication::processEvents();

  if( !m_area.loadRooms() )
  {
    if( !m_area.loadRooms() )
      qWarning( "Rooms are not loaded from the file '%s'", m_area.roomsFilePath().toUtf8().data() );
  }

  progressDialog.setValue( 1 );
  QApplication::processEvents();

  if( progressDialog.wasCanceled() )
    return;

  if( !m_area.loadMobs() )
  {
    if( !m_area.loadMobs() )
      qWarning( "Mobs are not loaded from the file '%s'", m_area.mobsFilePath().toUtf8().data() );
  }

  progressDialog.setValue( 2 );
  QApplication::processEvents();

  if( progressDialog.wasCanceled() )
    return;

  if( !m_area.loadItems() )
  {
    if( !m_area.loadItems() )
      qWarning( "Items are not loaded from the file '%s'", m_area.itemsFilePath().toUtf8().data() );
  }

  progressDialog.setValue( 3 );
  QApplication::processEvents();

  if( progressDialog.wasCanceled() )
    return;

  if( !m_area.loadShops() )
  {
    if( !m_area.loadShops() )
      qWarning( "Shops are not loaded from the file '%s'", m_area.shopsFilePath().toUtf8().data() );
  }

  progressDialog.setValue( 4 );
  QApplication::processEvents();

  if( progressDialog.wasCanceled() )
    return;

  if( !m_area.loadZones() )
  {
    if( !m_area.loadZones() )
      qWarning( "Zones are not loaded from the file '%s'", m_area.zonesFilePath().toUtf8().data() );
  }

  progressDialog.setValue( 5 );
  QApplication::processEvents();

  m_area.assignStatusToExits();

  progressDialog.setValue( 6 );
  QApplication::processEvents();

  if( !m_area.rooms().empty() )
  {
    mp_rbViewRooms->setChecked( true );
    showRooms();
  }
  else if( !m_area.mobs().empty() )
  {
    mp_rbViewMobs->setChecked( true );
    showMobs();
  }
  else if( !m_area.items().empty() )
  {
    mp_rbViewItems->setChecked( true );
    showItems();
  }
  else if( !m_area.shops().empty() )
  {
    mp_rbViewShops->setChecked( true );
    showShops();
  }
  else
  {
    mp_rbViewZones->setChecked( true );
    showZones();
  }

  mp_actSaveArea->setEnabled( false );
  QApplication::restoreOverrideCursor();
  QApplication::processEvents();
}

void WndArea::saveArea()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::saveArea() called." );
#endif

  QApplication::setOverrideCursor( Qt::WaitCursor );
  QProgressDialog progressDialog( trUtf8( "Salvataggio area in corso..." ),
                    trUtf8( "Interrompi" ), 0, 6, this );

  progressDialog.setValue( 0 );
  progressDialog.show();
  QApplication::processEvents();

  if( KreatorSettings::instance().assignZoneToRooms() )
    m_area.assignZoneToRooms();

  progressDialog.setValue( 1 );
  QApplication::processEvents();

  if( !m_area.saveRooms() )
  {
    qWarning( "Rooms are not saved in the file '%s'", m_area.roomsFilePath().toUtf8().data() );
  }

  progressDialog.setValue( 2 );
  QApplication::processEvents();

  if( progressDialog.wasCanceled() )
    return;

  if( !m_area.saveMobs() )
  {
    qWarning( "Mobs are not saved in the file '%s'", m_area.mobsFilePath().toUtf8().data() );
  }

  progressDialog.setValue( 3 );
  QApplication::processEvents();

  if( progressDialog.wasCanceled() )
    return;

  if( !m_area.saveItems() )
  {
    qWarning( "Items are not saved in the file '%s'", m_area.itemsFilePath().toUtf8().data() );
  }

  progressDialog.setValue( 4 );
  QApplication::processEvents();

  if( progressDialog.wasCanceled() )
    return;

  if( !m_area.saveShops() )
  {
    qWarning( "Shops are not saved in the file '%s'", m_area.shopsFilePath().toUtf8().data() );
  }

  progressDialog.setValue( 5 );
  QApplication::processEvents();

  if( progressDialog.wasCanceled() )
    return;

  if( !m_area.saveZones() )
  {
    qWarning( "Zones are not saved in the file '%s'", m_area.zonesFilePath().toUtf8().data() );
  }

  progressDialog.setValue( 6 );
  mp_actSaveArea->setEnabled( false );
  refreshTitle();
  QApplication::restoreOverrideCursor();
  QApplication::processEvents();
}

void WndArea::saveAreaAs()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::saveAreaAs() called." );
#endif

  QString sFilter = "";
  sFilter.sprintf( "Area Files (*.%s *.%s *.%s *.%s *.%s)", Settings::roomSuffix().toUtf8().data(),
    Settings::mobSuffix().toUtf8().data(), Settings::itemSuffix().toUtf8().data(),
    Settings::shopSuffix().toUtf8().data(), Settings::zoneSuffix().toUtf8().data() );

  QString file_selected = QFileDialog::getSaveFileName( this,
                            trUtf8( "Seleziona il file in cui salvare l'area" ),
                            Settings::zonesDirectory(), sFilter );

  if( file_selected.isNull() || file_selected.isEmpty() )
    return;

  setAreaFile( file_selected );
  saveArea();
}

void WndArea::changeView( int id )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::changeView( int ) called." );
#endif

  switch( id )
  {
  case 0:
    showRooms();
    break;
  case 1:
    showMobs();
    break;
  case 2:
    showItems();
    break;
  case 3:
    showShops();
    break;
  case 4:
    showZones();
    break;
  default:
    m_currentObjectTypeList = ObjectData::Object_Invalid;
    qWarning( "Invalid View ID selected." );
  }
}

void WndArea::refreshFileView()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::refreshFileView() called." );
#endif
  QString sLabel = "";
  QString sPath = "";
  switch( m_currentObjectTypeList )
  {
  case ObjectData::Object_Room:
    sLabel = trUtf8( "File delle Stanze" );
    sPath = m_area.roomsFilePath();
    break;

  case ObjectData::Object_Mob:
    sLabel = trUtf8( "File dei Mobs" );
    sPath = m_area.mobsFilePath();
    break;

  case ObjectData::Object_Item:
    sLabel = trUtf8( "File degli Oggetti" );
    sPath = m_area.itemsFilePath();
    break;

  case ObjectData::Object_Shop:
    sLabel = trUtf8( "File dei Negozi" );
    sPath = m_area.shopsFilePath();
    break;

  case ObjectData::Object_Zone:
    sLabel = trUtf8( "File delle Zone" );
    sPath = m_area.zonesFilePath();
    break;

  default:
    sLabel = trUtf8( "Errore  " );
    sPath = trUtf8( "tipo di ObjectData sconosciuto." );
  }

  mp_labelPage->setText( sLabel );
  mp_labelFile->setText( sPath );
}

void WndArea::refreshView()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::refreshView() called." );
#endif

  switch( m_currentObjectTypeList )
  {
  case ObjectData::Object_Room:
    refreshRoomsView();
    break;

  case ObjectData::Object_Mob:
    refreshMobsView();
    break;

  case ObjectData::Object_Item:
    refreshItemsView();
    break;

  case ObjectData::Object_Shop:
    refreshShopsView();
    break;

  case ObjectData::Object_Zone:
    refreshZonesView();
    break;

  default:
    qWarning( "Invalid View ID selected." );
  };
}

void WndArea::showRooms()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showRooms() called." );
#endif
  m_currentObjectTypeList = ObjectData::Object_Room;
  refreshFileView();
  refreshView();
}

void WndArea::showMobs()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showMobs() called." );
#endif
  m_currentObjectTypeList = ObjectData::Object_Mob;
  refreshFileView();
  refreshView();
}

void WndArea::showItems()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showItems() called." );
#endif
  m_currentObjectTypeList = ObjectData::Object_Item;
  refreshFileView();
  refreshView();
}

void WndArea::showShops()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showShops() called." );
#endif
  m_currentObjectTypeList = ObjectData::Object_Shop;
  refreshFileView();
  refreshView();
}

void WndArea::showZones()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showZones() called." );
#endif
  m_currentObjectTypeList = ObjectData::Object_Zone;
  refreshFileView();
  refreshView();
}

void WndArea::showEditDialog()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showEditDialog() called." );
#endif

  QList<QTreeWidgetItem*> wiList = mp_twObjectList->selectedItems();
  if( wiList.empty() )
    return;

  showEditDialog( wiList.first(), 0 );
}

void WndArea::showEditDialog( QTreeWidgetItem* item, int )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showEditDialog( QTreeWidgetItem*, int ) called." );
#endif
  if( !item )
    return;
  VNumber vnumSelected = Utils::string2vnumber( item->text( 0 ) );
  showEditDialog( m_currentObjectTypeList, vnumSelected );
}

void WndArea::showEditDialog( int object_type, VNumber vnumSelected )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showEditDialog( int, VNumber ) called." );
#endif

  if( object_type == ObjectData::Object_Zone )
  {
    if( m_area.hasZone( vnumSelected ) )
    {
      WndZone *pWndZone = new WndZone( m_area, m_area.zone( vnumSelected ), this );
      connect( pWndZone, SIGNAL( dataSaved() ), this, SLOT( refreshZonesView() ) );
      connect( pWndZone, SIGNAL( dataSaved() ), this, SLOT( somethingChanged() ) );
      pWndZone->exec();
      delete pWndZone;
    }
  }
  else if( object_type == ObjectData::Object_Room )
  {
    if( m_area.hasRoom( vnumSelected ) )
    {
      WndRoom *pWndRoom = new WndRoom( m_area, m_area.room( vnumSelected ), this );
      connect( pWndRoom, SIGNAL( dataSaved() ), this, SLOT( refreshRoomsView() ) );
      connect( pWndRoom, SIGNAL( dataSaved() ), this, SLOT( somethingChanged() ) );
      pWndRoom->show();
      m_childs.append( pWndRoom );
    }
  }
  else if( object_type == ObjectData::Object_Item )
  {
    if( m_area.hasItem( vnumSelected ) )
    {
      WndItem *pWndItem = new WndItem( m_area, m_area.item( vnumSelected ), this );
      connect( pWndItem, SIGNAL( dataSaved() ), this, SLOT( refreshItemsView() ) );
      connect( pWndItem, SIGNAL( dataSaved() ), this, SLOT( somethingChanged() ) );
      pWndItem->show();
      m_childs.append( pWndItem );
    }
  }
  else if( object_type == ObjectData::Object_Mob )
  {
    if( m_area.hasMob( vnumSelected ) )
    {
      WndMob *pWndMob = new WndMob( m_area, m_area.mob( vnumSelected ), this );
      connect( pWndMob, SIGNAL( dataSaved() ), this, SLOT( refreshMobsView() ) );
      connect( pWndMob, SIGNAL( dataSaved() ), this, SLOT( somethingChanged() ) );
      pWndMob->show();
      m_childs.append( pWndMob );
    }
  }
  else if( object_type == ObjectData::Object_Shop )
  {
    if( m_area.hasShop( vnumSelected ) )
    {
      WndShop* pWndShop = new WndShop( m_area, m_area.shop( vnumSelected ), this );
      connect( pWndShop, SIGNAL( dataSaved() ), this, SLOT( refreshShopsView() ) );
      connect( pWndShop, SIGNAL( dataSaved() ), this, SLOT( somethingChanged() ) );
      pWndShop->exec();
      delete pWndShop;
    }
  }
}

void WndArea::refreshRoomsView()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::refreshRoomsView() called." );
#endif
  QApplication::setOverrideCursor( Qt::WaitCursor );
  QApplication::processEvents();
  mp_twObjectList->clear();

  mp_twObjectList->setColumnCount( 4 );
  QStringList labels;
  labels << trUtf8( "# Stanza" ) << trUtf8( "Nome" ) << trUtf8( "Settore" ) << trUtf8( "Flags" );
  mp_twObjectList->setHeaderLabels( labels );
  QHeaderView* pHeader = mp_twObjectList->header();
  pHeader->resizeSection( 0, 80 );
  pHeader->resizeSection( 1, 270 );
  pHeader->resizeSection( 2, 120 );
  pHeader->setSortIndicator( 0, Qt::AscendingOrder );
  pHeader->setSortIndicatorShown( false );

  Area::rooms_const_iterator it = m_area.rooms().begin();
  while( it != m_area.rooms().end() )
  {
    QTreeWidgetItem *item = new QTreeWidgetItem( mp_twObjectList );
    item->setText( 0, Utils::vnumber2string( (*it).vnumber() ) );
    item->setText( 1, (*it).name() );
    item->setText( 2, ConstantName::roomSector( (*it).sectorType() ).toLower() );
    item->setText( 3, Utils::bitvector2string( (*it).flags(), Eleuconf::getAllFlagsCaption(Eleuconf::roomFlags) ).toLower() );
    ++it;
  }
  QApplication::restoreOverrideCursor();
  QApplication::processEvents();
}

void WndArea::refreshMobsView()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::refreshMobsView() called." );
#endif
  QApplication::setOverrideCursor( Qt::WaitCursor );
  QApplication::processEvents();
  mp_twObjectList->clear();

  mp_twObjectList->setColumnCount( 4 );
  QStringList labels;
  labels << trUtf8( "# Mob" ) << trUtf8( "Nome" ) << trUtf8( "Razza" ) << trUtf8( "Acts" );
  mp_twObjectList->setHeaderLabels( labels );
  QHeaderView* pHeader = mp_twObjectList->header();
  pHeader->resizeSection( 0, 80 );
  pHeader->resizeSection( 1, 270 );
  pHeader->resizeSection( 2, 120 );
  pHeader->setSortIndicator( 0, Qt::AscendingOrder );
  pHeader->setSortIndicatorShown( false );

  Area::mobs_const_iterator it = m_area.mobs().begin();
  while( it != m_area.mobs().end() )
  {
    QTreeWidgetItem *item = new QTreeWidgetItem( mp_twObjectList );
    item->setText( 0, Utils::vnumber2string( (*it).vnumber() ) );
    item->setText( 1, (*it).shortDescription() );
    item->setText( 2, ConstantName::characterRace( (*it).race() ).toLower() );
    item->setText( 3, Utils::bitvector2string( (*it).acts(), Eleuconf::getAllFlagsCaption(Eleuconf::mobActionsFlags) ).toLower() );
    ++it;
  }
  QApplication::restoreOverrideCursor();
  QApplication::processEvents();
}

void WndArea::refreshItemsView()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::refreshItemsView() called." );
#endif
  QApplication::setOverrideCursor( Qt::WaitCursor );
  QApplication::processEvents();
  mp_twObjectList->clear();

  mp_twObjectList->setColumnCount( 4 );
  QStringList labels;
  labels << trUtf8( "# Oggetto" ) << trUtf8( "Nome" ) << trUtf8( "Tipo" ) << trUtf8( "Flags" ) << trUtf8( "NewFlags" );
  mp_twObjectList->setHeaderLabels( labels );
  QHeaderView* pHeader = mp_twObjectList->header();
  pHeader->resizeSection( 0, 80 );
  pHeader->resizeSection( 1, 270 );
  pHeader->resizeSection( 2, 120 );
  pHeader->resizeSection( 3, 300 );
  pHeader->setSortIndicator( 0, Qt::AscendingOrder );
  pHeader->setSortIndicatorShown( false );

  Area::items_const_iterator it = m_area.items().begin();
  while( it != m_area.items().end() )
  {
    QTreeWidgetItem *item = new QTreeWidgetItem( mp_twObjectList );
    item->setText( 0, Utils::vnumber2string( (*it).vnumber() ) );
    item->setText( 1, (*it).shortDescription() );
    item->setText( 2, ConstantName::itemType( (*it).itemType() ).toLower() );
    item->setText( 3, Utils::bitvector2string( (*it).flags(), Eleuconf::getAllFlagsCaption(Eleuconf::objFlags) ).toLower() );
    item->setText( 4, Utils::bitvector2string( (*it).extraFlags(), Eleuconf::getAllFlagsCaption(Eleuconf::objFlags, 1) ).toLower() );
    ++it;
  }
  QApplication::restoreOverrideCursor();
  QApplication::processEvents();
}

void WndArea::refreshShopsView()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::refreshShopsView() called." );
#endif
  QApplication::setOverrideCursor( Qt::WaitCursor );
  QApplication::processEvents();
  mp_twObjectList->clear();

  mp_twObjectList->setColumnCount( 3 );
  QStringList labels;
  labels << trUtf8( "# Negozio " ) << trUtf8( "Locazione" ) << trUtf8( "Negoziante" );
  mp_twObjectList->setHeaderLabels( labels );
  QHeaderView* pHeader = mp_twObjectList->header();
  pHeader->resizeSection( 0, 80 );
  pHeader->resizeSection( 1, 270 );
  pHeader->setSortIndicator( 0, Qt::AscendingOrder );
  pHeader->setSortIndicatorShown( false );

  Area::shops_const_iterator it = m_area.shops().begin();
  while( it != m_area.shops().end() )
  {
    QTreeWidgetItem *item = new QTreeWidgetItem( mp_twObjectList );
    item->setText( 0, Utils::vnumber2string( (*it).vnumber() ) );
    item->setText( 1, m_area.shopName( (*it).vnumber() ) );
    item->setText( 2, m_area.mobName( (*it).keeper() ) );
    ++it;
  }
  QApplication::restoreOverrideCursor();
  QApplication::processEvents();
}

void WndArea::refreshZonesView()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::refreshZonesView() called." );
#endif
  QApplication::setOverrideCursor( Qt::WaitCursor );
  QApplication::processEvents();
  mp_twObjectList->clear();

  mp_twObjectList->setColumnCount( 5 );
  QStringList labels;
  labels << trUtf8( "# Zona" ) << trUtf8( "Nome" ) << trUtf8( "Repop" )
    << trUtf8( "Intervallo" ) << trUtf8( "Flags" ) ;
  mp_twObjectList->setHeaderLabels( labels );

  QHeaderView* pHeader = mp_twObjectList->header();
  pHeader->resizeSection( 0, 80 );
  pHeader->resizeSection( 1, 270 );
  pHeader->resizeSection( 2, 80 );
  pHeader->resizeSection( 3, 80 );

  pHeader->setSortIndicator( 0, Qt::AscendingOrder );
  pHeader->setSortIndicatorShown( false );

  Area::zones_const_iterator it = m_area.zones().begin();
  while( it != m_area.zones().end() )
  {
    QTreeWidgetItem *item = new QTreeWidgetItem( mp_twObjectList );
    item->setText( 0, Utils::vnumber2string( (*it).vnumber(), 3 ) );
    item->setText( 1, (*it).name() );
    item->setText( 2, ConstantName::zoneRepopMode( (*it).repopMode() ).toLower() );
    item->setText( 3, QString::number( (*it).repopInterval() ) + QString( " min." ) );
    item->setText( 4, Utils::bitvector2string( (*it).newFlags(), Eleuconf::getAllFlagsCaption(Eleuconf::zoneFlags) ).toLower() );
    ++it;
  }

  QApplication::restoreOverrideCursor();
  QApplication::processEvents();
}

void WndArea::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::somethingChanged() called." );
#endif
  mp_actSaveArea->setEnabled( true );
  refreshTitle();
}

void WndArea::removeObject()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::removeObject() called." );
#endif

  QList<QTreeWidgetItem*> wiList = mp_twObjectList->selectedItems();
  if( wiList.empty() )
    return;

  QTreeWidgetItem* item = wiList.first();

  VNumber vnumSelected = Utils::string2vnumber( item->text( 0 ) );

  if( QMessageBox::question( this, TS::MESSAGE_BOX_QUESTION,
                     TS::MESSAGE_ASK_REMOVE_VNUMBER.arg( vnumSelected ),
                     trUtf8( "Si" ), trUtf8( "No" ), QString(), 1, 1 ) )
    return;

  bool remove_references = ( m_currentObjectTypeList != ObjectData::Object_Zone
                     && m_currentObjectTypeList != ObjectData::Object_Shop
                     && !QMessageBox::question( this, TS::MESSAGE_BOX_QUESTION,
                     TS::MESSAGE_ASK_REMOVE_REFERENCES.arg( vnumSelected ),
                     trUtf8( "Si" ), trUtf8( "No" ), QString(), 0, 1 ) );

  if( m_currentObjectTypeList == ObjectData::Object_Zone )
  {
    m_area.removeZone( vnumSelected );
    refreshZonesView();
  }
  else if( m_currentObjectTypeList == ObjectData::Object_Room )
  {
    m_area.removeRoom( vnumSelected );
    if( remove_references )
      m_area.removeRoomReferences( vnumSelected );
    refreshRoomsView();
  }
  else if( m_currentObjectTypeList == ObjectData::Object_Item )
  {
    m_area.removeItem( vnumSelected );
    if( remove_references )
      m_area.removeItemReferences( vnumSelected );
    refreshItemsView();
  }
  else if( m_currentObjectTypeList == ObjectData::Object_Mob )
  {
    m_area.removeMob( vnumSelected );
    if( remove_references )
      m_area.removeMobReferences( vnumSelected );
    refreshMobsView();
  }
  else if( m_currentObjectTypeList == ObjectData::Object_Shop )
  {
    m_area.removeShop( vnumSelected );
    refreshShopsView();
  }
  else
    qWarning( "WndArea::removeObject() : unknown object_type." );
}

void WndArea::cloneObject()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::cloneObject() called." );
#endif

  QList<QTreeWidgetItem*> wiList = mp_twObjectList->selectedItems();
  if( wiList.empty() )
    return;

  QTreeWidgetItem* item = wiList.first();

  VNumber vnumSelected = Utils::string2vnumber( item->text( 0 ) );

  if( m_currentObjectTypeList == ObjectData::Object_Zone )
  {
    if( m_area.hasZone( vnumSelected ) )
    {
      Zone new_zone( m_area.zone( vnumSelected ) );
      new_zone.setVNumber( ( m_area.topZonesVNumber() + 1 ) );
      m_area.addZone( new_zone );
      refreshZonesView();
    }
  }
  else if( m_currentObjectTypeList == ObjectData::Object_Room )
  {
    if( m_area.hasRoom( vnumSelected ) )
    {
      Room new_room( m_area.room( vnumSelected ) );
      new_room.setVNumber( ( m_area.topRoomsVNumber() + 1 ) );
      m_area.addRoom( new_room );
      refreshRoomsView();
    }
  }
  else if( m_currentObjectTypeList == ObjectData::Object_Item )
  {
    if( m_area.hasItem( vnumSelected ) )
    {
      Item new_item( m_area.item( vnumSelected ) );
      new_item.setVNumber( ( m_area.topItemsVNumber() + 1 ) );
      m_area.addItem( new_item );
      refreshItemsView();
    }
  }
  else if( m_currentObjectTypeList == ObjectData::Object_Mob )
  {
    if( m_area.hasMob( vnumSelected ) )
    {
      Mob new_mob( m_area.mob( vnumSelected ) );
      new_mob.setVNumber( ( m_area.topMobsVNumber() + 1 ) );
      m_area.addMob( new_mob );
      refreshMobsView();
    }
  }
  else if( m_currentObjectTypeList == ObjectData::Object_Shop )
  {
    if( m_area.hasShop( vnumSelected ) )
    {
      Shop new_shop( m_area.shop( vnumSelected ) );
      new_shop.setVNumber( ( m_area.topShopsVNumber() + 1 ) );
      m_area.addShop( new_shop );
      refreshShopsView();
    }
  }
  else
    qWarning( "WndArea::cloneObject() : unknown object_type." );
}

void WndArea::createObject()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::createObject() called." );
#endif

  if( m_currentObjectTypeList == ObjectData::Object_Zone )
  {
    Zone new_zone( m_area.newZonesVNumber() );
    m_area.addZone( new_zone );
    refreshZonesView();
  }
  else if( m_currentObjectTypeList == ObjectData::Object_Room )
  {
    Room new_room( m_area.newRoomsVNumber() );
    if( KreatorSettings::instance().assignZoneToRooms() )
      new_room.setZone( m_area.zoneReferenced( new_room.vnumber() ) );
    m_area.addRoom( new_room );
    refreshRoomsView();
  }
  else if( m_currentObjectTypeList == ObjectData::Object_Item )
  {
    Item new_item( m_area.newItemsVNumber() );
    m_area.addItem( new_item );
    refreshItemsView();
  }
  else if( m_currentObjectTypeList == ObjectData::Object_Mob )
  {
    Mob new_mob( m_area.newMobsVNumber() );
    m_area.addMob( new_mob );
    refreshMobsView();
  }
  else if( m_currentObjectTypeList == ObjectData::Object_Shop )
  {
    Shop new_shop( m_area.newShopsVNumber() );
    m_area.addShop( new_shop );
    refreshShopsView();
  }
  else
    qWarning( "WndArea::createObject() : unknown object_type." );
}

void WndArea::changeFileName()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::createObject() called." );
#endif

  QString sFilter = QString( "*." ) + Settings::fileSuffix( m_currentObjectTypeList );
  QString file_selected = QFileDialog::getSaveFileName( this, trUtf8( "Seleziona il nuovo file" ),
                            Settings::objectDirectory( m_currentObjectTypeList ), sFilter );

  if( file_selected.isNull() || file_selected.isEmpty() )
    return;

  if( mp_labelFile->text() == file_selected )
    return;

  mp_labelFile->setText(file_selected);

  if( m_currentObjectTypeList == ObjectData::Object_Zone )
    m_area.setZonesFilePath( file_selected );
  else if( m_currentObjectTypeList == ObjectData::Object_Room )
    m_area.setRoomsFilePath( file_selected );
  else if( m_currentObjectTypeList == ObjectData::Object_Item )
    m_area.setItemsFilePath( file_selected );
  else if( m_currentObjectTypeList == ObjectData::Object_Mob )
    m_area.setMobsFilePath( file_selected );
  else if( m_currentObjectTypeList == ObjectData::Object_Shop )
    m_area.setShopsFilePath( file_selected );
  else
    qWarning( "WndArea::changeFileName() : unknown object_type." );
}

void WndArea::showReportCoins()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showReportCoins() called." );
#endif
  WndReport* pWnd = new WndReport( this );
  pWnd->setWindowTitle( trUtf8( "Report Monete - TS Kreator" ) );
  pWnd->setReport( Report::coins( m_area ) );
  m_childs.append( pWnd );
  pWnd->show();
}

void WndArea::showReportTreasures()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showReportTreasures() called." );
#endif
  WndReport* pWnd = new WndReport( this );
  pWnd->setWindowTitle( trUtf8( "Report Tesori - TS Kreator" ) );
  pWnd->setReport( Report::treasures( m_area ) );
  m_childs.append( pWnd );
  pWnd->show();
}

void WndArea::showReportExits()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showReportExits() called." );
#endif
  WndReport* pWnd = new WndReport( this );
  pWnd->setWindowTitle( trUtf8( "Report Uscite Esterne - TS Kreator" ) );
  pWnd->setReport( Report::exits( m_area ) );
  m_childs.append( pWnd );
  pWnd->show();
}

void WndArea::showReportKeys()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showReportKeys() called." );
#endif
  WndReport* pWnd = new WndReport( this );
  pWnd->setWindowTitle( trUtf8( "Report Chiavi - TS Kreator" ) );
  pWnd->setReport( Report::keys( m_area ) );
  m_childs.append( pWnd );
  pWnd->show();
}

void WndArea::showReportDeathRooms()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showReportDeathRooms() called." );
#endif
  WndReport* pWnd = new WndReport( this );
  pWnd->setWindowTitle( trUtf8( "Report Locazioni Mortali - TS Kreator" ) );
  pWnd->setReport( Report::deathRooms( m_area ) );
  m_childs.append( pWnd );
  pWnd->show();
}

void WndArea::showChangeVNumbers()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showChangeVNumbers() called." );
#endif
  WndChangeVNumbers* pWnd = new WndChangeVNumbers( m_area, this );
  connect( pWnd, SIGNAL( vnumbersChanged() ), this, SLOT( refreshView() ) );
  pWnd->show();
  pWnd->setFixedSize( pWnd->size() );
  pWnd->exec();
  delete pWnd;
}

void WndArea::showFind()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showFind() called." );
#endif
  WndFind* pWnd = new WndFind( m_area, this );
  connect( pWnd, SIGNAL( openEditWindow( int, VNumber ) ),
    this, SLOT( showEditDialog( int, VNumber ) ) );
  m_childs.append( pWnd );
  pWnd->show();
}

void WndArea::showTester()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndArea::showTester() called." );
#endif
  WndTester* pWnd = new WndTester( &m_area, this );
  m_childs.append( pWnd );
  pWnd->show();
}

void WndArea::showMultipleModify()
{
#if defined( KREATOR_DEBUG )
    qDebug( "WndArea::showMultipleModify() called." );
#endif
    WndMultipleModify* pWnd = new WndMultipleModify( &m_area, this );
    connect( pWnd, SIGNAL( roomSaved() ), this, SLOT( refreshRoomsView() ) );
    connect( pWnd, SIGNAL( roomSaved() ), this, SLOT( somethingChanged() ) );
    connect( pWnd, SIGNAL( objSaved() ), this, SLOT( refreshItemsView() ) );
    connect( pWnd, SIGNAL( objSaved() ), this, SLOT( somethingChanged() ) );
    connect( pWnd, SIGNAL( mobSaved() ), this, SLOT( refreshMobsView() ) );
    connect( pWnd, SIGNAL( mobSaved() ), this, SLOT( somethingChanged() ) );
    m_childs.append( pWnd );
    pWnd->show();
}

