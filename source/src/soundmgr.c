#define SOUNDMGR_VOLUME 100
#define SOUNDMGR_FADESPEED 15

var SOUNDMGR__handle = NULL;
var SOUNDMGR__volume = SOUNDMGR_VOLUME;
var SOUNDMGR__doFade = 0;
var SOUNDMGR__mute = 0;
SOUND* SOUNDMGR__nextSound = NULL;
SOUND* SOUNDMGR__currentSound = NULL;

void SOUNDMGR_setVolume(var vol)
{
	SOUNDMGR__volume = clamp(vol, 0, 100);
}

void SOUNDMGR_scheduleSound(SOUND* snd)
{
	SOUNDMGR__nextSound = snd;
}

void SOUNDMGR_stop()
{
	SOUNDMGR__nextSound = NULL;
	SOUNDMGR__mute = 1;
}

var SOUNDMGR_isPlaying(SOUND* snd)
{	
	if (
		(SOUNDMGR__currentSound != NULL && snd == SOUNDMGR__currentSound) ||
		(SOUNDMGR__nextSound != NULL && snd == SOUNDMGR__nextSound)
		)
	{
		//DEBUG_VAR((SOUNDMGR__currentSound==NULL),90);
		//DEBUG_VAR((SOUNDMGR__nextSound==NULL),120);
		return 1;
	}

	return 0;
}

void SOUNDMGR__fader_startup()
{
	var fadeVol;
	while(1)
	{
		if (SOUNDMGR__nextSound != NULL || SOUNDMGR__mute != 0)
		{
			if (snd_playing(SOUNDMGR__handle))
			{
				fadeVol = maxv(fadeVol - (SOUNDMGR_FADESPEED * time_step), 0);
				if (fadeVol == 0)
				{
					snd_stop(SOUNDMGR__handle);
					SOUNDMGR__currentSound = NULL;
				}
				else
				{
					snd_tune(SOUNDMGR__handle, fadeVol, 0, 0);
				}
			}
			else
			{
				fadeVol = SOUNDMGR__volume;
				if (SOUNDMGR__mute == 0)
				{
					SOUNDMGR__handle = snd_play(SOUNDMGR__nextSound, SOUNDMGR__volume, 0);
					SOUNDMGR__currentSound = SOUNDMGR__nextSound;
					SOUNDMGR__nextSound = NULL;
				}
				else
				{
					SOUNDMGR__mute = 0;
				}
			}
		}
		else
		{
			if (!snd_playing(SOUNDMGR__handle))
			{
				SOUNDMGR__currentSound = NULL;
			}
		}
		wait (1);
	}
}