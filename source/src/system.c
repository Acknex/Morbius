#ifndef _SYSTEM_C_
#define _SYSTEM_C_

#include "inventory.h"
#include "camera.h"
#include "items.h"
#include "combine.h"
#include "mousemgr.h"
#include "hud.h"
#include "dialogs.h"

void smartwalk_init();
void level_change(var level_id, var gate_id);
var is_level_loaded();

void startGame()
{
	me = NULL;
	proc_mode = PROC_GLOBAL;
	ITEM_load("xml\\items.xml"); // items.h
	COMBINATION_load("xml\\items.xml"); // combine.h
	level_change(0,-1);
	while(!is_level_loaded()) wait(1);
	inventory = inv_create(NULL, screen_size.x, 80);
	inv_show(inventory);
	inv_set_pos(inventory, 0, screen_size.y - bmap_height(inventory.panel.bmap));
}

void exitGame()
{
//error("cleanup on exit");
	COMBINATION_close();
	ITEM_close();
	HUD_close();
}

void quitGame()
{
	sys_exit(NULL);
}

void sys_init() {
	
	// rendering
	fps_min = 30;
	fps_max = 60;
	mip_flat = 2;
	//d3d_antialias = 4;
	shadow_stencil = 0; // activate external shadows
	
	// window + system
	preload_mode = 7;
	mouse_pointer = 0;
	mouse_mode = 4;
	mouse_range = 10000;
	
	double resFac = 0.85; //%
	long resX = (double) sys_metrics(SM_CXSCREEN) * resFac;
	long resY = (double) sys_metrics(SM_CYSCREEN) * resFac;
	
	video_set(resX, resY, 0, 2);
	video_window(NULL, NULL, 0, "Morbius - Die Utopia-Verschwörung");
	
	random_seed((sys_seconds % sys_month) * sys_hours - 42);
	
	HUD_init(); //any GUI related init should be called afterwards
	smartwalk_init();
	mousemgr_init();
	dlgInit();
	
	menuConfig.startGame = startGame;
	menuConfig.quitGame = quitGame;
	menuConfig.startCredits = credits_start;
	
	creditsConfig.ended = menu_open;

	on_close = quitGame;
	on_exit = exitGame;
}

#endif