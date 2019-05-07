#include "wnditem.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
#include <QListWidget>
#include "validator.h"
#include "config.h"
#include "constantname.h"
#include "handler.h"
#include "wnditemvalues.h"
#include "wnditemaffects.h"
#include "selectobject.h"
#include "wndextradescription.h"
#include "guiutils.h"
#include "kreatorsettings.h"

using namespace ts;


WndItem::WndItem( Area& ref_area, const Item& ref_item, QWidget* parent )
  : QDialog( parent ), Ui::GuiItem(),
    m_area( ref_area ),
    m_item( ref_item )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::WndItem( const Area&, const Item&, QWidget* ) called." );
#endif
  init();
  loadData();
  mp_leName->setFocus();
}

WndItem::~WndItem()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::~WndItem() called." );
#endif
}

void WndItem::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::init() called." );
#endif
  setupUi( this );
  KreatorSettings::instance().loadGuiStatus( "ItemWindow", this );

  mp_tbAddExtraDescription->setIcon( TS::GetAddIcon() );
  mp_tbEditExtraDescription->setIcon( TS::GetEditIcon() );
  mp_tbRemoveExtraDescription->setIcon( TS::GetRemoveIcon() );
  mp_tbFlags->setIcon( TS::GetEditIcon() );
  mp_tbNewFlags->setIcon( TS::GetEditIcon() );
  mp_tbWearPositions->setIcon( TS::GetEditIcon() );
  mp_tbEditValues->setIcon( TS::GetEditIcon() );
  mp_tbEditAffects->setIcon( TS::GetEditIcon() );
  TS::SetLeftArrow( mp_pbPrevItem );
  TS::SetRightArrow( mp_pbNextItem );
  mp_pbInit->setIcon( TS::GetInitIcon() );

  mp_leVNumber->setValidator( Validator::vnumber() );

  mp_comboType->clear();
  for( int i = 0; i < ITEM_TYPE_END; i++ )
    mp_comboType->insertItem( i, ConstantName::itemType( i ).toLower() );

  mp_teValues->setReadOnly( true );
  mp_teAffects->setReadOnly( true );
  mp_leWeight->setValidator( Validator::unsignedInteger() );
  mp_leGoldValue->setValidator( Validator::unsignedInteger() );
  mp_leRent->setValidator( Validator::integer() );

  connect( mp_comboType, SIGNAL( activated( int ) ), this, SLOT( typeSelected( int ) ) );
  connect( mp_tbFlags, SIGNAL( clicked() ), this, SLOT( editFlags() ) );
  connect( mp_tbNewFlags, SIGNAL( clicked() ), this, SLOT( editNewFlags() ) );
  connect( mp_tbWearPositions, SIGNAL( clicked() ), this, SLOT( editWearPositions() ) );
  connect( mp_tbEditValues, SIGNAL( clicked() ), this, SLOT( editValues() ) );
  connect( mp_tbEditAffects, SIGNAL( clicked() ), this, SLOT( editAffects() ) );
  connect( mp_pbPrevItem, SIGNAL( clicked() ), this, SLOT( showPrevItem() ) );
  connect( mp_pbNextItem, SIGNAL( clicked() ), this, SLOT( showNextItem() ) );
  connect( mp_lwExtraDescriptions, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ),
    this, SLOT( editExtraDescription( QListWidgetItem* ) ) );
  connect( mp_tbEditExtraDescription, SIGNAL( clicked() ), this, SLOT( editExtraDescription() ) );
  connect( mp_tbAddExtraDescription, SIGNAL( clicked() ), this, SLOT( addExtraDescription() ) );
  connect( mp_tbRemoveExtraDescription, SIGNAL( clicked() ), this, SLOT( removeExtraDescription() ) );

  connect( mp_leVNumber, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leName, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leShort, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_teLong, SIGNAL( textChanged() ), this, SLOT( somethingChanged() ) );
  connect( mp_teAction, SIGNAL( textChanged() ), this, SLOT( somethingChanged() ) );
  connect( mp_leWeight, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leGoldValue, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leRent, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );

  connect( mp_pbSave, SIGNAL( clicked() ), this, SLOT( saveData() ) );
  connect( mp_pbRestore, SIGNAL( clicked() ), this, SLOT( restoreData() ) );
  connect( mp_pbOk, SIGNAL( clicked() ), this, SLOT( saveAndClose() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
}

void WndItem::refreshPanel()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::refreshPanel() called." );
#endif

  mp_leVNumber->setText( QString::number( m_item.vnumber() ) );
  mp_leName->setText( m_item.name() );
  mp_leShort->setText( m_item.shortDescription() );
  mp_teLong->setPlainText( m_item.longDescription() );
  mp_teAction->setPlainText( m_item.actionDescription() );
  mp_comboType->setCurrentIndex( m_item.itemType() );
  typeSelected( m_item.itemType() );
  mp_leWeight->setText( QString::number( m_item.weight() ) );
  mp_leGoldValue->setText( QString::number( m_item.goldValue() ) );
  mp_leRent->setText( QString::number( m_item.rentCost() ) );
}

void WndItem::refreshFlags()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::refreshFlags() called." );
#endif
  mp_leFlags->setText( Utils::bitvector2string( m_item.flags(), Eleuconf::getAllFlagsCaption(Eleuconf::objFlags)).toLower() );
}

void WndItem::refreshNewFlags()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::refreshNewFlags() called." );
#endif
  mp_leNewFlags->setText( Utils::bitvector2string( m_item.extraFlags(), Eleuconf::getAllFlagsCaption(Eleuconf::objFlags, 1) ).toLower() );
}

void WndItem::refreshWearPositions()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::refreshWearPositions() called." );
#endif
  mp_leWearPositions->setText( Utils::bitvector2string( m_item.wearPositions(), Eleuconf::getAllFlagsCaption(Eleuconf::objWear) ).toLower() );
}

void WndItem::refreshTitle()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::refreshTitle() called." );
#endif
  QString sTitle = "";
  sTitle.sprintf( "Oggetto #%ld - %s", m_item.vnumber(), qPrintable( m_item.name() ) );
  if( mp_pbSave->isEnabled() )
    sTitle.append( QString( " [modificato]" ) );
  setWindowTitle( sTitle );
}

void WndItem::refreshValues()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::refreshValues() called." );
#endif

  QString sValueName0 = Handler::itemValueName( mp_comboType->currentIndex(), ITEM_VALUE_0 );
  QString sValueName1 = Handler::itemValueName( mp_comboType->currentIndex(), ITEM_VALUE_1 );
  QString sValueName2 = Handler::itemValueName( mp_comboType->currentIndex(), ITEM_VALUE_2 );
  QString sValueName3 = Handler::itemValueName( mp_comboType->currentIndex(), ITEM_VALUE_3 );
  QString sValue0 = Handler::itemValueToString( mp_comboType->currentIndex(), ITEM_VALUE_0, m_item );
  QString sValue1 = Handler::itemValueToString( mp_comboType->currentIndex(), ITEM_VALUE_1, m_item );
  QString sValue2 = Handler::itemValueToString( mp_comboType->currentIndex(), ITEM_VALUE_2, m_item );
  QString sValue3 = Handler::itemValueToString( mp_comboType->currentIndex(), ITEM_VALUE_3, m_item );

  QString sTxt = "";
  if( sValueName0 != "Value0" )
  {
    sTxt += sValueName0;
    sTxt += QString( ": " );
    sTxt += sValue0;
    sTxt += QString( "\n" );
  }

  if( sValueName1 != "Value1" )
  {
    sTxt += sValueName1;
    sTxt += QString( ": " );
    sTxt += sValue1;
    sTxt += QString( "\n" );
  }

  if( sValueName2 != "Value2" )
  {
    sTxt += sValueName2;
    sTxt += QString( ": " );
    sTxt += sValue2;
    sTxt += QString( "\n" );
  }

  if( sValueName3 != "Value3" )
  {
    sTxt += sValueName3;
    sTxt += QString( ": " );
    sTxt += sValue3;
    sTxt += QString( "\n" );
  }

  mp_teValues->setPlainText( sTxt );
}

void WndItem::refreshAffects()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::refreshAffects() called." );
#endif
  QString sAffects = "";
  QString sTmp = "";

  for( int i = 0; i < ITEM_AFFECT_POSITION_END; i++ )
  {
    ItemAffect ia = m_item.affect( i );
    if( ia.isValid() )
    {
      sTmp.sprintf( "%s: %s\n", qPrintable( ia.typeToString() ),
             qPrintable( ia.valueToString().toLower() ) );
      sAffects += sTmp;
      sTmp = "";
    }
  }

  mp_teAffects->setPlainText( sAffects );
}

void WndItem::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::somethingChanged() called." );
#endif
  mp_pbSave->setEnabled( true );
}

void WndItem::restoreData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::restoreData() called." );
#endif
  if( m_area.hasItem( m_item.vnumber() ) )
  {
    m_item = m_area.item( m_item.vnumber() );
    loadData();
  }
  else
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
                   TS::MESSAGE_WARNING_NO_VNUMBER.arg( m_item.vnumber() ) );
}

void WndItem::loadData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::loadData() called." );
#endif
  refreshPanel();
  refreshFlags();
  refreshNewFlags();
  refreshWearPositions();
  refreshValues();
  refreshAffects();
  refreshExtraDescriptions();
  mp_pbSave->setEnabled( false );
  refreshTitle();
}

void WndItem::saveData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::saveData() called." );
#endif
  VNumber old_vnum = m_item.vnumber();
  VNumber new_vnum = mp_leVNumber->text().toLong();

  if( old_vnum != new_vnum )
  {
    if( m_area.hasItem( new_vnum )
      && QMessageBox::question( this, TS::MESSAGE_BOX_QUESTION,
                        TS::MESSAGE_ASK_OVERWRITE_VNUMBER.arg( new_vnum ),
                        trUtf8( "Si" ), trUtf8( "No" ), QString(), 1, 1 )
      )
      return;

    if( !QMessageBox::question( this, TS::MESSAGE_BOX_QUESTION,
                        TS::MESSAGE_ASK_UPDATE_REFERENCES.arg( old_vnum ).arg( new_vnum ),
                        trUtf8( "Si" ), trUtf8( "No" ), QString(), 0, 1 )
      )
      m_area.changeItemVNumber( old_vnum, new_vnum, true );

  }

  m_item.setVNumber( new_vnum );
  m_item.setName( mp_leName->text() );
  m_item.setShortDescription( mp_leShort->text() );
  m_item.setLongDescription( mp_teLong->toPlainText() );
  m_item.setActionDescription( mp_teAction->toPlainText() );
  m_item.setItemType( mp_comboType->currentIndex() );
  m_item.setWeight( mp_leWeight->text().toInt() );
  m_item.setGoldValue( mp_leGoldValue->text().toInt() );
  m_item.setRentCost( mp_leRent->text().toInt() );

  m_area.addItem( m_item );
  m_area.setItemsChanged();
  m_area.sortItems();

  mp_pbSave->setEnabled( false );
  refreshTitle();
  emit( dataSaved() );
}

void WndItem::saveAndClose()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::saveAndClose() called." );
#endif
  if( mp_pbSave->isEnabled() )
    saveData();
  close();
}

void WndItem::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::closeEvent( QCloseEvent* ) called." );
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

  if( e->isAccepted() )
    KreatorSettings::instance().saveGuiStatus( "ItemWindow", this );
}

void WndItem::typeSelected( int iType )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::typeSelected( int ) called." );
#endif

  if( iType != m_item.itemType() )
  {
    refreshValues();
    somethingChanged();
  }
}

void WndItem::showNextItem()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::showNextItem() called." );
#endif
  QCloseEvent ce;
  closeEvent( &ce );
  if( !ce.isAccepted() )
    return;

  VNumber next_vnum = m_area.nextObjectInList( ObjectData::Object_Item, m_item.vnumber() );
  if( next_vnum != VNumberInvalid )
  {
    m_item = m_area.item( next_vnum );
    loadData();
    mp_pbPrevItem->setEnabled( true );
  }
  else
    mp_pbNextItem->setEnabled( false );
}

void WndItem::showPrevItem()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::showPrevItem() called." );
#endif

  QCloseEvent ce;
  closeEvent( &ce );
  if( !ce.isAccepted() )
    return;

  VNumber prev_vnum = m_area.prevObjectInList( ObjectData::Object_Item, m_item.vnumber() );
  if( prev_vnum != VNumberInvalid )
  {
    m_item = m_area.item( prev_vnum );
    loadData();
    mp_pbNextItem->setEnabled( true );
  }
  else
    mp_pbPrevItem->setEnabled( false );
}

void WndItem::editFlags()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::editFlags( int ) called." );
#endif
  BitVector vFlags = m_item.flags();
  BitVector vRetFlags = SelectObject::itemFlags( vFlags, this );
  if( vRetFlags == vFlags )
    return;
  m_item.setFlags( vRetFlags );
  refreshFlags();
  somethingChanged();
}

void WndItem::editNewFlags()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::editNewFlags( int ) called." );
#endif
  BitVector vFlags = m_item.extraFlags();
  BitVector vRetFlags = SelectObject::itemNewFlags( vFlags, this );
  if( vRetFlags == vFlags )
      return;
  m_item.setExtraFlags( vRetFlags );
  refreshNewFlags();
  somethingChanged();
}

void WndItem::editWearPositions()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::editWearPositions() called." );
#endif
  BitVector vFlags = m_item.wearPositions();
  BitVector vRetFlags = SelectObject::itemWearPositions( vFlags, this );
  if( vRetFlags == vFlags )
    return;
  m_item.setWearPositions( vRetFlags );
  refreshWearPositions();
  somethingChanged();
}

void WndItem::editValues()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::editValues() called." );
#endif
  if( m_item.itemType() != mp_comboType->currentIndex() )
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
      trUtf8( "E' stata rilevata una modifica nel tipo dell'oggetto che non e' stata applicata.\n"
              "Prima di modifica i campi Value e' necessario salvare le modifiche." ), trUtf8( "Ok" ) );
    return;
  }

  WndItemValues *pDialog = new WndItemValues( m_area, m_item, this );
  connect( pDialog, SIGNAL( dataSaved() ), this, SLOT( refreshValues() ) );
  connect( pDialog, SIGNAL( dataSaved() ), this, SLOT( somethingChanged() ) );
  pDialog->show();
  pDialog->setFixedSize( pDialog->size() );
  pDialog->exec();
  delete pDialog;
}

void WndItem::editAffects()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::editAffects() called." );
#endif

  WndItemAffects *pDialog = new WndItemAffects( m_area, m_item, this );
  connect( pDialog, SIGNAL( dataSaved() ), this, SLOT( refreshAffects() ) );
  connect( pDialog, SIGNAL( dataSaved() ), this, SLOT( somethingChanged() ) );
  pDialog->show();
  pDialog->setFixedSize( pDialog->size() );
  pDialog->exec();
  delete pDialog;
}

void WndItem::editExtraDescription()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::editExtraDescription() called." );
#endif
  QList<QListWidgetItem*> wiList = mp_lwExtraDescriptions->selectedItems();
  if( wiList.empty() )
    return;

  editExtraDescription( wiList.first() );
}

void WndItem::editExtraDescription( QListWidgetItem* item )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::editExtraDescription( QListWidgetItem* ) called." );
#endif

  if( !item )
    return;

  if( m_item.hasExtraDescription( item->text() ) )
  {
    ExtraDescription ex = m_item.extraDescription( item->text() );
    WndExtraDescription *pDialog = new WndExtraDescription( ex, this );
    pDialog->show();
    pDialog->setFixedSize( pDialog->size() );
    pDialog->exec();

    if( pDialog->okPressed() && ( pDialog->keysChanged() || pDialog->descriptionChanged() ) )
    {
      if( item->text() == "nuovaextra" )
        m_item.removeExtraDescription( "nuovaextra" );
      m_item.addExtraDescription( pDialog->extraDescriptionSelected() );
      refreshExtraDescriptions();
      somethingChanged();
    }

    delete pDialog;
  }
}

void WndItem::addExtraDescription()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::addExtraDescription() called." );
#endif
  QString sKey = trUtf8( "nuovaextra" );
  QString sDesc = trUtf8( "Non vedi nulla di particolare qui..." );

  if( m_item.hasExtraDescription( sKey ) )
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
              trUtf8( "Esiste gia' una descrizione con key 'nuovaextra'.\n"
                      "Modificate quella esistente prima di creare una nuova extra." ) );
    return;
  }

  m_item.addExtraDescription( ExtraDescription( sKey, sDesc ) );
  refreshExtraDescriptions();
  somethingChanged();
}

void WndItem::removeExtraDescription()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::removeExtraDescription() called." );
#endif
  QList<QListWidgetItem*> wiList = mp_lwExtraDescriptions->selectedItems();
  if( wiList.empty() )
    return;

  QString sKey = (wiList.first())->text();

  if( m_item.hasExtraDescription( sKey ) )
  {
    m_item.removeExtraDescription( sKey );
    refreshExtraDescriptions();
    somethingChanged();
  }
}

void WndItem::refreshExtraDescriptions()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItem::refreshExtraDescriptions() called." );
#endif
  mp_lwExtraDescriptions->clear();
  Item::extra_description_const_iterator it = m_item.extraDescriptions().begin();
  while( it != m_item.extraDescriptions().end() )
  {
  	QListWidgetItem* item = new QListWidgetItem( mp_lwExtraDescriptions );
    item->setText( (*it).keys() );

#if defined( KREATOR_DEBUG )
    qDebug( "WndZone::refreshExtraDescriptions(): add extra '%s'.", qPrintable( (*it).dumpObject() ) );
#endif
    ++it;
  }
}
