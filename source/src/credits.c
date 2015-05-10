#include "credits.h"
#include "math.h"

#define CREDITS_ITEMS_COUNT 8

STRING *msCreditsMusic = "media\\credits.ogg";
FONT *fontHelvetica64 = "Helvetica#64b";
STRING *strPrint = "#1024";
TEXT *txtPrint = { font = fontHelvetica64; string = strPrint; flags = OUTLINE | TRANSLUCENT; }

typedef struct
{
	var music;
	void *on_ent_remove;
	void *on_esc;
	ENTITY *core;
} CreditsData;

typedef struct
{
	VECTOR position;
	ANGLE rotation;
	float delay;
	char name[128];
	char activity[1024];
} CreditsStop;

CreditsData _credits;
CreditsStop _creditStop[10];

void credits_stop()
{
	if(creditsConfig.ended == NULL)
		return;
	
	media_stop(_credits.music);
	_credits.music = 0;
	
	on_ent_remove = _credits.on_ent_remove;
	on_esc = _credits.on_esc;
	
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
	
	int cur = 0;
	int state = 0;
	float time = 0;
	float offset = 0;
	while(1)
	{
		float speed = 0;
		switch(state)
		{
			case 0: // Moving
			{
				float f = smootherstep(0, 64, time);
				vec_lerp(camera.x, _creditStop[cur].position, _creditStop[cur+1].position, f);
				ang_lerp(camera.pan, _creditStop[cur].rotation, _creditStop[cur+1].rotation, f);
				if(time >= 64) {
					time = 0;
					state = 1;
					offset = 0;
				}
				break;
			}
			case 1: // Delay
			{
				float q = _creditStop[cur].delay / 4;
				
				if(time <= q)
					speed = time / q;
				else if (time <= 3 * q)
					speed = 1;
				else
					speed = 1 - (time-3*q)/q;
				DEBUG_VAR(q, 16);
				DEBUG_VAR(speed, 32);
				DEBUG_VAR(time, 48);
				
				if(time >= _creditStop[cur].delay) {
					time = 0;
					state = 0;
					cur += 1;
					if(cur >= CREDITS_ITEMS_COUNT)
					{
						credits_stop();
						return;
					}
				}
				break;
			}
		}
		
		offset += 3 * speed * time_step;
		
		float alpha = 100 * speed;
		
		str_cpy(strPrint, _creditStop[cur].name);
		txtPrint.pos_x = 16 + offset;
		txtPrint.pos_y = 16;
		txtPrint.alpha = alpha;
		draw_obj(txtPrint);
		
		str_cpy(strPrint, _creditStop[cur].activity);
		txtPrint.pos_x = screen_size.x - 16 - str_width(strPrint, fontHelvetica64) - offset;
		txtPrint.pos_y = screen_size.y - 16 - fontHelvetica64.dy;
		txtPrint.alpha = alpha;
		draw_obj(txtPrint);
		
		time += time_step;
		wait(1);
	}
}

void dumpPos()
{
	diag(str_printf(NULL, "\n%d %d %d; %d %d", camera.x, camera.y, camera.z, camera.pan, camera.tilt));
}

void credits_start()
{
	on_mouse_left = dumpPos;
	_credits.on_ent_remove = on_ent_remove;
	_credits.on_esc = on_esc;
	on_ent_remove = credits_ent_remove;
	on_esc = credits_stop;
	level_load("level\\office_credits.wmb");
	ent_create(NULL, vector(0, 0, 0), credits_core);
}


/*
	240933 98110 135096; 26987 -21053
	92561 -10865 191673; -119401 -17368
	-276979 25286 140674; 184320 0
	-29212 -131248 200413; 107590 -32661
	94001 305347 213461; 57951 -20866
	-127900 399147 167539; 127869 -26848
	91344 182217 183438; 259032 -11080
*/
void credits_startup()
{
	int i;
	for(i = 0; i < 8; i++)
	{
		_creditStop[i].delay = 96;
	}

	vec_set(_creditStop[0].position, vector(89.203125, 177.9462890625, 179.138671875));
	vec_set(_creditStop[0].rotation, vector(252.9609375, -10.8203125, 0));
	strcpy(_creditStop[0].name, "Christian Behrenberg");
	strcpy(_creditStop[0].activity, "Minispiele");
	
	vec_set(_creditStop[1].position, vector(235.2861328125, 95.810546875, 131.9296875));
	vec_set(_creditStop[1].rotation, vector(26.3544921875, -20.5595703125, 0));
	strcpy(_creditStop[1].name, "Kanal 21");
	strcpy(_creditStop[1].activity, "Location / Internet");

	vec_set(_creditStop[2].position, vector(90.3916015625, -10.6103515625, 187.1806640625));
	vec_set(_creditStop[2].rotation, vector(-116.6025390625, -16.9609375, 0));
	strcpy(_creditStop[2].name, "Robert Jäger");
	strcpy(_creditStop[2].activity, "Items, items, items!");

	vec_set(_creditStop[3].position, vector(-270.4873046875, 24.693359375, 137.376953125));
	vec_set(_creditStop[3].rotation, vector(180, 0, 0));
	strcpy(_creditStop[3].name, "Laptop");

	vec_set(_creditStop[4].position, vector(-28.52734375, -128.171875, 195.7158203125));
	vec_set(_creditStop[4].rotation, vector(105.068359375, -31.8955078125, 0));
	strcpy(_creditStop[4].name, "Chillkröte");

	vec_set(_creditStop[5].position, vector(91.7978515625, 298.1904296875, 208.4580078125));
	vec_set(_creditStop[5].rotation, vector(56.5927734375, -20.376953125, 0));
	strcpy(_creditStop[5].name, "Herd");

	vec_set(_creditStop[6].position, vector(-124.90234375, 389.7919921875, 163.6123046875));
	vec_set(_creditStop[6].rotation, vector(124.8720703125, -26.21875, 0));
	strcpy(_creditStop[6].name, "Christian Behrenberg");

	vec_set(_creditStop[7].position, vector(89.203125, 177.9462890625, 179.138671875));
	vec_set(_creditStop[7].rotation, vector(252.9609375, -10.8203125, 0));
}