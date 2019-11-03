// Submarine game
//author: github/return5
//license: GPL 2.0
//version: 0.7
//currently a work in progress.
//turned based game. player controls a submarine and is in a fight against an enemy submarine and an enemy destroyer.  also includes a cargo ship which can be destroyed for extra points


//---------------------------------------- headers ------------------------------------------------
#include <stdlib.h>
#include "units.h"
#include "macros.h"
#include "fireTor.h"
#include "printStuff.h"
#include "setOpts.h"
#include "createPieces.h"
#include "makeWin.h"

//---------------------------------------- prototypes ----------------------------------------------

int getDirection(void);
void movePlayer(void);
int moveShip(SHIP *const ship, const int new_x, const int new_y, const int new_z);

void gameLoop(void);
void exitGame(void);
void playerTurn(void);

//---------------------------------------- typedefs,enums,consts ----------------------------------


//----------------------------------------  global vars -------------------------------------------
static int play = 1;

//---------------------------------------- code ---------------------------------------------------

//checks if ship is at boundary, and if not, moves ship to new location, else returns -1
int moveShip(SHIP *const ship, const int new_x, const int new_y, const int new_z) {
	if( new_z < 0 || new_z > 4) {
		return -1;
	}
	else if(new_x < 0 || new_x >= X_EDGE) {
		return -1;
	}
	else if (new_y < 0 || new_y >= Y_EDGE) {
		return -1;
	}
	else {
		ship->x = new_x;
		ship->y = new_y;
		ship->z = new_z;
		return 0;
	}
}

//gets user input to move player_sub one space in any direction.
int getDirection(void) {
	MEVENT event;
	switch(getch()) {
		case 'q':
			return moveShip(player_sub,player_sub->x,player_sub->y,player_sub->z-1);
		case 'e':
			return moveShip(player_sub,player_sub->x,player_sub->y,player_sub->z+1);
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
			break;
		case 'g':
			setTorpedoFireLine();
			return 1;
			break;
		case KEY_MOUSE:
			if(getmouse(&event) == OK) { 
				if(event.bstate & BUTTON1_CLICKED){ //if left mouse button was clicked
					checkMouseLocation(event.x,event.y);
				}
			}
			return 1;
			break;
		default: 
			printToTxtScr(0,0,"wrong choice, please try again");
			getch();
			return -1;
	}
}

void movePlayer(void) {
	do {
		printToTxtScr(0,0,"'w''a''s''d' keys moves sub forward, back, left right.\n'q' and 'e' moves sub up and down.\n'f' exits menu");
	} while(getDirection() == -1);
	printPieces();
	updateLocationDisplay();
	updateHealthDisplay();
	updateAOETorDisplay();
	updateLastDetected();
}

void playerTurn(void) {
	while(player_sub->ap > 0) {
		movePlayer();
		updateAPDisplay();
	}
	player_sub->ap = 2;
}

void gameLoop(void) {
	while(play == 1) {
		while(player_sub->ap > 0) {
			updateAPDisplay();
			playerTurn();
		}
	}
}

//when the game is over.
void exitGame(void) {
	endwin();
}

int main(void) {	
	makeWin();
	createPieces();
	printPieces();
	printOptWin();
	updateLocationDisplay();
	updateHealthDisplay();
	updateAOETorDisplay();
	updateLastDetected();
	updateAPDisplay();
	gameLoop();
	exitGame();
	return EXIT_SUCCESS;
}