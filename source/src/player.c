#include "types.h"
#include "smartwalk.h"
#include "level_transition.h"
#include "itemmgr.h"
#include "inventory.h"

ENTITY* lastClickedEnt = NULL;
ENTITY* ent_flashlight = NULL;

action point_of_inter()
{
	set(my,INVISIBLE | PASSABLE);
	my.ENTITY_TYPE = TYPE_POINTEREST;
}

action flashlight()
{
	set(my,PASSABLE);
	wait(1);
	ent_flashlight = me;
}

action player_act()
{
	set(my,INVISIBLE);
	ent_flashlight = NULL;
	wait(1);
	level_change_set_player_position(my);
	c_trace(my.x,vector(my.x,my.y,my.z-256),USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
	my.z = target.z+0;
	reset(my,INVISIBLE);
	my.material = mat_character;
	player = me;
	c_setminmax(my);
	vec_fill(my.min_x,-16*my.scale_x);
	vec_fill(my.max_x,16*my.scale_x);
	vec_set(my.target_x,my.x);
	while(!inventory || !is_level_loaded()) wait(1);
	while(1)
	{
		
		// If instant level change is done, wait
		while(player_may_walk == 0) wait(1);
		VECTOR temp;
		vec_set(temp,mouse_dir3d);
		vec_scale(temp,9000);
		vec_add(temp,camera.x);
		c_trace(camera.x,temp,USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
		//TODO: take care of: item in hand
		
		//HACK: player close previously clicked item? stop early
		if (lastClickedEnt != NULL) 
		{
			if (vec_dist(my.x, lastClickedEnt.x) < PLAYER_NEAR_DIST && lastClickedEnt.ENTITY_TYPE == TYPE_ITEM) my.force_stop = 1;
		}
		
		if(my.force_stop)
		{
			if(my.ent_smartwalk)
			{
				smartwalk_destroy(pSMARTWALK(my.ent_smartwalk));
				my.ent_smartwalk = 0;
				vec_set(my.target_x,my.x);
			}
			my.force_stop = 0;
		}

		if(mouse_left)
		{
			if(mouse_left_off && input_fetch && mouse_pos.y < inventory.panel.pos_y) //screen_size.y-128
			{
				mouse_left_off = 0;
				c_ignore(GROUP_CURSOR_HELPER,0);
				you = NULL;
				c_trace(camera.x,temp,USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
				lastClickedEnt = you; //store entity which was clicked last
				if(trace_hit)
				{
					if(my.ent_smartwalk) smartwalk_destroy(pSMARTWALK(my.ent_smartwalk));
					my.ent_smartwalk = smartwalk_create_path(smd_level,my.x,target);
					if(my.ent_smartwalk)	vec_set(my.target_x,(pSMARTWALK(my.ent_smartwalk)->nodes)[pSMARTWALK(my.ent_smartwalk)->current_node]);
				}
			}
		}
		else mouse_left_off = 1;

		my.target_z = my.z;
		VECTOR temp,temp2;
		vec_diff(temp,my.target_x,my.x);
		if(vec_to_angle(temp2,temp) > 4)
		{
			my.pan += ang(temp2.x-my.pan)*0.85*time_step;
			my.tilt = 0;
			var dist = vec_limit(temp,5.25*my.scale_x*time_step);
			//c_move(me,nullvector,temp,IGNORE_PASSABLE | GLIDE);
			vec_add(my.x,temp);
			c_ignore(GROUP_CURSOR_HELPER,GROUP_ITEM,0);
			c_trace(vector(my.x,my.y,my.z+48),vector(my.x,my.y,my.z-256),USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
			my.z += (target.z+2-my.z)*time_step;
			my.skill20 += dist*1.41/my.scale_x;
			my.skill20 %= 100;
			ent_animate(me,"walk",my.skill20,ANM_CYCLE);
		}
		else
		{
			if(my.ent_smartwalk)
			{
				pSMARTWALK(my.ent_smartwalk)->current_node++;
				vec_set(my.target_x,(pSMARTWALK(my.ent_smartwalk)->nodes)[pSMARTWALK(my.ent_smartwalk)->current_node]);
				if(pSMARTWALK(my.ent_smartwalk)->current_node < pSMARTWALK(my.ent_smartwalk)->num_nodes) vec_set(my.target_x,(pSMARTWALK(my.ent_smartwalk)->nodes)[pSMARTWALK(my.ent_smartwalk)->current_node]);
				else
				{
					smartwalk_destroy(pSMARTWALK(my.ent_smartwalk));
					my.ent_smartwalk = 0;
					vec_set(my.target_x,my.x);
				}
				ent_animate(me,"walk",my.skill20,ANM_CYCLE);
			}
			else
			{
				if(my.skill20 < 25) my.skill20 = maxv(my.skill20-3*time_step,0);
				if(my.skill20 >= 25 && my.skill20 < 75) my.skill20 += clamp(50-1/40.0-my.skill20,-3*time_step,3*time_step);
				if(my.skill20 >= 75) my.skill20 = minv(my.skill20+3*time_step,100-1/40.0);
				ent_animate(me,"walk",my.skill20,ANM_CYCLE);
			}
		}
		my.skill60 += time_step;
		if(my.skill60 > 4)
		{
			my.skill60 -= 4;
			my.skill61 = 256*my.scale_x;
			my.skill62 = 0;
			for(you = ent_next(NULL); you; you = ent_next(you))
			{
				if(your.ENTITY_TYPE == TYPE_POINTEREST || your.ENTITY_TYPE == TYPE_ITEM)
				{
					var dist = vec_dist(vector(my.x,my.y,my.z+40*my.scale_x),your.x);
					if(dist < my.skill61)
					{
						my.skill61 = dist;
						my.skill62 = you;
					}
				}
			}
		}
		VECTOR dir,vang,temp;
		if(my.skill62)
		{
			you = (ENTITY*)my.skill62;
			vec_set(temp,your.x);
			vec_to_ent(temp,my);
			if(temp.x < 0) vec_scale(my.skill36,1-0.5*time_step);
			else
			{
				vec_diff(dir,temp,vector(0,0,45));
				vec_to_angle(vang,dir);
				vang.x = clamp(ang(vang.x),-70,70);
				vang.y = clamp(ang(vang.y),-70,70);
				vec_lerp(my.skill36,my.skill36,vang,0.25*time_step);
			}
		}
		else vec_scale(my.skill36,1-0.5*time_step);
		vec_set(temp,my.skill36);
		vec_scale(temp,0.5);
		ent_bonerotate(my,"Joint_2_4",temp);
		ent_bonerotate(my,"Joint_3_5",temp);
		
		if(ent_flashlight)
		{
			vec_for_bone(temp,my,"Joint_5");
			vec_set(ent_flashlight.x,temp);
			ang_for_bone(temp,my,"Joint_5");
			vec_set(ent_flashlight.pan,temp);
			
			vec_for_vertex(flashlight_start,ent_flashlight,133);
			vec_set(flashlight_end,vector(flashlight_range,0,0));
			vec_rotate(flashlight_end,ent_flashlight.pan);
			vec_add(flashlight_end,ent_flashlight.x);
		}
		wait(1);
	}
}

ENTITY* Player_getLastClickedEnt()
{			
	return lastClickedEnt;
}

void Player_resetLastClickedEnt()
{			
	lastClickedEnt = NULL;
}
