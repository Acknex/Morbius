/*
 *******************************************************************************
 * list.c
 * Creation date: 07.03.2007
 * Author:        Firoball
 *
 *******************************************************************************
 * $Date: 2010-08-05 00:23:10 +0200 (Do, 05 Aug 2010) $
 * $Revision: 4 $
 * $Author: Firo $
 *
 *******************************************************************************
 * Description
 *
 * Script for list management
 *
 * Comments
 * 
 * for short descriptions see comments in list.h
 *
 *******************************************************************************
 */

/* ----- INCLUDES ----- */


/* ----- GLOBALS ----- */


/* ----- EXTERNAL FUNCTIONS ----- */


void LIST_append(LIST* psHost, void* psNewItem)
{
	/* array limit reached - extend it */
	if ( ((psHost->iCount) + 1) == (LIST_ARRAY_STEPS * (psHost->iRange)) )
	{
		LIST__extend(psHost);	
	}

	/* add new item to list */
	(psHost->ppList)[psHost->iCount] = psNewItem;
	psHost->iCount ++;
}

void* LIST_getItem(LIST* psHost, int iIndex)
{
	/* return NULL for invalid indices */
	if ((iIndex >= psHost->iCount) || (iIndex < 0))
		return (NULL);
	else
		return ((psHost->ppList)[iIndex]); 
}

int LIST_items(LIST* psHost)
{
	return (psHost->iCount);
}

LIST* LIST_create()
{
	LIST* psTmpList;
	psTmpList = (LIST*)malloc(sizeof(LIST));
	LIST__init(psTmpList);
	return (psTmpList);
}

void LIST_remove(LIST* psHost)
{
	free(psHost);
	psHost = NULL;	
}

void LIST_removeAll(LIST* psHost)
{
	int i;

	/* cycle through pointer list */
	for(i = 0; i < LIST_items(psHost); i++)
		free(LIST_getItem(psHost, i));
	free(psHost);
	psHost = NULL;	
}

void LIST_removeItem(LIST* psHost, int iIndex)
{
	/* ignore invalid indices */
	if ((iIndex < psHost->iCount) && (iIndex >= 0))
		(psHost->ppList)[iIndex] = NULL;
}


/* ----- INTERNAL FUNCTIONS ----- */


void LIST__init(LIST *psHost)
{
	int i;
	psHost->iCount = 0;
	psHost->iRange = 1;	
	psHost->ppList = (void**)malloc(sizeof(void*) * (psHost->iRange) * LIST_ARRAY_STEPS);

	for (i = 0; i < LIST_ARRAY_STEPS * (psHost->iRange); i++)
		(psHost->ppList)[i] = NULL; 
}


void LIST__extend(LIST* psHost)
{
	int i;
	void** ppNewItemList;
	
	#ifdef SYSMSG_ACTIVE
	SYSMSG_print(SYSMSG_SHOW_DEBUG, "LIST__extend: List extended");
	#endif
	
	/* increase range for list pointer array */
	psHost->iRange++;
	/* allocate new pointer list */
	ppNewItemList = (void**)malloc(sizeof(void*) * (psHost->iRange) * LIST_ARRAY_STEPS);
	/* copy old pointer list */
	for (i = 0; i < psHost->iCount; i++)
	{
		ppNewItemList[i] = (psHost->ppList)[i];
	}
	/* remove old pointer list */
	free(psHost->ppList);
	/* point to new list */
	psHost->ppList = ppNewItemList;
}