#include "wndmultiplemodify.h"
#include <QHeaderView>
#include "area.h"
#include "selectobject.h"

WndMultipleModify::WndMultipleModify(Area  *area, QWidget* parent)
    : QDialog(parent), Ui::GuiMultipleModify()
{
    int i;
    QList<Room> rm;
    QList<Mob> mb;
    QList<Item> obj;

    setupUi(this);

    m_area = area;

    listRoom->setColumnCount(2);
    listRoom->header()->hide();

    listMob->setColumnCount(2);
    listMob->header()->hide();

    listObj->setColumnCount(2);
    listObj->header()->hide();

    QList<QTreeWidgetItem *> items;
    for(rm = m_area->rooms(), i = 0; i < rm.size(); i++) {
        QTreeWidgetItem *itm = new QTreeWidgetItem(QStringList() << QString::number(rm.at(i).vnumber()) << rm.at(i).name());
        itm->setFlags(itm->flags()|Qt::ItemIsUserCheckable);
        itm->setCheckState(0, Qt::Unchecked);
        items.append(itm);
    }
    listRoom->insertTopLevelItems(0, items);

    items.clear();
    for(mb = m_area->mobs(), i = 0; i < mb.size(); i++) {
        QTreeWidgetItem *itm = new QTreeWidgetItem(QStringList() << QString::number(mb.at(i).vnumber()) << mb.at(i).name());
        itm->setFlags(itm->flags()|Qt::ItemIsUserCheckable);
        itm->setCheckState(0, Qt::Unchecked);
        items.append(itm);
    }
    listMob->insertTopLevelItems(0, items);

    items.clear();
    for(obj = m_area->items(), i = 0; i < obj.size(); i++) {
        QTreeWidgetItem *itm = new QTreeWidgetItem(QStringList() << QString::number(obj.at(i).vnumber()) << obj.at(i).name());
        itm->setFlags(itm->flags()|Qt::ItemIsUserCheckable);
        itm->setCheckState(0, Qt::Unchecked);
        items.append(itm);
    }
    listObj->insertTopLevelItems(0, items);

    connect(pbRoomFlags, SIGNAL(clicked()), this, SLOT(roomFlagsClicked()));
    connect(pbActs, SIGNAL(clicked()), this, SLOT(actsClicked()));
    connect(pbNewActs, SIGNAL(clicked()), this, SLOT(newActsClicked()));
    connect(pbAffects, SIGNAL(clicked()), this, SLOT(affectsClicked()));
    connect(pbAffects2, SIGNAL(clicked()), this, SLOT(affects2Clicked()));
    connect(pbObjFlags, SIGNAL(clicked()), this, SLOT(objFlagsClicked()));
    connect(pbObjNewFlags, SIGNAL(clicked()), this, SLOT(objNewFlagsClicked()));
}

void WndMultipleModify::roomFlagsClicked()
{
     QList<int> ls = SelectObject::getMultipleFlags(Eleuconf::roomFlags,"Room Flags",this);
     QList<VNumber> vnLst = getTreeCheckedList(listRoom);
     for(int i = 0; i < vnLst.size(); i++) {
         Room rp = m_area->room(vnLst.at(i));
         BitVector flags = rp.flags();
         for(int k = 0; k < ls.size(); k++) {
             if(ls.at(k) == Qt::PartiallyChecked) {
                 Bit::remove(&flags, k);
             } else if(ls.at(k) ==  Qt::Checked) {
                 Bit::set(&flags, k);
             }
         }
         rp.setFlags(flags);
         m_area->addRoom(rp);
         m_area->setRoomsChanged();
         m_area->sortRooms();
         emit( roomSaved() );
     }
}

void WndMultipleModify::actsClicked()
{
    QList<Flags> list;

    for(int i = 0; i < Eleuconf::mobActionsFlags.size(); i++)
    {
        if(Eleuconf::getFlagsFindex(i, Eleuconf::mobActionsFlags) == 0) {
            list.append(Eleuconf::mobActionsFlags.at(i));
        }
    }
    QList<int> ls = SelectObject::getMultipleFlags(list,"Mob Acts",this);
    QList<VNumber> vnLst = getTreeCheckedList(listMob);
    for(int i = 0; i < vnLst.size(); i++) {
        Mob mob = m_area->mob(vnLst.at(i));
        BitVector acts = mob.acts();
        for(int k = 0; k < ls.size(); k++) {
            if(ls.at(k) == Qt::PartiallyChecked) {
                Bit::remove(&acts, k);
            } else if(ls.at(k) ==  Qt::Checked) {
                Bit::set(&acts, k);
            }
        }
        mob.setActs(acts);
        m_area->addMob(mob);
        m_area->setMobsChanged();
        m_area->sortMobs();
        emit( mobSaved() );
    }
}

void WndMultipleModify::newActsClicked()
{
    QList<Flags> list;

    for(int i = 0; i < Eleuconf::mobActionsFlags.size(); i++)
    {
        if(Eleuconf::getFlagsFindex(i, Eleuconf::mobActionsFlags) == 1) {
            list.append(Eleuconf::mobActionsFlags.at(i));
        }
    }
    QList<int> ls = SelectObject::getMultipleFlags(list,"Mob New Acts",this);
    QList<VNumber> vnLst = getTreeCheckedList(listMob);
    for(int i = 0; i < vnLst.size(); i++) {
        Mob mob = m_area->mob(vnLst.at(i));
        BitVector acts = mob.extraFlags();
        for(int k = 0; k < ls.size(); k++) {
            if(ls.at(k) == Qt::PartiallyChecked) {
                Bit::remove(&acts, k);
            } else if(ls.at(k) ==  Qt::Checked) {
                Bit::set(&acts, k);
            }
        }
        mob.setExtraFlags(acts);
        m_area->addMob(mob);
        m_area->setMobsChanged();
        m_area->sortMobs();
        emit( mobSaved() );
    }
}

void WndMultipleModify::affectsClicked()
{
    QList<Flags> list;

    for(int i = 0; i < Eleuconf::mobAffectionsFlags.size(); i++)
    {
        if(Eleuconf::getFlagsFindex(i, Eleuconf::mobAffectionsFlags) == 0) {
            list.append(Eleuconf::mobAffectionsFlags.at(i));
        }
    }

    QList<int> ls = SelectObject::getMultipleFlags(list,"Mob Affects",this);
    QList<VNumber> vnLst = getTreeCheckedList(listMob);
    for(int i = 0; i < vnLst.size(); i++) {
        Mob mob = m_area->mob(vnLst.at(i));
        BitVector aff = mob.affects();
        for(int k = 0; k < ls.size(); k++) {
            if(ls.at(k) == Qt::PartiallyChecked) {
                Bit::remove(&aff, k);
            } else if(ls.at(k) ==  Qt::Checked) {
                Bit::set(&aff, k);
            }
        }
        mob.setAffects(aff);
        m_area->addMob(mob);
        m_area->setMobsChanged();
        m_area->sortMobs();
        emit( mobSaved() );
    }
}

void WndMultipleModify::affects2Clicked()
{
    QList<Flags> list;

    for(int i = 0; i < Eleuconf::mobAffectionsFlags.size(); i++)
    {
        if(Eleuconf::getFlagsFindex(i, Eleuconf::mobAffectionsFlags) == 1) {
            list.append(Eleuconf::mobAffectionsFlags.at(i));
        }
    }
    QList<int> ls = SelectObject::getMultipleFlags(list,"Mob Affects2",this);
    QList<VNumber> vnLst = getTreeCheckedList(listMob);
    for(int i = 0; i < vnLst.size(); i++) {
        Mob mob = m_area->mob(vnLst.at(i));
        BitVector aff = mob.affects2();
        for(int k = 0; k < ls.size(); k++) {
            if(ls.at(k) == Qt::PartiallyChecked) {
                Bit::remove(&aff, k);
            } else if(ls.at(k) ==  Qt::Checked) {
                Bit::set(&aff, k);
            }
        }
        mob.setAffects2(aff);
        m_area->addMob(mob);
        m_area->setMobsChanged();
        m_area->sortMobs();
        emit( mobSaved() );
    }
}

void WndMultipleModify::objFlagsClicked()
{
    QList<Flags> list;

    for(int i = 0; i < Eleuconf::objFlags.size(); i++)
    {
        if(Eleuconf::getFlagsFindex(i, Eleuconf::objFlags) == 0) {
            list.append(Eleuconf::objFlags.at(i));
        }
    }
    QList<int> ls = SelectObject::getMultipleFlags(list, "Object Flags",this);
    QList<VNumber> vnLst = getTreeCheckedList(listObj);
    for(int i = 0; i < vnLst.size(); i++) {
        Item itm = m_area->item(vnLst.at(i));
        BitVector flags = itm.flags();
        for(int k = 0; k < ls.size(); k++) {
            if(ls.at(k) == Qt::PartiallyChecked) {
                Bit::remove(&flags, k);
            } else if(ls.at(k) ==  Qt::Checked) {
                Bit::set(&flags, k);
            }
        }
        itm.setFlags(flags);
        m_area->addItem(itm);
        m_area->setItemsChanged();
        m_area->sortItems();
        emit( objSaved() );
    }
}

void WndMultipleModify::objNewFlagsClicked()
{
    QList<Flags> list;

    for(int i = 0; i < Eleuconf::objFlags.size(); i++)
    {
        if(Eleuconf::getFlagsFindex(i, Eleuconf::objFlags) == 1) {
            list.append(Eleuconf::objFlags.at(i));
        }
    }
    QList<int> ls = SelectObject::getMultipleFlags(list,"Object New Flags",this);
    QList<VNumber> vnLst = getTreeCheckedList(listObj);
    for(int i = 0; i < vnLst.size(); i++) {
        Item itm = m_area->item(vnLst.at(i));
        BitVector flags = itm.extraFlags();
        for(int k = 0; k < ls.size(); k++) {
            if(ls.at(k) == Qt::PartiallyChecked) {
                Bit::remove(&flags, k);
            } else if(ls.at(k) ==  Qt::Checked) {
                Bit::set(&flags, k);
            }
        }
        itm.setExtraFlags(flags);
        m_area->addItem(itm);
        m_area->setItemsChanged();
        m_area->sortItems();
        emit( objSaved() );
    }
}

QList<VNumber> WndMultipleModify::getTreeCheckedList(QTreeWidget *tw)
{
    QList<VNumber> lst;
    bool ok;
    for(int i = 0; i < tw->topLevelItemCount(); i++)
    {
        if(tw->topLevelItem(i)->checkState(0) == Qt::Checked)
            lst << tw->topLevelItem(i)->text(0).toULong(&ok);
    }
    return lst;
}
