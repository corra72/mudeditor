#include "wnditemaffects.h"
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <QComboBox>
#include "validator.h"
#include "config.h"
#include "constantname.h"
#include "constants.h"
#include "selectobject.h"
#include "guiutils.h"



QComboBox* WndItemAffects::getComboBoxAffect( int id )
{
  switch( id )
  {
  case ITEM_AFFECT_POSITION_0:
    return mp_comboAffect0;
  case ITEM_AFFECT_POSITION_1:
    return mp_comboAffect1;
  case ITEM_AFFECT_POSITION_2:
    return mp_comboAffect2;
  case ITEM_AFFECT_POSITION_3:
    return mp_comboAffect3;
  case ITEM_AFFECT_POSITION_4:
    return mp_comboAffect4;
  default:
    return NULL;
  }
}

QLineEdit* WndItemAffects::getLineEditAffect( int id )
{
  switch( id )
  {
  case ITEM_AFFECT_POSITION_0:
    return mp_leAffect0;
  case ITEM_AFFECT_POSITION_1:
    return mp_leAffect1;
  case ITEM_AFFECT_POSITION_2:
    return mp_leAffect2;
  case ITEM_AFFECT_POSITION_3:
    return mp_leAffect3;
  case ITEM_AFFECT_POSITION_4:
    return mp_leAffect4;
  default:
    return NULL;
  }
}

WndItemAffects::WndItemAffects( const Area& ref_area, Item& ref_item, QWidget* parent )
  : QDialog( parent ), Ui::GuiItemAffects(),
    m_area( ref_area ),
    m_item( ref_item )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::WndItemAffects( const Area&, Item&, QWidget* ) called." );
#endif
  init();
  loadData();
}

WndItemAffects::~WndItemAffects()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::~WndItemAffects() called." );
#endif
}

void WndItemAffects::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::init() called." );
#endif
  setupUi( this );

  mp_tbAffect0->setIcon( TS::GetEditIcon() );
  mp_tbAffect1->setIcon( TS::GetEditIcon() );
  mp_tbAffect2->setIcon( TS::GetEditIcon() );
  mp_tbAffect3->setIcon( TS::GetEditIcon() );
  mp_tbAffect4->setIcon( TS::GetEditIcon() );

  for( int j = 0; j < ITEM_AFFECT_POSITION_END; j++ )
  {
    getComboBoxAffect( j )->clear();
    for( int i = 0; i < Eleuconf::countApplies(); i++ ) {
      if(Eleuconf::getAppliesDesc( i ) == QString("R"))
          getComboBoxAffect( j )->insertItem( i, Eleuconf::getAppliesName( i ) + QString(" (Riservato, non usare)") );
      else
          getComboBoxAffect( j )->insertItem( i, Eleuconf::getAppliesName( i ) );
    }
  }

  mp_leAffect0->setValidator( Validator::integer() );
  mp_leAffect1->setValidator( Validator::integer() );
  mp_leAffect2->setValidator( Validator::integer() );
  mp_leAffect3->setValidator( Validator::integer() );
  mp_leAffect4->setValidator( Validator::integer() );

  connect( mp_tbAffect0, SIGNAL( clicked() ), this, SLOT( editAffect0() ) );
  connect( mp_tbAffect1, SIGNAL( clicked() ), this, SLOT( editAffect1() ) );
  connect( mp_tbAffect2, SIGNAL( clicked() ), this, SLOT( editAffect2() ) );
  connect( mp_tbAffect3, SIGNAL( clicked() ), this, SLOT( editAffect3() ) );
  connect( mp_tbAffect4, SIGNAL( clicked() ), this, SLOT( editAffect4() ) );

  connect( mp_leAffect0, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leAffect1, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leAffect2, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leAffect3, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leAffect4, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboAffect0, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboAffect1, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboAffect2, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboAffect3, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboAffect4, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );

  connect( mp_pbOk, SIGNAL( clicked() ), this, SLOT( saveAndClose() ) );
  connect( mp_pbSave, SIGNAL( clicked() ), this, SLOT( saveData() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );

}

void WndItemAffects::refreshPanel()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::refreshPanel() called." );
#endif

  for( int i = 0; i < ITEM_AFFECT_POSITION_END; i++ )
  {
    ItemAffect ia = m_item.affect( i );
    getComboBoxAffect( i )->setCurrentIndex( ia.type() );
    getLineEditAffect( i )->setText( QString::number( ia.value() ) );
  }
}

void WndItemAffects::refreshTitle()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::refreshTitle() called." );
#endif
  QString sTitle = "";
  sTitle.sprintf( "ItemAffects Oggetto #%ld - %s", m_item.vnumber(), qPrintable( m_item.name() ) );
  if( mp_pbSave->isEnabled() )
    sTitle.append( QString( " [modificati]" ) );
  setWindowTitle( sTitle );
  mp_gbAffects->setTitle( ConstantName::itemType( m_item.itemType() ) );
}

void WndItemAffects::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::somethingChanged() called." );
#endif
  mp_pbSave->setEnabled( true );
  refreshTitle();
}

void WndItemAffects::loadData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::loadData() called." );
#endif
  refreshPanel();
  mp_pbSave->setEnabled( false );
  refreshTitle();
}

void WndItemAffects::saveData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::saveData() called." );
#endif

  for( int i = 0; i < ITEM_AFFECT_POSITION_END; i++ )
  {
    if( getComboBoxAffect( i )->currentIndex() != ITEM_AFFECT_NONE )
    {
      ItemAffect ia;
      ia.setType( getComboBoxAffect( i )->currentIndex() );
      ia.setValue( getLineEditAffect( i )->text().toLong() );
      m_item.setAffect( i, ia );
    }
    else
      m_item.removeAffect( i );
  }

  mp_pbSave->setEnabled( false );
  emit( dataSaved() );
}

void WndItemAffects::saveAndClose()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::saveAndClose() called." );
#endif
  if( mp_pbSave->isEnabled() )
    saveData();
  close();
}

void WndItemAffects::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::closeEvent( QCloseEvent* ) called." );
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

void WndItemAffects::editAffect0()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::editAffect0() called." );
#endif
  editAffect( ITEM_AFFECT_POSITION_0 );
}

void WndItemAffects::editAffect1()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::editAffect1() called." );
#endif
  editAffect( ITEM_AFFECT_POSITION_1 );
}

void WndItemAffects::editAffect2()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::editAffect2() called." );
#endif
  editAffect( ITEM_AFFECT_POSITION_2 );
}

void WndItemAffects::editAffect3()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::editAffect3() called." );
#endif
  editAffect( ITEM_AFFECT_POSITION_3 );
}

void WndItemAffects::editAffect4()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::editAffect4() called." );
#endif
  editAffect( ITEM_AFFECT_POSITION_4 );
}

void WndItemAffects::editAffect( int id )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndItemAffects::editAffect( int ) called." );
#endif

  QLineEdit *ple = getLineEditAffect( id );
  QComboBox *pcb = getComboBoxAffect( id );

  long newValue = 0;
  long curValue = ple->text().toLong();

  switch( pcb->currentIndex() )
  {
    case ITEM_AFFECT_RESISTANT:
	  newValue = SelectObject::damageImmunityType( curValue, "Resistenze", this );
      break;
    case ITEM_AFFECT_SUSCEPTIBLE:
	  newValue = SelectObject::damageImmunityType( curValue, "Suscettibilita'", this );
      break;
    case ITEM_AFFECT_IMMUNE:
      newValue = SelectObject::damageImmunityType( curValue, "Immunita'", this );
      break;

    case ITEM_AFFECT_EAT_SPELL:
    case ITEM_AFFECT_WEAPON_SPELL:
      newValue = SelectObject::spell( curValue, this );
      break;

    case ITEM_AFFECT_SPELL:
      newValue = SelectObject::affectFlags( curValue, this );
      break;

    case ITEM_AFFECT_ALIGNMENT_SLAYER:
      newValue = SelectObject::weaponAlignmentSlayer( curValue, this );
      break;

    case ITEM_AFFECT_RACE_SLAYER:
      newValue = SelectObject::race( curValue, this );
      break;

    case ITEM_AFFECT_BV2:
        newValue = SelectObject::affect2Flags( curValue, this );
          break;
    default:
        newValue = curValue;
        break;
  }

  if( newValue != curValue )
  {
    ple->setText( QString::number( newValue ) );
    //somethingChanged();
  }
}
