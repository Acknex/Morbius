#ifndef DIALOGS_C
#define DIALOGS_C

#include "sounds.h"
#include "hud.h"
#include "inventory.h"
#include "player.h"

void (*dlg__resizeEv)();
// ------------------------------------------------------------------------
// Freigeben des Dialogsystems
// ------------------------------------------------------------------------
void dlgFree()
{
	// Falls wir die Auflösung ändern müssen wir alles neuzeichnen
	if (panDialogBg != NULL) {
		ptr_remove(panDialogBg);
		panDialogBg = NULL;
	}
	
	if (txtDialog != NULL) {
		ptr_remove(txtDialog);
		txtDialog = NULL;
	}
	
	if (txtDecisions != NULL) {
		ptr_remove(txtDecisions);
		txtDecisions = NULL;
	}
	
	if (txtSpeaker != NULL) {
		ptr_remove(txtSpeaker);
		txtSpeaker = NULL;
	}
	
	if (panDialogBar1 != NULL) {
		ptr_remove(panDialogBar1);
		panDialogBar1 = NULL;
	}
	
	if (panDialogBar2 != NULL) {
		ptr_remove(panDialogBar2);
		panDialogBar2 = NULL;
	}	
}

// ------------------------------------------------------------------------
// Verbergen aller Dialogelemente
// ------------------------------------------------------------------------
void dlgHide()
{
	proc_kill((void*)dlgStart);
	if (panDialogBg != NULL) { reset(panDialogBg, SHOW);}
	if (txtDialog != NULL) { reset(txtDialog, SHOW);}
	if (txtDecisions != NULL) { reset(txtDecisions, SHOW);}
	if (txtSpeaker != NULL) { reset(txtSpeaker, SHOW);}
	if (panDialogBar1 != NULL) { reset(panDialogBar1, SHOW);}
	if (panDialogBar2 != NULL) { reset(panDialogBar2, SHOW);}
}

// ------------------------------------------------------------------------
// Erzeugen aller GUI-Elemente des Dialogs
// ------------------------------------------------------------------------
void dlgInit()
{
	// Neuerzeugen der Elemente
	panDialogBg = pan_create("", 22);
	set(panDialogBg, OVERLAY | TRANSLUCENT | LIGHT);
	vec_set(panDialogBg.blue, vector(0,0,0));

	txtDialog = txt_create(1, 23);
	set(txtDialog, WWRAP);
	txtDialog.font = fontDialogs;
	vec_set(txtDialog.blue, vector(255,255,255));

	txtDecisions = txt_create(4, 23);
	set(txtDecisions, WWRAP);
	txtDecisions.font = fontDialogs;
	vec_set(txtDecisions.blue, vector(255,255,255));

	txtSpeaker = txt_create(1, 23);
	set(txtSpeaker, WWRAP);
	txtSpeaker.font = fontDialogBold;
	vec_set(txtSpeaker.blue, vector(0,0,255));

	panDialogBar1 = pan_create("", 21);
	set(panDialogBar1, OVERLAY | TRANSLUCENT | LIGHT);
	vec_set(panDialogBar1.blue, vector(0,0,0));

	panDialogBar2 = pan_create("", 21);
	set(panDialogBar2, OVERLAY | TRANSLUCENT | LIGHT);
	vec_set(panDialogBar2.blue, vector(0,0,0));

	//dlgAlign();
	dlg__resizeEv = on_resize;
	on_resize = dlg_resize;
	dlg_resize();

}

// ------------------------------------------------------------------------
// Ausrichten der Dialoge
// (Muss nach jedem Auflösungswechsel aufgerufen werden)
// ------------------------------------------------------------------------
void dlgAlign(var fontSize)
{
	panDialogBar1.size_x = screen_size.x;
	panDialogBar1.size_y = screen_size.y * 0.09;//100;
	panDialogBar1.pos_x = 0;
	panDialogBar1.pos_y = 0;

	panDialogBar2.size_x = screen_size.x;
	panDialogBar2.size_y = screen_size.y * 0.09;//100;
	panDialogBar2.pos_x = 0;
	panDialogBar2.pos_y = screen_size.y - panDialogBar2.size_y;//100;

	panDialogBg.size_x = screen_size.x;
	panDialogBg.size_y = fontSize * 4;//130;
	panDialogBg.pos_x = 0;
	panDialogBg.pos_y = screen_size.y - panDialogBg.size_y - panDialogBar2.size_y - 10;//- 130 - 110;

	txtDialog.pos_x = panDialogBg.pos_x + fontSize;//40;
	txtDialog.pos_y = panDialogBg.pos_y + (1.5 * fontSize);//40;
	txtDialog.size_x = screen_size.x - (2 * (txtDialog.pos_x - panDialogBg.pos_x));
	txtDialog.size_y = fontSize * 3;//120;

	txtDecisions.pos_x = panDialogBg.pos_x + fontSize;//40;
	txtDecisions.pos_y = panDialogBg.pos_y + (1.5 * fontSize);//40;	
	txtDecisions.size_x = screen_size.x - (2 * (txtDecisions.pos_x - panDialogBg.pos_x));
	txtDecisions.size_y = fontSize * 3;//120;

	txtSpeaker.pos_x = panDialogBg.pos_x + fontSize;//40;
	txtSpeaker.pos_y = panDialogBg.pos_y + (0.5 * fontSize);//30;
	txtSpeaker.size_x = screen_size.x - (2 * (txtSpeaker.pos_x - panDialogBg.pos_x));//100;
	txtSpeaker.size_y = fontSize;//30;

}

// ------------------------------------------------------------------------
// Hilfsfunktion die das Tag mit der ID "id" ausliest
// ------------------------------------------------------------------------
XMLPAR* getDialogItemById(XMLPAR* _myXML, int _id)
{
	XMLPAR* tempItem;
	XMLATTRIB* tempAttr;
	STRING* tempStr = str_create("");
	STRING* tempStr2 = str_create("");
	int i = 0;
	
	tempItem = XMLPAR_getElementByIndex(_myXML,i);
	while (tempItem != NULL)
	{
		tempAttr = XMLATTRIB_getElementByAttribute(tempItem, "id");
		if (tempAttr != NULL)
		{
			XMLATTRIB_getContent(tempAttr,tempStr);
			str_for_num(tempStr2,_id);
			if (str_cmp(tempStr,tempStr2))
			{
				return tempItem;
			}
			i +=1;
			tempItem = XMLPAR_getElementByIndex(_myXML,i);
		}
	}
	return NULL;
}

// ------------------------------------------------------------------------
// Ermittelt die ID eines Tags
// ------------------------------------------------------------------------
int getDialogItemId(XMLPAR* _XMLItem)
{
	STRING* tempStr = str_create("");
	XMLATTRIB* tempAttr;
	tempAttr = XMLATTRIB_getElementByAttribute(_XMLItem, "id");
	if (tempAttr != NULL)
	{
		XMLATTRIB_getContent(tempAttr,tempStr);
		return str_to_int(tempStr);
	}
}

// ------------------------------------------------------------------------
// Erzeugen eines einfachen Dialogs
// ------------------------------------------------------------------------
void dlgStart(STRING* _speaker, STRING* _text, SOUND* _audio)
{
	// Wenn ein anderer Dialog aktiv ist, können wir keinen neuen starten
	if (dlgIsDialogActive()) return;
	inv_hide(inventory);
	player_may_walk = 0;
	mousemgr_set(MOUSE_TALK, NULL);
	mousemgr_hint(NULL);

	nIsDialogActive = 1;
	while(mouse_left) wait(1);
	
	str_cpy((txtSpeaker.pstring)[0], _speaker);
	str_cpy((txtDialog.pstring)[0], _text);
	set(txtSpeaker,SHOW);
	set(panDialogBg,SHOW);
	set(txtDialog,SHOW);

	// Wenn ein Sound definiert ist, warte mit dem Ausblenden der GUI
	// bis er vorüber ist. Ansonsten warte eine berechnete Zeitspanne.
	if (_audio != NULL)
	{
		var vTemp = snd_play(_audio, vSpeechVolume, 0);
		while(snd_playing(vTemp) != 0)
		{
			wait(1);
			if (mouse_left)
			{
				while(mouse_left) wait(1);
				break;
			}
		}
	}
	else
	{
		var vWaitTime = str_len(_text) * 30 * time_step;
		while (vWaitTime > 0)
		{
			wait(1);
			if (mouse_left)
			{
				while(mouse_left) wait(1);
				break;
			}
			vWaitTime -=1;
		}
	}

	reset(txtSpeaker,SHOW);
	reset(panDialogBg,SHOW);
	reset(txtDialog,SHOW);
	nIsDialogActive = 0;
	inv_show(inventory);
	player_may_walk = 1;
	mousemgr_set(MOUSE_DEFAULT, NULL);
	mousemgr_hint(NULL);
	
}



// ------------------------------------------------------------------------
// Starten eines komplexen XML-Dialogs
// ------------------------------------------------------------------------
int dlgStart(STRING* _dialogFile)
{	
	if (dlgIsDialogActive()) return;

	if (_dialogFile == NULL) return;
	
	inv_hide(inventory);
	player_may_walk = 0;
	mousemgr_set(MOUSE_TALK, NULL);
	mousemgr_hint(NULL);
	nIsDialogActive = 1;
	
	int nCancelDialog = 0;
	
	// 2 schwarze Balken oben und unten werden eingeblendet
	panDialogBar1.alpha = 0;
	panDialogBar2.alpha = 0;
	set(panDialogBar1,SHOW);
	set(panDialogBar2,SHOW);
	while(panDialogBar1.alpha < 100)
	{
		panDialogBar1.alpha +=20*time_step;
		panDialogBar2.alpha +=20*time_step;
		wait(1);
	}
	
	// Erstelle Bilder für die Auswahlboden
	bmapDialogBtnDown = bmap_createblack(screen_size.x, 15, 32);
	bmap_fill(bmapDialogBtnDown, vector(0,0,255), 50);
	bmapDialogBtnUp = bmap_createblack(screen_size.x, 15, 32);
	bmap_fill(bmapDialogBtnUp, vector(10,10,10), 50);

	XMLPAR *pParHndl, *pPar, *pParMain, *pParGoto, *pParChoice;
	XMLATTRIB * pAttrib;
	STRING* strXMLTag = str_create("");
	STRING* strXMLTemp = str_create("");
	STRING* strXMLSpeaker = str_create("");
	STRING* strXMLGotoTarget = str_create("");
	STRING* strChoiceTarget1 = str_create("");
	STRING* strChoiceTarget2 = str_create("");
	STRING* strChoiceTarget3 = str_create("");
	STRING* strChoiceTarget4 = str_create("");
	STRING* returnValue		 = str_create("");
	SOUND* tempSnd = NULL;
	XMLFILE* pXml = XMLFILE_create(_dialogFile);
	pParHndl = XMLFILE_parse(pXml);

	int i = 0;
	int j = 0;

	// Wenn die XML-Datei gelesen werden konnte...
	if (pParHndl != NULL)
	{

		// Hole das Root-Element
		pParMain = XMLPAR_getElementByTag(pParHndl, "Dialog");

		// Hole das erste Element unter dem Root
		pPar = XMLPAR_getElementByIndex(pParMain,i);

		// Lese das erste Tag
		XMLPAR_getTag(pPar, strXMLTag);
		
		// Solange wir das End-Tag nicht erreicht haben
		while (str_cmp(strXMLTag,"End") != 1)
		{
			
			// Wird bereits ein Sound abgespielt? Dann stoppe ihn
			if (snd_playing(vDialogSpeechHandle) > 0)
			{
				snd_stop(vDialogSpeechHandle);
			}
			
			// Der Spieler sagt etwas
			if (str_cmp(strXMLTag,"Player") == 1)
			{

				// Ein Soundfile wird abgespielt
				pAttrib = XMLATTRIB_getElementByAttribute(pPar, "file");
				if (pAttrib != NULL)
				{
					XMLATTRIB_getContent(pAttrib, strXMLTemp);
					tempSnd = snd_create(strXMLTemp);
					if (tempSnd != NULL)
					{
						vDialogSpeechHandle = snd_play(tempSnd, vSpeechVolume, 0);
					}
				}

				// Gibt es ein "returnvalue"-Atribut ändern wir die Variable "returnvalue"
				pAttrib = XMLATTRIB_getElementByAttribute(pPar, "returnvalue");
				if (pAttrib != NULL)
				{
					XMLATTRIB_getContent(pAttrib, returnValue);
				}				

				// Zeige den Text des Dialogelements an
				XMLPAR_getContent(pPar, strXMLTemp);
				str_cpy((txtDialog.pstring)[0],strXMLTemp);

				// ... sowie den Spielernamen
				pAttrib = XMLATTRIB_getElementByAttribute(pPar, "name");
				if (pAttrib != NULL)
				{
					XMLATTRIB_getContent(pAttrib, strXMLTemp);
					str_cpy((txtSpeaker.pstring)[0],strXMLTemp);
				}

				// Zeige den Dialog an
				set(txtSpeaker,SHOW);
				set(panDialogBg,SHOW);
				set(txtDialog,SHOW);
				i +=1;

				// Und hole das nächste Element das verarbeitet werden muss
				pPar = getDialogItemById(pParMain,i);
				i = getDialogItemId(pPar);
				
				// Dialog fortsetzen
				if (snd_playing(vDialogSpeechHandle) > 0)
				{
					while(snd_playing(vDialogSpeechHandle) > 0) {
						if (key_esc) {
							while(key_esc) wait(1);
							nCancelDialog = 1;
							snd_stop(vDialogSpeechHandle);
						}
						wait(1);
					}
				}
				else
				{
					// Wenn die Maus irgendwo geklickt und losgelassen wurde machen wir weiter
					while(!mouse_left && !key_esc) {
						wait(1);
					}
					if (key_esc) nCancelDialog = 1;
					while((mouse_left) || (key_esc)) wait(1);
				}
			}
			
			// Ein NPC sagt etwas
			if (str_cmp(strXMLTag,"NPC") == 1)
			{

				// Abspielen einer Sounddatei
				pAttrib = XMLATTRIB_getElementByAttribute(pPar, "file");
				if (pAttrib != NULL)
				{
					XMLATTRIB_getContent(pAttrib, strXMLTemp);
					
					tempSnd = snd_create(strXMLTemp);

					if (tempSnd != NULL)
					{
						vDialogSpeechHandle = snd_play(tempSnd, vSpeechVolume, 0);
					}
				}

				// Ändern des Rückgabewertes
				pAttrib = XMLATTRIB_getElementByAttribute(pPar, "returnvalue");
				if (pAttrib != NULL)
				{
					XMLATTRIB_getContent(pAttrib, returnValue);
				}


				// Anzeigen des Textes
				XMLPAR_getContent(pPar, strXMLTemp);
				str_cpy((txtDialog.pstring)[0],strXMLTemp);
				
				// ... Und des Namen der NPC
				pAttrib = XMLATTRIB_getElementByAttribute(pPar, "name");
				if (pAttrib != NULL)
				{
					XMLATTRIB_getContent(pAttrib, strXMLTemp);
					str_cpy((txtSpeaker.pstring)[0],strXMLTemp);
				}
				set(txtSpeaker,SHOW);
				set(panDialogBg,SHOW);
				set(txtDialog,SHOW);				
				i +=1;
				pPar = getDialogItemById(pParMain,i);
				i = getDialogItemId(pPar);
				
				// Dialog fortsetzen
				if (snd_playing(vDialogSpeechHandle) > 0)
				{
					while(snd_playing(vDialogSpeechHandle) > 0) {
						if (key_esc) {
							while(key_esc) wait(1);
							nCancelDialog = 1;
							snd_stop(vDialogSpeechHandle);
						}
						wait(1);
					}
				}
				else
				{
					// Wenn die Maus irgendwo geklickt und losgelassen wurde machen wir weiter
					while(!mouse_left && !key_esc) {
						wait(1);
					}
					if (key_esc) nCancelDialog = 1;
					while((mouse_left) || (key_esc)) wait(1);
				}
			}
			
			// Auswahlbox
			if (str_cmp(strXMLTag,"Choice") == 1)
			{
				
				// Es wurde noch keine Entscheidung gefällt
				nDecisionsMade = 0;

				// Das Decision-Panel wird erzeugt. Darauf werden gleich die Auswahlbuttons platziert
				panDecisionBg = pan_create("", 23);
				set(panDecisionBg, OVERLAY | TRANSLUCENT);
				panDecisionBg.size_x = screen_size.x;
				panDecisionBg.size_y = 130;
				panDecisionBg.pos_x = panDialogBg.pos_x;
				panDecisionBg.pos_y = panDialogBg.pos_y;
				
				// Hole das erste Unterelement.
				pParChoice = XMLPAR_getElementByIndex(pPar,0);

				// Wenn es existiert haben wir eine Entscheidung hinzuzufügen
				if (pParChoice != NULL)
				{
					XMLPAR_getContent(pParChoice, strXMLTemp);
					
					// Wohin müssen wir springen wenn das erste Item ausgewählt wurde?
					pAttrib = XMLATTRIB_getElementByAttribute(pParChoice, "target");
					if (pAttrib != NULL)
					{
						XMLATTRIB_getContent(pAttrib, strChoiceTarget1);
					}

					// Anzeigen des Textes
					str_cpy((txtDecisions.pstring)[0],strXMLTemp);

					// ... und Erstellen des Buttons
					pan_setbutton(panDecisionBg,0,1,0,40,bmapDialogBtnDown,bmapDialogBtnUp,bmapDialogBtnDown,NULL,dlgClickDialog,NULL,NULL);
				}
				else
				{
					str_cpy((txtDecisions.pstring)[0],"");
				}
				
				// Eine zweite Auswahl...
				pParChoice = XMLPAR_getElementByIndex(pPar,1);
				if (pParChoice != NULL)
				{
					XMLPAR_getContent(pParChoice, strXMLTemp);
					pAttrib = XMLATTRIB_getElementByAttribute(pParChoice, "target");
					if (pAttrib != NULL)
					{
						XMLATTRIB_getContent(pAttrib, strChoiceTarget2);
					}
					str_cpy((txtDecisions.pstring)[1],strXMLTemp);
					pan_setbutton(panDecisionBg,0,1,0,55,bmapDialogBtnDown,bmapDialogBtnUp,bmapDialogBtnDown,NULL,dlgClickDialog,NULL,NULL);
				}	
				else
				{
					str_cpy((txtDecisions.pstring)[1],"");
				}
					
				// Eine dritte Auswahl
				pParChoice = XMLPAR_getElementByIndex(pPar,2);
				if (pParChoice != NULL)
				{
					XMLPAR_getContent(pParChoice, strXMLTemp);
					pAttrib = XMLATTRIB_getElementByAttribute(pParChoice, "target");
					if (pAttrib != NULL)
					{
						XMLATTRIB_getContent(pAttrib, strChoiceTarget3);
					}
					str_cpy((txtDecisions.pstring)[2],strXMLTemp);
					pan_setbutton(panDecisionBg,0,1,0,70,bmapDialogBtnDown,bmapDialogBtnUp,bmapDialogBtnDown,NULL,dlgClickDialog,NULL,NULL);
				}
				else
				{
					str_cpy((txtDecisions.pstring)[2],"");
				}
				
				// Eine vierte Auswahl
				pParChoice = XMLPAR_getElementByIndex(pPar,3);
				if (pParChoice != NULL)
				{
					XMLPAR_getContent(pParChoice, strXMLTemp);
					pAttrib = XMLATTRIB_getElementByAttribute(pParChoice, "target");
					if (pAttrib != NULL)
					{
						XMLATTRIB_getContent(pAttrib, strChoiceTarget4);
					}
					str_cpy((txtDecisions.pstring)[3],strXMLTemp);
					pan_setbutton(panDecisionBg,0,1,0,85,bmapDialogBtnDown,bmapDialogBtnUp,bmapDialogBtnDown,NULL,dlgClickDialog,NULL,NULL);
				}
				else
				{
					str_cpy((txtDecisions.pstring)[3],"");
				}
				
				reset(txtDialog, SHOW);
				reset(txtSpeaker,SHOW);
				set(panDialogBg, SHOW);
				set(panDecisionBg, SHOW);
				set(txtDecisions, SHOW);

				// Solange keine Entscheidung getroffen wurde, warte...
				while(nDecisionsMade == 0)
				{
					wait(1);
				}
				while(mouse_left) wait(1);
				reset(panDecisionBg, SHOW);
				reset(txtDecisions, SHOW);
				
				switch (nDecisionsMade)
				{
					case 1: i = str_to_int(strChoiceTarget1); break;
					case 2: i = str_to_int(strChoiceTarget2); break;
					case 3: i = str_to_int(strChoiceTarget3); break;
					case 4: i = str_to_int(strChoiceTarget4); break;
					default:
						error("Decision not defined!");
				}

				pPar = XMLPAR_getElementByIndex(pParMain,i);
			}
			
			
			
			// Ein "Goto" veranlasst das das Dialogsystem zu einem Element unserer Wahl springt.
			if (str_cmp(strXMLTag,"Goto") == 1)
			{
				pAttrib = XMLATTRIB_getElementByAttribute(pPar, "target");
				if (pAttrib != NULL)
				{
					XMLATTRIB_getContent(pAttrib, strXMLGotoTarget);
					
					// Finde das Element mit der in "target" angegebenen ID
					pParGoto = XMLPAR_getElementByIndex(pParMain,j);
					pAttrib = XMLATTRIB_getElementByAttribute(pParGoto, "id");
					if (pAttrib != NULL)
					{
						XMLATTRIB_getContent(pAttrib, strXMLTemp);
						while(str_cmp(strXMLTemp,strXMLGotoTarget) != 1)
						{
							j +=1;
							pParGoto = XMLPAR_getElementByIndex(pParMain,j);
							pAttrib = XMLATTRIB_getElementByAttribute(pParGoto, "id");
							if (pAttrib != NULL)
							{
								XMLATTRIB_getContent(pAttrib, strXMLTemp);
							}
							else break;
						}
					}
				}
				j = 0;
				pPar = pParGoto;

				// Nun laden wir als nächstes Element das, was in "target" angegeben wurde
				i = getDialogItemId(pPar);
			}
			
			if (nCancelDialog == 1) {
				str_cpy(returnValue, "");
				break;
			}
			wait(1);
			
			XMLPAR_getTag(pPar, strXMLTag);
		}

		// Der Dialog ist abgearbeitet, räumen wir auf
		reset(panDialogBg,SHOW);
		reset(txtDialog,SHOW);
		reset(panDialogBg, SHOW);
		reset(txtDialog, SHOW);
		reset(txtSpeaker,SHOW);
	}
	XMLFILE_remove(pXml);

	// Verberge die schwarzen Balken des Dialogs
	while(panDialogBar1.alpha > 0)
	{
		panDialogBar1.alpha -=20*time_step;
		panDialogBar2.alpha -=20*time_step;
		wait(1);
	}
	reset(panDialogBar1,SHOW);
	reset(panDialogBar2,SHOW);
	
	nIsDialogActive = 0;
	
	inv_show(inventory);
	player_may_walk = 1;
	mousemgr_set(MOUSE_DEFAULT, NULL);
	mousemgr_hint(NULL);
	// "returnValue" wird zurückgegeben falls er existiert, sonst: -1
	if (str_cmp(returnValue,"") == 1)
	{
		return -1;
	}
	else
	{
		return str_to_num(returnValue);
	}
}

// ------------------------------------------------------------------------
// Funktion evaluiert welcher Button auf dem Dialogpanel gedrückt wurde
// ------------------------------------------------------------------------
void dlgClickDialog(var _buttonNumber, PANEL* _panel)
{
	switch (_buttonNumber)
	{
		case 1: nDecisionsMade = 1; break;
		case 2: nDecisionsMade = 2; break;
		case 3: nDecisionsMade = 3; break;
		case 4: nDecisionsMade = 4; break;
	}
}

// ------------------------------------------------------------------------
// Gibt zurück ob der Dialog im Moment aktiv ist
// ------------------------------------------------------------------------
int dlgIsDialogActive()
{
	return nIsDialogActive;
}

void dlg_resize()
{
	if (dlg__resizeEv != NULL)
	{
		dlg__resizeEv();
	}
	
	FONT* fnt = HUD_getFont();
	if (fnt != NULL)
	{
		txtDialog.font = fnt;
		txtDecisions.font = fnt;
		txtSpeaker.font = fnt; //todo bold font?
	}
	var fontSize = HUD_getFontSize();
	dlgAlign(fontSize);
}

#endif