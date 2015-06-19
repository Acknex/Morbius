#ifndef DIALOGS_H
#define DIALOGS_H

#include "xmlreader.h"

#include "xmlreader.c"

BMAP* bmapDialogBtnDown = NULL;
BMAP* bmapDialogBtnUp = NULL;

// Speichert ob bereits eine entscheidung in der Auswahlbox getroffen wurde.
// Solange nDecisionsMade 0 ist, wird die Funktion angehalten.
int nDecisionsMade = 0;

FONT* fontDialogs = "Arial#14";
FONT* fontDialogBold = "Arial#14b";

// Der Hintergrund für die Dialoge
PANEL* panDialogBg;

// Der obere schwarze Balken der bei den Dialogen eingeblendet wird
PANEL* panDialogBar1;

// Der untere schwarze Balken der bei den Dialogen eingeblendet wird
PANEL* panDialogBar2;

// Das Textfeld das die Dialoge enthält
TEXT* txtDialog;

// Das Panel das die Antwortmöglichkeiten beinhaltet. Muss dynamisch sein,
// da abhängig vom Dialog verschieden viele Buttons hinzugefügt werden müssen.
PANEL* panDecisionBg;

// Text der die Antwortmöglichkeiten anzeigt.
TEXT* txtDecisions;

// Text der anzeigt wer im Moment spricht
TEXT* txtSpeaker;

// Das Handle das das Abspielen von Sounds kontrolliert
var vDialogSpeechHandle = 0;

int nIsDialogActive = 0;

void dlgInit();
void dlgFree();
void dlgHide();
void dlgAlign(var fontSize);
void dlgClickDialog(var _buttonNumber, PANEL* _panel);
int dlgIsDialogActive();
STRING* dlgGetCurrentSpeaker();
void dlg_resize();
XMLPAR* getDialogItemById(XMLPAR *_myXML, int _id);
int getDialogItemId(XMLPAR* _XMLItem);
int dlgStart(STRING* _dialogFile);
void dlgStart(STRING* _speaker, STRING* _text, SOUND* _audio);

#include "dialogs.c"
#endif