#include "soundmgr.h"
#include "dialogs.h"

#define MUSICMGR_VOLUME 100
#define MUSICMGR_SILENTFAC 0.3
#define MUSICMGR_FADESPEED 15

TEXT* MUSICMGR__music = 
{
	string("Dungeon.OGG", "Dungeon.OGG", "relaxed.ogg", "Sumik_dj_-_wigi.ogg", "", "theGreekWoman.ogg", "");
}

var MUSICMGR__volume = MUSICMGR_VOLUME;
var MUSICMGR__fadeVolume = MUSICMGR_VOLUME;
var MUSICMGR__currentId = -1;
var MUSICMGR__handle = 0;

void MUSICMGR_setVolume(var vol)
{
	MUSICMGR__volume = clamp(vol, 0, 100);
}

void MUSICMGR_play(var id)
{
	if ((id < 0) || (id >= MUSICMGR__music->strings) || (id == MUSICMGR__currentId))
	{
error("id invalid or not changed");
		return;
	}
	
	//don't stop and start if song hasn't changed
	if (MUSICMGR__currentId != -1)
	{
		if (str_cmpi((MUSICMGR__music->pstring)[id], (MUSICMGR__music->pstring)[MUSICMGR__currentId]))
		{
error("song not changed not changed");
			MUSICMGR__currentId = id;
			return;
		}
	}
	
	MUSICMGR__currentId = id;

	var progress = MUSICMGR__volume;
	while(progress > 0)
	{
		progress = maxv(0, progress - MUSICMGR_FADESPEED * time_step);
		if (media_playing(MUSICMGR__handle))
		{
			media_tune(MUSICMGR__handle, progress, 0, 0);
		}
		wait(1);
	}
	if (media_playing(MUSICMGR__handle))
	{
		media_stop(MUSICMGR__handle);
		MUSICMGR__handle = 0;
	}

	//most likely no music file defined
	if (str_len((MUSICMGR__music->pstring)[id]) < 4)
	{
error("no new music file");
		return;
	}
	STRING* song = "#64";
	str_cpy (song, "media\\");
	str_cat (song, (MUSICMGR__music->pstring)[id]);
	MUSICMGR__handle = media_loop(song, NULL, 0);
	progress = 0;

	while(progress < MUSICMGR__volume)
	{
		progress = minv(MUSICMGR__volume, progress + MUSICMGR_FADESPEED * time_step);
		if (media_playing(MUSICMGR__handle))
		{
			media_tune(MUSICMGR__handle, progress, 0, 0);
		}
		wait(1);
	}	
	ptr_remove(song);
}

void MUSICMGR_stop()
{
	var progress = MUSICMGR__volume;
	while(progress > 0)
	{
		progress = maxv(0, progress - MUSICMGR_FADESPEED * time_step);
		if (media_playing(MUSICMGR__handle))
		{
			media_tune(MUSICMGR__handle, progress, 0, 0);
		}
		wait(1);
	}
	if (media_playing(MUSICMGR__handle))
	{
		media_stop(MUSICMGR__handle);
	}	
}

void MUSICMGR_fadeIn()
{
	while(MUSICMGR__fadeVolume < MUSICMGR__volume)
	{
		MUSICMGR__fadeVolume = minv(MUSICMGR__volume, MUSICMGR__fadeVolume + MUSICMGR_FADESPEED * time_step);
		if (media_playing(MUSICMGR__handle))
		{
			media_tune(MUSICMGR__handle, MUSICMGR__fadeVolume, 0, 0);
		}
		wait(1);
	}	
}

void MUSICMGR_fadeOut()
{
	MUSICMGR__fadeVolume = MUSICMGR__volume;
	while(MUSICMGR__fadeVolume > 10)
	{
		MUSICMGR__fadeVolume = maxv(0, MUSICMGR__fadeVolume - MUSICMGR_FADESPEED * time_step);
		if (media_playing(MUSICMGR__handle))
		{
			media_tune(MUSICMGR__handle, MUSICMGR__fadeVolume, 0, 0);
		}
		wait(1);
	}
}

void MUSICMGR__startup()
{
	var tuned = 0;
	while(1)
	{
		if (media_playing(MUSICMGR__handle))
		{
			if (SOUNDMGR_isActive() || dlgIsDialogActive())
			{
				if (tuned == 0)
				{
					tuned = 1;	
					media_tune(MUSICMGR__handle, MUSICMGR__volume * MUSICMGR_SILENTFAC, 0, 0);
				}
			}
			else
			{
				if (tuned == 1)
				{
					tuned = 0;
					media_tune(MUSICMGR__handle, MUSICMGR__volume, 0, 0);
				}
			}
		}
		wait(1);
	}
}

