#include "menu.h"

STRING *msDiscoMusic = "media\\Sumik_dj_-_wigi.ogg";

typedef struct {
    var music;


} MenuData;

MenuData _menu;

void menu_open()
{
    level_load(NULL);
	wait(-1);
    _menu.music = media_loop(msDiscoMusic, NULL, 100);
}
