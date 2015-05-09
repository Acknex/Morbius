var level_change_transition(var in)
{
	level_change_transition_progress = clamp(level_change_transition_progress+in*8*time_step,0,100);
	if(level_change_transition_progress > 0)
	{
		camera.stage = pp_level_change_transition;
	}
	else camera.stage = NULL;
	
	return level_change_transition_progress;
}

void level_change(var level_id, var gate_id)
{
	gate_entry_id = gate_id;
	input_fetch = 0;
	while(level_change_transition(1) < 100) wait(1);
	level_load((txt_level_wmbs.pstring)[level_id]);
	if(!smd_level) smd_level = smartwalkdata_create();
	smartwalkdata_fill(smd_level);
	while(level_change_transition(-1) > 0) wait(1);
	input_fetch = 1;
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

//skill1: this_id 0
//skill2: to_id 0
action level_gate()
{
	my.ENTITY_TYPE = TYPE_LEVEL_GATE;
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
	set(my,INVISIBLE | PASSABLE); //TRANSLUCENT
	while(1)
	{
		if(player)
		{
			if(player.x > my.skill10 && player.y > my.skill11 && player.x < my.skill12 && player.y < my.skill13)
			{
				level_change(integer(my.skill2*0.01),my.skill2);
				break;
			}
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
