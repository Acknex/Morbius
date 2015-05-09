typedef struct item 
{
	int id;
	STRING* name;
	STRING* description;

	STRING* imgfile;
	STRING* entfile; 

	char collectable;
	SOUND** snd_interact 
	int snd_count;	
} ITEM;



ITEM* Item_get(int id);
void Item_sndrnd(int id);
void Item_snd(int id, int soundnum);
