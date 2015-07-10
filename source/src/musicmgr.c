#include "soundmgr.h"
#include "dialogs.h"

#define MUSICMGR_VOLUME 100
#define MUSICMGR_SILENTFAC 0.3
#define MUSICMGR_FADESPEED 10

TEXT* MUSICMGR__music = 
{
	string("media\\Dungeon.OGG", "media\\Dungeon.OGG", "media\\relaxed.ogg", "media\\Sumik_dj_-_wigi.ogg", "media\\Bar.OGG", "media\\theGreekWoman.ogg", "");
}

var MUSICMGR__volume = MUSICMGR_VOLUME;
var MUSICMGR__fadeVolume = MUSICMGR_VOLUME;
var MUSICMGR__targetVolume = MUSICMGR_VOLUME;
var MUSICMGR__currentVolume = MUSICMGR_VOLUME;
var MUSICMGR__currentId = -1;
var MUSICMGR__handle = 0;
var MUSICMGR__isStopping = 0;

void MUSICMGR_setVolume(var vol)
{
	MUSICMGR__volume = clamp(vol, 0, 100);
}

void MUSICMGR_play(var id)
{
	if ((id < 0) || (id >= MUSICMGR__music->strings) || (id == MUSICMGR__currentId))
	{
		return;
	}
	
	//don't stop and start if song hasn't changed
	if (MUSICMGR__currentId != -1)
	{
		if (str_cmpi((MUSICMGR__music->pstring)[id], (MUSICMGR__music->pstring)[MUSICMGR__currentId]))
		{
			MUSICMGR__currentId = id;
			return;
		}
	}
	
	MUSICMGR__currentId = id;
	MUSICMGR_stop();
	wait_for(MUSICMGR_stop);

	//most likely no music file defined
	if (str_len((MUSICMGR__music->pstring)[id]) < 4)
	{
		return;
	}
	MUSICMGR__currentVolume = 0;

	//WED build pretends to be clever, so it would find the files without path
	//even in non referenced folders... -.-
	/*STRING* song = "#64";
	str_cpy (song, "media\\");
	str_cat (song, (MUSICMGR__music->pstring)[id]);
	MUSICMGR__handle = media_loop(song, NULL, MUSICMGR__currentVolume);
	ptr_remove(song);*/
	MUSICMGR__handle = media_loop((MUSICMGR__music->pstring)[id], NULL, MUSICMGR__currentVolume);
}

void MUSICMGR_stop()
{
	var progress = MUSICMGR__currentVolume;
	MUSICMGR__isStopping = 1;
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
	MUSICMGR__isStopping = 0;	
}

void MUSICMGR_close()
{
	if (media_playing(MUSICMGR__handle))
	{
		media_stop(MUSICMGR__handle);
		MUSICMGR__currentId = -1;
	}	
}

void MUSICMGR_fadeIn()
{
	MUSICMGR__fadeVolume = MUSICMGR__volume;
}

void MUSICMGR_fadeOut(var vol)
{
	MUSICMGR__fadeVolume = clamp(vol, 0, 100);
}

void MUSICMGR__startup()
{
	var tuned = 0;
	var volume;
	while(1)
	{
		if (media_playing(MUSICMGR__handle) && !MUSICMGR__isStopping)
		{
			if (SOUNDMGR_isActive() || dlgIsDialogActive())
			{
				volume = MUSICMGR__volume * MUSICMGR_SILENTFAC;
			}
			else
			{
				volume = MUSICMGR__volume;
			}
			MUSICMGR__targetVolume = minv(MUSICMGR__fadeVolume, volume);

			if (MUSICMGR__currentVolume < MUSICMGR__targetVolume)
			{
				MUSICMGR__currentVolume = minv(MUSICMGR__targetVolume, MUSICMGR__currentVolume + MUSICMGR_FADESPEED * time_step);
				media_tune(MUSICMGR__handle, MUSICMGR__currentVolume, 0, 0);
			}
			
			if (MUSICMGR__currentVolume > MUSICMGR__targetVolume)
			{
				MUSICMGR__currentVolume = maxv(MUSICMGR__targetVolume, MUSICMGR__currentVolume - MUSICMGR_FADESPEED * time_step);
				media_tune(MUSICMGR__handle, MUSICMGR__currentVolume, 0, 0);
			}			
		}
		wait(1);
	}
}

