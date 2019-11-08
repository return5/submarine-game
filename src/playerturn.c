//---------------------------------------- headers ------------------------------------------------
#include "playerturn.h"
#include "macros.h"
#include "printStuff.h"
#include "fireTor.h"
#include "setOpts.h"


//---------------------------------------- prototypes ----------------------------------------------
void setShipAp(void);
int moveShip(const int new_x, const int new_y, const int new_z);
void updateAP(void);
int getUserInput(void);
void movePlayer(void);

//---------------------------------------- code ---------------------------------------------------

void playerTurn(void) {
	while(player_sub->ap > 0) {
		movePlayer();
	}
	getch();
	updateAP();
	updateBoard();
}

//gets user input to move player_sub one space in any direction.
int getUserInput(void) {
	MEVENT event;
	switch(getch()) {
		case 'q':
			if (moveShip(player_sub->x,player_sub->y,player_sub->z-1) == -1) {
				 printToTxtScr(0,0,"you are already at the ocean floor");
				 getch();
				 return -1;
			}
			else {
				return 1;
			}
		case 'e':
			if (moveShip(player_sub->x,player_sub->y,player_sub->z+1) == -1) {
				 printToTxtScr(0,0,"you are already at the surface");
				 getch();
				 return -1;
			}
			else {
				return 1;
			}
		case 'w':
			player_sub->direction_facing = FORWARD; 
			return moveShip(player_sub->x,(player_sub->y)-1,player_sub->z);
		case 'a':
			player_sub->direction_facing = LEFT; 
			return moveShip(player_sub->x - X_NORM,player_sub->y,player_sub->z);
		case 's':
			player_sub->direction_facing = BACK; 
			return moveShip(player_sub->x,player_sub->y+1,player_sub->z);
		case 'd':
			player_sub->direction_facing = RIGHT; 
			return moveShip(player_sub->x + X_NORM,player_sub->y,player_sub->z);
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
	} while(getUserInput() == -1);
	updateBoard();
}

//checks if ship is at boundary, and if not, moves ship to new location, else returns -1
int moveShip(const int new_x, const int new_y, const int new_z) {
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
		player_sub->x = new_x;
		player_sub->y = new_y;
		player_sub->z = new_z;
		setShipAp();
		return 0;
	}
}
void setShipAp(void) {
	//if ship is less that 50 health, then sub 2 ap points
	player_sub->ap -= (player_sub->health < 50) ? 2 : 1;
	//checks if ship ap is less than zero, if it is, then set it to 0;
	player_sub->ap = (player_sub->ap < 0) ? 0 : player_sub->ap;
}

void updateAP(void) {
	player_sub->ap = (player_sub->health < 50 ) ? 1 : 2;
}