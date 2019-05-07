#include "wndshop.h"
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
#include "validator.h"
#include "config.h"
#include "constantname.h"
#include "handler.h"
#include "selectobject.h"
#include "guiutils.h"
#include "kreatorsettings.h"

using namespace ts;


WndShop::WndShop( Area& ref_area, const Shop& ref_shop, QWidget* parent )
  : QDialog( parent ), Ui::GuiShop(),
    m_area( ref_area ),
    m_shop( ref_shop )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::WndShop( const Area&, const Shop&, QWidget* ) called." );
#endif
  init();
  loadData();
  mp_leMob->setFocus();
  setModal( true );
}

WndShop::~WndShop()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::~WndShop() called." );
#endif
}

void WndShop::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::init() called." );
#endif
  setupUi( this );
  KreatorSettings::instance().loadGuiStatus( "ShopWindow", this );

  mp_tbMob->setIcon( TS::GetEditIcon() );
  mp_tbRoom->setIcon( TS::GetEditIcon() );
  mp_tbSell0->setIcon( TS::GetEditIcon() );
  mp_tbSell1->setIcon( TS::GetEditIcon() );
  mp_tbSell2->setIcon( TS::GetEditIcon() );
  mp_tbSell3->setIcon( TS::GetEditIcon() );
  mp_tbSell4->setIcon( TS::GetEditIcon() );
  TS::SetLeftArrow( mp_pbPrevShop );
  TS::SetRightArrow( mp_pbNextShop );

  mp_leVNumber->setValidator( Validator::vnumber() );
  mp_leMob->setValidator( Validator::vnumber() );
  mp_leRoom->setValidator( Validator::vnumber() );
  mp_leSell0->setValidator( Validator::vnumber() );
  mp_leSell1->setValidator( Validator::vnumber() );
  mp_leSell2->setValidator( Validator::vnumber() );
  mp_leSell3->setValidator( Validator::vnumber() );
  mp_leSell4->setValidator( Validator::vnumber() );

  mp_comboBuy0->clear();
  mp_comboBuy1->clear();
  mp_comboBuy2->clear();
  mp_comboBuy3->clear();
  mp_comboBuy4->clear();

  for( int i = 0; i < ITEM_TYPE_END; i++ )
  {
    mp_comboBuy0->insertItem( i, ConstantName::itemType( i ).toLower() );
    mp_comboBuy1->insertItem( i, ConstantName::itemType( i ).toLower() );
    mp_comboBuy2->insertItem( i, ConstantName::itemType( i ).toLower() );
    mp_comboBuy3->insertItem( i, ConstantName::itemType( i ).toLower() );
    mp_comboBuy4->insertItem( i, ConstantName::itemType( i ).toLower() );
  }

  connect( mp_leVNumber, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leMob, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leRoom, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leSell0, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leSell1, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leSell2, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leSell3, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leSell4, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboBuy0, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboBuy1, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboBuy2, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboBuy3, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboBuy4, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_sbProfitSell, SIGNAL( valueChanged( double ) ), this, SLOT( somethingChanged() ) );
  connect( mp_sbProfitBuy, SIGNAL( valueChanged( double ) ), this, SLOT( somethingChanged() ) );
  connect( mp_sbOpen1, SIGNAL( valueChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_sbOpen2, SIGNAL( valueChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_sbClose1, SIGNAL( valueChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_sbClose2, SIGNAL( valueChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboReaction0, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboReaction1, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leAnswer0, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leAnswer1, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leAnswer2, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leAnswer3, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leAnswer4, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leAnswer5, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leAnswer6, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );

  connect( mp_tbMob, SIGNAL( clicked() ), this, SLOT( editMob() ) );
  connect( mp_tbRoom, SIGNAL( clicked() ), this, SLOT( editRoom() ) );
  connect( mp_tbSell0, SIGNAL( clicked() ), this, SLOT( editSell0() ) );
  connect( mp_tbSell1, SIGNAL( clicked() ), this, SLOT( editSell1() ) );
  connect( mp_tbSell2, SIGNAL( clicked() ), this, SLOT( editSell2() ) );
  connect( mp_tbSell3, SIGNAL( clicked() ), this, SLOT( editSell3() ) );
  connect( mp_tbSell4, SIGNAL( clicked() ), this, SLOT( editSell4() ) );
  connect( mp_pbPrevShop, SIGNAL( clicked() ), this, SLOT( showPrevShop() ) );
  connect( mp_pbNextShop, SIGNAL( clicked() ), this, SLOT( showNextShop() ) );
  connect( mp_pbSave, SIGNAL( clicked() ), this, SLOT( saveData() ) );
  connect( mp_pbRestore, SIGNAL( clicked() ), this, SLOT( restoreData() ) );
  connect( mp_pbOk, SIGNAL( clicked() ), this, SLOT( saveAndClose() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
}

void WndShop::refreshPanel()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::refreshPanel() called." );
#endif

  mp_leVNumber->setText( QString::number( m_shop.vnumber() ) );
  mp_leMob->setText( QString::number( m_shop.keeper() ) );
  mp_leRoom->setText( QString::number( m_shop.room() ) );

  mp_leSell0->setText( QString::number( m_shop.itemToSell( 0 ) ) );
  mp_leSell1->setText( QString::number( m_shop.itemToSell( 1 ) ) );
  mp_leSell2->setText( QString::number( m_shop.itemToSell( 2 ) ) );
  mp_leSell3->setText( QString::number( m_shop.itemToSell( 3 ) ) );
  mp_leSell4->setText( QString::number( m_shop.itemToSell( 4 ) ) );

  mp_comboBuy0->setCurrentIndex( m_shop.itemTypeToBuy( 0 ) );
  mp_comboBuy1->setCurrentIndex( m_shop.itemTypeToBuy( 1 ) );
  mp_comboBuy2->setCurrentIndex( m_shop.itemTypeToBuy( 2 ) );
  mp_comboBuy3->setCurrentIndex( m_shop.itemTypeToBuy( 3 ) );
  mp_comboBuy4->setCurrentIndex( m_shop.itemTypeToBuy( 4 ) );

  mp_sbProfitSell->setValue( m_shop.profitSell() );
  mp_sbProfitBuy->setValue( m_shop.profitBuy() );

  mp_sbOpen1->setValue( m_shop.time( SHOP_TIME_OPEN_1 ) );
  mp_sbOpen2->setValue( m_shop.time( SHOP_TIME_OPEN_2 ) );
  mp_sbClose1->setValue( m_shop.time( SHOP_TIME_CLOSE_1 ) );
  mp_sbClose2->setValue( m_shop.time( SHOP_TIME_CLOSE_2 ) );

  mp_comboReaction0->setCurrentIndex( m_shop.reaction( SHOP_REACTION_TEMPER ) );
  mp_comboReaction1->setCurrentIndex( m_shop.reaction( SHOP_REACTION_ATTACK ) );

  mp_leAnswer0->setText( m_shop.answer( 0 ) );
  mp_leAnswer1->setText( m_shop.answer( 1 ) );
  mp_leAnswer2->setText( m_shop.answer( 2 ) );
  mp_leAnswer3->setText( m_shop.answer( 3 ) );
  mp_leAnswer4->setText( m_shop.answer( 4 ) );
  mp_leAnswer5->setText( m_shop.answer( 5 ) );
  mp_leAnswer6->setText( m_shop.answer( 6 ) );
}

void WndShop::refreshTitle()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::refreshTitle() called." );
#endif
  QString sTitle = "";
  sTitle.sprintf( "Negozio #%ld - %s", m_shop.vnumber(), qPrintable( m_area.roomName( m_shop.room() ) ) );
  if( mp_pbSave->isEnabled() )
    sTitle.append( QString( " [modificato]" ) );
  setWindowTitle( sTitle );
}

void WndShop::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::somethingChanged() called." );
#endif
  mp_pbSave->setEnabled( true );
}

void WndShop::restoreData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::restoreData() called." );
#endif
  if( m_area.hasShop( m_shop.vnumber() ) )
  {
    m_shop = m_area.shop( m_shop.vnumber() );
    loadData();
  }
  else
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
                   TS::MESSAGE_WARNING_NO_VNUMBER.arg( m_shop.vnumber() ) );
}

void WndShop::loadData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::loadData() called." );
#endif
  refreshPanel();
  mp_pbSave->setEnabled( false );
  refreshTitle();
}

void WndShop::saveData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::saveData() called." );
#endif
  VNumber old_vnum = m_shop.vnumber();
  VNumber new_vnum = mp_leVNumber->text().toLong();

  if( old_vnum != new_vnum )
  {
    if( m_area.hasShop( new_vnum )
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

  m_shop.setVNumber( new_vnum );
  m_shop.setKeeper( mp_leMob->text().toLong() );
  m_shop.setRoom( mp_leRoom->text().toLong() );
  m_shop.setItemToSell( 0, mp_leSell0->text().toLong() );
  m_shop.setItemToSell( 1, mp_leSell1->text().toLong() );
  m_shop.setItemToSell( 2, mp_leSell2->text().toLong() );
  m_shop.setItemToSell( 3, mp_leSell3->text().toLong() );
  m_shop.setItemToSell( 4, mp_leSell4->text().toLong() );
  m_shop.setItemTypeToBuy( 0, mp_comboBuy0->currentIndex() );
  m_shop.setItemTypeToBuy( 1, mp_comboBuy1->currentIndex() );
  m_shop.setItemTypeToBuy( 2, mp_comboBuy2->currentIndex() );
  m_shop.setItemTypeToBuy( 3, mp_comboBuy3->currentIndex() );
  m_shop.setItemTypeToBuy( 4, mp_comboBuy4->currentIndex() );
  m_shop.setProfitSell( mp_sbProfitSell->value() );
  m_shop.setProfitBuy( mp_sbProfitBuy->value() );
  m_shop.setTime( SHOP_TIME_OPEN_1, mp_sbOpen1->value() );
  m_shop.setTime( SHOP_TIME_OPEN_2, mp_sbOpen2->value() );
  m_shop.setTime( SHOP_TIME_CLOSE_1, mp_sbClose1->value() );
  m_shop.setTime( SHOP_TIME_CLOSE_2, mp_sbClose2->value() );
  m_shop.setReaction( SHOP_REACTION_TEMPER, mp_comboReaction0->currentIndex() );
  m_shop.setReaction( SHOP_REACTION_ATTACK, mp_comboReaction1->currentIndex() );
  m_shop.setAnswer( 0, mp_leAnswer0->text() );
  m_shop.setAnswer( 1, mp_leAnswer1->text() );
  m_shop.setAnswer( 2, mp_leAnswer2->text() );
  m_shop.setAnswer( 3, mp_leAnswer3->text() );
  m_shop.setAnswer( 4, mp_leAnswer4->text() );
  m_shop.setAnswer( 5, mp_leAnswer5->text() );
  m_shop.setAnswer( 6, mp_leAnswer6->text() );

  m_area.addShop( m_shop );
  m_area.setShopsChanged();
  m_area.sortShops();

  mp_pbSave->setEnabled( false );
  refreshTitle();
  emit( dataSaved() );
}

void WndShop::saveAndClose()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::saveAndClose() called." );
#endif
  if( mp_pbSave->isEnabled() )
    saveData();
  close();
}

void WndShop::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::closeEvent( QCloseEvent* ) called." );
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
    KreatorSettings::instance().saveGuiStatus( "ShopWindow", this );
}


void WndShop::showNextShop()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::showNextShop() called." );
#endif
  QCloseEvent ce;
  closeEvent( &ce );
  if( !ce.isAccepted() )
    return;

  VNumber next_vnum = m_area.nextObjectInList( ObjectData::Object_Shop, m_shop.vnumber() );
  if( next_vnum != VNumberInvalid )
  {
    m_shop = m_area.shop( next_vnum );
    loadData();
    mp_pbPrevShop->setEnabled( true );
  }
  else
    mp_pbNextShop->setEnabled( false );
}

void WndShop::showPrevShop()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::showPrevShop() called." );
#endif

  QCloseEvent ce;
  closeEvent( &ce );
  if( !ce.isAccepted() )
    return;

  VNumber prev_vnum = m_area.prevObjectInList( ObjectData::Object_Shop, m_shop.vnumber() );
  if( prev_vnum != VNumberInvalid )
  {
    m_shop = m_area.shop( prev_vnum );
    loadData();
    mp_pbNextShop->setEnabled( true );
  }
  else
    mp_pbPrevShop->setEnabled( false );
}

void WndShop::editSell0()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::editSell0() called." );
#endif
  VNumber def_vnum = mp_leSell0->text().toLong();
  VNumber vnumSelected = SelectObject::item( m_area, def_vnum, this );
  if( vnumSelected != def_vnum )
    mp_leSell0->setText( QString::number( vnumSelected ) );
}

void WndShop::editSell1()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::editSell1() called." );
#endif
  VNumber def_vnum = mp_leSell1->text().toLong();
  VNumber vnumSelected = SelectObject::item( m_area, def_vnum, this );
  if( vnumSelected != def_vnum )
    mp_leSell1->setText( QString::number( vnumSelected ) );
}

void WndShop::editSell2()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::editSell2() called." );
#endif
  VNumber def_vnum = mp_leSell2->text().toLong();
  VNumber vnumSelected = SelectObject::item( m_area, def_vnum, this );
  if( vnumSelected != def_vnum )
    mp_leSell2->setText( QString::number( vnumSelected ) );
}

void WndShop::editSell3()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::editSell3() called." );
#endif
  VNumber def_vnum = mp_leSell3->text().toLong();
  VNumber vnumSelected = SelectObject::item( m_area, def_vnum, this );
  if( vnumSelected != def_vnum )
    mp_leSell3->setText( QString::number( vnumSelected ) );
}


void WndShop::editSell4()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::editSell4() called." );
#endif
  VNumber def_vnum = mp_leSell4->text().toLong();
  VNumber vnumSelected = SelectObject::item( m_area, def_vnum, this );
  if( vnumSelected != def_vnum )
    mp_leSell4->setText( QString::number( vnumSelected ) );
}

void WndShop::editMob()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::editMob() called." );
#endif
  VNumber def_vnum = mp_leMob->text().toLong();
  VNumber vnumSelected = SelectObject::mob( m_area, def_vnum, this );
  if( vnumSelected != def_vnum )
    mp_leMob->setText( QString::number( vnumSelected ) );
}

void WndShop::editRoom()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndShop::editRoom() called." );
#endif
  VNumber def_vnum = mp_leRoom->text().toLong();
  VNumber vnumSelected = SelectObject::room( m_area, def_vnum, this );
  if( vnumSelected != def_vnum )
    mp_leRoom->setText( QString::number( vnumSelected ) );
}
