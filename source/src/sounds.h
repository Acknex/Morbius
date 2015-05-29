#ifndef SOUNDS_H
#define SOUNDS_H

var vMusicHandle = 0;
var vMusicVolume = 10;
var vSoundVolume = 10;
var vSpeechVolume = 70;//20;

void snPlayMusic(char* _file, int _loop);
void snPlayMusic(SOUND* _sound, int _loop);
void snPauseMusic();
void snStopMusic();
int snIsMusicPlayed();
void snSetMusicVolume(int _vol);
void levelRegionMusic();

#include "sounds.c"
#endif /*SOUNDS_H*/