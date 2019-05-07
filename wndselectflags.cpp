#include "wndselectflags.h"
#include <QPushButton>
#include <QString>
#include <QCloseEvent>
#include <QMessageBox>
#include <QAbstractButton>
#include <QCheckBox>
#include "constantname.h"
#include "config.h"
#include "bit.h"


WndSelectFlags::WndSelectFlags(QList<Flags> flags, QWidget* parent)
        : QDialog(parent), Ui::GuiSelectFlags(),
        m_okPressed(false)
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::WndSelectFlags( QList<Flags>, QWidget* ) called.");
#endif
    m_bv = 0;
    m_oldbv = 0;

    init();

    QAbstractButton* pButton = NULL;

    for (int i = 0; i < flags.size(); i++) {
        QCheckBox *cbTmp = new QCheckBox();
        cbTmp->setTristate(true);
        gridLayout1->addWidget(cbTmp, (i / 4), (i - (i / 4)*4), 1, 1);
        mp_bgFlags->addButton(cbTmp, i);
        pButton = mp_bgFlags->button(i);
        if (!pButton) {
            qWarning("WndSelectFlags::WndSelectFlags( BitVector bv, QList<Flags> flags, QWidget* parent ) unable to find button id #%d.", i);
            break;
        }

        if (flags.at(i).enabled)
            pButton->setEnabled(true);
        else
            pButton->setEnabled(false);

        pButton->setText(flags.at(i).name);
        pButton->setToolTip(flags.at(i).hint);

    }

    loadData();
}

WndSelectFlags::WndSelectFlags(BitVector bv, const QStringList &list, QWidget* parent)
        : QDialog(parent), Ui::GuiSelectFlags(),
        m_bv(bv),
        m_oldbv(bv),
        m_okPressed(false)
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::WndSelectFlags( BitVector, QWidget* ) called.");
#endif

    init();

    QAbstractButton* pButton = NULL;

    for (int i = 0; i < list.size(); i++) {
        QCheckBox *cbTmp = new QCheckBox();
        gridLayout1->addWidget(cbTmp, (i / 4), (i - (i / 4)*4), 1, 1);
        mp_bgFlags->addButton(cbTmp, i);
        pButton = mp_bgFlags->button(i);
        if (!pButton) {
            qWarning("WndSelectFlags::WndSelectFlags( BitVector, QWidget*, const char* ) unable to find button id #%d.", i);
            break;
        }

        pButton->setEnabled(true);
        pButton->setText(list.at(i).toLower());

    }

    loadData();
}

WndSelectFlags::WndSelectFlags(BitVector bv, QList<Flags> flags, QWidget* parent)
        : QDialog(parent), Ui::GuiSelectFlags(),
        m_bv(bv),
        m_oldbv(bv),
        m_okPressed(false)
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::WndSelectFlags( BitVector bv, QList<Flags> flags, QWidget* parent ) called.");
#endif

    init();

    QAbstractButton* pButton = NULL;

    for (int i = 0; i < flags.size(); i++) {
        QCheckBox *cbTmp = new QCheckBox();
        gridLayout1->addWidget(cbTmp, (i / 4), (i - (i / 4)*4), 1, 1);
        mp_bgFlags->addButton(cbTmp, i);
        pButton = mp_bgFlags->button(i);
        if (!pButton) {
            qWarning("WndSelectFlags::WndSelectFlags( BitVector bv, QList<Flags> flags, QWidget* parent ) unable to find button id #%d.", i);
            break;
        }

        if (flags.at(i).enabled)
            pButton->setEnabled(true);
        else
            pButton->setEnabled(false);

        pButton->setText(flags.at(i).name);
        pButton->setToolTip(flags.at(i).hint);

    }

    loadData();
}

WndSelectFlags::~WndSelectFlags()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::~WndSelectFlags() called.");
#endif
}

void WndSelectFlags::init()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::init() called.");
#endif
    setupUi(this);

    setWindowTitle("Seleziona i flags");


    mp_bgFlags = new MyQt::ButtonGroup(this);

    /*

    mp_bgFlags->addButton( mp_cb0, 0 );
    mp_bgFlags->addButton( mp_cb1, 1 );
    mp_bgFlags->addButton( mp_cb2, 2 );
    mp_bgFlags->addButton( mp_cb3, 3 );
    mp_bgFlags->addButton( mp_cb4, 4 );
    mp_bgFlags->addButton( mp_cb5, 5 );
    mp_bgFlags->addButton( mp_cb6, 6 );
    mp_bgFlags->addButton( mp_cb7, 7 );
    mp_bgFlags->addButton( mp_cb8, 8 );
    mp_bgFlags->addButton( mp_cb9, 9 );
    mp_bgFlags->addButton( mp_cb10, 10 );
    mp_bgFlags->addButton( mp_cb11, 11 );
    mp_bgFlags->addButton( mp_cb12, 12 );
    mp_bgFlags->addButton( mp_cb13, 13 );
    mp_bgFlags->addButton( mp_cb14, 14 );
    mp_bgFlags->addButton( mp_cb15, 15 );
    mp_bgFlags->addButton( mp_cb16, 16 );
    mp_bgFlags->addButton( mp_cb17, 17 );
    mp_bgFlags->addButton( mp_cb18, 18 );
    mp_bgFlags->addButton( mp_cb19, 19 );
    mp_bgFlags->addButton( mp_cb20, 20 );
    mp_bgFlags->addButton( mp_cb21, 21 );
    mp_bgFlags->addButton( mp_cb22, 22 );
    mp_bgFlags->addButton( mp_cb23, 23 );
    mp_bgFlags->addButton( mp_cb24, 24 );
    mp_bgFlags->addButton( mp_cb25, 25 );
    mp_bgFlags->addButton( mp_cb26, 26 );
    mp_bgFlags->addButton( mp_cb27, 27 );
    mp_bgFlags->addButton( mp_cb28, 28 );
    mp_bgFlags->addButton( mp_cb29, 29 );
    mp_bgFlags->addButton( mp_cb30, 30 );
    mp_bgFlags->addButton( mp_cb31, 31 );

    */
    connect(mp_pbSave, SIGNAL(clicked()), this, SLOT(saveData()));
    connect(mp_pbRestore, SIGNAL(clicked()), this, SLOT(restoreData()));
    connect(mp_pbOk, SIGNAL(clicked()), this, SLOT(saveAndClose()));
    connect(mp_pbCancel, SIGNAL(clicked()), this, SLOT(close()));

    connect(mp_bgFlags, SIGNAL(buttonClicked(int)), this, SLOT(somethingChanged()));
}

void WndSelectFlags::refreshTitle()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::refreshTitle() called.");
#endif
    QString sTitle = windowTitle();
    if (mp_pbSave->isEnabled() && !sTitle.contains("[modificati]"))
        sTitle.append(QString(" [modificati]"));
    setWindowTitle(sTitle);
}

void WndSelectFlags::refreshPanel()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::refreshPanel() called.");
#endif

    QAbstractButton* pButton = NULL;
    for (int i = 0; i < mp_bgFlags->buttons().size(); i++) {
        pButton = mp_bgFlags->button(i);
        if (!pButton) {
            qWarning("WndSelectFlags::refreshPanel() unable to find button id #%d.", i);
            break;
        }

        if (pButton->isEnabled())
            pButton->setChecked(Bit::has(m_bv, i));
    }
}

void WndSelectFlags::somethingChanged()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::somethingChanged() called.");
#endif
    mp_pbSave->setEnabled(true);
    refreshTitle();
}

void WndSelectFlags::restoreData()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::restoreData() called.");
#endif
    m_bv = m_oldbv;
    loadData();
}

void WndSelectFlags::loadData()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::loadData() called.");
#endif
    refreshPanel();
    mp_pbSave->setEnabled(false);
    refreshTitle();
}

void WndSelectFlags::saveData()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::saveData() called.");
#endif
    QAbstractButton* pButton = NULL;
    m_bv = 0;
    for (int i = 0; i < mp_bgFlags->buttons().size(); i++) {
        pButton = mp_bgFlags->button(i);
        if (!pButton) {
            qWarning("WndSelectFlags::saveData() unable to find button id #%d.", i);
            break;
        }

        if (pButton->isEnabled() && pButton->isChecked())
            Bit::set(&m_bv, i);
    }

    mp_pbSave->setEnabled(false);
    refreshTitle();
}

void WndSelectFlags::saveAndClose()
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::saveAndClose() called.");
#endif
    if (mp_pbSave->isEnabled())
        saveData();
    m_okPressed = true;
    close();
}

void WndSelectFlags::closeEvent(QCloseEvent* e)
{
#if defined( KREATOR_DEBUG )
    qDebug("WndSelectFlags::closeEvent( QCloseEvent* ) called.");
#endif

    if (mp_pbSave->isEnabled()) {
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
    } else
        e->accept();

}
