#include "credits.h"
#include "math.h"

BMAP *bmpCreditsText = "credits.png";
STRING *msCreditsMusic = "media\\credits.ogg";

struct
{
	var music;
	var scroll;
	void *on_ent_remove;
	void *on_esc;
	ENTITY *core;
} _credits;

void credits_stop()
{
	media_stop(_credits.music);
	_credits.music = 0;
	
	on_ent_remove = _credits.on_ent_remove;
	on_esc = _credits.on_esc;
	
	level_load(NULL);
	
	if(creditsConfig.ended == NULL)
		return;
		
	void fn();
	fn = creditsConfig.ended;
	fn();
}

void credits_ent_remove(ENTITY *ent)
{
	if(ent != _credits.core)
		return;
	credits_stop();
}

void credits_core()
{
	_credits.music = media_play(msCreditsMusic, NULL, 100);
	if(_credits.music == NULL)
		error("Failed to play media :(");
	
	proc_mode = PROC_LATE;
	
	//BMAP *screen = bmap_createblack(bmpCreditsText.width, bmpCreditsText.width, 888);
	
	while(1)
	{
		var scale = screen_size.x / bmpCreditsText.width;
		draw_quad(
			bmpCreditsText,
			vector(0, 0, 0),
			vector(0, _credits.scroll, 0),
			vector(bmpCreditsText.width, bmpCreditsText.width, 0),
			vector(scale, scale, 0),
			COLOR_WHITE,
			100,
			0);
		
		var speed = 1 + key_space;
		
		media_tune(_credits.music, 100, 100 * speed, 0);
		
		_credits.scroll = clamp(_credits.scroll + 10 * speed * time_step, 0, bmpCreditsText.height - screen_size.y);
		
		if((media_playing(_credits.music) == 0) && (_credits.scroll == (bmpCreditsText.height - screen_size.y - 1)))
		{
			credits_stop();
		}
		
		wait(1);
	}
}

void credits_start()
{
	memset(&_credits, 0, sizeof(_credits));
	_credits.on_ent_remove = on_ent_remove;
	_credits.on_esc = on_esc;
	on_ent_remove = credits_ent_remove;
	on_esc = credits_stop;
	level_load(NULL);
	ent_create(NULL, vector(0, 0, 0), credits_core);
}
