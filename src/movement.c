//---------------------------------------- headers ------------------------------------------------
#include "movement.h"
#include "macros.h"
#include "units.h"

//---------------------------------------- prototypes ----------------------------------------------
void setShipAp(SHIP *const ship);

//---------------------------------------- code ---------------------------------------------------

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
		ship->x = new_x;
		ship->y = new_y;
		ship->z = new_z;
		setShipAp(ship);
		return 0;
	}
}
void setShipAp(SHIP *const ship) {
	//if ship is less that 50 health, then sub 2 ap points
	ship->ap -= (ship->health < 50) ? 2 : 1;
	//checks if ship ap is less than zero, if it is, then set it to 0;
	ship->ap = (ship->ap < 0) ? 0 : ship->ap;
}

void updateAP(SHIP *const ship) {
	ship->ap = (ship->health < 50 ) ? 1 : 2;
}