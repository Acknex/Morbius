#include "itemmgr.h"
#include "player.h"
#include "level_transition.h"
#include "event.h"
#include "items.h"

#define BOIL_PARTICLE_REC 1
#define BOIL_PARTICLE_AMOUNT 3
#define BOIL_PARTICLE_SHOW FLAG1
#define OFFICE_GATE_ACTIVE FLAG1

ENTITY* OFFICE__officeChairEnt = NULL;
ENTITY* OFFICE__officeStartEnt = NULL;
ENTITY* OFFICE__officeBoilSmokeEnt = NULL;
ENTITY* OFFICE__LevelGateEnt = NULL;
BMAP* OFFICE__smokeBmap = "rauch2.tga";

void OFFICE__partSmokeFade(PARTICLE* p);
void OFFICE__partSmoke(PARTICLE* p);
void OFFICE__Level_gate_check();
void OFFICE__Level_gate_event(); 

//skill1: EntityType 1
//skill2: ItemId -1
//skill3: ItemBorder 25
action griechin()
{
	VECTOR* temp;

	my->tilt = 10;
	//upper legs
	temp = vector(0,80,0);
	ent_bonerotate(me,"Joint_3_3",temp);
	ent_bonerotate(me,"Joint_3_4",temp);
	//upper arms
	temp = vector(0,80,0);
	ent_bonerotate(me,"Joint_3",temp);
	ent_bonerotate(me,"Joint_3_2",temp);
	//left hand
	temp = vector(110,-25,30);
	ent_bonerotate(me,"Joint_4",temp);
	//right hand
	temp = vector(-110,-25,-30);
	ent_bonerotate(me,"Joint_4_2",temp);

	interactionItem_remoteStart();
}

action officeChair()
{
	OFFICE__officeChairEnt = me;
}

action officeStart()
{
	set(my, INVISIBLE | PASSABLE);
	OFFICE__officeStartEnt = me;
}

void OFFICE_morbius_sit()
{
	if (player != NULL)
	{
		if (OFFICE__officeChairEnt != NULL)
		{
			vec_set(&player->x, &OFFICE__officeChairEnt->x);
			vec_set(&player->pan, &OFFICE__officeChairEnt->pan);
			//dirty hackaround
			player->pan += 90;
			player->tilt = 10;
			player->z += 30;
		}
		VECTOR* temp = vector(0,80,0);
		ent_bonerotate(player,"Joint_3_3",temp);
		ent_bonerotate(player,"Joint_3_4",temp);
	}
}

void OFFICE_morbius_stand()
{
	if (player != NULL)
	{
		if (OFFICE__officeStartEnt != NULL)
		{
			vec_set(&player->x, &OFFICE__officeStartEnt->x);
			vec_set(&player->pan, &OFFICE__officeStartEnt->pan);
		}
		VECTOR* temp = vector(0,-80,0);
		ent_bonerotate(player,"Joint_3_3",temp);
		ent_bonerotate(player,"Joint_3_4",temp);
		//dirty hackaround: fool smartwalk
		vec_set(player->target_x,player->x);
	}
}

void OFFICE_morbius_goToChair()
{
return;
//this does not work as player gains control again
/*	if (OFFICE__officeChairEnt != NULL)
	{
		player_may_walk = 1;
		vec_set(player->target_x, OFFICE__officeChairEnt->x);
	}	*/
}

var OFFICE_morbius_isNearChair()
{
return 1;
/*	if (vec_dist(player->target_x, player->x) < 5)
	{
		return 1;
	}
	else
	{
		return 0;
	}*/
}

void OFFICE_startSmoke()
{
	if (OFFICE__officeBoilSmokeEnt != NULL)
	{
		set(OFFICE__officeBoilSmokeEnt, BOIL_PARTICLE_SHOW);
	}
}

void OFFICE_stopSmoke()
{
	if (OFFICE__officeBoilSmokeEnt != NULL)
	{
		reset(OFFICE__officeBoilSmokeEnt, BOIL_PARTICLE_SHOW);
	}
}

void OFFICE_enableLevelGate()
{
	if (OFFICE__LevelGateEnt != NULL)
	{
		set(OFFICE__LevelGateEnt, OFFICE_GATE_ACTIVE);
	}
}

void OFFICE__partSmokeFade(PARTICLE* p)
{
	p->alpha -= 3 * time_step;
	if (p->alpha <= 0) 
	{
		p->lifespan = 0;
	}
	
	p->size += time_step;
}


void OFFICE__partSmoke(PARTICLE* p)
{
	p->bmap = OFFICE__smokeBmap;
	p->lifespan = 100;
	p->size = 7 + random(5);
	p->alpha = 60 + random(10);
	vec_scale(p->vel_x, 0.2);
	set(p, TRANSLUCENT | MOVE | BRIGHT);
	p->event = OFFICE__partSmokeFade;
}

void OFFICE__Level_gate_check()
{
	reset(my, gateWasClicked);
	
	if (is(my, OFFICE_GATE_ACTIVE))
	{
		//reset(my, OFFICE_GATE_ACTIVE);
		EVENT_trigger((int)my->skill1);
	}
	else 
	{
		ITEM* item = ITEM_get(my->badItemId);
		
		if (item != NULL)
		{
			ITEM_interaction(item);
		}
	}
}

void OFFICE__Level_gate_event() 
{
	if (event_type == EVENT_CLICK) 
	{
		set(my, gateWasClicked);

		if (my.DOUBLE_CLICK_TIME >= 100) 
		{
			// Double clicked
			if (player != NULL)
			{
				player->force_stop = 1;
			}
			OFFICE__Level_gate_check(); // Instant level change
		} 
		else 
		{
			my.DOUBLE_CLICK_TIME = 110;
		}
	}
	else
	{
		level_gate_event();
	}
}

action officeBoilSmoke()
{
	OFFICE__officeBoilSmokeEnt = me;
	set(me, PASSABLE | INVISIBLE);
	var cnt = 0;
	while (1)
	{
		if (is(me, BOIL_PARTICLE_SHOW))
		{
			cnt += time_step;
			while(cnt >= BOIL_PARTICLE_REC)
			{
				effect(OFFICE__partSmoke, BOIL_PARTICLE_AMOUNT,
				_vec(my->x + random(10) - 5, my->y + random(10) - 5, my->z + random(10) -5),
				_vec(-2 - random(4), -2 - random(4), 10 + random(20)));
				cnt -= BOIL_PARTICLE_REC;
			}
		}
		wait(1);
	}
}

//skill1: event_trigger_id -1
//skill4: badItemId -1
action officeLevel_gate()
{
	OFFICE__LevelGateEnt = me;
	level_gate_init();	
	my->event = OFFICE__Level_gate_event;
		
	while(1)
	{
		if(player)
		{
			if(
				player.x > my.skill10 && player.y > my.skill11 && player.x < my.skill12 && player.y < my.skill13
				&& is(my, gateWasClicked)
			)
			{
				OFFICE__Level_gate_check();
			}
			else
			{
				if (my.DOUBLE_CLICK_TIME > 0) 
				{
					my.DOUBLE_CLICK_TIME -=1 * time_step;
				}
			}
		}
		
		wait(1);
	}
}