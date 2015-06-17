#include "credits.h"
#include "math.h"

BMAP *bmpCreditsText = "credits.png";
BMAP *bmpCreditsLogo = "morbius.png";
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
PANEL* creditsPanel = pan_create(" ", 12);
creditsPanel.bmap = bmap_createblack(1920,1200,8888);
set(creditsPanel, SHOW);	
set(creditsPanel, FILTER);	
	wait(1);
	
	_credits.scroll = -1.1 * 1200;
	while(1)
	{
creditsPanel.scale_y = screen_size.y / 1200;
creditsPanel.scale_x = creditsPanel.scale_y;
creditsPanel.pos_x = (screen_size.x - creditsPanel.bmap.width * creditsPanel.scale_x) * 0.5;
creditsPanel.pos_y = 0;
bmap_rendertarget(creditsPanel.bmap,0,1);
		if(_credits.scroll < -0.75 * 1200) {
			
			var alpha = clamp(4 * (abs(1200) - abs(_credits.scroll)), 0, 100);
			
			draw_quad(
				bmpCreditsLogo,
				vector(
					(1920 - bmpCreditsLogo.width) / 2,
					(1200 - bmpCreditsLogo.height) / 2,
					0),
				NULL,
				NULL,
				NULL,
				COLOR_WHITE,
				100 - alpha,
				0);
		}
		if(_credits.scroll >= -1200) {
			float scale = 1.2;
			//float aspect = screen_size.y / screen_size.x;
			int x = (1920 - scale * bmpCreditsText.width) / 2;
			draw_quad(
				bmpCreditsText,
				vector(x, -minv(_credits.scroll, 0), 0),
				vector(0, maxv(_credits.scroll, 0), 0),
				vector(bmpCreditsText.width, 1200 / scale, 0),
				vector(scale, scale, 1),
				COLOR_WHITE,
				100,
				0);
		}
bmap_rendertarget(NULL,0,0);
			
		_credits.scroll = minv(_credits.scroll + 2 * time_step, bmpCreditsText.height);
		
		if(_credits.scroll == bmpCreditsText.height)
		{
reset(creditsPanel, SHOW);	
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
