#include <acknex.h>

#define PRAGMA_POINTER
#define PRAGMA_PATH "..\src\"

#include "items.h"
#include "itemmgr.h"
#include <default.c>

void main()
{
	mouse_mode = 4;
	
	//random_seed(0);
	
	level_load("itemtest.wmb");
	if (ITEM_load("..\\items\\items.xml"))
	{
		/*ITEM* item = ITEM_get(1);
		ITEM_snd(item, 0);
		wait(-1);
		ITEM_snd(item, 1);
		wait(-1);
		ITEM_sndrnd(item);*/		
	}
	else
	{
		error("kaputt");
	}
	
	while(key_esc == 0)
	{
		wait(1);
	}
	
	ITEM_close();
	sys_exit(""); 
}