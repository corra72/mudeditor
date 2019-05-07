#include "wndmob.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSpinBox>
#include <QCheckBox>
#include <QToolButton>
#include "validator.h"
#include "config.h"
#include "constantname.h"
#include "selectobject.h"
#include "guiutils.h"
#include "kreatorsettings.h"

using namespace ts;


WndMob::WndMob( Area& ref_area, const Mob& ref_mob, QWidget* parent )
  : QDialog( parent ), Ui::GuiMob(),
    m_area( ref_area ),
    m_mob( ref_mob )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::WndMob( const Area&, const Mob&, QWidget* ) called." );
#endif
  init();
  loadData();
  mp_leName->setFocus();
}

WndMob::~WndMob()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::~WndMob() called." );
#endif
}

void WndMob::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::init() called." );
#endif
  setupUi( this );
  KreatorSettings::instance().loadGuiStatus( "MobWindow", this );

  mp_teDescription->setFont( TS::GetFixedFont() );

  mp_tbActs->setIcon( TS::GetEditIcon() );
  mp_tbNewActs->setIcon( TS::GetEditIcon() );
  mp_tbAffects->setIcon( TS::GetEditIcon() );
  mp_tbAffects2->setIcon( TS::GetEditIcon() );
  mp_tbResistances->setIcon( TS::GetEditIcon() );
  mp_tbImmunities->setIcon( TS::GetEditIcon() );
  mp_tbSusceptibilities->setIcon( TS::GetEditIcon() );
  TS::SetLeftArrow( mp_pbPrevMob );
  TS::SetRightArrow( mp_pbNextMob );
  mp_pbInit->setIcon( TS::GetInitIcon() );

  mp_leVNumber->setValidator( Validator::vnumber() );

  int i;
  mp_comboRace->clear();
  for( i = 0; i < CHARACTER_RACE_END; i++ )
    mp_comboRace->insertItem( i, ConstantName::characterRace( i ) );

  mp_comboGender->clear();
  for( i = 0; i < CHARACTER_GENDER_END; i++ )
    mp_comboGender->insertItem( i, ConstantName::characterGender( i ).toLower() );

  mp_comboLoadPosition->clear();
  mp_comboDefaultPosition->clear();
  for( i = 0; i < CHARACTER_POSITION_END; i++ )
  {
    mp_comboLoadPosition->insertItem( i, ConstantName::characterPosition( i ).toLower() );
    mp_comboDefaultPosition->insertItem( i, ConstantName::characterPosition( i ).toLower() );
  }

  mp_comboType->clear();
  for( i = 0; i < MOB_TYPE_END; i++ )
    mp_comboType->insertItem( i, ConstantName::mobType( i ).toLower() );

  mp_leAlignment->setValidator( Validator::integer() );
  mp_leXPBonus->setValidator( Validator::integer() );
  mp_leHPBonus->setValidator( Validator::integer() );
  mp_leGold->setValidator( Validator::unsignedInteger() );

//  connect( mp_pbInit, SIGNAL( clicked() ), this, SLOT( editInits() ));

  connect( mp_comboType, SIGNAL( activated( int ) ), this, SLOT( typeSelected( int ) ) );
  connect( mp_tbActs, SIGNAL( clicked() ), this, SLOT( editActs() ) );
  connect( mp_tbNewActs, SIGNAL( clicked() ), this, SLOT( editNewActs() ) );
  connect( mp_tbAffects, SIGNAL( clicked() ), this, SLOT( editAffects() ) );
  connect( mp_tbAffects2, SIGNAL( clicked() ), this, SLOT( editAffects2() ) );
  connect( mp_tbResistances, SIGNAL( clicked() ), this, SLOT( editResistances() ) );
  connect( mp_tbImmunities, SIGNAL( clicked() ), this, SLOT( editImmunities() ) );
  connect( mp_tbSusceptibilities, SIGNAL( clicked() ), this, SLOT( editSusceptibilities() ) );

  connect( mp_pbPrevMob, SIGNAL( clicked() ), this, SLOT( showPrevMob() ) );
  connect( mp_pbNextMob, SIGNAL( clicked() ), this, SLOT( showNextMob() ) );

  connect( mp_leVNumber, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leName, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leShort, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leLong, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboRace, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_sbLevel, SIGNAL( valueChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leAlignment, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leXPBonus, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leHPBonus, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboGender, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_sbAttacks, SIGNAL( valueChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_sbArmor, SIGNAL( valueChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_sbThac0, SIGNAL( valueChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leDamage, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboDefaultPosition, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_comboLoadPosition, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_sbSpellPower, SIGNAL( valueChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_sbAbsorbDam, SIGNAL( valueChanged( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_leGold, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_teDescription, SIGNAL( textChanged() ), this, SLOT( somethingChanged() ) );
  connect( mp_teRoomSound, SIGNAL( textChanged() ), this, SLOT( somethingChanged() ) );
  connect( mp_teNearSound, SIGNAL( textChanged() ), this, SLOT( somethingChanged() ) );

  connect( mp_pbSave, SIGNAL( clicked() ), this, SLOT( saveData() ) );
  connect( mp_pbRestore, SIGNAL( clicked() ), this, SLOT( restoreData() ) );
  connect( mp_pbOk, SIGNAL( clicked() ), this, SLOT( saveAndClose() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );
}

void WndMob::refreshPanel()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::refreshPanel() called." );
#endif

  mp_leVNumber->setText( QString::number( m_mob.vnumber() ) );
  mp_leName->setText( m_mob.name() );
  mp_comboType->setCurrentIndex( m_mob.mobType() );
  typeSelected( m_mob.mobType() );
  mp_leShort->setText( m_mob.shortDescription() );
  mp_leLong->setText( m_mob.longDescription() );
  mp_comboRace->setCurrentIndex( m_mob.race() );
  mp_sbLevel->setValue( m_mob.mobLevel() );
  mp_leAlignment->setText( QString::number( m_mob.alignment() ) );
  mp_leXPBonus->setText( QString::number( m_mob.xpBonus() ) );
  mp_leHPBonus->setText( QString::number( m_mob.hitPointsBonus() ) );
  mp_comboGender->setCurrentIndex( m_mob.gender() );
  mp_sbAttacks->setValue( (int)m_mob.attacks() );
  mp_sbArmor->setValue( m_mob.armor() / 10 );
  mp_sbThac0->setValue( m_mob.thac0() );
  mp_leDamage->setText( m_mob.damage().toString() );
  mp_comboDefaultPosition->setCurrentIndex( m_mob.mobDefaultPosition() );
  mp_comboLoadPosition->setCurrentIndex( m_mob.position() );
  mp_sbSpellPower->setValue( m_mob.spellPower() );
  mp_sbAbsorbDam->setValue( m_mob.absorbDam() );
  mp_leGold->setText( QString::number( m_mob.gold() ) );
  mp_teDescription->setPlainText( m_mob.description() );
  mp_teRoomSound->setPlainText( m_mob.roomSound() );
  mp_teNearSound->setPlainText( m_mob.nearSound() );
}

void WndMob::refreshTitle()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::refreshTitle() called." );
#endif
  QString sTitle = "";
  sTitle.sprintf( "Mob #%ld - %s", m_mob.vnumber(), qPrintable( m_mob.name() ) );
  if( mp_pbSave->isEnabled() )
    sTitle.append( QString( " [modificato]" ) );
  setWindowTitle( sTitle );

  if( mp_comboType->currentIndex() == MOB_TYPE_MULTI_ATTACKS || mp_comboType->currentIndex() == MOB_TYPE_UNBASHABLE || mp_comboType->currentIndex() == MOB_TYPE_SOUND )
  {
      mp_tbAffects2->setEnabled( true );
  } else {
      mp_tbAffects2->setEnabled( false );
  }
}

void WndMob::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::somethingChanged() called." );
#endif
  mp_pbSave->setEnabled( true );
  refreshTitle();
}

void WndMob::restoreData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::restoreData() called." );
#endif
  if( m_area.hasMob( m_mob.vnumber() ) )
  {
    m_mob = m_area.mob( m_mob.vnumber() );
    loadData();
  }
  else
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
                   TS::MESSAGE_WARNING_NO_VNUMBER.arg( m_mob.vnumber() ) );
}

void WndMob::loadData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::loadData() called." );
#endif
  refreshPanel();
  refreshAverageHpAndXP();
  refreshActs();
  refreshNewActs();
  refreshAffects();
  refreshAffects2();
  refreshResistances();
  refreshImmunities();
  refreshSusceptibilities();
  mp_pbSave->setEnabled( false );
  refreshTitle();
}

void WndMob::saveData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::saveData() called." );
#endif
  VNumber old_vnum = m_mob.vnumber();
  VNumber new_vnum = mp_leVNumber->text().toLong();

  if( old_vnum != new_vnum )
  {
    if( m_area.hasMob( new_vnum )
      && QMessageBox::question( this, TS::MESSAGE_BOX_QUESTION,
                        TS::MESSAGE_ASK_OVERWRITE_VNUMBER.arg( new_vnum ),
                        trUtf8( "Si" ), trUtf8( "No" ), QString(), 1, 1 )
      )
      return;

    if( !QMessageBox::question( this, TS::MESSAGE_BOX_QUESTION,
                        TS::MESSAGE_ASK_UPDATE_REFERENCES.arg( old_vnum ).arg( new_vnum ),
                        trUtf8( "Si" ), trUtf8( "No" ), QString(), 0, 1 )
      )
      m_area.changeMobVNumber( old_vnum, new_vnum, true );

  }

  m_mob.setVNumber( new_vnum );
  m_mob.setName( mp_leName->text() );
  m_mob.setMobType( mp_comboType->currentIndex() );
  m_mob.setShortDescription( mp_leShort->text() );
  m_mob.setLongDescription( mp_leLong->text() );
  m_mob.setRace( mp_comboRace->currentIndex() );
  m_mob.setMobLevel( mp_sbLevel->value() );
  m_mob.setAlignment( mp_leAlignment->text().toInt() );
  m_mob.setXpBonus( mp_leXPBonus->text().toLong() );
  m_mob.setHitPointsBonus( mp_leHPBonus->text().toInt() );
  m_mob.setGender( mp_comboGender->currentIndex() );
  m_mob.setAttacks( mp_sbAttacks->value() );
  m_mob.setArmor( mp_sbArmor->value() * 10 );
  m_mob.setHitRoll( 20 - mp_sbThac0->value() );
  m_mob.setDamage( Dice( mp_leDamage->text().simplified() ) );
  m_mob.setMobDefaultPosition( mp_comboDefaultPosition->currentIndex() );
  m_mob.setPosition( mp_comboLoadPosition->currentIndex() );
  m_mob.setSpellPower( mp_sbSpellPower->value() );
  m_mob.setAbsorbDam( mp_sbAbsorbDam->value() );
  m_mob.setGold( mp_leGold->text().toInt() );
  m_mob.setDescription( mp_teDescription->toPlainText() );
  m_mob.setRoomSound( mp_teRoomSound->toPlainText() );
  m_mob.setNearSound( mp_teNearSound->toPlainText() );


  m_area.addMob( m_mob );
  m_area.setMobsChanged();
  m_area.sortMobs();

  mp_pbSave->setEnabled( false );
  refreshTitle();
  refreshAverageHpAndXP();
  emit( dataSaved() );
}

void WndMob::saveAndClose()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::saveAndClose() called." );
#endif
  if( mp_pbSave->isEnabled() )
    saveData();
  close();
}

void WndMob::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::closeEvent( QCloseEvent* ) called." );
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
    KreatorSettings::instance().saveGuiStatus( "MobWindow", this );
}

void WndMob::typeSelected( int iType )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::typeSelected( int ) called." );
#endif

  if( iType != m_mob.mobType() )
  {
    somethingChanged();
  }

  mp_teRoomSound->setEnabled( iType == MOB_TYPE_SOUND );
  mp_teNearSound->setEnabled( iType == MOB_TYPE_SOUND );
}

void WndMob::showNextMob()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::showNextMob() called." );
#endif
  QCloseEvent ce;
  closeEvent( &ce );
  if( !ce.isAccepted() )
    return;

  VNumber next_vnum = m_area.nextObjectInList( ObjectData::Object_Mob, m_mob.vnumber() );
  if( next_vnum != VNumberInvalid )
  {
    m_mob = m_area.mob( next_vnum );
    loadData();
    mp_pbPrevMob->setEnabled( true );
  }
  else
    mp_pbNextMob->setEnabled( false );
}

void WndMob::showPrevMob()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::showPrevMob() called." );
#endif

  QCloseEvent ce;
  closeEvent( &ce );
  if( !ce.isAccepted() )
    return;

  VNumber prev_vnum = m_area.prevObjectInList( ObjectData::Object_Mob, m_mob.vnumber() );
  if( prev_vnum != VNumberInvalid )
  {
    m_mob = m_area.mob( prev_vnum );
    loadData();
    mp_pbNextMob->setEnabled( true );
  }
  else
    mp_pbPrevMob->setEnabled( false );
}

void WndMob::refreshActs()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::refreshActs() called." );
#endif
  mp_leActs->setText( Utils::bitvector2string( m_mob.acts(), Eleuconf::getAllFlagsCaption(Eleuconf::mobActionsFlags) ).toLower() );
}

void WndMob::refreshNewActs()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::refreshNewActs() called." );
#endif
  mp_leNewActs->setText( Utils::bitvector2string( m_mob.extraFlags(), Eleuconf::getAllFlagsCaption(Eleuconf::mobActionsFlags, 1) ).toLower() );
}

void WndMob::refreshAffects()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::refreshAffects() called." );
#endif
  mp_leAffects->setText( Utils::bitvector2string( m_mob.affects(), Eleuconf::getAllFlagsCaption(Eleuconf::mobAffectionsFlags) ).toLower() );
}

void WndMob::refreshAffects2()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::refreshAffects2() called." );
#endif
  mp_leAffects2->setText( Utils::bitvector2string( m_mob.affects2(), Eleuconf::getAllFlagsCaption(Eleuconf::mobAffectionsFlags, 1) ).toLower() );
}

void WndMob::refreshResistances()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::refreshResistances() called." );
#endif
  mp_leResistances->setText( Utils::bitvector2string( m_mob.resistances(), Eleuconf::getAllFlagsCaption(Eleuconf::resFlags) ).toLower() );
}

void WndMob::refreshImmunities()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::refreshImmunities() called." );
#endif
  mp_leImmunities->setText( Utils::bitvector2string( m_mob.immunities(), Eleuconf::getAllFlagsCaption(Eleuconf::resFlags) ).toLower() );
}

void WndMob::refreshSusceptibilities()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::refreshSusceptibilities() called." );
#endif
  mp_leSusceptibilities->setText( Utils::bitvector2string( m_mob.susceptibilities(), Eleuconf::getAllFlagsCaption(Eleuconf::resFlags) ).toLower() );
}

void WndMob::editActs()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::editActs( int ) called." );
#endif
  BitVector vFlags = m_mob.acts();
  BitVector vRetFlags = SelectObject::mobActs( vFlags, this );
  if( vRetFlags == vFlags )
    return;
  m_mob.setActs( vRetFlags );
  refreshActs();
  somethingChanged();
}

void WndMob::editNewActs()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::editNewActs( int ) called." );
#endif
  BitVector vFlags = m_mob.extraFlags();
  BitVector vRetFlags = SelectObject::mobNewActs( vFlags, this );
  if( vRetFlags == vFlags )
      return;
  m_mob.setExtraFlags( vRetFlags );
  refreshNewActs();
  somethingChanged();
}

void WndMob::editAffects()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::editAffects( int ) called." );
#endif
  BitVector vFlags = m_mob.affects();
  BitVector vRetFlags = SelectObject::affectMobFlags( vFlags, this );
  if( vRetFlags == vFlags )
    return;
  m_mob.setAffects( vRetFlags );
  refreshAffects();
  somethingChanged();
}

void WndMob::editAffects2()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::editAffects2( int ) called." );
#endif
  BitVector vFlags = m_mob.affects2();
  BitVector vRetFlags = SelectObject::affect2Flags( vFlags, this );
  if( vRetFlags == vFlags )
    return;
  m_mob.setAffects2( vRetFlags );
  refreshAffects2();
  somethingChanged();
}

void WndMob::editResistances()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::editResistances( int ) called." );
#endif
  BitVector vFlags = m_mob.resistances();
  BitVector vRetFlags = SelectObject::damageImmunityType( vFlags, "Resistenze", this );
  if( vRetFlags == vFlags )
    return;
  m_mob.setResistances( vRetFlags );
  refreshResistances();
  somethingChanged();
}

void WndMob::editImmunities()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::editImmunities( int ) called." );
#endif
  BitVector vFlags = m_mob.immunities();
  BitVector vRetFlags = SelectObject::damageImmunityType( vFlags, "Immunita'",this );
  if( vRetFlags == vFlags )
    return;
  m_mob.setImmunities( vRetFlags );
  refreshImmunities();
  somethingChanged();
}

void WndMob::editSusceptibilities()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::editSusceptibilities( int ) called." );
#endif
  BitVector vFlags = m_mob.susceptibilities();
  BitVector vRetFlags = SelectObject::damageImmunityType( vFlags, "Suscettibilita'", this );
  if( vRetFlags == vFlags )
    return;
  m_mob.setSusceptibilities( vRetFlags );
  refreshSusceptibilities();
  somethingChanged();
}
/*
void WndMob::editInits()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::editInits() called." );
#endif

    SelectObject::showInits(true, m_mob.vnumber(), m_area, this);
}
*/
void WndMob::refreshAverageHpAndXP()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMob::refreshAverageHpAndXP() called." );
#endif
  mp_leXP->setText( QString::number( m_mob.experience() ) );
  mp_leHP->setText( QString::number( m_mob.maxHitPoints() ) );
}
