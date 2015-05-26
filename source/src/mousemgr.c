#include "mousemgr.h"

BMAP *mousemgrMapDefault = "cur_point.png";
BMAP *mousemgrMapUse = "cur_use.png";
BMAP *mousemgrMapLook = "cur_look.png";
BMAP *mousemgrMapTalk = "cur_talk.png";
BMAP *mousemgrMapExit = "cur_exit.png";
BMAP *mousemgrMapGrab = "cursor_grab.tga";

var mousemgrOffsets[] = {
	 0,  0, // Default
	22, 22, // Use
	14, 16, // Look
	41, 37, // Talk,
	20, 17, // Exit
	22, 22  // Grab
};

int mousemgr_cursor = MOUSE_DEFAULT;

BMAP *mousemgr_decoration = NULL;
STRING *mousemgr_hint = "#512";

void mousemgr_set(int cursor, BMAP *decoration)
{
	mousemgr_cursor = cursor % 6;
	mousemgr_decoration = decoration;
}

void mousemgr_hint(STRING *text)
{
	if(text == NULL)
		str_cpy(mousemgr_hint, "");
	else
		str_cpy(mousemgr_hint, text);
}

int mousemgr_currentCursor()
{
	return mousemgr_cursor;
}

BMAP *mousemgr_currentDecoration()
{
	return mousemgr_decoration;
}

TEXT *mousemgrText = 
{
	font = mousemgrHintFont;
	string = (mousemgr_hint);
	red = 255;
	green = 255;
	blue = 255;
	flags = LIGHT | OUTLINE;
}

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
		
		if(mousemgr_decoration != NULL)
		{
			draw_quad(
				mousemgr_decoration,
				vector(x + map->height, y + 0.5 * map->height, 0),
				NULL,
				NULL, // Full size
				NULL,
				NULL,
				100,
				0);
		}
		if(str_len(mousemgr_hint) > 0)
		{
			mousemgrText.pos_x = minv(x + map->height, screen_size.x - str_width(mousemgr_hint, mousemgrHintFont));
			mousemgrText.pos_y = y - 0.5 * mousemgrHintFont->dy;
			draw_obj(mousemgrText);
		}
		currentFrame += animationSpeed * time_step;
		
		wait(1);
	}
}