// Submarine game
//author: github/return5
//license: GPL 2.0
//currently a work in progress.
//turned based game. player controls a submarine and is in a fight against an enemy submarine and an enemy destroyer.  also includes a cargo ship which can be destroyed for extra points

//---------------------------------------- headers ------------------------------------------------
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include "units.h"
//---------------------------------------- prototypes ----------------------------------------------
int initScreen(void);
void gameLoop(void);
void exitGame(void);
void createPieces(void);
void createWindows(void);
void makeShip(SHIP *ship,const int x_start, const int y_start, const int x_end, const int y_end,enum TYPE type);
void makeSub(SHIP *ship);
void makeBoat(SHIP *ship);
int getRandom(const int start, const int end);
void checkPiecesLocation(void);

//---------------------------------------- typedefs,enums,consts ----------------------------------
#define Y_EDGE 25
#define X_EDGE 75
#define SUB_SHIP ship->ship->sub
#define SUR_SHIP ship->ship->surface
#define PLAYER player_sub->SUB_SHIP
#define ENEMY_SUB enemy_sub->SUB_SHIP
#define ENEMY_SUR enemy_boat->SUR_SHIP

//----------------------------------------  global vars -------------------------------------------
	static WINDOW *main_wnd,*opt_wnd,*status_wnd;
	static SHIP *player_sub,*enemy_sub,*enemy_boat,*cargo_ship;
	static int play = 1;


//---------------------------------------- code ---------------------------------------------------


int getRandom(const int start, const int end) {
	return (rand() % (end - start)) + start;
}

void makeBoat(SHIP *ship) {
	SUR_SHIP = malloc(sizeof(SURFACE));
	if(ship->type == SURFACESHIP) { //if ship is surface ship and not a cargo ship
		SUR_SHIP->num_charges = 25;  //number of depth charges left
		SUR_SHIP->charges_dropped = 0;  //number of depth charges dropped at enemy
		SUR_SHIP->shots_fired = 0;  //number of cannon shots fired at enemy.
	}
	ship->z = 0;  // z=0 which is surface of water
}

void makeSub(SHIP *ship) {
	SUB_SHIP = malloc(sizeof(SUB));
	SUB_SHIP->num_aoetor = 2; //area of effect torpedoes is 2
	SUB_SHIP->last_detected = 100;  //init to large number
	ship->z = getRandom(1,5);  //set z to random num between 1 and 4
}

void makeShip(SHIP *ship,const int x_start, const int y_start, const int x_end, const int y_end,enum TYPE type){
	ship->health = 100;
	ship->last_knownx = 0;
	ship->last_knowny = 0;
	ship->x = getRandom(x_start,x_end);  //set x to random location between x_start adn x_end
	ship->y = getRandom(y_start,y_end);  //set y to random location between y_start adn y_end
	ship->ship = malloc(sizeof(SHIPS));
	ship->type = type;
	ship->turbo = 1;
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
	#define STATW 17 //status window width
	#define OPTH 5 //opt window height
	#define STATH 5 //status window height

	WINDOW *game_border,*status_border,*opt_border;
	main_wnd = newwin(Y_EDGE,X_EDGE,1,1);
	opt_wnd = newwin(OPTH,OPTW,Y_EDGE-OPTH+1,X_EDGE+3);
	status_wnd = newwin(STATH,STATW,1,X_EDGE+3);
	game_border = newwin(Y_EDGE+2,X_EDGE+2,0,0);
	wborder(game_border,'|','|','-', '-', '+', '+', '+', '+');
	status_border = newwin(STATH+2,STATW+2,0,X_EDGE+2);
	wborder(status_border,'|','|','-', '-', '+', '+', '+', '+');
	opt_border = newwin(OPTH+2,OPTW+2,Y_EDGE-OPTH,X_EDGE+2);
	wborder(opt_border,'|','|','-', '-', '+', '+', '+', '+');
	wrefresh(game_border);
	wrefresh(status_border);
	wrefresh(opt_border);
	wrefresh(main_wnd);
	for(int i = 0 ; i < OPTH; i++) {
		mvwprintw(opt_wnd,i,0,"%d",i);
	}
	wrefresh(opt_wnd);
	mvwprintw(status_wnd,0,0,"health is: 100");
	wrefresh(status_wnd);

}

//initialize ncurses
int initScreen(void) {
	//(!has_colors()) {  //if terminal can not display color return -1
//		return -1;
//	}
	start_color();
	initscr();        //start ncurses
	noecho();	      //dont display key strokes
	//cbreak();	     //disable line buffering
	curs_set(0);    //hide cursor
	keypad(stdscr,TRUE);  //enable keypad, needed for mouse.
	mousemask(ALL_MOUSE_EVENTS,NULL); //enable all mouse events
	refresh();
	return 1;
}

void gameLoop(void) {
	while(play == 1) {
		if(getch() == 'q') {
			play = 0;
		}
	}
}

//when the game is over.
void exitGame(void) {
	endwin();
}

void printPieces(void) {
	mvwprintw(main_wnd,player_sub->y,player_sub->x,"X");
	mvwprintw(status_wnd,1,0,"location: %d %d %d",player_sub->x,player_sub->y,player_sub->z);
	mvwprintw(main_wnd,enemy_sub->y,enemy_sub->x,"Y");
	mvwprintw(main_wnd,enemy_boat->y,enemy_boat->x,"Z");
	mvwprintw(main_wnd,cargo_ship->y,cargo_ship->x,"C");
	wrefresh(main_wnd);
	wrefresh(status_wnd);

}

int main(void) {
	srand(time(0));
	if(initScreen()) {
		createWindows();
		createPieces();
		checkPiecesLocation();
		printPieces();
		gameLoop();
		exitGame();
	}
	else {
		printf("sorry, but your current terminal doesnt support colors.\n");
	}
	return EXIT_SUCCESS;
}
