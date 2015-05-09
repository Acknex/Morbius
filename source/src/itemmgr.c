#include <acknex.h>
#include "items.h"

#define itemType skill1
#define itemId skill2
#define itemSequence skill3
#define itemHover FLAG1
#define itemRemove FLAG2

#define TYPE_INTERACTIONITEM 1

BMAP* cursor_grab = "cursor_grab.tga";
BMAP* cursor_look = "cursor_look.tga";
BMAP* cursor_point = "cursor_point.tga";

FONT* interActionItem__font = "Arial#20b";
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
ENTITY* interactionItem__find(int id);

void itemmgr_init()
{
	bmp_cursor_array[TYPE_ITEM_DEFAULT] = NULL; //bmap_create(".tga");
	bmp_cursor_array[TYPE_ITEM_GRAB] = bmap_create("cursor_grab.tga");
	bmp_cursor_array[TYPE_ITEM_LOOK] = bmap_create("cursor_look.tga");
	bmp_cursor_array[TYPE_ITEM_POINT] = bmap_create("cursor_point.tga");
	bmp_cursor_array[TYPE_ITEM_EXIT] = bmap_create("cursor_exit.tga");
	bmp_cursor_array[TYPE_ITEM_TALK] = NULL; //bmap_create(".tga");
	bmp_cursor_array[TYPE_ITEM_USE] = bmap_create("cursor_use.tga");
	bmp_cursor_array[TYPE_ITEM_SEARCH] = NULL; //bmap_create(".tga");
}

//skill1: ItemType 0
//skill2: ItemId -1
action interactionItem()
{
	mouse_map = cursor_point;

	my->itemType = TYPE_INTERACTIONITEM;
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
		mouse_map = cursor_point;
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
		resultId = ITEM_interaction(item, &my->itemSequence);
		
		if (resultId != ITEM_NONE)
		{
			//TODO add item with resultId to inventory
			ITEM* itemToAdd = ITEM_get(resultId);
			ITEM *resultIdItem = inv_create_item(resultId, itemToAdd->name, "Item description", 0, ITEM_TYPE_NEUTRAL);
			inv_add_item(inv, resultIdItem);
		}
		
		//error(str_for_num(NULL, my->itemSequence));
		if (ITEM_isLastSequence(item, my->itemSequence) != 0) 
		{
			if (item->collectable != 0)
			{
				//TODO: interaction
				ITEM *newItem = inv_create_item(item->id, item->name, "Item description", 0, ITEM_TYPE_NEUTRAL);
				inv_add_item(inv, newItem);
			}
			
			if (item->destroyable != 0)
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
			if (ITEM_isLastSequence(item, my->itemSequence) != 0 && item->collectable != 0)
				mouse_map = cursor_grab;
			else
				mouse_map = cursor_look;
		}
	}

	if (event_type == EVENT_RELEASE)
	{
		if (is(my, itemHover))
		{
			reset (my,itemHover);			
			reset (interActionItem__txt, SHOW);
			mouse_map = cursor_point;
		}
	}
	
}


//untested
void interactionItem_morph(int targetId, int morphId)
{
	ENTITY* ent;
	ITEM* item;
	
	ent = interactionItem__find(targetId);
	item = ITEM_get(morphId);
	
	if (item->entfile != NULL)
	{
		ent_morph(ent, item->entfile);
	}
	ent->itemId = morphId;
}

//untested
ENTITY* interactionItem__find(int id)
{
	ENTITY* ent = NULL;
	
	while (ent_next(ent) != NULL)
	{
		if (ent != NULL)
		{
			if (ent->itemType == TYPE_INTERACTIONITEM && ent->itemId == id)
				break;
		}
	}
	
	return ent;
}