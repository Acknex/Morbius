#include <acknex.h>
#include <default.c>

#define PRAGMA_PATH "models"
#define PRAGMA_PATH "textures"

//#define MENU_DEBUG

#include "src\\menu.h"

void test_close()
{
	level_load(NULL);
}

// Stub for non-animated models in level
void demo() { }

void quit()
{
	sys_exit(NULL);
}

function main()
{
	fps_max = 60;
	mouse_mode = 3;
	video_set(1280, 720, 0, 2);
	video_window(NULL, NULL, 0, "Morbius");
	on_q = test_close;
	
	wait(1);
	
	menuConfig.quitGame = quit;
	menuConfig.startGame = test_close;
	menu_open();
}