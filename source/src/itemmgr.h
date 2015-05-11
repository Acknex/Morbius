#ifndef ITEMMGR_H
#define ITEMMGR_H

BMAP* bmp_cursor_array[8];

void itemmgr_init();
void interactionItem_morph(int target, int morph);
void interactionItem_spawn(int id, VECTOR* position);


#include "itemmgr.c"

#endif