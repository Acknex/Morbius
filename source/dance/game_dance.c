#include <acknex.h>
#include <default.c>

TEXT* gdp = {
	string ("LRUD","UUDD");
}

#define GDP_PUZZLE_START_LEN 3

int gdp_state = 0;
// 0 = new
// 1 = get new puzzle
// 2 = initialize current puzzle trunc

int gdp_index = 0;
int gdp_len = GDP_PUZZLE_START_LEN;

int gdp_entered = 0;

int gdp_currkey = 0;

STRING* gdp_puzzle = "#100";
STRING* gdp_current = "#100";

void gdp_reset () {
	gdp_state = 0;
	gdp_index = 0;
	str_cpy(gdp_puzzle, "");
	str_cpy(gdp_current, "");
}

void gdp_ok ()
{
	gdp_state = 4;
}

void gdp_notok ()
{
	gdp_state = 5;
}

void gdp_set_L () {
	on_cul = gdp_ok;
	on_cur = on_cuu = on_cud = gdp_notok;
}

void gdp_set_R () {
	on_cur = gdp_ok;
	on_cul = on_cuu = on_cud = gdp_notok;
}

void gdp_set_U () {
	on_cuu = gdp_ok;
	on_cul = on_cur = on_cud = gdp_notok;
}

void gdp_set_D () {
	on_cud = gdp_ok;
	on_cuu = on_cul = on_cur = gdp_notok;
}

void gdp_setkeys (STRING* current, int entered)
{
	int a = str_getchr(current, entered+1);
	
	switch (a)
	{
		/* L */ case 76: gdp_set_L(); break;
		/* R */ case 82: gdp_set_R(); break;
		/* U */ case 85: gdp_set_U(); break;
		/* D */ case 68: gdp_set_D(); break;
	}
}

void gdp_resetkeys ()
{
	on_cur = on_cul = on_cuu = on_cud = NULL;
}

void gdp_resetlen ()
{
	gdp_len = GDP_PUZZLE_START_LEN;
}

void gdp_do () {

	switch (gdp_state)
	{
		// reset
		case 0: {
			gdp_state++;
		}
		
		// get new puzzle
		case 1: {
			str_cpy(gdp_puzzle, (gdp->pstring)[gdp_index]);
			str_cpy(gdp_current, "");
			gdp_resetlen();
			gdp_state++;
		}
		
		// initialize current puzzle trunc
		case 2: {
			str_cpy(gdp_current, gdp_puzzle);
			gdp_entered = 0;
			gdp_setkeys(gdp_current, gdp_entered);
			gdp_state++;
		}
		
		case 3: {
			draw_text(gdp_current, 0, 100, COLOR_RED);
			break;
		}
		
		// OK
		case 4: {
		
			gdp_resetkeys();
			
			// finished
			if (gdp_entered + 1 == str_len(gdp_current))
			{
				beep();
				gdp_index++;
				gdp_state = 1;
			}
			else // next
			{
				beep();
				gdp_entered++;
				gdp_setkeys(gdp_current, gdp_entered);
				gdp_state = 3;
			}
			
			break;
		}
		
		// NOT-OK
		case 5: {
			gdp_resetkeys();
			gdp_resetlen();
			gdp_state = 2;
			break;
		}
	}	
}

void main()
{
	gdp_reset();
	
	while (1)
	{
		gdp_do ();
		wait(1);
	}
}