#include "types.h"
#include "dialogs.h"
#include "inventory.h"
#include "level_transition.h"
#include "player.h"
#include "office.h"
#include "mousemgr.h"

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
//#define wait_for_dlg(x) error(x)
#define wait_for_snd(x) SOUNDMGR_scheduleSound(x); while (SOUNDMGR_isPlaying(x) != 0) wait (1)

//entryEvent, touchEvent
#define trigId skill2
#define eventId skill3
#define trigItemId skill4


SOUND* fritzCallSnd = "fritz_call.ogg";
SOUND* galepCallSnd = "galep_call.ogg";
SOUND* phoneHangupSnd = "phone_hangup.ogg";
SOUND* sugarBoilSnd = "boil.ogg";
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
		
		//Stock mit getaptem Angelhaken -> Gulli
		case 32:
		{
			interactionItem_spawn(ITEM_ID_GELDBOERSE);
			break;
		}
		
		//Topf -> Topf mit Zuckerrohr
		case 35:
		{
			interactionItem_morph(37, 55); //Herd -> benutzbarer Herd
			break;
		}
		
		//Zugängliche Telefonzelle -> Zugängliche benutzte Telefonzelle
		case 41: //use existing item ids for additional functionality
		{
			wait_for_dlg("xml\\monolog03.xml");
			wait_for_snd(fritzCallSnd);			
			wait_for_dlg("xml\\dialog02_fritz.xml");
			wait_for_snd(phoneHangupSnd);			
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
			wait_for_snd(phoneHangupSnd);			
			wait(-1.5);
			wait_for_dlg("xml\\monolog05.xml");			
			
			level_change(2, 1); //alley
			break;
		}
		
		//office startup
		case 1001: //use non existing item id (> 1000) for solely custom functionality
		{
//break;
			//fix this shit start
			error("event.c case 1001: TODO: Zeitungsartikelintrogedöns
			
Ffm / Bockenheim - In der vergangenen Nacht entdeckte die Frankfurter KriPo weitere Leichen im Stadtteil Bockenheim, welche auf brutale weise ermodet wurden. Der Serienmörder, welcher den Opfern nach dem Tod ein Z Zeichen auf die Brust markierte, wurde bisher noch nicht gefasst.");
			error("event.c case 1001: TODO: Zeitungsartikelintrogedöns
			
\"Wir setzen alles darauf, den Übeltäter auf die spur zu kommen. Leider ist er sehr sorgfältig und es gibt wenig Spuren an den Tatorten zu finden\", so Oberkommisar Seiffrig der Kriminalpolizei. \"Wir bitten, dass alle Einwohner welche Hinweise oder Augenzeugen zu den Morden haben, sich umgehend beim nähesten Polizeirevier zu melden.\"");
			error("event.c case 1001: TODO: Zeitungsartikelintrogedöns
			
Die Umstände, unter welchen die Opfer zu tode gekommen sind, werden von der KriPo Frankfurt noch enthalten, da diese angeblich zu brutal sind um sie der öffentlichkeit zu zeigen. Mehr auf seite 4.");
			//fix this shit end

			OFFICE_morbius_sit();
			OFFICE_morbius_startTalk();
				
			wait(-1.5);
			wait_for_dlg("xml\\monolog00.xml");
			//wait(-1.5);
			wait_for_dlg("xml\\dialog01_jcl.xml");
			wait_for_snd(phoneHangupSnd);			
			wait(-1.5);
			wait_for_dlg("xml\\monolog01.xml");
			
			OFFICE_morbius_stopTalk();
			OFFICE_morbius_stand();
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
		
		//alley corpse detected
		case 1004:
		{
			wait_for_dlg("xml\\monolog07.xml");
			break;
		}
		
		//enter dunkelkammer
		case 1005:
		{
			wait_for_dlg("xml\\monolog11.xml");
			break;
		}
		
		//leave alley
		case 1006:
		{
			level_change(5, 1); //greek_office
			break;
		}
		
		//alley done
		case 1007:
		{
			wait_for_dlg("xml\\monolog08.xml");
			break;
		}

		//enter greek office
		case 1008:		
		{
			OFFICE_morbius_sit();
			OFFICE_morbius_startTalk();
			
			//fool player - gecko feeding only allowed in first office level
			interactionItem_morph(6, 54); //Futter -> Fake Futter

			wait(-1.5);
			wait_for_dlg("xml\\dialog04_greek1.xml");

			OFFICE_morbius_stopTalk();
			OFFICE_morbius_stand();
			break;
		}
		
		//Herd einschalten
		case 1009:
		{
			mousemgr_set(MOUSE_USE, NULL);
			mousemgr_hint(NULL);
			wait (-1.5);
			OFFICE_startSmoke();	
			wait_for_snd(sugarBoilSnd);			
			OFFICE_stopSmoke();	
			interactionItem_morph(35, 36); //Topf mit Zuckerrohr -> Topf mit Zucker			
			mousemgr_set(MOUSE_DEFAULT, NULL);
			break;
		}
		
		//Thermoskanne gezuckert mit Griechin
		case 1010:
		{
			OFFICE_morbius_goToChair();
			while(!OFFICE_morbius_isNearChair()) wait(1);
			wait(-0.5);
			OFFICE_morbius_sit();
			OFFICE_morbius_startTalk();
			
			wait(-1.5);
			wait_for_dlg("xml\\dialog04_greek2.xml");

			OFFICE_morbius_stopTalk();
			OFFICE_morbius_stand();
			OFFICE_enableLevelGate();
			break;
		}
		
		//exit greek_office
		case 1011:
		{
			wait_for_dlg("xml\\monolog09.xml");
			level_change(3, 1); //kingmorph
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
	my->group = GROUP_CURSOR_HELPER;
	my->flags2 |= UNTOUCHABLE;

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

//skill1: EntityType 6
//skill2: TriggerId -1
//skill3: EventId -1
//skill4  triggerItemId -1
action touchEvent()
{
	VECTOR vecMin;
	VECTOR vecMax;

	set(my, INVISIBLE);
	my->ENTITY_TYPE = TYPE_EVENT_TRIGGER;
	my->group = GROUP_CURSOR_HELPER;
	my->flags2 |= UNTOUCHABLE;
	//TODO: might not work for rotated ents... see itemmgr.c
	vec_for_min(&vecMin, me);
	vec_for_max(&vecMax, me);
	vec_mul(&vecMin, &my->scale_x);
	vec_mul(&vecMax, &my->scale_x);
	vec_add(&vecMin, &my->x);
	vec_add(&vecMax, &my->x);

	if (my->trigId < 0 || my->trigId > 31)
	{
		return;
	}
	
	if ((EVENT__entrylock & (1<<my->trigId)) == 0)
	{
		while(!inventory || !is_level_loaded()) wait(1); //harghhhh
		while(!is(my, PASSABLE))
		{
			wait(1);
			if (
				(player->x > vecMin.x) && (player->x < vecMax.x) &&
				(player->y > vecMin.y) && (player->y < vecMax.y) &&
				(
					(my->trigItemId == -1) ||
					(inv_item_search(inventory, my->trigItemId) != NULL)
				)
			)
			{
				EVENT__entrylock |= (1<<my->trigId);
				set(my, PASSABLE);
				EVENT_trigger(my->eventId);
			}
		}
	}
	else
	{
		set(my, PASSABLE);
	}
}

action untouchableObject()
{
	my->flags2 |= UNTOUCHABLE;
	set(my, PASSABLE);
}

