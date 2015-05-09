#include <acknex.h>
#include "combine.h"
#include "xmlreader.h"
#include "list.h"

#include "xmlreader.c"
#include "list.c"

#define COMBINE_VOLUME 100



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
	
	COMBINATION__itemList = LIST_create();	
	COMBINATION__xml = XMLFILE_create(_chr(file));	
	hndl = XMLFILE_parse(COMBINATION__xml); 
	
	if (hndl != NULL)
	{
		xmlList = XMLPAR_getElementByTag(hndl, "combinations");
		count = XMLPAR_getTagElements(xmlList);
		for (i = 0; i < count; i++)
		{
			xmlItem = XMLPAR_getElementByIndex(xmlList, i);
			combination = (ITEM*)malloc(sizeof(COMBINATION));
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

	if (ITEMS__itemList != NULL)
	{
		for (i = 0; i < LIST_items(COMBINATION__combinationList); i++)
		{
			tmpCombination = (COMBINATION*)LIST_getItem(COMBINATION__combinationList, i);
			LIST_removeItem(COMBINATION__combinationList, i);
		}
		LIST_remove(ITEMS__itemList);
	}

	if (COMBINATION__xml != NULL)
	{
		XMLFILE_remove(COMBINATION__xml);
	}
}

int COMBINATION_combine(int id1, int id2, int* morphtargetId)
{
}

void COMBINATION__copyFromXml(COMBINATION* combination, XMLPAR* tag)
{
	XMLATTRIB* attrib;

	attrib = XMLATTRIB_getElementByAttribute(tag, "id1");
	if (attrib != NULL)
		item->id1 = str_to_int(XMLATTRIB_getPContent(attrib));
	else
		item->id1 = -1;

	attrib = XMLATTRIB_getElementByAttribute(tag, "id2");
	if (attrib != NULL)
		item->id2 = str_to_int(XMLATTRIB_getPContent(attrib));
	else
		item->id2 = -1;

	attrib = XMLATTRIB_getElementByAttribute(tag, "morphtarget");
	if (attrib != NULL)
		item->morphtargetId = str_to_int(XMLATTRIB_getPContent(attrib));
	else
		item->morphtargetId = -1;

	attrib = XMLATTRIB_getElementByAttribute(tag, "target");
	if (attrib != NULL)
		item->resultId = str_to_int(XMLATTRIB_getPContent(attrib));
	else
		item->resultId = -1;

}

