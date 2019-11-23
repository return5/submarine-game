//---------------------------------------- headers ------------------------------------------------
#include "printStuff.h"
#include "macros.h"
#include "computerturn.h"
#include "dectreefuncs.h"
#include "movement.h"
#include "windows.h"
#include <stdlib.h>

//---------------------------------------- prototypes ----------------------------------------------
static int getRandom(const int start, const int end);

//---------------------------------------- typedefs,enums,consts ----------------------------------


//----------------------------------------  global vars -------------------------------------------

//---------------------------------------- code ---------------------------------------------------

//random num in the range of start - (end -1)
static int getRandom(const int start, const int end) {
	return (rand() % (end - start)) + start;
}


void moveCompShip(void *ship, void *dec) {
	if(dec == NULL) {
		//do nothing
	}
	int x,y,z;
	SHIP *cur_ship = (SHIP*)(ship);
	do { 
		x = cur_ship->x;
		y = cur_ship->y;
		z = cur_ship->z;
		switch(getRandom(0,6)) {
			case 0: x -= X_NORM;
				break;
			case 1: x += X_NORM;
				break;
			case 2: y--;
				break;
			case 3: y++;
				break;	
			case 4: z++;
				break;
			case 5: z--;
				break;
			default: x = -100;   //for default get incredibly wrong value. makes moveship always return -1
				break;	
		}
	} while(moveShip(cur_ship,x,y,z) == -1); //movement.c
}

void secondFunc(void *ship, void *dec) {
	((dectree*) dec)->decFunc(ship,((dectree*) dec)->leftdec);
}

void startFunc(void *ship, void *dec) {
	((dectree*) dec)->decFunc(ship,((dectree*) dec)->leftdec);
}

void computerTurn(void) {
	ENEMIES *head = enemies;
	while(head != NULL) {
		while(head->ship->ap > 0) {
			head->ship->dtree->decFunc((void*)head->ship,(void*)head->ship->dtree);
			updateBoard();
		}
		updateAP((void*)head->ship);
		head = head->next;
	}
} 