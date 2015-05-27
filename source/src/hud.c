#include <windows.h>
#include "soundmgr.h"

#define HUD_FADESPEED 10
#define HUD_MINDISPLAYTIME 80
#define HUD_FONT_SIZE 40

FONT* HUD__font = "Boogaloo#30";
SOUND* HUD__managedSound = NULL;
void HUD__resizeEv();

TEXT* HUD_DescriptionTxt = 
{
	font = HUD__font;
	layer = 1;	
	pos_x = 0;
	pos_y = 0;
	flags |= TRANSLUCENT | WWRAP | SHADOW | CENTER_X;
	alpha = 0;
	string ("");
}



var HUD_descriptionTimer = 0;

void HUD_showDescription(STRING* description)
{
	str_cpy((HUD_DescriptionTxt->pstring)[0], description);
	set(HUD_DescriptionTxt, SHOW);
	HUD_descriptionTimer = HUD_MINDISPLAYTIME;
	HUD__managedSound = NULL;
}

void HUD_showDescription(STRING* description, SOUND* snd)
{
	HUD_showDescription(description);
	HUD__managedSound = snd;
}

void HUD_showDescription(STRING* description)
{
	str_cpy((HUD_DescriptionTxt->pstring)[0], description);
	set(HUD_DescriptionTxt, SHOW);
	HUD_descriptionTimer = HUD_MINDISPLAYTIME;
}

void HUD_hideDescription()
{
	HUD_descriptionTimer = 0;
	HUD__managedSound = NULL;
}

void HUD_init()
{
	HUD__resizeEv = on_resize;
	on_resize = HUD_resize;
	AddFontResource("media//Boogaloo-Regular.otf");
	HUD_resize();
}

void HUD_close()
{
	RemoveFontResource("media//Boogaloo-Regular.otf");
}


void HUD_resize()
{
	if (HUD__resizeEv != NULL)
	{
		HUD__resizeEv();
	}
	var scale = screen_size.y / 1200;
	STRING* strTemp = "#64";	
	str_printf(strTemp, "Boogaloo#%i", (int)(HUD_FONT_SIZE * scale));
	ptr_remove(HUD__font);
	HUD__font = font_create(strTemp);

	HUD_DescriptionTxt->font = HUD__font;
	HUD_DescriptionTxt->size_x = screen_size.x;
	HUD_DescriptionTxt->pos_x = screen_size.x * 0.5;
	HUD_DescriptionTxt->pos_y = screen_size.y * 0.05;
}

FONT* HUD_getFont()
{
	return HUD__font;
}

void HUD_fader_startup()
{
	var fadeAlpha = 0;
	while(1)
	{
		HUD_descriptionTimer = maxv(HUD_descriptionTimer - time_step, 0);
		if (HUD_descriptionTimer == 0 && SOUNDMGR_isPlaying(HUD__managedSound) == 0)
		{
			fadeAlpha = maxv(fadeAlpha - (HUD_FADESPEED * time_step), 0);
			if (fadeAlpha == 0)
			{
				reset(HUD_DescriptionTxt, SHOW);
				HUD__managedSound = NULL;
			}
			else
			{
				HUD_DescriptionTxt->alpha = fadeAlpha;
			}
		}
		else
		{
			fadeAlpha = minv(fadeAlpha + (HUD_FADESPEED * time_step), 100);
			HUD_DescriptionTxt->alpha = fadeAlpha;
		}
		wait (1);
		//DEBUG_VAR(HUD_descriptionTimer,30);
		//DEBUG_VAR(fadeAlpha,60);
	}
}
