#include "types.h"
#include "smartwalk.h"
#include "level_transition.h"
#include "itemmgr.h"
#include "inventory.h"

#define PLAYER_RUN_CLICKSPEED (-0.2)
#define PLAYER_RUN_THRESHOLD 100.0

var player_waitForRun = 0;
var player_runSpeed = 1;

ENTITY* lastClickedEnt = NULL;
ENTITY* ent_flashlight = NULL;


action point_of_inter()
{
	set(my,INVISIBLE | PASSABLE);
	my.ENTITY_TYPE = TYPE_POINTEREST;
}

void player_run_countdown(void)
{
	proc_kill(4);
	player_waitForRun = 1;
	wait(PLAYER_RUN_CLICKSPEED);
	player_waitForRun = 0;
}

action flashlight()
{
	set(my,PASSABLE | INVISIBLE | TRANSLUCENT);
	my.material = mat_mydefault;
	ent_mtlset(my,mat_flight_glow,2);
	wait(1);
	
	while(1)
	{
		if (inv_item_search(inventory, ITEM_ID_FLASHLIGHT) != NULL)
		{
			reset(me, INVISIBLE);
			ent_flashlight = me;
			/*if(my.x < -16 && my.y > 220)
			{
				ent_flashlight = me;
				my.vmask = 0;
			}
			else
			{
				my.vmask = (1<<1);
				ent_flashlight = NULL;
			}*/
		}
		else
		{
			ent_flashlight = NULL;
			set(me, INVISIBLE);
		}
		wait (1);
	}
}

action player_act()
{
	set(my,INVISIBLE);
	lastClickedEnt = NULL;
	my.flags2 |= UNTOUCHABLE;
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
	VECTOR lastTarget;
	vec_set(lastTarget,my.x);


	while(1)
	{
		VECTOR temp;
		
		// If instant level change is done, wait
		if (player_may_walk)
		{
			vec_set(temp,mouse_dir3d);
			vec_scale(temp,9000);
			vec_add(temp,camera.x);
			c_trace(camera.x,temp,USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
	
			if(my.force_stop)
			{
				if(my.ent_smartwalk)
				{
					smartwalk_destroy(pSMARTWALK(my.ent_smartwalk));
					my.ent_smartwalk = 0;
					vec_set(my.target_x,my.x);
					player_runSpeed = 1;
				}
				my.force_stop = 0;
			}
	
			if(mouse_left)
			{
				my.skill21 += time_step;
				if(my.skill21 > 1) // hold down left mouse button to run
				{
					//mouse_left_off = 1;
					my.skill21 -= 1;
					player_runSpeed = 1.875;
				}
				if(mouse_left_off && input_fetch && mouse_pos.y < inventory.panel.pos_y) //screen_size.y-128
				{
					mouse_left_off = 0;
					c_ignore(GROUP_CURSOR_HELPER,0);
					you = NULL;
					c_trace(camera.x,temp,USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
					lastClickedEnt = you; //store entity which was clicked last
					if (interactionItem_isNearPlayer(lastClickedEnt))
					{
						//probably the most terrible hack I've ever done
						continue;
					}
					if(trace_hit)
					{
						/*if (player_waitForRun)
						{
							if (vec_dist(target, lastTarget) < PLAYER_RUN_THRESHOLD)
							{
								player_runSpeed = 2;
							}
						}*/
						if(vec_dist(lastTarget,target) > 16*my.scale_x)
						{
							if(my.ent_smartwalk) smartwalk_destroy(pSMARTWALK(my.ent_smartwalk));
							my.ent_smartwalk = smartwalk_create_path(smd_level,my.x,target);
							if(my.ent_smartwalk)	vec_set(my.target_x,(pSMARTWALK(my.ent_smartwalk)->nodes)[pSMARTWALK(my.ent_smartwalk)->current_node]);
							//player_run_countdown();
							vec_set(lastTarget, target);
						}
					}
				}
			}
			else
			{
				//if(!my.ent_smartwalk) player_runSpeed = 1;
				mouse_left_off = 1;
				my.skill21 = -6;
			}
			
	
			if(my.ent_smartwalk)
			{
				my.target_z = my.z;
				VECTOR temp,temp2;
				vec_diff(temp,my.target_x,my.x);
				if(vec_to_angle(temp2,temp) < (4+16*time_step)*my.scale_x)
				{
					pSMARTWALK(my.ent_smartwalk)->current_node++;
					vec_set(my.target_x,(pSMARTWALK(my.ent_smartwalk)->nodes)[pSMARTWALK(my.ent_smartwalk)->current_node]);
					if(pSMARTWALK(my.ent_smartwalk)->current_node < pSMARTWALK(my.ent_smartwalk)->num_nodes)
					{
						vec_set(my.target_x,(pSMARTWALK(my.ent_smartwalk)->nodes)[pSMARTWALK(my.ent_smartwalk)->current_node]);
						vec_diff(temp,my.target_x,my.x);
					}
					else
					{
						smartwalk_destroy(pSMARTWALK(my.ent_smartwalk));
						my.ent_smartwalk = 0;
						vec_set(my.target_x,my.x);
						player_runSpeed = 1;
					}
				}
				if(my.ent_smartwalk)
				{
				my.skill22 = (player_runSpeed > 1);
					my.pan += ang(temp2.x-my.pan)*0.85*time_step;
					my.tilt = 0;
					var dist = vec_limit(temp,5.25*my.scale_x*time_step*player_runSpeed);
					//c_move(me,nullvector,temp,IGNORE_PASSABLE | GLIDE);
					vec_add(my.x,temp);
					c_ignore(GROUP_CURSOR_HELPER,GROUP_ITEM,0);
					c_trace(vector(my.x,my.y,my.z+48),vector(my.x,my.y,my.z-256),USE_POLYGON | IGNORE_ME | IGNORE_PASSABLE);
					my.z += (target.z+2-my.z)*time_step;
					my.skill20 += dist*1.41/my.scale_x*(1-my.skill22*0.33);
					my.skill20 %= 100;
				}
			}
			else
			{
				if(my.skill20 < 25) my.skill20 = maxv(my.skill20-3*time_step,0);
				if(my.skill20 >= 25 && my.skill20 < 75) my.skill20 += clamp(50-1/40.0-my.skill20,-3*time_step,3*time_step);
				if(my.skill20 >= 75) my.skill20 = minv(my.skill20+3*time_step,100-1/40.0);
			}
			/*if(my.skill22)
			{
				ent_animate(me,"run",my.skill20,ANM_CYCLE);
				if(my.skill23) ent_blendframe(me,me,"walk",my.skill20,my.skill23);
			}
			else
			{
				ent_animate(me,"walk",my.skill20,ANM_CYCLE);
				if(my.skill23) ent_blendframe(me,me,"run",my.skill20,my.skill23);
			}*/
			if(my.skill22) my.skill23 = minv(my.skill23+6*time_step,100);
			else my.skill23 = maxv(my.skill23-6*time_step,0);
				ent_animate(me,"walk",my.skill20,ANM_CYCLE);
				ent_blendframe(me,me,"run",my.skill20,my.skill23);
			
			
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
		}
		
		if(ent_flashlight)
		{
			vec_for_bone(temp,my,"Joint_5");
			vec_set(ent_flashlight.x,temp);
			ang_for_bone(temp,my,"Joint_5");
			vec_set(ent_flashlight.pan,vector(temp.x,temp.y*0.25,temp.z));
			ang_rotate(ent_flashlight.pan,vector(0,10+5*sinv(total_ticks*4),0));
			
			if(my.x < -16 && my.y > 370/*220*/)
			{
				ent_flashlight.vmask = 0;
				vec_for_vertex(flashlight_start,ent_flashlight,133);
				vec_set(flashlight_end,vector(flashlight_range,0,0));
				vec_rotate(flashlight_end,ent_flashlight.pan);
				vec_add(flashlight_end,ent_flashlight.x);
			}
			else
			{
				ent_flashlight.vmask = (1<<1);
				vec_set(flashlight_start,vector(-99999,-99999,-99999));
				vec_set(flashlight_end,flashlight_start);
			}
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

void Player_stop()
{
	player->force_stop = 1;
}
