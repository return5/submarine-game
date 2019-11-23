//---------------------------------------- headers ------------------------------------------------
#include "movement.h"
#include "macros.h"
#include "units.h"

//---------------------------------------- prototypes ----------------------------------------------

//---------------------------------------- code ---------------------------------------------------

//checks if ship is at boundary, and if not, moves ship to new location, else returns -1
int moveShip(void *ship, const int new_x, const int new_y, const int new_z) {
	SHIP *cur_ship = ((SHIP*) ship);
	if( new_z < 0 || new_z > 4 || (cur_ship->type != SUBMARINE && new_z != 0)) {
		return -1;
	}
	else if(new_x < 0 || new_x >= X_EDGE) {
		return -1;
	}
	else if (new_y < 0 || new_y >= Y_EDGE) {
		return -1;
	}
	else {
		cur_ship->x = new_x;
		cur_ship->y = new_y;
		cur_ship->z = new_z;
		setShipAp(ship);
		return 0;
	}
}
void setShipAp(void *ship) {
	SHIP *cur_ship = ((SHIP*)ship);
	//if ship is less that 50 health, then subtract 2 ap points
	cur_ship->ap -= (cur_ship->health < 50) ? 2 : 1;
	//checks if ship ap is less than zero, if it is, then set it to 0;
	cur_ship->ap = (cur_ship->ap < 0) ? 0 : cur_ship->ap;
}

void updateAP(void *ship) {
	SHIP *cur_ship = ((SHIP*)ship);
	cur_ship->ap = (cur_ship->health < 50 ) ? 1 : 2;
}