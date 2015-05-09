#include <acknex.h>

FONT* HUD__itemFnt = "Arial#20b";
TEXT* HUD_ItemTxt = 
{
	font = HUD__itemFnt;
	layer = 1;	
	pos_x = 0;
	pos_y = 0;
	flags |= CENTER_X, CENTER_Y;
	string ("");
}

TEXT* HUD_DescriptionTxt = 
{
	font = HUD__itemFnt;
	layer = 1;	
	pos_x = 0;
	pos_y = 0;
	flags |= SHOW;
	string ("");
}


TEXT* HUD_getItemText()
{
	return HUD_ItemTxt;
}

void HUD_showDescription(STRING* description)
{
	proc_kill(HUD_showDescription);
	str_cpy((HUD_DescriptionTxt->pstring)[0], description);
	set(HUD_DescriptionTxt, SHOW);
	wait(-5);
	reset(HUD_DescriptionTxt, SHOW);
}