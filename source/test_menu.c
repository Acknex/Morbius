#include <acknex.h>
#include <default.c>

#define PRAGMA_PATH "models"
#define PRAGMA_PATH "textures"

#include "src\\menu.h"

void test_close()
{
	level_load(NULL);
}

void next() { menu_switch(1); }
void prev() { menu_switch(-1); }

void demo() { }

void fade()
{
	menu_fade_and_trigger(_menu_stops[_menu.currentStop]);
}

void quit()
{
	sys_exit(NULL);
}

function main()
{
	fps_max = 60;
	video_set(1280, 720, 0, 2);
	video_window(NULL, NULL, 0, "Morbius");
	on_q = test_close;
	on_e = fade;
	on_cur = prev;
	on_cul = next;
	
	menuConfig.quitGame = quit;
	menuConfig.startGame = test_close;
	menu_open();
}