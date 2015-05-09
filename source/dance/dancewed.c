#ifndef dancewed_c
#define dancewed_c

var iwannaplay = 0;

var gg_arc = 70;

SOUND* snd_dd_song = "Sumik_dj_-_wigi.ogg";

ENTITY* ge_morbius = NULL;
ENTITY* ge_babe = NULL;

var waitfornextgdp = 0;

TEXT* gdp = {
	string ("LRUD","UUDD");
}

var sndvol = 100;

void snd_dd_init (TEXT* snd_dd, SOUND** snd_dd_p) {

	int i;
	for (i = 0; i < snd_dd->strings; i++) {
		snd_dd_p[i] = snd_create((snd_dd->pstring)[i]);
	}
}

void snd_dd_play (TEXT* snd_dd, SOUND** snd_dd_p)
{
	if (snd_dd != NULL && snd_dd_p != NULL) {
	
		int n = random(snd_dd->strings);
		
		if (n == snd_dd->pos_x) {
			n = (n+1) % snd_dd->strings;
		}
		
		snd_dd->pos_x = n;
		
		SOUND* s = snd_dd_p[n];
		if (s != NULL) {
			snd_play(s, sndvol, 0);
		}
	}
}

TEXT* snd_dd_shit = {string ("dd_shit_0.ogg","dd_shit_1.ogg","dd_shit_2.ogg");} SOUND* snd_dd_shit_p[3];
TEXT* snd_dd_tauntb = {string ("dd_tauntb_0.ogg","dd_tauntb_1.ogg","dd_tauntb_2.ogg","dd_tauntb_3.ogg");} SOUND* snd_dd_tauntb_p[4];
TEXT* snd_dd_tauntm = {string ("dd_tauntm_0.ogg","dd_tauntm_1.ogg","dd_tauntm_2.ogg","dd_tauntm_3.ogg","dd_tauntm_4.ogg");} SOUND* snd_dd_tauntm_p[5];
TEXT* snd_dd_exceeded = {string ("dd_exceeded_0.ogg","dd_exceeded_1.ogg");} SOUND* snd_dd_exceeded_p[5];
TEXT* snd_dd_letsplay = {string ("dd_letsplay_0.ogg","dd_letsplay_1.ogg");} SOUND* snd_dd_letsplay_p[5];
TEXT* snd_dd_soso = {string ("dd_soso_0.ogg","dd_soso_1.ogg");} SOUND* snd_dd_soso_p[5];
TEXT* snd_dd_success = {string ("dd_success_0.ogg","dd_success_1.ogg");} SOUND* snd_dd_success_p[5];
TEXT* snd_dd_nextstep = {string ("dd_nextstep_0.ogg","dd_nextstep_1.ogg");} SOUND* snd_dd_nextstep_p[5];

void snd_dd_shit_play () {snd_dd_play(snd_dd_shit, snd_dd_shit_p);}
void snd_dd_tauntb_play () {snd_dd_play(snd_dd_tauntb, snd_dd_tauntb_p);}
void snd_dd_tauntm_play () {snd_dd_play(snd_dd_tauntm, snd_dd_tauntm_p);}
void snd_dd_exceeded_play () {snd_dd_play(snd_dd_exceeded, snd_dd_exceeded_p);}
void snd_dd_letsplay_play () {snd_dd_play(snd_dd_letsplay, snd_dd_letsplay_p);}
void snd_dd_soso_play () {snd_dd_play(snd_dd_soso, snd_dd_soso_p);}
void snd_dd_success_play () {snd_dd_play(snd_dd_success, snd_dd_success_p);}
void snd_dd_nextstep_play () {snd_dd_play(snd_dd_nextstep, snd_dd_nextstep_p);}

void snd_dd_startup () {
	snd_dd_init(snd_dd_shit, snd_dd_shit_p);
	snd_dd_init(snd_dd_tauntb, snd_dd_tauntb_p);
	snd_dd_init(snd_dd_tauntm, snd_dd_tauntm_p);
	snd_dd_init(snd_dd_exceeded, snd_dd_exceeded_p);
	snd_dd_init(snd_dd_letsplay, snd_dd_letsplay_p);
	snd_dd_init(snd_dd_soso, snd_dd_soso_p);
	snd_dd_init(snd_dd_success, snd_dd_success_p);
	snd_dd_init(snd_dd_nextstep, snd_dd_nextstep_p);
}

STRING* gdp_puzzle = NULL;
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

int max_fails = 8;

var camlerpfac = 0.7;

int dd_cam_type = 0; // 0 = intro // 1 = start // 2 = play

action spawnkeyac () {

	set(my,ZNEAR|UNLIT);
	my->ambient = 200;
	my->material = mtl_unlit;
	my->scale_x = my->scale_y = my->scale_z = 0.1;
	
	var xx = 25;
	
	//If pan and tilt both are 0, but the roll angle is nonzero
	my->roll = 2;
	
	while (xx > 0) {
		var t = 3 * time_step;
		my->z += t;
		xx -= t;
		my->scale_x = my->scale_y = my->scale_z = my->scale_z+0.01 * time_step;
		wait(1);
	}
	
	ptr_remove(my);
}

int spawni = 0;

void spawnkey (ENTITY* ent, char* str) {
	
	VECTOR vv;
	vec_set(&vv, ent->x);
	vec_to_screen(vv, camera);
	
	spawni++;
	
	var gg = minv((screen_size.x - vv.x) / 3, vv.x / 3);
	
	vv.x += gg;
	
	vv.z = vec_dist(ent->x, camera->x);
   vec_for_screen(vv,camera);
	
	ent_create(str, &vv, spawnkeyac);
}


action esnd_song () {
	ent_playsound(my, snd_dd_song, 500);
}

// skill1: CAM_INDEX 0
// skill2: ArcStart 80
// skill3: ArcEnd 80
// skill4: Type 0
action dd_intro ()
{
	set(my, INVISIBLE);
	gdp_puzzle = (gdp->pstring)[gdp_index];
	
	int type = my->DD_CAM_TYPE;	
	
	if (my->DD_CAM_INDEX == 0 && my->DD_CAM_TYPE == 0) {
		gdp_index = 0;
		dd_cam_index = 0;		
		total_fails = 0;
		isfailed = 0;
		ent_create(NULL, nullvector, esnd_song);
	}	
	
	while (type != dd_cam_type) {
		wait(1);
	}
	
	while (dd_cam_index < my->DD_CAM_INDEX && type == dd_cam_type) {
		wait(1);
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
			gdp_puzzle = (gdp->pstring)[gdp_index];
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

var gg_stepcomplete = 0;

VECTOR ee_babe_pos;
VECTOR ee_babe_ang;

STRING* strspwawnkey = "#32";

action ee_play ()
{
	ge_babe = my;
	vec_set(&ee_babe_pos, my->x);
	vec_set(&ee_babe_ang, my->pan);
	
	gg_stepcomplete = 0;

	while (dd_cam_type != 2 && !isfailed) {
		ent_animate(my, fem_anim_intro, (total_ticks * 10) % 100, ANM_CYCLE);
		wait(1);
	}
	
	while (gdp_index < gdp->strings)
	{
		tt_waitforplayer = 0;
		switch_go = 1;
		
		wait(1);
		
		vec_set(my->x, &ee_babe_pos);
		vec_set(my->pan, &ee_babe_ang);
		
		gg_playlen = clamp(gg_startlen, 1, 	str_len(gdp_puzzle));
		
		while (gg_playlen <= str_len(gdp_puzzle) && !isfailed) {
		
			int i;
			
			for (i = 0; i < gg_playlen && !isfailed; i++) {
			
				// get char for LRUD
				var c = str_getchr(gdp_puzzle, i+1);
				str_cpy(gg_playstr, " ");
				str_setchr(gg_playstr, 1, c);
				
				// build anim str
				str_cpy(gg_playanim, "anim");
				str_cat(gg_playanim, gg_playstr);
				
				snd_dd_tauntb_play ();
				
				// keys
				str_cpy(strspwawnkey, "dd_arr_");
				str_cat(strspwawnkey, gg_playstr);
				str_cat(strspwawnkey, ".tga");				
				spawnkey(my, strspwawnkey);
			
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
			
			snd_dd_letsplay_play();
			
			var tt_timer = 1.5 * 16;
			while (tt_timer > 0 && !isfailed) {	
				ent_animate(my, fem_anim_finishplay, (total_ticks * 10) % 100, ANM_CYCLE);
				tt_timer -= time_step;
				wait(1);
			}
			
			// wait for player
		
			tt_waitforplayer = 1;
			
			while (tt_waitforplayer && !isfailed && !waitfornextgdp) {
				ent_animate(my, fem_anim_waitfor, (total_ticks * 10) % 100, ANM_CYCLE);
				wait(1);
			}
			
			wait(1);
			
			while (!switch_go && !isfailed && !waitfornextgdp) {
				ent_animate(my, fem_anim_waitfor, (total_ticks * 10) % 100, ANM_CYCLE);
				wait(1);
			}
			
			if (waitfornextgdp) {
				
				while (waitfornextgdp) {
					wait(1);
				}
				
				break;
				
			} else {
				gg_playlen++;
			}
		}
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
	
	wait(1);
	
	while (gg_playlen <= str_len(gdp_puzzle) && !isfailed) {
	
		while ((tt_waitforplayer == 0 && !isfailed) || waitfornextgdp) {
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
		
			if (waitfornextgdp) {
				break;
			}
		
			switch_go = 1;
			wait(1);
		}
		
		p = 100;
		firstFrame = 1;
		
		if (!waitfornextgdp)
		{
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

VECTOR ee_morbius_pos;
VECTOR ee_morbius_ang;

action ee_morbius ()
{
	ge_morbius = my;
	vec_set(&ee_morbius_pos, my->x);
	vec_set(&ee_morbius_ang, my->pan);
	
	while (gdp_index < gdp->strings)
	{
		while (gg_playlen <= str_len(gdp_puzzle) && !isfailed && !gg_stepcomplete) {
		
			vec_set(my->x, &ee_morbius_pos);
			vec_set(my->pan, &ee_morbius_ang);
		
			while ((!tt_waitforplayer || !switch_go) && !isfailed) {
				ent_animate(my, male_anim_waitafterplay, (total_ticks * 10) % 100, ANM_CYCLE);
				draw_text("LL-664",500,100,vector(100,100,255));
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
					snd_dd_shit_play ();
					fails++;
					total_fails++;
				} else {
					snd_dd_tauntm_play ();
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
			
			if (fails == 0) {
				// perfect
				snd_dd_success_play();
			} else if (fails <= 2) {
				snd_dd_soso_play ();
			}
			
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
			
			if (gg_playlen == str_len(gdp_puzzle)) {		
				
				gg_stepcomplete = 1;
				break;
				
			} else {
			
				// make a pause a little bit
				
				var tt_timer = 1.5 * 16;
				
				while (tt_timer > 0 && !isfailed) {	
					ent_animate(my, male_anim_waitafterplay, (total_ticks * 10) % 100, ANM_CYCLE);
					tt_timer -= time_step;
					draw_text("LL-775",500,100,vector(100,100,255));
					wait(1);
				}
			}
			
			tt_waitforplayer = 0;		
		}
		
		while (gg_stepcomplete) {
			draw_text("LL-785",500,100,vector(100,100,255));
			wait(1);
		}
		
		wait(1);
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
	
	snd_dd_exceeded_play();

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

void gdp_next ()
{
	gdp_index++;
	gdp_puzzle = (gdp->pstring)[gdp_index];
}

action ee_stepcompl ()
{
	set(my, INVISIBLE);
	
	waitfornextgdp = 0;
	
	while (!gg_stepcomplete) {
		wait(1);
	}	
	
	waitfornextgdp = 1;
	
	var p = 0;
	
	var firstFrame = 1;
	
	snd_dd_nextstep_play ();

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
	
	gdp_next();
	
	waitfornextgdp = 0;
	gg_stepcomplete = 0;
}

#endif /*dancewed_c*/