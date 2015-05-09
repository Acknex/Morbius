#ifndef TYPES_H
#define TYPES_H

// Entity Skills
#define target_x skill90
#define target_y skill91
#define target_z skill92
#define ent_smartwalk skill98
#define ENTITY_TYPE skill100

// Types
#define TYPE_NONE 0
#define TYPE_ITEM 1
#define TYPE_DYNAMIC_CAMERA 2
#define TYPE_LEVEL_GATE 3

// Macros
#define pSMARTWALK(pointer) ((SMARTWALK*)pointer)

// Global Variables

var mouse_left_off = 0;
var input_fetch = 1;

#endif