#include "report.h"
#include "constants.h"
#include "constantname.h"
#include "utils.h"
#include "eleuconf.h"

namespace ts
{
    QString Report::coins(const Area& area)
    {
        if (area.mobs().empty())
            return QString("L'area non contiene mobs.");

        QString sRet = "";
        QString sTmp = "";
        int tot_coins = 0;
        int tot_inits = 0;

        sRet.sprintf("Monete presenti sui Mobs dal VNumber %ld al VNumber %ld:\n\n",
                     area.bottomMobsVNumber(), area.topMobsVNumber());

        Area::mobs_const_iterator it = area.mobs().begin();
        while (it != area.mobs().end()) {
            if ((*it).gold() > 0) {
                tot_inits = area.countMobInits((*it).vnumber());
                sTmp = "";
                sTmp.sprintf("Il mob #%ld (%s) ha %d monete. [x%d]\n",
                             (*it).vnumber(), qPrintable((*it).shortDescription()), (*it).gold(), tot_inits);
                sRet += sTmp;
                tot_coins += ((*it).gold() * tot_inits);
            }
            ++it;
        }

        sRet += QString("\nIl numero totale delle monete presenti sui mobs e' %1.\n").arg(tot_coins);

        return sRet;
    }

    QString Report::treasures(const Area& area)
    {
        if (area.items().empty())
            return QString("L'area non contiene oggetti.");

        QString sRet = "";
        QString sTmp = "";
        int tot_coins = 0;
        int tot_inits = 0;

        sRet.sprintf("Tesori presenti negli oggetti dal VNumber %ld al VNumber %ld:\n\n",
                     area.bottomItemsVNumber(), area.topItemsVNumber());

        Area::items_const_iterator it = area.items().begin();
        while (it != area.items().end()) {
            if ((*it).itemType() == ITEM_TYPE_MONEY) {
                tot_inits = area.countItemInits((*it).vnumber());
                sTmp = "";
                sTmp.sprintf("L'oggetto #%ld (%s) e' un tesoro da %ld monete. [x%d]\n",
                             (*it).vnumber(), qPrintable((*it).shortDescription()),
                             (*it).value(ITEM_VALUE_0), tot_inits);
                sRet += sTmp;
                tot_coins += ((*it).value(ITEM_VALUE_0) * tot_inits);
            }

            ++it;
        }

        sRet += QString("\nIl numero totale delle monete presenti nei tesori e' %1.\n").arg(tot_coins);

        return sRet;
    }

    QString Report::exits(const Area& area)
    {
        if (area.rooms().empty())
            return QString("L'area non contiene stanze.");

        QString sRet = "";
        QString sTmp = "";
        bool has_one = false;

        Area::rooms_const_iterator it = area.rooms().begin();
        while (it != area.rooms().end()) {
            if (!(*it).exits().empty()) {
                Room::exits_const_iterator ite = (*it).exits().begin();
                while (ite != (*it).exits().end()) {
                    if (!(*ite).isFake() && !area.hasRoom((*ite).toRoom())) {
                        has_one = true;
                        sTmp = "";
                        sTmp.sprintf("L'uscita %s della stanza #%ld (%s) conduce alla locazione esterna #%ld.\n",
                                     qPrintable((*ite).name()), (*it).vnumber(),
                                     qPrintable((*it).name()), (*ite).toRoom());
                        sRet += sTmp;
                    }
                    ++ite;
                }
            }

            ++it;
        }

        if (!has_one)
            sRet = QString("Nessuna");

        return sRet;
    }

    QString Report::keys(const Area& area)
    {
        if (area.items().empty())
            return QString("L'area non contiene oggetti.");

        QString sRet = "";
        QString sTmp = "";
        bool has_one = false;

        sRet.sprintf("Chiavi presenti negli oggetti dal VNumber %ld al VNumber %ld:\n\n",
                     area.bottomItemsVNumber(), area.topItemsVNumber());


        Area::items_const_iterator it = area.items().begin();
        while (it != area.items().end()) {
            if ((*it).itemType() == ITEM_TYPE_KEY) {
                sTmp = "";
                sTmp.sprintf(" - #%ld \"%s\" (%s)\n", (*it).vnumber(),
                             qPrintable((*it).shortDescription()),
                             qPrintable(Utils::bitvector2string((*it).flags(), Eleuconf::getAllFlagsCaption(Eleuconf::objFlags)).toLower()));
                sRet += sTmp;
                has_one = true;
            }
            ++it;
        }

        if (!has_one)
            return QString("Nessuna.");

        sRet += QString("\n*****\n\nLista delle chiavi che aprono dei contenitori.\n\n");

        it = area.items().begin();
        has_one = false;

        while (it != area.items().end()) {
            if ((*it).itemType() == ITEM_TYPE_CONTAINER && (*it).value(ITEM_VALUE_2) > 0) {
                sTmp = "";
                has_one = true;
                if (area.hasItem((*it).value(ITEM_VALUE_2))) {
                    Item key_item = area.item((*it).value(ITEM_VALUE_2));
                    sTmp.sprintf("La chiave #%ld (%s) apre il contenitore #%ld (%s).\n",
                                 key_item.vnumber(), qPrintable(key_item.shortDescription()),
                                 (*it).vnumber(), qPrintable((*it).shortDescription()));
                } else
                    sTmp.sprintf("La chiave #%ld (sconosciuta) apre il contenitore #%ld (%s).\n",
                                 (*it).value(ITEM_VALUE_2), (*it).vnumber(), qPrintable((*it).shortDescription()));

                sRet += sTmp;
            }
            ++it;
        }

        if (!has_one)
            sRet += QString("Nessuna.\n");

        sRet += QString("\n*****\n\nLista delle chiavi che aprono le porte.\n\n");

        has_one = false;
        Area::rooms_const_iterator itr = area.rooms().begin();
        while (itr != area.rooms().end()) {
            if (!(*itr).exits().empty()) {
                Room::exits_const_iterator ite = (*itr).exits().begin();
                while (ite != (*itr).exits().end()) {
                    if (!(*ite).isFake() && (*ite).hasDoor() && (*ite).doorKey() > 0) {
                        has_one = true;
                        sTmp = "";
                        if (area.hasItem((*ite).doorKey())) {
                            Item key_item = area.item((*ite).doorKey());
                            sTmp.sprintf("La chiave #%ld (%s) apre l'uscita '%s' della locazione #%ld (%s).\n",
                                         key_item.vnumber(), qPrintable(key_item.shortDescription()),
                                         qPrintable((*ite).name()), (*itr).vnumber(), qPrintable((*itr).name()));
                        } else
                            sTmp.sprintf("La chiave #%ld (sconosciuta) apre l'uscita '%s' della locazione #%ld (%s).\n",
                                         (*ite).doorKey(), qPrintable((*ite).name()),
                                         (*itr).vnumber(), qPrintable((*itr).name()));

                        sRet += sTmp;
                    }
                    ++ite;
                }
            }

            ++itr;
        }

        if (!has_one)
            sRet += QString("Nessuna.\n");

        return sRet;
    }

    QString Report::deathRooms(const Area& area)
    {
        if (area.rooms().empty())
            return QString("L'area non contiene stanze.");

        QString sRet = "";
        QString sTmp = "";
        bool has_one = false;

        sRet.sprintf("Stanze DEATH presenti nelle locazioni dal VNumber %ld al VNumber %ld:\n\n",
                     area.bottomRoomsVNumber(), area.topRoomsVNumber());

        Area::rooms_const_iterator it = area.rooms().begin();
        while (it != area.rooms().end()) {
            if ((*it).isDeathRoom()) {
                has_one = true;
                sTmp = "";
                sTmp.sprintf(" - #%ld (%s).\n", (*it).vnumber(), qPrintable((*it).name()));
                sRet += sTmp;
            }
            ++it;
        }

        if (!has_one)
            sRet += QString("Nessuna.");

        return sRet;
    }

} // namespace ts
