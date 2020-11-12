//---------------------------------------- headers ------------------------------------------------
#include <stddef.h>
#include "units.h"
#include "macros.h"
#include "setOpts.h"

//---------------------------------------- prototypes ----------------------------------------------
static void useSonar(void);

//---------------------------------------- code ---------------------------------------------------


static void useSonar(void) {
	if(player_sub->ap > 0) {
		printToOptWin(2,2,"x",RED_COLOR);      //printStuff.c
		displaySonar(player_sub);             //printStuff.c
		shipDetected(player_sub);			 //weapons.c
		checkIfDetected(player_sub);              //weapons.c
		printToOptWin(2,2," ",0);          //printStuff.c
		player_sub->ap--;
	}
	else {
		printToTxtScr(0,0,"sorry, but you dont have enough ap");  //printStuff.c
		getch();
	}
}

void repairShip(void) {
	if(player_sub->ap > 0) {
		player_sub->health  = (player_sub->health > 74) ? 100 : player_sub->health + 25;
		printToTxtScr(0,0,"you spend your turn repairing your ship for 25 points");
		getch();
		player_sub->ap = 0;
	}
	else {
		printToTxtScr(0,0,"sorry, but you dont have enough ap.");  //printStuff.c
		getch();
	}
}

void useTurbo(void) {
	if(player_sub->turbo == 1) {
		player_sub->ap += 3;
		player_sub->turbo = 0;
		printToOptWin(2,0,"x",RED_COLOR);  //printStuff.c
	}
	else {
		printToTxtScr(0,0,"sorry, but you have already used turbo.");  //printStuff.c
		getch();
	}	
}

void useAOE(void) {
	if(PLAYER->using_aoe == 1) {
		PLAYER->using_aoe = 0;
		printToOptWin(2,1," ",0);  //printStuff.c
	}
	else if(player_sub->ap < 2) {
		printToTxtScr(0,0,"sorry, but you dont have enough AP.");  //printStuff.c
		getch();
	}
	else if(PLAYER->num_aoetor) {
		PLAYER->using_aoe = 1;
		printToOptWin(2,1,"x",RED_COLOR);  //printStuff.c
		setTorpedoFireLine();  
	}
	else {
		printToTxtScr(0,0,"sorry, but you are out of AOE torpedoes.");  //printStuff.c
		getch();
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
