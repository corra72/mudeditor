#include "wndzonecommand.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
#include "validator.h"
#include "config.h"
#include "selectobject.h"
#include "guiutils.h"


WndZoneCommand::WndZoneCommand( const Area& ref_area, ZoneCommandList& ref_zone_cmd_list, const ZoneCommand& zc, QWidget* parent )
  : QDialog( parent ), GuiZoneCommand(),
    m_area( ref_area ),
    m_zoneCommandList( ref_zone_cmd_list ),
    m_command( zc ),
    m_prevTypeId( -1 )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZoneCommand::WndZoneCommand( const Area&, Zone&, const ZoneCommand&, QWidget* ) called." );
#endif
  init();
  loadData();
  mp_comboType->setFocus();
  setModal(true);
}

WndZoneCommand::~WndZoneCommand()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZoneCommand::~WndZoneCommand() called." );
#endif
}

void WndZoneCommand::init()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZone::init() called." );
#endif

  setupUi( this );

  mp_tbArgument1->setIcon( TS::GetEditIcon() );
  mp_tbArgument2->setIcon( TS::GetEditIcon() );
  mp_tbArgument3->setIcon( TS::GetEditIcon() );
  mp_tbArgument4->setIcon( TS::GetEditIcon() );

  mp_comboType->clear();
  mp_comboType->insertItem( 0, "Carica Mob" );
  mp_comboType->insertItem( 1, "Carica Seguace" );
  mp_comboType->insertItem( 2, "Assegna Paura del Mob" );
  mp_comboType->insertItem( 3, "Assegna Odio verso il Mob" );
  mp_comboType->insertItem( 4, "Metti oggetto" );
  mp_comboType->insertItem( 5, "Indossa oggetto" );
  mp_comboType->insertItem( 6, "Dai oggetto" );
  mp_comboType->insertItem( 7, "Carica oggetto" );
  mp_comboType->insertItem( 8, "Inizializza porta" );
  mp_comboType->insertItem( 9, "Commento" );

  mp_labelLine->setDisabled( true );
  mp_leLine->setReadOnly( true );

  mp_leArgument1->setValidator( Validator::vnumber() );
  mp_leArgument2->setValidator( Validator::vnumber() );
  mp_leArgument3->setValidator( Validator::vnumber() );
  mp_leArgument4->setValidator( Validator::vnumber() );

  connect( mp_pbSave, SIGNAL( clicked() ), this, SLOT( saveData() ) );
  connect( mp_pbRestore, SIGNAL( clicked() ), this, SLOT( restoreData() ) );
  connect( mp_pbOk, SIGNAL( clicked() ), this, SLOT( saveAndClose() ) );
  connect( mp_pbCancel, SIGNAL( clicked() ), this, SLOT( close() ) );

  connect( mp_comboType, SIGNAL( activated( int ) ), this, SLOT( typeSelected( int ) ) );
  connect( mp_leArgument1, SIGNAL( textChanged( const QString& ) ),
    this, SLOT( somethingChanged() ) );
  connect( mp_leArgument2, SIGNAL( textChanged( const QString& ) ),
    this, SLOT( somethingChanged() ) );
  connect( mp_leArgument3, SIGNAL( textChanged( const QString& ) ),
    this, SLOT( somethingChanged() ) );
  connect( mp_leArgument4, SIGNAL( textChanged( const QString& ) ),
    this, SLOT( somethingChanged() ) );
  connect( mp_comboSeparator, SIGNAL( activated( int ) ), this, SLOT( somethingChanged() ) );
  connect( mp_pbAutoComment, SIGNAL( clicked() ), this, SLOT( generateComment() ) );
  connect( mp_teComment, SIGNAL( textChanged() ), this, SLOT( somethingChanged() ) );
  connect( mp_tbArgument1, SIGNAL( clicked() ), this, SLOT( editArgument1() ) );
  connect( mp_tbArgument2, SIGNAL( clicked() ), this, SLOT( editArgument2() ) );
  connect( mp_tbArgument3, SIGNAL( clicked() ), this, SLOT( editArgument3() ) );
  connect( mp_tbArgument4, SIGNAL( clicked() ), this, SLOT( editArgument4() ) );
}

void WndZoneCommand::refreshPanel()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZoneCommand::refreshPanel() called." );
#endif

  mp_leLine->setText( QString::number( m_command.id() ) );

  bool has_arg_1 = m_command.hasArgument( ZoneCommand::Argument1 );
  bool has_arg_2 = m_command.hasArgument( ZoneCommand::Argument2 );
  bool has_arg_3 = m_command.hasArgument( ZoneCommand::Argument3 );
  bool has_arg_4 = m_command.hasArgument( ZoneCommand::Argument4 );

  mp_labelArgument1->setEnabled( has_arg_1 );
  mp_leArgument1->setEnabled( has_arg_1 );
  mp_tbArgument1->setEnabled( has_arg_1 );
  mp_labelArgument2->setEnabled( has_arg_2 );
  mp_leArgument2->setEnabled( has_arg_2 );
  mp_tbArgument2->setEnabled( has_arg_2 );
  mp_labelArgument3->setEnabled( has_arg_3 );
  mp_leArgument3->setEnabled( has_arg_3 );
  mp_tbArgument3->setEnabled( has_arg_3 );
  mp_labelArgument4->setEnabled( has_arg_4 );
  mp_leArgument4->setEnabled( has_arg_4 );
  mp_tbArgument4->setEnabled( has_arg_4 );

  if( has_arg_1 )
    mp_leArgument1->setText( QString::number( m_command.argument( ZoneCommand::Argument1 ) ) );
  else
    mp_leArgument1->setText( "" );

  if( has_arg_2 )
    mp_leArgument2->setText( QString::number( m_command.argument( ZoneCommand::Argument2 ) ) );
  else
    mp_leArgument2->setText( "" );

  if( has_arg_3 )
    mp_leArgument3->setText( QString::number( m_command.argument( ZoneCommand::Argument3 ) ) );
  else
    mp_leArgument3->setText( "" );

  if( has_arg_4 )
    mp_leArgument4->setText( QString::number( m_command.argument( ZoneCommand::Argument4 ) ) );
  else
    mp_leArgument4->setText( "" );

  char c = m_command.type().unicode();
  bool bIsComment = false;

  switch( c )
  {
  case 'M':
    mp_comboType->setCurrentIndex( 0 );
    // M <if-flag> <mob vnum> <max #> <room vnum>
    mp_labelArgument1->setText( trUtf8( "VNumber" ) );
    mp_labelArgument2->setText( trUtf8( "Numero Massimo" ) );
    mp_tbArgument2->setEnabled( false );
    mp_labelArgument3->setText( trUtf8( "nella Stanza" ) );
    mp_labelArgument4->setText( trUtf8( "" ) );
    break;
  case 'C':
    mp_comboType->setCurrentIndex( 1 );
    // C <if-flag> <mob vnum> <max #> <room vnum>
    mp_labelArgument1->setText( trUtf8( "VNumber" ) );
    mp_labelArgument2->setText( trUtf8( "Numero Massimo" ) );
    mp_tbArgument2->setEnabled( false );
    mp_labelArgument3->setText( trUtf8( "nella Stanza" ) );
    mp_labelArgument4->setText( trUtf8( "" ) );
    break;
  case 'F':
    mp_comboType->setCurrentIndex( 2 );
    // F <if-flag> <field> <operator>
    mp_labelArgument1->setText( trUtf8( "verso" ) );
    mp_labelArgument2->setText( trUtf8( "tipo" ) );
    mp_labelArgument3->setText( trUtf8( "" ) );
    mp_labelArgument4->setText( trUtf8( "" ) );
    break;
  case 'H':
    mp_comboType->setCurrentIndex( 3 );
    // H <if-flag> <field> <operator>
    mp_labelArgument1->setText( trUtf8( "verso" ) );
    mp_labelArgument2->setText( trUtf8( "tipo" ) );
    mp_labelArgument3->setText( trUtf8( "" ) );
    mp_labelArgument4->setText( trUtf8( "" ) );
    break;
  case 'P':
    mp_comboType->setCurrentIndex( 4 );
    // P <if-flag> <loaded object vnum> <max #> <container vnum>
    mp_labelArgument1->setText( trUtf8( "VNumber" ) );
    mp_labelArgument2->setText( trUtf8( "Numero Massimo" ) );
    mp_tbArgument2->setEnabled( false );
    mp_labelArgument3->setText( trUtf8( "nel Contenitore" ) );
    mp_labelArgument4->setText( trUtf8( "" ) );
    break;
  case 'E':
    mp_comboType->setCurrentIndex( 5 );
    // E <if-flag> <object vnum> <max #> <equipment position>
    mp_labelArgument1->setText( trUtf8( "VNumber" ) );
    mp_labelArgument2->setText( trUtf8( "Numero Massimo" ) );
    mp_tbArgument2->setEnabled( false );
    mp_labelArgument3->setText( trUtf8( "nella Posizione" ) );
    mp_labelArgument4->setText( trUtf8( "" ) );
    break;
  case 'G':
    mp_comboType->setCurrentIndex( 6 );
    // G <if-flag> <object vnum> <max #>
    mp_labelArgument1->setText( trUtf8( "VNumber" ) );
    mp_labelArgument2->setText( trUtf8( "Numero Massimo" ) );
    mp_labelArgument3->setText( trUtf8( "in Inventario" ) );
    mp_labelArgument4->setText( trUtf8( "" ) );
    break;
  case 'O':
    mp_comboType->setCurrentIndex( 7 );
    // O <if-flag> <object vnum> <max #> <room vnum> <max room #>
    mp_labelArgument1->setText( trUtf8( "VNumber" ) );
    mp_labelArgument2->setText( trUtf8( "Numero Massimo (Mondo)" ) );
    mp_tbArgument2->setEnabled( false );
    mp_labelArgument3->setText( trUtf8( "nella Stanza" ) );
    mp_labelArgument4->setText( trUtf8( "Numero Massimo (Stanza)" ) );
    mp_tbArgument4->setEnabled( false );
    break;
  case 'D':
    mp_comboType->setCurrentIndex( 8 );
    // D <if-flag> <room vnum> <exit #> <doorstate>
    mp_labelArgument1->setText( trUtf8( "della Stanza" ) );
    mp_labelArgument2->setText( trUtf8( "verso la direzione" ) );
    mp_labelArgument3->setText( trUtf8( "nello stato" ) );
    mp_labelArgument4->setText( trUtf8( "" ) );
    break;
  default:
    mp_comboType->setCurrentIndex( 9 );
    // Comment
    mp_labelArgument1->setText( trUtf8( "" ) );
    mp_labelArgument2->setText( trUtf8( "" ) );
    mp_labelArgument3->setText( trUtf8( "" ) );
    mp_labelArgument4->setText( trUtf8( "" ) );
    bIsComment = true;
  }

  int iSeparatorId = mp_comboSeparator->findText( m_command.commentSeparator() );
  if( iSeparatorId >= 0 )
    mp_comboSeparator->setCurrentIndex( iSeparatorId );
  else
    mp_comboSeparator->setCurrentIndex( 0 );

  mp_teComment->setPlainText( m_command.comment() );
  m_prevTypeId = mp_comboType->currentIndex();
}

void WndZoneCommand::refreshTitle()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZoneCommand::refreshTitle() called." );
#endif
  QString sTitle = "";
  sTitle.sprintf( "Comando di Zona #%ld", m_command.id() );
  if( mp_pbSave->isEnabled() )
    sTitle.append( QString( " [modificato]" ) );
  setWindowTitle( sTitle );
}

void WndZoneCommand::somethingChanged()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZoneCommand::somethingChanged() called." );
#endif
  mp_pbSave->setEnabled( true );
  refreshTitle();
}

void WndZoneCommand::restoreData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZoneCommand::restoreData() called." );
#endif
  m_command = m_zoneCommandList.command( m_command.id() );
  loadData();
}

void WndZoneCommand::loadData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZoneCommand::loadData() called." );
#endif
  refreshPanel();
  mp_pbSave->setEnabled( false );
  refreshTitle();
  mp_comboType->setFocus();
}

void WndZoneCommand::saveData()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZoneCommand::saveData() called." );
#endif


  if( mp_leArgument1->isEnabled() )
    m_command.setArgument( ZoneCommand::Argument1, mp_leArgument1->text().toInt() );
  if( mp_leArgument2->isEnabled() )
    m_command.setArgument( ZoneCommand::Argument2, mp_leArgument2->text().toInt() );
  if( mp_leArgument3->isEnabled() )
    m_command.setArgument( ZoneCommand::Argument3, mp_leArgument3->text().toInt() );
  if( mp_leArgument4->isEnabled() )
    m_command.setArgument( ZoneCommand::Argument4, mp_leArgument4->text().toInt() );

  m_command.setCommentSeparator( mp_comboSeparator->currentText() );

  m_command.setComment( mp_teComment->toPlainText() );

  m_zoneCommandList.removeCommand( m_command.id() );
  m_zoneCommandList.addCommand( m_command );

  mp_pbSave->setEnabled( false );
  refreshTitle();
  emit( dataSaved() );
}

void WndZoneCommand::saveAndClose()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZoneCommand::saveAndClose() called." );
#endif
  if( mp_pbSave->isEnabled() )
    {
      generateComment();
      saveData();
    }
  close();
}

void WndZoneCommand::closeEvent( QCloseEvent* e )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZoneCommand::closeEvent( QCloseEvent* ) called." );
#endif

  if( mp_pbSave->isEnabled() )
  {
    switch( QMessageBox::question( this, TS::MESSAGE_BOX_QUESTION, TS::MESSAGE_ASK_SAVE,
              trUtf8( "Si" ), trUtf8( "No" ), trUtf8( "Annulla" ), 1, 2 ) )
    {
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
  else
    e->accept();
}

void WndZoneCommand::typeSelected( int id )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZoneCommand::typeSelected( int ) called." );
#endif

  if( id == m_prevTypeId )
    return;

  switch( id )
  {
  case 0:
    m_command.setType( 'M' );
    break;
  case 1:
    m_command.setType( 'C' );
    break;
  case 2:
    m_command.setType( 'F' );
    break;
  case 3:
    m_command.setType( 'H' );
    break;
  case 4:
    m_command.setType( 'P' );
    break;
  case 5:
    m_command.setType( 'E' );
    break;
  case 6:
    m_command.setType( 'G' );
    break;
  case 7:
    m_command.setType( 'O' );
    break;
  case 8:
    m_command.setType( 'D' );
    break;
  default:
    m_command.setType( '*' );
  }

  somethingChanged();
  refreshPanel();
}

void WndZoneCommand::generateComment()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZone::generateComment() called." );
#endif

  ZoneCommand zcTmp = m_command;

  if( mp_leArgument1->isEnabled() )
    zcTmp.setArgument( ZoneCommand::Argument1, mp_leArgument1->text().toInt() );
  if( mp_leArgument2->isEnabled() )
    zcTmp.setArgument( ZoneCommand::Argument2, mp_leArgument2->text().toInt() );
  if( mp_leArgument3->isEnabled() )
    zcTmp.setArgument( ZoneCommand::Argument3, mp_leArgument3->text().toInt() );
  if( mp_leArgument4->isEnabled() )
    zcTmp.setArgument( ZoneCommand::Argument4, mp_leArgument4->text().toInt() );

  mp_teComment->setPlainText( m_area.generateComment( zcTmp ) );
  somethingChanged();
}

void WndZoneCommand::editArgument1()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZone::editArgument1() called." );
#endif
  editArgument( 1 );
}

void WndZoneCommand::editArgument2()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZone::editArgument2() called." );
#endif
  editArgument( 2 );
}

void WndZoneCommand::editArgument3()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZone::editArgument3() called." );
#endif
  editArgument( 3 );
}

void WndZoneCommand::editArgument4()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZone::editArgument4() called." );
#endif
  editArgument( 4 );
}

void WndZoneCommand::editArgument( int id )
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndZone::editArgument( int ) called." );
#endif

  QLineEdit *pLe = NULL;

  switch( id )
  {
  case 1:
    pLe = mp_leArgument1;
    break;
  case 2:
    pLe = mp_leArgument2;
    break;
  case 3:
    pLe = mp_leArgument3;
    break;
  case 4:
    pLe = mp_leArgument4;
    break;
  default:
    qWarning( "WndZone::editArgument( int ): Invalid ID selected." );
    return;
  }

  long numSelected = -1;
  long default_num = pLe->text().toLong();

  char c = m_command.type().unicode();

  switch( c )
  {
  case 'M':
  case 'C':
    {
      // M <if-flag> <mob vnum> <max #> <room vnum>
      // C <if-flag> <mob vnum> <max #> <room vnum>
      if( id == 1 )
        numSelected = SelectObject::mob( m_area, default_num, this );
      else if( id == 3 )
        numSelected = SelectObject::room( m_area, default_num, this );
      else
        return;
    }
    break;

  case 'F':
  case 'H':
    {
      // F <if-flag> <field> <operator>
      // H <if-flag> <field> <operator>
      if( id == 1 )
        numSelected = SelectObject::reactionVersus( default_num, this );
      else if( id == 2 )
        numSelected = SelectObject::reactionValue( m_area, mp_leArgument1->text().toLong(), default_num, this );
      else
        return;
    }
    break;

  case 'P':
    {
      // P <if-flag> <loaded object vnum> <max #> <container vnum>
      if( id == 1 )
        numSelected = SelectObject::item( m_area, default_num, this );
      else if( id == 3 )
        numSelected = SelectObject::container( m_area, default_num, this );
      else
        return;
    }
    break;

  case 'E':
    {
      // E <if-flag> <object vnum> <max #> <equipment position>
      if( id == 1 )
        numSelected = SelectObject::item( m_area, default_num, this );
      else if( id == 3 )
        numSelected = SelectObject::equipmentPosition( default_num, this );
      else
        return;
    }
    break;

  case 'G':
    {
      // G <if-flag> <object vnum> <max #>
      if( id == 1 )
        numSelected = SelectObject::item( m_area, default_num, this );
      else
        return;
    }
    break;

  case 'O':
    {
      // O <if-flag> <object vnum> <max #> <room vnum> <max room #>
      if( id == 1 )
        numSelected = SelectObject::item( m_area, default_num, this );
      else if( id == 3 )
        numSelected = SelectObject::room( m_area, default_num, this );
      else
        return;
    }
    break;

  case 'D':
    {
      // D <if-flag> <room vnum> <exit #> <doorstate>
      if( id == 1 )
        numSelected = SelectObject::room( m_area, default_num, this );
      else if( id == 2 )
        numSelected = SelectObject::exit( m_area, mp_leArgument1->text().toLong(), default_num, this );
      else if( id == 3 )
        numSelected = SelectObject::exitStatus( default_num, this );
      else
        return;
    }
    break;

  default:
    return;
  }

  if( numSelected == default_num )
    return;

  pLe->setText( QString::number( numSelected ) );
}
