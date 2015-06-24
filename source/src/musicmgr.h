#ifndef MUSICMGR_H
#define MUSICMGR_H

void MUSICMGR_setVolume(var vol);
void MUSICMGR_play(var id);
void MUSICMGR_stop();
void MUSICMGR_close();
void MUSICMGR_fadeIn();
void MUSICMGR_fadeOut(var vol);

#include "musicmgr.c"
#endif