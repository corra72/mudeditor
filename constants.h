
#ifndef TS_CONSTANTS_H
#define TS_CONSTANTS_H

#include <QStringList>

namespace ts
{
   /* Command */
  const int CMD_PULL          = 224;
  const int CMD_TWIST         = 371;
  const int CMD_TURN          = 372;
  const int CMD_LIFT          = 373;
  const int CMD_PUSH          = 374;
  const int CMD_DIG           = 495;
  const int CMD_CUT           = 496;
  /* Command End */

  /* Item Values */
  const int ITEM_VALUE_0   = 0;
  const int ITEM_VALUE_1   = 1;
  const int ITEM_VALUE_2   = 2;
  const int ITEM_VALUE_3   = 3;
  const int ITEM_VALUE_END = 4;
  /* Item Values End */

  /* Item Affects Position */
  const int ITEM_AFFECT_POSITION_0   = 0;
  const int ITEM_AFFECT_POSITION_1   = 1;
  const int ITEM_AFFECT_POSITION_2   = 2;
  const int ITEM_AFFECT_POSITION_3   = 3;
  const int ITEM_AFFECT_POSITION_4   = 4;
  const int ITEM_AFFECT_POSITION_END = 5;
  /* Item Affects Position End */

  /* Item Types */
  const int ITEM_TYPE_NONE              = 0;
  const int ITEM_TYPE_LIGHT             = 1;
  const int ITEM_TYPE_SCROLL            = 2;
  const int ITEM_TYPE_WAND              = 3;
  const int ITEM_TYPE_STAFF             = 4;
  const int ITEM_TYPE_WEAPON            = 5;
  const int ITEM_TYPE_FIREWEAPON        = 6;
  const int ITEM_TYPE_MISSILE           = 7;
  const int ITEM_TYPE_TREASURE          = 8;
  const int ITEM_TYPE_ARMOR             = 9;
  const int ITEM_TYPE_POTION            = 10;
  const int ITEM_TYPE_WORN              = 11;
  const int ITEM_TYPE_OTHER             = 12;
  const int ITEM_TYPE_TRASH             = 13;
  const int ITEM_TYPE_TRAP              = 14;
  const int ITEM_TYPE_CONTAINER         = 15;
  const int ITEM_TYPE_NOTE              = 16;
  const int ITEM_TYPE_LIQUID_CONTAINER  = 17;
  const int ITEM_TYPE_KEY               = 18;
  const int ITEM_TYPE_FOOD              = 19;
  const int ITEM_TYPE_MONEY             = 20;
  const int ITEM_TYPE_PEN               = 21;
  const int ITEM_TYPE_BOAT              = 22;
  const int ITEM_TYPE_AUDIO             = 23;
  const int ITEM_TYPE_BOARD             = 24;
  const int ITEM_TYPE_TREE              = 25;
  const int ITEM_TYPE_ROCK              = 26;
//  const int ITEM_TYPE_MUS_INSTRUMENT    = 27;
//  const int ITEM_TYPE_SPELL_BOOK        = 28;
//  const int ITEM_TYPE_SEAL_CONTAINER    = 29;
  const int ITEM_TYPE_END               = 27;
  /* Item Types End */

  /* Item Affects */
  const int ITEM_AFFECT_NONE              = 0;
  const int ITEM_AFFECT_STR               = 1;
  const int ITEM_AFFECT_DEX               = 2;
  const int ITEM_AFFECT_INT               = 3;
  const int ITEM_AFFECT_WIS               = 4;
  const int ITEM_AFFECT_CON               = 5;
  const int ITEM_AFFECT_CHR               = 6;
  const int ITEM_AFFECT_SEX               = 7;
  const int ITEM_AFFECT_LEVEL             = 8;
  const int ITEM_AFFECT_AGE               = 9;
  const int ITEM_AFFECT_CHAR_WEIGHT       = 10;
  const int ITEM_AFFECT_CHAR_HEIGHT       = 11;
  const int ITEM_AFFECT_MANA              = 12;
  const int ITEM_AFFECT_HIT               = 13;
  const int ITEM_AFFECT_MOVE              = 14;
  const int ITEM_AFFECT_GOLD              = 15;
  const int ITEM_AFFECT_EXP               = 16;
  const int ITEM_AFFECT_AC                = 17;
  const int ITEM_AFFECT_ARMOR             = 17;
  const int ITEM_AFFECT_HITROLL           = 18;
  const int ITEM_AFFECT_DAMROLL           = 19;
  const int ITEM_AFFECT_SAVING_PARA       = 20;
  const int ITEM_AFFECT_SAVING_ROD        = 21;
  const int ITEM_AFFECT_SAVING_PETRI      = 22;
  const int ITEM_AFFECT_SAVING_BREATH     = 23;
  const int ITEM_AFFECT_SAVING_SPELL      = 24;
  const int ITEM_AFFECT_SAVE_ALL          = 25;
  const int ITEM_AFFECT_RESISTANT         = 26;
  const int ITEM_AFFECT_SUSCEPTIBLE       = 27;
  const int ITEM_AFFECT_IMMUNE            = 28;
  const int ITEM_AFFECT_SPELL             = 29;
  const int ITEM_AFFECT_WEAPON_SPELL      = 30;
  const int ITEM_AFFECT_EAT_SPELL         = 31;
  const int ITEM_AFFECT_BACKSTAB          = 32;
  const int ITEM_AFFECT_KICK              = 33;
  const int ITEM_AFFECT_SNEAK             = 34;
  const int ITEM_AFFECT_HIDE              = 35;
  const int ITEM_AFFECT_BASH              = 36;
  const int ITEM_AFFECT_PICK              = 37;
  const int ITEM_AFFECT_STEAL             = 38;
  const int ITEM_AFFECT_TRACK             = 39;
  const int ITEM_AFFECT_HITNDAM           = 40;
  const int ITEM_AFFECT_SPELLFAIL         = 41;
  const int ITEM_AFFECT_ATTACKS           = 42;
  const int ITEM_AFFECT_HASTE             = 43;
  const int ITEM_AFFECT_SLOW              = 44;
  const int ITEM_AFFECT_BV2               = 45;
  const int ITEM_AFFECT_FIND_TRAPS        = 46;
  const int ITEM_AFFECT_RIDE              = 47;
  const int ITEM_AFFECT_RACE_SLAYER       = 48;
  const int ITEM_AFFECT_ALIGNMENT_SLAYER  = 49;
  const int ITEM_AFFECT_MANA_REGEN        = 50;
  const int ITEM_AFFECT_HIT_REGEN         = 51;
  const int ITEM_AFFECT_MOVE_REGEN        = 52;
  const int ITEM_AFFECT_MOD_THIRST        = 53;
  const int ITEM_AFFECT_MOD_HUNGER        = 54;
  const int ITEM_AFFECT_MOD_DRUNK         = 55;
  const int ITEM_AFFECT_T_STR             = 56;
  const int ITEM_AFFECT_T_INT             = 57;
  const int ITEM_AFFECT_T_DEX             = 58;
  const int ITEM_AFFECT_T_WIS             = 59;
  const int ITEM_AFFECT_T_CON             = 60;
  const int ITEM_AFFECT_T_CHR             = 61;
  const int ITEM_AFFECT_T_HPS             = 62;
  const int ITEM_AFFECT_T_MOVE            = 63;
  const int ITEM_AFFECT_T_MANA            = 64;
  const int ITEM_AFFECT_SKIP              = 65;
  /* Item Applies End */

  /* Item Wear Position */
  const int ITEM_WEAR_POSITION_TAKE         = 0;
  const int ITEM_WEAR_FINGER			    = 1;
  const int ITEM_WEAR_NECK					= 2;
  const int ITEM_WEAR_BODY					= 3;
  const int ITEM_WEAR_HEAD					= 4;
  const int ITEM_WEAR_LEGS					= 5;
  const int ITEM_WEAR_FEET					= 6;
  const int ITEM_WEAR_HANDS					= 7;
  const int ITEM_WEAR_ARMS					= 8;
  const int ITEM_WEAR_SHIELD				= 9;
  const int ITEM_WEAR_ABOUT					= 10;
  const int ITEM_WEAR_WAISTE				= 11;
  const int ITEM_WEAR_WRIST					= 12;
  const int ITEM_WIELD						= 13;
  const int ITEM_HOLD						= 14;
  const int ITEM_THROW						= 15;
  const int ITEM_LIGHT_SOURCE				= 16;//Non usato
  const int ITEM_WEAR_BACK					= 17;
  const int ITEM_WEAR_EAR					= 18;
  const int ITEM_WEAR_EYE					= 19;
  const int ITEM_WEAR_POSITION_END			= 20;
  /* Item Wear Position End */

  /* Weapon Damage Type */
  const int ITEM_WEAPON_DAMAGE_TYPE_END      = 19;
  /* Weapon Damage Type End */

  /* Item Seal Container Types */
  const int ITEM_SEAL_CONTAINER_NONE   = 0;
  const int ITEM_SEAL_CONTAINER_PQ     = 1;
  const int ITEM_SEAL_CONTAINER_FIRE   = 2;
  const int ITEM_SEAL_CONTAINER_AIR    = 3;
  const int ITEM_SEAL_CONTAINER_WATER  = 4;
  const int ITEM_SEAL_CONTAINER_EARTH  = 5;
  const int ITEM_SEAL_CONTAINER_ELEM   = 6;
  const int ITEM_SEAL_CONTAINER_ALL    = 7;
  const int ITEM_SEAL_CONTAINER_EXP    = 8;
  const int ITEM_SEAL_CONTAINER_END    = 9;
  /* Item Seal Container Types End */

  const int ITEM_LIQUID_TYPE_END = 16;
  // See Handler.h for Liquid Name...

  /* Equipment Position */
  const int EQUIPMENT_POSITION_END          = 22;
  /* Equipment Position End */

  /* Room Sector Types */
  const int ROOM_SECTOR_INSIDE        = 0;
  const int ROOM_SECTOR_CITY          = 1;
  const int ROOM_SECTOR_FIELD         = 2;
  const int ROOM_SECTOR_FOREST        = 3;
  const int ROOM_SECTOR_HILLS         = 4;
  const int ROOM_SECTOR_MOUNTAIN      = 5;
  const int ROOM_SECTOR_WATER_SWIM    = 6;
  const int ROOM_SECTOR_WATER_NO_SWIM = 7;
  const int ROOM_SECTOR_AIR           = 8;
  const int ROOM_SECTOR_UNDERWATER    = 9;
  const int ROOM_SECTOR_DESERT        = 10;
  const int ROOM_SECTOR_TREE          = 11;
  const int ROOM_SECTOR_DARKCITY      = 12;
  const int ROOM_SECTOR_TELEPORT      = 13;
  const int ROOM_SECTOR_END           = 14;
  /* Room Sector Types End */

  /* Room Flags */
  const int ROOM_FLAG_DEATH              = 1;
  const int ROOM_FLAG_TUNNEL             = 8;
  /* Room Flags End */

  /* Room Teleport Flags */
  const int ROOM_TELEPORT_FLAG_LOOK   = 0;
  const int ROOM_TELEPORT_FLAG_COUNT  = 1;
  const int ROOM_TELEPORT_FLAG_RANDOM = 2;
  const int ROOM_TELEPORT_FLAG_SPIN   = 3;
  const int ROOM_TELEPORT_FLAG_NOEXIT = 4;    // da aggiungere sul mud
  /* Room Teleport Flags End */

  /* Exit Direction */
  const int EXIT_DIRECTION_INVALID = -1;
  const int EXIT_DIRECTION_NORTH   = 0;
  const int EXIT_DIRECTION_EAST    = 1;
  const int EXIT_DIRECTION_SOUTH   = 2;
  const int EXIT_DIRECTION_WEST    = 3;
  const int EXIT_DIRECTION_UP      = 4;
  const int EXIT_DIRECTION_DOWN    = 5;
  const int EXIT_DIRECTION_END     = 6;
  /* Exit Direction End */

  /* Exit Reverse Direction */
  const int EXIT_REVERSE_DIRECTION_INVALID = -1;
  const int EXIT_REVERSE_DIRECTION_NORTH   = 2;
  const int EXIT_REVERSE_DIRECTION_EAST    = 3;
  const int EXIT_REVERSE_DIRECTION_SOUTH   = 0;
  const int EXIT_REVERSE_DIRECTION_WEST    = 1;
  const int EXIT_REVERSE_DIRECTION_UP      = 5;
  const int EXIT_REVERSE_DIRECTION_DOWN    = 4;
  const int EXIT_REVERSE_DIRECTION_END     = 6;
  /* Exit Reverse Direction End */

  /* Exit Flag */
  const int EXIT_FLAG_ISDOOR    = 0;
  const int EXIT_FLAG_CLOSED    = 1;
  const int EXIT_FLAG_LOCKED    = 2;
  const int EXIT_FLAG_SECRET    = 3;
  const int EXIT_FLAG_NOTBASH   = 4;
  const int EXIT_FLAG_PICKPROOF = 6;
  const int EXIT_FLAG_CLIMB     = 7;
  const int EXIT_FLAG_MALE		= 8;
  const int EXIT_FLAG_NOLOOKT   = 9;
  /* Exit Flag End */

  /* Exit Open Command */
  const int EXIT_OPEN_COMMAND_DEFAULT = 0;
  const int EXIT_OPEN_COMMAND_PULL    = 1;
  const int EXIT_OPEN_COMMAND_TWIST   = 2;
  const int EXIT_OPEN_COMMAND_TURN    = 3;
  const int EXIT_OPEN_COMMAND_LIFT    = 4;
  const int EXIT_OPEN_COMMAND_PUSH    = 5;
  const int EXIT_OPEN_COMMAND_DIG     = 6;
  const int EXIT_OPEN_COMMAND_CUT     = 7;
  const int EXIT_OPEN_COMMAND_END     = 8;
  /* Exit Open Command End */

  /* Exit Status */
  const int EXIT_STATUS_OPENED = 0;
  const int EXIT_STATUS_CLOSED = 1;
  const int EXIT_STATUS_LOCKED = 2;
  const int EXIT_STATUS_END    = 3;
  /* Exit Status End */

  /* Races */
  const int CHARACTER_RACE_HALFBREED       = 0;
  const int CHARACTER_RACE_HUMAN           = 1;
  const int CHARACTER_RACE_ELVEN           = 2;
  const int CHARACTER_RACE_DWARF           = 3;
  const int CHARACTER_RACE_HALFLING        = 4;
  const int CHARACTER_RACE_GNOME           = 5;
  const int CHARACTER_RACE_REPTILE         = 6;
  const int CHARACTER_RACE_SPECIAL         = 7;
  const int CHARACTER_RACE_LYCANTH         = 8;
  const int CHARACTER_RACE_DRAGON          = 9;
  const int CHARACTER_RACE_UNDEAD          = 10;
  const int CHARACTER_RACE_ORC             = 11;
  const int CHARACTER_RACE_INSECT          = 12;
  const int CHARACTER_RACE_ARACHNID        = 13;
  const int CHARACTER_RACE_DINOSAUR        = 14;
  const int CHARACTER_RACE_FISH            = 15;
  const int CHARACTER_RACE_BIRD            = 16;
  const int CHARACTER_RACE_GIANT           = 17;
  const int CHARACTER_RACE_PREDATOR        = 18;
  const int CHARACTER_RACE_PARASITE        = 19;
  const int CHARACTER_RACE_SLIME           = 20;
  const int CHARACTER_RACE_DEMON           = 21;
  const int CHARACTER_RACE_SNAKE           = 22;
  const int CHARACTER_RACE_HERBIV          = 23;
  const int CHARACTER_RACE_TREE            = 24;
  const int CHARACTER_RACE_VEGGIE          = 25;
  const int CHARACTER_RACE_ELEMENT         = 26;
  const int CHARACTER_RACE_PLANAR          = 27;
  const int CHARACTER_RACE_DEVIL           = 28;
  const int CHARACTER_RACE_GHOST           = 29;
  const int CHARACTER_RACE_GOBLIN          = 30;
  const int CHARACTER_RACE_TROLL           = 31;
  const int CHARACTER_RACE_VEGMAN          = 32;
  const int CHARACTER_RACE_MFLAYER         = 33;
  const int CHARACTER_RACE_PRIMATE         = 34;
  const int CHARACTER_RACE_ENFAN           = 35;
  const int CHARACTER_RACE_DROW            = 36;
  const int CHARACTER_RACE_GOLEM           = 37;
  const int CHARACTER_RACE_SKEXIE          = 38;
  const int CHARACTER_RACE_TROGMAN         = 39;
  const int CHARACTER_RACE_PATRYN          = 40;
  const int CHARACTER_RACE_LABRAT          = 41;
  const int CHARACTER_RACE_SARTAN          = 42;
  const int CHARACTER_RACE_TYTAN           = 43;
  const int CHARACTER_RACE_SMURF           = 44;
  const int CHARACTER_RACE_ROO             = 45;
  const int CHARACTER_RACE_HORSE           = 46;
  const int CHARACTER_RACE_SKAVEN          = 47;
  const int CHARACTER_RACE_ASTRAL          = 48;
  const int CHARACTER_RACE_GOD             = 49;
  const int CHARACTER_RACE_GIANT_HILL      = 50;
  const int CHARACTER_RACE_GIANT_FROST     = 51;
  const int CHARACTER_RACE_GIANT_FIRE      = 52;
  const int CHARACTER_RACE_GIANT_CLOUD     = 53;
  const int CHARACTER_RACE_GIANT_STORM     = 54;
  const int CHARACTER_RACE_GIANT_STONE     = 55;
  const int CHARACTER_RACE_DRAGON_RED      = 56;
  const int CHARACTER_RACE_DRAGON_BLACK    = 57;
  const int CHARACTER_RACE_DRAGON_GREEN    = 58;
  const int CHARACTER_RACE_DRAGON_WHITE    = 59;
  const int CHARACTER_RACE_DRAGON_BLUE     = 60;
  const int CHARACTER_RACE_DRAGON_SILVER   = 61;
  const int CHARACTER_RACE_DRAGON_GOLD     = 62;
  const int CHARACTER_RACE_DRAGON_BRONZE   = 63;
  const int CHARACTER_RACE_DRAGON_COPPER   = 64;
  const int CHARACTER_RACE_DRAGON_BRASS    = 65;
  const int CHARACTER_RACE_UNDEAD_VAMPIRE  = 66;
  const int CHARACTER_RACE_UNDEAD_LICH     = 67;
  const int CHARACTER_RACE_UNDEAD_WIGHT    = 68;
  const int CHARACTER_RACE_UNDEAD_GHAST    = 69;
  const int CHARACTER_RACE_UNDEAD_SPECTRE  = 70;
  const int CHARACTER_RACE_UNDEAD_ZOMBIE   = 71;
  const int CHARACTER_RACE_UNDEAD_SKELETON = 72;
  const int CHARACTER_RACE_UNDEAD_GHOUL    = 73;
  const int CHARACTER_RACE_HALF_ELVEN      = 74;
  const int CHARACTER_RACE_HALF_OGRE       = 75;
  const int CHARACTER_RACE_HALF_ORC        = 76;
  const int CHARACTER_RACE_HALF_GIANT      = 77;
  const int CHARACTER_RACE_LIZARDMAN       = 78;
  const int CHARACTER_RACE_DARK_DWARF      = 79;
  const int CHARACTER_RACE_DEEP_GNOME      = 80;
  const int CHARACTER_RACE_GNOLL           = 81;
  const int CHARACTER_RACE_GOLD_ELF        = 82;
  const int CHARACTER_RACE_WILD_ELF        = 83;
  const int CHARACTER_RACE_SEA_ELF         = 84;
//  const int CHARACTER_RACE_BROWNIE         = 85;
//  const int CHARACTER_RACE_ANGEL           = 86;
  const int CHARACTER_RACE_END             = 85;
  /* Races End */

  /* Character Genders */
  const int CHARACTER_GENDER_NEUTRAL       = 0;
  const int CHARACTER_GENDER_MALE          = 1;
  const int CHARACTER_GENDER_FEMALE        = 2;
  const int CHARACTER_GENDER_END           = 3;
  /* Character Genders End */

  /* Character Positions */
  const int CHARACTER_POSITION_DEAD             = 0;
  const int CHARACTER_POSITION_MORTALLY_WOUNDED = 1;
  const int CHARACTER_POSITION_INCAPACITATED    = 2;
  const int CHARACTER_POSITION_STUNNED          = 3;
  const int CHARACTER_POSITION_SLEEPING         = 4;
  const int CHARACTER_POSITION_RESTING          = 5;
  const int CHARACTER_POSITION_SITTING          = 6;
  const int CHARACTER_POSITION_FIGHTING         = 7;
  const int CHARACTER_POSITION_STANDING         = 8;
  const int CHARACTER_POSITION_MOUNTED          = 9;
  const int CHARACTER_POSITION_END              = 10;
  /* Character Positions End */

  /* Character Abilities */
  const int CHARACTER_ABILITY_STRENGTH      = 0;
  const int CHARACTER_ABILITY_DEXTERITY     = 1;
  const int CHARACTER_ABILITY_CONSTITUTION  = 2;
  const int CHARACTER_ABILITY_INTELLIGENCE  = 3;
  const int CHARACTER_ABILITY_WISDOM        = 4;
  const int CHARACTER_ABILITY_CHARISMA      = 5;
  const int CHARACTER_ABILITY_END           = 6;
  /* Character Abilities End */

  /* Character Classes */
  const int CHARACTER_CLASS_MAGE        = 0;
  const int CHARACTER_CLASS_CLERIC      = 1;
  const int CHARACTER_CLASS_WARRIOR     = 2;
  const int CHARACTER_CLASS_THIEF       = 3;
  const int CHARACTER_CLASS_DRUID       = 4;
  const int CHARACTER_CLASS_MONK        = 5;
  const int CHARACTER_CLASS_BARBARIAN   = 6;
  const int CHARACTER_CLASS_SORCERER    = 7;
  const int CHARACTER_CLASS_PALADIN     = 8;
  const int CHARACTER_CLASS_RANGER      = 9;
  const int CHARACTER_CLASS_PSIONIST    = 10;
//  const int CHARACTER_CLASS_ANTIPALADIN = 11;
  const int CHARACTER_CLASS_END         = 11;
  /* Character Classes End */

  /* Mob Types */
  const int MOB_TYPE_NEW           = 0;
  const int MOB_TYPE_MULTI_ATTACKS = 1;
  const int MOB_TYPE_UNBASHABLE    = 2;
  const int MOB_TYPE_SOUND         = 3;
  const int MOB_TYPE_SIMPLE        = 4;
  const int MOB_TYPE_DETAILED      = 5;
  const int MOB_TYPE_END           = 6;
  /* Mob Types End */

  /* Mob Acts */
  const int MOB_ACT_SCAVENGER       = 2;
  const int MOB_ACT_ISNPC           = 3;
  const int MOB_ACT_AGGRESSIVE      = 5;
  const int MOB_ACT_WIMPY           = 7;
  const int MOB_ACT_POLYSELF        = 14;
  const int MOB_ACT_META_AGGRESSIVE = 15;
  const int MOB_ACT_HUGE            = 18;
  /* Mob Acts End */


  /* Mob Reactions */
  const int MOB_REACTION_GENDER     = 1;
  const int MOB_REACTION_RACE       = 2;
  //const int MOB_REACTION_RESERVED = 3;
  const int MOB_REACTION_CLASS      = 4;
  const int MOB_REACTION_EVIL       = 5;
  const int MOB_REACTION_GOOD       = 6;
  const int MOB_REACTION_VNUMBER    = 7;
  const int MOB_REACTION_END        = 8;
  /* Mob Reactions End */

  /* Zone Repop Modes */
  const int ZONE_REPOP_MODE_NEVER     = 0;
  const int ZONE_REPOP_MODE_IF_EMPTY  = 1;
  const int ZONE_REPOP_MODE_ALWAYS    = 2;
  const int ZONE_REPOP_MODE_END       = 3;
  /* Zone Repop Modes End */

  /* Zone Flags */
  const int ZONE_FLAG_IF_EMPTY        = 0;
  const int ZONE_FLAG_ALWAYS          = 1;
  /* Zone Flags End */

  /* Affect2 */
  const int AFFECT2_NONE               = 0;
  const int AFFECT2_ANIMAL_INVIS       = 1;
  const int AFFECT2_HEAT_STUFF         = 2;
  const int AFFECT2_LOG_ME             = 3;
  const int AFFECT2_BERSERK            = 4;
  const int AFFECT2_PARRY              = 5;
  const int AFFECT2_CON_ORDER          = 6;
  const int AFFECT2_AFK                = 7;
  const int AFFECT2_PKILLER            = 8;
  const int AFFECT2_DANGER_SENSE       = 9;
//  const int AFFECT2_BLINK              = 7;
//  const int AFFECT2_FORMATION          = 8;
//  const int AFFECT2_SECONDLINE         = 9;
//  const int AFFECT2_THIRDLINE          = 10;
//  const int AFFECT2_DETECT_ORIGINAL    = 11;
//  const int AFFECT2_MANA_SHIELD        = 12;
//  const int AFFECT2_CRUSADE            = 13;
//  const int AFFECT2_PAIN_MIRROR        = 14;
//  const int AFFECT2_FROSTSHIELD        = 15;
//  const int AFFECT2_ACIDSHIELD         = 16;
//  const int AFFECT2_ELECTRICSHIELD     = 17;
//  const int AFFECT2_ENERGYSHIELD       = 18;
//  const int AFFECT2_FIRESHIELD         = 19;

  /* Weapon Alignment Slayer */
  const QStringList strWeaponAlignmentSlayerName = QStringList() << "NEUTRAL" << "GOOD" << "EVIL";
  const int WEAPON_ALIGNMENT_SLAYER_GOOD        = 0;
  const int WEAPON_ALIGNMENT_SLAYER_NEUTRAL     = 1;
  const int WEAPON_ALIGNMENT_SLAYER_EVIL        = 2;
  const int WEAPON_ALIGNMENT_SLAYER_END         = 3;
  /* Weapon Alignment Slayer End */

  /* Shop */
  const int SHOP_MAX_ITEMS    = 5;

  const int SHOP_TIME_OPEN_1  = 0;
  const int SHOP_TIME_CLOSE_1 = 1;
  const int SHOP_TIME_OPEN_2  = 2;
  const int SHOP_TIME_CLOSE_2 = 3;
  const int SHOP_TIME_END     = 4;

  const int SHOP_REACTION_TEMPER = 0;
  const int SHOP_REACTION_ATTACK = 1;
  const int SHOP_REACTION_END    = 2;

  const int SHOP_ANSWER_NO_SUCH_ITEM_1 = 0;
  const int SHOP_ANSWER_NO_SUCH_ITEM_2 = 1;
  const int SHOP_ANSWER_DO_NOT_BUY     = 2;
  const int SHOP_ANSWER_MISSING_CASH_1 = 3;
  const int SHOP_ANSWER_MISSING_CASH_2 = 4;
  const int SHOP_ANSWER_SELL           = 5;
  const int SHOP_ANSWER_BUY            = 6;
  const int SHOP_ANSWER_END            = 7;

  /* Shop End */

} // namespace ts

#endif // TS_CONSTANTS_H

