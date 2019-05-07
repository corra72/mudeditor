#include "mob.h"
#include <QFile>
#include <QRegExp>
#include "constantname.h"
#include "random.h"
#include "dice.h"

#include <iostream>

using namespace std;

namespace ts
{

    void Mob::init()
    {
        m_name = defaultName();
        setShortDescription(defaultName());
        setLongDescription(defaultName() + QString(" is here."));
        m_roomSound = "";
        m_nearSound = "";
        m_acts = 0;
        m_mobType = MOB_TYPE_MULTI_ATTACKS;
        m_mobLevel = 1;
        m_hitPointsBonus = 0;
        m_xpBonus = 0;
        m_mobDefaultPosition = CHARACTER_POSITION_STANDING;
        m_mobBehaviours = "";
    }


    Mob::Mob()
            : CharacterData(ObjectData::Object_Mob)
    {
        init();
    }

    Mob::Mob(VNumber vnum)
            : CharacterData(ObjectData::Object_Mob, vnum)
    {
        init();
    }

    Mob::Mob(const Mob& m)
            : CharacterData((CharacterData)m)
    {
        m_roomSound = m.m_roomSound;
        m_nearSound = m.m_nearSound;
        m_acts = m.m_acts;
        m_mobType = m.m_mobType;
        m_mobLevel = m.m_mobLevel;
        m_hitPointsBonus = m.m_hitPointsBonus;
        m_xpBonus = m.m_xpBonus;
        m_mobDefaultPosition = m.m_mobDefaultPosition;
        m_mobBehaviours = m.m_mobBehaviours;
    }

    Mob::~Mob()
    {}

    Mob& Mob::operator=(const Mob& m)
    {
        if (this != &m) {
            CharacterData::operator=((CharacterData)m);
            m_roomSound = m.m_roomSound;
            m_nearSound = m.m_nearSound;
            m_acts = m.m_acts;
            m_mobType = m.m_mobType;
            m_mobLevel = m.m_mobLevel;
            m_hitPointsBonus = m.m_hitPointsBonus;
            m_xpBonus = m.m_xpBonus;
            m_mobDefaultPosition = m.m_mobDefaultPosition;
            m_mobBehaviours = m.m_mobBehaviours;
        }
        return *this;
    }

    void Mob::validate()
    {
        if (!hasAct(MOB_ACT_ISNPC))
            setAct(MOB_ACT_ISNPC);
        /*
        if( !hasAct( MOB_ACT_POLYSELF ) )
        {
          qWarning( "%s has flag %s set, removed.", dumpObject().toUtf8().data(),
            ConstantName::mobAct( MOB_ACT_POLYSELF ).toUtf8().data() );
          removeAct( MOB_ACT_POLYSELF );
        }
        */
    }

    static QChar GetMobTypeToSave(int id)
    {
        switch (id) {
            case MOB_TYPE_NEW:
                return 'N';
            case MOB_TYPE_MULTI_ATTACKS:
                return 'A';
            case MOB_TYPE_UNBASHABLE:
                return 'B';
            case MOB_TYPE_SOUND:
                return 'L';
            case MOB_TYPE_SIMPLE:
                return 'S';
            case MOB_TYPE_DETAILED:
                return 'D';
            default:
                return 'A';
        }
    }

    static int GetMobTypeFromLoad(char cid)
    {
        switch (cid) {
            case 'N':
                return MOB_TYPE_NEW;
            case 'A':
                return MOB_TYPE_MULTI_ATTACKS;
            case 'B':
                return MOB_TYPE_UNBASHABLE;
            case 'L':
                return MOB_TYPE_SOUND;
            case 'S':
                return MOB_TYPE_SIMPLE;
            case 'D':
                return MOB_TYPE_DETAILED;
            default:
                return MOB_TYPE_MULTI_ATTACKS;
        }
    }

    void Mob::load(FILE* pFile)
    {
        long lTmp;

        setName(Utils::readString(pFile, defaultName()));
        setShortDescription(Utils::readString(pFile, defaultName()));
        setLongDescription(Utils::readString(pFile, defaultName() + QString(" is here.")));
        setDescription(Utils::readString(pFile));

        QString sErrorMessage = "";
        sErrorMessage = dumpObject();

        lTmp = Utils::readNewFlags(pFile, &m_extraFlags);
        m_acts = lTmp;

        setAffects(Utils::readNumber(pFile, sErrorMessage + QString(" (MobAffects)")));
        setAlignment(Utils::readNumber(pFile, sErrorMessage + QString(" (MobAlignment)")));

        char cMobType;
        fscanf(pFile, " %c ", &cMobType);

        m_mobType = GetMobTypeFromLoad(cMobType);


        if (m_mobType == MOB_TYPE_SIMPLE || m_mobType == MOB_TYPE_MULTI_ATTACKS ||
                m_mobType == MOB_TYPE_NEW || m_mobType == MOB_TYPE_UNBASHABLE || m_mobType == MOB_TYPE_SOUND) {

            fscanf(pFile, " \n");

            if (m_mobType == MOB_TYPE_MULTI_ATTACKS || m_mobType == MOB_TYPE_UNBASHABLE || m_mobType == MOB_TYPE_SOUND) {
                setAttacks((float)Utils::readNumber(pFile, sErrorMessage + QString(" (Number of Attacks)")));
                setAffects2(Utils::readNumberInLine(pFile, sErrorMessage + QString(" (Affects2) ")));

                 if ((signed)m_affects2 == -1L)
                     m_affects2 = 0;

                /* Cambio gli shield dagli acts agli affect - rimosso per ora
                if (hasExtraFlags(6)) {
                    removeExtraFlags(6);
                    addAffect2(AFFECT2_FIRESHIELD);
                }

                if (hasExtraFlags(7)) {
                    removeExtraFlags(7);
                    addAffect2(AFFECT2_FROSTSHIELD);
                }

                if (hasExtraFlags(8)) {
                    removeExtraFlags(8);
                    addAffect2(AFFECT2_ELECTRICSHIELD);
                }

                if (hasExtraFlags(9)) {
                    removeExtraFlags(9);
                    addAffect2(AFFECT2_ENERGYSHIELD);
                }

                if (hasExtraFlags(10)) {
                    removeExtraFlags(10);
                    addAffect2(AFFECT2_ACIDSHIELD);
                } */
            }

            lTmp = Utils::readNumber(pFile, sErrorMessage + QString(" (MobLevel)"), 1);

            if (lTmp == -1) {
                setMobLevel(Utils::readNumber(pFile, sErrorMessage + QString(" (MobLevel)"), 1));
                setSpellPower(Utils::readNumber(pFile, sErrorMessage + QString(" (SpellPower)")));
                setAbsorbDam(Utils::readNumber(pFile, sErrorMessage + QString(" (AbsorbDam)")));
            } else
                setMobLevel(lTmp);

            setStrength((int)Utils::min(10 + Random::number(0, (int)Utils::max(1, mobLevel() / 5)), 18));
            setDexterity((int)Utils::min(10 + Random::number(0, (int)Utils::max(1, mobLevel() / 5)), 18));
            setConstitution((int)Utils::min(10 + Random::number(0, (int)Utils::max(1, mobLevel() / 5)), 18));
            setIntelligence((int)Utils::min(10 + Random::number(0, (int)Utils::max(1, mobLevel() / 5)), 18));
            setWisdom((int)Utils::min(10 + Random::number(0, (int)Utils::max(1, mobLevel() / 5)), 18));
            setCharisma((int)Utils::min(10 + Random::number(0, (int)Utils::max(1, mobLevel() / 5)), 18));

            setHitRoll((20 - Utils::readNumber(pFile, sErrorMessage + QString(" (HitRoll)"))));

            lTmp = Utils::readNumber(pFile, sErrorMessage + QString(" (Armor)"), 10);
            setArmor((lTmp *(m_mobType == MOB_TYPE_SIMPLE ? 1 : 10)));

            if (m_mobType == MOB_TYPE_SIMPLE) {
                setMaxHitPoints(Utils::readDice(pFile).roll());
            } else {
                setHitPointsBonus(Utils::readNumber(pFile, sErrorMessage + QString(" (MobHitPointsBonus)")));
                //setMaxHitPoints( ( Random::d8( mobLevel() ) + hitPointsBonus() ) );
            }
            setHitPoints(maxHitPoints());

            setDamage(Utils::readDice(pFile));
            fscanf(pFile, " \n");

            lTmp = Utils::readNumber(pFile, sErrorMessage + QString(" (Gold1)"), 0);
            bool read_also_race = (lTmp == -1);

            if (lTmp == -1)
                setGold(Utils::readNumber(pFile, sErrorMessage + QString(" (Gold2)"), 0));
            else
                setGold(lTmp);

            lTmp = Utils::readNumber(pFile, sErrorMessage + QString(" (Experience)"), 0);
            setXpBonus(lTmp);

            if (read_also_race) {
                setRace(Utils::readNumber(pFile, sErrorMessage + QString(" (Race)"), 0));
                /*
                Race( RaceID( fread_number( pFile ) ) );
                if( IsGiant() )
                abilities.str += number( 1, 4 );
                if( IsSmall() )
                abilities.str -= 1;
                */
            }

            setPosition(Utils::readNumber(pFile, sErrorMessage + QString(" (Position)"), CHARACTER_POSITION_STANDING));
            m_mobDefaultPosition = Utils::readNumber(pFile, sErrorMessage + QString(" (Position)"), CHARACTER_POSITION_STANDING);

            lTmp = Utils::readNumber(pFile, sErrorMessage + QString(" (Gender)"), CHARACTER_GENDER_NEUTRAL);
            if (lTmp < 3) {
                setGender(lTmp);
                resetResistances();
                resetImmunities();
                resetSusceptibilities();
            } else if (lTmp < 6) {
                setGender((lTmp - 3));
                setResistances(Utils::readNumber(pFile, sErrorMessage + QString(" (Resistances)")));
                setImmunities(Utils::readNumber(pFile, sErrorMessage + QString(" (Immunities)")));
                setSusceptibilities(Utils::readNumber(pFile, sErrorMessage + QString(" (Susceptibilities)")));
            } else {
                setGender(CHARACTER_GENDER_NEUTRAL);
                resetResistances();
                resetImmunities();
                resetSusceptibilities();
            }

            if (m_mobType == MOB_TYPE_SOUND) {
                setRoomSound(Utils::readString(pFile));
                setNearSound(Utils::readString(pFile));
            } else {
                setRoomSound("");
                setNearSound("");
            }
            /*
                   if( m_mobType == MOB_TYPE_UNBASHABLE )
                     setAct( MOB_ACT_HUGE );

                player.iClass = 0;

                TimeBirth() = time( 0 );
                TimePlayed() = 0;
                LastLogoff() = time(0);
                Weight( 200 );
                Height( 198 );

                for( int i = 0; i < 3; i++ )
                  GET_COND( this, i ) = -1;
                    */

        } else {
            /* The old monsters are down below here */

            fscanf(pFile, "\n");

            setStrength((Utils::readNumber(pFile, sErrorMessage + QString(" (Strength)"), 10)));
            setIntelligence((Utils::readNumber(pFile, sErrorMessage + QString(" (Intelligence)"), 10)));
            setWisdom((Utils::readNumber(pFile, sErrorMessage + QString(" (Wisdom)"), 10)));
            setDexterity((Utils::readNumber(pFile, sErrorMessage + QString(" (Dexterity)"), 10)));
            setConstitution((Utils::readNumber(pFile, sErrorMessage + QString(" (Constitution)"), 10)));
            setCharisma((int)Utils::min(10 + Random::number(0, (int)Utils::max(1, mobLevel() / 5)), 18));

            lTmp = Utils::readNumber(pFile, sErrorMessage + QString(" (MobHitPointsMin)"));
            setMaxHitPoints(Random::number(lTmp, Utils::readNumber(pFile, sErrorMessage + QString(" (MobHitPointsMax)"))));
            setHitPoints(maxHitPoints());

            lTmp = Utils::readNumber(pFile, sErrorMessage + QString(" (Armor)"), 10);
            setArmor((lTmp * 10));

            lTmp = Utils::readNumber(pFile, sErrorMessage + QString(" (MobManaPointsMin)"));
            setMaxManaPoints(Random::number(lTmp, Utils::readNumber(pFile, sErrorMessage + QString(" (MobManaPointsMax)"))));
            setManaPoints(maxManaPoints());

            lTmp = Utils::readNumber(pFile, sErrorMessage + QString(" (MobMovePointsMin)"));
            setMaxMovePoints(Random::number(lTmp, Utils::readNumber(pFile, sErrorMessage + QString(" (MobMovePointsMax)"))));
            setMovePoints(maxMovePoints());

            setGold(Utils::readNumber(pFile, sErrorMessage + QString(" (Gold)"), 0));
            setExperience(Utils::readNumber(pFile, sErrorMessage + QString(" (Experience)"), 1));
            setPosition(Utils::readNumber(pFile, sErrorMessage + QString(" (Position)"), CHARACTER_POSITION_STANDING));
            m_mobDefaultPosition = Utils::readNumber(pFile, sErrorMessage + QString(" (Position)"), CHARACTER_POSITION_STANDING);

            setGender(Utils::readNumber(pFile, sErrorMessage + QString(" (Gender)"), CHARACTER_GENDER_NEUTRAL));

            /*
            player.iClass = fread_number( pFile );

            fscanf(pFile, " %ld ", &lTmp);
            Level( WARRIOR_LEVEL_IND, lTmp );


            fscanf(pFile, " %ld ", &lTmp);
            TimeBirth() = time(0);
            TimePlayed() = 0;
            LastLogoff() = time(0);

            fscanf(pFile, " %ld ", &lTmp);
            Weight( lTmp );

            fscanf(pFile, " %ld \n", &lTmp);
            Height( lTmp );

            for( int i = 0; i < 3; i++ )
            {
            fscanf(pFile, " %ld ", &lTmp);
            GET_COND( this, i ) = lTmp;
            }
            fscanf(pFile, " \n ");

            for( int j = 0; j < 5; j++ )
            {
            fscanf(pFile, " %ld ", &lTmp);
            specials.apply_saving_throw[j] = lTmp;
            }

            fscanf(pFile, " \n ");
            */
            /* Set the damage as some standard 1d4 */
            /*
            points.damroll = 0;
            specials.damnodice = 1;
            specials.damsizedice = 6;
            */
            /* Calculate THAC0 as a formular of Level */

            //  points.hitroll = MAX( 1, Level( WARRIOR_LEVEL_IND ) - 3 );
        }

        readMobBehaviours(pFile);

        qDebug("%s loading completed.", dumpObject().toUtf8().data());
        /*
          tmpabilities = abilities;

          for( int i = 0; i < MAX_WEAR; i++ )
            equipment[ i ] = NULL;

          desc = NULL;

          SET_BIT( specials.act, ACT_ISNPC );

          generic = 0;
          commandp = 0;
          commandp2 = 0;
          waitp = 0;

          RemoveAct( ACT_SCRIPT );

          for( unsigned j = 0; j < scripts.size(); j++ )
          {
            if( scripts[ j ].iVNum == VNum () )
            {
              AddAct( ACT_SCRIPT );
              script = j;
              break;
            }
          }



          if( points.gold > Level( WARRIOR_LEVEL_IND ) * 1500 )
            mudlog( LOG_MOBILES, "%s has gold > level * 1500 (%d)", Description().c_str(), points.gold );

          SetRacialStuff();




          points.mana = getMaxManaPoints();
          points.move = getMaxMovePoints();

          return bc;
            */
    }

    void Mob::save(QTextStream& stream)
    {
        stream << "#" << m_vnumber << endl << flush;
        stream << m_name << "~" << endl << flush;
        stream << m_shortDescription << "~" << endl << flush;
        stream << m_longDescription << endl << "~" << endl << flush;
        stream << m_description << endl << "~" << endl << flush;

        Utils::writeNewFlags(stream, acts(), extraFlags());
        /*
        BitVector btTmp = acts();
        if( m_mobType == MOB_TYPE_UNBASHABLE )
          Bit::remove( &btTmp, MOB_ACT_HUGE );

        Utils::saveBitVector( stream, btTmp );
        stream << " " << flush;
        */
        Utils::saveBitVector(stream, m_affects);
        stream << " " << m_alignment << " " << flush;
        stream << GetMobTypeToSave(m_mobType) << flush;

        if (m_mobType == MOB_TYPE_MULTI_ATTACKS || m_mobType == MOB_TYPE_UNBASHABLE
                || m_mobType == MOB_TYPE_SOUND) {
            stream << " " << m_attacks << flush;
            if (affects2())
                stream << " " << affects2() << flush;
        }

        stream << endl << flush;
     //   stream << "-1 " << m_mobLevel << " " << m_spellPower << " " << m_absorbDam << " " << (20 - m_hitRoll) << " " << flush;    rimuovo spellpower e bsdam
        stream << m_mobLevel << " " << (20 - m_hitRoll) << " " << flush;
        stream << (m_armor / 10) << " " << m_hitPointsBonus << " " << m_damage.toString() << endl << flush;
        stream << "-1 " << flush;
        stream << m_gold << " " << m_xpBonus << " " << m_race << endl << flush;
        stream << m_position << " " << m_mobDefaultPosition << " " << flush;
        if (m_resistances > 0 || m_immunities > 0 || m_susceptibilities > 0) {
            stream << (m_gender + 3) << " " << flush;
            Utils::saveBitVector(stream, m_resistances);
            stream << " " << flush;
            Utils::saveBitVector(stream, m_immunities);
            stream << " " << flush;
            Utils::saveBitVector(stream, m_susceptibilities);
            stream << " " << endl << flush;
        } else {
            stream << m_gender << endl << flush;
        }

        if (m_mobType == MOB_TYPE_SOUND) {
            stream << m_roomSound << endl << "~" << endl << flush;
            stream << m_nearSound << endl << "~" << endl << flush;
        }

        stream << m_mobBehaviours;

        stream << flush;

        qDebug("%s saving completed.", dumpObject().toUtf8().data());
    }

    void Mob::generateExperience()
    {
        int aiBase[ 52 ] =
            {
                5,     10,    20,    35,   60,   90,   150,   225,    600,    900,
                1100,   1300,  1550,  1800,  2100,  2400,  2700,  3000,   3500,   4000,
                4500,   5000,  6000,  7000,  8000,  9000, 10000, 12000,  14000,  16000,
                20000,  22000, 22000, 22000, 22000, 32000, 32000, 32000,  32000,  32000,
                42000,  42000, 52000, 52000, 72000, 72000, 92000, 92000, 122000, 122000,
                150000, 200000
            };

        int aiHit[ 52 ] =
            {
                1,   1,   2,   3,   4,   5,   6,   8,  12,  14,
                15,  16,  17,  18,  19,  20,  23,  25,  28,  30,
                33,  35,  40,  45,  50,  55,  60,  70,  80,  90,
                100, 120, 120, 120, 120, 140, 140, 140, 140, 140,
                160, 160, 180, 180, 200, 200, 225, 225, 250, 250,
                275, 300
            };

        int aiFlags[ 52 ] =
            {
                10,     15,    20,    25,    30,    40,    75,   125,   175,   300,
                450,   700,   700,   950,  950,  1250,  1250,  1550,  1550,  2100,
                2100,  2600,  3000,  3500,  4000,  4500,  5000,  6000,  7000,  8000,
                10000, 12000, 12000, 12000, 12000, 14000, 14000, 14000, 14000, 14000,
                16000, 16000, 20000, 20000, 24000, 24000, 28000, 28000, 32000, 32000,
                36000, 40000
            };

        if (m_xpBonus > 100)
            qWarning("%s has an Experience Bonus > 100 (%ld).", dumpObject().toUtf8().data(), m_xpBonus);

        int iLevel = mobLevel();

        if (iLevel < 0) {
            setExperience(1);
            return;
        }

        if (iLevel > 51)
            iLevel = 51;

        int iExp = aiBase[ iLevel ] + ((aiHit[ iLevel ] / 2) * hitPoints()) + (aiFlags[ iLevel ] * m_xpBonus);

        setExperience(iExp);
    }

    void Mob::setXpBonus(long new_value)
    {
        m_xpBonus = new_value;
        if (m_mobType == MOB_TYPE_SIMPLE)
            setExperience(new_value);
        else if (new_value < 0)
            setExperience(-new_value);
        else
            generateExperience();

        if (hasAct(MOB_ACT_WIMPY))
            setExperience((m_experience - (m_experience / 10)));

        if (hasAct(MOB_ACT_AGGRESSIVE))
            setExperience((m_experience + (m_experience / 10)));

        if (hasAct(MOB_ACT_META_AGGRESSIVE) || !hasAct(MOB_ACT_WIMPY))
            setExperience((m_experience + (m_experience / 2)));

    }

    void Mob::generateAverageMaxHitPoints()
    {
        int iHp = static_cast<int>((m_mobLevel * 4.5) + m_hitPointsBonus);
        setMaxHitPoints(iHp);
    }


    void Mob::readMobBehaviours(FILE *fp)
    {
        char buf[256];
        long cur = ftell(fp);
        QString tmp;
        while (fscanf(fp, "%s", buf) && *buf=='E') {
            m_mobBehaviours += buf;
            m_mobBehaviours += '\n';

            if(buf[1] == '0') {
                tmp = Utils::readString(fp) + "~\n";
                tmp.remove(QRegExp("^(\n\r|\n)"));
                m_mobBehaviours += qPrintable(tmp);
            } else if(buf[1] == '1') {
                fscanf(fp, "%s", buf);
                m_mobBehaviours += buf;
                m_mobBehaviours += '\n';
            }

            fscanf(fp, "%s", buf);
            if(*buf !='R')
                return;
            m_mobBehaviours += buf;
            m_mobBehaviours += '\n';

            switch(buf[1]) {
                case '0':
                case '2':
                    tmp = Utils::readString(fp) + "~\n";
                    tmp.remove(QRegExp("^(\n\r|\n)"));
                    m_mobBehaviours += qPrintable(tmp);
                    break;
                case '1':
                case '3':
                case '4':
                case '5':
                case '6':
                    fscanf(fp, "%s", buf);
                    m_mobBehaviours += buf;
                    m_mobBehaviours += '\n';
                    break;
            }

            cur = ftell(fp);
        }
        fseek(fp, cur, SEEK_SET);
    }

} // namespace ts
