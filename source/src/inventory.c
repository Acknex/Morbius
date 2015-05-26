#ifndef _INVENTORY_C_
#define _INVENTORY_C_

#include "itemmgr.h"

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
	
	// Create item description
	inv.itemDescription = txt_create(1, INV_PANEL_LAYER + 1);
	set(inv.itemDescription, OUTLINE | WWRAP);
	inv.itemDescription.size_x = bmap_width(inv.panel.bmap);
	inv.itemDescription.font = fontInventoryDescription;
	str_cpy((inv.itemDescription.pstring)[0], "Dies ist ein\nTest!");
	
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
		while(_inv.itr != NULL) {
			if (_inv.itr.panel != NULL)
			{	
				set(_inv.itr.panel, SHOW);
				_inv.itr.panel.pos_x = _inv.panel.pos_x + 1 + INV_ITEMS_OFFSET_X + (INV_ITEM_GAP * (i%INV_ITEMS_X)) + (INV_ITEM_SIZE*(i%INV_ITEMS_X));
				_inv.itr.panel.pos_y = _inv.panel.pos_y + 1 + INV_ITEMS_OFFSET_Y + (INV_ITEM_GAP * (integer(i/INV_ITEMS_X))) + (INV_ITEM_SIZE*(integer(i/INV_ITEMS_X)));		
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

						//I will go to hell for this...
						Item* searchItem = NULL;
						Inventory* tempInv = (Inventory*)item.inv;
						tempInv.itr = tempInv.head;
						while(tempInv.itr != NULL)
						{
							searchItem = (Item*)tempInv.itr.panel.skill_x;
							if (searchItem.id == targetId) 
								break;
							inv_increate_iterator(item.inv);
						}

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
				//TODO: play random fail sound
				//snd_play(...);
				//TODO: show random fail message
				//HUD_showDescription(msg);			
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
						//set(tempInv.itemDescription, SHOW);
						//str_cpy((tempInv.itemDescription.pstring)[0], tempItem.name);
						mousemgr_hint(tempItem->name);
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
						//reset(tempInv.itemDescription, SHOW);
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

#endif