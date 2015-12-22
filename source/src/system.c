#ifndef _SYSTEM_C_
#define _SYSTEM_C_

#include "inventory.h"
#include "camera.h"
#include "items.h"
#include "combine.h"
#include "mousemgr.h"
#include "hud.h"
#include "dialogs.h"
#include "chapter.h"
#include "musicmgr.h"
#include "kingmorph.h"
#include "event.h"
#include "menu.h"

void smartwalk_init();
void level_change(var level_id, var gate_id);
var is_level_loaded();
void openMenu();

void startGame()
{
	me = NULL;
	proc_mode = PROC_GLOBAL;
	ITEM_resetProgress();
	EVENT_reset();
	//level_change(0,-1); //ruin
	//level_change(2,-1); //alley
	level_change(4,-1); //office
	//level_change(5,-1); //greek_office
	//level_change(3,-1); //kingmorph
	//level_change(6,-1); //final_office
	while(!is_level_loaded()) wait(1);
	if (inventory == NULL) //create new inventory
	{
		inventory = inv_create(NULL, screen_size.x, 80);
	}
	else //or cleanup existing
	{
		inventory.itr = inventory.head;
		while(inventory.itr != NULL)
		{
			Item* searchItem = (Item*)inventory.itr.panel.skill_x;
			inv_remove_item(inventory, searchItem); 
			inv_increate_iterator(inventory);
		}
	}
	inv_show(inventory);
	var vScale = screen_size.y / 1200;	
	inv_set_pos(inventory, 0, screen_size.y - bmap_height(inventory.panel.bmap) * vScale);	
	
	on_esc = openMenu;
}

void exitGame()
{
//error("cleanup on exit");
	CHAPTER_close();
	COMBINATION_close();
	ITEM_close();
	HUD_close();
	MUSICMGR_close();
}

void quitGame()
{
	sys_exit(NULL);
}

void quitToMainMenu()
{
	openMenu();

	if (!is_level_loaded()) return;
	if (EVENT_isLocked()) return; 

	on_esc = NULL;
	inv_hide(inventory);
	MUSICMGR_close();
	KINGMORPH_exit(); //elegant solution with on_ent_remove fucks up somewhere, so use crowbar method
	menu_open();
	/*todo
	- fix camera intro sequence in office. broken after first run. no idea why
	*/
}

var inGameMenuActive = 0;
TEXT* inGameMenuText = 
{ 
	layer = 100;
	string("Quit to Menu [y/n]"); 
	FLAGS = CENTER_X | CENTER_Y | LIGHT | SHADOW | FILTER | TRANSLUCENT;
	red = 255;
	green = 255;
	blue = 255;	
}

void openMenu()
{
	if (!inGameMenuActive)
	{
		//don't cancel ongoing routines
		if (!is_level_loaded()) return;
		if (EVENT_isLocked()) return; 
		
		FONT* fnt = HUD_getFont();
		if (fnt == NULL) return;
		
		inv_hide(inventory);
		player_may_walk = 0;
		on_n = openMenu;
		on_y = quitToMainMenu;
		on_z = quitToMainMenu;
		on_j = quitToMainMenu;
		inGameMenuActive = 1;
		set(inGameMenuText, SHOW);
		//clumsy solution
		while(inGameMenuActive)
		{
			inGameMenuText->font = fnt;
			inGameMenuText->pos_x = screen_size.x * 0.5;
			inGameMenuText->pos_y = screen_size.y * 0.5;
			inGameMenuText->alpha = 60 + 40 * abs(sinv(total_ticks * 7));
			wait(1);
		}
	}
	else
	{
		inv_show(inventory);
		player_may_walk = 1;
		on_n = NULL;
		on_y = NULL;
		on_z = NULL;
		on_j = NULL;
		inGameMenuActive = 0;
		reset(inGameMenuText, SHOW);
	}	
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
	video_window(NULL, NULL, 0, "Morbius - The Utopia Conspiracy");
	
	random_seed((sys_seconds % sys_month) * sys_hours - 42);
	
	HUD_init(); //any GUI related init should be called afterwards
	smartwalk_init();
	mousemgr_init();
	dlgInit();
	ITEM_load("xml\\items.xml"); // items.h
	COMBINATION_load("xml\\combinations.xml"); // combine.h
	CHAPTER_load("xml\\chapters.xml"); // chapter.h

	menuConfig.startGame = startGame;
	menuConfig.quitGame = quitGame;
	menuConfig.startCredits = credits_start;
	
	creditsConfig.ended = menu_open;

	on_close = quitGame;
	on_exit = exitGame;
//	on_esc = quitGame;
}

#endif