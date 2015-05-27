#ifndef HUD_H
#define HUD_H

void HUD_showDescription(STRING* description);
void HUD_showDescription(STRING* description, SOUND* snd);
void HUD_hideDescription();
void HUD_init();
void HUD_close();
void HUD_resize();
FONT* HUD_getFont();

#include "hud.c"

#endif
