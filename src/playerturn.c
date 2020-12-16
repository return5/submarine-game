//---------------------------------------- headers ------------------------------------------------
#include <ncurses.h>
#include "playerturn.h"
#include "units.h"
#include "macros.h"
#include "movement.h"
#include "printStuff.h"
#include "main.h"
#include "setOpts.h"
#include "weapons.h"

//---------------------------------------- prototypes ----------------------------------------------
static int getUserInput(void);
static void movePlayer(void);
static void checkIfWin(void);

//---------------------------------------- code ---------------------------------------------------

//gets user input to move player_sub one space in any direction.
static int getUserInput(void) {
	MEVENT event;
	switch(getch()) {
		case 'q':
			switch (moveShip(player_sub,player_sub->x,player_sub->y,player_sub->z-1)) {
				 case -1: 
					printToTxtScr(0,0,"you are already at the surface");
					getch();
				 	return -1;
				 default: return 1;
			}
		case 'e':
			switch(moveShip(player_sub,player_sub->x,player_sub->y,player_sub->z+1)) {
				case -1:
					printToTxtScr(0,0,"you are already at the ocean floor");
					getch();
					return -1;
				default: return 1;
			}
		case 'w':
			player_sub->direction_facing = FORWARD; 
			return moveShip(player_sub,player_sub->x,(player_sub->y)-1,player_sub->z);
		case 'a':
			player_sub->direction_facing = LEFT; 
			return moveShip(player_sub,player_sub->x - X_NORM,player_sub->y,player_sub->z);
		case 's':
			player_sub->direction_facing = BACK; 
			return moveShip(player_sub,player_sub->x,player_sub->y+1,player_sub->z);
		case 'd':
			player_sub->direction_facing = RIGHT; 
			return moveShip(player_sub,player_sub->x + X_NORM,player_sub->y,player_sub->z);
		case 'f': return 1;  //exit menu when f key is pressed
		case 'g':
			setTorpedoFireLine();
			return 1;
		case KEY_MOUSE:
			switch(getmouse(&event)) { 
				case OK:
					if(event.bstate & BUTTON1_CLICKED){ //if left mouse button was clicked
						checkMouseLocation(event.x,event.y);  //setOpts.c
					}
					break;
				default: //do nothing
					break;
			}
			return 1;
		case 'j' :
			printToTxtScr(0,0,"exit game(y/n)?");
			switch(getch()) {
				case 'Y':
				case 'y': 
					play = 0;
					return 1;
				default: return -1;
			}
			return -1;
		default: 
			printToTxtScr(0,0,"wrong choice, please try again");
			getch();
			return -1;
	}
}

static void movePlayer(void) {
	do {
		printToTxtScr(0,0,"'w''a''s''d' keys moves sub forward, back, left right.\n'q' and 'e' moves sub up and down.\n'f' exits menu");
	} while(getUserInput() == -1);
}

static void checkIfWin(void) {
	if(num_enemies == 0) {
		play = 0;       //sets play to 0 if all enemies have been destroyed
		printToTxtScr(0,0,"congrats, you won.");
		getch();
	}
}

void playerTurn(void) {
	checkIfWin();
	resetAP(player_sub);   //movement.c
	updateBoard();            //printstuff.c
	updateLastDetected();     //printstuff.c 
	while(player_sub->ap > 0 && play == 1) {
		movePlayer();
		setShipAp(player_sub);    //movement.c
		updateBoard();            //printstuff.c
	}
}
