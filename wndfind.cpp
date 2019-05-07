#include "wndfind.h"
#include <QTreeWidget>
#include <QPushButton>
#include <QHeaderView>
#include <QLineEdit>
#include <QMessageBox>
#include <QApplication>
#include <QCloseEvent>
#include <QLabel>
#include "config.h"
#include "kreatorsettings.h"
#include "utils.h"


WndFind::WndFind( const Area& ref_area, QWidget* parent )
  : QDialog( parent ), Ui::GuiFind(), m_area( ref_area ), m_keywords( "" )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndFind::WndFind( Area&, QWidget* ) called." );
#endif
  init();
}

WndFind::~WndFind()
{

}

void WndFind::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndFind::init() called." );
#endif

  setupUi( this );
  setWindowTitle( trUtf8( "Cerca - TS Kreator" ) );
  KreatorSettings::instance().loadGuiStatus( "FindWindow", this );

  mp_bgFilters = new MyQt::ButtonGroup( this );
  mp_bgFilters->addButton( mp_cbRooms, 1 );
  mp_bgFilters->addButton( mp_cbMobs, 2 );
  mp_bgFilters->addButton( mp_cbItems, 3 );
  mp_bgFilters->addButton( mp_cbCaseSensitive, 4 );
  KreatorSettings::instance().loadButtonGroupStatus( "FindFilters", mp_bgFilters );

  mp_twList->setAlternatingRowColors( true );
  mp_twList->setColumnCount( 5 );
  mp_twList->hideColumn( 1 );
  QStringList labels;
  labels << trUtf8( "Tipo" ) << trUtf8( "ObjectID" ) << trUtf8( "Dove" ) << trUtf8( "VNumber" ) << trUtf8( "Nome" );
  mp_twList->setHeaderLabels( labels );
  QHeaderView* pHeader = mp_twList->header();
  pHeader->resizeSection( 0, 100 );
  pHeader->resizeSection( 2, 200 );
  pHeader->resizeSection( 3, 100 );
  pHeader->setStretchLastSection( true );

  connect( mp_twList, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ),
    this, SLOT( showObject( QTreeWidgetItem*, int ) ) );
  connect( mp_leKeywords, SIGNAL( textChanged( const QString& ) ), this, SLOT( somethingChanged() ) );
  connect( mp_bgFilters, SIGNAL( buttonClicked( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_pbFind, SIGNAL( clicked() ), this, SLOT( find() ) );
  connect( mp_pbClose, SIGNAL( clicked() ), this, SLOT( close() ) );
}

void WndFind::find()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndFind::find() called." );
#endif
  m_keywords = mp_leKeywords->text().simplified();

  if( m_keywords.isEmpty() )
  {
    QMessageBox::warning( this, TS::MESSAGE_BOX_WARNING,
       TS::MESSAGE_WARNING_FIELD_IS_EMPTY.arg( trUtf8( "Parole da cercare" ) ) );
    mp_leKeywords->setFocus();
    return;
  }

  QApplication::setOverrideCursor( Qt::WaitCursor );
  QApplication::processEvents();

  mp_twList->clear();
  int iCount = 0;

  if( mp_cbRooms->isChecked() )
    iCount += findInRooms();
  if( mp_cbMobs->isChecked() )
    iCount += findInMobs();
  if( mp_cbItems->isChecked() )
    iCount += findInItems();

  mp_pbFind->setEnabled( false );

  mp_labelReport->setText( trUtf8( "Trovate %1 occorrenze." ).arg( iCount ) );

  QApplication::restoreOverrideCursor();
  QApplication::processEvents();
}

int WndFind::findInRooms()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndFind::findInRooms() called." );
#endif
  int iCount = 0;
  Qt::CaseSensitivity cs = mp_cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;

  Area::rooms_const_iterator itr = m_area.rooms().begin();
  while( itr != m_area.rooms().end() )
  {
    if( (*itr).name().contains( m_keywords, cs ) )
    {
      addRoom( trUtf8( "Nome" ), (*itr).vnumber(), (*itr).name() );
      iCount++;
    }

    if( (*itr).description().contains( m_keywords, cs ) )
    {
      addRoom( trUtf8( "Descrizione" ), (*itr).vnumber(), (*itr).name() );
      iCount++;
    }

    if( !(*itr).extraDescriptions().empty() )
    {
      Room::extra_description_const_iterator ited = (*itr).extraDescriptions().begin();
      while( ited != (*itr).extraDescriptions().end() )
      {
        if( (*ited).keys().contains( m_keywords, cs ) )
        {
          addRoom( trUtf8( "Extra: %1" ).arg( (*ited).keys() ), (*itr).vnumber(), (*itr).name() );
          iCount++;
        }
        if( (*ited).description().contains( m_keywords, cs ) )
        {
          addRoom( trUtf8( "Extra: %1 (desc)" ).arg( (*ited).keys() ), (*itr).vnumber(), (*itr).name() );
          iCount++;
        }
        ++ited;
      }
    }

    if( !(*itr).exits().empty() )
    {
      Room::exits_const_iterator itex = (*itr).exits().begin();
      while( itex != (*itr).exits().end() )
      {
        if( (*itex).keys().contains( m_keywords, cs ) )
        {
          addRoom( trUtf8( "Uscita: %1 (keys)" ).arg( (*itex).name() ), (*itr).vnumber(), (*itr).name() );
          iCount++;
        }
        if( (*itex).description().contains( m_keywords, cs ) )
        {
          addRoom( trUtf8( "Uscita: %1 (desc)" ).arg( (*itex).name() ), (*itr).vnumber(), (*itr).name() );
          iCount++;
        }
        ++itex;
      }
    }

    ++itr;
  }

  return iCount;
}

int WndFind::findInMobs()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndFind::findInMobs() called." );
#endif

  int iCount = 0;
  Qt::CaseSensitivity cs = mp_cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;

  Area::mobs_const_iterator itm = m_area.mobs().begin();
  while( itm != m_area.mobs().end() )
  {
    if( (*itm).name().contains( m_keywords, cs ) )
    {
      addMob( trUtf8( "Nome" ), (*itm).vnumber(), (*itm).shortDescription() );
      iCount++;
    }

    if( (*itm).shortDescription().contains( m_keywords, cs ) )
    {
      addMob( trUtf8( "Short" ), (*itm).vnumber(), (*itm).shortDescription() );
      iCount++;
    }

    if( (*itm).longDescription().contains( m_keywords, cs ) )
    {
      addMob( trUtf8( "Long" ), (*itm).vnumber(), (*itm).shortDescription() );
      iCount++;
    }

    if( (*itm).roomSound().contains( m_keywords, cs ) )
    {
      addMob( trUtf8( "Suono nella stanza" ), (*itm).vnumber(), (*itm).shortDescription() );
      iCount++;
    }

    if( (*itm).nearSound().contains( m_keywords, cs ) )
    {
      addMob( trUtf8( "Suono nelle stanze vicine" ), (*itm).vnumber(), (*itm).shortDescription() );
      iCount++;
    }

    ++itm;
  }
  return iCount;
}

int WndFind::findInItems()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndFind::findInItems() called." );
#endif

  int iCount = 0;
  Qt::CaseSensitivity cs = mp_cbCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;

  Area::items_const_iterator iti = m_area.items().begin();
  while( iti != m_area.items().end() )
  {
    if( (*iti).name().contains( m_keywords, cs ) )
    {
      addItem( trUtf8( "Nome" ), (*iti).vnumber(), (*iti).name() );
      iCount++;
    }

    if( (*iti).shortDescription().contains( m_keywords, cs ) )
    {
      addItem( trUtf8( "Short" ), (*iti).vnumber(), (*iti).shortDescription() );
      iCount++;
    }

    if( (*iti).longDescription().contains( m_keywords, cs ) )
    {
      addItem( trUtf8( "Long" ), (*iti).vnumber(), (*iti).shortDescription() );
      iCount++;
    }

    if( (*iti).actionDescription().contains( m_keywords, cs ) )
    {
      addItem( trUtf8( "Descrizione Azione" ), (*iti).vnumber(), (*iti).shortDescription() );
      iCount++;
    }

    if( !(*iti).extraDescriptions().empty() )
    {
      Item::extra_description_const_iterator ited = (*iti).extraDescriptions().begin();
      while( ited != (*iti).extraDescriptions().end() )
      {
        if( (*ited).keys().contains( m_keywords, cs ) )
        {
          addItem( trUtf8( "Extra: %1" ).arg( (*ited).keys() ), (*iti).vnumber(), (*iti).shortDescription() );
          iCount++;
        }
        if( (*ited).description().contains( m_keywords, cs ) )
        {
          addItem( trUtf8( "Extra: %1 (desc)" ).arg( (*ited).keys() ), (*iti).vnumber(), (*iti).shortDescription() );
          iCount++;
        }
        ++ited;
      }
    }
    ++iti;
  }

  return iCount;
}

void WndFind::addRoom( const QString& where, VNumber vnum, const QString& room_name )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndFind::addRoom( const QString&, VNumber, const QString& ) called." );
#endif
  QTreeWidgetItem *item = new QTreeWidgetItem( mp_twList );
  item->setText( 0, trUtf8( "Stanza" ) );
  item->setText( 1, QString::number( ObjectData::Object_Room ) );
  item->setText( 2, where );
  item->setText( 3, Utils::vnumber2string( vnum ) );
  item->setText( 4, room_name );

}

void WndFind::addMob( const QString& where, VNumber vnum, const QString& mob_name )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndFind::addMob( const QString&, VNumber, const QString& ) called." );
#endif
  QTreeWidgetItem *item = new QTreeWidgetItem( mp_twList );
  item->setText( 0, trUtf8( "Mob" ) );
  item->setText( 1, QString::number( ObjectData::Object_Mob ) );
  item->setText( 2, where );
  item->setText( 3, Utils::vnumber2string( vnum ) );
  item->setText( 4, mob_name );
}

void WndFind::addItem( const QString& where, VNumber vnum, const QString& item_name )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndFind::addItem( const QString&, VNumber, const QString& ) called." );
#endif
  QTreeWidgetItem *item = new QTreeWidgetItem( mp_twList );
  item->setText( 0, trUtf8( "Oggetto" ) );
  item->setText( 1, QString::number( ObjectData::Object_Item ) );
  item->setText( 2, where );
  item->setText( 3, Utils::vnumber2string( vnum ) );
  item->setText( 4, item_name );
}

void WndFind::showObject( QTreeWidgetItem* item, int )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndFind::showObject( QTreeWidgetItem*, int ) called." );
#endif
  if( !item )
    return;

  int object_id = item->text( 1 ).toInt();
  VNumber vnum =  Utils::string2vnumber( item->text( 3 ) );
  emit( openEditWindow( object_id, vnum ) );
}

void WndFind::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndFind::closeEvent( QCloseEvent* ) called." );
#endif
  e->accept();

  if( e->isAccepted() )
  {
    KreatorSettings::instance().saveGuiStatus( "FindWindow", this );
    KreatorSettings::instance().saveButtonGroupStatus( "FindFilters", mp_bgFilters );
  }
}

void WndFind::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndFind::somethingChanged() called." );
#endif
  mp_pbFind->setEnabled( true );
}
