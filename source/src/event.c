#include "dialogs.h"
#include "inventory.h"
#include "level_transition.h"

#include "dialogs.c"

#define ITEM_ID_MUENZEN 40

int EVENT__triggerId = -1;
var EVENT__stop = 0;

void EVENT__evaluate(int triggerId);

void EVENT_trigger(int triggerId)
{
	EVENT__triggerId = triggerId;
}

void EVENT_stop()
{
	EVENT__stop = 1;	
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
		if(EVENT__triggerId != -1)
		{
			EVENT__evaluate(EVENT__triggerId);	
			EVENT__triggerId = -1;
		}
		wait (1);
	}
}

void EVENT__evaluate(int triggerId)
{
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
			//error("TODO: custom Telefondialog1");
			dlgStart("xml\\dialog02_fritz.xml");
			break;
		}
		
		//Zugängliche benutzte Telefonzelle -> (invalid item id)
		case 1001: //use non existing item id (> 1000) for solely custom functionality
		{
			//error("TODO: custom Telefondialog2");
			Item* item = inv_item_search(inventory, ITEM_ID_MUENZEN);
			inv_remove_item(item.inv,item);
			
			dlgStart("xml\\dialog03_galep.xml");
			while(dlgIsDialogActive())
			{
				wait (1);
			}
			dlgStart("xml\\monolog05.xml");			
			while(dlgIsDialogActive())
			{
				wait (1);
			}
			level_change(2, 1);
			break;
		}
		
		default:
		{
			break;
		}
	}
}



