
#include "wndmobinit.h"
#include "guiutils.h"
#include "config.h"
#if 0
using namespace ts;

WndMobInit::WndMobInit( VNumber mvnu, VNumber room, int max,Area& ref_area, QWidget* parent)
    : QDialog( parent ), Ui::GuiMobInit()
{
    #if defined( KREATOR_DEBUG )
    qDebug( "WndMobInit::WndMobInit called." );
    #endif

    setupUi(this);

    tb_light->setIcon( TS::GetEditIcon() );
    tb_ringDx->setIcon( TS::GetEditIcon() );
    tb_ringSx->setIcon( TS::GetEditIcon() );
    tb_neck1->setIcon( TS::GetEditIcon() );
    tb_neck2->setIcon( TS::GetEditIcon() );
    tb_body->setIcon( TS::GetEditIcon() );
    tb_head->setIcon( TS::GetEditIcon() );
    tb_legs->setIcon( TS::GetEditIcon() );
    tb_feet->setIcon( TS::GetEditIcon() );
    tb_hands->setIcon( TS::GetEditIcon() );
    tb_arms->setIcon( TS::GetEditIcon() );
    tb_shield->setIcon( TS::GetEditIcon() );
    tb_cloak->setIcon( TS::GetEditIcon() );
    tb_waist->setIcon( TS::GetEditIcon() );
    tb_wristDx->setIcon( TS::GetEditIcon() );
    tb_wristSx->setIcon( TS::GetEditIcon() );
    tb_wield->setIcon( TS::GetEditIcon() );
    tb_hold->setIcon( TS::GetEditIcon() );
    tb_back->setIcon( TS::GetEditIcon() );
    tb_earDx->setIcon( TS::GetEditIcon() );
    tb_earSx->setIcon( TS::GetEditIcon() );
    tb_eye->setIcon( TS::GetEditIcon() );

    tb_addItem->setIcon( TS::GetAddIcon() );
    tb_cloneItem->setIcon( TS::GetCopyIcon() );
    tb_editItem->setIcon( TS::GetEditIcon() );
    tb_removeItem->setIcon( TS::GetRemoveIcon() );

    tb_addFollowers->setIcon( TS::GetAddIcon() );
    tb_cloneFollowers->setIcon( TS::GetCopyIcon() );
    tb_editFollowers->setIcon( TS::GetEditIcon() );
    tb_removeFollowers->setIcon( TS::GetRemoveIcon() );

	sb_limit->setValue(max);

    for(int i = 0; i < ref_area.zones().size(); ++i) {
        cb_zone->addItem(ref_area.zones().at(i).name());
    }

    for(int i = 0; i < ref_area.rooms().size(); ++i) {
        cb_room->addItem(QString(ref_area.rooms().at(i).name()+"("+QString::number( ref_area.rooms().at(i).vnumber() )+")"));
        if(ref_area.rooms().at(i).vnumber() == room) {
            cb_room->setCurrentIndex( i );
        }
    }

	for(int i = 0; i < ref_area.items().size() ; ++i) {
		if(ref_area.items().at(i).itemType() == ITEM_TYPE_LIGHT) {
			cb_light->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_FINGER)) {
			cb_ringDx->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
			cb_ringSx->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_NECK)) {
			cb_neck1->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
			cb_neck2->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_BODY)) {
			cb_body->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_HEAD)) {
			cb_head->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_LEGS)) {
			cb_legs->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_FEET)) {
			cb_feet->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_HANDS)) {
			cb_hands->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_ARMS)) {
			cb_arms->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_SHIELD)) {
			cb_shield->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_ABOUT)) {
			cb_cloak->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_WAISTE)) {
			cb_waist->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_WRIST)) {
			cb_wristDx->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
			cb_wristSx->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WIELD)) {
			cb_wield->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_HOLD)) {
			cb_hold->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_BACK)) {
			cb_back->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_EAR)) {
			cb_earDx->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
			cb_earSx->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
		if(ref_area.items().at(i).hasWearPosition(ITEM_WEAR_EYE)) {
			cb_eye->addItem(QString(ref_area.items().at(i).shortDescription()+"("+QString::number( ref_area.items().at(i).vnumber() )+")"));
		}
	}


}

WndMobInit::~WndMobInit()
{
#if defined( KREATOR_DEBUG )
  qDebug( "WndMobInit::~WndMobInit() called." );
#endif
}
#endif
