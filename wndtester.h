
#ifndef TS_WNDTESTER_H
#define TS_WNDTESTER_H

#include <QDialog>
#include "ui_guitester.h"
#include "constantname.h"
#include "area.h"
#include "wndarea.h"

using namespace ts;

class QMenu;
class QAction;


class WndTester : public QDialog, Ui::GuiTester
{
  Q_OBJECT

public:
  WndTester( Area*, QWidget* );
  ~WndTester();

protected slots:
	void mudChangeText();
        void roomCreated();

protected:
	void init();
	void go(VNumber);
	void do_move(int cmd);
	QString ParseAnsiColors(const QString &text);
	QString ansi_parse(const QString &code);
        void help();
        void teleport();
	void checkMobInit(VNumber vnum);
	void rstat();
        void closeEvent( QCloseEvent* );
	int inverseDirection(int dir);
        void editRoom(VNumber room);

protected:
	QScrollBar *scrollBar;
	Area m_area;
	Area *m_area_data;
	VNumber cr;
        QWidget *parentwnd;

};
/*
class ts: public QMainWindow
    {
        Q_OBJECT

    public:
    ts();
	QMainWindow *MainWindow;
	QWidget *centralWidget;
    QLineEdit *lineEdit;
    QPushButton *invia;
    QFrame *frame;
    QTextBrowser *textBrowser;

    private slots:
        void newFile();
		void shut();
        void open();
        void save();
		void aboutQt();
		void mudChangeText();

    private:
        void createActions();
        void createMenus();
		void setupToolBar();
		void setupMenuBar();
		void check_world(FILE* fl);
		void setup_dir(FILE * fl, long room, int dir);
		void load_one_room(FILE *fl, struct room_data *rp);
		struct room_data *room_find_or_create(struct room_data *rb[], long key);
		void allocate_room(long room_number);
        void list_exits_in_room(long vnum);
        int is_abbrev(char *arg1, char *arg2);
        void go(struct room_data *rp);
        char *one_argument(char *argument, char *first_arg);
        int search_block(char *arg, char **list, bool exact);
        int fill_word(char *argument);
        struct room_data *real_roomp(long virtuale);
        void do_move(int cmd);
        void rstat();
        char *ParseAnsiColors(char *txt);
        char *ansi_parse(char *code);
	void zreset();
	//struct index_data *generate_indices(FILE * fl, int *top, int *sort_top, int *alloc_top);
    void load_zones(FILE *fl);



		QMenuBar *bar;
		QMenu *dockWidgetMenu;
		QToolBar *toolbar;
        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *formatMenu;
        QMenu *helpMenu;
        QActionGroup *alignmentGroup;
		QScrollBar *scrollBar;
        QAction *newAct;
        QAction *openAct;
        QAction *saveAct;

        QAction *exitAct;
        QAction *aboutQtAct;

    };
	*/
#endif // TS_WNDTESTER_H

