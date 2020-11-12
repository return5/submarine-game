//---------------------------------------- headers ------------------------------------------------
#include "macros.h"
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include "weapons.h"
#include "main.c"
//---------------------------------------- prototypes ----------------------------------------------
static void getSingleTargetX(SHIP **targets, const int index,const int abs,const int limit);
static void getSingleTargetY(SHIP **targets, const int index,const int abs,const int limit);
static void checkIfTargetHitX(SHIP *const target, const int start, const int end);
static void checkIfTargetHitY(SHIP *const target, const int start, const int end);
static void checkAOE(SHIP *const target,const int x, const int y,const int z);
static void damageTarget(SHIP *const target, const int damage);
static void checkBlastRadius(const int x,const int y,const int z);
static void torpedoFireLineRight(int const limit);
static void torpedoFireLineLeft(int const limit);
static void torpedoFireLineDown(int const limit);
static void confirmFireTorpedo(const int limit);
static void fireTorpedoForward(const int limit);
static void targetDestroyed(SHIP *const target);
static void torpedoFireLineUp(int const limit);
static void damagePlayerSub(const int damage);
static void fireTorpedoRight(const int limit);
static void fireTorpedoBack(const int limit);
static void fireTorpedoLeft(const int limit);
static void damagedShip(SHIP *const ship);
static int checkZ(const SHIP *const ship);
static void fireTorpedo(const int limit);
static void decreaseAOEDIst(int limit);
static void increaseAOEDist(int limit);
static void useAOETor(const int limit);
static void checkForEnemySubs(void);
static int getLimit(void);
//---------------------------------------- code ---------------------------------------------------

//when player's current location becomes known, reset the shot_at var in all comp players. 
void updateShotAt(void) {
	ENEMIES *head = enemies;
	while(head != NULL) {
		head->ship->shot_at = 0;
		switch(head->ship->type) {
			case SURFACESHIP: head->SURSHIP->dropped_charge = 0;
				break;
			default: //do nothing
				break;
		}
		head = head->next;
	}
}

static void damagedShip(SHIP *const ship) {
	switch(ship->type) {
		case SUBMARINE: printToTxtScr(0,0,"you damaged the enemy sub.");
			break;
		case SURFACESHIP: printToTxtScr(0,0,"you damaged the enemy surfaceship.");
			break;
		default: printToTxtScr(0,0,"you damaged the enemy cargo ship.");
			break;
	}
	getch();
}
static void damagePlayerSub(const int damage){
	player_sub->health -= damage;
	char c[31];
	snprintf(c,31,"you were hit for %d damage.",damage);
	printToTxtScr(0,0,c);
	updateHealthDisplay();
	getch();
	if(player_sub->health < 1) {
		printToTxtScr(0,0,"your sub was destroyed. game over.");
		getch();
		play = 0;
	}
}


static void targetDestroyed(SHIP *const target) {
	if(target != player_sub) {
		switch(target->type) {
			case SUBMARINE: printToTxtScr(0,0,"congragulations, you destroyed enemy sub.");
				break;
			case SURFACESHIP: printToTxtScr(0,0,"congragulations, you destroyed enemy surfaceship.");
				break;
			default: printToTxtScr(0,0,"congragulations, you destroyed enemy cargo ship.");
				break;
		}
		target->health = 0;
		target->ap = 0;
		if(enemies->ship == target) {
			enemies = enemies->next;
		}
		else {
			ENEMIES *head = enemies;
			ENEMIES *prev;
			while(head->ship != target && head != NULL) {
				prev = head;
				head = head->next;
			}
			prev->next = head->next;
		}
		num_enemies--;
		getch();
	}
}

static void damageTarget(SHIP *const target, const int damage) {
	if(target != player_sub) {
		target->health -= damage;
		if(target->health <= 0) {
			targetDestroyed(target);
		}
		else if(damage == 0) {
			printToTxtScr(0,0,"sorry, torpedo missed target.");
			getch();
		}
		else if(damage > 0) {
			damagedShip(target);
		}
	}
	else {
		damagePlayerSub(damage);
	}
}

//if enemy ship is within the blast, then do damage to it
static void checkAOE(SHIP *const target,const int x, const int y,const int z) {
	int damage = 0;
	switch((int)sqrt(pow((x-target->x),2) + pow((y-target->y),2) + pow((z - target->z),2))) {
		case 0: damage = 100;
			break;
		case 1: damage = 75;
			break;
		case 2:	damage = 50;
			break;
		case 3:	damage = 25;
			break;
		default: //missed
			break;
	}
	if(damage > 0) {
		damageTarget(target,damage);
		getch();
	}
}

//checks if an enemy ship is within the blast radius
static void checkBlastRadius(const int x,const int y,const int z) {
	ENEMIES *head = enemies;
	while(head != NULL) {
		checkAOE(head->ship,x,y,z);
		head = head->next;
	}
}

void subShootAOE(SHIP *const ship, const int x_off, const int y_off) {	
	const int x = player_sub->last_knownx + x_off * X_NORM;
	const int y = player_sub->last_knowny + y_off;
	displayAOE(x,y,YELLOW_COLOR,1);
	getch();
	checkAOE(player_sub,x,y,ship->z);
}

//if firing along x axis, checks if enemy ship is hit
static void checkIfTargetHitX(SHIP *const target, const int start, const int end) {
	printPieces();
	if(target->x >= start && target->x <= end) {
		animateTorpedo(abs(player_sub->x - target->x) / X_NORM);
		targetDestroyed(target);
	}
	else {
		animateTorpedo(abs(start - end) / X_NORM);
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}
}

//if firing along y axis, checks if an enemy ship was hit
static void checkIfTargetHitY(SHIP *const target, const int start, const int end) {
	printPieces();
	if(target->y >= start && target->y <= end) {
		animateTorpedo(abs(player_sub->y - target->y));
		targetDestroyed(target);
	}
	else {
		animateTorpedo(abs(start - end));
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}
}

//sets target to the ship which is closest to player_sub in x direction
static void getSingleTargetX(SHIP **targets, const int index,const int abs,const int limit) {
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
static void getSingleTargetY(SHIP **targets, const int index,const int abs,const int limit) {
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
static int checkZ(const SHIP *const ship) {
	if(PLAYER->using_aoe == 1) {
		return 1;
	}
	else if(player_sub->z == ship->z) {
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
	return 0;
}

void shootDepthCharge(SHIP *const ship, const int x_off,const int y_off,const int z) {
	printToTxtScr(0,0,"shoot depth");
	const int x = player_sub->last_knownx + x_off * X_NORM;
	const int y = player_sub->last_knowny + y_off;
	displayAOE(x,y,YELLOW_COLOR,1);
	getch();
	checkAOE(player_sub,x,y,z);
	SURSHIP->num_charges--;
}

void shootCannon(const int x_off,const int y_off) {
	if(x_off == player_sub->x && y_off == player_sub->y) {
		printToTxtScr(0,0,"you were hit by enemy destroyer's cannon.");
		getch();
		damagePlayerSub(50);
	}
}

//checks if any enemy ships are in the same direction and on same line as player_sub is firing
static void fireTorpedoRight(const int limit) {
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
		printPieces();
		animateTorpedo(limit / X_NORM);
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}
}

//checks if any enemy ships are in the same direction and on same line as player_sub is firing
static void fireTorpedoLeft(const int limit) {
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
		printPieces();
		animateTorpedo(limit / X_NORM);
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}
}

//checks if any enemy ships are in the same direction and on same line as player_sub is firing
static void fireTorpedoBack(const int limit) {
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
		printPieces();
		animateTorpedo(limit);
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}
}

//checks if any enemy ships are in the same direction and on same line as player_sub is firing
static void fireTorpedoForward(const int limit) {
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
		printPieces();
		animateTorpedo(limit);
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}
}

//player fires an aoe torpedo
static void useAOETor(const int limit) {
	PLAYER->num_aoetor--;
	player_sub->ap -= 2;
	PLAYER->using_aoe = 0;
	printToOptWin(2,1," ",0);
	animateTorpedo((player_sub->direction_facing == LEFT || player_sub->direction_facing == RIGHT) ? limit / X_NORM : limit) ;
	displayAOE(getX(limit),getY(limit),YELLOW_COLOR,1);
	checkBlastRadius((getX(limit)),getY(limit),player_sub->z);
	getch();
}

static void fireTorpedo(const int limit) {
	if(PLAYER->using_aoe == 1) {
		useAOETor(limit);
	}
	else {
		switch(player_sub->direction_facing) {
			case FORWARD: fireTorpedoForward(limit);
				break;
			case BACK: fireTorpedoBack(limit);
				break;
			case LEFT: fireTorpedoLeft(limit);
				break;
			case RIGHT: fireTorpedoRight(limit);
				break;
			default: //do nothing. should not reach here.
				break;
		}
		player_sub->ap -= 1;
	}
}

//decrease the range of the AOE torpedo
static void decreaseAOEDIst(int limit) {
	printPieces();	
	switch(player_sub->direction_facing) {
		case FORWARD:
			if(limit > 3) {
				limit -= 1;
			}
			torpedoFireLineUp(limit);
			break;
		case LEFT: 
			if(limit > 3 * X_NORM) {
				limit -= 1 * X_NORM;
			}
			torpedoFireLineLeft(limit);
			break;
		case RIGHT:
			if(limit > 3 * X_NORM) {
				limit -= 1 * X_NORM;
			}
			torpedoFireLineRight(limit);
			break;
		case BACK: 
			if(limit > 3) {
				limit -= 1;
			}
			torpedoFireLineDown(limit);
			break;	
		default: //do nothing
			break;	
	}
	displayAOE(getX(limit),getY(limit),RED_COLOR,0);
	confirmFireTorpedo(limit);		
}

//increase the range of the AOE torpedo
static void increaseAOEDist(int limit) {
	printPieces();
	switch(player_sub->direction_facing) {
		case FORWARD:
			if(player_sub->y - limit > 0 && limit < TORPEDODISTANCE) {
				limit += 1;
			}
			torpedoFireLineUp(limit);
			break;
		case LEFT: 
			if(player_sub->x - limit * X_NORM > 0 && limit < TORPEDODISTANCE * X_NORM) {
				limit += 1 * X_NORM;
			}
			torpedoFireLineLeft(limit);
			break;
		case RIGHT:
			if(player_sub->x - limit * X_NORM < X_EDGE && limit < TORPEDODISTANCE * X_NORM) {
				limit += 1 * X_NORM;
			}
			torpedoFireLineRight(limit);
			break;
		case BACK: 
			if(player_sub->y + limit < Y_EDGE && limit < TORPEDODISTANCE) {
				limit += 1;
			}
			torpedoFireLineDown(limit);
			break;	
		default: //do nothing
			break;	
	}
	displayAOE(getX(limit),getY(limit),RED_COLOR,0);
	confirmFireTorpedo(limit);				
}

static void confirmFireTorpedo(const int limit) {
	MEVENT event;
	printToTxtScr(0,0,"press 'g' to fire torpedo.\n'w''a''s''d' to change direction\nif using AOE 'q' and 'e' change distance\nany other key cancels order");
	switch(getch()) {
		case 'g':
			if(player_sub->ap > 0) {
				fireTorpedo(limit);
			}
			else {
				printToTxtScr(0,0,"sorry, but you lack suffecient AP");
			}
			printToOptWin(2,1," ",0);
			PLAYER->using_aoe = 0;
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
		case 'q': 
			if(PLAYER->using_aoe == 1) {
				increaseAOEDist(limit);
			}
			break;
		case 'e':
			if(PLAYER->using_aoe == 1) {
				decreaseAOEDIst(limit);
			}
			break;	
		case KEY_MOUSE:
			if(getmouse(&event) == OK) { 
				if(event.bstate & BUTTON1_CLICKED){ //if left mouse button was clicked
					checkMouseLocation(event.x,event.y);
				}
			}		
			break;
		default: 
			PLAYER->using_aoe = 0;
			printToOptWin(2,1," ",0);
			break;
	}
	printPieces();
}


int getY(const int limit) {
	switch(player_sub->direction_facing) {
		case FORWARD: return player_sub->y - limit;
		case BACK: return player_sub->y + limit;
		case LEFT: 
		case RIGHT: return player_sub->y;
		default: return 1; //defualt value, should never get here. 
	}
	return 0;
}

int getX(const int limit) {
	switch(player_sub->direction_facing) {
		case FORWARD: 
		case BACK: return player_sub->x;
		case LEFT: return player_sub->x - limit;
		case RIGHT: return player_sub->x + limit;
		default: return 1; //defualt value, should never get here. 
	}
	return 0;
}

static void torpedoFireLineRight(int const limit) {
	for(int i = X_NORM; i < limit; i += X_NORM) {
		printToMain(player_sub->x+i,player_sub->y,"-",RED_COLOR);
	}
	printToMain(player_sub->x+limit,player_sub->y,">",RED_COLOR);
}

static void torpedoFireLineLeft(int const limit) {
	for(int i = X_NORM; i < limit; i += X_NORM) {
		printToMain(player_sub->x-i,player_sub->y,"-",RED_COLOR);
	}
	printToMain(player_sub->x-limit,player_sub->y,"<",RED_COLOR);
}

static void torpedoFireLineDown(int const limit) {
	for(int i = 1; i < limit; i++) {
		printToMain(player_sub->x,player_sub->y + i,"|",RED_COLOR);
	}
	printToMain(player_sub->x,player_sub->y+limit,"v",RED_COLOR);
}

static void torpedoFireLineUp(int const limit) {
	for(int i = 1; i < limit; i++) {
		printToMain(player_sub->x,player_sub->y - i,"|",RED_COLOR);
	}
	printToMain(player_sub->x,player_sub->y-limit,"^",RED_COLOR);
}

static int getLimit(void) {
	switch(player_sub->direction_facing) {
		case FORWARD: return (player_sub->y > TORPEDODISTANCE) ? TORPEDODISTANCE : player_sub->y;
		case BACK: return (player_sub->y < Y_EDGE - TORPEDODISTANCE - 1) ? TORPEDODISTANCE : Y_EDGE - player_sub->y - 1;
		case LEFT: return (player_sub->x > TORPEDODISTANCE * X_NORM) ? TORPEDODISTANCE * X_NORM : player_sub->x;
		case RIGHT: return (player_sub->x < X_EDGE - TORPEDODISTANCE * X_NORM) ? TORPEDODISTANCE * X_NORM : X_EDGE - player_sub->x - 1;
		default: return 0; //should never get here
	}
}

//if sonar detects a submarine
void shipDetected(SHIP *const ship) {
	ship->last_knownx = ship->x;
	ship->last_knowny = ship->y;
	ship->last_knownz = ship->z;
	SUBSHIP->last_detected = 0;
	SUBSHIP->turns_sonar = 0;
	ship->detected = 1;
	if (ship != player_sub) {
		printToTxtScr(0,0,"enemy ship located");
		printLastDetected(ship);
	}
	else {
		printToTxtScr(0,0,"enemy has detected you");
		updateShotAt();
	}
	getch();
}

static void checkForEnemySubs(void) {
	ENEMIES *head = enemies;
	while(head != NULL) {
		switch(head->ship->type) { 
		//only looking for subs. 
			case SUBMARINE:
				//if player is within 6 blocks of enemy sub, counting diffrences in z coordinate, then mark as found
				if((int) sqrt(pow(player_sub->x - head->ship->x,2) + pow(player_sub->y - head->ship->y,2) + pow(player_sub->z - head->ship->z, 2))< 6) {			
					shipDetected(head->ship);
				}
				break;
			default: //do nothing
				break;
		}
		head = head->next;
	}
}

static void checkForPlayerSub(const SHIP *const ship) {
	//if player is within 6 blocks of enemy sub, counting diffrences in z coordinate, then mark as found
	if((int) sqrt(pow(player_sub->x - ship->x,2) + pow(player_sub->y - ship->y,2) + pow(player_sub->z - ship->z,2)) < 6) {
		shipDetected(player_sub);
	}
	
}
//if player current position is same location as last known location return 1
static int checkLastKnownLoc(void) {
	if(player_sub->x == player_sub->last_knownx && player_sub->y == player_sub->last_knowny && player_sub->z == player_sub->last_knownz) {
		return 1;
	}
	return 0;
}

void computerShootTor(const int hit_or_miss) {
	if(hit_or_miss && checkLastKnownLoc()) {
		printToTxtScr(0,0,"enemy sub shot a torpedo at you");
		getch();
		damagePlayerSub(100);
	}
	else {
		printToTxtScr(0,0,"enemy sub shot a torpedo but missed");
		getch();
	}
}

//checks to see if enemy sub has been found with sonar.
void checkIfDetected(const SHIP *const sub) {
	if(sub == player_sub) {
		checkForEnemySubs();
	}
	else {
		checkForPlayerSub(sub);
	}
}

void setTorpedoFireLine(void) {
	const int limit = getLimit();
	switch(player_sub->direction_facing) {
		case FORWARD: torpedoFireLineUp(limit);
			break;
		case BACK: torpedoFireLineDown(limit);
			break;
		case LEFT: torpedoFireLineLeft(limit);
			break;
		case RIGHT: torpedoFireLineRight(limit);
			break;
		default: //shouldnt reach here. 
			break;
	}
	if(PLAYER->using_aoe == 1) {
		displayAOE(getX(limit),getY(limit),RED_COLOR,0);
	}
	confirmFireTorpedo(limit);
}
