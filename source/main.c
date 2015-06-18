#include <acknex.h>

#define PRAGMA_PATH "src"
#define PRAGMA_PATH "shaders"
#define PRAGMA_LEVEL "level\\alley.wmp"
#define PRAGMA_LEVEL "level\\ruin.wmp"
#define PRAGMA_LEVEL "level\\ruin_indoor.wmp"
#define PRAGMA_LEVEL "level\\office.wmp"
#define PRAGMA_LEVEL "level\\greek_office.wmp"
#define PRAGMA_LEVEL "level\\kingmorph.wmp"
#define PRAGMA_LEVEL "level\\kingmorph_menu.wmp"
#define PRAGMA_LEVEL "level\\office_credits.wmp"
#define PRAGMA_BIND "samples.wrs"

#include "bindings.h"
#include "morbius.h"
#include "mousemgr.h"
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
#include "office.h"

#define DEBUG
#ifdef DEBUG
	#include <default.c>
#endif

void main() {
	d3d_antialias = 9;
	sys_init(); // system.c
	wait(1);
	menu_open(); // menu.c
//startGame();
}
