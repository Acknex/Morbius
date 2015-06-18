#ifndef ITEMMGR_H
#define ITEMMGR_H

void interactionItem_spawn(int id);
void interactionItem_spawn(int id, VECTOR* position, VECTOR* angle, var border);
void interactionItem_morph(int targetId, int morphId);
var interactionItem_isNearPlayer(ENTITY* ent);
void interactionItem_remoteStart();

#include "itemmgr.c"

#endif