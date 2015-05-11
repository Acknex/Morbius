#include <acknex.h>
#include "items.h"
#include "combine.h"
#include "types.h"
#include "inventory.h"
#include "hud.h"
#include "materials.h"

#define itemType skill1
#define itemId skill2
#define itemHover FLAG1
#define itemRemove FLAG2
#define itemWasClicked FLAG3

#define ITEM_MINPLAYERDIST 50

TEXT* interActionItem__txt;

void interactionItem__clicked();
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
	set(my, INVISIBLE);
	my->itemType = TYPE_ITEM;
	my.group = GROUP_ITEM;
	
	//restore item state: start
	if (my->itemId == -1)
	{
		//wait(1);
		ptr_remove(me);
		return;
	}
	
	ITEM*	item = ITEM_get(my->itemId);
	if (item->wasRemoved != 0)
	{
		//wait(1);
		ptr_remove(me);
		return;
	}
	
	if (item->wasMorphedTo != -1)
	{
		interactionItem_morph(item->id, item->wasMorphedTo);
	}
	//restore item state: end

	my.ENTITY_TYPE = TYPE_ITEM;
	my.material = mat_item;
	VECTOR vmin,vmax;
	vec_for_min(vmin,my);
	vec_for_max(vmax,my);
	vec_scale(vmin,my.scale_x);
	vec_scale(vmax,my.scale_x);
	vec_sub(vmax,vmin);
	my.skill41 = floatv(4.0/(1+vec_length(vmax)*0.1));
	reset(my, INVISIBLE | TRANSLUCENT);
	my->event = interactionItem__eventHandler;
	my->emask |= ENABLE_CLICK | ENABLE_TOUCH | ENABLE_RELEASE;
	
	while(!is(my, itemRemove))
	{
		if (is(my, itemHover))
		{
			interActionItem__txt->pos_x = mouse_pos.x;
			interActionItem__txt->pos_y = mouse_pos.y - 10;
		}

		if (is(my, itemWasClicked))
		{
			if (player != NULL)
			{
				if(vec_dist(player->x, my->x) < ITEM_MINPLAYERDIST)
				{
					interactionItem__clicked();
				}
			}
			else 
			{
				//only needed for levels without player, maybe remove later
				interactionItem__clicked();
			}
			
			reset(my, itemWasClicked);
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


void interactionItem__clicked()
{
	ITEM* item = ITEM_get(my->itemId);
	int resultId;

	if (item == NULL)
		return;

	// If we have an item in hand
	if (itemInHand != NULL) 
	{
		int targetId;
		resultId = COMBINATION_combine(itemInHand->id, item->id, &targetId);
		ITEM* handItem = ITEM_get(itemInHand->id);

		if (resultId != ITEM_NONE)
		{
			//item in inventory
			if(handItem->destroyable != 0)
			{
				//remove from inventory
			}				

			itemInHand = NULL;
			mouse_map = bmp_cursor_array[TYPE_ITEM_LOOK];
		

			//morph defined target item
			if (targetId != ITEM_NONE)
			{
				interactionItem_morph(targetId, resultId);
				//TODO inventory morph
			}
			else
			{
				//create new inventory item with resultId;
			}			

			//item in world
			ITEM* myItem = ITEM_get(my->itemId);
			if(myItem->destroyable != 0)
			{
				ITEM_collect(item);
				set(my, itemRemove);
			}
		}
		
		//TODO: use inventory item on inventory item. This is not handled here!!				
	}
	else
	{
		resultId = ITEM_interaction(item);
	

		if (resultId != ITEM_NONE)
		{
			//TODO add item with resultId to inventory
			ITEM* itemToAdd = ITEM_get(resultId);
			Item *resultIdItem = inv_create_item(resultId, itemToAdd->name, "Item description", 0, bmap_create(itemToAdd->imgfile));
			inv_add_item(inventory, resultIdItem);
		}
		
		if (ITEM_isLastSequence(item) != 0) 
		{
			if (item->collectable != 0)
			{
				//TODO: interaction
				Item *newItem = inv_create_item(item->id, item->name, "Item description", 0, bmap_create(item->imgfile));
				
				inv_add_item(inventory, newItem);
				ITEM_collect(item);
				set(my, itemRemove);
			}
			
			//else if (item->destroyable != 0)
			//{
			//	set(my, itemRemove);
			//}
		}
	}
}

void interactionItem__eventHandler()
{
	ITEM* item = ITEM_get(my->itemId);
	
	if (item == NULL)
		return;
		
	if (event_type == EVENT_CLICK)
	{
		STRING* str = str_printf(NULL,"item id %d", (int)my->itemId);
		//error(str);
		draw_text(str, 50,50, vector(255,255,255));
		wait(1);
		set(my, itemWasClicked);
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
	
	ITEM* oldItem = ITEM_get(targetId);
	oldItem->wasMorphedTo = morphId; //save morph state for level change
	
	ent = interactionItem__find(targetId);
	item = ITEM_get(morphId);

	if (ent == NULL)
	{
		STRING* str = str_printf(NULL,"Entity with item id %d not found!", targetId);
		error(str);
		return;
	}

//error(item->entfile);	
	if ((item->entfile != NULL) && (ent != NULL))
	{
		ent_morph(ent, item->entfile);
	}
		//STRING* str = str_printf(NULL,"morph id %d", morphId);
		//error(str);
	ent->itemId = morphId;
	ent->itemType = TYPE_ITEM;
//		STRING* str = str_printf(NULL,"morph id %d", ent->itemId);
//		error(str);
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