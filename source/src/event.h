#ifndef EVENT_H
#define EVENT_H

void EVENT_trigger(int triggerId);
void EVENT_stop();
var EVENT_isLocked();

#include "event.c"

#endif