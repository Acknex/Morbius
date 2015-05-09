#include "types.h"
#include "smartwalk.h"
#include "level_transition.h"

action player_act()
{
	set(my,INVISIBLE);
	wait(1);
	level_change_set_player_position(my);
	c_trace(my.x,vector(my.x,my.y,my.z-256),USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
	my.z = target.z+32;
	reset(my,INVISIBLE);
	player = me;
	c_setminmax(my);
	vec_fill(my.min_x,-16);
	vec_fill(my.max_x,16);
	vec_set(my.target_x,my.x);
	while(1)
	{
		if(mouse_left)
		{
			if(mouse_left_off && input_fetch)
			{
				mouse_left_off = 0;
				VECTOR temp;
				vec_set(temp,mouse_dir3d);
				vec_scale(temp,9000);
				vec_add(temp,camera.x);
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
			var dist = vec_limit(temp,8.75*time_step);
			//c_move(me,nullvector,temp,IGNORE_PASSABLE | GLIDE);
			vec_add(my.x,temp);
			c_trace(my.x,vector(my.x,my.y,my.z-256),USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
			my.z += (target.z+32-my.z)*time_step;
			my.skill20 += dist*1.1;
			my.skill20 %= 100;
			ent_animate(me,"run",my.skill20,ANM_CYCLE);
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
			}
		}
		wait(1);
	}
}
