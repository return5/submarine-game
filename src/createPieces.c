
//---------------------------------------- headers -------------------------------------------------
#include "createPieces.h"
#include "macros.h"
#include "units.h"
#include <stdlib.h>
#include <time.h>
#include "computerturn.h"
#include "makedectree.h"

//---------------------------------------- prototypes ----------------------------------------------

int getRandom(const int start, const int end);
void makeBoat(SHIP *const ship);
void makeSub(SHIP *const ship);
void makeShip(SHIP *const ship,const int x_start, const int y_start, const int x_end, const int y_end,enum TYPE type);
void checkPiecesLocation(void);
void createPieces(void);

//---------------------------------------- code ---------------------------------------------------

//random num in the range of start - (end -1)
int getRandom(const int start, const int end) {
	return (rand() % (end - start)) + start;
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
	ship->detected = 0;
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
	srand(time(0));
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
	checkPiecesLocation();
	makeDecTrees();  //makedectree.c
}