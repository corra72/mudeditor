
#include "eleuconf.h"
#include <iostream>

using namespace std;

QList<Applies> Eleuconf::appl;
QList<Aspells> Eleuconf::aspl;
QList<Flags> Eleuconf::mobActionsFlags;
QList<Flags> Eleuconf::mobAffectionsFlags;
QList<Flags> Eleuconf::roomFlags;
QList<Flags> Eleuconf::teleportFlags;
QList<Flags> Eleuconf::containerFlags;
QList<Flags> Eleuconf::objFlags;
QList<Flags> Eleuconf::exitFlags;
QList<Flags> Eleuconf::resFlags;
QList<Flags> Eleuconf::objWear;
QList<Flags> Eleuconf::trapFlags;
QList<Flags> Eleuconf::zoneFlags;
QList<Flags> Eleuconf::drinkContainerFlags;

Eleuconf::Eleuconf()
{
}

Eleuconf::~Eleuconf()
{
}

void Eleuconf::readAll()
{
    readApplies();
    readAspells();
    readFlags(":/conf/mobactions.ini", &mobActionsFlags);
    readFlags(":/conf/mobaffections.ini", &mobAffectionsFlags);
    readFlags(":/conf/roomflags.ini", &roomFlags);
    readFlags(":/conf/teleportflags.ini", &teleportFlags);
	readFlags(":/conf/containerflags.ini", &containerFlags);
	readFlags(":/conf/objflags.ini", &objFlags);
	readFlags(":/conf/exitflags.ini", &exitFlags);
	readFlags(":/conf/resistanceflags.ini", &resFlags);
	readFlags(":/conf/objwear.ini", &objWear);
	readFlags(":/conf/trapflags.ini", &trapFlags);
	readFlags(":/conf/zoneflags.ini", &zoneFlags);
    readFlags(":/conf/drinkcontainerflags.ini", &drinkContainerFlags);
     /*
    readFlags("conf/ZoneFlags.ini", zoneFlags);

    readOne("/root/Eleutheria/Muds/Ts2/eqpos.ini", eqpos);
    readFlags("/root/Eleutheria/Muds/Ts2/ItemAffections.ini", iaffect);
    readOne("/root/Eleutheria/Muds/Ts2/itypes.ini", itypes);
    readLGrid();
    readSpace("/root/Eleutheria/Muds/Ts2/malign.ini", malign);
    readOne("/root/Eleutheria/Muds/Ts2/mclasses.ini", mclasses);
    readOne("/root/Eleutheria/Muds/Ts2/mpos.ini", mpos);
    readOne("/root/Eleutheria/Muds/Ts2/Mraces.ini", mraces);
    readMTypes();
    readOne("/root/Eleutheria/Muds/Ts2/nshpmsgs.ini", nshpmsgs);
    readOCmds();

    readOne("/root/Eleutheria/Muds/Ts2/rtypes.ini", rtypes);
    readOne("/root/Eleutheria/Muds/Ts2/shpmsgs.ini", shpmsgs);
    readTdT();
    readFlags("/root/Eleutheria/Muds/Ts2/TrapFlags.ini", trapflags);
    readWGrid();

    */
}

void Eleuconf::readApplies()
{
    QString line;

	QFile fl(":/conf/applies.ini");
    if(!fl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("file applies.ini non aperto");
        return;
    } else {
        qDebug("file applies.ini aperto");

        QTextStream in(&fl);
		in.setCodec("UTF-8");

        appl.clear();
        Applies tmp( QString("none"), QString("none"));
        appl.append( tmp );

        line = in.readLine();
        if(!line.isEmpty()) {

            QStringList list = line.split("|");
            Applies tmp( list.at(0), list.at(1) );
            appl.append(tmp);
        }
        while(!in.atEnd())
        {
            line = in.readLine();
            if(line.isEmpty())
                continue;
            QStringList list = line.split("|");

            Applies tmp( list.at(0), list.at(1) );
            appl.append(tmp);
        }
        fl.close();
    }
}

QString Eleuconf::getAppliesName(int index)
{
    int i;
    for (i = 0; i < appl.size(); ++i) {
        if(i == index)
            return appl.at(i).name;
    }
    return NULL;
}

QString Eleuconf::getAppliesDesc(int index)
{
    int i;
    for (i = 0; i < appl.size(); ++i) {
        if(i == index)
            return appl.at(i).desc;
    }
    return NULL;
}

int Eleuconf::countApplies()
{
    return appl.size();
}

void Eleuconf::readAspells()
{
    QString line;

    bool ok;

	QFile fl(":/conf/aspells.ini");
    if(!fl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("file aspells.ini non aperto");
        return;
    } else {
        qDebug("file aspells.ini aperto");

        QTextStream in(&fl);
		in.setCodec("UTF-8");

        aspl.clear();

        line = in.readLine();
        if(!line.isEmpty()) {
            QStringList list = line.split("|");
            Aspells tmp(list.at(0), list.at(2), list.at(1).toShort(&ok, 10), list.at(3).toShort(&ok, 10));
            aspl.append(tmp);
        }

        while(!in.atEnd())
        {
            line = in.readLine();
            if(line.isEmpty())
                continue;
            QStringList list = line.split("|");

            Aspells tmp(list.at(0), list.at(2), list.at(1).toShort(&ok, 10), list.at(3).toShort(&ok, 10));
            aspl.append(tmp);
        }

        fl.close();
    }
}

QString Eleuconf::getAspellsName(int index)
{
    int i;
    for (i = 0; i < aspl.size(); ++i) {
        if(i == index)
            return aspl.at(i).name;
    }
    return NULL;
}

int Eleuconf::getAspellsNum(int index)
{
    int i;
    for (i = 0; i < aspl.size(); ++i) {
        if(i == index)
            return aspl.at(i).num;
    }
    return -1;
}

QString Eleuconf::getAspellsFlags(int index)
{
    int i;
    for (i = 0; i < aspl.size(); ++i) {
        if(i == index)
            return aspl.at(i).flags;
    }
    return NULL;
}

int Eleuconf::getAspellsLevel(int index)
{
    int i;
    for (i = 0; i < aspl.size(); ++i) {
        if(i == index)
            return aspl.at(i).level;
    }
    return -1;
}

int Eleuconf::countAspells()
{
    return aspl.size();
}

void Eleuconf::readFlags(const QString &name, QList<Flags> *flag)
{
    QString line;
    bool ok;

    QFile fl(name);
    if(!fl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("file %s non aperto", qPrintable(name));
        return;
    } else {
        qDebug("file %s aperto", qPrintable(name));

        QTextStream in(&fl);
		in.setCodec("UTF-8");

        flag->clear();

        line = in.readLine();
        if(!line.isEmpty()) {
            QStringList list = line.split("|");
            Flags tmp(list.at(3), list.at(4), list.at(5), list.at(2).toShort(&ok, 10) ? true :  false, list.at(0).toShort(&ok, 10), list.at(1).toShort(&ok, 10));
            flag->append(tmp);
        }

        while(!in.atEnd())
        {
            line = in.readLine();
            if(line.isEmpty())
                continue;

            QStringList list = line.split("|");

            Flags tmp(list.at(3), list.at(4), list.at(5), list.at(2).toShort(&ok, 10) ? true :  false, list.at(0).toShort(&ok, 10), list.at(1).toShort(&ok, 10));
            flag->append(tmp);
        }
        fl.close();
    }
}

int Eleuconf::getFlagsFindex(int index, QList<Flags> flag)
{
    int i;
    for (i = 0; i < flag.size(); ++i) {
        if(i == index)
            return flag.at(i).findex;
    }
    return 0;
}

int Eleuconf::getFlagsBindex(int index, QList<Flags> flag)
{
    int i;
    for (i = 0; i < flag.size(); ++i) {
        if(i == index)
            return flag.at(i).bindex;
    }
    return 0;
}

bool Eleuconf::getFlagsEnabled(int index, QList<Flags> flag)
{
    int i;
    for (i = 0; i < flag.size(); ++i) {
        if(i == index)
            return flag.at(i).enabled;
    }
    return false;
}

QString Eleuconf::getFlagsName(int index, QList<Flags> flag)
{
    int i;
    for (i = 0; i < flag.size(); ++i) {
        if(i == index)
            return flag.at(i).name;
    }
    return NULL;
}

QString Eleuconf::getFlagsCaption(int index, QList<Flags> flag)
{
    int i;
    for (i = 0; i < flag.size(); ++i) {
        if(i == index)
            return flag.at(i).caption;
    }
    return NULL;
}

QString Eleuconf::getFlagsHint(int index, QList<Flags> flag)
{
    int i;
    for (i = 0; i < flag.size(); ++i) {
        if(i == index)
            return flag.at(i).hint;
    }
    return NULL;
}

QStringList Eleuconf::getAllFlagsCaption(QList<Flags> flag, int type)
{
    QStringList lst;
    int i;
    for (i = 0; i < flag.size(); ++i) {
        if(flag.at(i).findex == type)
            lst << flag.at(i).caption;
    }

    return lst;
}
#if 0

int Eleuconf::countOne(struct One* one)
{
    int i;
    struct One* tmp = one;
    for(i = 0; tmp; i++)
        tmp = tmp->next;
    return i;
}

void Eleuconf::readOne(QString name, struct One* one)
{
    struct One* tmps;

    QFile fl(name);
    if(!fl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout<<"file "<<name.toUtf8().data()<<" non aperto"<<endl;
        return;
    } else {
        cout<<"file "<<name.toUtf8().data()<<" aperto"<<endl;

        one = new One;
        tmps = one;

        QTextStream in(&fl);


        tmps->name = in.readLine();

        while(!in.atEnd())
        {
            tmps->next = new One;
            tmps = tmps->next;
            tmps->next = NULL;

            tmps->name = in.readLine();
        }
        fl.close();
    }
}

QString Eleuconf::getOneName(int index, struct One* one)
{
    int i;
    struct One* tmp = one;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->name;
        tmp = tmp->next;
    }
    return NULL;
}

void Eleuconf::readLGrid()
{
    QString line;
    struct LGrid* tmps;
    bool ok;

    QFile fl("/root/Eleutheria/Muds/Ts2/LGrid.ini");
    if(!fl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout<<"file LGrid.ini non aperto"<<endl;
        return;
    } else {
        cout<<"file LGrid.ini aperto"<<endl;

        lgrid = new LGrid;
        tmps = lgrid;

        QTextStream in(&fl);

        line = in.readLine();

        while(!in.atEnd())
        {
            line = in.readLine();

            if(line.isEmpty())
                continue;
            QStringList list = line.split("|");

            tmps->name = list.at(0);
            tmps->drunk = list.at(1).toShort(&ok, 10);
            tmps->sazi = list.at(2).toShort(&ok, 10);
            tmps->sete = list.at(3).toShort(&ok, 10);

            tmps->next = new LGrid;
            tmps = tmps->next;
            tmps->next = NULL;
        }
        fl.close();
    }
}

QString Eleuconf::getLGridName( int index)
{
    int i;
    struct LGrid* tmp = lgrid;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->name;
        tmp = tmp->next;
    }
    return NULL;
}

int Eleuconf::getLGridDrunk( int index)
{
    int i;
    struct LGrid* tmp = lgrid;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->drunk;
        tmp = tmp->next;
    }
    return 0;
}

int Eleuconf::getLGridSazi( int index)
{
    int i;
    struct LGrid* tmp = lgrid;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->sazi;
        tmp = tmp->next;
    }
    return 0;
}

int Eleuconf::getLGridSete( int index)
{
    int i;
    struct LGrid* tmp = lgrid;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->sete;
        tmp = tmp->next;
    }
    return 0;
}

int Eleuconf::countLGrid()
{
    int i;
    struct LGrid* tmp = lgrid;
    for(i = 0; tmp; i++) {
        tmp = tmp->next;
    }
    return i;
}

void Eleuconf::readSpace(QString name, struct Space* space)
{
    struct Space* tmps;
    QString line, strtmp;
    bool ok;

    QFile fl(name);
    if(!fl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout<<"file "<<name.toUtf8().data()<<" non aperto"<<endl;
        return;
    } else {
        cout<<"file "<<name.toUtf8().data()<<" aperto"<<endl;

        space = new Space;
        tmps = space;

        QTextStream in(&fl);


        line = in.readLine();
        if(!line.isEmpty())
        {
            tmps->val = line.left(line.indexOf(" ")).toShort(&ok, 10);
            tmps->name = line.mid(line.indexOf(" "));
        }



        while(!in.atEnd())
        {
            line = in.readLine();

            if(line.isEmpty())
                continue;

            tmps->val = line.left(line.indexOf(" ")).toShort(&ok, 10);
            tmps->name = line.mid(line.indexOf(" "));

            tmps->next = new Space;
            tmps = tmps->next;
            tmps->next = NULL;
        }
        fl.close();
    }
}

int Eleuconf::getSpaceVal(int index, struct Space* space)
{
    int i;
    struct Space* tmp = space;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->val;
        tmp = tmp->next;
    }
    return 0;
}

QString Eleuconf::getSpaceName(int index, struct Space* space)
{
    int i;
    struct Space* tmp = space;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->name;
        tmp = tmp->next;
    }
    return NULL;
}

void Eleuconf::readMTypes()
{
    struct MTypes* tmps;
    QString line, strtmp;

    QFile fl("/root/Eleutheria/Muds/Ts2/mtypes.ini");
    if(!fl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout<<"file mtypes.ini non aperto"<<endl;
        return;
    } else {
        cout<<"file mtypes.ini aperto"<<endl;

        mtypes = new MTypes;
        tmps = mtypes;

        QTextStream in(&fl);


        line = in.readLine();
        if(!line.isEmpty())
        {
            tmps->type = line.left(line.indexOf(" "));
            tmps->name = line.mid(line.indexOf(" "));
        }



        while(!in.atEnd())
        {
            line = in.readLine();
            if(line.isEmpty())
                continue;

            tmps->type = line.left(line.indexOf(" "));
            tmps->name = line.mid(line.indexOf(" "));

            tmps->next = new MTypes;
            tmps = tmps->next;
            tmps->next = NULL;
        }
        fl.close();
    }
}

QString Eleuconf::getMTypesType(int index)
{
    int i;
    struct MTypes* tmp = mtypes;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->type;
        tmp = tmp->next;
    }
    return NULL;
}

QString Eleuconf::getMTypesName(int index)
{
    int i;
    struct MTypes* tmp = mtypes;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->name;
        tmp = tmp->next;
    }
    return NULL;
}

int Eleuconf::countMTypes()
{
    int i;
    struct MTypes* tmp = mtypes;
    for(i = 0; tmp; i++) {
        tmp = tmp->next;
    }
    return i;
}

void Eleuconf::readOCmds()
{
    QString line;
    struct OCmds* tmps;
    bool ok;

    QFile fl("/root/Eleutheria/Muds/Ts2/OpenCommands.ini");
    if(!fl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout<<"file OpenCommands non aperto"<<endl;
        return;
    } else {
        cout<<"file OpenCommands aperto"<<endl;

        ocmds = new OCmds;
        tmps = ocmds;

        QTextStream in(&fl);

        line = in.readLine();
        if(!line.isEmpty()) {
            QStringList list = line.split("|");

            tmps->name = list.at(0);
            tmps->cmd =  list.at(1).toShort(&ok, 10);
        }

        while(!in.atEnd())
        {
            line = in.readLine();
            if(line.isEmpty())
                continue;
            QStringList list = line.split("|");

            tmps->name = list.at(0);
            tmps->cmd =  list.at(1).toShort(&ok, 10);

            tmps->next = new OCmds;
            tmps = tmps->next;
            tmps->next = NULL;
        }
        fl.close();
    }
}

QString Eleuconf::getOCmdsName(int index)
{
    int i;
    struct OCmds* tmp = ocmds;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->name;
        tmp = tmp->next;
    }
    return NULL;
}

int Eleuconf::getOCmdsCmd(int index)
{
    int i;
    struct OCmds* tmp = ocmds;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->cmd;
        tmp = tmp->next;
    }
    return 0;
}

int Eleuconf::countOcmds()
{
    int i;
    struct OCmds* tmp = ocmds;
    for(i = 0; tmp; i++) {
        tmp = tmp->next;
    }
    return i;
}

void Eleuconf::readTdT()
{
    QString line;
    struct TdT* tmps;
    bool ok;

    QFile fl("/root/Eleutheria/Muds/Ts2/TrapDamageTypes.ini");
    if(!fl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout<<"file TrapDamageTypes.ini non aperto"<<endl;
        return;
    } else {
        cout<<"file TrapDamageTypes.ini aperto"<<endl;

        tdt = new TdT;
        tmps = tdt;

        QTextStream in(&fl);

        line = in.readLine();
        if(!line.isEmpty()) {
            QStringList list = line.split("|");

            tmps->name = list.at(0);
            tmps->val =  list.at(1).toShort(&ok, 10);
        }

        while(!in.atEnd())
        {
            line = in.readLine();
            if(line.isEmpty())
                continue;
            QStringList list = line.split("|");

            tmps->name = list.at(0);
            tmps->val =  list.at(1).toShort(&ok, 10);

            tmps->next = new TdT;
            tmps = tmps->next;
            tmps->next = NULL;
        }
        fl.close();
    }
}

QString Eleuconf::getTdTName(int index)
{
    int i;
    struct TdT* tmp = tdt;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->name;
        tmp = tmp->next;
    }
    return NULL;
}

int Eleuconf::getTdTVal(int index)
{
    int i;
    struct TdT* tmp = tdt;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->val;
        tmp = tmp->next;
    }
    return 0;
}

int Eleuconf::countTdT()
{
    int i;
    struct TdT* tmp = tdt;
    for(i = 0; tmp; i++) {
        tmp = tmp->next;
    }
    return i;
}

void Eleuconf::readWGrid()
{
    QString line;
    struct WGrid* tmps;

    QFile fl("/root/Eleutheria/Muds/Ts2/WGrid.ini");
    if(!fl.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout<<"file WGrid.ini non aperto"<<endl;
        return;
    } else {
        cout<<"file WGrid.ini aperto"<<endl;

        wgrid = new WGrid;
        tmps = wgrid;

        QTextStream in(&fl);

        line = in.readLine();

        while(!in.atEnd())
        {
            line = in.readLine();

            if(line.isEmpty())
                continue;
            QStringList list = line.split("|");

            tmps->name = list.at(0);
            tmps->type = list.at(1);
            tmps->msg = list.at(2);
            tmps->ita = list.at(3);

            tmps->next = new WGrid;
            tmps = tmps->next;
            tmps->next = NULL;
        }
        fl.close();
    }
}

QString Eleuconf::getWGridName( int index)
{
    int i;
    struct WGrid* tmp = wgrid;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->name;
        tmp = tmp->next;
    }
    return NULL;
}

QString Eleuconf::getWGridType( int index)
{
    int i;
    struct WGrid* tmp = wgrid;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->type;
        tmp = tmp->next;
    }
    return NULL;
}

QString Eleuconf::getWGridMsg( int index)
{
    int i;
    struct WGrid* tmp = wgrid;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->msg;
        tmp = tmp->next;
    }
    return NULL;
}

QString Eleuconf::getWGridIta( int index)
{
    int i;
    struct WGrid* tmp = wgrid;
    for (i = 0; tmp; i++) {
        if(i == index)
            return tmp->ita;
        tmp = tmp->next;
    }
    return NULL;
}

int Eleuconf::countWGrid()
{
    int i;
    struct WGrid* tmp = wgrid;
    for(i = 0; tmp; i++) {
        tmp = tmp->next;
    }
    return i;
}
#endif
