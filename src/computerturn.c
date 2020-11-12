//---------------------------------------- headers ------------------------------------------------
#include <stdlib.h>
#include "computerturn.h"
#include"macros.h"

//---------------------------------------- macros -------------------------------------------------

#define UNUSED __attribute__((unused))  //disable warnings about unused vars
//---------------------------------------- code ---------------------------------------------------

//random num in the range of start - (end -1)
static int getRandom(const int start, const int end) {
	return (rand() % (end - start)) + start;
}

//randomly return 1 or 2. biased towards returning 1
int chooseRand(UNUSED SHIP *const ship, UNUSED void *dec) {
	return (getRandom(0,10)  < 7) ? 1 : 2;
}

//is player_sub within range of an AOE torpedo attack. 
int isInTorRange(SHIP *const ship, UNUSED void *dec) {
	return (getDistance(player_sub,ship) < TORPEDODISTANCE + 3) ? 1 : 2;
}

//is player sub within distance of a regular torpedo attack and in the same x or y of enemy sub and aqlso +/- 1 z of enemy sub
int isInRegTorRange(SHIP *const ship, UNUSED void *dec) {	
	return (getDistance(player_sub,ship) <= TORPEDODISTANCE && (player_sub->x == ship->x || player_sub->y == ship->y) && player_sub->z >= ship->z - 1 && player_sub->z <= ship->z + 1) ? 1 : 2;
}

//does ship have AOE tor left in inventory
int haveAOE(SHIP *const ship, UNUSED void *dec) {
	return (SUBSHIP->num_aoetor > 0) ? 1 : 2;
}

//sub uses sonar.
int useSonar(SHIP *const ship, void *dec) {
	switch(SUBSHIP->used_sonar) {
		case 0:
			displaySonar(ship);      //printStuff.c
			shipDetected(ship);     //weapons.c
			checkIfDetected(ship);  //weapons.c
			SUBSHIP->used_sonar = 1;
			SUBSHIP->turns_sonar = 0;
			break;
		default: moveCompShip(ship,dec);
			break;
	}
	return 0;
}

//shoot a AOE tor
int shootAOE(SHIP *const ship,void *dec) {
	if(SUBSHIP->num_aoetor > 0) {
		const int x_off = getRandom(0,3) * ((getRandom(0,2) < 1) ? 1 : -1);
		const int y_off = getRandom(0,3) * ((getRandom(0,2) < 1) ? 1 : -1);
		subShootAOE(ship,x_off,y_off); //weapons.c
		SUBSHIP->num_aoetor--;
		ship->shot_at = 1;
	}
	else {
		moveCompShip(ship,dec); //movement.c
	}
	return 0;
}

//enemy sub shoots regular torpedo
int shootTor(UNUSED SHIP *const ship, UNUSED void *dec) {
	computerShootTor((getRandom(0,3) < 2) ? 1 : 2); //shoot regular torpedo with a 1/3 chance to miss
	ship->shot_at = 1;
	return 0;	
}

//has submarine used sonar within the last 10 turns and also not in current turn
int timeSinceSonar(SHIP *const ship, UNUSED void *dec) {
	return (SUBSHIP->turns_sonar < 7) ? 1 : 2; 
}

//choose random number to decide if ship should move towards player_sub or reg move.  
//biaseds towards regular move
int chooseRandThree(UNUSED SHIP *const ship, UNUSED void *dec) {
	//return 1 to move towards player_sub
	return (getRandom(0,10) > 5) ? 1 : 2 ;
}


//if player sub is not far from ship, then return 1
int distTowards (SHIP *const ship, UNUSED void *dec) {
	return (getDistance(player_sub,ship) < TORPEDODISTANCE + 6) ? 1 : 2;
}

//make a random +/- 0-3 offset for x coordinate
int getXOff(const SHIP *const ship) {
	int x;
	const int n = (getRandom(0,2) < 1) ? 1 : -1;
	do {
		x = getRandom(0,4);
	}while(abs(ship->x + (x * n)) > TORPEDODISTANCE);
	return x;
}

//make a random +/- 0-3 offset for y coordinate
int getYOff(const SHIP *const ship) {
	int y;
	const int n = (getRandom(0,2) < 1) ? 1 : -1;
	do {
		y = getRandom(0,4);
	}while(abs(ship->y + (y * n)) > TORPEDODISTANCE);
	return y;
}

//if player sub is within range, surface ship wil shoot at it with a random x and y offset. 
int surShipShootAt(SHIP *const ship, void *dec) {
		switch(player_sub->z) { //check is player_sub is on surface or not
		//if on surface, shoot with cannon. random offset of 0-1 for x and y
		case 0: shootCannon((getRandom(0,2) < 1) ? getRandom(0,2) : -1 * getRandom(0,2),(getRandom(0,2) < 1) ? getRandom(0,2) : -1 * getRandom(0,2)); 
			break;
		//under water, so use depth charges. random offset of 0-2 for x and y
		default: 
			if(SURSHIP->num_charges > 0 && getRandom(0,5)+ SURSHIP->dropped_charge < 3) {
				shootDepthCharge(ship,getXOff(ship),getYOff(ship),getRandom(1,5)); 
				SURSHIP->dropped_charge++;
			}
			else {
				moveCompShip(ship,dec);
			}
		break;
	}
	ship->shot_at = 1;
	return 0;
}

//choose random number to decide if ship should move towards player_sub or move where ever
int chooseRandTwo(UNUSED SHIP *const ship, UNUSED void *dec) {
	//return 1 to move towards player_sub
	return getRandom(0,2) + 1;
}

//has ship shot at player_sub last known location yet?
int haveShotAt(SHIP *const ship, UNUSED void *dec) {
	//shot_at == 0  means not shot at yet. should return 1 in that case
	return ship->shot_at + 1;
}

//choose random number to decide if ship moves or shoots
int chooseRandOne(UNUSED SHIP *const ship, UNUSED void *dec) {
	//return 1 to move and 2 to shoot
	return (getRandom(0,10) < 7 ) ? 1: 2;
}

//is player sub within firing range of ship?
int isInRange(SHIP *const ship, UNUSED void *dec) {
	//return 1 for yes, player_sub is within range
	return (getDistance(player_sub,ship) < TORPEDODISTANCE) ? 1 : 2;
}

//has player_sub's location been known recently
int timeSinceKnown(UNUSED SHIP *const ship, UNUSED void *dec) {
	//return 1 if player sub's location was known within last 10 turns
	return (PLAYER->last_detected < 10) ? 1 : 2;
}

//has player_sub's location been discovered
int isLocKnown(UNUSED SHIP *const ship, UNUSED void *dec) {
	//return 1 if player_sub's location has been detected
	return (player_sub->detected != 0 ) ? 1 : 2;
}

//loop through computer player's decision tree
void loopDecTree(SHIP *const ship) {
	dectree *dec = ship->dtree;
	int ctrl;
	do {
		ctrl = dec->decFunc(ship,(void*)dec);
		switch(ctrl) {
			case 1: dec = dec->leftdec;
				break;
			case 2: dec = dec->rightdec;
				break;
			default:  //do nothing
				break;
		}
	} while(ctrl != 0);
}

//update attricbutes for ships for each turn
void updateShip(SHIP *const ship) {
	resetAP(ship); //reset AP for that ship's next turn	
	switch(ship->type) {
		case SUBMARINE: 
			SUBSHIP->last_detected++;
			SUBSHIP->turns_sonar++;
			SUBSHIP->used_sonar = 0;
			break;
		default: //do nothing
			break;
	}
}

//loop through each computer ship and go through its decision tree for that turn
void computerTurn(void) {
	ENEMIES *head = enemies;
	while(head != NULL && play == 1) {
		updateShip(head->ship); //update ship.
		while(head->ship->ap > 0) { //while ship still has ap left.
			loopDecTree(head->ship);
			setShipAp(head->ship);
			updateBoard();
		}
		head = head->next;
	}
} 
