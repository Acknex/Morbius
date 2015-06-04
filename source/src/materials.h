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

BMAP* bmp_flashlight_mask = "flashlight_mask.tga";
MATERIAL* mat_flashlight =
{
	effect = "flashlight.fx";
	flags = AUTORELOAD;
	skin1 = bmp_flashlight_mask;
}

MATERIAL* mat_level_change_transition =
{
	effect = "level_change_t.fx";
	flags = AUTORELOAD;
}

MATERIAL* mat_coffee_can =
{
	effect = "coffee_can.fx";
	flags = AUTORELOAD;
}

MATERIAL* mat_mydefault =
{
	effect = "mydefault.fx";
	flags = AUTORELOAD;
}

MATERIAL* mat_flight_glow =
{
	effect = "flight_glow.fx";
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

action offmod_init()
{
	my.material = mat_flashlight; //mat_mydefault;
	//ent_mtlset(my,mat_flashlight,3);
	//ent_mtlset(my,mat_flashlight,4);
}

#endif