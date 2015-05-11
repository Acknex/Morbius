#ifndef MATERIALS_H
#define MATERIALS_H
///////////////////////////////////////////
// Materials

MATERIAL* mat_cloud =
{
	effect = "cloud.fx";
	flags = AUTORELOAD;
}

MATERIAL* mat_character =
{
	effect = "character.fx";
	flags = AUTORELOAD;
}

MATERIAL* mat_item =
{
	effect = "item.fx";
	flags = AUTORELOAD;
}

MATERIAL* mat_level_change_transition =
{
	effect = "level_change_t.fx";
	flags = AUTORELOAD;
}

///////////////////////////////////////////
// Views for postprocessing

VIEW* pp_level_change_transition =
{
	flags = PROCESS_TARGET;
	material = mat_level_change_transition;
}

///////////////////////////////////////////
//
#endif