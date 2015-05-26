
#include <acknex.h>


// C FILE for the dark alley.


void alley_fade_p(PARTICLE* p)
{
	p.alpha -= p.skill_a*time_step;
	if (p.alpha <= 0) p.lifespan = 0;
	
	p.size = minv(p.skill_b, p.size+time_step*5);
}

BMAP* bmp_alley_smoke = "rauch2.tga";

void alley_smoke_p(PARTICLE* p)
{
	p.skill_a = 3;
	p.bmap = bmp_alley_smoke;
	p.lifespan = 1200+random(600);
	p.skill_b = 20 + random(15);
	p.size = 0;
	p.alpha = 64;
	vec_scale(p.vel_x, 0.4);
	set(p, TRANSLUCENT | MOVE | BRIGHT);
	p.event = alley_fade_p;
}

action alley_smoke(void)
{
	set(me, PASSABLE | INVISIBLE);
	var partout = 0;
	while (1)
	{
		partout += time_step/3;
		effect(alley_smoke_p, partout,
		_vec(me.x+random(10)-5, me.y+random(10)-5, me.z+random(10)-5),
		_vec(-5-random(10), -2-random(4), 2+random(4)));
		partout -= integer(partout);
		wait(1);
	}
}
