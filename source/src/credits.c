#include "credits.h"
#include "math.h"
#include "musicmgr.h"

BMAP *bmpCreditsText = "credits.png";
BMAP *bmpCreditsLogo = "morbius.png";
STRING *msCreditsMusic = "media\\Silence.OGG";
var credits_mouse_mode = mouse_mode;

void credits__music();

struct
{
	var music;
	var scroll;
	void *on_ent_remove;
	void *on_esc;
	ENTITY *core;
} _credits;

var creditsStoppedHack = 0;
void credits_stop()
{
	creditsStoppedHack = 1;
	if (media_playing(_credits.music))
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
	mouse_mode = credits_mouse_mode;
}

void credits_ent_remove(ENTITY *ent)
{
	if(ent != _credits.core)
		return;
	credits_stop();
}

void credits_core()
{
	vec_set(sky_color, vector(1, 1, 1));
	credits__music();
	
	proc_mode = PROC_LATE;
	
	wait(1);
	
	_credits.scroll = -1.1 * screen_size.y;
	while(!creditsStoppedHack)
	{
		if(_credits.scroll < -0.75 * screen_size.y) {
			
			var alpha = clamp(4 * (abs(screen_size.y) - abs(_credits.scroll)), 0, 100);
			
			draw_quad(
				bmpCreditsLogo,
				vector(
					(screen_size.x - bmpCreditsLogo.width) / 2,
					(screen_size.y - bmpCreditsLogo.height) / 2,
					0),
				NULL,
				NULL,
				NULL,
				COLOR_WHITE,
				100 - alpha,
				0);
		}

		if(_credits.scroll >= -screen_size.y) {
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
		}
			
		_credits.scroll = minv(_credits.scroll + 2 * time_step, bmpCreditsText.height);
		
		if(_credits.scroll >= bmpCreditsText.height)
		{
			credits_stop();
		}
		
		wait(1);
	}
}

void credits_start()
{
	creditsStoppedHack = 0;
	MUSICMGR_stop();
	credits_mouse_mode = mouse_mode;
	mouse_mode = 0;
	memset(&_credits, 0, sizeof(_credits));
	_credits.on_ent_remove = on_ent_remove;
	_credits.on_esc = on_esc;
	on_ent_remove = credits_ent_remove;
	on_esc = credits_stop;
	level_load(NULL);
	ent_create(NULL, vector(0, 0, 0), credits_core);
}

void credits__music()
{
	wait (-3);
	_credits.music = media_loop(msCreditsMusic, NULL, 100);
	//if(_credits.music == NULL)
		//error("Failed to play media :(");
}