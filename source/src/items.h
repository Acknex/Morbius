#ifndef ITEMS_H
#define ITEMS_H

#define ITEM_NONE -1

typedef struct item 
{
	int id;
	STRING* name;
	STRING* description;

	STRING* imgfile;
	STRING* entfile; 

	int collectable;
	int destroyable;
	SOUND* snd_interact[3]; 
	var snd_count;	
} ITEM;



ITEM* ITEM_get(int id);
void ITEM_sndrnd(int id);
void ITEM_snd(int id, var soundnum);

int ITEM_load(STRING* file);
void ITEM_close();

#include "items.c"
#endif