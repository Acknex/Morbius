#include <acknex.h>
#include <default.c>

#define PRAGMA_PATH "..//src";

#include "camera.h"

#include "camera.c"

// Defines für die Bewegung des Spielers
#define WALK_SPEED 2
#define RUN_SPEED 4
#define MOUSE_SENSITIVITY 20
#define PLAYER_HEIGHT 6


// Funktion zum Bewegen des Spielers
function movePlayer() {
	player = me;
	int nTraceDown = 0;
	VECTOR vecPlayerMoveSpeed;
	while(1) {
		nTraceDown = c_trace(my.x, vector(my.x,my.y,my.z-500), IGNORE_ME|IGNORE_PASSENTS|IGNORE_PASSABLE|IGNORE_SPRITES|USE_BOX);
		vecPlayerMoveSpeed.z = -(nTraceDown - PLAYER_HEIGHT);
		vecPlayerMoveSpeed.x = (key_w - key_s) * (WALK_SPEED+key_shiftl*RUN_SPEED) * time_step;
		vecPlayerMoveSpeed.y = 0;
		my.pan += (key_a - key_d) * time_step * 10;
		c_move(me, vecPlayerMoveSpeed.x, nullvector, IGNORE_PASSABLE | IGNORE_PASSENTS | GLIDE);
		wait(1);
	}
}

// Aktion für den Spieler
action actPlayer() {
	
	player = me;
	
	// Aktualisieren der Kollisionshülle
	c_setminmax(player);
	wait(1);
	vec_set(player.min_x,vector(-25,-25,-35)); 
	vec_set(player.max_x,vector(25,25,55));
	
	// Bewegen der Kamera und der Entity
	movePlayer();
	cameraInit();
	
	activeCameraType = CAMERA_TYPE_MULTIPLE_FOLLOW;
}

void main() {
	level_load("..//maps//camera_test.wmb");
	
	while(1) {
		cameraLoop();
		
		if (key_1) {
			while(key_1) wait(1);
			activeCameraType = CAMERA_TYPE_FIXED_FOLLOW;
		}
		
		if (key_2) {
			while(key_2) wait(1);
			activeCameraType = CAMERA_TYPE_SPLINE;
		}
		
		if (key_3) {
			while(key_3) wait(1);
			activeCameraType = CAMERA_TYPE_AXIS;
		}
		
		if (key_4) {
			while(key_4) wait(1);
			activeCameraType = CAMERA_TYPE_MULTIPLE_FOLLOW;
		}
		
		wait(1);
	}
}