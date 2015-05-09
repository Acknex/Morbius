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
	
	if (ITEM_load("..\\items\\items.xml"))
	{
		//cnt = ITEM_count();
		//for (i = 0; i < cnt; i++)
		{
			item = ITEM_get(1);
			if (item == NULL) error("möp");
			str = str_printf(NULL, "id %d - name %s", (int)item->id, _chr(item->name));
			error(str);
		}
		ITEM_close();
	}
	else
	{
		error("kaputt");
	}
	
	while(key_esc == 0)
	{
		wait(1);
	}
	
	sys_exit("");
}