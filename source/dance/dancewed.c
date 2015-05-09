#ifndef dancewed_c
#define dancewed_c

var iwannaplay = 1;

var gg_arc = 70;

ENTITY* ge_morbius = NULL;
ENTITY* ge_babe = NULL;

TEXT* gdp = {
	string ("LRUD","UUDD");
}

STRING* gdp_puzzle = "#100";
int gdp_index = 0;

int total_fails = 0;
var isfailed = 0;

#define DD_FPS 30
#define DD_FAC 1.5

#define DD_CAM_INDEX skill1
#define DD_CAM_ARC_START skill2
#define DD_CAM_ARC_END skill3
#define DD_CAM_TYPE skill4
int dd_cam_index = 0;

int max_fails = 3;

var camlerpfac = 0.7;

int dd_cam_type = 0; // 0 = intro // 1 = start // 2 = play

// skill1: CAM_INDEX 0
// skill2: ArcStart 80
// skill3: ArcEnd 80
// skill4: Type 0
action dd_intro ()
{
	set(my, INVISIBLE);
	
	int type = my->DD_CAM_TYPE;	
	
	if (my->DD_CAM_INDEX == 0 && my->DD_CAM_TYPE == 0) {
		gdp_index = 0;
		dd_cam_index = 0;		
		total_fails = 0;
		isfailed = 0;
	}	
	
	while (type != dd_cam_type) {
		wait(1);
	}
	
	while (dd_cam_index < my->DD_CAM_INDEX && type == dd_cam_type) {
		wait(1);
	}
	
	if (dd_cam_type == 2) {
		str_cpy(gdp_puzzle, (gdp->pstring)[gdp_index]);
	}
	
	camera->arc = gg_arc;
	
	var p = 0;
	
	var firstFrame = 1;
	
	while (1) {	
		
		VECTOR vCam, vTarget;
		
		vec_for_bone(&vCam, my, "Camera");
		if (firstFrame) {
			vec_set(camera->x, &vCam);
			firstFrame = 0;
		} else {
			vec_lerp(camera->x, camera->x, &vCam, camlerpfac);
		}
		
		vec_for_bone(&vTarget, my, "Target");
		vec_to_angle(camera.pan,vec_diff(NULL, &vTarget, camera.x));
		
		if (type == 0 || type == 1) {		
			
			ent_animate(my, "", p, 0);
			p = clamp(p + (DD_FPS / 16) * DD_FAC * time_step, 0, 100);
			
			if (p >= 100 || iwannaplay) {
				break;
			}
			
		} else if (type == 2) {
		
			ent_animate(my, "", (1 - (absv(p - 100) / 100)) * 100, 0);
			
			p = (p + (DD_FPS / 16) * DD_FAC * time_step);
			p %= 200;
			
		}
		
		camera->arc = gg_arc;
		
		camera->roll = 0;
		
		draw_text(my->type, 100, 100, COLOR_WHITE);
		
		wait(1);
	}
	
	camera->arc = gg_arc;
	
	if (type == 0) {	
		if (is(my, FLAG1)) {
			dd_cam_index = 0;
			dd_cam_type++;
		} else {
			dd_cam_index++;
		}	
	} else if (type == 1) {
		dd_cam_index = 0;
		dd_cam_type++;
	}
}

STRING* gg_playstr = "#4";
STRING* gg_playanim = "#4";

int tt_waitforplayer = 0;

var switch_go = 0;

// skill1: CAM_INDEX 0
// skill2: ArcStart 80
// skill3: ArcEnd 80
// skill4: Type 0
action dd_play ()
{
	set(my, INVISIBLE);
	
	int type = my->DD_CAM_TYPE;	
	
	if (my->DD_CAM_INDEX == 0 && my->DD_CAM_TYPE == 0) {
		gdp_index = 0;
		dd_cam_index = 0;		
	}
	
	while (type != dd_cam_type) {
		wait(1);
	}
	
	while (1)
	{	
		while (dd_cam_index < my->DD_CAM_INDEX && type == dd_cam_type) {
			wait(1);
		}
		
		if (dd_cam_type == 2) {
			str_cpy(gdp_puzzle, (gdp->pstring)[gdp_index]);
		}	
	
		camera->arc = gg_arc;
		
		var p = 0;
		
		var firstFrame = 1;
		
		while (tt_waitforplayer == 0) {	
			
			VECTOR vCam, vTarget;
			
			vec_for_bone(&vCam, my, "Camera");
			if (firstFrame) {
				vec_set(camera->x, &vCam);
				firstFrame = 0;
			} else {
				vec_lerp(camera->x, camera->x, &vCam, camlerpfac);
			}
			
			vec_for_bone(&vTarget, my, "Target");
			vec_to_angle(camera.pan,vec_diff(NULL, &vTarget, camera.x));
			
			ent_animate(my, "", (1 - (absv(p - 100) / 100)) * 100, 0);
			
			p = (p + (DD_FPS / 16) * DD_FAC * time_step);
			p %= 200;
			
			camera->arc = gg_arc;
			
			camera->roll = 0;
			
			draw_text(my->type, 100, 100, COLOR_WHITE);
			
			wait(1);
		}
		
		camera->arc = gg_arc;
		
		if (type == 0) {	
			if (is(my, FLAG1)) {
				dd_cam_index = 0;
				dd_cam_type++;
			} else {
				dd_cam_index++;
			}	
		} else if (type == 1) {
			dd_cam_index = 0;
			dd_cam_type++;
		}
		
		while (tt_waitforplayer == 1) {
			wait(1);
		}
		
		dd_cam_index = 0;
	
		wait(1);	
	}
}

// skill1: CAM_INDEX 0
// skill2: ArcStart 80
// skill3: ArcEnd 80
// skill4: Type 0
action dd_play2 ()
{
	set(my, INVISIBLE);
	
	int type = my->DD_CAM_TYPE;	
	
	if (my->DD_CAM_INDEX == 0 && my->DD_CAM_TYPE == 0) {
		gdp_index = 0;
		dd_cam_index = 0;		
	}
	
	while (type != dd_cam_type) {
		wait(1);
	}
	
	while (1)
	{	
		while (tt_waitforplayer == 0) {
			wait(1);
		}
		
		wait(1);
		
		while (switch_go == 0) {
			wait(1);
		}		
		
		camera->arc = gg_arc;
		
		var p = 0;
		
		var firstFrame = 1;
		
		while (tt_waitforplayer == 1) {	
			
			VECTOR vCam, vTarget;
			
			vec_for_bone(&vCam, my, "Camera");
			if (firstFrame) {
				vec_set(camera->x, &vCam);
				firstFrame = 0;
			} else {
				vec_lerp(camera->x, camera->x, &vCam, camlerpfac);
			}
			
			vec_for_bone(&vTarget, my, "Target");
			vec_to_angle(camera.pan,vec_diff(NULL, &vTarget, camera.x));
			
			ent_animate(my, "", (1 - (absv(p - 100) / 100)) * 100, 0);
			
			p = (p + (DD_FPS / 16) * DD_FAC * time_step);
			p %= 200;
			
			camera->arc = gg_arc;
			
			camera->roll = 0;
			
			draw_text(my->type, 100, 100, COLOR_WHITE);
			
			wait(1);
		}
		
		camera->arc = gg_arc;
		
		wait(1);	
	}
}

int gg_playlen = 0;
int gg_startlen = 3;

STRING* fem_anim_intro = "stand";
STRING* fem_anim_finishplay = "stand";
STRING* fem_anim_waitfor = "stand";

action ee_play ()
{
	ge_babe = my;

	while (dd_cam_type != 2 && !isfailed) {
		ent_animate(my, fem_anim_intro, (total_ticks * 10) % 100, ANM_CYCLE);
		wait(1);
	}
	
	tt_waitforplayer = 0;
	switch_go = 1;
	
	int len = str_len(gdp_puzzle);	
	gg_playlen = clamp(gg_startlen, 1, len);
	
	while (gg_playlen <= len && !isfailed) {
	
		int i;
		
		for (i = 0; i < gg_playlen && !isfailed; i++) {
		
			// get char for LRUD
			var c = str_getchr(gdp_puzzle, i+1);
			str_cpy(gg_playstr, " ");
			str_setchr(gg_playstr, 1, c);
			
			// build anim str
			str_cpy(gg_playanim, "anim");
			str_cat(gg_playanim, gg_playstr);
		
			var p = 0;
			
			// play anim str
			while (p < 100 && !isfailed) {
			
				p = clamp(p + 5 * time_step, 0, 100);
				DEBUG_VAR(p, 200);
				
				// ent_animate... switch!
				ent_animate(my, "jump", p, 0);
				//ent_animate(my, gg_playanim, p, 0);
				draw_text(gg_playanim, 100, 300, COLOR_RED);
				
				wait(1);
			}	
		}
		
		// make a pause a little bit
		
		var tt_timer = 1.5 * 16;
		while (tt_timer > 0 && !isfailed) {	
			ent_animate(my, fem_anim_finishplay, (total_ticks * 10) % 100, ANM_CYCLE);
			tt_timer -= time_step;
			wait(1);
		}
		
		// wait for player
	
		tt_waitforplayer = 1;
		
		while (tt_waitforplayer && !isfailed) {
			ent_animate(my, fem_anim_waitfor, (total_ticks * 10) % 100, ANM_CYCLE);
			wait(1);
		}
		
		wait(1);
		
		while (!switch_go && !isfailed) {
			ent_animate(my, fem_anim_waitfor, (total_ticks * 10) % 100, ANM_CYCLE);
			wait(1);
		}
		
		gg_playlen++;
	}
}



// skill1: CAM_INDEX 0
// skill2: ArcStart 80
// skill3: ArcEnd 80
// skill4: Type 0
action dd_switch ()
{
	set(my, INVISIBLE);
	
	int type = my->DD_CAM_TYPE;	
	
	switch_go = 0;
	int len = str_len(gdp_puzzle);
	
	while (gg_playlen <= len && !isfailed) {
	
		while (tt_waitforplayer == 0 && !isfailed) {
			wait(1);
		}
	
		camera->arc = gg_arc;
		
		var p = 0;
		
		var firstFrame = 1;
		
		while (!isfailed) {	
		
			switch_go = 0;
			
			VECTOR vCam, vTarget;
			
			vec_for_bone(&vCam, my, "Camera");
			if (firstFrame) {
				vec_set(camera->x, &vCam);
				firstFrame = 0;
			} else {
				vec_lerp(camera->x, camera->x, &vCam, camlerpfac);
			}
			
			vec_for_bone(&vTarget, my, "Target");
			vec_to_angle(camera.pan,vec_diff(NULL, &vTarget, camera.x));
			
			ent_animate(my, "", p, 0);
			p = clamp(p + (DD_FPS / 16) * DD_FAC * time_step, 0, 100);
			
			if (p >= 100) {
				break;
			}
				
			camera->arc = gg_arc;
			
			camera->roll = 0;
			
			draw_text(my->type, 100, 100, COLOR_WHITE);
			
			wait(1);
		}
		
		camera->arc = gg_arc;
		
		while (tt_waitforplayer && !isfailed) {
			switch_go = 1;
			wait(1);
		}
		
		p = 100;
		firstFrame = 1;
		
		while (!isfailed) {	
		
			switch_go = 0;
			
			VECTOR vCam, vTarget;
			
			vec_for_bone(&vCam, my, "Camera");
			if (firstFrame) {
				vec_set(camera->x, &vCam);
				firstFrame = 0;
			} else {
				vec_lerp(camera->x, camera->x, &vCam, camlerpfac);
			}
			
			vec_for_bone(&vTarget, my, "Target");
			vec_to_angle(camera.pan,vec_diff(NULL, &vTarget, camera.x));
			
			ent_animate(my, "", p, 0);
			p = clamp(p - (DD_FPS / 16) * DD_FAC * time_step, 0, 100);
			
			if (p <= 0) {
				break;
			}
				
			camera->arc = gg_arc;
			
			camera->roll = 0;
			
			draw_text(my->type, 100, 100, COLOR_WHITE);
			
			wait(1);
		}	
		
		camera->arc = gg_arc;
		
		switch_go = 1;
	}
}

var gg_isdone = 0;
var gg_isgood = 0;

void gdp_ok ()
{
	gg_isdone = 1;
	gg_isgood = 1;
}

void gdp_notok ()
{
	gg_isdone = 1;
	gg_isgood = 0;
}

void gdp_set_L () {
	on_cul = gdp_ok;
	on_cur = on_cuu = on_cud = gdp_notok;
}

void gdp_set_R () {
	on_cur = gdp_ok;
	on_cul = on_cuu = on_cud = gdp_notok;
}

void gdp_set_U () {
	on_cuu = gdp_ok;
	on_cul = on_cur = on_cud = gdp_notok;
}

void gdp_set_D () {
	on_cud = gdp_ok;
	on_cuu = on_cul = on_cur = gdp_notok;
}

void gdp_setkeys (STRING* current, int entered)
{
	int a = str_getchr(current, entered+1);
	
	switch (a)
	{
		/* L */ case 76: gdp_set_L(); break;
		/* R */ case 82: gdp_set_R(); break;
		/* U */ case 85: gdp_set_U(); break;
		/* D */ case 68: gdp_set_D(); break;
	}
}

void gdp_resetkeys ()
{
	on_cur = on_cul = on_cuu = on_cud = NULL;
}

STRING* deb_gg_playstr = "#100";

STRING* male_anim_waitafterplay = "stand";
STRING* male_anim_waitforkey = "stand";
STRING* male_anim_fail = "puke";

STRING* male_anim_perfect = "jump";
STRING* male_anim_okok = "puke";
STRING* male_anim_notgood = "die";

action ee_morbius ()
{
	ge_morbius = my;

	int len = str_len(gdp_puzzle);	

	while (gg_playlen <= len && !isfailed) {
	
		while ((!tt_waitforplayer || !switch_go) && !isfailed) {
			ent_animate(my, male_anim_waitafterplay, (total_ticks * 10) % 100, ANM_CYCLE);
			wait(1);
		}
	
		int i;
		
		int fails = 0;
		
		for (i = 0; i < gg_playlen && !isfailed;) {
		
			// get char for LRUD
			var c = str_getchr(gdp_puzzle, i+1);
			str_cpy(gg_playstr, " ");
			str_setchr(gg_playstr, 1, c);
			
			// build anim str
			str_cpy(gg_playanim, "anim");
			str_cat(gg_playanim, gg_playstr);
			
			gdp_resetkeys();
			gdp_setkeys(gdp_puzzle, i);
			
			gg_isdone = 0;			
			while (!gg_isdone && !isfailed) {
				ent_animate(my, male_anim_waitforkey, (total_ticks * 10) % 100, ANM_CYCLE);
				draw_text(gg_playstr, 100, 300, COLOR_WHITE);
				draw_text(str_printf(NULL, "gg_playlen = %d", gg_playlen), 100, 350, COLOR_WHITE);
				wait(1);
			}
			
			gdp_resetkeys();
			
			if (!gg_isgood) {
				fails++;
				total_fails++;
			}
			
			var p = 0;
			
			// play anim str
			while (p < 100 && !isfailed) {
			
				p = clamp(p + 5 * time_step, 0, 100);
				DEBUG_VAR(p, 200);
				
				if (gg_isgood) {				
					ent_animate(my, "jump", p, 0);
					//ent_animate(my, gg_playanim, p, 0);
					draw_text(gg_playanim, 100, 300, COLOR_RED);
					draw_text("OK", 100, 350, COLOR_RED);
				} else {
					ent_animate(my, male_anim_fail, p, ANM_CYCLE);
					draw_text("FAIL", 100, 300, COLOR_RED);
				}
				
				wait(1);
			}	
			
			if (total_fails >= max_fails) {
				isfailed = 1;
			}			
			
			if (gg_isgood) {
				i++;
			}
		}
		
		var p = 0;
		
		// celebrate
		while (p < 100 && !isfailed) {
		
			p = clamp(p + 5 * time_step, 0, 100);
			
			if (fails == 0) {
				// perfect
				ent_animate(my, male_anim_perfect, p, 0);
			} else if (fails <= 2) {
				ent_animate(my, male_anim_okok, p, 0);
			} else {
				// too much fails
				ent_animate(my, male_anim_notgood, p, 0);
			}			
			
			wait(1);
		}	
		
		// make a pause a little bit
		
		var tt_timer = 1.5 * 16;
		while (tt_timer > 0 && !isfailed) {	
			ent_animate(my, male_anim_waitafterplay, (total_ticks * 10) % 100, ANM_CYCLE);
			tt_timer -= time_step;
			wait(1);
		}
		
		tt_waitforplayer = 0;
	}
}

void sync_babe (ENTITY* cammdl)
{
	if (ge_babe != NULL) {
	
		VECTOR bxyz;
		vec_for_bone(&bxyz, cammdl, "Babe");
		ge_babe->x = bxyz.x;
		ge_babe->y = bxyz.y;
		
		VECTOR toy;
		vec_for_bone(&toy, cammdl, "Sex");
		toy.z = bxyz.z;
		
		vec_to_angle(ge_babe->pan, vec_diff(NULL, &toy, &bxyz));
	}
}

void sync_morbius (ENTITY* cammdl)
{
	if (ge_morbius != NULL) {
	
		VECTOR bxyz;
		vec_for_bone(&bxyz, cammdl, "Morbius");
		ge_morbius->x = bxyz.x;
		ge_morbius->y = bxyz.y;
		
		VECTOR toy;
		vec_for_bone(&toy, cammdl, "Pop");
		toy.z = bxyz.z;
		
		vec_to_angle(ge_morbius->pan, vec_diff(NULL, &toy, &bxyz));
	}
}

action ee_exceed ()
{
	set(my, INVISIBLE);
	
	while (total_fails < max_fails) {
		wait(1);
	}	
	
	isfailed = 1;
	
	var p = 0;
	
	var firstFrame = 1;

	while (p < 100) {
		
		VECTOR vCam, vTarget;
		
		vec_for_bone(&vCam, my, "Camera");
		if (firstFrame) {
			vec_set(camera->x, &vCam);
			firstFrame = 0;
		} else {
			vec_lerp(camera->x, camera->x, &vCam, camlerpfac);
		}
		
		sync_babe(my);
		sync_morbius(my);
		
		vec_for_bone(&vTarget, my, "Target");
		vec_to_angle(camera.pan,vec_diff(NULL, &vTarget, camera.x));
		
		ent_animate(my, "", p, 0);
		p = clamp(p + (DD_FPS / 16) * DD_FAC * time_step, 0, 100);		
		
		wait(1);
	}
}

#endif /*dancewed_c*/