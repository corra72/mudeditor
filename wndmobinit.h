#if 0
#ifndef TS_WNDMOBINIT_H
#define TS_WNDMOBINIT_H

#include <QDialog>
#include "ui_guimobinit.h"
#include "area.h"
#include "wndzone.h"

using namespace ts;


class WndMobInit : public QDialog, private Ui::GuiMobInit
{
    Q_OBJECT

    public:
        WndMobInit( VNumber mvnu, VNumber room, int max, Area&, QWidget* parent);
        virtual ~WndMobInit();

  /*  signals:
        void dataSaved();

    protected slots:
        void saveData();
        void restoreData();
        void somethingChanged();
        void saveAndClose();
        void editInit();
        void editInit( QTreeWidgetItem*, int );
        void removeInit();
        void addInit();
        void copyInit();

    protected:
        void init();
        void closeEvent( QCloseEvent* );
        void loadData();
        void refreshTitle();
        void refreshView();

    private:

        Area& m_area;
        Zone m_zone;
        VNumber m_vnum;
        bool m_mob;
        QList<int> m_association;
        */

};
#endif
#endif
