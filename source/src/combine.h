#ifndef COMBINATION_H
#define COMBINATION_H

typedef struct combination
{
	int id1;
	int id2;
	int morphtargetId;
	int resultId;
	
}COMBINATION


int COMBINATION_combine(int id1, int id2, int* morphtargetId);

int COMBINATION_load(STRING* file);
void COMBINATION_close();

#endif