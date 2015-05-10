#include <acknex.h>
#include "items.h"
#include "combine.h"
#include "types.h"
#include "inventory.h"
#include "hud.h"

#define itemType skill1
#define itemId skill2
#define itemSequence skill3
#define itemHover FLAG1
#define itemRemove FLAG2


//BMAP* cursor_grab = "cursor_grab.tga";
//BMAP* cursor_look = "cursor_look.tga";
//BMAP* cursor_point = "cursor_point.tga";

TEXT* interActionItem__txt;

void interactionItem__eventHandler();
ENTITY* interactionItem__find(int id);

void itemmgr_init()
{
	interActionItem__txt = HUD_getItemText();

	bmp_cursor_array[TYPE_ITEM_DEFAULT] = NULL; //bmap_create(".tga");
	bmp_cursor_array[TYPE_ITEM_GRAB] = bmap_create("cursor_grab.tga");
	bmp_cursor_array[TYPE_ITEM_LOOK] = bmap_create("cursor_look.tga");
	bmp_cursor_array[TYPE_ITEM_POINT] = bmap_create("cursor_point.tga");
	bmp_cursor_array[TYPE_ITEM_EXIT] = bmap_create("cursor_exit.tga");
	bmp_cursor_array[TYPE_ITEM_TALK] = bmap_create("cursor_talk.tga");
	bmp_cursor_array[TYPE_ITEM_USE] = bmap_create("cursor_use.tga");
	bmp_cursor_array[TYPE_ITEM_SEARCH] = NULL; //bmap_create(".tga");

	mouse_map = bmp_cursor_array[TYPE_ITEM_POINT];
}

//skill1: ItemType 0
//skill2: ItemId -1
action interactionItem()
{
	my->itemType = TYPE_ITEM;
	my->itemSequence = 0;
	
	if (my->itemId == -1)
	{
		wait(1);
		ptr_remove(me);
	}
	my->event = interactionItem__eventHandler;
	my->emask |= ENABLE_CLICK | ENABLE_TOUCH | ENABLE_RELEASE;
	
	while(!is(my, itemRemove))
	{
		if (is(my, itemHover))
		{
			interActionItem__txt->pos_x = mouse_pos.x;
			interActionItem__txt->pos_y = mouse_pos.y - 10;
		}
		wait(1);
	}

	if (is(my, itemHover))
	{
		reset (interActionItem__txt, SHOW);
		mouse_map = bmp_cursor_array[TYPE_ITEM_POINT];
	}
	ptr_remove(me);
}


void interactionItem__eventHandler()
{
	ITEM* item = ITEM_get(my->itemId);
	int resultId;
	
	if (item == NULL)
		return;
		
	if (event_type == EVENT_CLICK)
	{
		
		// If we have an item in hand
		if (itemInHand != NULL) 
		{
			int targetId;
			resultId = COMBINATION_combine(itemInHand->id, item->id, &targetId);
			ITEM* handItem = ITEM_get(itemInHand->id);

			//item in inventory
			if(handItem->destroyable != 0)
			{
				//remove from inventory
			}				

			if (resultId != ITEM_NONE)
			{
				itemInHand = NULL;
				mouse_map = bmp_cursor_array[TYPE_ITEM_LOOK];
			}

			//morph defined target item
			if (targetId != ITEM_NONE)
			{
				//World morph
				interactionItem_morph(targetId, resultId);
				//TODO inventory morph?
			}
			else
			{
				//create new inventory item with resultId;
			}			

			//item in world
			ITEM* myItem = ITEM_get(my->itemId);
			if(myItem->destroyable != 0)
			{
				set(my, itemRemove);
			}
			return;
			
			//TODO: use inventory item on inventory item. This is not handled here!!				
		}
		else
		{
			resultId = ITEM_interaction(item, &my->itemSequence);
		}

		if (resultId != ITEM_NONE)
		{
			//TODO add item with resultId to inventory
			ITEM* itemToAdd = ITEM_get(resultId);
			Item *resultIdItem = inv_create_item(resultId, itemToAdd->name, "Item description", 0, ITEM_TYPE_NEUTRAL, bmap_create(itemToAdd->imgfile));
			inv_add_item(inventory, resultIdItem);
		}
		
		//error(str_for_num(NULL, my->itemSequence));
		if (ITEM_isLastSequence(item, my->itemSequence) != 0) 
		{
			if (item->collectable != 0)
			{
				//TODO: interaction
				Item *newItem = inv_create_item(item->id, item->name, "Item description", 0, ITEM_TYPE_NEUTRAL, bmap_create(item->imgfile));
				
				inv_add_item(inventory, newItem);
				set(my, itemRemove);
			}
			
			else if (item->destroyable != 0)
			{
				set(my, itemRemove);
			}
		}
	}
	
	if (event_type == EVENT_TOUCH)
	{
		interActionItem__txt->pos_x = mouse_pos.x;
		interActionItem__txt->pos_y = mouse_pos.y - 10;

		if (!is(my, itemHover))
		{
			set (my, itemHover);	
			str_cpy((interActionItem__txt->pstring)[0], item->name);
			set (interActionItem__txt, SHOW);
			//if (ITEM_isLastSequence(item, my->itemSequence) != 0 && item->collectable != 0)
			if (itemInHand != NULL) {
				// ToDo: Don't change mouse cursor
				// mouse_map = bmp_cursor_array[TYPE_ITEM_USE];
			}
			else
			{
				mouse_map = bmp_cursor_array[TYPE_ITEM_LOOK];
			}
		}
	}

	if (event_type == EVENT_RELEASE)
	{
		if (is(my, itemHover))
		{
			reset (my,itemHover);			
			reset (interActionItem__txt, SHOW);
			if (itemInHand != NULL)
			{
				mouse_map = itemInHand->image;
				//TODO
			}
			else
			{
				mouse_map = bmp_cursor_array[TYPE_ITEM_POINT];
			}
		}
	}
	
}

void interactionItem_morph(int targetId, int morphId)
{
	ENTITY* ent;
	ITEM* item;
	
	ent = interactionItem__find(targetId);
	item = ITEM_get(morphId);
	
	if ((item->entfile != NULL) && (ent != NULL))
	{
		ent_morph(ent, item->entfile);
	}
	ent->itemId = morphId;
}

ENTITY* interactionItem__find(int id)
{
	ENTITY* ent = NULL;
	
	while (ent = ent_next(ent))
	{
		if (ent != NULL)
		{
			if (ent->itemType == TYPE_ITEM)
			{
				if (ent->itemId == id)
				{
					break;
				}
			}
		}
	}
	return ent;
}