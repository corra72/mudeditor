#ifndef TS_ELEUCONF_H
#define TS_ELEUCONF_H

#include <QFile>
#include <QString>
#include <QDataStream>
#include <QTextStream>
#include <QStringList>
#include <QList>


class Applies
{
    public:
        Applies( QString n, QString d ) { name = n; desc = d; } ;
        QString  name;
        QString  desc;
};


class Aspells
{
    public:
        Aspells( QString n, QString f, int nm, int lev) { name = n; flags = f; num = nm; level = lev;};
        QString name;
        int num;
        QString  flags;
        int level;
};


class Flags
{
    public:
        Flags( QString n, QString c, QString h, bool e, int f, int b ) { name = n; caption = c; hint = h; findex = f; bindex = b; enabled = e; };
        int findex;
        int bindex;
        bool enabled;
        QString name;
        QString caption;
        QString hint;
};

#if 0
class One
{
    public:
        One( QString name, QList<One> one );
        QString name;
};


class LGrid
{
    public:
        LGrid();
        QString name;
        int drunk;
        int sazi;
        int sete;
};


class Space
{
    public:
        Space( QString name, QList<Space> space );
        int val;
        QString name;
};


class MTypes
{
    public:
        MTypes();
        QString type;
        QString name;
};


class OCmds
{
    public:
        OCmds();
        QString name;
        int cmd;
};


class TdT
{
    public:
        TdT();
        QString name;
        int val;
};


class WGrid
{
    public:
        WGrid();
        QString name;
        QString type;
        QString msg;
        QString ita;
};
#endif

class Eleuconf
{
    public:

        static void readAll();
        static void readApplies();
        static void readAspells();

        static QString getAppliesName(int index);
        static QString getAppliesDesc(int index);
        static int countApplies();

        static QString getAspellsName(int index);
        static int getAspellsNum(int index);
        static QString getAspellsFlags(int index);
        static int getAspellsLevel(int index);
        static int countAspells();

        static void readFlags(const QString &name, QList<Flags> *flag);
        static int getFlagsFindex(int index, QList<Flags> flag);
        static int getFlagsBindex(int index, QList<Flags> flag);
        static bool getFlagsEnabled(int index, QList<Flags> flag);
        static QString getFlagsName(int index, QList<Flags> flag);
        static QString getFlagsCaption(int index, QList<Flags> flag);
        static QString getFlagsHint(int index, QList<Flags> flag);
        static QStringList getAllFlagsCaption(QList<Flags> flag, int type = 0);
#if 0
        static QString getOneName(int index, struct One* one);
        static int countOne(struct One* one);

        static QString getLGridName(int index);
        static int getLGridDrunk(int index);
        static int getLGridSazi(int index);
        static int getLGridSete(int index);
        static int countLGrid();

        static int getSpaceVal(int index, struct Space* space);
        static QString getSpaceName(int index, struct Space* space);

        static QString getMTypesType(int index);
        static QString getMTypesName(int index);
        static int countMTypes();

        static QString getOCmdsName(int index);
        static int getOCmdsCmd(int index);
        static int countOcmds();

        static QString getTdTName(int index);
        static int getTdTVal(int index);
        static int countTdT();

        static QString getWGridName(int index);
        static QString getWGridType(int index);
        static QString getWGridMsg(int index);
        static QString getWGridIta(int index);
        static int countWGrid();
#endif



        static QList<Applies> appl;

        static QList<Aspells> aspl;

        static QList<Flags> mobActionsFlags, mobAffectionsFlags, roomFlags, teleportFlags, containerFlags, objFlags, exitFlags, resFlags, objWear, trapFlags, zoneFlags, drinkContainerFlags;
        private:
#if 0
        static QList<Flags> foodFlags, itemAffectFlags, keyFlags, mobActionsFlags, zoneFlags;

        static QList<One> eqPos, itemTypes, mobClasses, mobPos, mobRaces, nShopMessages, roomTypes, shopMessages;

        static QList<LGrid> lgrid;

        static QList<Space> mobAlign;

        static QList<MTypes> mtypes;

        static QList<OCmds> ocmds;

        static QList<TdT> tdt;

        static QList<WGrid> wgrid;
#endif
    protected:
        Eleuconf();
        ~Eleuconf();

};

#endif /* TS_ELEUCONF_H */
