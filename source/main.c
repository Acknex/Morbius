#include <acknex.h>

#define PRAGMA_PATH "graphics\\cursors";
#define PRAGMA_PATH "src";
#define PRAGMA_PATH "models";
#define PRAGMA_PATH "textures";

#include "morbius.h"
#include "menu.h"
#include "system.h"
#include "types.h"
#include "smartwalk.h"
#include "game.h"
#include "materials.h"
#include "level_transition.h"
#include "player.h"

#ifdef DEBUG
#include <default.c>
#endif

void main() {
	sys_init();
	menu_open();
}