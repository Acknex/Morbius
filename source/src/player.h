#ifndef PLAYER_H
#define PLAYER_H

int player_may_walk = 1;

action player_act();
ENTITY* Player_getLastClickedEnt();

#include "player.c"
#endif
