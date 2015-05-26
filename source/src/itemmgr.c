#include <acknex.h>
#include "items.h"
#include "combine.h"
#include "types.h"
#include "inventory.h"
#include "hud.h"
#include "materials.h"
#include "event.h"
#include "player.h"

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

	mousemgr_set(MOUSE_DEFAULT, NULL);
}

//skill1: ItemType 1
//skill2: ItemId -1
action interactionItem()
{
	set(my, INVISIBLE);
	my->itemType = TYPE_ITEM;
	my->group = GROUP_ITEM;
	
	wait(1); //needed for interactionItem_spawn call to set my->itemId properly
	//restore item state: start
	if (my->itemId == -1)
	{
		//wait(1);
		ptr_remove(me);
		return;
	}
	
	ITEM* item = ITEM_get(my->itemId);
	if (item->wasRemoved != 0)
	{
		//wait(1);
		ptr_remove(me);
		return;
	}
	
	while (item->wasMorphedTo != -1)
	{
		//loop through all morph stages
		interactionItem_morph(item->id, item->wasMorphedTo);
		item = ITEM_get(my->itemId);
	}
	//restore item state: end

	my->ENTITY_TYPE = TYPE_ITEM;
	my->material = mat_item;
	VECTOR vmin,vmax;
	vec_for_min(vmin, my);
	vec_for_max(vmax, my);
	vec_scale(vmin, my->scale_x);
	vec_scale(vmax, my->scale_x);
	vec_sub(vmax, vmin);
	my.skill41 = floatv(4.0 / (1 + vec_length(vmax) * 0.1));

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
					reset(my, itemWasClicked);					
				}
				if (Player_getLastClickedEnt() != me)
				{
					//error("new click done");
					reset(my, itemWasClicked);					
				}
			}
			else 
			{
				//only needed for levels without player, maybe remove later
				interactionItem__clicked();
			}
			
		}
		wait(1);
	}

	if (is(my, itemHover))
	{
		reset (interActionItem__txt, SHOW);
		mousemgr_set(MOUSE_DEFAULT, NULL);
	}
	ptr_remove(me);
}

//untested
void interactionItem_spawn(int id, VECTOR* position)
{
	ITEM* item = ITEM_get(id);
	if (item != NULL)	
	{
		if (item->entfile != NULL)
		{
			you = ent_create(item->entfile, position, interactionItem);
			your->itemId = id;
		}
	}
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
			if(handItem->destroyable == 0)
			{
				inv_add_item(inventory, itemInHand);
			}
			mousemgr_set(MOUSE_DEFAULT, NULL);
		
			//only perform morph and inventory actions if resultId points to real item
			//fake resultIds may be used to trigger custom events
			ITEM* resultItem = ITEM_get(resultId);
			if (resultItem != NULL)
			{
				if (targetId != ITEM_NONE)
				{
					//morph defined target item
					interactionItem_morph(targetId, resultId);
					//TODO inventory morph
				}
				else
				{
					//UNTESTED
					//create new inventory item with resultId;
					Item *resultIdItem = inv_create_item(resultId, resultItem->name, "Item description", 0, bmap_create(resultItem->imgfile));
					inv_add_item(inventory, resultIdItem);
				}			
			}
	
			//item in world
			if (item->destroyable != 0)
			{
				ITEM_collect(item);
				set(my, itemRemove);
			}
				
			EVENT_trigger(resultId);
		}
		else
		{
			inv_add_item(inventory, itemInHand);
			//TODO: play random fail sound
			//snd_play(...);
			//TODO: show random fail message
			//HUD_showDescription(msg);			
		}
		itemInHand = NULL;
		
		//TODO: use inventory item on inventory item. This is not handled here!!				
	}
	else
	{
		resultId = ITEM_interaction(item);
	

		if (resultId != ITEM_NONE)
		{
			ITEM* itemToAdd = ITEM_get(resultId);
			if (itemToAdd != NULL)
			{
				Item *resultIdItem = inv_create_item(resultId, itemToAdd->name, "Item description", 0, bmap_create(itemToAdd->imgfile));
				inv_add_item(inventory, resultIdItem);
			}

			EVENT_trigger(resultId);
		}
		
		if (ITEM_isLastSequence(item) != 0) 
		{
			if (item->collectable != 0)
			{
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
		//wait(1);
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
				mousemgr_set(MOUSE_USE, itemInHand.image);
			}
			else
			{
				if (item->collectable != 0)
				{
					mousemgr_set(MOUSE_GRAB, NULL);
				}
				else
				{
					mousemgr_set(MOUSE_LOOK, NULL);
				}
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
				mousemgr_set(MOUSE_DEFAULT, itemInHand->image);
				//TODO
			}
			else
			{
				mousemgr_set(MOUSE_DEFAULT, NULL);
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

	if ((item->entfile != NULL) && (ent != NULL))
	{
		ent_morph(ent, item->entfile);
	}
	ent->itemId = morphId;
	ent->itemType = TYPE_ITEM;
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