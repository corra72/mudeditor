
#ifndef TS_MOB_H
#define TS_MOB_H

#include <QTextStream>
#include <stdio.h>
#include "characterdata.h"



namespace ts
{

  class Mob : public CharacterData
  {
  public:
    void init();
    void validate();

    Mob();
    Mob( VNumber );
    Mob( const Mob& );

    virtual ~Mob();

    Mob& operator=( const Mob& );

    const QString& roomSound() const { return m_roomSound; }
    const QString& nearSound() const { return m_nearSound; }
    BitVector acts() const { return m_acts; }
    bool hasAct( int flag ) const { return Bit::has( m_acts, flag ); }
    int mobType() const { return m_mobType; }
    int mobLevel() const { return m_mobLevel; }
    int hitPointsBonus() const { return m_hitPointsBonus; }
    long xpBonus() const { return m_xpBonus; }
    int mobDefaultPosition() const { return m_mobDefaultPosition; }

    void setRoomSound( const QString& txt ) { m_roomSound = Utils::removeTilde( txt.trimmed() ); }
    void setNearSound( const QString& txt ) { m_nearSound = Utils::removeTilde( txt.trimmed() ); }
    void setAct( int flag ) { Bit::set( &m_acts, flag ); }
    void setActs( BitVector new_value ) { m_acts = new_value; }
    void removeAct( int flag ) { Bit::remove( &m_acts, flag ); }
    void setMobType( int new_value ) { m_mobType = new_value; }
    void setMobLevel( int new_value ) { m_mobLevel = new_value; }
    void setHitPointsBonus( int new_value ) { m_hitPointsBonus = new_value; generateAverageMaxHitPoints(); }
    void setXpBonus( long );
    void setMobDefaultPosition( int new_value ) { m_mobDefaultPosition = new_value; }

    void load( FILE* );
    void save( QTextStream& );

    void readMobBehaviours(FILE *fp);

  private:
    void generateExperience();
    void generateAverageMaxHitPoints();

  protected:
    QString m_roomSound;
    QString m_nearSound;
    BitVector m_acts;
    int m_mobType;
    int m_mobLevel;
    int m_hitPointsBonus;
    long m_xpBonus;
    int m_mobDefaultPosition;
    QString m_mobBehaviours;
  };

} // namespace ts

#endif // TS_MOB_H


