#include <acknex.h>

#define PRAGMA_PATH "src";

#include "src/morbius.h"
#include "src/system.h"
#include "src/game.h"

function main()
{
	video_set(1280, 720, 0, 2);
	video_window(NULL, NULL, 0, "Morbius");
	
	sys_init();
	
	game_loop();
}