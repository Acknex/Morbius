#include <acknex.h>

FONT* HUD__itemFnt = "Arial#20b";

TEXT* HUD_DescriptionTxt = 
{
	font = HUD__itemFnt;
	layer = 1;	
	pos_x = 0;
	pos_y = 0;
	flags |= SHOW;
	string ("");
}


void HUD_hideDescription();

var HUD_descriptionTimer;
void HUD_showDescription(STRING* description)
{
	str_cpy((HUD_DescriptionTxt->pstring)[0], description);
	set(HUD_DescriptionTxt, SHOW);

//clumsy
	HUD_descriptionTimer = 5* 16;
	HUD_hideDescription();
}

void HUD_hideDescription()
{
	while(HUD_descriptionTimer > 0)
	{
		HUD_descriptionTimer -= time_step;
		wait(1);
	}
	reset(HUD_DescriptionTxt, SHOW);
}