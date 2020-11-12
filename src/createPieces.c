
//---------------------------------------- headers -------------------------------------------------
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "createPieces.h"
#include "units.h"
#include "macros.h"
//----------------------------------------  global vars -------------------------------------------
int num_enemies = 0;

//---------------------------------------- prototypes ----------------------------------------------
static int getRandom(const int start, const int end);
static void makeBoat(SHIP *const ship);
static void makeSub(SHIP *const ship);
static void makeShip(SHIP *const ship,const int x_start, const int y_start, const int x_end, const int y_end,enum TYPE type);
static void setFacingDirection(SHIP *const ship);
static ENEMIES *makeNewNode(const int type);
static ENEMIES *addEnemy(ENEMIES *enemy_list,const int type);

//---------------------------------------- code ---------------------------------------------------

//random num in the range of start - (end -1)
static int getRandom(const int start, const int end) {
	return (rand() % (end - start)) + start;
}

//init a random facing direction
static void setFacingDirection(SHIP *const ship) {
	switch(getRandom(0,4)) {
		case 0: ship->direction_facing = FORWARD;
			break;
		case 1: ship->direction_facing = BACK;
			break;
		case 2: ship->direction_facing = LEFT;
			break;
		case 3: ship->direction_facing = RIGHT;
			break;		
		default: //do nothing. should never get here
			break;
	}
}

static void makeBoat(SHIP *const ship) {
	SURSHIP = malloc(sizeof(SURFACE));
	if(ship->type == SURFACESHIP) { //if ship is surface ship and not a cargo ship
		SURSHIP->num_charges = 25;  //number of depth charges left
		SURSHIP->dropped_charge = 0; //has ship dropped a depth charge this turn. 0 for no.
	}
	ship->z = 0;  // z=0 which is surface of water
}

static void makeSub(SHIP *const ship) {
	SUBSHIP = malloc(sizeof(SUB));
	SUBSHIP->num_aoetor = 2; //area of effect torpedoes is 2
	SUBSHIP->last_detected = -1;  //init to neg number
	SUBSHIP->used_sonar = 0;
	ship->z = getRandom(1,5);  //set z to random num between 1 and 4
}

static void makeShip(SHIP *const ship,const int x_start, const int y_start, const int x_end, const int y_end,enum TYPE type){
	ship->health = 100;
	do {
		ship->x = getRandom(x_start,x_end);  //set x to random location between x_start adn x_end
	} while(!(ship->x % X_NORM));  //make sure x location is divisable by 2. 
	ship->y = getRandom(y_start,y_end);  //set y to random location between y_start adn y_end
	ship->typeship = malloc(sizeof(SHIPTYPE));
	ship->type = type;
	ship->turbo = 1;
	ship->ap = 2;
	ship->detected = 0;
	ship->shot_at = 0;
	(type == SUBMARINE)? makeSub(ship) : makeBoat(ship);
	setFacingDirection(ship);
	ship->prev_x = ship->x;	
	ship->prev_y = ship->y;	
	ship->prev_z = ship->z;
	ship->last_knownx = ship->x;
	ship->last_knowny = ship->y;
}

static ENEMIES *makeNewNode(const int type) {
	ENEMIES *newnode = malloc(sizeof(ENEMIES));
	newnode->ship = malloc(sizeof(SHIP));
	switch(type) {
		case SUBMARINE: 
		makeShip(newnode->ship,(X_EDGE/2)+5,(Y_EDGE/2)+5,X_EDGE-1,Y_EDGE-1,type);
			break;
		case SURFACESHIP: 
		makeShip(newnode->ship,0,0,X_EDGE-1,Y_EDGE-1,type);
			break;
		case CARGOSHIP: 
		makeShip(newnode->ship,X_EDGE/2,0,X_EDGE-1,Y_EDGE-1,type);
			break;
		default: //do nothing
			break;
	}
	return newnode;
}

static ENEMIES *addEnemy(ENEMIES *enemy_list,const int type) {
	num_enemies++;
	ENEMIES *head = enemy_list;
	if(head == NULL) {
		head = makeNewNode(type);
		return head;
	}
	else {
		while(head->next != NULL) {
			head = head->next;
		}
		head->next = makeNewNode(type);
	}
	return enemy_list;
}

void createPieces(void) {
	srand(time(0));   //seed random number generator
	player_sub = malloc(sizeof(SHIP));
	makeShip(player_sub,0,0,(X_EDGE/2)-5,(Y_EDGE/2)-5,SUBMARINE);
	enemies = addEnemy(enemies,SURFACESHIP);
	enemies = addEnemy(enemies,CARGOSHIP);
	switch(difficulty) {
		case 4: enemies = addEnemy(enemies,SUBMARINE);  //very hard = 2 subs, 2 surface shipes
		//FALLTHRU 
		case 3: enemies = addEnemy(enemies,SURFACESHIP); //hard = 2 surface ships, 1 sub
		//FALLTHRU 
		case 2: enemies = addEnemy(enemies,SUBMARINE);  //normal = 1 surface ship, 1 sub
			break;
		default: //easy = 1 surcace ship, 0 subs
			break;
	}
	//checkShipsDontOverlap();
	makeDecTrees();  //makedectree.c
}
