#include <acknex.h>

#define PRAGMA_POINTER
#define PRAGMA_PATH "..\src\"

#include "items.h"

void main()
{
	var i;
	var cnt;
	STRING* str;
	ITEM* item;
	
	random_seed(0);
	
	if (ITEM_load("..\\items\\items.xml"))
	{
		item = ITEM_get(1);
		if (item == NULL) error("möp");
		ITEM_snd(item, 0);
		wait(-1);
		ITEM_snd(item, 1);
		wait(-1);
		ITEM_sndrnd(item);		
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