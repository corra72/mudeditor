
#ifndef TS_WNDAREA_H
#define TS_WNDAREA_H

#include <QMainWindow>
#include "myqt.h"
#include "ui_guiarea.h"
#include "area.h"

using namespace ts;

class QMenu;
class QAction;
class QToolBar;


class WndArea : public QMainWindow, private Ui::GuiArea
{
  Q_OBJECT

  public:
    explicit WndArea( QWidget* parent = 0, const char* name = "" );
    ~WndArea();

    bool isChanged() { return m_area.isChanged(); }
    void loadArea( const QString& );

  public slots:
    void saveArea();
    void saveAreaAs();
    void refreshRoomsView();
    void somethingChanged();

  protected slots:
    void showEditDialog( QTreeWidgetItem*, int );
    void showEditDialog( int, VNumber );
    void showEditDialog();
    void changeFileName();
    void changeView( int );
    void refreshMobsView();
    void refreshItemsView();
    void refreshShopsView();
    void refreshZonesView();
    void refreshView();
    void removeObject();
    void createObject();
    void cloneObject();
    void showReportCoins();
    void showReportTreasures();
    void showReportExits();
    void showReportKeys();
    void showReportDeathRooms();
    void showChangeVNumbers();
    void showFind();
	void showTester();
    void showMultipleModify();

  protected:
    void init();
    void assignFileNames( const QString& );
    void showRooms();
    void showMobs();
    void showItems();
    void showShops();
    void showZones();
    void closeEvent( QCloseEvent* );
    void refreshTitle();
    void initMenuArea();
    void initMenuActions();
    void initMenuReport();
    void initMenuUtils();
    void refreshFileView();
    void setAreaFile( const QString& );

  protected:
    QAction* mp_actSaveArea;
    QAction* mp_actSaveAreaAs;
    QAction* mp_actCloseArea;
    QAction* mp_actNew;
    QAction* mp_actEdit;
    QAction* mp_actClone;
    QAction* mp_actRemove;
    QAction* mp_actReportCoins;
    QAction* mp_actReportTreasures;
    QAction* mp_actReportExits;
    QAction* mp_actReportKeys;
    QAction* mp_actReportDeathRooms;
    QAction* mp_actChangeVNumbers;
    QAction* mp_actFind;
	QAction* mp_actTester;
    QAction* mp_actMultipleModify;

    QMenu* mp_menuArea;
    QMenu* mp_menuActions;
    QMenu* mp_menuReport;
    QMenu* mp_menuUtils;

    QToolBar* mp_barArea;
    QToolBar* mp_barActions;
    QToolBar* mp_barReport;
    QToolBar* mp_barUtils;

  private:
    Area m_area;
    int m_currentObjectTypeList;
    MyQt::ButtonGroup* mp_bgViews;
    QList<QWidget*> m_childs;

};

#endif // TS_WNDAREA_H

