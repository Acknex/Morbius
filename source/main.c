#include <acknex.h>

#define PRAGMA_PATH "graphics\\cursors";
#define PRAGMA_PATH "graphics\\items";
#define PRAGMA_PATH "src";
#define PRAGMA_PATH "models";
#define PRAGMA_PATH "textures";

#include "morbius.h"
#include "menu.h"
#include "credits.h"
#include "system.h"
#include "types.h"
#include "smartwalk.h"
#include "game.h"
#include "materials.h"
#include "level_transition.h"
#include "player.h"
#include "inventory.h"
#include "camera.h"
#include "alley.c"

#define DEBUG
#ifdef DEBUG
#include <default.c>
#endif

void main() {
	sys_init(); // system.c
	menu_open(); // menu.c
}
