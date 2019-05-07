
#ifndef TS_CHARACTERDATA_H
#define TS_CHARACTERDATA_H


#include <QString>
#include "objectdata.h"
#include "constants.h"
#include "bit.h"
#include "utils.h"
#include "dice.h"


namespace ts
{

  class CharacterData : public ObjectData
  {
  public:
    void init();
    void validate();

    CharacterData();
    CharacterData( Type );
    CharacterData( Type, VNumber, const QString& object_name = "" );
    CharacterData( const CharacterData& );

    virtual ~CharacterData();

    virtual CharacterData& operator=( const CharacterData& );

    const QString& shortDescription() const { return m_shortDescription; }
    const QString& longDescription() const { return m_longDescription; }
    const QString& description() const { return m_description; }
    int gender() const { return m_gender; }
    int race() const { return m_race; }
    int weight() const { return m_weight; }
    int height() const { return m_height; }
    int alignment() const { return m_alignment; }
    int strength() const { return m_abilities[ CHARACTER_ABILITY_STRENGTH ]; }
    int dexterity() const { return m_abilities[ CHARACTER_ABILITY_DEXTERITY ]; }
    int constitution() const { return m_abilities[ CHARACTER_ABILITY_CONSTITUTION ]; }
    int intelligence() const { return m_abilities[ CHARACTER_ABILITY_INTELLIGENCE ]; }
    int wisdom() const { return m_abilities[ CHARACTER_ABILITY_WISDOM ]; }
    int charisma() const { return m_abilities[ CHARACTER_ABILITY_CHARISMA ]; }
    int hitPoints() const { return m_hitPoints; }
    int maxHitPoints() const { return m_maxHitPoints; }
    int manaPoints() const { return m_manaPoints; }
    int maxManaPoints() const { return m_maxManaPoints; }
    int movePoints() const { return m_movePoints; }
    int maxMovePoints() const { return m_maxMovePoints; }
    float attacks() const { return m_attacks; }
    int hitRoll() const { return m_hitRoll; }
    int thac0() const { return 20 - m_hitRoll; }
    int armor() const { return m_armor; }
    Dice damage() const { return m_damage; }
    int damageBonus() const { return m_damage.bonus(); }
    int gold() const { return m_gold; }
    BitVector affects() const { return m_affects; }
    BitVector affects2() const { return m_affects2; }
    BitVector immunities() const { return m_immunities; }
    BitVector resistances() const { return m_resistances; }
    BitVector susceptibilities() const { return m_susceptibilities; }
    bool hasAffect( int flag ) const { return Bit::has( m_affects, flag ); }
    bool hasAffect2( int flag ) const { return Bit::has( m_affects2, flag ); }
    bool hasImmunity( int flag ) const { return Bit::has( m_immunities, flag ); }
    bool hasResistance( int flag ) const { return Bit::has( m_resistances, flag ); }
    bool hasSusceptibility( int flag ) const { return Bit::has( m_susceptibilities, flag ); }
    long experience() const { return m_experience; }
    int position() const { return m_position; }
    int spellPower() const { return m_spellPower; }
    int absorbDam() const { return m_absorbDam; }
    BitVector extraFlags() const { return m_extraFlags; }
    bool hasExtraFlags( int flag ) const { return Bit::has ( m_extraFlags, flag); }

    void setShortDescription( const QString& txt ) { m_shortDescription = Utils::removeTilde( txt.simplified() ); }
    void setLongDescription( const QString& txt ) { m_longDescription = Utils::removeTilde( txt.trimmed() ); }
    void setDescription( const QString& txt ) { m_description = Utils::removeTilde( txt.trimmed() ); }
    void setGender( int new_value ) { m_gender = new_value; }
    void setRace( int new_value ) { m_race = new_value; }
    void setWeight( int new_value ) { m_weight = new_value; }
    void setHeight( int new_value ) { m_height = new_value; }
    void setAlignment( int new_value ) { m_alignment = new_value; }
    void setStrength( int new_value ) { m_abilities[ CHARACTER_ABILITY_STRENGTH ] = new_value; }
    void setDexterity( int new_value ) { m_abilities[ CHARACTER_ABILITY_DEXTERITY ] = new_value; }
    void setConstitution( int new_value ) { m_abilities[ CHARACTER_ABILITY_CONSTITUTION ] = new_value; }
    void setIntelligence( int new_value ) { m_abilities[ CHARACTER_ABILITY_INTELLIGENCE ] = new_value; }
    void setWisdom( int new_value ) { m_abilities[ CHARACTER_ABILITY_WISDOM ] = new_value; }
    void setCharisma( int new_value ) { m_abilities[ CHARACTER_ABILITY_CHARISMA ] = new_value; }
    void setHitPoints( int new_value ) { m_hitPoints = new_value; }
    void setMaxHitPoints( int new_value ) { m_maxHitPoints = new_value; }
    void setManaPoints( int new_value ) { m_manaPoints = new_value; }
    void setMaxManaPoints( int new_value ) { m_maxManaPoints = new_value; }
    void setMovePoints( int new_value ) { m_movePoints = new_value; }
    void setMaxMovePoints( int new_value ) { m_maxMovePoints = new_value; }
    void setAttacks( float new_value ) { m_attacks = new_value; }
    void setHitRoll( int new_value ) { m_hitRoll = new_value; }
    void setArmor( int new_value ) { m_armor = new_value; }
    void setDamage( const Dice& new_value ) { m_damage = new_value; }
    void setDamageBonus( int new_value ) { m_damage.setBonus( new_value ); }
    void setGold( int new_value ) { m_gold = new_value; }
    void addAffect( int flag ) { Bit::set( &m_affects, flag ); }
    void addAffect2( int flag ) { Bit::set( &m_affects2, flag ); }
    void addImmunity( int flag ) { Bit::set( &m_immunities, flag ); }
    void addResistance( int flag ) { Bit::set( &m_resistances, flag ); }
    void addSusceptibility( int flag ) { Bit::set( &m_susceptibilities, flag ); }
    void removeAffect( int flag ) { Bit::remove( &m_affects, flag ); }
    void removeAffect2( int flag ) { Bit::remove( &m_affects2, flag ); }
    void removeImmunity( int flag ) { Bit::remove( &m_immunities, flag ); }
    void removeResistance( int flag ) { Bit::remove( &m_resistances, flag ); }
    void removeSusceptibility( int flag ) { Bit::remove( &m_susceptibilities, flag ); }
    void resetAffects() { m_affects = 0; }
    void resetAffects2() { m_affects2 = 0; }
    void resetImmunities() { m_immunities = 0; }
    void resetResistances() { m_resistances = 0; }
    void resetSusceptibilities() { m_susceptibilities = 0; }
    void setAffects( BitVector new_vector ) { m_affects = new_vector; }
    void setAffects2( BitVector new_vector ) { m_affects2 = new_vector; }
    void setImmunities( BitVector new_vector ) { m_immunities = new_vector; }
    void setResistances( BitVector new_vector ) { m_resistances = new_vector; }
    void setSusceptibilities( BitVector new_vector ) { m_susceptibilities = new_vector; }
    void setExperience( long new_value ) { m_experience = new_value; }
    void setPosition( long new_value ) { m_position = new_value; }
    void setSpellPower( int new_value ) { m_spellPower = new_value; }
    void setAbsorbDam( int new_value ) { m_absorbDam = new_value; }
    void setExtraFlags( BitVector new_value ) { m_extraFlags = new_value; }
    void addExtraFlags( int flag ) { Bit::set( &m_extraFlags, flag); }
    void removeExtraFlags( int flag ) { Bit::remove( &m_extraFlags, flag); }

  protected:
    QString m_shortDescription;
    QString m_longDescription;
    QString m_description;
    int m_gender;
    int m_race;
    int m_weight;
    int m_height;
    int m_alignment;
    int m_abilities[ CHARACTER_ABILITY_END ];
    int m_hitPoints;
    int m_maxHitPoints;
    int m_manaPoints;
    int m_maxManaPoints;
    int m_movePoints;
    int m_maxMovePoints;
    float m_attacks;
    BitVector m_extraFlags;
    int m_hitRoll;
    int m_armor;
    Dice m_damage; // "3d6+5"
    int m_gold;
    BitVector m_affects;
    BitVector m_affects2;
    BitVector m_immunities;
    BitVector m_resistances;
    BitVector m_susceptibilities;
    long m_experience;
    int m_position;
    int m_spellPower;
    int m_absorbDam;

    /*
    spellfail

    */
  };

} // namespace ts

#endif // TS_CHARACTERDATA_H

