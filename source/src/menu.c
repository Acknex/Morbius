#include "menu.h"

#define MENU_BASE_STOP 1
#define MENU_NUM_STOPS 6
#define MENU_IDLE_TICKS 16

STRING *msDiscoMusic = "media\\Sumik_dj_-_wigi.ogg";

typedef struct {
	VECTOR position;
	ANGLE rotation;
	char title[128];
} MenuStop;

typedef struct {
    var music;
	void *on_ent_remove;
	ENTITY *core;
	int currentStop;
	int nextStop;
	float lerp;
	int isIdle;
} MenuData;

MenuData _menu;
MenuStop _menu_stops[MENU_NUM_STOPS];

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
	while(1)
	{
		draw_text(_menu_stops[_menu.currentStop].title, 16, 16, COLOR_RED);
		
		// Go from idle to first entry
		if(_menu.isIdle && key_any)
		{
			_menu.nextStop = MENU_BASE_STOP;
			_menu.isIdle = 0;
		}
		
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
			vec_set(camera.x, _menu_stops[_menu.currentStop].position);
			vec_set(camera.pan, _menu_stops[_menu.currentStop].rotation);
		}
		
		// Check for mouse movement
		if(vec_length(mickey) > 0)
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

void menu_startup()
{
	vec_set(_menu_stops[0].position, vector(-621, 579, 209));
	vec_set(_menu_stops[0].rotation, vector(317, -18, 0));
	strcpy(_menu_stops[0].title, "<IDLE MENU>");
	
	vec_set(_menu_stops[1].position, vector(-366, 357, 55));
	vec_set(_menu_stops[1].rotation, vector(317, -14, 0));
	strcpy(_menu_stops[1].title, "Start Game");
	
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
}