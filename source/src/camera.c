#ifndef DIALOGS_C
#define DIALOGS_C

void cameraLoop() {
	switch(activeCameraType) {
		case CAMERA_TYPE_FIXED_FOLLOW:
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
	cameraPathPlayerPos = cameraPathPlayerPos / ((vecSplineCamMax.x - vecSplineCamMin.x) + 0.0001); // Avoid 0 division
	cameraPathPlayerPos = cameraPathPlayerPos * cameraPathLength;
	
	path_spline(entCameraPathEntity,camera.x,cameraPathPlayerPos);
	
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

#endif