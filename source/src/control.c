/////////////////////////////////////////////////////////////////////
// Default key and debug functions
/////////////////////////////////////////////////////////////////////
#ifndef default_c
#define default_c
#include <acknex.h> // standard engine objects



var def_shot_num = 0;
void def_shot() 
{ 
	file_for_screen("shot_",def_shot_num); 
	beep(); 
	def_shot_num++; 
}

void def_video() 
{
	var mode = video_mode;
	while(1) {
		if (!key_shift) 
			mode++; 
		else 
			mode--;
		mode = cycle(mode,6,13); 
		if (video_switch(mode,0,0)) 
			break;
   }
}

void def_screen() 
{
	if (!key_alt) 
		return;
	var mode = video_screen + 1;
	if (mode > 2) 
		mode = 1;
	if (video_switch(0,0,mode) == 0) 
		beep();
}

var def_mvol = 50;
var def_svol = 50;
void def_sound() 
{
    if (sound_vol==0) { 
		sound_vol = def_svol; 
		midi_vol = def_mvol; 
		return; 
	}
    if (midi_vol==0) { 
		def_svol = sound_vol; 
		sound_vol = 0; 
		return; 
	}
    def_mvol = midi_vol; 
	midi_vol = 0;
}




void def_startup() 
{ 
	if (!on_f5) on_f5 = def_video;
	if (!on_f6) on_f6 = def_shot;
	if (!on_f12) on_f12 = def_sound;
	if (!on_enter) on_enter = def_screen;
}

#endif
