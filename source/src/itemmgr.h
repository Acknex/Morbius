#ifndef ITEMMGR_H
#define ITEMMGR_H

void interactionItem_spawn(int id);
void interactionItem_spawn(int id, VECTOR* position, VECTOR* angle, var border);
var interactionItem_isNearPlayer(ENTITY* ent);

#include "itemmgr.c"

#endif