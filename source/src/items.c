#include <acknex.h>
#include "items.h"
#include "xmlreader.h"
#include "list.h"

#include "xmlreader.c"
#include "list.c"


#define ITEM_VOLUME 100

LIST* ITEMS__itemList;
XMLFILE* ITEMS__xml;

void ITEM__copyFromXml(ITEM* item, XMLPAR* tag);
void ITEM__cleanup(ITEM* item);


int ITEM_load(STRING* file)
{
	XMLPAR* hndl;
	XMLPAR* xmlItem;
	XMLPAR* xmlList;
	ITEM* item;
	var count;
	var i;
	
	ITEMS__itemList = LIST_create();	
	ITEMS__xml = XMLFILE_create(_chr(file));	
	hndl = XMLFILE_parse(ITEMS__xml); 
	
	if (hndl != NULL)
	{
		xmlList = XMLPAR_getElementByTag(hndl, "items");
		count = XMLPAR_getTagElements(xmlList);
		for (i = 0; i < count; i++)
		{
			xmlItem = XMLPAR_getElementByIndex(xmlList, i);
			item = (ITEM*)malloc(sizeof(ITEM));
			ITEM__copyFromXml(item, xmlItem);
			LIST_append(ITEMS__itemList, (void*)item);
		}
		return 1;
	}
	printf("ITEM_load: parse error. File content or file path invalid?");
	
	return 0;
}

void ITEM_close()
{
	int i;
	ITEM* tmpItem;

	if (ITEMS__itemList != NULL)
	{
		for (i = 0; i < LIST_items(ITEMS__itemList); i++)
		{
			tmpItem = (ITEM*)LIST_getItem(ITEMS__itemList, i);
			ITEM__cleanup(tmpItem);
			LIST_removeItem(ITEMS__itemList, i);
		}
		LIST_remove(ITEMS__itemList);
	}

	if (ITEMS__xml != NULL)
	{
		XMLFILE_remove(ITEMS__xml);
	}
}

ITEM* ITEM_get(int id)
{
	int i;
	ITEM* tmpItem;
	
	for (i = 0; i < LIST_items(ITEMS__itemList); i++)
	{
		tmpItem = (ITEM*)LIST_getItem(ITEMS__itemList, i);
		if (tmpItem->id == id)
		{
			return tmpItem;
		}
	}
	
	return NULL;
}

void ITEM_snd(int id, var soundnum)
{
	ITEM* tmpItem;
	if (id < LIST_items(ITEMS__itemList) && id >= 0)
	{
		tmpItem = ITEM_get(id);
		if (soundnum > 0 && soundnum < tmpItem->snd_count)
		{
			snd_play(tmpItem->snd_interact[soundnum], ITEM_VOLUME, 0);
		}
	}
}

void ITEM_sndrnd(int id)
{
	ITEM* tmpItem;
	var soundnum;
	if (id < LIST_items(ITEMS__itemList) && id >= 0)
	{
		tmpItem = ITEM_get(id);
		if (soundnum > 0 && soundnum < tmpItem->snd_count)
		{
			soundnum = integer(random(tmpItem->snd_count));
			snd_play(tmpItem->snd_interact[soundnum], ITEM_VOLUME, 0);
		}
	}
}

void ITEM__copyFromXml(ITEM* item, XMLPAR* tag)
{
	XMLATTRIB* attrib;
	STRING* str;

	attrib = XMLATTRIB_getElementByAttribute(tag, "id");
	if (attrib != NULL)
		item->id = str_to_int(XMLATTRIB_getPContent(attrib));
	else
		item->id = -1;

	str = str_create("");
	attrib = XMLATTRIB_getElementByAttribute(tag, "name");
	if (attrib != NULL)
		XMLATTRIB_getContent(attrib, str);
	item->name = str;
	
	str = str_create("");
	attrib = XMLATTRIB_getElementByAttribute(tag, "description");
	if (attrib != NULL)
		XMLATTRIB_getContent(attrib, str);
	item->description = str;
	
	str = str_create("");
	attrib = XMLATTRIB_getElementByAttribute(tag, "imgfile");
	if (attrib != NULL)
		XMLATTRIB_getContent(attrib, str);
	item->imgfile = str;
	
	str = str_create("");
	attrib = XMLATTRIB_getElementByAttribute(tag, "entfile");
	if (attrib != NULL)
		XMLATTRIB_getContent(attrib, str);
	item->entfile = str;
	
	attrib = XMLATTRIB_getElementByAttribute(tag, "collectable");
	if (attrib != NULL)
		item->collectable = 1;
	else
		item->collectable = 0;
	

	attrib = XMLATTRIB_getElementByAttribute(tag, "destroyable");
	if (attrib != NULL)
		item->destroyable = 1;
	else
		item->destroyable = 0;

	/* load sounds */
	item->snd_count = 0;
	item->snd_interact[0] = NULL;
	item->snd_interact[1] = NULL;
	item->snd_interact[2] = NULL;

	attrib = XMLATTRIB_getElementByAttribute(tag, "sound1");
	if (attrib != NULL)
	{
		item->snd_interact[item->snd_count] = snd_create(XMLATTRIB_getPContent(attrib));
		item->snd_count++;
	}

	attrib = XMLATTRIB_getElementByAttribute(tag, "sound2");
	if (attrib != NULL)
	{
		item->snd_interact[item->snd_count] = snd_create(XMLATTRIB_getPContent(attrib));
		item->snd_count++;
	}

	attrib = XMLATTRIB_getElementByAttribute(tag, "sound3");
	if (attrib != NULL)
	{
		item->snd_interact[item->snd_count] = snd_create(XMLATTRIB_getPContent(attrib));
		item->snd_count++;
	}

			//printf("tag %s id %s", _chr(XMLPAR_getPTag(tag)), _chr(XMLATTRIB_getPContent(attrib)));
	//printf("id %d name %s", item->id, _chr(item->name));
}

void ITEM__cleanup(ITEM* item)
{
	int i;
	
	if (item == NULL)
		return;
	
	if (item->name != NULL)
		ptr_remove(item->name);
	if (item->description != NULL)
		ptr_remove(item->description);
	if (item->imgfile != NULL)
		ptr_remove(item->imgfile);
	if (item->entfile != NULL)
		ptr_remove(item->entfile);
		
	for (i = 0; i < item->snd_count; i++)
	{
		ptr_remove(item->snd_interact[i]);
	}
}



