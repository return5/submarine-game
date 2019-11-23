//---------------------------------------- headers ------------------------------------------------
#include "playerturn.h"
#include "macros.h"
#include "printStuff.h"
#include "fireTor.h"
#include "setOpts.h"
#include <ncurses.h>
#include "movement.h"
//---------------------------------------- prototypes ----------------------------------------------
int getUserInput(void);
void movePlayer(void);

//---------------------------------------- code ---------------------------------------------------

//gets user input to move player_sub one space in any direction.
int getUserInput(void) {
	MEVENT event;
	switch(getch()) {
		case 'q':
			if (moveShip((void*)player_sub,player_sub->x,player_sub->y,player_sub->z-1) == -1) {
				 printToTxtScr(0,0,"you are already at the ocean floor");
				 getch();
				 return -1;
			}
			else {
				return 1;
			}
		case 'e':
			if (moveShip((void*)player_sub,player_sub->x,player_sub->y,player_sub->z+1) == -1) {
				 printToTxtScr(0,0,"you are already at the surface");
				 getch();
				 return -1;
			}
			else {
				return 1;
			}
		case 'w':
			player_sub->direction_facing = FORWARD; 
			return moveShip((void*)player_sub,player_sub->x,(player_sub->y)-1,player_sub->z);
		case 'a':
			player_sub->direction_facing = LEFT; 
			return moveShip((void*)player_sub,player_sub->x - X_NORM,player_sub->y,player_sub->z);
		case 's':
			player_sub->direction_facing = BACK; 
			return moveShip((void*)player_sub,player_sub->x,player_sub->y+1,player_sub->z);
		case 'd':
			player_sub->direction_facing = RIGHT; 
			return moveShip((void*)player_sub,player_sub->x + X_NORM,player_sub->y,player_sub->z);
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

void playerTurn(void) {
	while(player_sub->ap > 0) {
		movePlayer();
	}
	getch();
	updateBoard();
	updateAP((void*)player_sub);
}