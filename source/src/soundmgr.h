#ifndef SOUNDMGR_H
#define SOUNDMGR_H

void SOUNDMGR_setVolume(var vol);
void SOUNDMGR_playAtOnce(SOUND* snd);
void SOUNDMGR_scheduleSound(SOUND* snd);
var SOUNDMGR_isPlaying(SOUND* snd);
void SOUNDMGR_stop();

#include "soundmgr.c"

#endif