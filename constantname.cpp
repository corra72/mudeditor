#include "constantname.h"
#include <QStringList>
#include "constants.h"

namespace ts
{

  static inline QString GetName_Private( const QString* vect_name, int vect_start, int vect_end, int vect_id )
  {
    return ( vect_id < vect_start || vect_id >= vect_end ) ? QString( "UNDEFINED" ) : vect_name[ vect_id ];
  }

  const QString strRaceName[] =
  {
    "Mezzosangue",
    "Umano",
    "Elfo della luna",
    "Nano",
    "Halfling",
    "Gnomo",
    "Rettile",
    "Mysterion",
    "Licantropo",
    "Draconiano",
    "Non morto",
    "Orco",
    "Insetto",
    "Aracnoide",
    "Dinosauro",
    "Pesce",
    "Uccello",
    "Gigante (generico)",
    "Carnivoro",
    "Parassita",
    "Melma",
    "Demone",
    "Serpente",
    "Erbivoro",
    "Albero",
    "Vegetale",
    "Elementale",
    "Planare",
    "Diavolo",
    "Fantasma",
    "Goblin",
    "Troll",
    "Uomo vegetale",
    "Mindflayer",
    "Scimmia",
    "Enfan",
    "Elfo oscuro",
    "Golem",
    "Skexie",
    "Troglodita",
    "Patryn",
    "Labrynthian",
    "Sartan",
    "Titano",
    "Puffo",
    "Canguro",
    "Cavallo",
    "Skaven",
    "Creatura Astrale",
    "Dio",
    "Gigante delle colline",
    "Gigante dei ghiacci",
    "Gigante del fuoco",
    "Gigante delle nuvole",
    "Gigante delle tempeste",
    "Gigante della pietra",
    "Drago rosso",
    "Drago nero",
    "Drago verde",
    "Drago bianco",
    "Drago blu",
    "Drago d'argento",
    "Drago d'oro",
    "Drago di bronzo",
    "Drago rame",
    "Drago d'ottone",
    "Vampiro",
    "Lich",
    "Wight",
    "Ghast",
    "Spettro",
    "Zombie",
    "Scheletro",
    "Ghoul",
    "Mezz'elfo",
    "Mezz'ogre",
    "Mezz'orco",
    "Mezzo gigante",
    "Uomo Lucertola",
    "Nano oscuro",
    "Gnomo abissale",
    "Gnoll",
    "Elfo dorato",
    "Elfo selvaggio",
    "Elfo dei mari",
//    "Brownie (non implementato)",
//    "Angelo (non implementato)",
//    "Arcangelo (non implementato)",
//    "Unicorno d'argento (non implementato)",
    "\n"
  };

  QString ConstantName::characterRace( int id )
  {
    return GetName_Private( strRaceName, 0, CHARACTER_RACE_END, id );
  }

  const QString strItemTypeName[] =
  {
    "NONE",
    "LIGHT",
    "SCROLL",
    "WAND",
    "STAFF",
    "WEAPON",
    "FIRE WEAPON (archi, balestre)",
    "MISSILE (dardi, frecce)",
    "TREASURE",
    "ARMOR",
    "POTION",
    "WORN (non usare)",
    "OTHER",
    "TRASH",
    "TRAP",
    "CONTAINER",
    "NOTE",
    "LIQUID CONTAINER",
    "KEY",
    "FOOD",
    "MONEY",
    "PEN",
    "BOAT",
    "AUDIO",
    "BOARD",
    "TREE",
    "ROCK",
//    "MUSIC INSTRUMENT (non implementato)",
//    "SPELL BOOK (non implementato)",
//    "SEAL CONTAINER (non implementato)",
    "\n"
  };

  QString ConstantName::itemType( int id )
  {
    return GetName_Private( strItemTypeName, 0, ITEM_TYPE_END, id );
  }

  const QString strItemWeaponDamageTypeName[] =
  {
    "smite",
    "stab",
    "whip",
    "slash",
    "smash",
    "cleave",
    "crush",
    "bludgeon",
    "claw",
    "bite",
    "sting",
    "pierce",
    "blast",
    "range weapon",
    "acid (non implementato)",
    "cold (non implementato)",
    "fire (non implementato)",
    "electricity (non implementato)",
    "energy (non implementato)",
    "\n"
  };

  QString ConstantName::itemWeaponDamageType( int id )
  {
    return GetName_Private( strItemWeaponDamageTypeName, 0, ITEM_WEAPON_DAMAGE_TYPE_END, id );
  }

  const QString strItemSealContainerType[] =
  {
    "Scegli il tipo (non implementato)",
    "divini",
    "fuoco",
    "aria",
    "acqua",
    "terra",
    "elementali",
    "tutti",
    "exp",
    "\n"
  };

  QString ConstantName::itemSealContainerType( int id )
  {
    return GetName_Private( strItemSealContainerType, 0, ITEM_SEAL_CONTAINER_END, id );
  }

  const QString strEquipmentPositionName[] =
  {
    "come luce",
    "sul dito destro",
    "sul dito sinistro",
    "intorno al collo (1)",
    "intorno al collo (2)",
    "sul corpo",
    "sulla testa",
    "sulle gambe",
    "ai piedi",
    "sulle mani",
    "sulle braccia",
    "come scudo",
    "intorno al corpo",
    "intorno alla vita",
    "sul polso destro",
    "sul polso sinistro",
    "come arma principale",
    "in mano",
    "sulle spalle",
    "all'orecchio destro",
    "all'orecchio sinistro",
    "davanti agli occhi",
    "\n"
  };

  QString ConstantName::equipmentPositionName( int id )
  {
    return GetName_Private( strEquipmentPositionName, 0, EQUIPMENT_POSITION_END, id );
  }

  const QString strRoomSectorName[] =
  {
    "INSIDE",
    "CITY",
    "FIELD",
    "FOREST",
    "HILLS",
    "MOUNTAINS",
    "WATER SWIM",
    "WATER NO-SWIM",
    "AIR",
    "UNDERWATER",
    "DESERT",
    "TREE",
    "DARK CITY",
    "TELEPORT",
    "\n"
  };

  QString ConstantName::roomSector( int id )
  {
    return GetName_Private( strRoomSectorName, 0, ROOM_SECTOR_END, id );
  }

  const QString strExitDirectionName[] =
  {
    "NORTH",
    "EAST",
    "SOUTH",
    "WEST",
    "UP",
    "DOWN",
    "\n"
  };

  QString ConstantName::exitDirection( int id )
  {
    return GetName_Private( strExitDirectionName, 0, EXIT_DIRECTION_END, id );
  }

  const QString strExitDirectionListName[] =
  {
    "$c0010North",
    "$c0011East",
    "$c0012South",
    "$c0013West",
    "$c0014Up",
    "$c0015Down"
    "\n"
  };

  QString ConstantName::exitDirectionListName( int id )
  {
    return GetName_Private( strExitDirectionListName, 0, EXIT_DIRECTION_END, id );
  }

  const QString strExitDirectionToName[] =
  {
    "nord",
    "est",
    "sud",
    "ovest",
    "l'alto",
    "il basso",
    "\n"
  };

  QString ConstantName::exitDirectionToName( int id )
  {
    return GetName_Private( strExitDirectionToName, 0, EXIT_DIRECTION_END, id );
  }

  const QString strExitDirectionFromName[] =
  {
    " nord",
    " est",
    " sud",
    " ovest",
    "ll'alto",
    "l basso"
    "\n"
  };

  QString ConstantName::exitDirectionFromName( int id )
  {
    return GetName_Private( strExitDirectionFromName, 0, EXIT_DIRECTION_END, id );
  }

  const QString strExitReverseDirectionName[] =
  {
    "south",
    "west",
    "north",
    "east",
    "down",
    "up",
    "\n"
  };

  QString ConstantName::exitReverseDirectionName( int id )
  {
    return GetName_Private( strExitReverseDirectionName, 0, EXIT_DIRECTION_END, id );
  }

  const QString strExitOpenCommandName[] =
  {
    "OPEN",
    "PULL",
    "TWIST",
    "TURN",
    "LIFT",
    "PUSH",
    "DIG",
    "CUT",
    "\n"
  };

  QString ConstantName::exitOpenCommand( int id )
  {
    return GetName_Private( strExitOpenCommandName, 0, EXIT_OPEN_COMMAND_END, id );
  }

  const QString strExitStatusName[] =
  {
    "APERTA",
    "CHIUSA",
    "CHIUSA A CHIAVE",
    "\n"
  };

  QString ConstantName::exitStatus( int id )
  {
    return GetName_Private( strExitStatusName, 0, EXIT_STATUS_END, id );
  }

  const QString strCharacterGenderName[] =
  {
    "INDEFINITO",
    "MASCHIO",
    "FEMMINA",
    "\n"
  };

  QString ConstantName::characterGender( int id )
  {
    return GetName_Private( strCharacterGenderName, 0, CHARACTER_GENDER_END, id );
  }

  const QString strMobTypeName[] =
  {
    "NEW",
    "MULTI-ATTACKS",
    "UNBASHABLE",
    "SOUND",
    "SIMPLE (non usare)",
    "DETAILED (non usare)",
    "\n"
  };

  QString ConstantName::mobType( int id )
  {
    return GetName_Private( strMobTypeName, 0, MOB_TYPE_END, id );
  }

  const QString strCharacterPositionName[] =
  {
    "DEAD",
    "MORTALLY-WOUNDED",
    "INCAPACITATED",
    "STUNNED",
    "SLEEPING",
    "RESTING",
    "SITTING",
    "FIGHTING",
    "STANDING",
    "MOUNTED",
    "\n"
  };

  QString ConstantName::characterPosition( int id )
  {
    return GetName_Private( strCharacterPositionName, 0, CHARACTER_POSITION_END, id );
  }

  const QString strCharacterAbilityName[] =
  {
    "FORZA",
    "DESTREZZA",
    "COSTITUZIONE",
    "INTELLIGENZA",
    "SAGGEZZA",
    "CARISMA",
    "\n"
  };

  QString ConstantName::characterAbility( int id )
  {
    return GetName_Private( strCharacterAbilityName, 0, CHARACTER_ABILITY_END, id );
  }

  const QString strCharacterClassName[] =
  {
    "MAGO",
    "CHIERICO",
    "GUERRIERO",
    "LADRO",
    "DRUIDO",
    "MONACO",
    "BARBARO",
    "STREGONE",
    "PALADINO",
    "RANGER",
    "PSIONICO",
 //   "ANTIPALADINO (non implementato)",
    "\n"
  };

  QString ConstantName::characterClass( int id )
  {
    return GetName_Private( strCharacterClassName, 0, CHARACTER_CLASS_END, id );
  }

  const QString strMobReactionVersusTypeName[] =
  {
    "UNKNOWN",
    "SESSO",
    "RAZZA",
    "RISERVATO (non usare)",
    "CLASSE",
    "ALLINEAMENTO <=",
    "ALLINEAMENTO >=",
    "VNUMBER",
    "\n"
  };

  QString ConstantName::mobReactionVersusType( int id )
  {
    return GetName_Private( strMobReactionVersusTypeName, 1, MOB_REACTION_END, id );
  }

  const QString strZoneRepopMoneName[] =
  {
    "NEVER",
    "IF_EMPTY",
    "ALWAYS",
    "\n"
  };

  QString ConstantName::zoneRepopMode( int id )
  {
    return GetName_Private( strZoneRepopMoneName, 0, ZONE_REPOP_MODE_END, id );
  }

} // namespace ts
