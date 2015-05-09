#include "menu.h"

STRING *msDiscoMusic = "media\\Sumik_dj_-_wigi.mp3";

typedef struct {
    var music;


} MenuData;

MenuData _menu;

void menu_open()
{
    level_load(NULL);

    _menu.music = media_loop(msDiscoMusic, NULL, 100);
}
