#ifndef ITEMS_H
#define ITEMS_H

#define ITEM_NONE -1

#include "list.h"

typedef struct item 
{
	int id;
	STRING* name;
	STRING* imgfile;
	STRING* entfile; 

	int collectable;
	int destroyable;

	LIST* sequences;
	
	//internal information
	int wasRemoved;
	int progress;
	int wasMorphedTo;
} ITEM;

typedef struct sequence
{
	SOUND* snd_interact;
	STRING* description;
	int resultId;
} SEQUENCE;


int ITEM_load(STRING* file);
void ITEM_close();
ITEM* ITEM_get(int id);
void ITEM_resetProgress();
var ITEM_isLastSequence(ITEM* item);
var ITEM_isBeingCollected(ITEM* item);
var ITEM_isNowCollectable(ITEM* item);
int ITEM_interaction(ITEM* item);
void ITEM_collect(ITEM* item);

#include "items.c"
#endif