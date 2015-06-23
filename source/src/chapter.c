#include <windows.h>
#include "xmlreader.h"
#include "list.h"

#include "xmlreader.c"
#include "list.c"

#define CHAPTER_NONE -1
#define CHAPTER_FONT_SMALL_SIZE 100
#define CHAPTER_FONT_BIG_SIZE 140
#define CHAPTER_FONT_FADESPEED 10
#define CHAPTER_TYPEWRITER_WAIT 4

FONT* CHAPTER__fontBig = "Kingthings Trypewriter 2#90";
FONT* CHAPTER__fontSmall = "Kingthings Trypewriter 2#60";
SOUND* CHAPTER__typeSound = "typewriter.ogg";
SOUND* CHAPTER__spaceSound = "typewriter_space.ogg";
SOUND* CHAPTER__returnSound = "typewriter_return.ogg";
LIST* CHAPTER__chapterList;
XMLFILE* CHAPTER__xml;
var CHAPTER__requestStop;
var CHAPTER__lastMouseMode;

CHAPTER* CHAPTER__get(int id);
void CHAPTER__copyFromXml(CHAPTER* chapter, XMLPAR* tag);
void CHAPTER__cleanup(CHAPTER* chapter);
void CHAPTER__clearString(STRING* strTarget, STRING* strSource);
void CHAPTER__copyLetter(STRING* strTarget, STRING* strSource, var pos);
void CHAPTER__keyListener();

PANEL* CHAPTER__bgPanel = 
{
	layer = 50;	
	flags = FILTER;
}

TEXT* CHAPTER__headlineText = 
{
	strings = 1;
	flags = FILTER | LIGHT | CENTER_X | TRANSLUCENT;
	blue = 255;
	green = 255;
	red = 255;
	alpha = 0;
	font = CHAPTER__fontBig;
	layer = 51;
}

TEXT* CHAPTER__contentText = 
{
	strings = 3;
	flags = FILTER | LIGHT | CENTER_X | TRANSLUCENT;
	blue = 255;
	green = 255;
	red = 255;
	alpha = 0;
	font = CHAPTER__fontSmall;
	layer = 51;
}

void (*CHAPTER__resizeEv)();


int CHAPTER_load(STRING* file)
{
	XMLPAR* hndl;
	XMLPAR* xmlItem;
	XMLPAR* xmlList;
	CHAPTER* chapter;
	var count;
	var i;
	
	AddFontResource("media//Kingthings Trypewriter 2.ttf");
	//SendMessage(HWND_BROADCAST, WM_FONTCHANGE, (WPARAM)0, (LPARAM)0);
	CHAPTER__bgPanel->bmap = bmap_createblack(64, 64, 888);
	CHAPTER__bgPanel->size_x = 64;
	CHAPTER__bgPanel->size_y = 64;
	CHAPTER__requestStop = 0;
	
	CHAPTER__resizeEv = on_resize;
	on_resize = CHAPTER_resize;
	CHAPTER_resize();

	CHAPTER__chapterList = LIST_create();	
	CHAPTER__xml = XMLFILE_create(_chr(file));	
	hndl = XMLFILE_parse(CHAPTER__xml); 
	
	if (hndl != NULL)
	{
		xmlList = XMLPAR_getElementByTag(hndl, "chapters");
		count = XMLPAR_getTagElements(xmlList);
		for (i = 0; i < count; i++)
		{
			xmlItem = XMLPAR_getElementByIndex(xmlList, i);
			chapter = (CHAPTER*)malloc(sizeof(CHAPTER));
			CHAPTER__copyFromXml(chapter, xmlItem);
			LIST_append(CHAPTER__chapterList, (void*)chapter);
		}
		return 1;
	}
	printf("CHAPTER_load: parse error. File content or file path invalid?");
	
	return 0;
}

void CHAPTER_close()
{
	int i;
	CHAPTER* tmpChapter;

	if (CHAPTER__chapterList != NULL)
	{
		for (i = 0; i < LIST_items(CHAPTER__chapterList); i++)
		{
			tmpChapter = (CHAPTER*)LIST_getItem(CHAPTER__chapterList, i);
			CHAPTER__cleanup(tmpChapter);
			LIST_removeItem(CHAPTER__chapterList, i);
		}
		LIST_remove(CHAPTER__chapterList);
	}

	if (CHAPTER__xml != NULL)
	{
		XMLFILE_remove(CHAPTER__xml);
	}

	ptr_remove(CHAPTER__bgPanel->bmap);
	RemoveFontResource("media//Kingthings Trypewriter 2.ttf");
	//SendMessage(HWND_BROADCAST, WM_FONTCHANGE, (WPARAM)0, (LPARAM)0);
}


void CHAPTER_show(int id)
{
	if (CHAPTER_isVisible())
	{
		return;
	}
	
	CHAPTER* chapter = CHAPTER__get(id);
	if (chapter == NULL)
	{
		return;
	}
	
	CHAPTER__requestStop = 0;
	CHAPTER__lastMouseMode = mouse_mode;
	mouse_mode = 0;
	
	set(CHAPTER__bgPanel, SHOW);
	wait(1);
	set(CHAPTER__headlineText, SHOW);
	set(CHAPTER__contentText, SHOW);
	CHAPTER__headlineText->alpha = 100;
	CHAPTER__contentText->alpha = 100;
	CHAPTER__keyListener();
	
	var i;
	var cnt;
	var line;

	for (line = 0; line < chapter->line_count; line++)
	{
		CHAPTER__clearString((CHAPTER__contentText->pstring)[line], chapter->line[line]);
	}
	
	if (chapter->title != NULL)
	{
		CHAPTER__clearString((CHAPTER__headlineText->pstring)[0], chapter->title);
		for (i = 1; i <= str_len(chapter->title); i++)
		{
			CHAPTER__copyLetter((CHAPTER__headlineText->pstring)[0], chapter->title, i);
	
			for(cnt = 0; cnt < CHAPTER_TYPEWRITER_WAIT; cnt += time_step)
			{
				wait (1);
			}
			if (CHAPTER__requestStop)
			{
				return;
			}
		}
		snd_play(CHAPTER__returnSound, 100, 0);
		wait(-0.5);
	}
	
	for (line = 0; line < chapter->line_count; line++)
	{
		snd_play(CHAPTER__returnSound, 100, 0);
		wait(-0.5);
		for (i = 1; i <= str_len(chapter->line[line]); i++)
		{
			CHAPTER__copyLetter((CHAPTER__contentText->pstring)[line], chapter->line[line], i);
	
			for(cnt = 0; cnt < CHAPTER_TYPEWRITER_WAIT; cnt += time_step)
			{
				wait (1);
			}
			if (CHAPTER__requestStop)
			{
				return;
			}
		}
	}
	
	wait(-3);
	CHAPTER_hide();
}

void CHAPTER_hide()
{
	if (!CHAPTER_isVisible())
	{
		return;
	}
	
	CHAPTER__requestStop = 1;
	while (CHAPTER__headlineText->alpha > 0)
	{
		wait (1);
		CHAPTER__headlineText->alpha -= CHAPTER_FONT_FADESPEED * time_step;
		CHAPTER__contentText->alpha = CHAPTER__headlineText->alpha;
	}
	CHAPTER__headlineText->alpha = 0;
	CHAPTER__contentText->alpha = 0;
	reset(CHAPTER__headlineText, SHOW);
	reset(CHAPTER__contentText, SHOW);
	reset(CHAPTER__bgPanel, SHOW);
	
	mouse_mode = CHAPTER__lastMouseMode;
}

var CHAPTER_isVisible()
{
	if (is(CHAPTER__bgPanel, SHOW))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void CHAPTER_resize()
{
	var scale = screen_size.y / 1200;
	STRING* strTemp = "#64";	

	str_printf(strTemp, "Kingthings Trypewriter 2#%i", (int)(CHAPTER_FONT_SMALL_SIZE * scale));
	ptr_remove(CHAPTER__fontSmall);
	CHAPTER__fontSmall = font_create(strTemp);

	str_printf(strTemp, "Kingthings Trypewriter 2#%i", (int)(CHAPTER_FONT_BIG_SIZE * scale));
	ptr_remove(CHAPTER__fontBig);
	CHAPTER__fontBig = font_create(strTemp);

	CHAPTER__bgPanel->scale_x = (screen_size.x + 10) / CHAPTER__bgPanel->size_x;
	CHAPTER__bgPanel->scale_y = (screen_size.y + 10) / CHAPTER__bgPanel->size_y;

	CHAPTER__headlineText->size_x = screen_size.x * 0.9;
	CHAPTER__headlineText->size_y = screen_size.y * 0.2;
	CHAPTER__headlineText->pos_x = screen_size.x * 0.5;
	CHAPTER__headlineText->pos_y = screen_size.y * 0.1;
	CHAPTER__headlineText->font = CHAPTER__fontBig;

	CHAPTER__contentText->size_x = screen_size.x * 0.9;
	CHAPTER__contentText->size_y = screen_size.y * 0.4;
	CHAPTER__contentText->pos_x = screen_size.x * 0.5;
	CHAPTER__contentText->pos_y = screen_size.y * 0.4;
	CHAPTER__contentText->font = CHAPTER__fontSmall;

	//trigger any chained resize event
	if (CHAPTER__resizeEv != NULL)
	{
		CHAPTER__resizeEv();
	}
}


CHAPTER* CHAPTER__get(int id)
{
	int i;
	CHAPTER* tmpChapter;
	
	if (CHAPTER__chapterList == NULL)
		return NULL;
		
	for (i = 0; i < LIST_items(CHAPTER__chapterList); i++)
	{
		tmpChapter = (CHAPTER*)LIST_getItem(CHAPTER__chapterList, i);
		if (tmpChapter->id == id)
		{
			return tmpChapter;
		}
	}
	
	return NULL;
}

void CHAPTER__copyFromXml(CHAPTER* chapter, XMLPAR* tag)
{
	XMLATTRIB* attrib;
	STRING* str;

	attrib = XMLATTRIB_getElementByAttribute(tag, "id");
	if (attrib != NULL)
		chapter->id = str_to_int(XMLATTRIB_getPContent(attrib));
	else
		chapter->id = CHAPTER_NONE;

	chapter->title = NULL;
	attrib = XMLATTRIB_getElementByAttribute(tag, "title");
	if (attrib != NULL)
	{
		str = str_create("");
		XMLATTRIB_getContent(attrib, str);
		chapter->title = str;
	}
	
	chapter->line_count = 0;
	chapter->line[0] = NULL;
	chapter->line[1] = NULL;
	chapter->line[2] = NULL;

	attrib = XMLATTRIB_getElementByAttribute(tag, "line1");
	if (attrib != NULL)
	{
		str = str_create("");
		XMLATTRIB_getContent(attrib, str);
		chapter->line[chapter->line_count] = str;
		chapter->line_count++;
	}
	
	attrib = XMLATTRIB_getElementByAttribute(tag, "line2");
	if (attrib != NULL)
	{
		str = str_create("");
		XMLATTRIB_getContent(attrib, str);
		chapter->line[chapter->line_count] = str;
		chapter->line_count++;
	}
	
	attrib = XMLATTRIB_getElementByAttribute(tag, "line3");
	if (attrib != NULL)
	{
		str = str_create("");
		XMLATTRIB_getContent(attrib, str);
		chapter->line[chapter->line_count] = str;
		chapter->line_count++;
	}
	
	
}

void CHAPTER__cleanup(CHAPTER* chapter)
{
	if (chapter == NULL)
		return;
	
	if (chapter->title != NULL)
		ptr_remove(chapter->title);
	if (chapter->line[0] != NULL)
		ptr_remove(chapter->line[0]);
	if (chapter->line[1] != NULL)
		ptr_remove(chapter->line[1]);
	if (chapter->line[2] != NULL)
		ptr_remove(chapter->line[2]);
}

void CHAPTER__clearString(STRING* strTarget, STRING* strSource)
{
	STRING* strTemp ="#64";
	str_printf(strTemp, "#%i", (int)str_len(strSource));
	if (strTarget != NULL)
	{
		ptr_remove(strTarget);
	}
	strTarget = str_create(strTemp);	
	ptr_remove(strTemp);
}

void CHAPTER__copyLetter(STRING* strTarget, STRING* strSource, var pos)
{
	var c = str_getchr(strSource, pos);
	if (c != ' ')
	{
		snd_play(CHAPTER__typeSound, 100, 0);
	}
	else
	{
		snd_play(CHAPTER__spaceSound, 100, 0);
	}
	str_setchr(strTarget, pos, c);
}

void CHAPTER__keyListener()
{
	while (!CHAPTER__requestStop)
	{
		if(key_esc != 0 || key_space != 0 || key_enter != 0 || mouse_left != 0)
		{
			CHAPTER__requestStop = 1;
			CHAPTER_hide();
		}
		wait (1);
	}
}