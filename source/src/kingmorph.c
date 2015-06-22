#define DANCE_HAND_PERCENT 100
#define DANCE_FEET_PERCENT 40

action kingmorphDance()
{
	var animHand = 0;
	var animHandR = 0;
	var animFeet = 0;
	var animFeetR = 0;
	var offset = 0;
	VECTOR* walk;
	VECTOR pos;

	vec_set(&pos, &my->x);
	
	while(1)
	{
		if (animFeet >= DANCE_FEET_PERCENT / 2)
		{
			animFeetR = DANCE_FEET_PERCENT - animFeet;
		}
		else
		{
			animFeetR = animFeet;
		}
		ent_animate(me,"walk", animFeetR + offset, ANM_CYCLE);
		animFeet += 6 * time_step;
		if (animFeet > DANCE_FEET_PERCENT)
		{
			offset = 50 - offset;
			animFeet -= DANCE_FEET_PERCENT;
		}

		if (animHand >= DANCE_FEET_PERCENT / 2)
		{
			animHandR = DANCE_FEET_PERCENT - animHand;
		}
		else
		{
			animHandR = animHand;
		}
		ent_animate(me,"talk", animHandR, ANM_ADD | ANM_CYCLE);
		animHand = cycle(animHand + 6 * time_step, 0, DANCE_FEET_PERCENT);

			
		walk = vector(animFeetR * 1.5, sinv(total_ticks * 30) * 5, 0);
		vec_rotate(walk, &my->pan);
		vec_add(walk, pos);
		vec_set(&my->x, walk);

		wait(1);
	}
}

action kingmorphCbabe()
{
	var anim = 0;
	
	while(1)
	{
		ent_animate(me, "duck", anim, ANM_CYCLE);
		anim = cycle(anim + 3* time_step, 0, 100);
		wait(1);
	}
}