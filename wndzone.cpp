#include "wndzone.h"
#include <QTreeWidget>
#include <QLineEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QMenu>
#include <QLabel>
#include <QCloseEvent>
#include <QCursor>
#include <QHeaderView>
#include <QApplication>
#include <QRegExp>
#include "wndzonecommand.h"
#include "validator.h"
#include "config.h"
#include "constantname.h"
#include "selectobject.h"
#include "guiutils.h"
#include "kreatorsettings.h"

using namespace ts;


WndZone::WndZone(Area& ref_area, const Zone& ref_zone, QWidget* parent)
        : QDialog(parent), Ui::GuiZone(),
        m_area(ref_area),
        m_zone(ref_zone),
        m_zoneCommands(m_zone.commandList()),
        m_lastLineSelected(0),
        m_commandGroupCopied()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::WndZone( const Area&, const Zone&, QWidget* ) called.");
#endif
    init();
    loadData();
    mp_leName->setFocus();
    setModal(true);
}

WndZone::~WndZone()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::~WndZone() called.");
#endif
}

void WndZone::init()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::init() called.");
#endif
    setupUi(this);
    KreatorSettings::instance().loadGuiStatus("ZoneWindow", this);

    mp_tbFlags->setIcon(TS::GetEditIcon());

    mp_leVNumber->setValidator(Validator::vnumber());

    mp_twZoneCommands->header()->hide();

    mp_comboRepopMode->clear();
    for (int i = 0; i < ZONE_REPOP_MODE_END; i++)
        mp_comboRepopMode->insertItem(i, ConstantName::zoneRepopMode(i).toLower());

    mp_bgFilters = new MyQt::ButtonGroup(this);
    mp_bgFilters->addButton(mp_cbGroup, 0);
    mp_bgFilters->addButton(mp_cbMobs, 1);
    mp_bgFilters->addButton(mp_cbItems, 2);
    mp_bgFilters->addButton(mp_cbDoors, 3);
    mp_bgFilters->addButton(mp_cbComments, 4);
    mp_bgFilters->addButton(mp_cbSpaces, 5);
    mp_bgFilters->addButton(mp_cbExpand, 6);
    KreatorSettings::instance().loadButtonGroupStatus("ZoneCommandFilters", mp_bgFilters);

    connect(mp_pbSave, SIGNAL(clicked()), this, SLOT(saveData()));
    connect(mp_pbRestore, SIGNAL(clicked()), this, SLOT(restoreData()));
    connect(mp_pbOk, SIGNAL(clicked()), this, SLOT(saveAndClose()));
    connect(mp_pbCancel, SIGNAL(clicked()), this, SLOT(close()));

    connect(mp_leVNumber, SIGNAL(textChanged(const QString&)), this, SLOT(somethingChanged()));
    connect(mp_leMaxVNumber, SIGNAL(textChanged(const QString&)), this, SLOT(somethingChanged()));
    connect(mp_leRepopInterval, SIGNAL(textChanged(const QString&)), this, SLOT(somethingChanged()));
    connect(mp_leName, SIGNAL(textChanged(const QString&)), this, SLOT(somethingChanged()));
    connect(mp_comboRepopMode, SIGNAL(activated(int)), this, SLOT(somethingChanged()));
    connect(mp_lePwpLevel, SIGNAL(textChanged(const QString&)), this, SLOT(somethingChanged()));

    connect(mp_bgFilters, SIGNAL(buttonClicked(int)), this, SLOT(refreshView()));
    connect(mp_tbFlags, SIGNAL(clicked()), this, SLOT(editFlags()));
    connect(mp_leFilter, SIGNAL(textEdited(const QString&)), this, SLOT(refreshView()));
    connect(mp_leFilter, SIGNAL(returnPressed()), this, SLOT(refreshView()));

    mp_leMaxVNumber->setValidator(Validator::vnumber());
    mp_leRepopInterval->setValidator(Validator::integer());
    mp_lePwpLevel->setValidator(Validator::integer());

    mp_popupMenu = new QMenu(this);
    mp_popupMenu->addAction(QIcon(":/images/edit.png"), trUtf8("Modifica..."), this, SLOT(editCommandLine()));
    mp_popupMenu->addSeparator();
    mp_actUpdateMaxInits = mp_popupMenu->addAction(QIcon(":/images/max.png"), trUtf8("Aggiorna maxaggi"), this, SLOT(updateMaxInits()));
    mp_actUpdateMaxInits->setEnabled(false);
    mp_popupMenu->addSeparator();
    mp_popupMenu->addAction(QIcon(":/images/add.png"), trUtf8("Inserisci una linea vuota"), this, SLOT(addNewEmptyLine()));
    mp_popupMenu->addAction(QIcon(":/images/cut.png"), trUtf8("Taglia"), this, SLOT(cutCommandLine()));
    mp_popupMenu->addAction(QIcon(":/images/copy.png"), trUtf8("Copia"), this, SLOT(copyCommandLine()));
    mp_actPaste = mp_popupMenu->addAction(QIcon(":/images/paste.png"), trUtf8("Incolla"), this, SLOT(pasteCommandLine()));
    mp_actPaste->setEnabled(false);
    mp_popupMenu->addSeparator();
    mp_popupMenu->addAction(QIcon(":/images/remove.png"), trUtf8("Elimina"), this, SLOT(removeCommandLine()));
    mp_twZoneCommands->setMenu(mp_popupMenu);

    connect(mp_twZoneCommands, SIGNAL(itemSelectionChanged()), this, SLOT(lineSelected()));
    connect(mp_twZoneCommands, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this, SLOT(editCommandLine()));

}

void WndZone::refreshPanel()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::refreshPanel() called.");
#endif
    mp_leVNumber->setText(QString::number(m_zone.vnumber()));
    mp_leName->setText(m_zone.name());
    mp_leMinVNumber->setText(QString::number(m_zone.minVNumber()));
    mp_leMaxVNumber->setText(QString::number(m_zone.maxVNumber()));
    mp_comboRepopMode->setCurrentIndex(m_zone.repopMode());
    mp_leRepopInterval->setText(QString::number(m_zone.repopInterval()));
    mp_lePwpLevel->setText(QString::number(m_zone.pwpLevel()));
}

void WndZone::refreshView()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::refreshView() called.");
#endif
    QApplication::setOverrideCursor(Qt::WaitCursor);
    qApp->processEvents();

    mp_twZoneCommands->clear();
    mp_twZoneCommands->setAlternatingRowColors(mp_cbGroup->isChecked());
    ZoneCommandList::const_iterator it = m_zoneCommands.begin();

    QTreeWidgetItem* last_item = 0;
    QTreeWidgetItem* item = 0;
    QTreeWidgetItem* selected_item = 0;
    QTreeWidgetItem* selected_parent_item = 0;
    bool has_regexp_filter = !mp_leFilter->text().trimmed().isEmpty();
    QRegExp reg_exp(mp_leFilter->text().toLower().trimmed());
    QString sTxt = "";
    bool regexp_filter_accepted = true;

    while (it != m_zoneCommands.end()) {
        sTxt = (*it).toString(true);
        regexp_filter_accepted = (!has_regexp_filter || sTxt.toLower().contains(reg_exp) || sTxt.toLower().isEmpty());

        if (regexp_filter_accepted) {
            if ((mp_cbMobs->isChecked() && (*it).isMobCommand())
                    || (mp_cbItems->isChecked() && (*it).isItemCommand())
                    || (mp_cbDoors->isChecked() && (*it).isDoorInit())) {

                if (mp_cbGroup->isChecked()) {
                    if ((*it).hasParent()) {
                        if (last_item)
                            item = new QTreeWidgetItem(last_item);
                        else
                            item = new QTreeWidgetItem(mp_twZoneCommands);
                    } else {
                        item = new QTreeWidgetItem(mp_twZoneCommands);
                        if (last_item && mp_cbExpand->isChecked())
                            mp_twZoneCommands->expandItem(last_item);
                        last_item = item;
                    }
                } else
                    item = new QTreeWidgetItem(mp_twZoneCommands);
            } else if ((*it).isOnlyComment() && (*it).comment().isEmpty() && mp_cbSpaces->isChecked())
                item = new QTreeWidgetItem(mp_twZoneCommands);
            else if ((*it).isOnlyComment() && !(*it).comment().isEmpty() && mp_cbComments->isChecked())
                item = new QTreeWidgetItem(mp_twZoneCommands);
        }

        if (item) {
            item->setText(0, Utils::vnumber2string((*it).id(), 6));
            item->setText(1, sTxt);
            if ((*it).id() == m_lastLineSelected) {
                selected_item = item;
                if ((*it).hasParent() && mp_cbGroup->isChecked())
                    selected_parent_item = last_item;
            }
            item = 0;
        }
        ++it;
    }

    if (selected_item) {
        if (selected_parent_item)
            mp_twZoneCommands->expandItem(selected_parent_item);
        mp_twZoneCommands->clearSelection();
        mp_twZoneCommands->setItemSelected(selected_item, true);
        mp_twZoneCommands->scrollToItem(selected_item);
    }

    QApplication::restoreOverrideCursor();
    qApp->processEvents();
}

void WndZone::refreshTitle()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::refreshTitle() called.");
#endif
    QString sTitle = "";
    sTitle.sprintf("Zona #%ld - %s", m_zone.vnumber(), m_zone.name().toUtf8().data());
    if (mp_pbSave->isEnabled())
        sTitle.append(QString(" [modificata]"));
    setWindowTitle(sTitle);
}

void WndZone::somethingChanged()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::somethingChanged() called.");
#endif
    mp_pbSave->setEnabled(true);
    refreshTitle();
}

void WndZone::restoreData()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::restoreData() called.");
#endif
    if (m_area.hasZone(m_zone.vnumber())) {
        m_zone = m_area.zone(m_zone.vnumber());
        m_zoneCommands = m_zone.commandList();
        loadData();
    } else
        QMessageBox::warning(this, TS::MESSAGE_BOX_WARNING,
                             TS::MESSAGE_WARNING_NO_VNUMBER.arg(m_zone.vnumber()));
}

void WndZone::loadData()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::loadData() called.");
#endif
    refreshPanel();
    refreshView();
    refreshFlags();
    mp_pbSave->setEnabled(false);
    refreshTitle();
    mp_leName->setFocus();
}

void WndZone::saveData()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::saveData() called.");
#endif
    VNumber old_vnum = m_zone.vnumber();
    VNumber new_vnum = mp_leVNumber->text().toLong();

    if (old_vnum != new_vnum) {
        if (m_area.hasZone(new_vnum)
                && QMessageBox::question(this, TS::MESSAGE_BOX_QUESTION,
                                         TS::MESSAGE_ASK_OVERWRITE_VNUMBER.arg(new_vnum),
                                         trUtf8("Si"), trUtf8("No"), QString(), 1, 1)
           )
            return;

        if (!QMessageBox::question(this, TS::MESSAGE_BOX_QUESTION,
                                   TS::MESSAGE_ASK_UPDATE_REFERENCES.arg(old_vnum).arg(new_vnum),
                                   trUtf8("Si"), trUtf8("No"), QString(), 0, 1)
           )
            m_area.changeZoneVNumber(old_vnum, new_vnum);
    }

    m_zone.setVNumber(new_vnum);
    m_zone.setName(mp_leName->text());
    m_zone.setMaxVNumber(mp_leMaxVNumber->text().toLong());
    m_zone.setRepopMode(mp_comboRepopMode->currentIndex());
    m_zone.setRepopInterval(mp_leRepopInterval->text().toLong());
    bool ok;
    m_zone.setPwpLevel(mp_lePwpLevel->text().toInt(&ok, 10));

    m_zone.setCommandList(m_zoneCommands);

    m_area.addZone(m_zone);
    m_area.setZonesChanged();
    m_area.sortZones();
    refreshPanel();
    mp_pbSave->setEnabled(false);
    refreshTitle();
    emit(dataSaved());
}

void WndZone::saveAndClose()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::saveAndClose() called.");
#endif
    if (mp_pbSave->isEnabled())
        saveData();
    close();
}

void WndZone::closeEvent(QCloseEvent* e)
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::closeEvent( QCloseEvent* ) called.");
#endif

    if (mp_pbSave->isEnabled()) {
        if (KreatorSettings::instance().applyWithoutAsking()) {
            saveData();
            e->accept();
        } else {
            switch (QMessageBox::question(this, TS::MESSAGE_BOX_QUESTION, TS::MESSAGE_ASK_SAVE,
                                          trUtf8("Si"), trUtf8("No"), trUtf8("Annulla"), 1, 2)) {
                case 0:
                    saveData();
                    e->accept();
                    break;
                case 1:
                    e->accept();
                    break;
                default:
                    e->ignore();
                    break;
            }
        }
    } else
        e->accept();

    if (e->isAccepted()) {
        KreatorSettings::instance().saveGuiStatus("ZoneWindow", this);
        KreatorSettings::instance().saveButtonGroupStatus("ZoneCommandFilters", mp_bgFilters);
    }
}

QTreeWidgetItem* WndZone::selectedItem()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::selectedItem() called.");
#endif
    QList<QTreeWidgetItem*> wiList = mp_twZoneCommands->selectedItems();
    return wiList.empty() ? 0 : wiList.first();
}

void WndZone::addNewEmptyLine()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::addNewCommandLine() called.");
#endif
    QTreeWidgetItem* item = selectedItem();
    if (!item) {
        QMessageBox::warning(this, TS::MESSAGE_BOX_WARNING,
                             trUtf8("E' necessario selezionare una linea.\nIl nuovo comando sara' inserito prima della linea selezionata."));
        return;
    }

    int iLine = Utils::string2vnumber(item->text(0));
    m_zoneCommands.addCommand(iLine);
    refreshView();
    somethingChanged();
}

void WndZone::removeCommandLine()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::removeCommandLine() called.");
#endif
    QTreeWidgetItem* item = selectedItem();
    if (!item) {
        QMessageBox::warning(this, TS::MESSAGE_BOX_WARNING,
                             trUtf8("E' necessario selezionare la linea che si vuole eliminare."));
        return;
    }

    int iLine = Utils::string2vnumber(item->text(0));
    m_zoneCommands.removeCommand(iLine);
    refreshView();
    somethingChanged();
}

void WndZone::editCommandLine()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::editCommandLine() called.");
#endif
    QTreeWidgetItem* item = selectedItem();
    if (!item) {
        QMessageBox::warning(this, TS::MESSAGE_BOX_WARNING,
                             trUtf8("E' necessario selezionare prima una linea per modificare."));
        return;
    }

    int iLine = Utils::string2vnumber(item->text(0));

    if (!m_zoneCommands.hasCommand(iLine)) {
        QMessageBox::warning(this, TS::MESSAGE_BOX_WARNING,
                             trUtf8("Alla linea selezionata non e' piu' associato un comando di zona."));
        return;
    }

    WndZoneCommand *pWndZoneCommand = new WndZoneCommand(m_area, m_zoneCommands, m_zoneCommands.command(iLine), this);
    connect(pWndZoneCommand, SIGNAL(dataSaved()), this, SLOT(somethingChanged()));
    pWndZoneCommand->show();
    pWndZoneCommand->setFixedSize(pWndZoneCommand->size());
    pWndZoneCommand->exec();
    disconnect(pWndZoneCommand, SIGNAL(dataSaved()), this, SLOT(somethingChanged()));
    delete pWndZoneCommand;

    item->setText(1, m_zoneCommands.command(iLine).toString(true));
}

void WndZone::lineSelected()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::lineSelected( QListViewItem* ) called.");
#endif
    QTreeWidgetItem* item = selectedItem();
    if (!item)
        return;

    m_lastLineSelected = Utils::string2vnumber(item->text(0));
    if (!m_zoneCommands.hasCommand(m_lastLineSelected)) {
        qWarning("Alla linea selezionata non e' piu' associato un comando di zona.");
        return;
    }

    ZoneCommand zc = m_zoneCommands.command(m_lastLineSelected);
    mp_actUpdateMaxInits->setEnabled(zc.hasMaxField());
}

void WndZone::editFlags()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::editFlags( int ) called.");
#endif
    BitVector vFlags = m_zone.newFlags();
    BitVector vRetFlags = SelectObject::zoneFlags(vFlags, this);
    if (vRetFlags == vFlags)
        return;
    m_zone.setNewFlags(vRetFlags);
    refreshFlags();
    somethingChanged();
}

void WndZone::refreshFlags()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::refreshFlags() called.");
#endif
    mp_leFlags->setText(Utils::bitvector2string(m_zone.newFlags(), Eleuconf::getAllFlagsCaption(Eleuconf::zoneFlags)).toLower());
}

void WndZone::keyPressEvent(QKeyEvent* e)
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::keyPressEvent( QKeyEvent* ) called.");
#endif
    if (mp_leFilter->hasFocus() && (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return))
        e->ignore();
    else
        QDialog::keyPressEvent(e);
}

void WndZone::copyCommandLine()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::copyCommandLine() called.");
#endif
    QTreeWidgetItem* item = selectedItem();
    if (!item)
        return;

    int iLine = Utils::string2vnumber(item->text(0));

    if (!m_zoneCommands.hasCommand(iLine)) {
        QMessageBox::warning(this, TS::MESSAGE_BOX_WARNING,
                             trUtf8("Alla linea selezionata non e' piu' associato un comando di zona."));
        return;
    }

    ZoneCommandGroup zcg;
    zcg.setParent(m_zoneCommands.command(iLine));
    for (int i = 0; i < item->childCount(); i++) {
        int son_line = Utils::string2vnumber(item->child(i)->text(0));
        if (m_zoneCommands.hasCommand(son_line))
            zcg.addSon(m_zoneCommands.command(son_line));
    }

    m_commandGroupCopied = zcg;
    mp_actPaste->setEnabled(true);
}

void WndZone::pasteCommandLine()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::pasteCommandLine() called.");
#endif

    if (m_commandGroupCopied.isNull())
        return;

    QTreeWidgetItem* item = selectedItem();
    if (!item)
        return;

    int iLine = Utils::string2vnumber(item->text(0));

    ZoneCommandGroup zcg = m_commandGroupCopied;
    ZoneCommand zc = m_commandGroupCopied.parent();
    zc.setId(iLine);
    zcg.setParent(zc);

    m_zoneCommands.addCommandGroup(zcg);
    refreshView();
    somethingChanged();
}

void WndZone::cutCommandLine()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::cutCommandLine() called.");
#endif
    copyCommandLine();
    removeCommandLine();
}

void WndZone::updateMaxInits()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndZone::updateMaxInits() called.");
#endif

    QTreeWidgetItem* item = selectedItem();
    if (!item)
        return;

    int iLine = Utils::string2vnumber(item->text(0));

    if (!m_zoneCommands.hasCommand(iLine)) {
        QMessageBox::warning(this, TS::MESSAGE_BOX_WARNING,
                             trUtf8("Alla linea selezionata non e' piu' associato un comando di zona."));
        return;
    }

    ZoneCommand zc = m_zoneCommands.command(iLine);
    if (!zc.hasMaxField()) {
        QMessageBox::warning(this, TS::MESSAGE_BOX_WARNING,
                             trUtf8("Alla linea selezionata non e' piu' associato un comando con il campo per il maxaggio."));
        return;
    }

    QString sName = "";
    int iCountInits = 0;
    int iUpdate = 0;

    if (zc.hasMobField()) {
        sName = m_area.mobName(zc.argument(ZoneCommand::Argument1));
        iCountInits = m_zoneCommands.countMobs(zc.argument(ZoneCommand::Argument1));
    } else {
        sName = m_area.itemName(zc.argument(ZoneCommand::Argument1));
        iCountInits = m_zoneCommands.countItems(zc.argument(ZoneCommand::Argument1));
    }

    QString sTxt = "";
    sTxt.sprintf("Si vuole aggiornare tutti i maxaggi per l'entita' #%ld (%s)?\n"
                 "%ld = maxaggio del comando selezionato\n"
                 "%d = numero totale inizializzazioni",
                 zc.argument(ZoneCommand::Argument1), qPrintable(sName),
                 zc.argument(ZoneCommand::Argument2), iCountInits);

    switch (QMessageBox::question(this, TS::MESSAGE_BOX_QUESTION, sTxt,
                                  QString("Aggiorna a %1").arg(zc.argument(ZoneCommand::Argument2)),
                                  QString("Aggiorna a %1").arg(iCountInits), QString(trUtf8("Annulla")),
                                  2, 2)) {
        case 0:
            iUpdate = zc.argument(ZoneCommand::Argument2);
            break;
        case 1:
            iUpdate = iCountInits;
            break;
        default:
            iUpdate = -1;
    }

    if (iUpdate > 0) {
        int iUpdateCount = 0;
        if (zc.hasMobField())
            iUpdateCount = m_zoneCommands.setMaxMobs(zc.argument(ZoneCommand::Argument1), iUpdate);
        else
            iUpdateCount = m_zoneCommands.setMaxItems(zc.argument(ZoneCommand::Argument1), iUpdate);

        QMessageBox::information(this, TS::MESSAGE_BOX_INFORMATION,
                                 trUtf8("Sono stati Aggiornati %1 riferimenti.").arg(iUpdateCount));

        refreshView();
        somethingChanged();
    }
}
