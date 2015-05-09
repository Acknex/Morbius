#ifndef SMARTWALK_H
#define SMARTWALK_H

struct _SMARTWALKDATA
{
	int num_regions;
	int *neighbors;
	int *path_prev;
	int *path_stack;
	var *path_dist;
	VECTOR* regionMin;
	VECTOR* regionMax;
	VECTOR* center;
	VECTOR* edgeMin;
	VECTOR* edgeMax;
};

struct _SMARTWALK
{
	int current_node;
	int num_nodes;
	int direct;
	VECTOR* nodes;
};

typedef struct _SMARTWALKDATA SMARTWALKDATA;
typedef struct _SMARTWALK SMARTWALK;

SMARTWALKDATA *smd_level;

//#define ent_smartwalk skill84
//#define pSMARTWALK(pointer) ((SMARTWALK*)pointer)

///////////////////////////////

/* Create a smartwalk data container for subsequent smartwalk calls. */
SMARTWALKDATA* smartwalkdata_create();

/* Destroy a smartwalkdata object. */
void smartwalkdata_destroy(SMARTWALKDATA* data);

SMARTWALK* smartwalk_create();

/* Call this function (if (ent_smartwalk != 0)) before entity removal or target has been found. */
void smartwalk_destroy(SMARTWALK* walk);

/* Call after level_load(), to be used with a level specific SMARTWALKDATA object. */
void smartwalkdata_fill(SMARTWALKDATA* data);

/* Find the region closest to vpos. */
var smartwalkdata_find_region(SMARTWALKDATA* data, VECTOR* vpos, var modify);

void line_projection(VECTOR* vresult, VECTOR* vpoint, VECTOR* vstart, VECTOR* vtarget);

/* Find a path from vstart to vtarget and save it in a SMARTWALK walk. */
SMARTWALK* smartwalk_create_path(SMARTWALKDATA* data, VECTOR* vstart, VECTOR* vtarget);

var vec_limit(VECTOR* vec, var range);

void draw_line3dfull(VECTOR* vfrom, VECTOR* vto, COLOR* color, var alpha);

void smartwalk_draw(SMARTWALK* walk, COLOR* color, var alpha);

////////////////////////////////////////////
#include "smartwalk.c"
#endif 
