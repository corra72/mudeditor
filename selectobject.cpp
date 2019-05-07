#include "selectobject.h"
#include <QCheckBox>
#include <QInputDialog>
#include <QMessageBox>
#include "config.h"
#include "utils.h"
#include "constantname.h"
#include "constants.h"
#include "handler.h"
// #include "wndinit.h"

using namespace ts;

namespace
{

  QStringList GetStringList( int iStart, int iEnd, QString (*FunctionConstantName)(int), int* pCurrentItem, int DefaultValue )
  {
    QStringList sList;
    QString sTmp = "";
    if( pCurrentItem )
      *pCurrentItem = 0;

    for( int i = iStart; i < iEnd; i++ )
    {
      if( pCurrentItem && i == DefaultValue )
        *pCurrentItem = i - iStart;
      sTmp.sprintf( "%d - %s", i, qPrintable( FunctionConstantName( i ) ) );
      sList << sTmp;
      sTmp = "";
    }
    return sList;
  }

}

long SelectObject::getLong( const QStringList& string_list, const QString& label, int pos, long def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::getLong( const QStringList&, const Qstring&, int, long, QWidget* ) called." );
#endif

  bool ok = false;
  QString sRet = QInputDialog::getItem( parent, TS::PROGRAM_NAME, label, string_list, pos, false, &ok );

  if( ok )
  {
    sRet = sRet.trimmed();

    if( sRet.size() <= 2 )
      return def_value;

    // We have problems with -1 value :-)

    int iIndex = sRet.indexOf( QString( "-" ), 1 );
    if( iIndex != -1 )
      sRet.truncate( iIndex - 1 );

    long lRet = sRet.toLong( &ok );
    if( ok )
      return lRet;
  }

  return def_value;
}

VNumber SelectObject::zone( const Area& area, VNumber def_vnum, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::zone( const Area&, VNumber, QWidget* ) called." );
#endif

  if( area.zones().empty() )
  {
    QMessageBox::information( parent, TS::MESSAGE_BOX_INFORMATION,
      QObject::trUtf8( "L'area selezionata non ha zone." ) );
    return def_vnum;
  }

  Area::zones_const_iterator it = area.zones().begin();
  QStringList sList;
  QString sTmp = "";
  int cur_item = 0;
  int iTmp = 0;

  while( it != area.zones().end() )
  {
    if( (*it).vnumber() == def_vnum )
      cur_item = iTmp;
    sTmp.sprintf( "%ld - %s", (*it).vnumber(), (*it).name().toUtf8().data() );
    sList << sTmp;
    sTmp = "";
    ++it;
    ++iTmp;
  }

  sList << QString::number( ZoneVNumberInvalid );

  return getLong( sList, QObject::trUtf8( "Seleziona una zona" ), cur_item, def_vnum, parent );
}

VNumber SelectObject::item( const Area& area, VNumber def_vnum, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::item( const Area&, VNumber, QWidget* ) called." );
#endif

  if( area.items().empty() )
  {
    QMessageBox::information( parent, TS::MESSAGE_BOX_INFORMATION,
      QObject::trUtf8( "L'area selezionata non ha oggetti." ) );
    return def_vnum;
  }

  Area::items_const_iterator it = area.items().begin();
  QStringList sList;
  QString sTmp = "";
  int cur_item = 0;
  int iTmp = 0;

  while( it != area.items().end() )
  {
    if( (*it).vnumber() == def_vnum )
      cur_item = iTmp;
    sTmp.sprintf( "%ld - %s", (*it).vnumber(), (*it).shortDescription().toUtf8().data() );
    sList << sTmp;
    sTmp = "";
    ++it;
    ++iTmp;
  }

  return getLong( sList, QObject::trUtf8( "Seleziona un oggetto" ), cur_item, def_vnum, parent );
}

VNumber SelectObject::key( const Area& area, VNumber def_vnum, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::key( const Area&, VNumber, QWidget* ) called." );
#endif

  if( area.items().empty() )
  {
    QMessageBox::information( parent, TS::MESSAGE_BOX_INFORMATION,
      QObject::trUtf8( "L'area selezionata non ha oggetti." ) );
    return def_vnum;
  }

  Area::items_const_iterator it = area.items().begin();
  QStringList sList;
  QString sTmp = "";
  int cur_item = 0;
  int iTmp = 2;

  sTmp = QObject::trUtf8( "-1 - Nessuna Serratura" );
  sList << sTmp;
  if( def_vnum == -1 )
    cur_item = 0;
  sTmp = QObject::trUtf8( "0 - Serratura senza chiave" );
  sList << sTmp;
  if( def_vnum == 0 )
    cur_item = 1;

  while( it != area.items().end() )
  {
    if( (*it).vnumber() == def_vnum )
      cur_item = iTmp;
    sTmp.sprintf( "%ld - %s", (*it).vnumber(), (*it).shortDescription().toUtf8().data() );
    sList << sTmp;
    sTmp = "";
    ++it;
    ++iTmp;
  }

  return getLong( sList, QObject::trUtf8( "Seleziona una chiave" ), cur_item, def_vnum, parent );
}

VNumber SelectObject::container( const Area& area, VNumber def_vnum, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::container( const Area&, VNumber, QWidget* ) called." );
#endif

  Area::items_const_iterator it = area.items().begin();
  QStringList sList;
  QString sTmp = "";
  int cur_item = 0;
  int iTmp = 0;

  while( it != area.items().end() )
  {
    if( (*it).isContainer() )
    {
      if( (*it).vnumber() == def_vnum )
        cur_item = iTmp;
      sTmp.sprintf( "%ld - %s", (*it).vnumber(), (*it).shortDescription().toUtf8().data() );
      sList << sTmp;
      sTmp = "";
      ++iTmp;
    }
    ++it;
  }

  if( sList.empty() )
  {
    QMessageBox::information( parent, TS::MESSAGE_BOX_INFORMATION,
      QObject::trUtf8( "L'area selezionata non ha oggetti contenitori." ) );
    return def_vnum;
  }

  return getLong( sList, QObject::trUtf8( "Seleziona un oggetto contenitore" ), cur_item, def_vnum, parent );
}

VNumber SelectObject::mob( const Area& area, VNumber def_vnum, QWidget* parent  )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::mob( const Area&, VNumber, QWidget* ) called." );
#endif

  if( area.mobs().empty() )
  {
    QMessageBox::information( parent, TS::MESSAGE_BOX_INFORMATION,
      QObject::trUtf8( "L'area selezionata non ha mob." ) );
    return def_vnum;
  }

  Area::mobs_const_iterator it = area.mobs().begin();
  QStringList sList;
  QString sTmp = "";
  int cur_item = 0;
  int iTmp = 0;

  while( it != area.mobs().end() )
  {

    if( (*it).vnumber() == def_vnum )
      cur_item = iTmp;
    sTmp.sprintf( "%ld - %s", (*it).vnumber(), (*it).shortDescription().toUtf8().data() );
    sList << sTmp;
    sTmp = "";
    ++it;
    ++iTmp;
  }

  return getLong( sList, QObject::trUtf8( "Seleziona un mob" ), cur_item, def_vnum, parent );
}

VNumber SelectObject::room( const Area& area, VNumber def_vnum, QWidget* parent  )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::room( const Area&, VNumber, QWidget* ) called." );
#endif

  if( area.rooms().empty() )
  {
    QMessageBox::information( parent, TS::MESSAGE_BOX_INFORMATION,
      QObject::trUtf8( "L'area selezionata non ha stanze." ) );
    return def_vnum;
  }

  Area::rooms_const_iterator it = area.rooms().begin();
  QStringList sList;
  QString sTmp = "";
  int cur_item = 0;
  int iTmp = 0;
  while( it != area.rooms().end() )
  {
    if( (*it).vnumber() == def_vnum )
      cur_item = iTmp;
    sTmp.sprintf( "%ld - %s", (*it).vnumber(), (*it).name().toUtf8().data() );
    sList << sTmp;
    sTmp = "";
    ++it;
    ++iTmp;
  }

  return getLong( sList, QObject::trUtf8( "Seleziona una stanza" ), cur_item, def_vnum, parent );
}

VNumber SelectObject::exitRoom( const Area& area, VNumber def_vnum, QWidget* parent  )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::room( const Area&, VNumber, QWidget* ) called." );
#endif

  if( area.rooms().empty() )
  {
    QMessageBox::information( parent, TS::MESSAGE_BOX_INFORMATION,
      QObject::trUtf8( "L'area selezionata non ha stanze." ) );
    return def_vnum;
  }

  Area::rooms_const_iterator it = area.rooms().begin();
  QStringList sList;
  QString sTmp = "";
  int cur_item = 0;
  int iTmp = 1;

  sTmp = QObject::trUtf8( "0 - Finta uscita" );
  sList << sTmp;
  if( def_vnum == -1 )
    cur_item = 0;

  while( it != area.rooms().end() )
  {
    if( (*it).vnumber() == def_vnum )
      cur_item = iTmp;
    sTmp.sprintf( "%ld - %s", (*it).vnumber(), (*it).name().toUtf8().data() );
    sList << sTmp;
    sTmp = "";
    ++it;
    ++iTmp;
  }

  return getLong( sList, QObject::trUtf8( "Seleziona la stanza dove conduce l'uscita" ), cur_item, def_vnum, parent );
}

int SelectObject::reactionVersus( int def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::reactionVersus( int, QWidget* ) called." );
#endif

  qDebug( "RV: DEFVALUE= %d", def_value );
  int cur_item = 0;
  QStringList sList = GetStringList( 1, MOB_REACTION_END, ConstantName::mobReactionVersusType, &cur_item, def_value );
  qDebug( "RV: CUR_ITEM= %d", cur_item );
  return getLong( sList, QObject::trUtf8( "Seleziona verso cosa il mob deve avere una reazione" ), cur_item, def_value, parent );
}

int SelectObject::reactionValue( const Area& area, int current_type, int def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::reactionValue( int, int, QWidget* ) called." );
#endif

  int cur_item = 0;
  QStringList sList;

  switch( current_type )
  {
  case MOB_REACTION_GENDER:
    sList = GetStringList( 0, CHARACTER_GENDER_END, ConstantName::characterGender, &cur_item, def_value );
    break;

  case MOB_REACTION_RACE:
    sList = GetStringList( 0, CHARACTER_RACE_END, ConstantName::characterRace, &cur_item, def_value );
    break;

  case MOB_REACTION_CLASS:
    sList = GetStringList( 0, CHARACTER_CLASS_END, ConstantName::characterClass, &cur_item, def_value );
    break;

  case MOB_REACTION_VNUMBER:
    return mob( area, def_value, parent );

  default:
    return def_value;
  }

  return getLong( sList, QObject::trUtf8( "Seleziona il valore della reazione" ), cur_item, def_value, parent );

}

int SelectObject::equipmentPosition( int def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::equipmentPosition( int, QWidget* ) called." );
#endif

  int cur_item = 0;
  QStringList sList = GetStringList( 0, EQUIPMENT_POSITION_END, ConstantName::equipmentPositionName, &cur_item, def_value );
  return getLong( sList, QObject::trUtf8( "Seleziona la posizione nell'equipaggiamento" ), cur_item, def_value, parent );
}

int SelectObject::exitStatus( int def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::exitStatus( int, QWidget* ) called." );
#endif

  int cur_item = 0;
  QStringList sList = GetStringList( 0, EXIT_STATUS_END, ConstantName::exitStatus, &cur_item, def_value );
  return getLong( sList, QObject::trUtf8( "Seleziona lo stato dell'uscita" ), cur_item, def_value, parent );
}

int SelectObject::exit( const Area& area, VNumber vnum_room, int def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::exit( const Area&, VNumber, int, QWidget* ) called." );
#endif

  if( !area.hasRoom( vnum_room ) )
  {
    QMessageBox::information( parent, TS::MESSAGE_BOX_INFORMATION,
      QObject::trUtf8( "La stanza dell'uscita selezionata non esiste." ) );
    return def_value;
  }

  const Room& r = area.room( vnum_room );

  QStringList sList;
  QString sTmp = "";
  int cur_item = 0;
  int iTmp = 0;
  for( int i = 0; i < EXIT_DIRECTION_END; i++ )
  {
    if( r.hasExit( i ) )
    {
      if( i == def_value )
        cur_item = iTmp;
      sTmp.sprintf( "%d - %s", i, ConstantName::exitDirection( i ).toUtf8().data() );
      sList << sTmp;
      sTmp = "";
      iTmp++;
    }
  }

  return getLong( sList, QObject::trUtf8( "Seleziona la direzione dell'uscita" ), cur_item, def_value, parent );
}

int SelectObject::spell( int def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::spell( int, QWidget* ) called." );
#endif

  QStringList sList;
  QString sTmp = "";
  int cur_item = 0;
  int i = 0;
  QList<Spell>::const_iterator it = Handler::spells().begin();
  while( it != Handler::spells().end() )
  {
    if( (*it).id() == def_value )
      cur_item = i;

    sTmp.sprintf( "%d - %s", (*it).id(), qPrintable( (*it).name() ) );
    sList << sTmp;

    ++i;
    ++it;
    sTmp = "";
  }

  return getLong( sList, QObject::trUtf8( "Seleziona un'incantesimo" ), cur_item, def_value, parent );
}

int SelectObject::itemTrapDamageType( int def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::itemTrapDamageType( int, QWidget* ) called." );
#endif

  QStringList sList;
  QString sTmp = "";
  int cur_item = 0;
  int i = 0;
  QList<TrapDamageType>::const_iterator it = Handler::trapDamageTypes().begin();
  while( it != Handler::trapDamageTypes().end() )
  {
    if( (*it).id() == def_value )
      cur_item = i;

    sTmp.sprintf( "%d - %s", (*it).id(), qPrintable( (*it).name() ) );
    sList << sTmp;

    ++i;
    ++it;
    sTmp = "";
  }

  return getLong( sList, QObject::trUtf8( "Seleziona un tipo di danno" ), cur_item, def_value, parent );
}

int SelectObject::itemWeaponDamageType( int def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::itemWeaponDamageType( int, QWidget* ) called." );
#endif
  int cur_item = 0;
  QStringList sList = GetStringList( 0, ITEM_WEAPON_DAMAGE_TYPE_END, ConstantName::itemWeaponDamageType, &cur_item, def_value );
  return getLong( sList, QObject::trUtf8( "Seleziona un tipo di danno" ), cur_item, def_value, parent );
}

int SelectObject::itemType( int def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::itemType( int, QWidget* ) called." );
#endif
  int cur_item = 0;
  QStringList sList = GetStringList( 0, ITEM_TYPE_END, ConstantName::itemType, &cur_item, def_value );
  return getLong( sList, QObject::trUtf8( "Seleziona il tipo di oggetto" ), cur_item, def_value, parent );
}

int SelectObject::liquid( int def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::liquid( int, QWidget* ) called." );
#endif
  int cur_item = 0;
  QStringList sList = GetStringList( 0, ITEM_LIQUID_TYPE_END, Handler::liquidName, &cur_item, def_value );
  return getLong( sList, QObject::trUtf8( "Seleziona il tipo di liquido" ), cur_item, def_value, parent );
}

int SelectObject::weaponAlignmentSlayer( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::weaponAlignmentSlayer( int, QWidget* ) called." );
#endif
  return getBitVector( bv, strWeaponAlignmentSlayerName, "Slay Align", parent);
}

int SelectObject::race( int def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::race( int, QWidget* ) called." );
#endif

  int cur_item = 0;
  QStringList sList = GetStringList( 0, CHARACTER_RACE_END, ConstantName::characterRace, &cur_item, def_value );
  return getLong( sList, QObject::trUtf8( "Seleziona la razza" ), cur_item, def_value, parent );
}

BitVector SelectObject::getBitVector( BitVector bv, const QStringList &list, const QString &title, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::getBitVector( BitVector, QStringList, QWidget* ) called." );
#endif

  WndSelectFlags *pWnd = new WndSelectFlags( bv, list, parent );
  pWnd->setWindowTitle(title);
  pWnd->show();
  pWnd->setFixedSize( pWnd->size() );
  pWnd->exec();

  if( pWnd->okPressed() && pWnd->bitVectorChanged() )
    bv = pWnd->bitVertorSelected();

  delete pWnd;

  return bv;
}

BitVector SelectObject::getFlagsBitVector( BitVector bv, const QList<Flags> &list, const QString &title, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::getFlagsBitVector( BitVector, int, QString (*FunctionConstantName)(int), QWidget* ) called." );
#endif

  WndSelectFlags *pWnd = new WndSelectFlags( bv, list, parent );
  pWnd->setWindowTitle(title);
  pWnd->show();
  pWnd->setFixedSize( pWnd->size() );
  pWnd->exec();

  if( pWnd->okPressed() && pWnd->bitVectorChanged() )
    bv = pWnd->bitVertorSelected();

  delete pWnd;

  return bv;
}

QList<int> SelectObject::getMultipleFlags(const QList<Flags> &list, const QString &title, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
    qDebug( "SelectObject::getMultipleFlags( QList<Flags>, QString (*FunctionConstantName)(int), QWidget* ) called." );
#endif
    QButtonGroup *bg = NULL;
    QList<int> ls;
    WndSelectFlags *pWnd = new WndSelectFlags( list, parent );
    pWnd->setWindowTitle(title);
    pWnd->show();
    pWnd->setFixedSize( pWnd->size() );
    pWnd->exec();

    if( pWnd->okPressed() && pWnd->bitVectorChanged() )
        bg = pWnd->getFlags();

    if(bg) {
        for(int i = 0; i < bg->buttons().size(); i++)
         {
             QCheckBox *cb = (QCheckBox*)bg->buttons().at(i);
             ls << cb->checkState();
         }
    }

    delete pWnd;

    return ls;
}

BitVector SelectObject::damageImmunityType( BitVector bv, const QString &title, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::damageImmunityType( BitVector, QWidget* ) called." );
#endif

  return getFlagsBitVector( bv, Eleuconf::resFlags, title, parent );
}

BitVector SelectObject::roomFlags( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::roomFlags( BitVector, QWidget* ) called." );
#endif

  return getFlagsBitVector( bv, Eleuconf::roomFlags, "Flags della stanza", parent );
}

BitVector SelectObject::roomTeleportFlags( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::roomTeleportFlags( BitVector, QWidget* ) called." );
#endif

  return getFlagsBitVector( bv, Eleuconf::teleportFlags, "Flags della teleport", parent );
}

BitVector SelectObject::exitFlags( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::exitFlags( BitVector, QWidget* ) called." );
#endif

  return getFlagsBitVector( bv, Eleuconf::exitFlags, "Flags dell'uscita", parent );
}

BitVector SelectObject::itemContainerFlags( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::itemContainerFlags( BitVector, QWidget* ) called." );
#endif

  return getFlagsBitVector( bv, Eleuconf::containerFlags, "Flags dei container", parent );
}

BitVector SelectObject::itemLiquidContainerFlags( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::itemLiquidContainerFlags( BitVector, QWidget* ) called." );
#endif

  return getBitVector( bv, Eleuconf::getAllFlagsCaption(Eleuconf::drinkContainerFlags), "Flag del liquid container", parent );
}

BitVector SelectObject::itemTrapTriggers( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::itemTrapTriggers( BitVector, QWidget* ) called." );
#endif

  return getFlagsBitVector( bv, Eleuconf::trapFlags, "Flags della trappola", parent );
}

BitVector SelectObject::itemFlags( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::itemFlags( BitVector, QWidget* ) called." );
#endif

  QList<Flags> list;

  for(int i = 0; i < Eleuconf::objFlags.size(); i++)
  {
      if(Eleuconf::getFlagsFindex(i, Eleuconf::objFlags) == 0) {
          list.append(Eleuconf::objFlags.at(i));
      }
  }

  return getFlagsBitVector( bv, list, "Flags dell'oggetto", parent );
}

BitVector SelectObject::itemNewFlags( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::itemNewFlags( BitVector, QWidget* ) called." );
#endif

    QList<Flags> list;

  for(int i = 0; i < Eleuconf::objFlags.size(); i++)
  {
      if(Eleuconf::getFlagsFindex(i, Eleuconf::objFlags) == 1) {
          list.append(Eleuconf::objFlags.at(i));
      }
  }

  return getFlagsBitVector( bv, list, "Nuovi flags dell'oggetto", parent );
}

BitVector SelectObject::itemWearPositions( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::itemWearPositions( BitVector, QWidget* ) called." );
#endif

  return getFlagsBitVector(bv, Eleuconf::objWear, "Wear positions", parent);
}

int  SelectObject::itemSealContainerTypes( int def_value, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::itemSealContainerType( BitVector, QWidget* ) called." );
#endif

  int cur_item = 1;
  QStringList sList = GetStringList( 0, ITEM_SEAL_CONTAINER_END, ConstantName::itemSealContainerType, &cur_item, def_value );
  return getLong( sList, QObject::trUtf8( "Tipo di seal container" ), cur_item, def_value, parent );
}

BitVector SelectObject::affectFlags( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::affectFlags( BitVector, QWidget* ) called." );
#endif

  return getBitVector( bv, Eleuconf::getAllFlagsCaption(Eleuconf::mobAffectionsFlags), "Affect flags", parent );
}

BitVector SelectObject::affectMobFlags( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::affectMobFlags( BitVector, QWidget* ) called." );
#endif

  QList<Flags> list;

  for(int i = 0; i < Eleuconf::mobAffectionsFlags.size(); i++)
  {
      if(Eleuconf::getFlagsFindex(i, Eleuconf::mobAffectionsFlags) == 0) {
          list.append(Eleuconf::mobAffectionsFlags.at(i));
      }
  }

  return getFlagsBitVector(bv, list, "Mob Affections", parent);
}

BitVector SelectObject::affect2Flags( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::affect2Flags( BitVector, QWidget* ) called." );
#endif

  QList<Flags> list;

  for(int i = 0; i < Eleuconf::mobAffectionsFlags.size(); i++)
  {
      if(Eleuconf::getFlagsFindex(i, Eleuconf::mobAffectionsFlags) == 1) {
          list.append(Eleuconf::mobAffectionsFlags.at(i));
      }
  }

  return getFlagsBitVector(bv, list, "Mob Affections 2", parent);
}

BitVector SelectObject::mobActs( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::mobActs( BitVector, QWidget* ) called." );
#endif

  QList<Flags> list;

  for(int i = 0; i < Eleuconf::mobActionsFlags.size(); i++)
  {
      if(Eleuconf::getFlagsFindex(i, Eleuconf::mobActionsFlags) == 0) {
          list.append(Eleuconf::mobActionsFlags.at(i));
      }
  }

  return getFlagsBitVector(bv, list, "Mob Acts", parent);
}

BitVector SelectObject::mobNewActs( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::mobNewActs( BitVector, QWidget* ) called." );
#endif

  QList<Flags> list;

  for(int i = 0; i < Eleuconf::mobActionsFlags.size(); i++)
  {
      if(Eleuconf::getFlagsFindex(i, Eleuconf::mobActionsFlags) == 1) {
          list.append(Eleuconf::mobActionsFlags.at(i));
      }
  }

  return getFlagsBitVector(bv, list, "Mob New Acts", parent);
}

BitVector SelectObject::zoneFlags( BitVector bv, QWidget* parent )
{
#if defined( KREATOR_DEBUG )
  qDebug( "SelectObject::zoneFlags( BitVector, QWidget* ) called." );
#endif

  return getFlagsBitVector(bv, Eleuconf::zoneFlags, "Flag della zona", parent);
}
#if 0
void SelectObject::showInits(bool mob, VNumber vnum, Area& ref_area, QWidget* parent)
{
    return;
// #if defined( KREATOR_DEBUG )
  // qDebug( "SelectObject::showInits(bool mob, VNumber vnum, Area& ref_area, const Zone& ref_zone, QWidget* parent) called." );
// #endif
  //   WndInit init(mob, vnum , ref_area, parent);
    // init.show();
 //    init.setFixedSize( init.size() );
   //  init.exec();
}
#endif
