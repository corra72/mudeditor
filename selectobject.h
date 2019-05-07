
#ifndef TS_SELECTOBJECT_H
#define TS_SELECTOBJECT_H

#include <QStringList>
#include "area.h"
#include "wndselectflags.h"


using namespace ts;

class QWidget;


class SelectObject
{
public:
  static VNumber zone( const Area&, VNumber def_vnum = -1, QWidget* parent = 0 );
  static VNumber item( const Area&, VNumber def_vnum = VNumberInvalid, QWidget* parent = 0 );
  static VNumber container( const Area&, VNumber def_vnum = VNumberInvalid, QWidget* parent = 0 );
  static VNumber key( const Area&, VNumber def_vnum = VNumberInvalid, QWidget* parent = 0 );
  static VNumber mob( const Area&, VNumber def_vnum = VNumberInvalid, QWidget* parent = 0 );
  static VNumber room( const Area&, VNumber def_vnum = VNumberInvalid, QWidget* parent = 0 );
  static VNumber exitRoom( const Area&, VNumber def_vnum = VNumberInvalid, QWidget* parent = 0 );
  static int reactionVersus( int def_type, QWidget* parent = 0 );
  static int reactionValue( const Area&, int current_type, int def_value, QWidget* parent = 0 );
  static int equipmentPosition( int def_value, QWidget* parent = 0 );
  static int exitStatus( int def_value, QWidget* parent = 0 );
  static int exit( const Area&, VNumber vnum_room, int def_value, QWidget* parent = 0 );
  static int spell( int def_value, QWidget* parent = 0 );
  static int itemWeaponDamageType( int def_value, QWidget* parent = 0 );
  static int itemTrapDamageType( int def_value, QWidget* parent = 0 );
  static int itemType( int def_value, QWidget* parent = 0 );
  static int liquid( int def_value, QWidget* parent = 0 );
  static int weaponAlignmentSlayer( BitVector bv, QWidget* parent = 0 );
  static int race( int def_value, QWidget* parent = 0 );
  static int itemSealContainerTypes( int def_value, QWidget* parent = 0 );

  static BitVector roomFlags( BitVector, QWidget* parent = 0 );
  static BitVector roomTeleportFlags( BitVector, QWidget* parent = 0 );
  static BitVector exitFlags( BitVector, QWidget* parent = 0 );
  static BitVector itemContainerFlags( BitVector, QWidget* parent = 0 );
  static BitVector itemLiquidContainerFlags( BitVector, QWidget* parent = 0 );
  static BitVector itemTrapTriggers( BitVector, QWidget* parent = 0 );
  static BitVector itemFlags( BitVector, QWidget* parent = 0 );
  static BitVector itemNewFlags( BitVector, QWidget* parent = 0 );
  static BitVector itemWearPositions( BitVector, QWidget* parent = 0 );
  static BitVector affectFlags( BitVector, QWidget* parent = 0 );
  static BitVector affectMobFlags( BitVector, QWidget* parent = 0 );
  static BitVector affect2Flags( BitVector, QWidget* parent = 0 );
  static BitVector mobActs( BitVector, QWidget* parent = 0 );
  static BitVector mobNewActs( BitVector, QWidget* parent = 0 );
  static BitVector damageImmunityType( BitVector, const QString&, QWidget* parent = 0 );
  static BitVector zoneFlags( BitVector, QWidget* parent = 0 );
  static QList<int> getMultipleFlags(const QList<Flags> &list, const QString &title, QWidget* parent );

  //static void showInits(bool mob, VNumber vnum, Area& ref_area, QWidget* parent);
protected:
  static long getLong( const QStringList&, const QString&, int, long, QWidget* parent = 0 );
  static BitVector getBitVector( BitVector, const QStringList &list, const QString &title, QWidget* parent = 0 );
  static BitVector getFlagsBitVector( BitVector bv, const QList<Flags> &list, const QString &title, QWidget* paren = 0 );


private:
  SelectObject() {}
  ~SelectObject() {}

};

#endif // TS_SELECTOBJECT_H
