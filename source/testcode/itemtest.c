#include <acknex.h>

#define PRAGMA_POINTER
#define PRAGMA_PATH "..\src\"

#include "items.h"
#include "combine.h"
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
		error("item kaputt");
	}
	
	if (COMBINATION_load("..\\items\\items.xml"))
	{
		int targetid;
		int resultid = COMBINATION_combine(8,7, &targetid);

		error(str_for_num(NULL, targetid));
		error(str_for_num(NULL, resultid));
	}
	else
	{
		error("item kaputt");
	}
	
	while(key_esc == 0)
	{
		wait(1);
	}
	
	COMBINATION_close();
	ITEM_close();
	sys_exit(""); 
}