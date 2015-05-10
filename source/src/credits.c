#include "credits.h"

STRING *msCreditsMusic = "media\\credits.ogg";

typedef struct
{
	var music;
} CreditsData;

CreditsData _credits;

void credits_stop()
{
	if(creditsConfig.ended == NULL)
		return;
	void fn();
	fn = creditsConfig.ended;
	fn();
}

void credits_core()
{
	_credits.music = media_play(msCreditsMusic, NULL, 100);
	if(_credits.music == NULL)
		error("Failed to play media :(");
}

void credits_start()
{
	level_load("level\\office.wmb");
	ent_create(NULL, vector(0, 0, 0), credits_core);
}