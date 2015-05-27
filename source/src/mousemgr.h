#ifndef _MOUSEMGR_H_
#define _MOUSEMGR_H_

#define MOUSE_DEFAULT	0
#define MOUSE_USE		1
#define MOUSE_LOOK		2
#define MOUSE_TALK		3
#define MOUSE_EXIT		4
#define MOUSE_GRAB		5

/**
 * @brief The font used for displaying the hint.
 */
FONT *mousemgrHintFont = "Arial#32";

/**
 * @brief Starts the mouse manager.
 */
void mousemgr_init();

/**
 * @brief Sets the moues cursor.
 * @param cursor The ID of the mouse cursor.
 * @param decoration An image shown bottom right of the mouse cursor.
 */
void mousemgr_set(int cursor, BMAP *decoration);

/**
 * @brief Sets a text hint at the mouse cursor.
 * @param text The text to be shown. To hide the text, use a blank string.
 */
void mousemgr_hint(STRING *text);

/**
 * @brief Gets the current cursor.
 */
int mousemgr_currentCursor();

/**
 * @brief Gets the current mouse decoration.
 */
BMAP *mousemgr_currentDecoration();

void mousemgr_resize();

#include "mousemgr.c"

#endif // #ifndef _MOUSEMGR_H_