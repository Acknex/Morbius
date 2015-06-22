#include "player.h"
#include "camera.h"
#include "event.h
#include "dialogs.h"
#include "soundmgr.h"
#include "types.h"
#include "inventory.h"
#include "items.h"
#include "chapter.h"

void dynamicLevel_gate_check();
void dynamicLevel_gate_event();

var level_change_transition(var in)
{
	VIEW* view_stage;
	level_change_transition_progress = clamp(level_change_transition_progress+in*8*time_step,0,100);
	if(level_change_transition_progress > 0)
	{
		if(camera.stage != pp_level_change_transition)
		{
		view_stage = camera.stage;
		camera.stage = pp_level_change_transition;
		pp_level_change_transition.stage = view_stage;
	}
	}
	else
	{
		if(camera.stage == pp_level_change_transition)
		{
			camera.stage = pp_level_change_transition.stage;
		}
	}
	
	return level_change_transition_progress;
}

void level_change(var level_id, var gate_id)
{
	level_change(level_id, gate_id, 0);
}

void level_change(var level_id, var gate_id, var silent)
{
	// Always reset camera to NOT use the spline function during level transitions
	level_loaded = 0;
	activeCameraType = CAMERA_TYPE_FIXED_FOLLOW;
	
	gate_entry_id = gate_id;
	input_fetch = 0;
	
	SOUNDMGR_stop();
	while(level_change_transition(1) < 100) wait(1);
	level_load((txt_level_wmbs.pstring)[level_id]);
	vec_set(sky_color,COLOR_BLACK);
	wait(1);
	if (!silent)
	{
		CHAPTER_show(level_id);
	}
	if(!isCameraInitialized())
	{
		cameraInit();
		cameraLoop();
	}
	if(!smd_level) smd_level = smartwalkdata_create();
	smartwalkdata_fill(smd_level);
	if (!silent)
	{
		while(CHAPTER_isVisible()) wait(1);
	}
	level_loaded = 1;
	player_may_walk = 1;
	while(level_change_transition(-1) > 0) wait(1);
	input_fetch = 1;
	mousemgr_set(MOUSE_DEFAULT, NULL);
	mousemgr_hint(NULL);
}

var is_level_loaded()
{
	return level_loaded;
}

//skill1: blue 0
//skill2: green 0
//skill3: red 0
//skill4: fog_start 0
//skill5: fog_end 0
//skill6: fog_color 0
action sky_color_fog_act()
{
	vec_set(sky_color,vector(my.skill1,my.skill2,my.skill3));
	camera.fog_start = my.skill4;
	camera.fog_end = my.skill5;
	fog_color = my.skill6;
	ptr_remove(me);
}

void level_gate_event() {
	if (dlgIsDialogActive() != 0 || EVENT_isLocked() != 0)
		return;
		
	if (event_type == EVENT_TOUCH) {
		mousemgr_set(MOUSE_EXIT, NULL);
	}
	
	if (event_type == EVENT_RELEASE) {
		if (itemInHand != NULL)
		{
			mousemgr_set(MOUSE_DEFAULT, itemInHand->image);
		}
		else
		{
			mousemgr_set(MOUSE_DEFAULT, NULL);
		}
	}
	
	if (event_type == EVENT_CLICK) {
		if (my.DOUBLE_CLICK_TIME >= 100) {
			// Double clicked
			player_may_walk = 0;
			var silent = 0;
			if(is(me, FLAG2)) silent = 1;
			level_change(integer(my.skill2*0.01),my.skill2, silent); // Instant level change
		} else {
			my.DOUBLE_CLICK_TIME = 110;
		}
	}
}

void level_gate_init()
{
	my.ENTITY_TYPE = TYPE_LEVEL_GATE;
	my.SUB_TYPE = TYPE_ITEM_EXIT;
	my.group = GROUP_CURSOR_HELPER;
	vec_set(my.skill20,vector(32,0,0));
	vec_rotate(my.skill20,my.pan);
	vec_add(my.skill20,my.x);
	c_setminmax(my);
	VECTOR temp,temp2;
	vec_set(temp,my.min_x);
	vec_rotate(temp,my.pan);
	vec_add(temp,my.x);
	vec_set(temp2,my.max_x);
	vec_rotate(temp2,my.pan);
	vec_add(temp2,my.x);
	my.skill10 = minv(temp.x,temp2.x);
	my.skill11 = minv(temp.y,temp2.y);
	my.skill12 = maxv(temp.x,temp2.x);
	my.skill13 = maxv(temp.y,temp2.y);
	set(my, POLYGON | TRANSLUCENT); //TRANSLUCENT
	my.alpha = 0;
	my.emask = ENABLE_TOUCH | ENABLE_CLICK | ENABLE_RELEASE;
}

//skill1: this_id 0
//skill2: to_id 0
//flag2: silent 0
action level_gate()
{
	level_gate_init();	
	my.event = level_gate_event;
	
	while(1)
	{
		if(player)
		{
			if(player.x > my.skill10 && player.y > my.skill11 && player.x < my.skill12 && player.y < my.skill13)
			{
				var silent = 0;
				if(is(me, FLAG2)) silent = 1;
				level_change(integer(my.skill2*0.01),my.skill2, silent);
				break;
			}
		}
		
		if (my.DOUBLE_CLICK_TIME > 0) {
			my.DOUBLE_CLICK_TIME -=1 * time_step;
		}
		wait(1);
	}
}

void level_change_set_player_position(ENTITY* ent_pl)
{
	if(gate_entry_id >= 0)
	{
		for(you = ent_next(NULL); you; you = ent_next(you))
		{
			if(your.ENTITY_TYPE == TYPE_LEVEL_GATE && your.skill1 == gate_entry_id)
			{
				vec_set(ent_pl.x,your.skill20);
				vec_set(ent_pl.pan,your.pan);
				break;
			}
		}
	}
}

//dynamicLevel_gate
#define triggerItemId skill3
#define badItemId skill4
#define goodItemId skill5
#define activeItemId skill6
#define ALLOWBADGATE FLAG1
#define gateWasClicked FLAG3


//skill1: this_id 0
//skill2: to_id 0
//skill3: triggerItemId -1
//skill4: badItemId -1
//skill5: goodItemId -1
//flag1: allowBadGate 0
//flag2: silent 0
action dynamicLevel_gate()
{
	level_gate_init();	
	my.event = dynamicLevel_gate_event;
	int itemId;
		
	while(1)
	{
		if (inv_item_search(inventory, my->triggerItemId) != NULL)
		{
			my->activeItemId = my->goodItemId;
		}
		else
		{
			my->activeItemId = my->badItemId;
		}
		
/*		VECTOR* minvec = vector(my.skill10,my.skill11, -100);
		VECTOR* maxvec = vector(my.skill12,my.skill13, 100);
		draw_box3d(minvec,maxvec,vector(0,0,255),100);*/
		if(player)
		{
			if(
				player.x > my.skill10 && player.y > my.skill11 && player.x < my.skill12 && player.y < my.skill13
				&& is(my, gateWasClicked)
			)
			{
				dynamicLevel_gate_check();
				//break;
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

void dynamicLevel_gate_check()
{
	reset(my, gateWasClicked);
	
	ITEM* item = ITEM_get(my->activeItemId);
	
	if (item != NULL)
	{
		ITEM_interaction(item);
	
		if (ITEM_isLastSequence(item) != 0 && 
			(
				((my->activeItemId == my->badItemId) && is(my, ALLOWBADGATE)) ||
				(my->activeItemId == my->goodItemId)
			)
		)
		{
			var silent = 0;
			if(is(me, FLAG2)) silent = 1;
			
			level_change(integer(my.skill2*0.01),my.skill2, silent);
		}
	}
}

void dynamicLevel_gate_event() 
{
	if (dlgIsDialogActive() != 0 || EVENT_isLocked() != 0)
		return;
		
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
			dynamicLevel_gate_check(); // Instant level change
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

