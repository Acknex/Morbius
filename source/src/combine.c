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
	XMLPAR* xmlItem;
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
			COMBINATION__copyFromXml(combination, xmlItem);
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
			tmpCombination = (ITEM*)LIST_getItem(COMBINATION__combinationList, i);
			COMBINATION__cleanup(tmpItem);
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
