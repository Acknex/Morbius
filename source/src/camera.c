#ifndef CAMERA_C
	#define CAMERA_C

	#include "types.h"

	var cameraPaused = 0;
	
	void cameraLoop() {

		while(player == NULL) wait(1);

		while(player) {
			
			if (!cameraPaused)
			{
				switch(activeCameraType) {
					case CAMERA_TYPE_FIXED_FOLLOW:
					case CAMERA_TYPE_MULTIPLE_FOLLOW:
					moveCameraFixedFollow();
					break;
					
					case CAMERA_TYPE_SPLINE:
					moveCameraSpline();
					break;
					
					case CAMERA_TYPE_AXIS:
					moveCameraAxis(0);
					break;
				}
			}
			wait(1);
		}
	}

	void actSplineCamMin() {
		vec_set(vecSplineCamMin, my.x);
	}

	void actSplineCamMax() {
		vec_set(vecSplineCamMax, my.x);
	}

	void cameraInit() {
		camera_initialized = 1;
		entCameraPathEntity = ent_create(NULL, nullvector, NULL);
		path_set(entCameraPathEntity, "path_000");
		cameraPathLength = path_length(entCameraPathEntity);
		cameraPathPlayerPos = 0;	
	}

	var isCameraInitialized()
	{
		return camera_initialized;
	}

	void moveCameraFixedFollow() {
		vec_set(vecCameraTemp.x, player.x);
		vec_sub(vecCameraTemp.x, camera.x);
		vec_to_angle(camera.pan, vecCameraTemp);
	}

	var get_nearest_path_point(ENTITY* ent, char* pathname)
	{
		ENTITY* e = ent_create(NULL, ent->x, NULL);
		path_set(e, pathname);
		var length = path_length(e);
		var optDist = 0;
		var entDist = 10000;
		
		var dist = 0;
		var vLastPos[3];
		var vDir[3];
		while(dist <= length)
		{
			path_spline(e,e.x,dist);
			dist += 5*time_step;
			if (vec_dist(e->x, ent->x) < entDist) {
				entDist = vec_dist(e->x, ent->x);
				optDist = dist;
			}
		}
		ptr_remove(e);
		return optDist;
	}

	void moveCameraSpline() {
		VECTOR temp;
		
		cameraPathPlayerPos += clamp((get_nearest_path_point(player, "path_000")-cameraPathPlayerPos)*0.25,-16,16)*time_step;
		path_spline(entCameraPathEntity, temp, cameraPathPlayerPos);
		vec_lerp(camera.x,camera.x,temp,0.125*time_step);
		//cameraPathPlayerPosOld = cameraPathPlayerPos;

		// Look at the player
		vec_set(vecCameraTemp.x, vector(player.x,player.y,player.z+48));
		vec_sub(vecCameraTemp.x, camera.x);
		vec_to_angle(temp, vecCameraTemp);
		camera.pan += ang(temp.x-camera.pan)*0.35*time_step;
		camera.tilt += ang(temp.y-camera.tilt)*0.35*time_step;
	}

	void moveCameraAxis(int axis) {
		switch(axis) {
			case 0: camera.x = player.x; break;
			case 1: camera.y = player.y; break;
			case 2: camera.z = player.z; break;
		}
		vec_set(vecCameraTemp.x, player.x);
		vec_sub(vecCameraTemp.x, camera.x);
		vec_to_angle(camera.pan, vecCameraTemp);
	}

	void cameraPause()
	{
		cameraPaused = 1;
	}
	
	void cameraResume()
	{
		cameraPaused = 0;
	}
	
	void actDynamicCamera() {
		my.ENTITY_TYPE = TYPE_DYNAMIC_CAMERA;
		set(me,PASSABLE);
		set(me,INVISIBLE);
		while(1) {
			if (activeCameraType == CAMERA_TYPE_MULTIPLE_FOLLOW) {
				var dist = c_trace(my.x,player.x,IGNORE_SPRITES|USE_BOX|IGNORE_ME);
				if (you == player)  {
					
					if (vec_dist(camera.x,player.x) >
					vec_dist(my.x, player.x)) {
						
						vec_set(camera.x,	my.x);
					}
				}
			}
			wait(1);
		}
	}

	void actSetCameraType() {
		set(me,PASSABLE);
		set(me,INVISIBLE);
		if (my.skill2 == 1) {
			cameraInit();
		}
		
		if ((my.skill2 == 0) || (my.skill2 > 1)) {
			vec_set(camera.x, my.x);
		}
		
		activeCameraType = my.skill2;
	}

	//skill1: max_nodes 0
	action actCameraIntro()
	{
		set(my,INVISIBLE | PASSABLE | UNLIT);
		my.flags2 |= UNTOUCHABLE;
		path_getnode(my,1,my.x,NULL);
		my.skill10 = 2;
		path_getnode(my,my.skill10,my.skill12,NULL);
		
		while(!player) wait(1);
		
		while(my.skill10 <= my.skill1)
		{
			VECTOR temp,temp2;
			
			my.skill11 = minv(my.skill11+time_step,24);
			vec_diff(temp,my.skill12,my.x);
			if(vec_length(temp) > my.skill11*time_step) vec_normalize(temp,my.skill11*time_step);
			else
			{
				my.skill10++;
				if(my.skill10 <= my.skill1) path_getnode(my,my.skill10,my.skill12,NULL);
			}
			vec_add(my.x,temp);
			vec_set(camera.x,my.x);
			
			vec_diff(temp, vector(player.x,player.y,player.z+48), camera.x);
			vec_to_angle(temp2, temp);
			camera.pan += ang(temp.x-camera.pan)*0.35*time_step;
			camera.tilt += ang(temp.y-camera.tilt)*0.35*time_step;
			
			wait(1);
		}
		ptr_remove(me);
	}

#endif