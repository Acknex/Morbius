#include "types.h"
#include "dialogs.h"
#include "inventory.h"
#include "level_transition.h"
#include "player.h"

int EVENT__triggerId = -1;
var EVENT__stop = 0;
var EVENT__locked = 0;

void EVENT__evaluate(int triggerId);
void EVENT__lock();
void EVENT__unlock();

void EVENT_trigger(int triggerId)
{
	EVENT__triggerId = triggerId;
}

void EVENT_stop()
{
	EVENT__stop = 1;	
}

var EVENT_isLocked()
{
	return EVENT__locked;
}

void EVENT__listener_startup()
{
	/* a listener loop is used in order to decouple
	 * every custom event trigger from any entity which
	 * might be the trigger source.
	 * Otherwise strange behaviour may show up if the entity
	 * is removed.
	 */
	
	while(EVENT__stop == 0)
	{
		if(EVENT__triggerId != -1 && EVENT__locked == 0)
		{
			EVENT__evaluate(EVENT__triggerId);	
			EVENT__triggerId = -1;
		}
		wait (1);
	}
}

//CUSTOM PART START
#define wait_for_dlg(x) dlgStart(x); while (dlgIsDialogActive() != 0) wait (1)

SOUND* fritzCallSnd = "fritz_call.ogg";
SOUND* galepCallSnd = "galep_call.ogg";

void EVENT__evaluate(int triggerId)
{
	EVENT__lock();
	
	//Modify switch/case as needed
	switch(triggerId)
	{
		//Abgeschlossene Schublade -> Schublade
		case 4:
		{
			//Items in drawer must be spawned or at least their position must be adapted
			//otherwise they would be visible with locked drawer
			//TODO: maybe solution for automation: allow parentId in XML?
			error("TODO: trigger custom spawn revolver und visitenkarte");
			//interactionItem_spawn(id, position);
			//interactionItem_spawn(1, visitenkartepos);
			//interactionItem_spawn(2, revolverpos);
			break;
		}
		
		//Zugängliche Telefonzelle -> Zugängliche benutzte Telefonzelle
		case 41: //use existing item ids for additional functionality
		{
			wait_for_dlg("xml\\monolog03.xml");

			SOUNDMGR_scheduleSound(fritzCallSnd);
			while (SOUNDMGR_isPlaying(fritzCallSnd) != 0) wait (1);
			
			wait_for_dlg("xml\\dialog02_fritz.xml");
			break;
		}
		
		//Zugängliche benutzte Telefonzelle -> zweimal benutzte Telefonzelle
		case 42: //use non existing item id (> 1000) for solely custom functionality
		{
			wait_for_dlg("xml\\monolog04.xml");

			Item* item = inv_item_search(inventory, ITEM_ID_MUENZEN);
			inv_remove_item(item.inv,item);

			SOUNDMGR_scheduleSound(galepCallSnd);
			while (SOUNDMGR_isPlaying(galepCallSnd) != 0) wait (1);
			
			wait_for_dlg("xml\\dialog03_galep.xml");
			wait(-1.5);
			wait_for_dlg("xml\\monolog05.xml");			
			
			level_change(2, 1);
			break;
		}
		
		default:
		{
			break;
		}
	}
	
	EVENT__unlock();
}

void EVENT__lock()
{
	EVENT__locked = 1;

	inv_hide(inventory);
	player_may_walk = 0;
}

void EVENT__unlock()
{
	EVENT__locked = 0;

	inv_show(inventory);
	player_may_walk = 1;
}

