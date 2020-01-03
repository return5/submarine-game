//---------------------------------------- headers ------------------------------------------------
#include "movement.h"

//---------------------------------------- prototypes ----------------------------------------------


//---------------------------------------- code ---------------------------------------------------

//random num in the range of start - (end -1)
static int getRandom(const int start, const int end) {
	return (rand() % (end - start)) + start;
}

//return distance from last known location of ship1 to ship2
int getDistance(const SHIP *const ship1,const SHIP *const ship2) {
	const int x_diff = (ship1->last_knownx - ship2->x) / X_NORM;
	const int y_diff = ship1->last_knowny - ship2->y;
	const int z_diff = ship1->last_knownz - ship2->z;
	return (int) sqrt(pow(x_diff,2) + pow(y_diff,2) + pow(z_diff,2));
}

//return closest x coordinate away player_sub
int moveAwayX(SHIP *const ship) {
	return (player_sub->last_knownx > ship->x) ? ship->x - X_NORM : ship->x + X_NORM;
}

//return closest y coordinate away player_sub
int moveAwayY(SHIP *const ship) {
	return (player_sub->last_knowny > ship->y) ? ship->y - 1 : ship->y + 1;
}

//move away from player_sub
int moveAway(SHIP *const ship, void *dec) {
	if(ship == NULL && dec == NULL) {
		//whatever
	}
	int x = ship->x;
	int y = ship->y;
	if (abs(ship->x - player_sub->last_knownx) < abs(ship->y - player_sub->last_knowny)) {
		x = moveAwayX(ship);
	}
	else {
		y = moveAwayY(ship);
	}
	if(moveShip(ship,x,y,ship->z) == -1) {
		moveCompShip(ship,dec);
	}
	
	return 0;
}

//return closest x coordinate toward player_sub
int moveTowardsX(SHIP *const ship) {
	return (player_sub->last_knownx < ship->x) ? ship->x - X_NORM : ship->x + X_NORM;
}

//return closest y coordinate toward player_sub
int moveTowardsY(SHIP *const ship) {
	return (player_sub->last_knowny < ship->y) ? ship->y - 1 : ship->y + 1;
}

//move ship towards player_sub last known location
int moveTowards(SHIP *const ship, void *dec) {
	if(ship == NULL && dec == NULL) {
		//whatever
	}
	int x = ship->x;
	int y = ship->y;
	if (abs(ship->x - player_sub->last_knownx) < abs(ship->y - player_sub->last_knowny)) {
		x = moveTowardsX(ship);
	}
	else {
		y = moveTowardsY(ship);
	}
	switch(moveShip(ship,x,y,ship->z)) {
		case -1: return moveCompShip(ship,dec);
		default: return 0;
			break;
	}
}

//checks if ship is at boundary, and if not, moves ship to new location, else returns -1
int moveShip(SHIP *const ship, const int new_x, const int new_y, const int new_z) {
	//if ship tries to move to last place it was and isnt the player, then forbid it
	if(ship != player_sub && new_x == ship->prev_x && new_y == ship->prev_y && new_z == ship->prev_z) {
		return -1;
	}
	else {
		switch(new_z) { //checks new z to make sure it is in bounds
			case 0: //okay nothing to see here.
				break;
			case 1 ... 4: 
				switch(ship->type) { //if z is anythign but 0 and not a sub, then error
					case SUBMARINE: //do nothing. all is well.
						break;
					default: return -1; //error, return -1
				}
				break;	
			default: return -1;	
		}
		switch(new_x) { //checks new x to make sure it is in bounds
			case 0 ... (X_EDGE -1): //do nothing, all is well
				break;
			default : return -1;
		}	

		switch(new_y) {  //checks new y to make sure it is in bounds
			case 0 ... (Y_EDGE -1): //do nothing, all is well
				break;
			default : return -1;
		}

		ship->prev_x = ship->x;
		ship->prev_y = ship->y;
		ship->prev_z = ship->z;		
		ship->x = new_x;
		ship->y = new_y;
		ship->z = new_z;
		return 0;
	}
}
int moveRandDirection(SHIP *const ship) {	
	int x,y,z;
	x = ship->x;
	y = ship->y;
	z = ship->z;
	switch(getRandom(0,6)) {
		case 0: 
			x -= X_NORM;
			ship->direction_facing = RIGHT;
			break;
		case 1: 
			x += X_NORM;
			ship->direction_facing = LEFT;
			break;
		case 2: 
			y--;
			ship->direction_facing = FORWARD;
			break;
		case 3: 
			y++;
			ship->direction_facing = BACK;
			break;	
		case 4: z++;
			break;
		case 5: z--;
			break;
		default: x = -100;   //for default get incredibly wrong value. makes moveship always return -1
			break;	
	}
	return moveShip(ship,x,y,z);
}

int moveSameDirection(SHIP *const ship) {
	int x,y,z;
	x = ship->x;
	y = ship->y;
	z = ship->z;
	switch(ship->direction_facing) {
		case FORWARD: y--;
			break;
		case BACK: y++;
			break;
		case LEFT: x -= X_NORM;
			break;
		case RIGHT: x += X_NORM;
			break;	
		default: x = -100; //dummy value, never should get here
			break;	
	}
	return moveShip(ship,x,y,z);
}

int moveCompShip(SHIP *const ship, void *dec) {
	if(dec == NULL) {
		//do nothing
	} 
	while((getRandom(0,10) < 7) ? moveSameDirection(ship) : moveRandDirection(ship) == -1);
	return 0;
}

void setShipAp(SHIP *const ship) {
	//if ship is less that 50 health, then subtract 2 ap points
	ship->ap -= (ship->health > 49) ? 1 : 2;
	//checks if ship ap is less than zero, if it is, then set it to 0;
	ship->ap = (ship->ap < 0) ? 0 : ship->ap;
}

void resetAP(SHIP *const ship) {
	ship->ap = (ship->health < 50 ) ? 1 : 2;
}
