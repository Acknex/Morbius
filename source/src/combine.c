#include <acknex.h>
#include "combine.h"
#include "hud.h"
#include "xmlreader.h"
#include "list.h"

#include "xmlreader.c"
#include "list.c"


LIST* COMBINATION__combinationList;
XMLFILE* COMBINATION__xml;

void COMBINATION__copyFromXml(COMBINATION* combination, XMLPAR* tag);
void COMBINATION__cleanup(COMBINATION* combination);


int COMBINATION_load(STRING* file)
{
	XMLPAR* hndl;
	XMLPAR* xmlCombination;
	XMLPAR* xmlList;
	COMBINATION* combination;
	var count;
	var i;
	
	COMBINATION__combinationList = LIST_create();	
	COMBINATION__xml = XMLFILE_create(_chr(file));	
	hndl = XMLFILE_parse(COMBINATION__xml); 
	
	if (hndl != NULL)
	{
		xmlList = XMLPAR_getElementByTag(hndl, "combinations");
		count = XMLPAR_getTagElements(xmlList);
		for (i = 0; i < count; i++)
		{
			xmlCombination = XMLPAR_getElementByIndex(xmlList, i);
			combination = (COMBINATION*)malloc(sizeof(COMBINATION));
			COMBINATION__copyFromXml(combination, xmlCombination);
			LIST_append(COMBINATION__combinationList, (void*)combination);
		}
		return 1;
	}
	printf("COMBINATION_load: parse error. File content or file path invalid?");
	
	return 0;
}

void COMBINATION_close()
{
	int i;
	COMBINATION* tmpCombination;

	if (COMBINATION__combinationList != NULL)
	{
		for (i = 0; i < LIST_items(COMBINATION__combinationList); i++)
		{
			tmpCombination = (COMBINATION*)LIST_getItem(COMBINATION__combinationList, i);
			COMBINATION__cleanup(tmpCombination);
			LIST_removeItem(COMBINATION__combinationList, i);
		}
		LIST_remove(COMBINATION__combinationList);
	}

	if (COMBINATION__xml != NULL)
	{
		XMLFILE_remove(COMBINATION__xml);
	}
}

int COMBINATION_combine(int id1, int id2, int* morphtargetId)
{
	int i;
	COMBINATION* tmpCombination;

	for (i = 0; i < LIST_items(COMBINATION__combinationList); i++)
	{
		tmpCombination = (COMBINATION*)LIST_getItem(COMBINATION__combinationList, i);
		if (
			(tmpCombination->id1 == id1 && tmpCombination->id2 == id2) ||
			(tmpCombination->id1 == id2 && tmpCombination->id2 == id1)
		)
		{
			//found
			if (tmpCombination->snd_interact != NULL)
			{
				SOUNDMGR_scheduleSound(tmpCombination->snd_interact);
			}

			if (tmpCombination->description != NULL)
			{
				if (tmpCombination->snd_interact != NULL)		
					HUD_showDescription(tmpCombination->description, tmpCombination->snd_interact);
				else
					HUD_showDescription(tmpCombination->description);
			}

			*morphtargetId = tmpCombination->morphtargetId;
			return tmpCombination->resultId;
		}
	}

	//not found
	*morphtargetId = -1;
	return -1;
}

void COMBINATION__copyFromXml(COMBINATION* combination, XMLPAR* tag)
{
	XMLATTRIB* attrib;
	STRING* str;

	attrib = XMLATTRIB_getElementByAttribute(tag, "id1");
	if (attrib != NULL)
		combination->id1 = str_to_int(XMLATTRIB_getPContent(attrib));
	else
		combination->id1 = -1;

	attrib = XMLATTRIB_getElementByAttribute(tag, "id2");
	if (attrib != NULL)
		combination->id2 = str_to_int(XMLATTRIB_getPContent(attrib));
	else
		combination->id2 = -1;

	attrib = XMLATTRIB_getElementByAttribute(tag, "morphtarget");
	if (attrib != NULL)
		combination->morphtargetId = str_to_int(XMLATTRIB_getPContent(attrib));
	else
		combination->morphtargetId = -1;

	attrib = XMLATTRIB_getElementByAttribute(tag, "result");
	if (attrib != NULL)
		combination->resultId = str_to_int(XMLATTRIB_getPContent(attrib));
	else
		combination->resultId = -1;

	attrib = XMLATTRIB_getElementByAttribute(tag, "sound");
	combination->snd_interact = NULL;
	if (attrib != NULL)
	{
		combination->snd_interact = snd_create(XMLATTRIB_getPContent(attrib));
	}

	combination->description = NULL;
	attrib = XMLATTRIB_getElementByAttribute(tag, "description");
	if (attrib != NULL)
	{
		str = str_create("");
		XMLATTRIB_getContent(attrib, str);
		combination->description = str;
	}
	
}

void COMBINATION__cleanup(COMBINATION* combination)
{
	int i;
	
	if (combination == NULL)
		return;
	
	if (combination->snd_interact != NULL)
		ptr_remove(combination->snd_interact);

	if (combination->description != NULL)
		ptr_remove(combination->description);
}
