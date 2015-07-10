#ifndef EVENT_H
#define EVENT_H

void EVENT_trigger(int triggerId);
void EVENT_stop();
var EVENT_isLocked();
void EVENT_reset();

#include "event.c"

#endif