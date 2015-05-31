#include "types.h"
#include "event.h"
#include "player.h"
#include "level_transition.h"
#include "items.h"
#include "dialogs.h"
#include "inventory.h"

//officeLevel_gate
#define triggerItemId skill3
#define badItemId skill4
#define goodItemId skill5
#define activeItemId skill6
#define ALLOWBADGATE FLAG1


action officeChair()
{
	OFFICE__officeChairEnt = me;
}

action officeStart()
{
	set(my, INVISIBLE | PASSABLE);
	OFFICE__officeStartEnt = me;
}

//skill1: this_id 0
//skill2: to_id 0
//skill3: triggerItemId -1
//skill4: badItemId -1
//skill5: goodItemId -1
//flag1: allowBadGate
action officeLevel_gate()
{
	level_gate_init();	
	my.event = officeLevel_gate_event;
	int itemId;
		
	while(1)
	{
		if (inv_item_search(inventory, my->triggerItemId) != NULL)
		{
			my->activeItemId = my->goodItemId;
		}
		else
		{
			my->activeItemId = my->badItemId;
		}
		
		
		if(player)
		{
			if(player.x > my.skill10 && player.y > my.skill11 && player.x < my.skill12 && player.y < my.skill13)
			{
				officeLevel_gate_check();
				break;
			}
		}
		
		if (my.DOUBLE_CLICK_TIME > 0) 
		{
			my.DOUBLE_CLICK_TIME -=1 * time_step;
		}
		wait(1);
	}
}

void officeLevel_gate_check()
{
	ITEM* item = ITEM_get(my->activeItemId);
	int resultId;
	
	if (item != NULL)
	{
		resultId = ITEM_interaction(item);
	
		if (ITEM_isLastSequence(item) != 0 && 
			(
				((my->activeItemId == my->badItemId) && is(my, ALLOWBADGATE)) ||
				(my->activeItemId == my->goodItemId)
			)
		)
		//if (resultId != ITEM_NONE)
		{
			level_change(integer(my.skill2*0.01),my.skill2);
		}
	}
}

void officeLevel_gate_event() 
{
	if (dlgIsDialogActive() != 0 || EVENT_isLocked() != 0)
		return;
		
	if (event_type == EVENT_CLICK) 
	{
		if (my.DOUBLE_CLICK_TIME >= 100) 
		{
			// Double clicked
			if (player != NULL)
			{
				player->force_stop = 1;
			}
			officeLevel_gate_check(); // Instant level change
		} 
		else 
		{
			my.DOUBLE_CLICK_TIME = 110;
		}
	}
	else
	{
		level_gate_event();
	}
}

