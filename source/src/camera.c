#ifndef DIALOGS_C
#define DIALOGS_C

#include "types.h"

void cameraLoop() {
	
	while(player == NULL) wait(1);
	
	while(player) {
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
	entCameraPathEntity = ent_create(NULL, nullvector, NULL);
	path_set(entCameraPathEntity, "path_000");
	cameraPathLength = path_length(entCameraPathEntity);
	cameraPathPlayerPos = 0;	
}

void moveCameraFixedFollow() {
	vec_set(vecCameraTemp.x, player.x);
	vec_sub(vecCameraTemp.x, camera.x);
	vec_to_angle(camera.pan, vecCameraTemp);
}

void moveCameraSpline() {
	// Calculate procentual difference between level min/max
	cameraPathPlayerPos = player.x - vecSplineCamMin.x;
	cameraPathPlayerPos = cameraPathPlayerPos / (vec_dist(vecSplineCamMax.x, vecSplineCamMin.x) + 0.0001); // Avoid 0 division
	cameraPathPlayerPos = cameraPathPlayerPos * cameraPathLength;
	
	if (cameraPathPlayerPos < 0) {
		cameraPathPlayerPos = cameraPathPlayerPos * -1;
	}
	
	path_spline(entCameraPathEntity, camera.x, cameraPathPlayerPos);
	
	// Look at the player
	vec_set(vecCameraTemp.x, player.x);
	vec_sub(vecCameraTemp.x, camera.x);
	vec_to_angle(camera.pan, vecCameraTemp);
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
	
	if (my.skill2 == 1) {
		cameraInit();
	}
	
	if ((my.skill2 == 0) || (my.skill2 > 1)) {
		vec_set(camera.x, my.x);
	}
	
	activeCameraType = my.skill2;
}

#endif