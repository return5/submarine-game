// Submarine game
//author: github/return5
//license: GPL 2.0
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
int initScreen(void);
void gameLoop(void);
void exitGame(void);
void createPieces(void);
void createWindows(void);
void makeShip(SHIP *const ship,const int x_start, const int y_start, const int x_end, const int y_end,enum TYPE type);
void makeSub(SHIP *const ship);
void makeBoat(SHIP *const ship);
int getRandom(const int start, const int end);
void checkPiecesLocation(void);
void movePlayer(void);
void printPieces(void);
int moveShip(SHIP *const ship,const int new_x, const int new_y, const int new_z);
void printToTxtScr(const int x, const int y, const char *const str);
void updateLocationDisplay(void);
void updateAOETorDisplay(void);
void updateLastDetected(void);
void updateAPDisplay(void);
void updateDirection(SHIP *const ship, const int new_x, const int new_y);

//---------------------------------------- typedefs,enums,consts ----------------------------------
#define Y_EDGE 25
#define X_EDGE 75
#define SUB_SHIP ship->ship->sub
#define SUR_SHIP ship->ship->surface
#define PLAYER player_sub->ship->sub
#define ENEMY_SUB enemy_sub->SUB_SHIP
#define ENEMY_SUR enemy_boat->SUR_SHIP
enum DIRECTION {UP,DOWN,FORWARD,BACK,LEFT,RIGHT};

//----------------------------------------  global vars -------------------------------------------
	static WINDOW *main_win,*opt_win,*status_win,*text_win;
	SHIP *player_sub,*enemy_sub,*enemy_boat,*cargo_ship;
	static int play = 1;
	static int has_detected = 0; //when player is first detected it flips to one


//---------------------------------------- code ---------------------------------------------------


int getRandom(const int start, const int end) {
	return (rand() % (end - start)) + start;
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
	mvwprintw(status_win,0,0,"                   ");  //blank line to clear line 0,0
	mvwprintw(status_win,0,0,"Locations: %d %d %d",player_sub->x,player_sub->y,player_sub->z);
	wrefresh(status_win);
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

//gets user input to move player_sub one space in any direction.
int getDirection(int *const direction) {
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
			return moveShip(player_sub,player_sub->x-1,player_sub->y,player_sub->z);
		case 's':
			*direction = BACK; 
			return moveShip(player_sub,player_sub->x,player_sub->y+1,player_sub->z);
		case 'd':
			*direction = RIGHT; 
			return moveShip(player_sub,player_sub->x+1,player_sub->y,player_sub->z);
		case 'f': return 1;  //exit menu when f key is pressed
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

void makeBoat(SHIP *const ship) {
	SUR_SHIP = malloc(sizeof(SURFACE));
	if(ship->type == SURFACESHIP) { //if ship is surface ship and not a cargo ship
		SUR_SHIP->num_charges = 25;  //number of depth charges left
		SUR_SHIP->charges_dropped = 0;  //number of depth charges dropped at enemy
		SUR_SHIP->shots_fired = 0;  //number of cannon shots fired at enemy.
	}
	ship->z = 0;  // z=0 which is surface of water
}

void makeSub(SHIP *const ship) {
	SUB_SHIP = malloc(sizeof(SUB));
	SUB_SHIP->num_aoetor = 2; //area of effect torpedoes is 2
	SUB_SHIP->last_detected = -1;  //init to neg number
	ship->z = getRandom(1,5);  //set z to random num between 1 and 4
}

void makeShip(SHIP *const ship,const int x_start, const int y_start, const int x_end, const int y_end,enum TYPE type){
	ship->health = 100;
	ship->last_knownx = 0;
	ship->last_knowny = 0;
	ship->x = getRandom(x_start,x_end);  //set x to random location between x_start adn x_end
	ship->y = getRandom(y_start,y_end);  //set y to random location between y_start adn y_end
	ship->ship = malloc(sizeof(SHIPS));
	ship->type = type;
	ship->turbo = 1;
	ship->ap = 2;
	if(type == SUBMARINE) {
		makeSub(ship);
	}
	else {
		makeBoat(ship);
	}

}

//checks to makes sure enemy boat and cargo ship arnt at the same location. if they are gets new random location for both
void checkPiecesLocation(void) {
	while((cargo_ship->x == enemy_boat->x) && (cargo_ship->y == enemy_boat->y)) {
		cargo_ship->x = getRandom(X_EDGE/2,X_EDGE-1);
		cargo_ship->y = getRandom(0,Y_EDGE-1);
		enemy_boat->x = getRandom(0,X_EDGE-1);
		enemy_boat->y = getRandom(0,Y_EDGE-1);
	}
}

void createPieces(void) {
	const int size_ship = sizeof(SHIP);
	player_sub = malloc(size_ship);
	makeShip(player_sub,0,0,(X_EDGE/2)-5,(Y_EDGE/2)-5,SUBMARINE);
	enemy_sub = malloc(size_ship);
	makeShip(enemy_sub,(X_EDGE/2)+5,(Y_EDGE/2)+5,X_EDGE-1,Y_EDGE-1,SUBMARINE);
	enemy_boat = malloc(size_ship);
	makeShip(enemy_boat,0,0,X_EDGE-1,Y_EDGE-1,SURFACESHIP);
	cargo_ship = malloc(size_ship);
	makeShip(cargo_ship,X_EDGE/2,0,X_EDGE-1,Y_EDGE-1,CARGOSHIP);
}

void createWindows(void) {
	#define OPTW 15  //opt window width
	#define OPTH 5 //opt window height
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

//initialize ncurses
int initScreen(void) {
	//(!has_colors()) {  //if terminal can not display color return -1
//		return -1;
//	}
//start_color();
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
		movePlayer();
	}
}

//when the game is over.
void exitGame(void) {
	endwin();
}

void printPieces(void) {
	wclear(main_win);
	mvwprintw(main_win,player_sub->y,player_sub->x,"X");
	mvwprintw(main_win,enemy_sub->y,enemy_sub->x,"Y");
	mvwprintw(main_win,enemy_boat->y,enemy_boat->x,"Z");
	mvwprintw(main_win,cargo_ship->y,cargo_ship->x,"C");
	wrefresh(main_win);

}

int main(void) {
	srand(time(0));
	if(initScreen()) {
		createWindows();
		createPieces();
		checkPiecesLocation();
		printPieces();
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
