#ifndef LEVEL_TRANSITION_H
#define LEVEL_TRANSITION_H

var level_change_transition_progress = 0;
var gate_entry_id = -1;
var input_fetch = 1;

TEXT* txt_level_wmbs =
{
	string("ruin.wmb","ruin_indoor.wmb");
}

var level_change_transition(var in);

void level_change(var level_id, var gate_id);

//skill1: blue 0
//skill2: green 0
//skill3: red 0
//skill4: fog_start 0
//skill5: fog_end 0
//skill6: fog_color 0
action sky_color_fog_act();

//skill1: this_id 0
//skill2: to_id 0
action level_gate();

void level_change_set_player_position(ENTITY* ent_pl);

#include "level_transition.c"
#endif