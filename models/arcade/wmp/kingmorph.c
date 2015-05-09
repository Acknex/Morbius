
#include <acknex.h>
#include <default.c>




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

void main(void)
{
	level_load("kingmorph.wmb");
	
	//ENTITY* lotti = ent_create("humanoid.mdl", _vec(0, 0, 0), king_dancer);
	
	on_p = shot;
	
}