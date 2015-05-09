#include <acknex.h>

#define PRAGMA_PATH "src";
#define PRAGMA_PATH "models";
#define PRAGMA_PATH "textures";

#include "morbius.h"
#include "system.h"
#include "game.h"
#include "menu.h"

#ifdef DEBUG
#include <default.c>
#endif



void main()
{
	sys_init();
	menu_open();
}