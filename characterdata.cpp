
#include "characterdata.h"

namespace ts
{
  void CharacterData::init()
  {
    m_shortDescription = "";
    m_longDescription = "";
    m_description = "";
    m_gender = CHARACTER_GENDER_NEUTRAL;
    m_race = 0;
    m_weight = 100;
    m_height = 100;
    m_alignment = 0;
    int i = 0;
    for( i = 0; i < CHARACTER_ABILITY_END; i++ )
      m_abilities[ i ] = 10;
    m_hitPoints = 10;
    m_maxHitPoints = 10;
    m_manaPoints = 100;
    m_maxManaPoints = 100;
    m_movePoints = 100;
    m_maxMovePoints = 100;
    m_attacks = 1.0;
    m_hitRoll = 0;
    m_armor = 10;
    m_damage = Dice( QString( "1d2+0" ) );
    m_gold = 0;
    resetAffects();
    resetAffects2();
    resetImmunities();
    resetResistances();
    resetSusceptibilities();
    m_experience = 1;
    m_position = CHARACTER_POSITION_STANDING;
    m_spellPower = 0;
    m_absorbDam = 0;
    m_extraFlags = 0;
  }

  CharacterData::CharacterData()
    : ObjectData( Object_Mob )
  {
    init();
  }


  CharacterData::CharacterData( Type object_type )
    : ObjectData( object_type )
  {
    init();
  }

  CharacterData::CharacterData( Type object_type, VNumber vnum, const QString& object_name  )
    : ObjectData( object_type, vnum, object_name )
  {
    init();
  }

  CharacterData::CharacterData( const CharacterData& cd )
    : ObjectData( (ObjectData)cd )
  {
    m_shortDescription = cd.m_shortDescription;
    m_longDescription = cd.m_longDescription;
    m_description = cd.m_description;
    m_gender = cd.m_gender;
    m_race = cd.m_race;
    m_weight = cd.m_weight;
    m_height = cd.m_height;
    m_alignment = cd.m_alignment;
    int i = 0;
    for( i = 0; i < CHARACTER_ABILITY_END; i++ )
      m_abilities[ i ] = cd.m_abilities[ i ];
    m_hitPoints = cd.m_hitPoints;
    m_maxHitPoints = cd.m_maxHitPoints;
    m_manaPoints = cd.m_manaPoints;
    m_maxManaPoints = cd.m_maxManaPoints;
    m_movePoints = cd.m_movePoints;
    m_maxMovePoints = cd.m_maxMovePoints;
    m_attacks = cd.m_attacks;
    m_hitRoll = cd.m_hitRoll;
    m_armor = cd.m_armor;
    m_damage = cd.m_damage;
    m_gold = cd.m_gold;
    m_affects = cd.m_affects;
    m_affects2 = cd.m_affects2;
    m_immunities = cd.m_immunities;
    m_resistances = cd.m_resistances;
    m_susceptibilities = cd.m_susceptibilities;
    m_experience = cd.m_experience;
    m_position = cd.m_position;
    m_spellPower = cd.m_spellPower;
    m_absorbDam = cd.m_absorbDam;
    m_extraFlags = cd.m_extraFlags;
  }

  CharacterData::~CharacterData()
  {}

  CharacterData& CharacterData::operator=( const CharacterData& cd )
  {
    if( this != &cd )
    {
      ObjectData::operator=( (ObjectData&)cd );
      m_shortDescription = cd.m_shortDescription;
      m_longDescription = cd.m_longDescription;
      m_description = cd.m_description;
      m_gender = cd.m_gender;
      m_race = cd.m_race;
      m_weight = cd.m_weight;
      m_height = cd.m_height;
      m_alignment = cd.m_alignment;
      int i = 0;
      for( i = 0; i < CHARACTER_ABILITY_END; i++ )
        m_abilities[ i ] = cd.m_abilities[ i ];
      m_hitPoints = cd.m_hitPoints;
      m_maxHitPoints = cd.m_maxHitPoints;
      m_manaPoints = cd.m_manaPoints;
      m_maxManaPoints = cd.m_maxManaPoints;
      m_movePoints = cd.m_movePoints;
      m_maxMovePoints = cd.m_maxMovePoints;
      m_attacks = cd.m_attacks;
      m_hitRoll = cd.m_hitRoll;
      m_armor = cd.m_armor;
      m_damage = cd.m_damage;
      m_gold = cd.m_gold;
      m_affects = cd.m_affects;
      m_affects2 = cd.m_affects2;
      m_immunities = cd.m_immunities;
      m_resistances = cd.m_resistances;
      m_susceptibilities = cd.m_susceptibilities;
      m_experience = cd.m_experience;
      m_position = cd.m_position;
      m_spellPower = cd.m_spellPower;
      m_absorbDam = cd.m_absorbDam;
      m_extraFlags = cd.m_extraFlags;
    }
    return *this;
  }

  void CharacterData::validate()
  {}

} // namespace ts

