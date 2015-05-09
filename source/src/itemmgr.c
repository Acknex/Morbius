#include <acknex.h>
#include "items.h"

#define itemType skill1
#define itemId skill2
#define itemHover FLAG1
#define itemRemove FLAG2

#define TYPE_INTERACTIONITEM 1


FONT* interActionItem__font = "Arial#12b";
TEXT* interActionItem__txt = 
{
	font = interActionItem__font;
	layer = 1;	
	pos_x = 0;
	pos_y = 0;
	flags |= CENTER_X, CENTER_Y;
	string ("");
}

void interactionItem__eventHandler();


//skill1: ItemType 0
//skill2: ItemId -1
action interactionItem()
{
	my->itemType = TYPE_INTERACTIONITEM;
	
	if (my->itemId == -1)
	{
		wait(1);
		ptr_remove(me);
	}
	my->event = interactionItem__eventHandler;
	my->emask |= ENABLE_CLICK | ENABLE_TOUCH | ENABLE_RELEASE;
	
	while(is(my, itemRemove))
	{
		wait(1);
	}
	ptr_remove(me);
}


void interactionItem__eventHandler()
{
	ITEM* item = ITEM_get(my->itemId);
	if (item == NULL)
		return;
		
	if (event_type == EVENT_CLICK)
	{
		if (item->collectable != 0)
		{
			//TODO: interaction
		}
		else
		{
			//TODO: interaction
		}
		
		if (item->destroyable != 0)
		{
			set(my, itemRemove);
		}
	}
	
	if (event_type == EVENT_TOUCH)
	{
		interActionItem__txt->pos_x = mouse_pos.x;
		interActionItem__txt->pos_x = mouse_pos.y;

		if (!is(my, itemHover))
		{
			set (my, itemHover);	
			str_cpy((interActionItem__txt->pstring)[0], item->name);
			set (interActionItem__txt, SHOW);
		}
	}

	if (event_type == EVENT_RELEASE)
	{
		if (is(my, itemHover))
		{
			reset (my,itemHover);			
			reset (interActionItem__txt, SHOW);
		}
	}
	
}