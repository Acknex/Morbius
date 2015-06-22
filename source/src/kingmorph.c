#define DANCE_HAND_PERCENT 100
#define DANCE_FEET_PERCENT 40

ENTITY* KINGMORPH__cbabe = NULL;

void KINGMORPH_morbius_LookAtCbabe()
{
	if (KINGMORPH__cbabe == NULL)
	{
		return;
	}
	
	VECTOR lookat;
	ANGLE lookdir;
	var lookStart;
	var lookEnd;
	var progress = 0;
	
	vec_set(&lookat, &KINGMORPH__cbabe->x);
	vec_sub(&lookat, &player->x);
	vec_to_angle(&lookdir, &lookat);
	lookStart = player->pan;
	lookEnd = lookdir.pan;
	
	while(progress < 100)
	{
		player->pan = lookStart + (lookEnd - lookStart) * progress * 0.01;
		progress += 15 * time_step;
		wait(1);
	}
	player->pan = lookEnd;
}

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

//skill1: EntityType 1
//skill2: ItemId -1
//skill3: ItemBorder 25
action kingmorphCbabe()
{
	var anim = 0;
	
	interactionItem_remoteStart();
	KINGMORPH__cbabe = me;	
	while(1)
	{
		ent_animate(me, "duck", anim, ANM_CYCLE);
		anim = cycle(anim + 3* time_step, 0, 100);
		wait(1);
	}
}
