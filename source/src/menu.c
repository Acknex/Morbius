#include "menu.h"

#define MENU_BASE_STOP 1
#define MENU_NUM_STOPS 6
#define MENU_IDLE_TICKS 128

STRING *msDiscoMusic = "media\\Sumik_dj_-_wigi.ogg";
BMAP *bmpMenuLogo = "graphics\\textures\\logo.png";
FONT *fontCalibri48 = "Calibri#48b";

typedef struct {
	VECTOR position;
	ANGLE rotation;
	VECTOR positionFade;
	ANGLE rotationFade;
	char title[128];
	void *trigger;
} MenuStop;

typedef struct {
    var music;
	void *on_ent_remove;
	ENTITY *core;
	int currentStop;
	int nextStop;
	float lerp;
	int isIdle;
	float fade;
} MenuData;

MenuData _menu;
MenuStop _menu_stops[MENU_NUM_STOPS];

STRING *_menuText = "#256";
TEXT *_menuPen = { string = _menuText; flags = CENTER_X | CENTER_Y | TRANSLUCENT; font = fontCalibri48; }

var ang_lerp_single(var a1, var a2, var f)
{
	var angle = ang(a1 - a2);
	return ang(a1 - (f * angle));
}

ANGLE* ang_lerp(ANGLE* a, ANGLE* a1, ANGLE* a2, var f)
{
	ANGLE ai;
	ai.pan = ang_lerp_single(a1->pan, a2->pan, f);
	ai.tilt = ang_lerp_single(a1->tilt, a2->tilt, f);
	ai.roll = ang_lerp_single(a1->roll, a2->roll, f);
	
	if(a != NULL)
		vec_set(a, &ai);
	
	return vector(ai.pan, ai.tilt, ai.roll);
}

float smootherstep(float edge0, float edge1, float x)
{
    // Scale, and clamp x to 0..1 range
    x = clamp((x - edge0)/(edge1 - edge0), 0.0, 1.0);
    // Evaluate polynomial
    return x*x*x*(x*(x*6 - 15) + 10);
}

void menu_fade_and_trigger(MenuStop *stop)
{
	for(_menu.fade = 0; _menu.fade <= 1.0; _menu.fade += 0.05 * time_step)
	{
		// Fade camera movement
		vec_lerp(camera.x, stop.position, stop.positionFade, _menu.fade);
		ang_lerp(camera.pan, stop.rotation, stop.rotationFade, _menu.fade );
		media_tune(_menu.music, maxv(1, 100 - 120 * _menu.fade), 0, 0);
		wait(1);
	}
	media_pause(_menu.music);
	if(stop.trigger != NULL)
	{
		void fn();
		fn = stop.trigger;
		fn();
	}
}

void menu_switch(int dir)
{
	if((dir != 1) && (dir != -1))
		return;
	if(_menu.currentStop != _menu.nextStop)
		return;
	_menu.nextStop = cycle(_menu.currentStop + dir, MENU_BASE_STOP, MENU_NUM_STOPS);
}

void menu_close()
{
	media_stop(_menu.music);
	_menu.music = 0;
	
	_menu.on_ent_remove = on_ent_remove;
}

void menu_ent_remove(ENTITY *ent)
{
	if(ent != _menu.core)
		return;
	menu_close();
}

void menu_core()
{
	_menu.music = media_loop(msDiscoMusic, NULL, 100);
	
	var idleTime = 0;
	var logoAlpha = 100;
	var textAlpha = 100;
	var textBlinkTime = 0;
	while(1)
	{
		draw_text(_menu_stops[_menu.currentStop].title, 16, 16, COLOR_RED);
		
		if(_menu.fade <= 0.0)
		{
			// Only do camera movement if we are not fading out right now
			if(_menu.currentStop != _menu.nextStop)
			{
				draw_text(str_for_float(NULL, _menu.lerp), 16, 32, COLOR_RED);
				draw_text(str_for_int(NULL, _menu.currentStop), 128, 32, COLOR_RED);
				draw_text(str_for_int(NULL, _menu.nextStop), 160, 32, COLOR_RED);
				
				float f = smootherstep(0.0, 1.0, _menu.lerp);
				
				vec_lerp(camera.x, _menu_stops[_menu.currentStop].position, _menu_stops[_menu.nextStop].position, f);
				ang_lerp(camera.pan, _menu_stops[_menu.currentStop].rotation, _menu_stops[_menu.nextStop].rotation, f);
				
				float lerpSpeed = 0.07;
				if(_menu.nextStop == 0)
				{
					lerpSpeed = 0.03;
				}
				_menu.lerp += lerpSpeed * time_step;
				if(_menu.lerp > 1.0)
				{
					_menu.currentStop = _menu.nextStop;
					_menu.lerp = 0.0;
				}
				
				// Reset idle on swap
				idleTime = 0;
			}
			else
			{
				// Go from idle to first entry
				if(_menu.isIdle && key_any)
				{
					_menu.nextStop = MENU_BASE_STOP;
					_menu.isIdle = 0;
				}
				
				vec_set(camera.x, _menu_stops[_menu.currentStop].position);
				vec_set(camera.pan, _menu_stops[_menu.currentStop].rotation);
			}
		}
		
		// Check for mouse movement
		if((vec_length(mickey) > 0) || key_any)
		{
			idleTime = 0;
		}
		else
		{
			idleTime += time_step;
			if(idleTime >= MENU_IDLE_TICKS)
			{
				_menu.isIdle = 1;
				_menu.nextStop = 0; // Return to idle stop
				_menu.lerp = 0;
			}
		}
		
		var logoBlendSpeed = 20;
		if(_menu.isIdle)
		{
			// Blend logo in
			logoAlpha = minv(100, logoAlpha + logoBlendSpeed * time_step);
		}
		else
		{
			// Blend logo out
			logoAlpha = maxv(0, logoAlpha - logoBlendSpeed * time_step);
		}
		
		// Draw logo scaled and centered
		if(logoAlpha > 0)
		{
			var width = 0.7 * screen_size.x;
			var height = width * bmap_height(bmpMenuLogo) / bmap_width(bmpMenuLogo);
			var scale = width / bmap_width(bmpMenuLogo);
			draw_quad(
				bmpMenuLogo,
				vector(0.5 * (screen_size.x - width), 0.5 * (screen_size.y - height), 0),
				NULL, // Offset
				NULL, // Size,
				vector(scale, scale, 0),
				COLOR_WHITE,
				logoAlpha,
				0);
			
			// Blink cycle of the key-press text.
			var blinkAngle = cycle(20 * textBlinkTime, 0, 720);
			var blinkAlpha = 100;
			if(blinkAngle > 360)
			{
				blinkAlpha = 50 + 50 * cosv(blinkAngle - 360);
			}
			
			// Only draw key press text if transition is finished
			if(_menu.currentStop == _menu.nextStop)
			{
				textAlpha = minv(100, textAlpha + 15 * time_step);
				textBlinkTime += time_step;
			}
			else
			{
				textAlpha = maxv(0, textAlpha - 15 * time_step);
			}
			// Draw key press text
			_menuPen.alpha = 0.01 * textAlpha * 0.01 * blinkAlpha * logoAlpha;
			_menuPen.pos_x = 0.5 * screen_size.x;
			_menuPen.pos_y = 0.5 * (screen_size.y + height) + 0;
			str_cpy(_menuText, "Press any key");
			draw_obj(_menuPen);
		
		}
		else
		{
			textBlinkTime = 0;
		}
		
		draw_quad(
			NULL,
			vector(0, 0, 0),
			NULL,
			screen_size,
			NULL,
			COLOR_BLACK,
			100 * _menu.fade,
			0);
		
		draw_text(str_for_num(NULL, logoAlpha), 16, 48, COLOR_RED);
		draw_text(str_for_num(NULL, idleTime), 72, 48, COLOR_RED);
		draw_text(str_for_num(NULL, textBlinkTime), 128, 48, COLOR_RED);
		
		wait(1);
	}
}

void menu_open()
{
	_menu.on_ent_remove = on_ent_remove;
	on_ent_remove = menu_ent_remove;
    level_load("level\\disco.wmb");
	_menu.isIdle = 1;
	_menu.core = ent_create(NULL, vector(0,0,0), menu_core);
}

void menu_trigger_start()
{
	if(menuConfig.startGame != NULL)
	{
		void fn();
		fn = menuConfig.startGame;
		fn();
	}
}

void menu_trigger_quit()
{
	if(menuConfig.quitGame != NULL)
	{
		void fn();
		fn = menuConfig.quitGame;
		fn();
	}
}

void menu_startup()
{
	vec_set(_menu_stops[0].position, vector(-621, 579, 209));
	vec_set(_menu_stops[0].rotation, vector(317, -18, 0));
	strcpy(_menu_stops[0].title, "<IDLE MENU>");
	
	vec_set(_menu_stops[1].position, vector(-366, 357, 55));
	vec_set(_menu_stops[1].rotation, vector(317, -14, 0));
	strcpy(_menu_stops[1].title, "Start Game");
	_menu_stops[1].trigger = menu_trigger_start;
	
	vec_set(_menu_stops[2].position, vector(-112, 244, 50));
	vec_set(_menu_stops[2].rotation, vector(271, -14, 0));
	strcpy(_menu_stops[2].title, "Load Game");
	
	vec_set(_menu_stops[3].position, vector(269, 276, 71));
	vec_set(_menu_stops[3].rotation, vector(228, -19, 0));
	strcpy(_menu_stops[3].title, "Options");
	
	vec_set(_menu_stops[4].position, vector(346, -208, 62));
	vec_set(_menu_stops[4].rotation, vector(138, -16, 0));
	strcpy(_menu_stops[4].title, "Credits");
	
	vec_set(_menu_stops[5].position, vector(-291, -289, 62));
	vec_set(_menu_stops[5].rotation, vector(45, -16, 0));
	strcpy(_menu_stops[5].title, "Quit Game");
	_menu_stops[5].trigger = menu_trigger_quit;
	
	int i;
	for(i = MENU_BASE_STOP; i < MENU_NUM_STOPS; i++)
	{
		vec_set(_menu_stops[i].positionFade, vector(0, 0, 16));
		vec_add(_menu_stops[i].positionFade, _menu_stops[i].position);
		
		vec_set(_menu_stops[i].rotationFade, vector(15, -10, 0));
		vec_add(_menu_stops[i].rotationFade, _menu_stops[i].rotation);
	}
}