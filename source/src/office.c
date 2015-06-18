#include "itemmgr.h"
#include "player.h"

ENTITY* OFFICE__officeChairEnt = NULL;
ENTITY* OFFICE__officeStartEnt = NULL;

//skill1: EntityType 1
//skill2: ItemId -1
//skill3: ItemBorder 25
action griechin()
{
	VECTOR* temp;

	my->tilt = 10;
	//upper legs
	temp = vector(0,80,0);
	ent_bonerotate(me,"Joint_3_3",temp);
	ent_bonerotate(me,"Joint_3_4",temp);
	//upper arms
	temp = vector(0,80,0);
	ent_bonerotate(me,"Joint_3",temp);
	ent_bonerotate(me,"Joint_3_2",temp);
	//left hand
	temp = vector(110,-25,30);
	ent_bonerotate(me,"Joint_4",temp);
	//right hand
	temp = vector(-110,-25,-30);
	ent_bonerotate(me,"Joint_4_2",temp);

	interactionItem_remoteStart();
}

action officeChair()
{
	OFFICE__officeChairEnt = me;
}

action officeStart()
{
	set(my, INVISIBLE | PASSABLE);
	OFFICE__officeStartEnt = me;
}

void morbius_sit()
{
	if (player != NULL)
	{
		if (OFFICE__officeChairEnt != NULL)
		{
			vec_set(&player->x, &OFFICE__officeChairEnt->x);
			vec_set(&player->pan, &OFFICE__officeChairEnt->pan);
			//dirty hackaround
			player->pan += 90;
			player->tilt = 10;
			player->z += 30;
		}
		VECTOR* temp = vector(0,80,0);
		ent_bonerotate(player,"Joint_3_3",temp);
		ent_bonerotate(player,"Joint_3_4",temp);
	}
}

void morbius_stand()
{
	if (player != NULL)
	{
		if (OFFICE__officeStartEnt != NULL)
		{
			vec_set(&player->x, &OFFICE__officeStartEnt->x);
			vec_set(&player->pan, &OFFICE__officeStartEnt->pan);
		}
		VECTOR* temp = vector(0,-80,0);
		ent_bonerotate(player,"Joint_3_3",temp);
		ent_bonerotate(player,"Joint_3_4",temp);
		//dirty hackaround: fool smartwalk
		vec_set(player->target_x,player->x);
	}
}