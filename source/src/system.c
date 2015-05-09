#ifndef _SYSTEM_C_
#define _SYSTEM_C_

#include "inventory.h"

void itemmgr_init();
void smartwalk_init();
void level_change(var level_id, var gate_id);

void startGame()
{
	level_change(0,-1);
	inv_show(inventory);
}

void sys_init() {
	
	// rendering
	fps_max = 60;
	mip_flat = 2;
	d3d_antialias = 4;
	shadow_stencil = 0; // activate external shadows
	
	// window + system
	preload_mode = 7;
	// mouse_pointer = 0;
	mouse_mode = 4;
	mouse_range = 10000;
	
	double resFac = 0.85; //%
	long resX = (double) sys_metrics(SM_CXSCREEN) * resFac;
	long resY = (double) sys_metrics(SM_CYSCREEN) * resFac;
	
	video_set(resX, resY, 0, 2);
	video_window(NULL, NULL, 0, "Morbius - Die Utopia-Verschwörung");
	
	random_seed((sys_seconds % sys_month) * sys_hours - 42);
	
	Inventory* inventory = inv_create(NULL);
	itemmgr_init();
	smartwalk_init();
	menuConfig.startGame = startGame;
}

#endif