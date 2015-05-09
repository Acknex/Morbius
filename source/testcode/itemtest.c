#include <acknex.h>

//#define PRAGMA_POINTER
#define PRAGMA_PATH "..\src\"

#include "items.h"
#include "combine.h"
#include "inventory.h"
#include "itemmgr.h"
#include <default.c>

void main()
{
	mouse_mode = 4;
	video_mode = 10;
	
	//random_seed(0);
itemmgr_init();	
	level_load("itemtest.wmb");
	if (ITEM_load("..\\items\\items.xml"))
	{
		//ITEM* item = ITEM_get(1);
		//SEQUENCE* seq = (SEQUENCE*)LIST_getItem(item->sequences,1);
		//snd_play(seq->snd_interact, 100, 0);
/*		ITEM_snd(item, 0);
		wait(-1);
		ITEM_snd(item, 1);
		wait(-1);
		ITEM_sndrnd(item);*/		
	}
	else
	{
		error("item kaputt");
	}
	
	if (COMBINATION_load("..\\items\\items.xml"))
	{
		int targetid;
		//int resultid = COMBINATION_combine(8,7, &targetid);

		//error(str_for_num(NULL, targetid));
		//error(str_for_num(NULL, resultid));
	}
	else
	{
		error("item kaputt");
	}
	
	// Create inventory
	inventory = inv_create(NULL, screen_size.x, 50);
	inv_set_pos(inventory, 0, screen_size.y - bmap_height(inventory.panel.bmap));
	inv_show(inventory);
		
	while(key_esc == 0)
	{
		wait(1);
	}
	
	COMBINATION_close();
	ITEM_close();
	sys_exit(""); 
}