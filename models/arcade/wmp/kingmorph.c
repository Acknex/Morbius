
#include <acknex.h>
#include <default.c>


var lerp(var v1, var v2, var f)
{ return ((1-f)*v1 + f*v2); }


void shot(void)
{

	file_for_screen("shot.png", 0);
}


action king_dancer(void)
{
	MATERIAL* dancemat = mtl_create();
	dancemat.ambient_red = random(255);
	dancemat.ambient_green = random(255);
	dancemat.ambient_blue = random(255);

	ent_morph(me, "humanoid.mdl");
	wait(1);
	c_setminmax(my);
	my.material = dancemat;
	my.z -= c_trace(my.x, _vec(my.x, my.y, my.z - 1000), USE_POLYGON) + my.min_z;

	var anm = random(200);
	var cyc = 0;
	var spd = random(5)/10.0 + 0.85;
	
	while (1)
	{
		anm = (anm + time_step * spd) % 200;
		
		if (anm > 100)
		{
			cyc = 100 - (anm - 100);
		}
		else
		{
			cyc = anm;
		}
		
		ent_animate(me, "punch", cyc * 0.6, 0);
		wait(1);
	}
}

action king_cbabe(void)
{
	while (1)
	{
		my.pan -= time_step * 16;
		wait(1);
	}
}

action king_light(void)
{
	set(me, PASSABLE | INVISIBLE);
	
	VECTOR clr;
	
	my.lightrange = 300;
	
	while(1)
	{
		clr.x = random(255);
		clr.y = random(255);
		clr.z = random(255);

		var cyc = random(4);
		var fade = 0;

		while(fade < 1)
		{
			my.red = lerp(my.red, clr.x, fade);
			my.green = lerp(my.green, clr.y, fade);
			my.blue = lerp(my.blue, clr.z, fade);
			fade += time_step/5.0;
			wait(1);
		}
		
		wait(-cyc);
	}
}

void main(void)
{
	level_load("kingmorph.wmb");
	camera.ambient = 0;
	
	vec_set(camera.x, _vec(886, -400, 200));
	camera.pan = 210;
	//ENTITY* lotti = ent_create("humanoid.mdl", _vec(0, 0, 0), king_dancer);
	
	on_p = shot;
	
}