#include "camera.h"
#include "musicmgr.h"
#include "itemmgr.h"
#include "dancewed.h"

#define DANCE_HAND_PERCENT 80
#define DANCE_FEET_PERCENT 40
#ifndef trigItemId
	#define trigItemId skill4
#endif


STRING* KINGMORPH__strAckmania = "media\\ackmania_tiny.wmv";
STRING* KINGMORPH__strLotteria = "media\\lotteria_tiny.wmv";
ENTITY* KINGMORPH__cbabe = NULL;
ENTITY* KINGMORPH__ackmaniaCam = NULL;
ENTITY* KINGMORPH__lotteriaCam = NULL;
ENTITY* KINGMORPH__ackmania = NULL;
ENTITY* KINGMORPH__lotteria = NULL;
ENTITY* KINGMORPH__griechin = NULL;
var KINGMORPH__lastMouseMode;
var KINGMORPH__ackmaniaHandle = 0;
var KINGMORPH__lotteriaHandle = 0;

void KINGMORPH__lerpVec(VECTOR* out, VECTOR* start, VECTOR* end, var percentage);
void KINGMORPH__lerpAng(ANGLE* out, ANGLE* start, ANGLE* end, var percentage);


void KINGMORPH_morbius_LookAtCbabe()
{
	if (KINGMORPH__cbabe == NULL)
	{
		return;
	}
	
	VECTOR lookat;
	ANGLE lookdir;
	var lookStart;
	var lookEnd;
	var progress = 0;
	
	vec_set(&lookat, &KINGMORPH__cbabe->x);
	vec_sub(&lookat, &player->x);
	vec_to_angle(&lookdir, &lookat);
	lookStart = player->pan;
	lookEnd = lookdir.pan;
	
	while(progress < 100)
	{
		player->pan = lookStart + ang(lookEnd - lookStart) * progress * 0.01;
		progress += 15 * time_step;
		wait(1);
	}
	player->pan = lookEnd;
}

void KINGMORPH_ackmania()
{
	VECTOR camPos;
	VECTOR diffPos;
	ANGLE camAng;
	ANGLE diffAng;
	VECTOR temp;
	var progress = 0;

	if(KINGMORPH__ackmaniaCam == NULL)
	{
		return;
	}
	
	cameraPause();
	MUSICMGR_fadeOut(10);
	KINGMORPH__lastMouseMode = mouse_mode;
	mouse_mode = 0;
	
	vec_set(&camPos, &camera->x);
	vec_set(&camAng, &camera->pan);
	
	while(progress < 100)
	{
		wait(1);
		progress = minv(progress + (7 * time_step), 100);
		KINGMORPH__lerpVec(&camera->x, &camPos, &KINGMORPH__ackmaniaCam->x, progress * 0.01);
		KINGMORPH__lerpAng(&camera->pan, &camAng, &KINGMORPH__ackmaniaCam->pan, progress * 0.01);
		if (media_playing(KINGMORPH__ackmaniaHandle))
			media_tune(KINGMORPH__ackmaniaHandle, progress, 0, 0);		
	}
	set(player, INVISIBLE);

	while(key_esc == 0 && key_space == 0 && key_enter == 0 && mouse_left == 0) wait(1);
	
	reset(player, INVISIBLE);
	MUSICMGR_fadeIn();
	while(progress > 0)
	{
		wait(1);
		progress = maxv(progress - (7 * time_step), 0);
		KINGMORPH__lerpVec(&camera->x, &camPos, &KINGMORPH__ackmaniaCam->x, progress * 0.01);
		KINGMORPH__lerpAng(&camera->pan, &camAng, &KINGMORPH__ackmaniaCam->pan, progress * 0.01);
		if (media_playing(KINGMORPH__ackmaniaHandle))
			media_tune(KINGMORPH__ackmaniaHandle, progress, 0, 0);		
	}

	mouse_mode = KINGMORPH__lastMouseMode;
	cameraResume();
}

void KINGMORPH_lotteria()
{
	VECTOR camPos;
	VECTOR diffPos;
	ANGLE camAng;
	ANGLE diffAng;
	VECTOR temp;
	var progress = 0;

	if(KINGMORPH__lotteriaCam == NULL)
	{
		return;
	}
	
	cameraPause();
	MUSICMGR_fadeOut(10);
	KINGMORPH__lastMouseMode = mouse_mode;
	mouse_mode = 0;
	
	vec_set(&camPos, &camera->x);
	vec_set(&camAng, &camera->pan);
	
	while(progress < 100)
	{
		wait(1);
		progress = minv(progress + (7 * time_step), 100);
		KINGMORPH__lerpVec(&camera->x, &camPos, &KINGMORPH__lotteriaCam->x, progress * 0.01);
		KINGMORPH__lerpAng(&camera->pan, &camAng, &KINGMORPH__lotteriaCam->pan, progress * 0.01);
		if (media_playing(KINGMORPH__lotteriaHandle))
			media_tune(KINGMORPH__lotteriaHandle, progress, 0, 0);		
	}
	set(player, INVISIBLE);

	while(key_esc == 0 && key_space == 0 && key_enter == 0 && mouse_left == 0) wait(1);
	
	reset(player, INVISIBLE);
	MUSICMGR_fadeIn();
	while(progress > 0)
	{
		wait(1);
		progress = maxv(progress - (7 * time_step), 0);
		KINGMORPH__lerpVec(&camera->x, &camPos, &KINGMORPH__lotteriaCam->x, progress * 0.01);
		KINGMORPH__lerpAng(&camera->pan, &camAng, &KINGMORPH__lotteriaCam->pan, progress * 0.01);
		if (media_playing(KINGMORPH__lotteriaHandle))
			media_tune(KINGMORPH__lotteriaHandle, progress, 0, 0);		
	}

	mouse_mode = KINGMORPH__lastMouseMode;
	cameraResume();
}

void KINGMORPH_dance()
{
	cameraPause();
	KINGMORPH__lastMouseMode = mouse_mode;
	mouse_mode = 0;
	if (KINGMORPH__griechin != NULL)
	{
		set(KINGMORPH__griechin, INVISIBLE);
	}
	set(player, INVISIBLE);
	//set(player, INVISIBLE);
	dd_start();
	while(dd_running())
	{
		wait(1);
	}
	//reset(player, INVISIBLE);
	mouse_mode = KINGMORPH__lastMouseMode;
	if (KINGMORPH__griechin != NULL)
	{
		reset(KINGMORPH__griechin, INVISIBLE);
	}
	reset(player, INVISIBLE);
	cameraResume();
}

//skill1: EntityType 1
//skill2: ItemId -1
//skill3: ItemBorder 25
//skill4  triggerItemId -1
action kingmorphDance()
{
	var animHand = 0;
	var animHandR = 0;
	var animFeet = 0;
	var animFeetR = 0;
	var offset = 0;
	VECTOR* walk;
	VECTOR pos;

	KINGMORPH__griechin = me;
	set(my, INVISIBLE | PASSABLE);
	while(!inventory || !is_level_loaded()) wait(1); //harghhhh
	if (inv_item_search(inventory, my->trigItemId) == NULL)
	{
		my->flags2 |= UNTOUCHABLE;
		return;
	}
		
	reset(my, INVISIBLE);
	interactionItem_remoteStart();	
	vec_set(&pos, &my->x);
	
	while(1)
	{
		if (animFeet >= DANCE_FEET_PERCENT / 2)
		{
			animFeetR = DANCE_FEET_PERCENT - animFeet;
		}
		else
		{
			animFeetR = animFeet;
		}
		ent_animate(me,"walk", animFeetR + offset, ANM_CYCLE);
		animFeet += 6 * time_step;
		if (animFeet > DANCE_FEET_PERCENT)
		{
			offset = 50 - offset;
			animFeet -= DANCE_FEET_PERCENT;
		}

		if (animHand >= DANCE_HAND_PERCENT / 2)
		{
			animHandR = DANCE_HAND_PERCENT - animHand;
		}
		else
		{
			animHandR = animHand;
		}
		ent_animate(me,"talk", animHandR, ANM_ADD | ANM_CYCLE);
		animHand = cycle(animHand + 6 * time_step, 0, DANCE_HAND_PERCENT);

			
		walk = vector(animFeetR * 1.5, sinv(total_ticks * 30) * 5, 0);
		vec_rotate(walk, &my->pan);
		vec_add(walk, pos);
		vec_set(&my->x, walk);

		wait(1);
	}
}

//skill1: EntityType 1
//skill2: ItemId -1
//skill3: ItemBorder 25
action kingmorphCbabe()
{
	var anim = 0;
	
	interactionItem_remoteStart();
	KINGMORPH__cbabe = me;	
	while(1)
	{
		ent_animate(me, "duck", anim, ANM_CYCLE);
		anim = cycle(anim + 3* time_step, 0, 100);
		wait(1);
	}
}

action kingmorphLotti()
{
	my->flags2 |= UNTOUCHABLE;
	while(me) 
	{
		ent_animate(me, "Play", 18 * total_ticks, ANM_CYCLE);
		wait(1);
	}
}

action kingmorphLottifant()
{
	my->flags2 |= UNTOUCHABLE;
	while(me) 
	{
		ent_animate(me, "Action", 16 * total_ticks, ANM_CYCLE);
		wait(1);
	}
}

action kingmorphAckmaniaCm()
{
	set(my, PASSABLE);
	set(my, INVISIBLE);
	my->flags2 |= UNTOUCHABLE;
	KINGMORPH__ackmaniaCam= me;
}

action kingmorphLotteriaCm()
{
	set(my, PASSABLE);
	set(my, INVISIBLE);
	my->flags2 |= UNTOUCHABLE;
	KINGMORPH__lotteriaCam= me;
}

//skill1: EntityType 1
//skill2: ItemId -1
//skill3: ItemBorder 25
action kingmorphAckmania()
{
	KINGMORPH__ackmania = me;
	BMAP* video = bmap_createblack(320, 240, 888);
	KINGMORPH__ackmaniaHandle = media_loop(KINGMORPH__strAckmania, video, 0);
	ent_setskin(me, video, 2);
	interactionItem_remoteStart();
}

//skill1: EntityType 1
//skill2: ItemId -1
//skill3: ItemBorder 25
action kingmorphLotteria()
{
	KINGMORPH__lotteria = me;
	BMAP* video = bmap_createblack(320, 240, 888);
	KINGMORPH__lotteriaHandle = media_loop(KINGMORPH__strLotteria, video, 0);
	ent_setskin(me, video, 2);
	interactionItem_remoteStart();
}

void KINGMORPH__lerpVec(VECTOR* out, VECTOR* start, VECTOR* end, var percentage)
{
	VECTOR diff;
	vec_set(&diff, end);
	vec_sub(&diff, start);
	vec_set(out, start);
	vec_scale(&diff, percentage);
	vec_add(out, &diff);
}

void KINGMORPH__lerpAng(ANGLE* out, ANGLE* start, ANGLE* end, var percentage)
{
	ANGLE diff;
	vec_set(&diff, end);
	vec_sub(&diff, start);
	diff.pan = ang(diff.pan);
	diff.tilt = ang(diff.tilt);
	diff.roll = ang(diff.roll);
	vec_set(out, start);
	vec_scale(&diff, percentage);
	vec_add(out, &diff);
}

void KINGMORPH_exit()
{
		if (media_playing(KINGMORPH__ackmaniaHandle))
		{
			media_stop(KINGMORPH__ackmaniaHandle);
		}
		if (media_playing(KINGMORPH__lotteriaHandle))
		{
			media_stop(KINGMORPH__lotteriaHandle);
		}
}
