#include "mousemgr.h"

BMAP *mousemgrMapDefault = "cur_point.png";
BMAP *mousemgrMapUse = "cur_use.png";
BMAP *mousemgrMapLook = "cur_look.png";
BMAP *mousemgrMapTalk = "cur_talk.png";
BMAP *mousemgrMapExit = "cur_exit.png";
BMAP *mousemgrMapGrab = "cur_use.png";

var mousemgrOffsets[] = {
	 0,  0, // Default
	22, 22, // Use
	14, 16, // Look
	41, 37, // Talk,
	20, 17, // Exit
	22, 22  // Grab
};

void mousemgr_init()
{
	BMAP *maps[6];
	maps[MOUSE_DEFAULT] = mousemgrMapDefault;
	maps[MOUSE_USE] = mousemgrMapUse;
	maps[MOUSE_LOOK] = mousemgrMapLook;
	maps[MOUSE_TALK] = mousemgrMapTalk;
	maps[MOUSE_EXIT] = mousemgrMapExit;
	maps[MOUSE_GRAB] = mousemgrMapGrab;
	
	proc_mode = PROC_LATE;
	
	var animationSpeed = 1; // 1 Frame/Tick
	var currentFrame = 0;
	while(1)
	{
		int id = mousemgr_cursor % 6;
		
		int x = mouse_pos.x - mousemgrOffsets[2 * id + 0];
		int y = mouse_pos.y - mousemgrOffsets[2 * id + 1];
		
		BMAP *map = maps[id];
		
		int numFrames = map->width / map->height;
		int frame = ((int)currentFrame) % numFrames;
		
		draw_quad(
			map,
			vector(x, y, 0),
			vector(frame * map->height, 0, 0),
			vector(map->height, map->height, 0),
			NULL,
			NULL,
			100,
			0);
		
		currentFrame += animationSpeed * time_step;
		
		wait(1);
	}
}