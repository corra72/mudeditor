#ifndef TS_WNDMULTIPLEMODIFY_H
#define TS_WNDMULTIPLEMODIFY_H

#include <QDialog>
#include <QMap>
#include "ui_guimultiplemodify.h"
#include "area.h"

using namespace ts;

class WndMultipleModify : public QDialog, private Ui::GuiMultipleModify
{
    Q_OBJECT

    public:
        WndMultipleModify(Area *area, QWidget* parent);
        QList<VNumber> getTreeCheckedList(QTreeWidget *tw);
    signals:
        void roomSaved();
        void objSaved();
        void mobSaved();
    private slots:
        void roomFlagsClicked();
        void actsClicked();
        void newActsClicked();
        void affectsClicked();
        void affects2Clicked();
        void objFlagsClicked();
        void objNewFlagsClicked();


    private:
        Area *m_area;
};

#endif
