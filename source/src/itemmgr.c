#include "items.h"
#include "combine.h"
#include "types.h"
#include "inventory.h"
#include "materials.h"
#include "event.h"
#include "player.h"
#include "dialogs.h"
#include "mousemgr.h"

#define itemId skill2
#define itemBorder skill3
#define itemVecMin skill60
#define itemVecMin_x skill60
#define itemVecMin_y skill61
#define itemVecMin_z skill62
#define itemVecMax skill63
#define itemVecMax_x skill63
#define itemVecMax_y skill64
#define itemVecMax_z skill65
#define itemHover FLAG1
#define itemRemove FLAG2
#define itemWasClicked FLAG3

SOUND* ITEMS__collectSnd = "collect.wav";


void interactionItem__clicked();
void interactionItem__eventHandler();
ENTITY* interactionItem__find(int id);
void interactionItem__findSpawnPoint(int id, VECTOR* position, VECTOR* angle, var* border);
void interactionItem__setNearZone(VECTOR* vec, var border);
var interactionItem__isNear(ENTITY* ent);

//skill1: EntityType 1
//skill2: ItemId -1
//skill3: ItemBorder 25
//flag5: vmask 0
action interactionItem()
{
	set(my, INVISIBLE);
	reset(my, PASSABLE);

	my->ENTITY_TYPE = TYPE_ITEM;
	my->group = GROUP_ITEM;
	
	wait(1); //needed for interactionItem_spawn call to set my->itemId properly
	if (my->itemBorder == 0) my->itemBorder = 25;
	if (my->itemId == -1)
	{
		ptr_remove(me);
		return;
	}
	
	vec_for_min(&my->itemVecMin, me);
	vec_for_max(&my->itemVecMax, me);
	interactionItem__setNearZone(&my->itemVecMin, -my->itemBorder);
	interactionItem__setNearZone(&my->itemVecMax, my->itemBorder);
	
	//restore item state: start
	ITEM* item = ITEM_get(my->itemId);
	while (item->wasMorphedTo != -1)
	{
		//loop through all morph stages
		interactionItem_morph(item->id, item->wasMorphedTo);
		item = ITEM_get(my->itemId);
	}

	if ((item->collectable != 0) && (ITEM_isNowCollectable(item) != 0))
	{
		my->material = mat_item;
	}

	if (item->wasRemoved != 0)
	{
		ptr_remove(me);
		return;
	}	
	//restore item state: end

	if(is(my,FLAG5)) my.vmask |= (1<<1);
//	my->material = mat_item;
	reset(my, INVISIBLE | TRANSLUCENT);
	my->event = interactionItem__eventHandler;
	my->emask |= ENABLE_CLICK | ENABLE_TOUCH | ENABLE_RELEASE;
	
	while(!is(my, itemRemove))
	{
		//draw_box3d(my->itemVecMin,my->itemVecMax,vector(0,0,255),100);
		//draw_box3d(my->min_x,my->max_x,vector(0,255,255),100);
		if (is(my, itemWasClicked) && dlgIsDialogActive() == 0)
		{
				if (interactionItem__isNear(player))
				{
					Player_stop();
					interactionItem__clicked();
					reset(my, itemWasClicked);					
				}
				
				if (Player_getLastClickedEnt() != me)
				{
					reset(my, itemWasClicked);					
				}
			
		}
		wait(1);
	}

	if (is(my, itemHover))
	{
		mousemgr_set(MOUSE_DEFAULT, NULL);
		mousemgr_hint(NULL);
	}
	if (Player_getLastClickedEnt() == me)
	{
		Player_resetLastClickedEnt();
	}
	ptr_remove(me);
}

//skill1: EntityType 5
//skill2: ItemId -1
//skill3: ItemBorder 25
action interactionSpawnPnt()
{
	set(my, INVISIBLE | PASSABLE);
	my->ENTITY_TYPE = TYPE_ITEM_SPAWNPOINT;

	//restore laste state after level change
	ITEM* item = ITEM_get(my->itemId);
	if (item != NULL)	
	{
		if (item->hasSpawned != 0)
		{
			interactionItem_spawn(my->itemId, &my->x, &my->pan, my->itemBorder);
		}
	}	
}

void interactionItem_remoteStart()
{
	interactionItem();
}

var interactionItem_isNearPlayer(ENTITY* ent)
{
	if (ent != NULL)
	{
		if (ent->ENTITY_TYPE == TYPE_ITEM)
		{
			//this is hack
			ENTITY* caller = me;
			me = ent;
			if(interactionItem__isNear(caller))
			{
				me = caller;
				return 1;
			}
			me = caller;
		}
	}

	return 0;
}

void interactionItem_spawn(int id)
{
	VECTOR pos;
	VECTOR dir;
	var border;
	
	interactionItem__findSpawnPoint(id, &pos, &dir, &border);
	interactionItem_spawn(id, &pos, &dir, border);
}

void interactionItem_spawn(int id, VECTOR* position, VECTOR* angle, var border)
{
	ITEM* item = ITEM_get(id);
	if (item != NULL)	
	{
		if (item->entfile != NULL)
		{
			you = ent_create(item->entfile, position, interactionItem);
			your->itemId = id;
			your->itemBorder = border;
			vec_set(&your->pan, angle);
			item->hasSpawned = 1; //save spawn state for level change
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
			
			//only perform morph and inventory actions if resultId points to real item
			//fake resultIds may be used to trigger custom events
			ITEM* resultItem = ITEM_get(resultId);
			if (resultItem != NULL)
			{
				if (targetId != ITEM_NONE)
				{
					//morph defined target item
					interactionItem_morph(targetId, resultId);
				}
				else
				{
					//create new inventory item with resultId;
					Item *resultIdItem = inv_create_item(resultId, resultItem->name, "Item description", 0, bmap_create(resultItem->imgfile));
					inv_add_item(inventory, resultIdItem);
					SOUNDMGR_playAtOnce(ITEMS__collectSnd);			
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
		}
		itemInHand = NULL;
		mousemgr_set(MOUSE_DEFAULT, NULL);
		
		//use inventory item on inventory item is handled in inventory.c!!				
	}
	else
	{
		resultId = ITEM_interaction(item);
		

		if ((item->collectable != 0) && (ITEM_isNowCollectable(item) != 0))
		{
			my->material = mat_item;
		}

		if (resultId != ITEM_NONE)
		{
			ITEM* itemToAdd = ITEM_get(resultId);
			if (itemToAdd != NULL)
			{
				Item *resultIdItem = inv_create_item(resultId, itemToAdd->name, "Item description", 0, bmap_create(itemToAdd->imgfile));
				inv_add_item(inventory, resultIdItem);
				SOUNDMGR_playAtOnce(ITEMS__collectSnd);			
			}

			EVENT_trigger(resultId);
		}
		
		if (ITEM_isBeingCollected(item) != 0)
		//if (ITEM_isLastSequence(item) != 0) 
		{
			//if (item->collectable != 0)
			//{
				Item *newItem = inv_create_item(item->id, item->name, "Item description", 0, bmap_create(item->imgfile));
				
				inv_add_item(inventory, newItem);
				SOUNDMGR_playAtOnce(ITEMS__collectSnd);			
				ITEM_collect(item);
				EVENT_trigger(item->id);
				set(my, itemRemove);
			//}
			
			//else if (item->destroyable != 0)
			//{
				//	set(my, itemRemove);
			//}
		}
	}
}

void interactionItem__eventHandler()
{
	if (dlgIsDialogActive() != 0 || EVENT_isLocked() != 0)
	return;
	
	ITEM* item = ITEM_get(my->itemId);
	
	if (item == NULL)
	return;
	
	if (event_type == EVENT_CLICK)
	{
		set(my, itemWasClicked);
	}
	
	if (event_type == EVENT_TOUCH)
	{
		if (!is(my, itemHover))
		{
			set (my, itemHover);	
			mousemgr_hint(item->name);
			if (itemInHand != NULL) 
			{
				mousemgr_set(MOUSE_USE, itemInHand->image);
			}
			else
			{
				if ((item->collectable != 0) && (ITEM_isNowCollectable(item) != 0))
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
			mousemgr_hint(NULL);	
			if (itemInHand != NULL)
			{
				mousemgr_set(MOUSE_DEFAULT, itemInHand->image);
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
		//STRING* str = str_printf(NULL,"Entity with item id %d not found!", targetId);
		//error(str);
		return;
	}

	if ((item->entfile != NULL) && (ent != NULL))
	{
		ent_morph(ent, item->entfile);
	}
	ent->itemId = morphId;
	ent->ENTITY_TYPE = TYPE_ITEM;

	if ((item->collectable != 0) && (ITEM_isNowCollectable(item) != 0))
	{
		ent->material = mat_item;
	}
	else
	{
		//stop item flashing when morphing from collectable to non collectable
		if (ent->material == mat_item)
		{
			ent->material = mtl_model;
		}
	}

	if (is(ent, itemHover))
	{
		mousemgr_hint(item->name);
		if ((item->collectable != 0) && (ITEM_isNowCollectable(item) != 0))
		{
			mousemgr_set(MOUSE_GRAB, NULL);
		}
		else
		{
			mousemgr_set(MOUSE_LOOK, NULL);
		}
	}
}

ENTITY* interactionItem__find(int id)
{
	ENTITY* ent = NULL;
	
	while (ent = ent_next(ent))
	{
		if (ent != NULL)
		{
			if (ent->ENTITY_TYPE == TYPE_ITEM)
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

void interactionItem__findSpawnPoint(int id, VECTOR* position, VECTOR* angle, var* border)
{
	ENTITY* ent = NULL;
	vec_set(position, nullvector);
	vec_set(angle, nullvector);
	
	if (player != NULL)
	{
		//fallback: spawn at player position if spawnpoint was not placed
		vec_set(position, &player->x);
	}

	while (ent = ent_next(ent))
	{
		if (ent != NULL)
		{
			if (ent->ENTITY_TYPE == TYPE_ITEM_SPAWNPOINT)
			{
				if (ent->itemId == id)
				{
					vec_set(position, &ent->x);
					vec_set(angle, &ent->pan);
					*border = ent->itemBorder;
					break;
				}
			}
		}
	}
}

void interactionItem__setNearZone(VECTOR* vec, var border)
{
	vec_mul(vec, &my->scale_x);
	vec_add(vec, vector(border, border, border));
	var vPan = integer((my->pan + 45) / 90) * 90;
	vec_rotate(vec, vector(vPan, 0, 0));
	vec_add(vec, &my->x);
}

var interactionItem__isNear(ENTITY* ent)
{
	if (ent != NULL)
	{
		if (
		(
			(my->itemVecMin_x < my->itemVecMax_x && ent->x > my->itemVecMin_x && ent->x < my->itemVecMax_x) ||
			(my->itemVecMin_x > my->itemVecMax_x && ent->x < my->itemVecMin_x && ent->x > my->itemVecMax_x)
		)
		&&
		(
			(my->itemVecMin_y < my->itemVecMax_y && ent->y > my->itemVecMin_y && ent->y < my->itemVecMax_y) ||
			(my->itemVecMin_y > my->itemVecMax_y && ent->y < my->itemVecMin_y && ent->y > my->itemVecMax_y)
		)
		)
		{
			return 1;
		}
	}
	return 0;
}
