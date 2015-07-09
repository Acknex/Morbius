#define PANELS_FADESPEED 7
#define PANELS_ROTATESPEED 45
#define PANELS_STARTANGLE 1024

var PANELS__active = 0;

PANEL* PANELS__bgPanel = 
{
	layer = 50;	
	flags = FILTER | TRANSLUCENT;
}

PANEL* PANELS__introNewsPanel =
{
	layer = 51;	
	flags = FILTER;
	bmap = "intronews.pcx";
}

PANEL* PANELS__goodNewsPanel =
{
	layer = 51;	
	flags = FILTER;
	bmap = "goodnews.pcx";
}

PANEL* PANELS__badNewsPanel =
{
	layer = 51;	
	flags = FILTER;
	bmap = "badnews.pcx";
}

PANEL* PANELS__emailPanel =
{
	layer = 51;	
	flags = FILTER | TRANSLUCENT;
	bmap = "emailnews.pcx";
}

void (*PANELS__resizeEv)();

void PANELS__fadeInBg();
void PANELS__fadeOutBg();
void PANELS__keyListener();

void PANELS_showIntroNews()
{
	if (PANELS__active)
	{
		return;
	}
	
	PANELS__active = 1;
	PANELS__fadeInBg();
	PANELS__keyListener();	
	PANELS__introNewsPanel->angle = PANELS_STARTANGLE;
	set(PANELS__introNewsPanel, SHOW);

	while(PANELS__introNewsPanel->angle > 0 && PANELS__active)
	{
		PANELS__introNewsPanel->angle = maxv(0, PANELS__introNewsPanel->angle - PANELS_ROTATESPEED * time_step);
		PANELS__introNewsPanel->scale_x = (1 - (PANELS__introNewsPanel->angle / PANELS_STARTANGLE)) * screen_size.y / 1200;
		PANELS__introNewsPanel->scale_y = PANELS__introNewsPanel->scale_x; 
		wait (1);
	}
}

void PANELS_showGoodNews()
{
	if (PANELS__active)
	{
		return;
	}
	
	PANELS__active = 1;
	PANELS__fadeInBg();
	PANELS__keyListener();	
	PANELS__goodNewsPanel->angle = PANELS_STARTANGLE;
	set(PANELS__goodNewsPanel, SHOW);

	while(PANELS__goodNewsPanel->angle > 0 && PANELS__active)
	{
		PANELS__goodNewsPanel->angle = maxv(0, PANELS__goodNewsPanel->angle - PANELS_ROTATESPEED * time_step);
		PANELS__goodNewsPanel->scale_x = (1 - (PANELS__goodNewsPanel->angle / PANELS_STARTANGLE)) * screen_size.y / 1200;
		PANELS__goodNewsPanel->scale_y = PANELS__goodNewsPanel->scale_x; 
		wait (1);
	}
}

void PANELS_showBadNews()
{
	if (PANELS__active)
	{
		return;
	}
	
	PANELS__active = 1;
	PANELS__fadeInBg();
	PANELS__keyListener();
	PANELS__badNewsPanel->angle = PANELS_STARTANGLE;
	set(PANELS__badNewsPanel, SHOW);

	while(PANELS__badNewsPanel->angle > 0 && PANELS__active)
	{
		PANELS__badNewsPanel->angle = maxv(0, PANELS__badNewsPanel->angle - PANELS_ROTATESPEED * time_step);
		PANELS__badNewsPanel->scale_x = (1 - (PANELS__badNewsPanel->angle / PANELS_STARTANGLE)) * screen_size.y / 1200;
		PANELS__badNewsPanel->scale_y = PANELS__badNewsPanel->scale_x; 
		wait (1);
	}
}

void PANELS_showEmail()
{
	if (PANELS__active)
	{
		return;
	}
	
	PANELS__active = 1;
	PANELS__fadeInBg();
	PANELS__keyListener();
	PANELS__emailPanel->alpha = 0;
	set(PANELS__emailPanel, SHOW);

	while(PANELS__emailPanel->alpha < 100 && PANELS__active)
	{
		wait (1);
		PANELS__emailPanel->alpha += PANELS_FADESPEED * time_step;
	}
	if (PANELS__active)
	{
		PANELS__emailPanel->alpha = 100;
	}
}

void PANELS_hide()
{
	if (!PANELS__active)
	{
		return;
	}
	
	reset(PANELS__introNewsPanel, SHOW);
	reset(PANELS__goodNewsPanel, SHOW);
	reset(PANELS__badNewsPanel, SHOW);
	reset(PANELS__emailPanel, SHOW);
	PANELS__active = 0;
	PANELS__fadeOutBg();
}

var PANELS_isVisible()
{
	return PANELS__active;
}

void PANELS_resize()
{
	var vScale = screen_size.y / 1200;
	
	PANELS__bgPanel->scale_x = (screen_size.x + 10) / PANELS__bgPanel->size_x;
	PANELS__bgPanel->scale_y = (screen_size.y + 10) / PANELS__bgPanel->size_y;

	PANELS__introNewsPanel->scale_x = vScale;
	PANELS__introNewsPanel->scale_y = vScale;
	PANELS__introNewsPanel->pos_x = (screen_size.x - PANELS__introNewsPanel->size_x * PANELS__introNewsPanel->scale_x) * 0.5;
	PANELS__introNewsPanel->pos_y = (screen_size.y - PANELS__introNewsPanel->size_y * PANELS__introNewsPanel->scale_y) * 0.5;

	PANELS__goodNewsPanel->scale_x = vScale;
	PANELS__goodNewsPanel->scale_y = vScale;
	PANELS__goodNewsPanel->pos_x = (screen_size.x - PANELS__goodNewsPanel->size_x * PANELS__goodNewsPanel->scale_x) * 0.5;
	PANELS__goodNewsPanel->pos_y = (screen_size.y - PANELS__goodNewsPanel->size_y * PANELS__goodNewsPanel->scale_y) * 0.5;

	PANELS__badNewsPanel->scale_x = vScale;
	PANELS__badNewsPanel->scale_y = vScale;
	PANELS__badNewsPanel->pos_x = (screen_size.x - PANELS__badNewsPanel->size_x * PANELS__badNewsPanel->scale_x) * 0.5;
	PANELS__badNewsPanel->pos_y = (screen_size.y - PANELS__badNewsPanel->size_y * PANELS__badNewsPanel->scale_y) * 0.5;

	PANELS__emailPanel->scale_x = vScale * 1200 / PANELS__emailPanel->size_y;
	PANELS__emailPanel->scale_y = PANELS__emailPanel->scale_x;
	PANELS__emailPanel->pos_x = (screen_size.x - PANELS__emailPanel->size_x * PANELS__emailPanel->scale_x) * 0.5;
	PANELS__emailPanel->pos_y = (screen_size.y - PANELS__emailPanel->size_y * PANELS__emailPanel->scale_y) * 0.5;

	//trigger any chained resize event
	if (PANELS__resizeEv != NULL)
	{
		PANELS__resizeEv();
	}
}

void PANELS__startup()
{
	PANELS__bgPanel->bmap = bmap_createblack(64, 64, 888);
	PANELS__bgPanel->size_x = 64;
	PANELS__bgPanel->size_y = 64;

	PANELS__introNewsPanel->center_x = PANELS__introNewsPanel->size_x * 0.5;
	PANELS__introNewsPanel->center_y = PANELS__introNewsPanel->size_y * 0.5;

	PANELS__goodNewsPanel->center_x = PANELS__goodNewsPanel->size_x * 0.5;
	PANELS__goodNewsPanel->center_y = PANELS__goodNewsPanel->size_y * 0.5;

	PANELS__badNewsPanel->center_x = PANELS__badNewsPanel->size_x * 0.5;
	PANELS__badNewsPanel->center_y = PANELS__badNewsPanel->size_y * 0.5;

	PANELS__resizeEv = on_resize;
	on_resize = PANELS_resize;
	PANELS_resize();
}

void PANELS__fadeInBg()
{
	set(PANELS__bgPanel, SHOW);
/*	PANELS__bgPanel->alpha = 0;
	while (PANELS__bgPanel->alpha < 100 && PANELS__active)
	{
		wait(1);
		PANELS__bgPanel->alpha += PANELS_FADESPEED * time_step;
	}*/
	if (PANELS__active)
	{
		PANELS__bgPanel->alpha = 100;
	}
}

void PANELS__fadeOutBg()
{
	while (PANELS__bgPanel->alpha > 0 && !PANELS__active)
	{
		wait(1);
		PANELS__bgPanel->alpha -= PANELS_FADESPEED * time_step;
	}
	if (!PANELS__active)
	{
		PANELS__bgPanel->alpha = 0;
		reset(PANELS__bgPanel, SHOW);
	}
}

void PANELS__keyListener()
{
	while (PANELS__active)
	{
		if(key_esc != 0 || key_space != 0 || key_enter != 0 || mouse_left != 0)
		{
			PANELS_hide();
		}
		wait (1);
	}
}