#include "wnditemvalues.h"
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include "validator.h"
#include "config.h"
#include "constantname.h"
#include "constants.h"
#include "selectobject.h"
#include "guiutils.h"


WndItemValues::WndItemValues( const Area& ref_area, Item& ref_item, QWidget* parent )
  : QDialog( parent ), Ui::GuiItemValues(),
    m_area( ref_area ),
    m_item( ref_item )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::WndItemValues( const Area&, Item&, QWidget* ) called." );
#endif
  init();
  loadData();
}

WndItemValues::~WndItemValues()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::~WndItemValues() called." );
#endif
}

void WndItemValues::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::init() called." );
#endif
  setupUi( this );

  mp_tbValue0->setIcon( TS::GetEditIcon() );
  mp_tbValue1->setIcon( TS::GetEditIcon() );
  mp_tbValue2->setIcon( TS::GetEditIcon() );
  mp_tbValue3->setIcon( TS::GetEditIcon() );

  mp_leValue0->setValidator( Validator::integer() );
  mp_leValue1->setValidator( Validator::integer() );
  mp_leValue2->setValidator( Validator::integer() );
  mp_leValue3->setValidator( Validator::integer() );

  connect( mp_tbValue0, SIGNAL( clicked() ), this, SLOT( editValue0() ) );
  connect( mp_tbValue1, SIGNAL( clicked() ), this, SLOT( editValue1() ) );
  connect( mp_tbValue2, SIGNAL( clicked() ), this, SLOT( editValue2() ) );
  connect( mp_tbValue3, SIGNAL( clicked() ), this, SLOT( editValue3() ) );

  connect( mp_leValue0, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leValue1, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leValue2, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leValue3, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );

  connect( mp_pbOk, SIGNAL( clicked() ), this, SLOT( saveAndClose() ) );
  connect( mp_pbSave, SIGNAL( clicked() ), this, SLOT( saveData() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );

}

void WndItemValues::refreshPanel()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::refreshPanel() called." );
#endif

  enableValueButton( ITEM_VALUE_0, false );
  enableValueButton( ITEM_VALUE_1, false );
  enableValueButton( ITEM_VALUE_2, false );
  enableValueButton( ITEM_VALUE_3, false );

  mp_leValue0->setText( QString::number( m_item.value( ITEM_VALUE_0 ) ) );
  mp_leValue1->setText( QString::number( m_item.value( ITEM_VALUE_1 ) ) );
  mp_leValue2->setText( QString::number( m_item.value( ITEM_VALUE_2 ) ) );
  mp_leValue3->setText( QString::number( m_item.value( ITEM_VALUE_3 ) ) );
  mp_labelValue0->setText( m_item.valueName( ITEM_VALUE_0 ) );
  mp_labelValue1->setText( m_item.valueName( ITEM_VALUE_1 ) );
  mp_labelValue2->setText( m_item.valueName( ITEM_VALUE_2 ) );
  mp_labelValue3->setText( m_item.valueName( ITEM_VALUE_3 ) );

  switch( m_item.itemType() )
  {
  case ITEM_TYPE_LIGHT:
    enableValueButton( ITEM_VALUE_2, true );
    break;

  case ITEM_TYPE_SCROLL:
  case ITEM_TYPE_POTION:
    enableValueButton( ITEM_VALUE_1, true );
    enableValueButton( ITEM_VALUE_2, true );
    enableValueButton( ITEM_VALUE_3, true );
    break;

  case ITEM_TYPE_WAND:
  case ITEM_TYPE_STAFF:
    enableValueButton( ITEM_VALUE_3, true );
    break;

  case ITEM_TYPE_WEAPON:
    enableValueButton( ITEM_VALUE_3, true );
    break;

  case ITEM_TYPE_FIREWEAPON:
  case ITEM_TYPE_MISSILE:
  case ITEM_TYPE_ARMOR:
  case ITEM_TYPE_TREASURE:
  case ITEM_TYPE_WORN:
  case ITEM_TYPE_OTHER:
  case ITEM_TYPE_TRASH:
  case ITEM_TYPE_NOTE:
  case ITEM_TYPE_PEN:
  case ITEM_TYPE_BOAT:
  case ITEM_TYPE_BOARD:
  case ITEM_TYPE_TREE:
  case ITEM_TYPE_ROCK:
  case ITEM_TYPE_KEY:
  case ITEM_TYPE_FOOD:
  case ITEM_TYPE_MONEY:
  case ITEM_TYPE_AUDIO:
    break;

  case ITEM_TYPE_TRAP:
    enableValueButton( ITEM_VALUE_0, true );
    enableValueButton( ITEM_VALUE_1, true );
    break;

  case ITEM_TYPE_CONTAINER:
    enableValueButton( ITEM_VALUE_1, true );
    enableValueButton( ITEM_VALUE_2, true );
    break;

  case ITEM_TYPE_LIQUID_CONTAINER:
    enableValueButton( ITEM_VALUE_2, true );
    enableValueButton( ITEM_VALUE_3, true );
    break;

//  case ITEM_TYPE_SEAL_CONTAINER:
//    enableValueButton( ITEM_VALUE_0, true );
//    break;

  default:
    qWarning( "Invalid ItemType '%d' in %s.", m_item.itemType(), qPrintable( m_item.dumpObject() ) );
    return;
  }
}

void WndItemValues::refreshTitle()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::refreshTitle() called." );
#endif
  QString sTitle = "";
  sTitle.sprintf( "ItemValues Oggetto #%ld - %s", m_item.vnumber(), qPrintable( m_item.name() ) );
  if( mp_pbSave->isEnabled() )
    sTitle.append( QString( " [modificati]" ) );
  setWindowTitle( sTitle );
  mp_gbValues->setTitle( ConstantName::itemType( m_item.itemType() ) );
}

void WndItemValues::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::somethingChanged() called." );
#endif
  mp_pbSave->setEnabled( true );
  refreshTitle();
}

void WndItemValues::loadData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::loadData() called." );
#endif
  refreshPanel();
  mp_pbSave->setEnabled( false );
  refreshTitle();
}

void WndItemValues::saveData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::saveData() called." );
#endif

  m_item.setValue( ITEM_VALUE_0, mp_leValue0->text().toInt() );
  m_item.setValue( ITEM_VALUE_1, mp_leValue1->text().toInt() );
  m_item.setValue( ITEM_VALUE_2, mp_leValue2->text().toInt() );
  m_item.setValue( ITEM_VALUE_3, mp_leValue3->text().toInt() );

  mp_pbSave->setEnabled( false );
  refreshTitle();
  emit( dataSaved() );
}

void WndItemValues::saveAndClose()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::saveAndClose() called." );
#endif
  if( mp_pbSave->isEnabled() )
    saveData();
  close();
}

void WndItemValues::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::closeEvent( QCloseEvent* ) called." );
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

void WndItemValues::enableValueButton( int id, bool enable )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::enableValueButton( int, bool ) called." );
#endif
  QToolButton *ptb;

  switch( id )
  {
  case ITEM_VALUE_0:
    ptb = mp_tbValue0;
    break;
  case ITEM_VALUE_1:
    ptb = mp_tbValue1;
    break;
  case ITEM_VALUE_2:
    ptb = mp_tbValue2;
    break;
  case ITEM_VALUE_3:
    ptb = mp_tbValue3;
    break;
  default:
    qWarning( "Invalid ItemValue '%d' selected.", id );
    return;
  }

  ptb->setEnabled( enable );
}


void WndItemValues::editValue0()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::editValue0() called." );
#endif
  if( m_item.itemType() == ITEM_TYPE_TRAP )
  {
    BitVector vFlags = mp_leValue0->text().toInt();
    BitVector vRetFlags = SelectObject::itemTrapTriggers( vFlags, this );
    if( vRetFlags == vFlags )
      return;
    mp_leValue0->setText( QString::number( vRetFlags ) );
  }/* else if( m_item.itemType() == ITEM_TYPE_SEAL_CONTAINER )
  {
    int type = SelectObject::itemSealContainerTypes( mp_leValue0->text().toInt(), this );
    if( type ==  mp_leValue0->text().toInt() )
      return;
    mp_leValue0->setText( QString::number( type ) );
  } */
}

void WndItemValues::editValue1()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::editValue1() called." );
#endif
  if( m_item.itemType() == ITEM_TYPE_CONTAINER )
  {
    BitVector vFlags = mp_leValue1->text().toInt();
    BitVector vRetFlags = SelectObject::itemContainerFlags( vFlags, this );
    if( vRetFlags == vFlags )
      return;
    mp_leValue1->setText( QString::number( vRetFlags ) );
  }
  else if( m_item.itemType() == ITEM_TYPE_TRAP )
  {
    int dam_id = SelectObject::itemTrapDamageType( mp_leValue1->text().toInt(), this );
    if( dam_id == mp_leValue1->text().toInt() )
      return;
    mp_leValue1->setText( QString::number( dam_id ) );
  }
  else
    TS::SelectSpell( mp_leValue1, this );
}

void WndItemValues::editValue2()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::editValue2() called." );
#endif
  if( m_item.itemType() == ITEM_TYPE_CONTAINER )
  {
    VNumber def_vnum = mp_leValue2->text().toLong();
    VNumber vnum_selected = SelectObject::key( m_area, def_vnum, this );
    if( vnum_selected == def_vnum )
      return;
    mp_leValue2->setText( QString::number( vnum_selected ) );
  }
  else if( m_item.itemType() == ITEM_TYPE_LIQUID_CONTAINER )
  {
    int liquid_id = SelectObject::liquid( mp_leValue2->text().toInt(), this );
    if( liquid_id == mp_leValue2->text().toInt() )
      return;
    mp_leValue2->setText( QString::number( liquid_id ) );
  }
  else
    TS::SelectSpell( mp_leValue2, this );
}

void WndItemValues::editValue3()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemValues::editValue3() called." );
#endif
  if( m_item.itemType() == ITEM_TYPE_WEAPON )
  {
    int dam_id = SelectObject::itemWeaponDamageType( mp_leValue3->text().toInt(), this );
    if( dam_id == mp_leValue3->text().toInt() )
      return;
    mp_leValue3->setText( QString::number( dam_id ) );
  }
  else if( m_item.itemType() == ITEM_TYPE_LIQUID_CONTAINER )
  {
    BitVector vFlags = mp_leValue3->text().toInt();
    BitVector vRetFlags = SelectObject::itemLiquidContainerFlags( vFlags, this );
    if( vRetFlags == vFlags )
      return;
    mp_leValue3->setText( QString::number( vRetFlags ) );
  }
  else
    TS::SelectSpell( mp_leValue3, this );
}
