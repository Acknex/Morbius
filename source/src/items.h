#ifndef ITEMS_H
#define ITEMS_H

#define ITEM_NONE -1

typedef struct item 
{
	int id;
	STRING* name;
	STRING* imgfile;
	STRING* entfile; 

	int collectable;
	int destroyable;

	//LIST* sequences;
	STRING* description[3];
	var desc_count;

	SOUND* snd_interact[3]; 
	var snd_count;	
} ITEM;

typedef struct sequence
{
	SOUND* snd_interact;
	STRING* description;
} SEQUENCE;


ITEM* ITEM_get(int id);
void ITEM_sndrnd(ITEM* item);
void ITEM_snd(ITEM* item, var soundnum);

int ITEM_load(STRING* file);
void ITEM_close();

#include "items.c"
#endif