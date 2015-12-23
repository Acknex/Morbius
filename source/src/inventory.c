#ifndef _INVENTORY_C_
#define _INVENTORY_C_

#include "itemmgr.h"
#include "event.h"

void (*inv__resizeEv)();

Inventory* inv_create(BMAP* _bg, int width, int height) {
	Inventory* inv = sys_malloc(sizeof(Inventory));
	
	int nInvItemsX = 0;
	int nInvItemsY = 0;
	
	// Create background panel
	inv.panel = pan_create("on_click = inv_on_click;", INV_PANEL_LAYER);
	inv.panel.skill_x = inv;
	if (_bg == NULL) {
		inv.panel.bmap = bmap_createblack(width, height, 24);
		set(inv.panel, TRANSLUCENT);
		inv.panel.alpha = 60;
	} else {
		inv.panel.bmap = _bg;
	}
	//this is sort of hacky
	var vScale = screen_size.y / 1200;	
	inv.panel.scale_y = vScale; //leave original panel y size due to missing height reference
	
	// Create item description
	inv.itemDescription = txt_create(1, INV_PANEL_LAYER + 1);
	set(inv.itemDescription, OUTLINE | WWRAP);
	inv.itemDescription.size_x = bmap_width(inv.panel.bmap);
	inv.itemDescription.font = fontInventoryDescription;
	str_cpy((inv.itemDescription.pstring)[0], "Dies ist ein\nTest!");
	
	inv__resizeEv = on_resize;
	on_resize = inv_resize;
	inv_resize();

	return inv;
}

void inv_free(Inventory* _inv) {
	if (_inv != NULL) {
		ptr_remove(_inv.panel);
		ptr_remove(_inv.itemDescription);
		sys_free(_inv);
		_inv = NULL;
	}
}

int inv_is_visible(Inventory* _inv) {
	if (_inv == NULL) return 0;
	if (is(_inv.panel, SHOW)) {
		return 1;
	} else {
		return 0;
	}
}

void inv_show(Inventory* _inv) {
	if (_inv != NULL) {
		set(_inv.panel, SHOW);
		
		// Show items
		_inv.itr = _inv.head;
		int i = 0;
		var vScale = screen_size.y / 1200;
		while(_inv.itr != NULL) {
			if (_inv.itr.panel != NULL)
			{	
				set(_inv.itr.panel, SHOW);
				_inv.itr.panel.pos_x = _inv.panel.pos_x + 1 + (INV_ITEMS_OFFSET_X + (INV_ITEM_GAP * (i%INV_ITEMS_X)) + (INV_ITEM_SIZE*(i%INV_ITEMS_X))) * _inv.itr.panel.scale_x;
				_inv.itr.panel.pos_y = _inv.panel.pos_y + 1 + (INV_ITEMS_OFFSET_Y + (INV_ITEM_GAP * (integer(i/INV_ITEMS_X))) + (INV_ITEM_SIZE*(integer(i/INV_ITEMS_X)))) * _inv.itr.panel.scale_y;		
				_inv.itr.panel.scale_x = vScale;
				_inv.itr.panel.scale_y = vScale;
			}
			i+=1;
			inv_increate_iterator(_inv);
		}
	}
}

void inv_hide(Inventory* _inv) {
	if (_inv != NULL) {
		reset(_inv.panel, SHOW);
		reset(_inv.itemDescription, SHOW);
		
		// Hide items
		_inv.itr = _inv.head;
		int i = 0;
		while(_inv.itr != NULL) {
			if (_inv.itr.panel != NULL)
			{	
				reset(_inv.itr.panel, SHOW);
			}
			i+=1;
			inv_increate_iterator(_inv);
		}
	}
}

void inv_set_pos(Inventory* _inv, int _x, int _y) {
	if (_inv != NULL) {
		if (_inv.panel != NULL) {
			_inv.panel.pos_x = _x;
			_inv.panel.pos_y = _y;
			_inv.itemDescription.pos_x = _inv.panel.pos_x + INV_DESC_TEXT_POS_X;
			_inv.itemDescription.pos_y = _inv.panel.pos_y + INV_DESC_TEXT_POS_Y;
		}
	}
}

void inv_add_item(Inventory* _inv, Item* _item) {
	if (_inv == NULL || _item == NULL) return;
	
	// Inventory full?
	if (_inv.count >= INV_ITEMS_X * INV_ITEMS_Y) {
		return;
	}
	
	if(_inv.tail == NULL) {
      _inv.head = _inv.tail = _item;
      _inv.head.next = NULL;
      _inv.head.prev = NULL;
      layer_sort(_item.panel, _inv.panel.layer+1);
   } else {
      _item.prev = _inv.tail;
      _item.next = NULL;
      _inv.tail.next = _item;
      _inv.tail = _item;
      layer_sort(_item.panel, _inv.panel.layer+1);
   }
   // Assert an inventory to an item to be able to show
   // an item description
   _item.inv = _inv;
	_inv.count += 1;
	
	if (inv_is_visible(_inv) == 1) {
		inv_hide(_inv);
		inv_show(_inv);
	}
}

void inv_remove_item(Inventory* _inv, Item* _item) {
	
	#ifdef DEBUG_MODE
		if (_inv == NULL) printf("inv_remove_item - inv is null");
		if (_item == NULL) printf("inv_remove_item - item is null");
	#endif
	
	if (_inv == NULL || _item == NULL) return;
	Item* tempItem = NULL;
	_inv.itr = _inv.head;
	// Browse all items
	while(_inv.itr != NULL) {
		// Found the right one?
		if (_inv.itr == _item) {
			tempItem = _inv.itr;
			// Is it the first in the list?
			if (tempItem == _inv.head) {
				_inv.head = _inv.head.next;
			} else {
				// ... or the last?
				if (tempItem == _inv.tail) {
					_inv.tail = _inv.tail.prev;
				}
			}
			// Linkt items in front and behind
			if (tempItem.prev != NULL) {
				tempItem.prev.next = tempItem.next;
			}
			if (tempItem.next != NULL) {
				tempItem.next.prev = tempItem.prev;
			}
			if (_inv.head == NULL) {
				_inv.tail = NULL;
			}
			
			// Hide item
			reset(_item.panel, SHOW);
			
			_inv.count -=1;
			break;
		}
		inv_increate_iterator(_inv);
	}
	//sys_free(_item); makes inventory go crashy crashy -.-
}

Item* inv_create_item(int _id, STRING* _name, STRING* _descr, int _value) {
	Item* item = sys_malloc(sizeof(Item));
	item.id = _id;
	item.name = str_create("");
	if (_name != NULL) str_cpy(item.name, _name);
	item.description = str_create("");
	if (_descr != NULL) str_cpy(item.description, _descr);
	item.value = _value;
	item.image = bmap_createblack(INV_ITEM_SIZE, INV_ITEM_SIZE, 24);
	bmap_fill(item.image, vector(integer(random(255)),integer(random(255)),integer(random(255))), 100);
	item.panel = pan_create("", INV_PANEL_LAYER + 1);
	item.panel.size_x = INV_ITEM_SIZE;
	item.panel.size_y = INV_ITEM_SIZE;
	set(item.panel, FILTER);
	pan_setbutton(item.panel, 0, 0, 0, 0, item.image, item.image, item.image, item.image, inv_item_click, inv_item_leave, inv_item_enter);
	// Assert item to panel so that the item can be referenced from the panel (click events)
	item.panel.skill_x = item;
	return item;
}

Item* inv_create_item(int _id, STRING* _name, STRING* _descr, int _value, BMAP* _bitmap) {
	Item* item = sys_malloc(sizeof(Item));
	item.id = _id;
	item.name = str_create("");
	if (_name != NULL) str_cpy(item.name, _name);
	item.description = str_create("");
	if (_descr != NULL) str_cpy(item.description, _descr);
	item.value = _value;
	item.image = _bitmap;
	item.panel = pan_create("", INV_PANEL_LAYER + 1);
	item.panel.size_x = INV_ITEM_SIZE;
	item.panel.size_y = INV_ITEM_SIZE;
	set(item.panel, FILTER);
	pan_setbutton(item.panel, 0, 0, 0, 0, item.image, item.image, item.image, item.image, inv_item_click, inv_item_leave, inv_item_enter);
	// Assert item to panel so that the item can be referenced from the panel (click events)
	item.panel.skill_x = item;
	return item;
}


// Internal use
void inv_increate_iterator(Inventory* _inv) {
	if(_inv.itr != NULL) {
		_inv.itr = _inv.itr.next;
	}
}

void inv_hide_items(Inventory* _inv) {
	if (_inv != NULL) {
		_inv.itr = _inv.head;
		while(_inv.itr != NULL)
		{
			if (_inv.itr.panel != NULL)
			{
				reset(_inv.itr.panel, SHOW);
			}
			inv_increate_iterator(_inv);
		}
	}
}

void inv_show_items(Inventory* _inv) {
	if (_inv != NULL) {
		_inv.itr = _inv.head;
		while(_inv.itr != NULL)
		{
			if (_inv.itr.panel != NULL)
			{
				set(_inv.itr.panel, SHOW);
			}
			inv_increate_iterator(_inv);
		}
	}
}

Item* inv_item_search(Inventory* inv, int id)
{
	if (inv == NULL) 
	{
		return NULL;
	}
	
	Item* searchItem = NULL;
	inv.itr = inv.head;
	while(inv.itr != NULL)
	{
		searchItem = (Item*)inv.itr.panel.skill_x;
		if (searchItem.id == id) 
			return searchItem;
		inv_increate_iterator(inv);
	}
	return NULL;
}

void inv_item_click(var _buttonNumber, PANEL* _panel) {
	if (_panel != NULL) {
		
		if (itemInHand != NULL) { // Does the player carry an item in his hand? -> combine
		
			Item* item = (Item*)_panel.skill_x;
			int targetId;
			int resultId;
			resultId = COMBINATION_combine(itemInHand->id, item->id, &targetId);
			ITEM* handItem = ITEM_get(itemInHand->id);

			if (resultId != ITEM_NONE)
			{
				//item in inventory
				if(handItem->destroyable == 0)
				{
					inv_add_item(item.inv, itemInHand);
				}
				mousemgr_set(MOUSE_LOOK, NULL);		
	
				ITEM* resultItem = ITEM_get(resultId);
				if (resultItem != NULL)
				{
					//morph defined target item
					//inventory does not need morphing -> just drop item with targetId and add new item with resultId
					if (targetId != ITEM_NONE)
					{
						//this is only triggered if a targetId for a morph was specified
						//for inventory normally this can be solved in the xml markup without morphing but for convenience
						//it should be supported.

						Item* searchItem = NULL;
						searchItem = inv_item_search((Inventory*)item.inv, targetId);
						if (searchItem != NULL)
						{
							inv_remove_item(searchItem.inv,searchItem);
						}
					}
					//create new inventory item with resultId;
					//item->imgfile should be tested for file existance or NULL before usage here. wrong file names will cause strange error messages
					Item *resultIdItem = inv_create_item(resultId, resultItem->name, "Item description", 0, bmap_create(resultItem->imgfile));
					inv_add_item(inventory, resultIdItem);
				}		
	
				ITEM* myItem = ITEM_get(item->id);
				if(myItem->destroyable != 0)
				{
					inv_remove_item(item.inv,item);
				}
					
				EVENT_trigger(resultId);
			}
			else
			{
				inv_add_item(inventory, itemInHand);
			}
			inv_hide(itemInHand.inv);
			inv_show(itemInHand.inv);
			itemInHand = NULL;
			mousemgr_set(MOUSE_DEFAULT, NULL);
			
			/*inv_add_item(itemInHand.inv,itemInHand);
			inv_hide(itemInHand.inv);
			inv_show(itemInHand.inv);
			itemInHand = NULL;
			mousemgr_set(MOUSE_DEFAULT, NULL);*/
		} else {
			if (_panel.skill_x != NULL) { // The reference between panel and item is created using skill_x
				Item* tempItem = (Item*)_panel.skill_x;
				if (tempItem != NULL) {
					itemInHand = tempItem;
					inv_remove_item(tempItem.inv,tempItem);
					inv_hide(tempItem.inv);
					inv_show(tempItem.inv);
					mousemgr_hint(NULL);
					mousemgr_set(MOUSE_DEFAULT, tempItem.image);
				}
			}
		}
	}
}

void inv_item_enter(var _buttonNumber, PANEL* _panel) {
	if (_panel != NULL) {
		if (_panel.skill_x != NULL) {
			Item* tempItem = (Item*)_panel.skill_x;
			if (tempItem != NULL) {
				Inventory* tempInv = (Inventory*)(tempItem.inv);
				if (tempInv != NULL) {
					if (tempInv.itemDescription != NULL) {
						// Show item description
						mousemgr_hint(tempItem->name);
						if (itemInHand != NULL)
						{
							mousemgr_set(MOUSE_USE, itemInHand->image);
						}
					}
				}
			}
		}
	}
}

void inv_item_leave(var _buttonNumber, PANEL* _panel) {
	if (_panel != NULL) {
		if (_panel.skill_x != NULL) {
			Item* tempItem = (Item*)_panel.skill_x;
			if (tempItem != NULL) {
				Inventory* tempInv = (Inventory*)(tempItem.inv);
				if (tempInv != NULL) {
					if (tempInv.itemDescription != NULL) {
						// Hide item description
						mousemgr_hint(NULL);
						if (itemInHand != NULL)
						{
							mousemgr_set(MOUSE_DEFAULT, itemInHand->image);
						}
					}
				}
			}
		}
	}
}

void inv_on_click(PANEL* _pan) {
	
	// Is there an item to be placed in the inventory of this panel?
	if (itemInHand != NULL) {
		if (_pan.skill_x != NULL) {
			Inventory* inv = (Inventory*)_pan.skill_x;
			inv_add_item(inv, itemInHand);
			itemInHand = NULL;
			mousemgr_set(MOUSE_DEFAULT, NULL);
			inv_hide(inv);
			inv_show(inv);
		}
	}
}

void inv_loop() {
	if (mouse_right || key_esc) {
		while(mouse_right || key_esc) wait(1);
		if (itemInHand != NULL) {
			if (itemInHand.inv != NULL) {
				
				inv_add_item(itemInHand.inv, itemInHand);
				inv_hide(itemInHand.inv);
				inv_show(itemInHand.inv);
				itemInHand = NULL;
				mousemgr_set(MOUSE_DEFAULT, NULL);
			}
		}
	}
}


void inv_resize()
{
	if (inv__resizeEv != NULL)
	{
		inv__resizeEv();
	}

	if (inventory != NULL)
	{
		var vScale = screen_size.y / 1200;
		
		//this is sort of hacky
		ptr_remove(inventory.panel.bmap);
		inventory.panel.bmap = bmap_createblack(screen_size.x, inventory.panel.size_y, 24);
		inventory.panel.size_x = screen_size.x;
		inventory.panel.scale_y = vScale; //leave original panel y size due to missing height reference
		inv_set_pos(inventory, 0, screen_size.y - (bmap_height(inventory.panel.bmap) * vScale));	
		//todo: scale whole inventory - not working due to item positioning
		inventory.itr = inventory.head;
		while(inventory.itr != NULL)
		{
			if (inventory.itr.panel != NULL)
			{
				inventory.itr.panel.scale_x = vScale;
				inventory.itr.panel.scale_y = vScale;
			}
			inv_increate_iterator(inventory);
		}
		if (inv_is_visible(inventory) == 1) 
		{
			inv_hide(inventory);
			inv_show(inventory);
		}		
	}
}

#endif