//---------------------------------------- headers ------------------------------------------------

#include "macros.h"
#include "fireTor.h"
#include "printStuff.h"
#include "setOpts.h"
#include <stddef.h>

//---------------------------------------- prototypes ----------------------------------------------
void useTurbo(void);
void repairShip(void);
void useSonar(void);
void useAOE(void);void checkIfDetected(const int i, const int j);
void shipDetected(SHIP *const ship);

//---------------------------------------- code ---------------------------------------------------


void useSonar(void) {
	if(player_sub->ap > 0) {
		printToOptWin(2,2,"x",RED_COLOR);
		displaySonar();
		shipDetected(player_sub);
		printToOptWin(2,2," ",0);
		player_sub->ap--;
	}
	else {
		printToTxtScr(0,0,"sorry, but you dont have enough ap");
	}
}

void repairShip(void) {
	if(player_sub->ap >= 2) {
		player_sub->health  += 15;
		if(player_sub->health > 100) {
			player_sub->health = 100;
		}
	}
	else {
		printToTxtScr(0,0,"sorry, but you dont have enough ap.");
	}

}

void useTurbo(void) {
	if(player_sub->turbo == 1) {
		player_sub->ap += 3;
		player_sub->turbo = 0;
		printToOptWin(2,0,"x",RED_COLOR);
	}
	else {
		printToTxtScr(0,0,"sorry, but you have already used turbo.");
	}	
}

void useAOE(void) {
	if(PLAYER->using_aoe == 1) {
		PLAYER->using_aoe = 0;
		printToOptWin(2,1," ",0);
	}
	else if(player_sub->ap < 2) {
		printToTxtScr(0,0,"sorry, but you dont have enough AP.");
	}
	else if(PLAYER->num_aoetor) {
		PLAYER->using_aoe = 1;
		printToOptWin(2,1,"x",RED_COLOR);
		setTorpedoFireLine();
	}
	else {
		printToTxtScr(0,0,"sorry, but you are out of AOE torpedoes.");
	}
}

//if sonar detects a submarine
void shipDetected(SHIP *const ship) {
	ship->last_knownx = ship->x;
	ship->last_knowny = ship->y;
	ship->last_knownz = ship->z;
	ship->ship->sub->last_detected = 0;
	ship->detected = 1;
	if (ship != player_sub) {
		printToTxtScr(0,0,"enemy ship located");
		printLastDetected(ship);
	}
	else {
		printToTxtScr(0,0,"enemy has detected you");
	}
}


//checks to see if enemy sub has been found with sonar.
void checkIfDetected(const int i, const int j) {
	ENEMIES *head = enemies;
	while(head != NULL) {
		if(head->ship->type == SUBMARINE) { //only looking for subs. 
			if(player_sub->x + j == head->ship->x && head->ship->y == player_sub->y - i) {
				shipDetected(head->ship);
			}
			else if(player_sub->x - j == head->ship->x && head->ship->y == player_sub->y - i) {
				shipDetected(head->ship);			
			}
			else if(player_sub->x + j == head->ship->x && head->ship->y == player_sub->y + i) {
				shipDetected(head->ship);			
			}
			else if(player_sub->x - j == head->ship->x && head->ship->y == player_sub->y + i) {
				shipDetected(head->ship);
			}
		}
		head = head->next;
	}
}

//checks where mouse was when it was clicked. if clicked on opt boxes, then do those things 
void checkMouseLocation(const int x, const int y) {
	if(x == 80) {
		switch(y) {
		case 22: useTurbo();
			break;
		case 23: useAOE();
			break;
		case 24: useSonar();
			break;
		case 25: repairShip();
			break;
		default: //do nothing
			break;		
		}
	}
}