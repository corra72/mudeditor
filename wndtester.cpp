#include "wndtester.h"
#include "wndroom.h"
#include <QScrollBar>
#include <QCheckBox>
#include <QCloseEvent>
#include "eleuconf.h"
#include "kreatorsettings.h"
#include <typeinfo>
#include <qmessagebox.h>

using namespace ts;

const QString listExits[] =
    {
        "<font color=\"#00FF00\">Nord</font>",
        "<font color=\"yellow\">Est</font>",
        "<font color=\"blue\">Sud</font>",
        "<font color=\"#FF48FF\">Ovest</font>",
        "<font color=\"#00FFFF\">Alto</font>",
        "<font color=\"white\">Basso</font>"
    };

#define FG_BLACK         "<font color=black size=4>"
#define FG_RED           "<font color=#800000 size=4>"
#define FG_GREEN         "<font color=#00B300 size=4>"
#define FG_BROWN         "<font color=#808000 size=4>"
#define FG_BLUE          "<font color=#000080 size=4>"
#define FG_MAGENTA       "<font color=#800080 size=4>"
#define FG_CYAN          "<font color=#008080 size=4>"
#define FG_LT_GRAY       "<font color=#C0C0C0 size=4>"
#define FG_DK_GRAY       "<font color=#808080 size=4>"
#define FG_LT_RED        "<font color=#FF0000 size=4>"
#define FG_LT_GREEN      "<font color=#00FF00 size=4>"
#define FG_YELLOW        "<font color=#FFFF00 size=4>"
#define FG_LT_BLUE       "<font color=#0000FF size=4>"
#define FG_LT_MAGENTA    "<font color=#FF00FF size=4>"
#define FG_LT_CYAN       "<font color=#00FFFF size=4>"
#define FG_WHITE         "<font color=#FFFFFF size=4>"

WndTester::WndTester(Area *ref_area, QWidget* parent)
        : QDialog(parent), Ui::GuiTester()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndTester::WndTester( QWidget* ) called.");
#endif
    m_area_data = ref_area;
    m_area = *(ref_area);
    /*
    if (typeid(WndArea) == typeid(*parent))
    {
        //QMessageBox::information( this, "Kreator", "OK");
        this->parentwnd = dynamic_cast<WndArea*>(parent);
    } else {
        //QMessageBox::information( this, "Kreator", QString(typeid(WndArea).name()) + " " + QString(typeid(*parent).name()));
    }*/
    parentwnd = parent;
    init();
    le_Input->setFocus();
    go(m_area.rooms().at(0).vnumber());
}

WndTester::~WndTester()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndTester::~WndTester() called.");
#endif
}

void WndTester::init()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndTester::init() called.");
#endif
    setupUi(this);
    KreatorSettings::instance().loadGuiStatus("TesterWindow", this);
    textBrowser->setFontPointSize(12);
    scrollBar = textBrowser->verticalScrollBar();

    //connect(le_Input, SIGNAL(returnPressed()), pb_Invia, SLOT(animateClick()));
    connect(le_Input, SIGNAL(returnPressed()), le_Input, SLOT(selectAll()));
    connect(pb_Invia, SIGNAL(clicked()), this, SLOT(mudChangeText()));

    textBrowser->append(ParseAnsiColors("$c0009Digita help per avere informazioni sui comandi disponibili"));
}

void WndTester::go(VNumber vnum)
{
#if defined( KREATOR_DEBUG )
    qDebug("WndTester::go(VNumber vnum) called.");
#endif

    if (m_area.hasRoom(vnum)) {
        cr = vnum;
        QString buf;
        statusbar->showMessage(QString("Vnum: %1").arg(vnum));
        Room rp = m_area.room(vnum);
        buf = QString("<font color=\"yellow\" size=4>").append(ParseAnsiColors(rp.name())).append("</font>");
        if (rp.sectorType() == ROOM_SECTOR_TELEPORT) {
            buf.append(ParseAnsiColors(" $c0015 (TELEPORT)"));
        }
        textBrowser->append(buf);
        textBrowser->append(QString("<font color=\"lightgray\" size=4>").append(ParseAnsiColors(rp.description())).append("</font>"));

        buf.clear();
        for (int i = 0; i < rp.exits().size();i++) {
            buf.append(' ' + listExits[rp.exits().at(i).direction()]);
            if (rp.exits().at(i).isClosed())
                buf.append(" (closed)");
            if (rp.exits().at(i).hasFlag(EXIT_FLAG_SECRET))
                buf.append(ParseAnsiColors(" $c0009(secret)"));
            if (rp.exits().at(i).isLocked())
                buf.append(" (locked)");
        }
        if (buf.isEmpty())
            textBrowser->append(QString("<font color=\"lightgray\" size=4>Uscite visibili: Nessuna</font>"));
        else
            textBrowser->append(QString("<font color=\"lightgray\" size=4>Uscite visibili:").append(buf).append("</font>"));

        checkMobInit(vnum);

        textBrowser->append(ParseAnsiColors("$c0015-----------------"));
        textBrowser->append(ParseAnsiColors("$c0007Uscite visibili:"));

        buf.clear();
        for (int i = 0; i < rp.exits().size();i++) {
            if (m_area.hasRoom(rp.exits().at(i).toRoom())) {
                buf = QString("$c0007 %1 - %2 #%3").arg(rp.exits().at(i).name()).arg(m_area.room(rp.exits().at(i).toRoom()).name()).arg(rp.exits().at(i).toRoom());
                if (rp.exits().at(i).isClosed())
                    buf.append(" (closed)");
                if (rp.exits().at(i).isLocked())
                    buf.append(" (locked)");
                if (rp.exits().at(i).hasFlag(EXIT_FLAG_SECRET))
                    buf.append(" (secret)");
                textBrowser->append(ParseAnsiColors(buf));
            }
        }
        textBrowser->append(ParseAnsiColors("$c0015-----------------"));
    } else
        textBrowser->append(QString("<font color=\"red\" size=4>").append("Quella stanza non esiste").append("</font>"));
}

void WndTester::mudChangeText()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndTester::mudChangeText() called.");
#endif
    QString vv = le_Input->text();
    QString buf = vv.section(' ', 0, 0);
    QString buf2 = vv.section(' ', 1, 2);
    buf = buf.toLower();

    textBrowser->append(QString("<font color=\"yellow\" size=4>").append(buf));

    if (QString("go").startsWith(buf)) {
        bool ok;
        go(buf2.toLong(&ok, 10));
    } else if (QString("look").startsWith(buf)) {
        go(cr);
    } else if (QString("rstat").startsWith(buf)) {
        rstat();
    } else if (QString("redit").startsWith(buf)) {
        editRoom(cr);
    } else if (QString("south").startsWith(buf) || QString("sud").startsWith(buf))
        do_move(EXIT_DIRECTION_SOUTH);
    else if (QString("est").startsWith(buf) || QString("east").startsWith(buf))
        do_move(EXIT_DIRECTION_EAST);
    else if (QString("nord").startsWith(buf) || QString("north").startsWith(buf))
        do_move(EXIT_DIRECTION_NORTH);
    else if (QString("west").startsWith(buf) || QString("ovest").startsWith(buf))
        do_move(EXIT_DIRECTION_WEST);
    else if (QString("up").startsWith(buf) || QString("alto").startsWith(buf))
        do_move(EXIT_DIRECTION_UP);
    else if (QString("down").startsWith(buf) || QString("basso").startsWith(buf))
        do_move(EXIT_DIRECTION_DOWN);
    else if (QString("help").startsWith(buf))
        help();
    else if (QString("teleport").startsWith(buf))
        teleport();
    else textBrowser->append(ParseAnsiColors("$c0007Cosa?"));

    scrollBar->setValue(scrollBar->maximum() + scrollBar->pageStep());
}

int WndTester::inverseDirection(int dir) {
	switch (dir) {
                case EXIT_DIRECTION_NORTH: //N
                        dir=EXIT_DIRECTION_SOUTH;
			break;
                case EXIT_DIRECTION_EAST: //E
                        dir=EXIT_DIRECTION_WEST;
			break;
                case EXIT_DIRECTION_SOUTH: //S
                        dir=EXIT_DIRECTION_NORTH;
			break;
                case EXIT_DIRECTION_WEST: //W
                        dir=EXIT_DIRECTION_EAST;
			break;
                case EXIT_DIRECTION_UP: //U
                        dir=EXIT_DIRECTION_DOWN;
			break;
                case EXIT_DIRECTION_DOWN: //D
                        dir=EXIT_DIRECTION_UP;
			break;
	}
	return dir;
}

void WndTester::do_move(int cmd)
{
#if defined( KREATOR_DEBUG )
    qDebug("WndTester::do_move(int cmd) called.");
#endif
    Room rp = m_area.room(cr);
    if (!rp.hasExit(cmd)) {
        if (!chkCreateRooms->isChecked()) {
            textBrowser->append("<font color=\"red\" size=4>Non puoi andare in quella direzione.</font>");
            return;
        }
        else {
		// stanza corrente e la nuova da creare
		Room m_room = m_area_data->room(cr);
                Room new_room( m_area_data->firstFreeRoomVNumber(cr) );

		// assegna la zona alla room
		if( KreatorSettings::instance().assignZoneToRooms() )
			new_room.setZone( m_area_data->zoneReferenced( new_room.vnumber() ) );

		// uscita da qui alla nuova room
		Exit pToNew(cmd, m_room.vnumber());
		pToNew.setToRoom(new_room.vnumber());

		// uscita dalla nuova room a qui
		Exit pFromNew = pToNew.reversed();

		// aggingi uscite ad ambedue le room (corrente e nuova)
		m_room.addExit(pToNew);
		new_room.addExit(pFromNew);

		// aggiunge la room all area
		m_area_data->addRoom( new_room );
		m_area_data->addRoom( m_room );

		// mostra dialogo per edit della nuova room
                editRoom( new_room.vnumber() );

		m_area = *(m_area_data);
		go(new_room.vnumber());
		return;
		// vai nella nuova room
	}
    }
    Exit exitp = rp.exit(cmd);
    if (exitp.hasKeyHole() && exitp.doorKey())
        textBrowser->append(ParseAnsiColors(QString("$c0010Attraversi un'uscita chiusa dalla chiave: %1").arg(exitp.doorKey())));
    go(exitp.toRoom());
}

void WndTester::editRoom(VNumber room) {
    WndRoom *pWndRoom = new WndRoom( *(m_area_data), m_area_data->room( room ), this );
    pWndRoom->somethingChanged();
    if (parentwnd) {
        connect( pWndRoom, SIGNAL( dataSaved() ), parentwnd, SLOT( refreshRoomsView() ) );
        connect( pWndRoom, SIGNAL( dataSaved() ), parentwnd, SLOT( somethingChanged() ) );
        connect( pWndRoom, SIGNAL( dataSaved() ), this, SLOT( roomCreated() ) );
    }
    pWndRoom->setModal(true);
    pWndRoom->show();
}

void WndTester::roomCreated() {
    // prendi copia dell area dal pointer originale (il tester non lavora a vivo sul pointer)
    m_area = *(m_area_data);
    go(cr);
}

QString WndTester::ParseAnsiColors(const QString &text)
{
#if defined( KREATOR_DEBUG )
    qDebug("WndTester::ParseAnsiColors(QString txt) called.");
#endif
    QString txt = text;
    int i;

    for (i = txt.indexOf(QRegExp("\\$c[0-9]{4,4}"));i != -1; i = txt.indexOf(QRegExp("\\$c[0-9]{4,4}"))) {

        QString tmp = txt.mid(i + 2, i + 4);
        tmp = ansi_parse(tmp);
        txt.replace(i, 6, tmp);
    }
    for (i = 0; i < txt.size(); i++) {
        if (txt.at(i) == '\n') {
            txt.replace(i, 1, "<br>");
        }
    }

    return txt;
}

QString WndTester::ansi_parse(const QString &code)
{
#if defined( KREATOR_DEBUG )
    qDebug("WndTester::ansi_parse(QString code) called.");
#endif
    QString f;

    switch (code[2].unicode()) {
        case '0':
            switch (code[3].unicode()) {
                case '0':
                    f = FG_BLACK;
                    break;
                case '1':
                    f = FG_RED;
                    break;
                case '2':
                    f = FG_GREEN;
                    break;
                case '3':
                    f = FG_BROWN;
                    break;
                case '4':
                    f = FG_BLUE;
                    break;
                case '5':
                    f = FG_MAGENTA;
                    break;
                case '6':
                    f = FG_CYAN;
                    break;
                case '7':
                    f = FG_LT_GRAY;
                    break;
                case '8':
                    f = FG_DK_GRAY;
                    break;
                case '9':
                    f = FG_LT_RED;
                    break;
                default:
                    f = FG_DK_GRAY;
                    break;
            }
            break;

        case '1':
            switch (code[3].unicode()) {
                case '0':
                    f = FG_LT_GREEN;
                    break;
                case '1':
                    f = FG_YELLOW;
                    break;
                case '2':
                    f = FG_LT_BLUE;
                    break;
                case '3':
                    f = FG_LT_MAGENTA;
                    break;
                case '4':
                    f = FG_LT_CYAN;
                    break;
                case '5':
                    f = FG_WHITE;
                    break;
                default:
                    f = FG_LT_GREEN;
                    break;
            }
            break;

        default:
            f = FG_LT_RED;
            break;
    }

    return f;
}

void WndTester::help()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndTester::help() called.");
#endif
    textBrowser->append(ParseAnsiColors("$c0014----------------------------------------------"));
    textBrowser->append(ParseAnsiColors("$c0010go vnum: $c0014vai nella stanza (vnum)"));
    textBrowser->append(ParseAnsiColors("$c0010teleport: $c0014attiva la teleport"));
    textBrowser->append(ParseAnsiColors("$c0010rstat: $c0014stat della stanza"));
    textBrowser->append(ParseAnsiColors("$c0010redit: $c0014edit della stanza"));
    textBrowser->append(ParseAnsiColors("$c0014----------------------------------------------"));
}

void WndTester::teleport()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndTester::teleport() called.");
#endif

    Room rp = m_area.room(cr);

    if (rp.sectorType() == ROOM_SECTOR_TELEPORT) {
        go(rp.teleportToRoom());
    }
}

void WndTester::checkMobInit(VNumber vnum)
{
#if defined( KREATOR_DEBUG )
    qDebug("WndTester::CheckMobInit(VNumber) called.");
#endif
    int i;
    for (i = 0; i < m_area.zones().size() ; i++) {
        ZoneCommandList zcl = m_area.zones().at(i).commandList();
        ZoneCommandList::const_iterator it = zcl.begin();
        while (it != zcl.end()) {
            qWarning(qPrintable(it->toString(false)));
            if (it->isMobLoad() && it->argument(ZoneCommand::Argument3) == vnum) {
                textBrowser->append(ParseAnsiColors("$c0007" + m_area.mob(it->argument(ZoneCommand::Argument1)).longDescription()));
            }
            it++;
        }
    }
}

void WndTester::rstat()
{
    Room rp = m_area.room(cr);

    QString buf = QString("$c0005Room name: $c0011%1$c0005, Zona : $c0014%2$c0005. V-Number : $c0010%3").arg(rp.name()).arg(rp.zone()).arg(rp.vnumber());
    textBrowser->append(ParseAnsiColors(buf));

    textBrowser->append(ParseAnsiColors("$c0005Sector type : $c0014" + ConstantName::roomSector(rp.realSectorType())));

    buf = "$c0005Room flags: $c0014" + Utils::bitvector2string(rp.flags(), Eleuconf::getAllFlagsCaption(Eleuconf::roomFlags));
    textBrowser->append(ParseAnsiColors(buf));


    textBrowser->append(ParseAnsiColors("$c0005Descrizione:"));

    textBrowser->append(ParseAnsiColors("$c0007" + rp.description()));

    buf = "$c0005Extra description keywords(s):$c0007 ";

    if (rp.extraDescriptions().size() > 0) {
        buf += "\n\r";
        for (int i = 0; i < rp.extraDescriptions().size(); i++) {
            buf += rp.extraDescriptions().at(i).keys() + "\n\r";
        }
        buf += "\n\r";
    } else {
        buf += " Nessuna\n\r";
    }
    textBrowser->append(ParseAnsiColors(buf));

    textBrowser->append(ParseAnsiColors("$c0005------- $c0014Uscite $c0005-------"));
    for (int i = 0; i < rp.exits().size(); i++) {
        buf = QString("\n\r$c0005Direzione $c0015%1").arg(rp.exits().at(i).name());
        if (!rp.exits().at(i).keys().isEmpty()) {
            buf += "Keyword : $c0014" + rp.exits().at(i).keys();
        }
        textBrowser->append(ParseAnsiColors(buf));

        if (!rp.exits().at(i).description().isEmpty()) {
            textBrowser->append(ParseAnsiColors("$c0005Descrizione:"));
            textBrowser->append(ParseAnsiColors("$c0007" + rp.exits().at(i).description()));
        }

        buf = "$c0005Exit flag: $c0015" + Utils::bitvector2string(rp.exits().at(i).flags(), Eleuconf::getAllFlagsCaption(Eleuconf::exitFlags));
        textBrowser->append(ParseAnsiColors(buf));

        buf = QString("$c0005Key num: $c0015%1").arg(rp.exits().at(i).doorKey());
        textBrowser->append(ParseAnsiColors(buf));

        buf = QString("$c0005Alla stanza: $c0015%1").arg(rp.exits().at(i).toRoom());
        textBrowser->append(ParseAnsiColors(buf));

        if (rp.exits().at(i).openCommand() > 0) {
            buf = QString("$c0005OpenCommand: $c0015%1").arg(rp.exits().at(i).openCommand());
            textBrowser->append(ParseAnsiColors(buf));
        }
    }
    textBrowser->append("");
    return;
}

void WndTester::closeEvent(QCloseEvent* e)
{
    e->accept();
    KreatorSettings::instance().saveGuiStatus("TesterWindow", this);
}

