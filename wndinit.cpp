#include "wndinit.h"
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <QListWidget>
#include <QHeaderView>
#include "wndmobinit.h"
#include "guiutils.h"
#include "config.h"
#include "zoneinitlist.h"


using namespace ts;


WndInit::WndInit( bool mob, VNumber vnum, Area& ref_area, QWidget* parent )
  : QDialog( parent ), Ui::GuiInit(),
    m_area( ref_area ),
    m_vnum( vnum ),
    m_mob( mob )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::WndInit( Area&, const Zone&, QWidget* ) called." );
#endif
  init();
  loadData();
}

WndInit::~WndInit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::~WndInit() called." );
#endif
}

void WndInit::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::init() called." );
#endif
  setupUi( this );


  mp_tbAdd->setIcon( TS::GetAddIcon() );
  mp_tbEdit->setIcon( TS::GetEditIcon() );
  mp_tbRemove->setIcon( TS::GetRemoveIcon() );
  mp_tbCopy->setIcon( TS::GetCopyIcon() );

  mp_twInits->setColumnCount(1);
  mp_twInits->header()->hide();

  connect( mp_twInits, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( editInit() ) );
  connect( mp_tbEdit, SIGNAL( clicked() ), this, SLOT( editInit() ) );
  connect( mp_tbAdd, SIGNAL( clicked() ), this, SLOT( addInit() ) );
  connect( mp_tbRemove, SIGNAL( clicked() ), this, SLOT( removeInit() ) );
  connect( mp_tbCopy, SIGNAL( clicked() ), this, SLOT( copyInit() ) );

  connect( mp_pbSave, SIGNAL( clicked() ), this, SLOT( saveData() ) );
  connect( mp_pbRestore, SIGNAL( clicked() ), this, SLOT( restoreData() ) );
  connect( mp_pbOk, SIGNAL( clicked() ), this, SLOT( saveAndClose() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );

}

void WndInit::refreshView()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::refreshView() called." );
#endif

  mp_twInits->clear();

  QList<Zone> zon = m_area.zones();
  Zone zone;

  for(int i = 0;i < zon.size(); i++) {
      zone = zon.at(i);
      for(int k = 0; k < zone.initList().mobSize(); k++) {
          ZoneCommand zc = zone.initList().mobAt(k).master().parent();
          if(zc.argument(ZoneCommand::Argument1) == m_vnum) {
              QTreeWidgetItem *itm = new QTreeWidgetItem( mp_twInits );
              QString tmp;
			      for(int j = 0; j < m_area.rooms().size(); ++j) {
					  if(zc.argument(ZoneCommand::Argument3) == m_area.rooms().at(j).vnumber())
						  tmp.sprintf("Room: %ld (%s)", m_area.rooms().at(j).vnumber(), m_area.rooms().at(j).name().toUtf8().data());

			  //tmp.sprintf("Room: %ld", zc.argument(ZoneCommand::Argument3));
              itm->setText(0, tmp);
			  QList<QVariant> list;
			  list << (int)zc.argument(ZoneCommand::Argument0)
				   << (int)zc.argument(ZoneCommand::Argument1)
				   << (int)zc.argument(ZoneCommand::Argument2)
				   << (int)zc.argument(ZoneCommand::Argument3)
				   << (int)zc.argument(ZoneCommand::Argument4);
			  itm->setData(0, Qt::UserRole, list);
#if defined( KREATOR_DEBUG )
    qDebug( "WndZone::refreshView(): %ld add '%ld'.", m_vnum, zc.argument(ZoneCommand::Argument3));
#endif
          }
      }
  }
 }
}

void WndInit::refreshTitle()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::refreshTitle() called." );
#endif
  QString sTitle = "";

  if(m_mob)
	  sTitle.sprintf( "Mob #%ld", m_vnum );
  else
	  sTitle.sprintf( "Oggetto #%ld", m_vnum );

  if( mp_pbSave->isEnabled() )
    sTitle.append( QString( " [modificato]" ) );
  setWindowTitle( sTitle );
}

void WndInit::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::somethingChanged() called." );
#endif
  mp_pbSave->setEnabled( true );
}

void WndInit::restoreData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::restoreData() called." );
#endif
  if( m_area.hasZone( m_zone.vnumber() ) )
  {
    m_zone = m_area.zone( m_zone.vnumber() );
    loadData();
  }
  else
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
                   TS::MESSAGE_WARNING_NO_VNUMBER.arg( m_zone.vnumber() ) );
}

void WndInit::loadData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::loadData() called." );
#endif
  refreshView();
  mp_pbSave->setEnabled( false );
  refreshTitle();
}

void WndInit::saveData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::saveData() called." );
#endif


  mp_pbSave->setEnabled( false );
  refreshTitle();
  emit( dataSaved() );
}

void WndInit::saveAndClose()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::saveAndClose() called." );
#endif
  if( mp_pbSave->isEnabled() )
    saveData();
  close();
}

void WndInit::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::closeEvent( QCloseEvent* ) called." );
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

void WndInit::editInit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::editInit() called." );
#endif
  QList<QTreeWidgetItem*> wiList = mp_twInits->selectedItems();
  if( wiList.empty() )
    return;

  editInit( wiList.first(), 0 );
}

void WndInit::editInit( QTreeWidgetItem* item, int )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::editInit( QTreeWidgetItem* ) called." );
#endif

  //bool ok;

  if( !item )
    return;
  QList<QVariant> tmp = item->data(0, Qt::UserRole).toList();
  WndMobInit *mobInit = new WndMobInit(m_vnum, tmp.at(3).toInt(), tmp.at(2).toInt(), m_area, this);
  mobInit->show();
  mobInit->exec();
}

void WndInit::addInit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::addInit() called." );
#endif

  refreshView();
  somethingChanged();
}

void WndInit::removeInit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::removeInit() called." );
#endif
  QList<QTreeWidgetItem*> wiList = mp_twInits->selectedItems();
  if( wiList.empty() )
    return;


}

void WndInit::copyInit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndInit::copyInit() called." );
#endif
  QList<QTreeWidgetItem*> wiList = mp_twInits->selectedItems();
  if( wiList.empty() )
    return;


}
