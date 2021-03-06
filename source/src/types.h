#ifndef TYPES_H
#define TYPES_H

// Entity Skills
#define DOUBLE_CLICK_TIME skill89 // Saves ticks to last click to simulate a double click
#define target_x skill90
#define target_y skill91
#define target_z skill92
#define force_stop skill93
#define ent_smartwalk skill98
#define SUB_TYPE skill99
#define ENTITY_TYPE skill100

// Types
#define TYPE_NONE 0
#define TYPE_ITEM 1
#define TYPE_DYNAMIC_CAMERA 2
#define TYPE_LEVEL_GATE 3
#define TYPE_POINTEREST 4 // (point of interest)
#define TYPE_ITEM_SPAWNPOINT 5
#define TYPE_EVENT_TRIGGER 6

#define TYPE_ITEM_DEFAULT 0
#define TYPE_ITEM_GRAB 1
#define TYPE_ITEM_LOOK 2
#define TYPE_ITEM_POINT 3
#define TYPE_ITEM_EXIT 4
#define TYPE_ITEM_TALK 5
#define TYPE_ITEM_USE 6
#define TYPE_ITEM_SEARCH 7

// Groups
#define GROUP_CURSOR_HELPER 10
#define GROUP_ITEM 11

// Item Ids
#define ITEM_ID_VISITENKARTE 1
#define ITEM_ID_REVOLVER 2
#define ITEM_ID_GECKOFOOD 6
#define ITEM_ID_LAPTOP 10
#define ITEM_ID_FLASHLIGHT 16
#define ITEM_ID_MUENZEN 40
#define ITEM_ID_GELDBOERSE 31
#define ITEM_ID_SUGARCANEPOT 35
#define ITEM_ID_SUGARPOT 36
#define ITEM_ID_HERD 37
#define ITEM_ID_FAKEGECKOFOOD 54
#define ITEM_ID_USABLEHERD 55
#define ITEM_ID_EMAILLAPTOP 66
#define ITEM_ID_NEWSPAPER 67

// Macros
#define pSMARTWALK(pointer) ((SMARTWALK*)pointer)

// Global Variables

var mouse_left_off = 0;
var input_fetch = 1;
var flashlight_range = 1200;

VECTOR flashlight_start,flashlight_end;

#endif