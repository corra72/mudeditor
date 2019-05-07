
#ifndef TS_CONSTANTNAME_H
#define TS_CONSTANTNAME_H

#include <qstring.h>


namespace ts
{
  class ConstantName
  {
  public:
    static QString itemType( int );
//     static QString itemFlag( int );
//     static QString itemNewFlag( int );
//     static QString itemWearPosition( int );
    static QString itemWeaponDamageType( int );
    static QString itemTrapTrigger( int );
//     static QString itemContainerFlag( int );
    static QString itemSealContainerType( int id );
//     static QString itemLiquidContainerFlag( int );
    static QString equipmentPositionName( int );
    static QString roomSector( int );
//     static QString roomFlag( int );
//     static QString roomTeleportFlag( int );
    static QString exitDirection( int );
    static QString exitDirectionListName( int );
    static QString exitDirectionToName( int );
    static QString exitDirectionFromName( int );
    static QString exitReverseDirectionName( int );
//     static QString exitFlag( int );
    static QString exitOpenCommand( int );
    static QString exitStatus( int );
    static QString characterRace( int );
    static QString characterGender( int );
    static QString characterPosition( int );
    static QString characterAbility( int );
    static QString characterGem( int );
    static QString characterClass( int );
    static QString mobType( int );
//     static QString mobAct( int );
//     static QString mobNewAct( int );
    static QString mobReactionVersusType( int );
//     static QString damageImmunityType( int );
//     static QString affectFlag( int );
//     static QString affectMobFlag( int );
//     static QString affect2Flag( int );
//     static QString weaponAlignmentSlayer( int );
    static QString zoneRepopMode( int );
//     static QString zoneFlag( int );

  private:
    ConstantName() {}
    ~ConstantName() {}

  };

} // namespace ts

#endif // TS_CONSTANTNAME_H
