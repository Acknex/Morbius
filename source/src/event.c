#include "types.h"
#include "dialogs.h"
#include "inventory.h"
#include "level_transition.h"
#include "player.h"

//DO NOT EDIT - START
int EVENT__triggerId = -1;
var EVENT__stop = 0;
var EVENT__locked = 0;

void EVENT__evaluate(int triggerId);
void EVENT__lock();
void EVENT__unlock();

void EVENT_trigger(int triggerId)
{
	EVENT__triggerId = triggerId;
}

void EVENT_stop()
{
	EVENT__stop = 1;	
}

var EVENT_isLocked()
{
	return EVENT__locked;
}

void EVENT__listener_startup()
{
	/* a listener loop is used in order to decouple
	 * every custom event trigger from any entity which
	 * might be the trigger source.
	 * Otherwise strange behaviour may show up if the entity
	 * is removed.
	 */
	
	while(EVENT__stop == 0)
	{
		if(EVENT__triggerId != -1 && EVENT__locked == 0)
		{
			EVENT__evaluate(EVENT__triggerId);	
			EVENT__triggerId = -1;
		}
		wait (1);
	}
}
//DO NOT EDIT - END



//CUSTOM PART START
#define wait_for_dlg(x) dlgStart(x); while (dlgIsDialogActive() != 0) wait (1)
#define wait_for_snd(x) SOUNDMGR_scheduleSound(x); while (SOUNDMGR_isPlaying(x) != 0) wait (1)

#define trigId skill2
#define eventId skill3

SOUND* fritzCallSnd = "fritz_call.ogg";
SOUND* galepCallSnd = "galep_call.ogg";
ENTITY* morbiusChair = NULL;
ENTITY* morbiusStart = NULL;
long EVENT__entrylock = 0;

void EVENT__evaluate(int triggerId)
{
	EVENT__lock();
	
	//Modify switch/case as needed
	switch(triggerId)
	{
		//Abgeschlossene Schublade -> Schublade
		case 4:
		{
			//Items in drawer must be spawned or at least their position must be adapted
			//otherwise they would be visible with locked drawer
			//TODO: maybe solution for automation: allow parentId in XML?
			interactionItem_spawn(ITEM_ID_VISITENKARTE);
			interactionItem_spawn(ITEM_ID_REVOLVER);
			break;
		}
		
		//Zugängliche Telefonzelle -> Zugängliche benutzte Telefonzelle
		case 41: //use existing item ids for additional functionality
		{
			wait_for_dlg("xml\\monolog03.xml");
			wait_for_snd(fritzCallSnd);			
			wait_for_dlg("xml\\dialog02_fritz.xml");
			break;
		}
		
		//Zugängliche benutzte Telefonzelle -> zweimal benutzte Telefonzelle
		case 42:
		{
			wait_for_dlg("xml\\monolog04.xml");

			Item* item = inv_item_search(inventory, ITEM_ID_MUENZEN);
			inv_remove_item(item.inv,item);

			wait_for_snd(galepCallSnd);			
			wait_for_dlg("xml\\dialog03_galep.xml");
			wait(-1.5);
			wait_for_dlg("xml\\monolog05.xml");			
			
			level_change(2, 1);
			break;
		}
		
		//office startup
		case 1001: //use non existing item id (> 1000) for solely custom functionality
		{
			//fix this shit start
			error("event.c case 1001: TODO CERTAINLY NOT BY FIRO: Zeitungsartikelintrogedöns");
			//fix this shit end

			VECTOR* temp;
			if (player != NULL)
			{
				if (morbiusChair != NULL)
				{
					vec_set(&player->x, &morbiusChair->x);
					vec_set(&player->pan, &morbiusChair->pan);
					//dirty hackaround
					player->pan += 90;
					player->tilt = 10;
					player->z += 30;
				}
				temp = vector(0,80,0);
				ent_bonerotate(player,"Joint_3_3",temp);
				ent_bonerotate(player,"Joint_3_4",temp);
			}
			
			wait(-1.5);
			wait_for_dlg("xml\\monolog00.xml");
			//wait(-1.5);
			wait_for_dlg("xml\\dialog01_jcl.xml");
			wait(-1.5);
			wait_for_dlg("xml\\monolog01.xml");
			
			if (player != NULL)
			{
				if (morbiusStart != NULL)
				{
					vec_set(&player->x, &morbiusStart->x);
					vec_set(&player->pan, &morbiusStart->pan);
				}
				temp = vector(0,-80,0);
				ent_bonerotate(player,"Joint_3_3",temp);
				ent_bonerotate(player,"Joint_3_4",temp);
				//dirty hackaround: fool smartwalk
				vec_set(player->target_x,player->x);
			}
			break;			
		}
		
		//ruin startup
		case 1002:
		{
			wait_for_dlg("xml\\monolog02.xml");
			break;
		}
		
		//alley startup
		case 1003:
		{
			wait_for_dlg("xml\\monolog06.xml");
			break;
		}
		
		default:
		{
			break;
		}
	}
	
	EVENT__unlock();
}

void EVENT__lock()
{
	EVENT__locked = 1;

	inv_hide(inventory);
	player_may_walk = 0;
}

void EVENT__unlock()
{
	EVENT__locked = 0;

	inv_show(inventory);
	player_may_walk = 1;
}

//skill1: EntityType 6
//skill2: TriggerId -1
//skill3: EventId -1
action entryEvent()
{
	set(my, INVISIBLE | PASSABLE);
	my->ENTITY_TYPE = TYPE_EVENT_TRIGGER;

	if (my->trigId < 0 || my->trigId > 31)
	{
		return;
	}
	
	if ((EVENT__entrylock & (1<<my->trigId)) == 0)
	{
		while(!inventory || !is_level_loaded()) wait(1); //harghhhh
		EVENT__entrylock |= (1<<my->trigId);
		EVENT_trigger(my->eventId);
	}
}

action officeChair()
{
	morbiusChair = me;
}

action officeStart()
{
	set(my, INVISIBLE | PASSABLE);
	morbiusStart = me;
}

#ifdef habgradkeinenbockdrauf
//copied and patched from level_transition.c
void officeLevel_gate_event() 
{
	if (dlgIsDialogActive() != 0 || EVENT_isLocked() != 0)
		return;
		
	if (event_type == EVENT_CLICK) 
	{
		if (my.DOUBLE_CLICK_TIME >= 100) 
		{
			// Double clicked
			player_may_walk = 0;
			level_change(integer(my.skill2*0.01),my.skill2); // Instant level change
		} 
		else 
		{
			my.DOUBLE_CLICK_TIME = 110;
		}
	}
	else
	{
		level_gate_event();
	}
}

//skill1: this_id 0
//skill2: to_id 0
action officeLevel_gate()
{
	my.ENTITY_TYPE = TYPE_LEVEL_GATE;
	my.SUB_TYPE = TYPE_ITEM_EXIT;
	my.group = GROUP_CURSOR_HELPER;
	vec_set(my.skill20,vector(32,0,0));
	vec_rotate(my.skill20,my.pan);
	vec_add(my.skill20,my.x);
	c_setminmax(my);
	VECTOR temp,temp2;
	vec_set(temp,my.min_x);
	vec_rotate(temp,my.pan);
	vec_add(temp,my.x);
	vec_set(temp2,my.max_x);
	vec_rotate(temp2,my.pan);
	vec_add(temp2,my.x);
	my.skill10 = minv(temp.x,temp2.x);
	my.skill11 = minv(temp.y,temp2.y);
	my.skill12 = maxv(temp.x,temp2.x);
	my.skill13 = maxv(temp.y,temp2.y);
	set(my, POLYGON | TRANSLUCENT); //TRANSLUCENT
	my.alpha = 0;
	
	my.event = level_gate_event;
	my.emask = ENABLE_TOUCH | ENABLE_CLICK | ENABLE_RELEASE;
	
	while(1)
	{
		if(player)
		{
			if(player.x > my.skill10 && player.y > my.skill11 && player.x < my.skill12 && player.y < my.skill13)
			{
				level_change(integer(my.skill2*0.01),my.skill2);
				break;
			}
		}
		
		if (my.DOUBLE_CLICK_TIME > 0) 
		{
			my.DOUBLE_CLICK_TIME -=1 * time_step;
		}
		wait(1);
	}
}
#endif