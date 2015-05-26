#ifndef _MOUSEMGR_H_
#define _MOUSEMGR_H_

#define MOUSE_DEFAULT	0
#define MOUSE_USE		1
#define MOUSE_LOOK		2
#define MOUSE_TALK		3
#define MOUSE_EXIT		4
#define MOUSE_GRAB		5

/**
 * @brief The current cursor icon of the mouse.
 */ 
int mousemgr_cursor = MOUSE_DEFAULT;

/**
 * @brief Cursor hint that is shown bottom right of the cursor
 */
BMAP *mousemgr_hint = NULL;

/**
 * @brief Starts the mouse manager.
 */
void mousemgr_init();

#include "mousemgr.c"

#endif // #ifndef _MOUSEMGR_H_