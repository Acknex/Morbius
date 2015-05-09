#ifndef SOUNDS_C
#define SOUNDS_C

// ************************************************************************
// ************************************************************************
//
// Abspielen von Musikstücken
//
// ************************************************************************
// ************************************************************************

// ------------------------------------------------------------------------
// Spielt ein Musikstück aus einer Datei ab
// ------------------------------------------------------------------------
void snPlayMusic(char* _file, int _loop)
{

	if (file_exists(_file))
	{
		
		SOUND* sndNewSound = snd_create(_file);
		snPlayMusic(sndNewSound, _loop);
	}
	else
	{
		#ifdef DEBUG
			printf("File %s does not exist!", _file);
		#endif
	}
}

// ------------------------------------------------------------------------
// Spielt ein Musikstück aus einem Sound-Objekt ab
// ------------------------------------------------------------------------
void snPlayMusic(SOUND* _sound, int _loop)
{
	if (snd_playing(vMusicHandle) != 0)
	{
		snStopMusic();
	}

	if (_sound != NULL)
	{
		if (_loop)
		{
			vMusicHandle = snd_loop(_sound, vMusicVolume, 0);
		}
		else
		{
			vMusicHandle = snd_play(_sound, vMusicVolume, 0);
		}
	} 
	else
	{
		#ifdef DEBUG
			printf("Sound is NULL");
		#endif
	}
}

// ------------------------------------------------------------------------
// Pausiert ein Musikstück
// ------------------------------------------------------------------------
void snPauseMusic()
{
	if (snd_playing(vMusicHandle) != 0)
	{
		snd_pause(vMusicHandle);
	}
}

// ------------------------------------------------------------------------
// Stoppt die Musik
// ------------------------------------------------------------------------
void snStopMusic()
{
	if (snd_playing(vMusicHandle) != 0)
	{
		snd_stop(vMusicHandle);
		vMusicHandle = 0;
	}
}

// ------------------------------------------------------------------------
// Gibt an, ob derzeit Musik abgespielt wird
// ------------------------------------------------------------------------
int snIsMusicPlayed()
{
	if (snd_playing(vMusicHandle) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

// ------------------------------------------------------------------------
// Ändert die Lautstärke der Musik
// ------------------------------------------------------------------------
void snSetMusicVolume(int _vol)
{
	vMusicVolume = clamp(_vol, 0, 100);
	if (vMusicHandle != 0)
	{
		snd_tune(vMusicHandle, vMusicVolume, 0, 0);
	}
}

#endif /*SOUNDS_C*/