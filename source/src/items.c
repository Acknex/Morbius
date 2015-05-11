#include <acknex.h>
#include "items.h"
#include "hud.h"
#include "xmlreader.h"
#include "list.h"

#include "xmlreader.c"
#include "list.c"


#define ITEM_VOLUME 100

LIST* ITEMS__itemList;
XMLFILE* ITEMS__xml;

void ITEM__copyFromXml(ITEM* item, XMLPAR* tag);
void ITEM__cleanup(ITEM* item);
void ITEM__loadSequences(ITEM* item, XMLPAR* tag);
void ITEM__copySequqenceFromXml(SEQUENCE* sequence, XMLPAR* tag);

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
			ITEM__loadSequences(item, xmlItem);
			LIST_append(ITEMS__itemList, (void*)item);
		}
		ITEM_resetProgress();
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
	
	if (ITEMS__itemList == NULL)
		return NULL;
		
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

void ITEM_resetProgress()
{
	int i;
	ITEM* tmpItem;
	
	for (i = 0; i < LIST_items(ITEMS__itemList); i++)
	{
		tmpItem = (ITEM*)LIST_getItem(ITEMS__itemList, i);
		tmpItem->wasRemoved = 0;
		tmpItem->progress = 0;
		tmpItem->wasMorphedTo = -1;
	}
}

var ITEM_isLastSequence(ITEM* item)
{
	if (item == NULL)
		return;
	
	if (item->progress == LIST_items(item->sequences))
		return 1;
	else
		return 0;
}

int ITEM_interaction(ITEM* item)
{
	SEQUENCE* tmpSequence;
	
	if (item == NULL)
		return;

	item->progress = minv(item->progress, LIST_items(item->sequences) - 1);
	if (item->progress >= 0)
	{
		tmpSequence = (SEQUENCE*)LIST_getItem(item->sequences, item->progress);
		if (tmpSequence->snd_interact != NULL)
		{
			snd_play(tmpSequence->snd_interact, ITEM_VOLUME, 0);
		}
		
		if (tmpSequence->description != NULL)
		{
			HUD_showDescription(tmpSequence->description);
		}

		//get stuck on last step
		if (item->progress < LIST_items(item->sequences))
			item->progress++;
		
		return tmpSequence->resultId;
	}	
	
	return ITEM_NONE;
}

void ITEM_collect(ITEM* item)
{
	if (item != NULL)
	{
		item->wasRemoved = 1;
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
		item->id = ITEM_NONE;

	str = str_create("");
	attrib = XMLATTRIB_getElementByAttribute(tag, "name");
	if (attrib != NULL)
		XMLATTRIB_getContent(attrib, str);
	item->name = str;
	
	item->imgfile = NULL;
	attrib = XMLATTRIB_getElementByAttribute(tag, "imgfile");
	if (attrib != NULL)
	{
		str = str_create("");
		XMLATTRIB_getContent(attrib, str);
		item->imgfile = str;
	}
	
	item->entfile = NULL;
	attrib = XMLATTRIB_getElementByAttribute(tag, "entfile");
	if (attrib != NULL)
	{
		str = str_create("");
		XMLATTRIB_getContent(attrib, str);
		item->entfile = str;
	}
	
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

	/* load descriptions 
	item->desc_count = 0;
	item->description[0] = NULL;
	item->description[1] = NULL;
	item->description[2] = NULL;

	attrib = XMLATTRIB_getElementByAttribute(tag, "description1");
	if (attrib != NULL)
	{
		str = str_create("");
		XMLATTRIB_getContent(attrib, str);
		item->description = str;
		item->desc_count++;
	}
	
	attrib = XMLATTRIB_getElementByAttribute(tag, "description2");
	if (attrib != NULL)
	{
		str = str_create("");
		XMLATTRIB_getContent(attrib, str);
		item->description = str;
		item->desc_count++;
	}
	
	attrib = XMLATTRIB_getElementByAttribute(tag, "description3");
	if (attrib != NULL)
	{
		str = str_create("");
		XMLATTRIB_getContent(attrib, str);
		item->description = str;
		item->desc_count++;
	}*/
	
	
	/* load sounds 
	item->snd_count = 0;
	item->snd_interact[0] = NULL;
	item->snd_interact[1] = NULL;
	item->snd_interact[2] = NULL;

	attrib = XMLATTRIB_getElementByAttribute(tag, "sound1");
	if (attrib != NULL)
	{
		item->snd_interact[item->snd_count] = snd_create(XMLATTRIB_getPContent(attrib));
		if (item->snd_interact[item->snd_count] != NULL)
			item->snd_count++;
	}

	attrib = XMLATTRIB_getElementByAttribute(tag, "sound2");
	if (attrib != NULL)
	{
		item->snd_interact[item->snd_count] = snd_create(XMLATTRIB_getPContent(attrib));
		if (item->snd_interact[item->snd_count] != NULL)
			item->snd_count++;
	}

	attrib = XMLATTRIB_getElementByAttribute(tag, "sound3");
	if (attrib != NULL)
	{
		item->snd_interact[item->snd_count] = snd_create(XMLATTRIB_getPContent(attrib));
		if (item->snd_interact[item->snd_count] != NULL)
			item->snd_count++;
	}*/
}

void ITEM__cleanup(ITEM* item)
{
	int i;
	SEQUENCE* sequence;
	
	if (item == NULL)
		return;
	
	if (item->name != NULL)
		ptr_remove(item->name);
	/*for (i = 0; i < item->desc_count; i++)
	{
		if (item->description != NULL)
			ptr_remove(item->description);
	}*/
	if (item->imgfile != NULL)
		ptr_remove(item->imgfile);
	if (item->entfile != NULL)
		ptr_remove(item->entfile);
		
	/*for (i = 0; i < item->snd_count; i++)
	{
		ptr_remove(item->snd_interact[i]);
	}*/
	
	if (item->sequences != NULL)
	{
		for (i = 0; i < LIST_items(item->sequences); i++)
		{
			sequence = (SEQUENCE*)LIST_getItem(item->sequences, i);
			if (sequence->snd_interact != NULL)
				ptr_remove(sequence->snd_interact);
			if (sequence->description != NULL)
				ptr_remove(sequence->description);
			LIST_removeItem(item->sequences, i);
		}		
	}
}

void ITEM__loadSequences(ITEM* item, XMLPAR* tag)
{
	XMLPAR* xmlSequence;
	XMLPAR* xmlList;
	SEQUENCE* sequence;
	var count;
	var i;
		
	if (tag != NULL)
	{
		item->sequences = LIST_create();	
		count = XMLPAR_getTagElements(tag);
		for (i = 0; i < count; i++)
		{
			xmlSequence = XMLPAR_getElementByIndex(tag, i);
			sequence = (SEQUENCE*)malloc(sizeof(SEQUENCE));
			ITEM__copySequqenceFromXml(sequence, xmlSequence);
			LIST_append(item->sequences, (void*)sequence);
		}
	}
}

void ITEM__copySequqenceFromXml(SEQUENCE* sequence, XMLPAR* tag)
{
	XMLATTRIB* attrib;
	STRING* str;

	attrib = XMLATTRIB_getElementByAttribute(tag, "sound");
	sequence->snd_interact = NULL;
	if (attrib != NULL)
	{
		sequence->snd_interact = snd_create(XMLATTRIB_getPContent(attrib));
	}
	
	sequence->description = NULL;
	attrib = XMLATTRIB_getElementByAttribute(tag, "description");
	if (attrib != NULL)
	{
		str = str_create("");
		XMLATTRIB_getContent(attrib, str);
		sequence->description = str;
	}
	
	attrib = XMLATTRIB_getElementByAttribute(tag, "result");
	if (attrib != NULL)
		sequence->resultId = str_to_int(XMLATTRIB_getPContent(attrib));
	else
		sequence->resultId = ITEM_NONE;

}


