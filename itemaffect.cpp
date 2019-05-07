
#include "itemaffect.h"
#include "utils.h"
#include "constantname.h"
#include "handler.h"
#include "eleuconf.h"
#include <QTextStream>

namespace ts
{
    QString ItemAffect::dumpObject() const
    {
        QString sDump = "";
        sDump.sprintf("ItemAffect %s by %ld", qPrintable(Eleuconf::getAppliesName(m_type)), m_value);
        return sDump;
    }

    ItemAffect::ItemAffect()
            : m_type(ITEM_AFFECT_NONE),
            m_value(0)
    {
#if defined( TS_DEBUG_PANIC )
        qDebug("%s created (default).", qPrintable(dumpObject()));
#endif
    }

    ItemAffect::~ItemAffect()
    {
#if defined( TS_DEBUG_PANIC )
        qDebug("%s deleted.", qPrintable(dumpObject()));
#endif

    }

    ItemAffect::ItemAffect(const ItemAffect& ia)
            : m_type(ia.m_type),
            m_value(ia.m_value)
    {
#if defined( TS_DEBUG_PANIC )
        qDebug("%s created (by copy).", qPrintable(dumpObject()));
#endif
    }

    ItemAffect& ItemAffect::operator=(const ItemAffect& ia)
    {
        if (this != &ia) {
#if defined( TS_DEBUG_PANIC )
            qDebug("%s overwritten by %s.", qPrintable(dumpObject()), qPrintable(ia.dumpObject()));
#endif
            m_type = ia.m_type;
            m_value = ia.m_value;
        }
        return *this;
    }

    void ItemAffect::load(FILE* pFile, VNumber vnum)
    {
        int iTmp = Utils::readNumber(pFile, "(ItemAffectType)");
        if (iTmp <= ITEM_AFFECT_NONE && iTmp >= Eleuconf::countApplies()) {
            qWarning("Invalid ItemAffectType '%d' found in object vnumber %ld.", iTmp, vnum);
            return;
        }
        m_type = iTmp;
        m_value = Utils::readNumber(pFile, "(ItemAffectValue)");
        qDebug("Item #%ld -> %s loading completed.", vnum, qPrintable(dumpObject()));
    }

    void ItemAffect::save(QTextStream& stream)
    {
        if (isNull())
            return;
        stream << "A" << endl << flush;
        stream << m_type << " " << m_value << endl << flush;
        qDebug("%s saving completed.", qPrintable(dumpObject()));
    }

    QString ItemAffect::typeToString() const
    {
        return Eleuconf::getAppliesName(m_type);
    }

    QString ItemAffect::valueToString() const
    {
        QString sValue = "";

        switch (m_type) {
            case ITEM_AFFECT_RESISTANT:
            case ITEM_AFFECT_SUSCEPTIBLE:
            case ITEM_AFFECT_IMMUNE:
                sValue = Utils::bitvector2string(m_value, Eleuconf::getAllFlagsCaption(Eleuconf::resFlags));
                break;

            case ITEM_AFFECT_EAT_SPELL:
            case ITEM_AFFECT_WEAPON_SPELL:
                sValue = Handler::spellName(m_value);
                break;

            case ITEM_AFFECT_SPELL:
                sValue = Utils::bitvector2string(m_value, Eleuconf::getAllFlagsCaption(Eleuconf::mobAffectionsFlags));
                break;

            case ITEM_AFFECT_ALIGNMENT_SLAYER:
                sValue = Utils::bitvector2string(m_value, Eleuconf::getAllFlagsCaption(Eleuconf::trapFlags));
                break;

            case ITEM_AFFECT_RACE_SLAYER:
                sValue = ConstantName::characterRace(m_value);
                break;

            default:
                sValue = (m_value > 0 ? QString("+%1").arg(m_value) : QString::number(m_value));
        }

        return sValue;
    }


} // namespace ts

