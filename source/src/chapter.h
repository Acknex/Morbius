#ifndef CHAPTER_H
#define CHAPTER_H

typedef struct chapter
{
	int id;
	STRING* title;
	STRING* line[3];
	var line_count;
} CHAPTER;


void CHAPTER_load(STRING* file);
void CHAPTER_close();
void CHAPTER_show(int id);
void CHAPTER_hide();
var CHAPTER_isVisible();
void CHAPTER_resize();

#include "chapter.c"
#endif