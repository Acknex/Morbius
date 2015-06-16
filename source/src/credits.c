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
	vec_set(sky_color, vector(0, 0, 0));
	_credits.music = media_play(msCreditsMusic, NULL, 100);
	if(_credits.music == NULL)
		error("Failed to play media :(");
	
	proc_mode = PROC_LATE;
	
	wait(1);
	
	_credits.scroll = -screen_size.y;
	while(1)
	{
		float scale = 1.2;
		float aspect = screen_size.y / screen_size.x;
		int x = (screen_size.x - scale * bmpCreditsText.width) / 2;
		draw_quad(
			bmpCreditsText,
			vector(x, -minv(_credits.scroll, 0), 0),
			vector(0, maxv(_credits.scroll, 0), 0),
			vector(bmpCreditsText.width, screen_size.y / scale, 0),
			vector(scale, scale, 1),
			COLOR_WHITE,
			100,
			0);
		
		_credits.scroll = minv(_credits.scroll + 2 * time_step, bmpCreditsText.height);
		
		if(_credits.scroll == bmpCreditsText.height)
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
