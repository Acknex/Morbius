#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_NEAR_DIST 45
int player_may_walk = 1;

action player_act();
ENTITY* Player_getLastClickedEnt();
void Player_resetLastClickedEnt();

#include "player.c"
#endif
