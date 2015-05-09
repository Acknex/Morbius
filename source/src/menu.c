#include "menu.h"

#define MENU_BASE_STOP 1
#define MENU_NUM_STOPS 6
#define MENU_IDLE_TICKS 128

var options_show = 0;
STRING *msDiscoMusic = "media\\Sumik_dj_-_wigi.ogg";
BMAP *bmpMenuLogo = "graphics\\textures\\logo.png";
FONT *fontCalibri48 = "Calibri#48b";
SOUND *sndMenuClick = "sounds\\menu-click.wav";

typedef struct {
	VECTOR position;
	VECTOR positionText;
	ANGLE rotation;
	VECTOR positionFade;
	ANGLE rotationFade;
	char title[128];
	void *trigger;
	BMAP *textMap;
} MenuStop;

typedef struct {
	var music;
	void *on_ent_remove;
	void *on_space;
	void *on_cul;
	void *on_cur;
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
	on_space = _menu.on_space;
	on_cul = _menu.on_cul;
	on_cur = _menu.on_cur;
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
		#ifdef MENU_DEBUG
			draw_text(_menu_stops[_menu.currentStop].title, 16, 16, COLOR_RED);
		#endif
		
		if(options_show)
		{
			draw_quad(NULL,vector(0, 0, 0),NULL,screen_size,NULL,COLOR_BLACK,75,0);
			draw_text("Welcome to the worst option menu ever. Enjoy your stay!",20,20,COLOR_WHITE);
			draw_text("Toggle Fullscreen:",20,40,COLOR_WHITE);
			draw_text("Toggle Resolution:",20,60,COLOR_WHITE);
			draw_text("Master Volume:",20,80,COLOR_WHITE);
			var mouse_line = floor((mouse_pos.y-40)/20.0);
			if(mouse_line >= 0 && mouse_line <= 2)
			{
				draw_quad(NULL,vector(0, 40+mouse_line*20, 0),NULL,vector(screen_size.x,20,0),NULL,COLOR_WHITE,25,0);
				if(mouse_left)
				{
					if(mouse_left_off)
					{
						mouse_left_off = 0;
						beep();
					}
				}
				else mouse_left_off = 1;
			}
		}
		else
		{
			if(_menu.fade <= 0.0)
			{
				// Only do camera movement if we are not fading out right now
				if(_menu.currentStop != _menu.nextStop)
				{
					#ifdef MENU_DEBUG
						draw_text(str_for_float(NULL, _menu.lerp), 16, 32, COLOR_RED);
						draw_text(str_for_int(NULL, _menu.currentStop), 128, 32, COLOR_RED);
						draw_text(str_for_int(NULL, _menu.nextStop), 160, 32, COLOR_RED);
					#endif
					
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
			_menuPen.flags |= OUTLINE;
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
		
		#ifdef MENU_DEBUG
			draw_text(str_for_num(NULL, logoAlpha), 16, 48, COLOR_RED);
			draw_text(str_for_num(NULL, idleTime), 72, 48, COLOR_RED);
			draw_text(str_for_num(NULL, textBlinkTime), 128, 48, COLOR_RED);
			
			if(mouse_left)
			{
				VECTOR from, to;
				vec_set(from, mouse_pos);
				vec_set(to, mouse_pos);
				from.z = 0;
				to.z = 1000;
				vec_for_screen(from, camera);
				vec_for_screen(to, camera);
				
				if(c_trace(from, to, IGNORE_PASSABLE | USE_POLYGON))
				{
					diag(str_printf(
					NULL, 
					"\n%d: %d %d %d", 
					(int)_menu.currentStop, 
					(int)target.x, 
					(int)target.y, 
					(int)target.z));
				}
			}
		#endif
		
		wait(1);
	}
}

void _menu_look_at(ENTITY *ent, VECTOR *position)
{
	vec_diff(ent.pan, position, ent.x);
	vec_to_angle(ent.pan, ent.pan);
	ent.roll = 0;
}

void _menu_item_init()
{
	my.flags = DECAL | LIGHT | TRANSLUCENT;
	vec_set(my.blue, COLOR_WHITE);
	
	var down = 0;
	var active = 0;
	float blend = 0;
	while(1)
	{
		VECTOR from, to;
		vec_set(from, mouse_pos);
		vec_set(to, mouse_pos);
		from.z = 0;
		to.z = 1000;
		vec_for_screen(from, camera);
		vec_for_screen(to, camera);
		
		var p = active;
		active = 0;
		vec_set(my.blue, COLOR_WHITE);
		if(c_trace(from, to, IGNORE_MODELS | IGNORE_PASSABLE | USE_POLYGON))
		{
			if((you == me) && (_menu.currentStop == my.skill1) && (_menu.currentStop == _menu.nextStop))
			{
				vec_set(my.blue, COLOR_RED);
				active = 1;
				if((mouse_left != down) && mouse_left && (my.event != NULL))
				{
					void fn();
					fn = my.event;
					fn();
				}		
			}
		}
		if((p != active) && active)
		{
			snd_play(sndMenuClick, 100, 0);
		}
		down = mouse_left;
		
		float blendSpeed = 0.1;
		if(((_menu.currentStop == my.skill1) && (_menu.currentStop == _menu.nextStop)) || (_menu.nextStop == my.skill1))
		{
			blend = clamp(blend + blendSpeed * time_step, 0, 1);
		}
		else
		{
			blend = clamp(blend - blendSpeed * time_step, 0, 1);
		}
		
		my.alpha = 100 * smootherstep(0, 1, blend);
		
		wait(1);
	}
}

void menu_entity_trigger()
{
	if(my.skill1 == 3) options_show = 1;
	else menu_fade_and_trigger(_menu_stops[my.skill1]);
}

void menu_nav_next()
{
	menu_switch(1);
}

void menu_nav_prev()
{
	menu_switch(-1);
}

void menu_trigger()
{
	if(_menu.currentStop != _menu.nextStop)
	return;
	menu_fade_and_trigger(_menu_stops[_menu.currentStop]);
}

void menu_regenerate_bitmaps()
{
	int i;
	for(i = MENU_BASE_STOP; i < MENU_NUM_STOPS; i++)
	{
		var width = 1.2 * str_width(_menu_stops[i].title, fontCalibri48);
		var height = 1.2 * fontCalibri48.dy;
		
		_menu_stops[i].textMap = bmap_createblack(width, height, 8888);
		
		bmap_rendertarget(_menu_stops[i].textMap, 0, 0);
		
		_menuPen.alpha = 100;
		_menuPen.pos_x = 0.5 * width;
		_menuPen.pos_y = 0.5 * height;
		_menuPen.flags &= ~OUTLINE;
		str_cpy(_menuText, _menu_stops[i].title);
		draw_obj(_menuPen);
		
		bmap_rendertarget(NULL, 0, 0);
	}
	
	bmap_rendertarget(NULL, 0, 0);
}

void menu_open()
{
	wait(1); // Using graphic functions
	
	// Initialize menuData
	memset(_menu, 0, sizeof(MenuData));
	_menu.on_ent_remove = on_ent_remove;
	_menu.on_space = on_space;
	_menu.on_cur = on_cur;
	_menu.on_cul = on_cul;
	on_ent_remove = menu_ent_remove;
	on_space = menu_trigger;
	on_cur = menu_nav_prev;
	on_cul = menu_nav_next;
	
	
	level_load("level\\kingmorph.wmb");
	
	_menu.isIdle = 1;
	_menu.core = ent_create(NULL, vector(0,0,0), menu_core);
	
	// Create main menu text bitmaps
	menu_regenerate_bitmaps();
	
	// Create main menu text entries
	int i;
	for(i = MENU_BASE_STOP; i < MENU_NUM_STOPS; i++)
	{
		ENTITY *menuItem = ent_create("graphics\\textures\\stub.png", _menu_stops[i].positionText, _menu_item_init);
		ent_clone(menuItem);
		
		vec_fill(menuItem.scale_x, 0.03);
		menuItem.scale_x *= bmap_width(_menu_stops[i].textMap);
		menuItem.scale_y *= bmap_height(_menu_stops[i].textMap);
		menuItem.scale_z = 0.5;
		
		_menu_look_at(menuItem, _menu_stops[i].position);
		
		ent_setskin(menuItem, _menu_stops[i].textMap, 1);
		
		menuItem.flags2 |= UNTOUCHABLE;
		menuItem.skill1 = i;
		menuItem.event = menu_entity_trigger;
		
		VECTOR dir;
		vec_diff(dir, _menu_stops[i].position, _menu_stops[i].positionText);
		
		var size = menuItem.scale_x * 32;
		
		VECTOR left, right;
		vec_cross(left, dir, vector(0, 0, 1));
		vec_cross(right, dir, vector(0, 0, -1));
		vec_normalize(left, size + 24);
		vec_normalize(right, size + 24);
		vec_add(left, _menu_stops[i].positionText);
		vec_add(right, _menu_stops[i].positionText);
		
		ENTITY *navLeft = ent_create("graphics\\textures\\navigate-left.png", left, _menu_item_init);
		vec_fill(navLeft.scale_x, 0.3);
		vec_set(navLeft.pan, menuItem.pan);
		navLeft.event = menu_nav_next;
		navLeft.skill1 = i;
		
		ENTITY *navRight = ent_create("graphics\\textures\\navigate-right.png", right, _menu_item_init);
		vec_fill(navRight.scale_x, 0.3);
		vec_set(navRight.pan, menuItem.pan);
		navRight.event = menu_nav_prev;
		navRight.skill1 = i;
	}
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

void menu_trigger_options()
{
	error("huhu");
	/*	if(menuConfig.Options != NULL)
	{
		void fn();
		fn = menuConfig.Options;
		fn();
	}*/
}


void menu_trigger_credits()
{
	if(menuConfig.startCredits != NULL)
	{
		void fn();
		fn = menuConfig.startCredits;
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
	vec_set(_menu_stops[0].position, vector(1056, -280, 334));
	vec_set(_menu_stops[0].rotation, vector(203, -12, 0));
	strcpy(_menu_stops[0].title, "<IDLE MENU>");
	
	vec_set(_menu_stops[1].position, vector(-92, -1348, 443));
	vec_set(_menu_stops[1].rotation, vector(52, -36, 0));
	vec_set(_menu_stops[1].positionText, vector(276, -821, -20));
	strcpy(_menu_stops[1].title, "Start Game");
	_menu_stops[1].trigger = menu_trigger_start;
	
	vec_set(_menu_stops[2].position, vector(-84, -1098, 322));
	vec_set(_menu_stops[2].rotation, vector(154, -15, 0));
	vec_set(_menu_stops[2].positionText, vector(-540, -935, 130));
	strcpy(_menu_stops[2].title, "Load Game");
	
	vec_set(_menu_stops[3].position, vector(-407, -1090, 217));
	vec_set(_menu_stops[3].rotation, vector(242, -26, 0));
	vec_set(_menu_stops[3].positionText, vector(-485, -1310, 100));
	strcpy(_menu_stops[3].title, "Options");
	_menu_stops[3].trigger = menu_trigger_options;
	
	vec_set(_menu_stops[4].position, vector(611, -625, 226));
	vec_set(_menu_stops[4].rotation, vector(340, -13, 0));
	vec_set(_menu_stops[4].positionText, vector(959, -650, 100));
	strcpy(_menu_stops[4].title, "Credits");
	_menu_stops[4].trigger = menu_trigger_credits;
	
	vec_set(_menu_stops[5].position, vector(215, -562, 224));
	vec_set(_menu_stops[5].rotation, vector(155, -18, 0));
	vec_set(_menu_stops[5].positionText, vector(-136, -425, 110));
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