#include "units.h"
#include "macros.h"
#include "fireTor.h"
#include "printStuff.h"
#include "setOpts.h"

//---------------------------------------- prototypes ----------------------------------------------
void useTurbo(void);
void repairShip(void);
void useSonar(void);
void useAOE(void);void checkIfDetected(const int i, const int j);
void shipDetected(SHIP *const ship);

//---------------------------------------- code ---------------------------------------------------


void useSonar(void) {
	wattron(opt_win,COLOR_PAIR(RED_COLOR));
	printToOptWin(2,2,"x");
	wattroff(opt_win,COLOR_PAIR(RED_COLOR));
	displaySonar();
	shipDetected(player_sub);
	printToOptWin(2,2," ");
}

void repairShip(void) {

}

void useTurbo(void) {
	if(player_sub->turbo == 1) {
		player_sub->ap += 3;
		player_sub->turbo = 0;
		wattron(opt_win,COLOR_PAIR(RED_COLOR));
		printToOptWin(2,0,"x");
		wattroff(opt_win,COLOR_PAIR(RED_COLOR));
		updateAPDisplay();
	}
	else {
		printToTxtScr(0,0,"sorry, but you have already used turbo.");
		getch();
	}	
}

void useAOE(void) {
	if(PLAYER->using_aoe == 1) {
		PLAYER->using_aoe = 0;
		printToOptWin(2,1," ");
	}
	else if(player_sub->ap < 2) {
		printToTxtScr(0,0,"sorry, but you dont have enough AP.");
		getch();
	}
	else if(PLAYER->num_aoetor) {
		PLAYER->using_aoe = 1;
		wattron(opt_win,COLOR_PAIR(RED_COLOR));
		printToOptWin(2,1,"x");
		wattroff(opt_win,COLOR_PAIR(RED_COLOR));
		setTorpedoFireLine();
	}
	else {
		printToTxtScr(0,0,"sorry, but you are out of AOE torpedoes.");
		getch();
	}
}

//if sonar detects a submarine
void shipDetected(SHIP *const ship) {
	ship->last_knownx = ship->x;
	ship->last_knowny = ship->y;
	ship->last_knownz = ship->z;
	ship->ship->sub->last_detected = 0;
	ship->detected = 1;
	(ship != player_sub)? printLastDetected(ship),printToTxtScr(0,0,"enemy ship located"): printToTxtScr(0,0,"enemy has detected you");
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