// Submarine game
//author: github/return5
//license: GPL 2.0
//version: 0.3
//currently a work in progress.
//turned based game. player controls a submarine and is in a fight against an enemy submarine and an enemy destroyer.  also includes a cargo ship which can be destroyed for extra points


//---------------------------------------- headers ------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include "units.h"
#include <unistd.h>
//---------------------------------------- prototypes ----------------------------------------------
int getRandom(const int start, const int end);
void checkIfTargetHitX(SHIP *const target, const int start, const int end);
void checkIfTargetHitY(SHIP *const target, const int start, const int end);
void getSingleTargetX(SHIP **targets, const int index,const int abs,const int limit);
void getSingleTargetY(SHIP **targets, const int index,const int abs,const int limit);
void fireTorpedoForward(const int limit);
void fireTorpedoBack(const int limit);
void fireTorpedoLeft(const int limit);
void fireTorpedoRight(const int limit);
void fireTorpedo(const int limit);
void confirmFireTorpedo(const int limit);
int torpedoFireLineRight(void);
int torpedoFireLineLeft(void);
int torpedoFireLineDown(void);
int torpedoFireLineUp(void);
void setTorpedoFireLine(void);
void updateDirection(SHIP *const const ship, const int new_x, const int new_y);
void updateAPDisplay(void);
void updateLastDetected(void);
void updateAOETorDisplay(void);
void updateHealthDisplay(void);
void updateLocationDisplay(void);
void printToMain(const int x, const int y, const char *const str);
void printToTxtScr(const int x, const int y, const char *const str);
int moveShip(SHIP *const ship, const int new_x, const int new_y, const int new_z);
int getDirection(int *const direction);
void movePlayer(void);
void makeBoat(SHIP *const ship);
void makeSub(SHIP *const ship);
void makeShip(SHIP *const ship,const int x_start, const int y_start, const int x_end, const int y_end,enum TYPE type);
void checkPiecesLocation(void);
void createPieces(void);
void createWindows(void);
void initColors(void);
int initScreen(void);
void gameLoop(void);
void exitGame(void);
void printPieces(void);
void printPlayerSub(void);
int checkZ(const SHIP *const ship);
void targetDestroyed(SHIP *const target);
void playerTurn(void);
void printOptWin(void);
void checkMouseLocation(const int x, const int y);
void useTurbo(void);
void repairShip(void);
void useSonar(void);
void displayAOE(int const x, int const y);
void useAOE(void);

//---------------------------------------- typedefs,enums,consts ----------------------------------
#define Y_EDGE 25
#define X_EDGE 75
#define X_NORM 2  //value to make spacing of x coordinate look liek spacing of y coordinate
#define SHIP_1 enemies->ship
#define SHIP_2 enemies->next->ship
#define SHIP_3 enemies->next->next->ship
#define PLAYER player_sub->ship->sub
#define TORPEDODISTANCE 10 //distance a torpedo can fire.

enum DIRECTION {UP,DOWN,FORWARD,BACK,LEFT,RIGHT};

typedef struct ENEMIES { //used to make linked list to hold enemy ships
	SHIP *ship;
	struct ENEMIES *next;
}ENEMIES;

//----------------------------------------  global vars -------------------------------------------
static WINDOW *main_win,*opt_win,*status_win,*text_win;
static SHIP *player_sub; //player controlled submarine
static ENEMIES *enemies; //holds enemy ships
static int play = 1;
static int has_detected = 0; //when player is first detected it flips to one

//---------------------------------------- code ---------------------------------------------------


int getRandom(const int start, const int end) {
	return (rand() % (end - start)) + start;
}

void targetDestroyed(SHIP *const target) {
	switch(target->type) {
		case SUBMARINE: printToTxtScr(0,0,"congragulations, you destroyed enemy sub.");
			break;
		case SURFACESHIP: printToTxtScr(0,0,"congragulations, you destroyed enemy surfaceship.");
			break;
		default: printToTxtScr(0,0,"congragulations, you destroyed enemy cargo ship.");
			break;
	}
	getch();
	target->health = 0;
	target->ap = 0;
}

//if firing along x axis, checks if enemy ship is hit
void checkIfTargetHitX(SHIP *const target, const int start, const int end) {
	if(target->x >= start && target->x <= end) {
		targetDestroyed(target);
	}
	else {
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}
}

//if firing along y axis, checks if an enemy ship was hit
void checkIfTargetHitY(SHIP *const target, const int start, const int end) {
	if(target->y >= start && target->y <= end) {
		targetDestroyed(target);
	}
	else {
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}
}

//sets target to the ship which is closest to player_sub in x direction
void getSingleTargetX(SHIP **targets, const int index,const int abs,const int limit) {
	SHIP *target = targets[0];
	for(int i = 1; i < index; i++) {
		 //if distances between player sub and target[i] is less than distances between target and player sub, set target to targets[i]
		if((targets[i]->x - player_sub->x) * abs < (target->x - player_sub->x) * abs) {
			target = targets[i];
		}
	}
	checkIfTargetHitX(target,((abs == -1)? player_sub->x - limit : player_sub->x + 1),((abs == -1)? player_sub->x - 1 : player_sub->x + limit));
}

//sets target to the ship which is closest to player_sub in y direction
void getSingleTargetY(SHIP **targets, const int index,const int abs,const int limit) {
	SHIP *target = targets[0];
	for(int i = 1; i < index; i++) {
		 //if distances between player sub and target[i] is less than distances between target and player sub, set target to targets[i]
		if((targets[i]->y - player_sub->y) * abs < (target->y - player_sub->y) * abs) {
			target = targets[i];
		}
	}
	checkIfTargetHitY(target,((abs == -1)? player_sub->y - limit : player_sub->y + 1),((abs == -1)? player_sub->y - 1 : player_sub->y + limit));
}

//checks if enemy ship is on the same z or one above/below to player sub
int checkZ(const SHIP *const ship) {
	if(player_sub->z == ship->z) {
		return 1;
	}
	else if(player_sub->z == ship->z+1) {
		return 1;
	}
	else if(player_sub->z == ship->z-1) {
		return 1;	
	}
	else {
		return 0;
	}
}

//checks if any enemy ships are in the same direction and on same line as player_sub is firing
void fireTorpedoRight(const int limit) {
	SHIP *targets[3];
	int index = 0;
	ENEMIES *head = enemies;
	while(head != NULL) {
		if(head->ship->y == player_sub->y && checkZ(head->ship) && head->ship->x > player_sub->x) { //if enemy is to right of player sub
			targets[index++] = head->ship;
		}
		head = head->next;
	}
	if(index > 0) {
		getSingleTargetX(targets,index,1,limit);
	}
	else {
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}
}

//checks if any enemy ships are in the same direction and on same line as player_sub is firing
void fireTorpedoLeft(const int limit) {
	SHIP *targets[3];
	int index = 0;
	ENEMIES *head = enemies;
	while(head != NULL) {
		if(head->ship->y == player_sub->y && checkZ(head->ship) && head->ship->x < player_sub->x) { //if enemy sub is to left of player sub
			targets[index++] = head->ship;
		}
		head = head->next;
	}
	if(index > 0) {
		getSingleTargetX(targets,index,-1,limit);
	}
	else {
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}

}

//checks if any enemy ships are in the same direction and on same line as player_sub is firing
void fireTorpedoBack(const int limit) {
	SHIP *targets[3];
	int index = 0;
	ENEMIES *head = enemies;
	while(head != NULL) {
		if(head->ship->x == player_sub->x && checkZ(head->ship) && head->ship->y > player_sub->y) { //if enemy is below player sub
			targets[index++] = head->ship;
		}
		head = head->next;
	}
	if(index > 0) {
		getSingleTargetY(targets,index,1,limit);
	}
	else {
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}

}

//checks if any enemy ships are in the same direction and on same line as player_sub is firing
void fireTorpedoForward(const int limit) {
	SHIP *targets[3];
	int index = 0;
	ENEMIES *head = enemies;
	while(head != NULL) {
		if(head->ship->x == player_sub->x && checkZ(head->ship) && head->ship->y < player_sub->y) { //if enemy is above player sub
			targets[index++] = head->ship;
		}
		head = head->next;
	}
	if(index > 0) {
		getSingleTargetY(targets,index,-1,limit);
	}
	else {
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}

}

void fireTorpedo(const int limit) {
	switch(player_sub->direction_facing) {
		case FORWARD: fireTorpedoForward(limit);
			break;
		case BACK: fireTorpedoBack(limit);
			break;
		case LEFT: fireTorpedoLeft(limit);
			break;
		case RIGHT: fireTorpedoRight(limit);
			break;
	}
}

void confirmFireTorpedo(const int limit) {
	printToTxtScr(0,0,"press 'g' to fire torpedo.\n'w''a''s''d' to change direction\nany other key cancels order");
	switch(getch()) {
		case 'g':
			if(player_sub->ap > 0) {
				fireTorpedo(limit);
			}
			else {
				printToTxtScr(0,0,"sorry, but you lack suffecient AP");
			}
			break;
		case 'w': 
			player_sub->direction_facing = FORWARD;
			printPieces();
			setTorpedoFireLine();
			break;
		case 'a': 
			player_sub->direction_facing = LEFT;
			printPieces();
			setTorpedoFireLine();
			break; 
		case 's': 
			player_sub->direction_facing = BACK;
			printPieces();
			setTorpedoFireLine();
			break;
		case 'd': 
			player_sub->direction_facing = RIGHT;
			printPieces();
			setTorpedoFireLine();
			break;	
		default: //do nothing
			break;
	}
	wclear(main_win);
	printPieces();
}

void displayAOE(int const x, int const y) {
	int offset = 2;
	for(int i = 0; i < 3; i++) {
		for(int j = offset * X_NORM; j >= 0; j -= X_NORM) {
			printToMain(x+j,y-i,"#");
			printToMain(x-j,y-i,"#");
			printToMain(x+j,y+i,"#");
			printToMain(x-j,y+i,"#");
		}
		offset--;
	}
}

int torpedoFireLineRight(void) {
	const int limit = (player_sub->x < X_EDGE - TORPEDODISTANCE * X_NORM - 1) ? TORPEDODISTANCE * X_NORM : X_EDGE - player_sub->x - 1;
	for(int i = X_NORM; i < limit; i+= X_NORM) {
			printToMain(player_sub->x+i,player_sub->y,"-");
	}
	printToMain(player_sub->x+limit,player_sub->y,">");
	return limit;
}

int torpedoFireLineLeft(void) {
	const int limit = (player_sub->x > TORPEDODISTANCE * X_NORM) ? TORPEDODISTANCE * X_NORM : player_sub->x;
	for(int i = X_NORM; i < limit; i+= X_NORM) {
			printToMain(player_sub->x-i,player_sub->y,"-");
	}
	printToMain(player_sub->x-limit,player_sub->y,"<");
	return limit;
}

int torpedoFireLineDown(void) {
	const int limit = (player_sub->y < Y_EDGE - TORPEDODISTANCE - 1) ? TORPEDODISTANCE : Y_EDGE - player_sub->y - 1;
	for(int i = 1; i < limit; i++) {
		printToMain(player_sub->x,player_sub->y + i,"|");
	}
	printToMain(player_sub->x,player_sub->y+limit,"v");
	return limit;
}

int torpedoFireLineUp(void) {
	const int limit = (player_sub->y > TORPEDODISTANCE) ? TORPEDODISTANCE : player_sub->y;
	for(int i = 1; i < limit; i++) {
		printToMain(player_sub->x,player_sub->y - i,"|");
	}
	printToMain(player_sub->x,player_sub->y-limit,"^");
	return limit;
}

void setTorpedoFireLine(void) {
	int limit;
	wattron(main_win,COLOR_PAIR(1));  //turn color red on
	switch(player_sub->direction_facing) {
		case FORWARD: 
			limit = torpedoFireLineUp();
			if(PLAYER->using_aoe) {
				displayAOE(player_sub->x,player_sub->y - limit);
			}
			break;
		case BACK: 
			limit = torpedoFireLineDown();
			if(PLAYER->using_aoe) {
				displayAOE(player_sub->x,player_sub->y + limit);
			}
			break;
		case LEFT: 
			limit = torpedoFireLineLeft();
			if(PLAYER->using_aoe) {
				displayAOE(player_sub->x - limit, player_sub->y);
			}
			break;
		case RIGHT: 
			limit = torpedoFireLineRight();
			if(PLAYER->using_aoe) {
				displayAOE(player_sub->x + limit, player_sub->y);
			}
			break;
	}
	wattroff(main_win,COLOR_PAIR(1)); //turn color red off
	confirmFireTorpedo(limit);
}

//update the direction in which the ship is facing
void updateDirection(SHIP *const const ship, const int new_x, const int new_y) {
	if(new_x != ship->x) {
		if(new_x > ship->x) {
			ship->direction_facing = RIGHT;
		}
		else {
			ship->direction_facing = LEFT;
		}
	}
	else if(new_y != ship->y) {
		if(new_y > ship->y) {
			ship->direction_facing = BACK;
		}
		else {
			ship->direction_facing = FORWARD;
		}
	}
}

//updates the display to show ammount of action points you have left
void updateAPDisplay(void) {
	mvwprintw(status_win,4,0,"AP points: %d ",player_sub->ap);
	wrefresh(status_win);
}

//update the display to show how many turns since last detected
void updateLastDetected(void) {
	if(has_detected) {
		PLAYER->last_detected++;
	}
	mvwprintw(status_win,3,0,"detected: %d ",PLAYER->last_detected);
	wrefresh(status_win);
}

//updates the display to show how many AOE torpedoes you have left
void updateAOETorDisplay(void) {
	mvwprintw(status_win,2,0,"AOE Tor: %d",PLAYER->num_aoetor);
	wrefresh(status_win);
}

//update the display to show current health
void updateHealthDisplay(void) {
	mvwprintw(status_win,1,0,"Health: %d  ",player_sub->health);
	wrefresh(status_win);
}

//updates display to show current location as x y z
void updateLocationDisplay(void) {
	mvwprintw(status_win,0,10,"          ");  //blank line to clear line 0,0
	mvwprintw(status_win,0,0,"Locations: %d %d %d",(player_sub->x / X_NORM),player_sub->y,player_sub->z);
	wrefresh(status_win);
}

void printToOptWin(const int x, const int y, const char *const str) {
	mvwprintw(opt_win,y,x,"%s",str);
	wrefresh(opt_win);
}

//print str to main window at x,y
void printToMain(const int x, const int y, const char *const str) {
	mvwprintw(main_win,y,x,"%s",str);
	wrefresh(main_win);
}

//print str to text_win at location x,y
void printToTxtScr(const int x, const int y, const char *const str) {
	wclear(text_win);
	mvwprintw(text_win,y,x,"%s",str);
	wrefresh(text_win);
}

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
		updateDirection(ship,new_x,new_y);
		player_sub->x = new_x;
		player_sub->y = new_y;
		player_sub->z = new_z;
		return 0;
	}
}

void useTurbo(void) {
	if(player_sub->turbo == 1) {
		player_sub->ap += 2;
		player_sub->turbo = 0;
		wattron(opt_win,COLOR_PAIR(1));
		printToOptWin(2,0,"x");
		wattroff(opt_win,COLOR_PAIR(1));
		updateAPDisplay();
	}
	else {
		printToTxtScr(0,0,"sorry, but you have already used turbo.");
		getch();
	}	
}

void useAOE(void) {
	if(PLAYER->num_aoetor) {
		PLAYER->using_aoe = 1;
		wattron(opt_win,COLOR_PAIR(1));
		printToOptWin(2,1,"x");
		wattroff(opt_win,COLOR_PAIR(1));
	}
	else {
		printToTxtScr(0,0,"sorry, but you are out of AOE torpedoes.");
		getch();
	}
}

void useSonar(void) {

}

void repairShip(void) {

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

//gets user input to move player_sub one space in any direction.
int getDirection(int *const direction) {
	MEVENT event;
	switch(getch()) {
		case 'q':
			*direction = UP; 
			return moveShip(player_sub,player_sub->x,player_sub->y,player_sub->z-1);
		case 'e':
			*direction = DOWN; 
			return moveShip(player_sub,player_sub->x,player_sub->y,player_sub->z+1);
		case 'w':
			*direction = FORWARD; 
			return moveShip(player_sub,player_sub->x,(player_sub->y)-1,player_sub->z);
		case 'a':
			*direction = LEFT; 
			return moveShip(player_sub,player_sub->x - X_NORM,player_sub->y,player_sub->z);
		case 's':
			*direction = BACK; 
			return moveShip(player_sub,player_sub->x,player_sub->y+1,player_sub->z);
		case 'd':
			*direction = RIGHT; 
			return moveShip(player_sub,player_sub->x + X_NORM,player_sub->y,player_sub->z);
		case 'f': return 1;  //exit menu when f key is pressed
			break;
		case 'g':
			setTorpedoFireLine();
			return -1;
			break;
		case KEY_MOUSE:
			if(getmouse(&event) == OK) { 
				if(event.bstate & BUTTON1_CLICKED){ //if left mouse button was clicked
					checkMouseLocation(event.x,event.y);
				}
			}
			break;
		default: 
			printToTxtScr(0,0,"wrong choice, please try again");
			getch();
			return -1;
	}
}

void movePlayer(void) {
	int direction;
	do {
		printToTxtScr(0,0,"'w''a''s''d' keys moves sub forward, back, left right.\n'q' and 'e' moves sub up and down.\n'f' exits menu");
	} while(getDirection(&direction) == -1);
	printPieces();
	updateLocationDisplay();
	updateHealthDisplay();
	updateAOETorDisplay();
	updateLastDetected();
}

void playerTurn(void) {
	movePlayer();
}

void printOptWin(void) {
	mvwprintw(opt_win,0,1,"[ ]  turbo");
	mvwprintw(opt_win,1,1,"[ ]  AOE tor");
	mvwprintw(opt_win,2,1,"[ ]  sonar");
	mvwprintw(opt_win,3,1,"[ ]  repair");
	wrefresh(opt_win);
}

void printPlayerSub(void) {
	mvwprintw(main_win,player_sub->y,player_sub->x,"x");
	wrefresh(main_win);
}

void makeBoat(SHIP *const ship) {
	#define SUR_SHIP ship->ship->surface
	SUR_SHIP = malloc(sizeof(SURFACE));
	if(ship->type == SURFACESHIP) { //if ship is surface ship and not a cargo ship
		SUR_SHIP->num_charges = 25;  //number of depth charges left
		SUR_SHIP->charges_dropped = 0;  //number of depth charges dropped at enemy
		SUR_SHIP->shots_fired = 0;  //number of cannon shots fired at enemy.
	}
	ship->z = 0;  // z=0 which is surface of water
}

void makeSub(SHIP *const ship) {
	#define SUB_SHIP ship->ship->sub
	SUB_SHIP = malloc(sizeof(SUB));
	SUB_SHIP->num_aoetor = 2; //area of effect torpedoes is 2
	SUB_SHIP->last_detected = -1;  //init to neg number
	ship->z = getRandom(1,5);  //set z to random num between 1 and 4
}

void makeShip(SHIP *const ship,const int x_start, const int y_start, const int x_end, const int y_end,enum TYPE type){
	ship->health = 100;
	ship->last_knownx = 0;
	ship->last_knowny = 0;
	do {
		ship->x = getRandom(x_start,x_end);  //set x to random location between x_start adn x_end
	} while(!(ship->x % X_NORM));  //make sure x location is divisable by 2. 
	ship->y = getRandom(y_start,y_end);  //set y to random location between y_start adn y_end
	ship->ship = malloc(sizeof(SHIPS));
	ship->type = type;
	ship->turbo = 1;
	ship->ap = 2;
	(type == SUBMARINE)? makeSub(ship) : makeBoat(ship);
}

//checks to makes sure enemy boat and cargo ship arnt at the same location. if they are gets new random location for both
void checkPiecesLocation(void) {
	while((SHIP_3->x == SHIP_2->x) && (SHIP_3->y == SHIP_2->y)) {
		SHIP_3->x = getRandom(X_EDGE/2,X_EDGE-1);
		SHIP_3->y = getRandom(0,Y_EDGE-1);
		SHIP_2->x = getRandom(0,X_EDGE-1);
		SHIP_2->y = getRandom(0,Y_EDGE-1);
	}
}

void createPieces(void) {
	const int size_ship = sizeof(SHIP);
	const int size_enemies = sizeof(ENEMIES);
	player_sub = malloc(size_ship);
	makeShip(player_sub,0,0,(X_EDGE/2)-5,(Y_EDGE/2)-5,SUBMARINE);
	enemies = malloc(size_enemies);
	enemies->ship = malloc(size_ship);
	makeShip(enemies->ship,(X_EDGE/2)+5,(Y_EDGE/2)+5,X_EDGE-1,Y_EDGE-1,SUBMARINE);
	enemies->next = malloc(size_enemies);
	enemies->next->ship = malloc(size_ship);
	makeShip(enemies->next->ship,0,0,X_EDGE-1,Y_EDGE-1,SURFACESHIP);
	enemies->next->next = malloc(size_enemies);
	enemies->next->next->ship = malloc(size_ship);
	makeShip(enemies->next->next->ship,X_EDGE/2,0,X_EDGE-1,Y_EDGE-1,CARGOSHIP);
}

void createWindows(void) {
	#define OPTW 15  //opt window width
	#define OPTH 4 //opt window height
	#define STATW 19 //status window width
	#define STATH 5 //status window height
	#define TEXTH 3 //text window height
	#define TEXTW X_EDGE //text window width

	WINDOW *game_border,*status_border,*opt_border,*text_border;
	main_win = newwin(Y_EDGE,X_EDGE,1,1);
	opt_win = newwin(OPTH,OPTW,Y_EDGE-OPTH+1,X_EDGE+3);
	status_win = newwin(STATH,STATW,1,X_EDGE+3);
	text_win = newwin(TEXTH,TEXTW,Y_EDGE+3,1);
	game_border = newwin(Y_EDGE+2,X_EDGE+2,0,0);
	wborder(game_border,'|','|','-', '-', '+', '+', '+', '+');
	status_border = newwin(STATH+2,STATW+2,0,X_EDGE+2);
	wborder(status_border,'|','|','-', '-', '+', '+', '+', '+');
	opt_border = newwin(OPTH+2,OPTW+2,Y_EDGE-OPTH,X_EDGE+2);
	wborder(opt_border,'|','|','-', '-', '+', '+', '+', '+');
	text_border = newwin(TEXTH+2,TEXTW+2,Y_EDGE+2,0);
	wborder(text_border,'|','|','-', '-', '+', '+', '+', '+');
	wrefresh(game_border);
	wrefresh(status_border);
	wrefresh(opt_border);
	wrefresh(text_border);
	wrefresh(main_win);
}

void initColors(void) {
	start_color();
	init_pair(1,COLOR_RED,COLOR_BLACK);
}

//initialize ncurses
int initScreen(void) {
	//(!has_colors()) {  //if terminal can not display color return -1
//		return -1;
//	}
	start_color();
	initscr();        //start ncurses
	noecho();	      //dont display key strokes
	cbreak();	     //disable line buffering
	curs_set(0);    //hide cursor
	keypad(stdscr,TRUE);  //enable keypad, needed for mouse.
	mousemask(ALL_MOUSE_EVENTS,NULL); //enable all mouse events
	refresh();
	return 1;
}

void gameLoop(void) {
	while(play == 1) {
		while(player_sub->ap > 0) {
			playerTurn();
		}
	}
}

//when the game is over.
void exitGame(void) {
	endwin();
}

void printPieces(void) {
	wclear(main_win);
	mvwprintw(main_win,player_sub->y,player_sub->x,"X");
	ENEMIES *head = enemies;
	char c[2];
	c[1] = '\n';
	while(head != NULL) {
		switch(head->ship->type) {
		case SUBMARINE: c[0] = 'Y';
			break;
		case SURFACESHIP: c[0] ='Z';
			break;
		default:c[0] = 'C';
			break;			
		}
		mvwprintw(main_win,head->ship->y,head->ship->x,"%s",c);
		head = head->next;
	}
	wrefresh(main_win);

}

int main(void) {
	srand(time(0));
	if(initScreen()) {
		initColors();
		createWindows();
		createPieces();
		checkPiecesLocation();
		printPieces();
		printOptWin();
		updateLocationDisplay();
		updateHealthDisplay();
		updateAOETorDisplay();
		updateLastDetected();
		updateAPDisplay();
		gameLoop();
		exitGame();
	}
	else {
		printf("sorry, but your current terminal doesnt support colors.\n");
	}
	return EXIT_SUCCESS;
}
