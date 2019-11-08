//---------------------------------------- headers ------------------------------------------------
#include "macros.h"
#include "fireTor.h"
#include "printStuff.h"
#include "setOpts.h"
#include <stdlib.h>

//---------------------------------------- prototypes ----------------------------------------------

void checkIfTargetHitX(SHIP *const target, const int start, const int end);
void checkIfTargetHitY(SHIP *const target, const int start, const int end);
void getSingleTargetX(SHIP **targets, const int index,const int abs,const int limit);
void getSingleTargetY(SHIP **targets, const int index,const int abs,const int limit);
void fireTorpedoForward(const int limit);
void fireTorpedoBack(const int limit);
void fireTorpedoLeft(const int limit);
void fireTorpedoRight(const int limit);
void torpedoFireLineUp(int const limit);
void fireTorpedo(const int limit);
void confirmFireTorpedo(const int limit);
void torpedoFireLineRight(int const limit);
void torpedoFireLineLeft(int const limit);
void torpedoFireLineDown(int const limit);
void setTorpedoFireLine(void);
int checkZ(const SHIP *const ship);
void targetDestroyed(SHIP *const target);
void damageTarget(SHIP *const target, const int damage);
int getLimitX(void);
int getLimitY(void);
void decreaseAOEDIst(int limit);
void increaseAOEDist(int limit);
void checkAOEZ(SHIP *const target,const int dist_x, const int dist_y);
void checkBlastRadius(const int x,const int y);
void damagedShip(SHIP *const ship);
void useAOETor(const int limit);

//---------------------------------------- code ---------------------------------------------------


void damagedShip(SHIP *const ship) {
	switch(ship->type) {
		case SUBMARINE: printToTxtScr(0,0,"you damaged the enemy sub.");
			break;
		case SURFACESHIP: printToTxtScr(0,0,"you damaged the enemy surfaceship.");
			break;
		default: printToTxtScr(0,0,"you damaged the enemy cargo ship.");
			break;
	}
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
}

void damageTarget(SHIP *const target, const int damage) {
	target->health -= damage;
	if(target->health <= 0) {
		targetDestroyed(target);
	}
	else if(damage == 0) {
		printToTxtScr(0,0,"sorry, torpedo missed target.");
	}
	else if(damage > 0) {
		damagedShip(target);
	}
}

//if enemy ship is within the z of a blast, then do damage to it
void checkAOEZ(SHIP *const target,const int dist_x, const int dist_y) {
	int damage = 0;
	switch(abs(player_sub->z - target->z)) {
		case 0: 
			if(dist_x < 1 && dist_y < 1 ){
				damage = 100;
			}
			else if (dist_x < 2 && dist_y < 2) {
				damage = 75;
			}
			else if (dist_x < 3 && dist_y < 3) {
				damage = 50;
			}
			break;
		case 1: 
			if(dist_x < 1 && dist_y < 1) {
				damage = 75;
			}
			else if (dist_x < 2 && dist_y < 2) {
				damage = 50;
			}
			break;
		case 2:	
			if(dist_x ==  0 && dist_y == 0) {
				damage = 50;
			}
			break;
		default: //missed
			break;
	}
	if(damage > 0) {
		damageTarget(target,damage);
		getch();
	}
}

//checks if an enemy ship is within the blast radius in x and y
void checkBlastRadius(const int x,const int y) {
	ENEMIES *head = enemies;
	while(head != NULL) {
		const int dist_x = abs(x - (head->ship->x / X_NORM));
		const int dist_y = abs(y - head->ship->y);
		if( dist_x < 3 &&  dist_y < 3){
			checkAOEZ(head->ship,dist_x,dist_y);
		}
		head = head->next;
	}
}

//if firing along x axis, checks if enemy ship is hit
void checkIfTargetHitX(SHIP *const target, const int start, const int end) {
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
void checkIfTargetHitY(SHIP *const target, const int start, const int end) {
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
		printPieces();
		animateTorpedo(limit / X_NORM);
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
		printPieces();
		animateTorpedo(limit / X_NORM);
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
		printPieces();
		animateTorpedo(limit);
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
		printPieces();
		animateTorpedo(limit);
		printToTxtScr(0,0,"sorry, torpedo missed target.");
		getch();
	}
}

//player fires an aoe torpedo
void useAOETor(const int limit) {
	PLAYER->num_aoetor--;
	player_sub->ap -= 2;
	PLAYER->using_aoe = 0;
	printToOptWin(2,1," ");
	animateTorpedo((player_sub->direction_facing == LEFT || player_sub->direction_facing == RIGHT) ? limit / X_NORM : limit) ;
	displayAOE(limit,YELLOW_COLOR,1);
	checkBlastRadius((getX(limit) / X_NORM),getY(limit));
	getch();
}

void fireTorpedo(const int limit) {
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
void decreaseAOEDIst(int limit) {
	printPieces();	
	wattron(main_win,COLOR_PAIR(RED_COLOR));
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
	wattroff(main_win,COLOR_PAIR(RED_COLOR));
	displayAOE(limit,RED_COLOR,0);
	confirmFireTorpedo(limit);		
}

//increase the range of the AOE torpedo
void increaseAOEDist(int limit) {
	printPieces();
	wattron(main_win,COLOR_PAIR(RED_COLOR));
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
	wattroff(main_win,COLOR_PAIR(RED_COLOR));
	displayAOE(limit,RED_COLOR,0);
	confirmFireTorpedo(limit);				
}

void confirmFireTorpedo(const int limit) {
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
			printToOptWin(2,1," ");
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
			printToOptWin(2,1," ");
			break;
	}
	wclear(main_win);
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

void torpedoFireLineRight(int const limit) {
	for(int i = X_NORM; i < limit; i+= X_NORM) {
		printToMain(player_sub->x+i,player_sub->y,"-");
	}
	printToMain(player_sub->x+limit,player_sub->y,">");
}

void torpedoFireLineLeft(int const limit) {
	for(int i = X_NORM; i < limit; i+= X_NORM) {
		printToMain(player_sub->x-i,player_sub->y,"-");
	}
	printToMain(player_sub->x-limit,player_sub->y,"<");
}

void torpedoFireLineDown(int const limit) {
	for(int i = 1; i < limit; i++) {
		printToMain(player_sub->x,player_sub->y + i,"|");
	}
	printToMain(player_sub->x,player_sub->y+limit,"v");
}

void torpedoFireLineUp(int const limit) {
	for(int i = 1; i < limit; i++) {
		printToMain(player_sub->x,player_sub->y - i,"|");
	}
	printToMain(player_sub->x,player_sub->y-limit,"^");
}

int getLimitX(void) {
	if(player_sub->direction_facing == RIGHT) {
		return (player_sub->x < X_EDGE - TORPEDODISTANCE * X_NORM) ? TORPEDODISTANCE * X_NORM : X_EDGE - player_sub->x - 1;
	}
	else {
		return (player_sub->x > TORPEDODISTANCE * X_NORM) ? TORPEDODISTANCE * X_NORM : player_sub->x;
	}
	return 0;
}

int getLimitY(void) {
	if(player_sub->direction_facing == FORWARD) {
		return (player_sub->y > TORPEDODISTANCE) ? TORPEDODISTANCE : player_sub->y;

	}
	else {
		return (player_sub->y < Y_EDGE - TORPEDODISTANCE - 1) ? TORPEDODISTANCE : Y_EDGE - player_sub->y - 1;
	}
	return 0;
}

void setTorpedoFireLine(void) {
	int limit;
	wattron(main_win,COLOR_PAIR(RED_COLOR));  //turn color red on
	switch(player_sub->direction_facing) {
		case FORWARD: 
			limit = getLimitY();
			torpedoFireLineUp(limit);
			break;
		case BACK: 
			limit = getLimitY();
			torpedoFireLineDown(limit);
			break;
		case LEFT:
			limit = getLimitX(); 
			torpedoFireLineLeft(limit);
			break;
		case RIGHT:
			limit = getLimitX(); 
			torpedoFireLineRight(limit);
			break;
		default: limit = 0;  //default init. shouldnt reach here. 
			break;
	}
	wattroff(main_win,COLOR_PAIR(RED_COLOR)); //turn color red off
	if(PLAYER->using_aoe == 1) {
		displayAOE(limit,RED_COLOR,0);
	}
	confirmFireTorpedo(limit);
}