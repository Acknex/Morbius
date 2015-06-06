#include <acknex.h>

#define PRAGMA_POINTER
#define PRAGMA_PATH "..\src\"

#include "chapter.h"

void main()
{
	vec_set(&screen_color,vector(255,0,0)); // blau 
	video_mode = 10;
wait(1);
	CHAPTER_load("..//xml//chapters.xml");
	wait(-2);
	
	CHAPTER_show(0);
	
	while(key_esc == 0)
	{
		wait(1);
	}
	CHAPTER_hide();
	while (CHAPTER_isVisible())
	{
		wait (1);
	}
	
	wait (-2);
	
	CHAPTER_close();
	sys_exit(""); 
}
//#include <default.c>