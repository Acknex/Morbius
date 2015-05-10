#include "types.h"
#include "smartwalk.h"
#include "level_transition.h"
#include "itemmgr.h"
#include "inventory.h"

action point_of_inter()
{
	set(my,INVISIBLE | PASSABLE);
	my.ENTITY_TYPE = TYPE_POINTEREST;
}

action player_act()
{
	set(my,INVISIBLE);
	wait(1);
	level_change_set_player_position(my);
	c_trace(my.x,vector(my.x,my.y,my.z-256),USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
	my.z = target.z+0;
	reset(my,INVISIBLE);
	player = me;
	c_setminmax(my);
	vec_fill(my.min_x,-16);
	vec_fill(my.max_x,16);
	vec_set(my.target_x,my.x);
	while(1)
	{
		VECTOR temp;
		vec_set(temp,mouse_dir3d);
		vec_scale(temp,9000);
		vec_add(temp,camera.x);
		c_trace(camera.x,temp,USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
		//TODO: take care of: item in hand
		mouse_map = bmp_cursor_array[TYPE_ITEM_DEFAULT];
		if(you)
		{
			if (itemInHand == NULL) { 
				if(your.ENTITY_TYPE == TYPE_ITEM || your.ENTITY_TYPE == TYPE_LEVEL_GATE) mouse_map = bmp_cursor_array[TYPE_ITEM_EXIT];
			}
		}
		if(mouse_left)
		{
			if(mouse_left_off && input_fetch)
			{
				mouse_left_off = 0;
		c_ignore(GROUP_CURSOR_HELPER,0);
		c_trace(camera.x,temp,USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
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
			var dist = vec_limit(temp,5.25*time_step);
			//c_move(me,nullvector,temp,IGNORE_PASSABLE | GLIDE);
			vec_add(my.x,temp);
			c_trace(vector(my.x,my.y,my.z+48),vector(my.x,my.y,my.z-256),USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
			my.z += (target.z+2-my.z)*time_step;
			my.skill20 += dist*1.41;
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
			my.skill61 = 256;
			my.skill62 = 0;
			for(you = ent_next(NULL); you; you = ent_next(you))
			{
				if(your.ENTITY_TYPE == TYPE_POINTEREST)
				{
					var dist = vec_dist(vector(my.x,my.y,my.z+40),your.x);
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
			/*vec_for_bone(tpos,my,"Joint_3_5");
			ang_for_bone(tang,my,"Joint_3_5");
			vec_diff(dir,your.x,tpos);
			vec_to_angle(vang,dir);
			ang_diff(dir,vang,my.pan);
			if(abs(ang(dir.x)) < 60)
			{
				dir.z = 0;
				dir.x = clamp(ang(dir.x),-45,45);
				dir.y = clamp(-ang(dir.y),-45,45);
				ent_bonerotate(my,"Joint_3_5",dir);
			}*/
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
		wait(1);
	}
}


