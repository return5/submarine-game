//---------------------------------------- headers ------------------------------------------------
#include <stdlib.h>
#include "makedectree.h"
#include "units.h"

//---------------------------------------- typedefs,enums,consts ----------------------------------
typedef int (*func_pointer)(SHIP *const ship, void *dec);

//---------------------------------------- prototypes ----------------------------------------------
static void makeCargoDecTree(SHIP *const ship);
static void makeSubDecTree(SHIP *const ship);
static void makeSurDecTree(SHIP *const ship);
static dectree *makeTree(const int *const num_ars,const func_pointer *const functions,dectree *dec,const int limit);
static dectree *addToTree(dectree *dec,const int num,const func_pointer func);

//---------------------------------------- code ---------------------------------------------------

static dectree *addToTree(dectree *dec,const int num, const func_pointer func) {
	if(dec == NULL) {
		dec = malloc(sizeof(dectree));
		dec->decFunc = func;
		dec->leftdec = dec->rightdec = NULL;
		dec->num = num;
	}
	else if(num <= dec->num ){
		dec->leftdec = addToTree(dec->leftdec,num,func);
	}
	else if(num > dec->num) {
		dec->rightdec = addToTree(dec->rightdec,num,func);
	}
	return dec;
}

 static dectree *makeTree(const int *const num_ars,const func_pointer *const functions,dectree *dec,const int limit) {
	for(int i = 0; i < limit; i++) {
		dec = addToTree(dec,num_ars[i],functions[i]);
	}
	return dec;
}

static void makeSurDecTree(SHIP *const ship) {
	const int num_ars[] = { 100,40,20,10,2,1,3,11,30,25,24,26,35,45,44,50,120 };
	const func_pointer surship_decar[] = { 

		isLocKnown,timeSinceKnown,isInRange,haveShotAt,chooseRandOne,moveCompShip,
		surShipShootAt,surShipShootAt,distTowards,chooseRandTwo,moveTowards,moveCompShip,
		moveCompShip,chooseRandThree,moveTowards,moveCompShip,moveCompShip
	};	
	ship->dtree = NULL;
	ship->dtree = makeTree(num_ars,surship_decar,ship->dtree,sizeof(num_ars)/sizeof(int));
}

static void makeSubDecTree(SHIP *const ship) {
	const int num_ars[] = 
		{ 
			150,130,120,110,100,95,94,96,101,
			118,114,112,111,113,116,115,117,119,
			122,121,123,134,132,131,133,136,135,
			137,174,171,176,175,177
		};
	const func_pointer subship_decar[] = 
		{ 
			isLocKnown,timeSinceKnown,isInTorRange,isInRegTorRange,haveShotAt,chooseRand,moveCompShip,shootTor,shootTor,
			haveAOE,haveShotAt,chooseRand,shootAOE,moveCompShip,chooseRand,moveCompShip,useSonar,moveCompShip,
			chooseRand,moveCompShip,useSonar,timeSinceSonar,chooseRand,moveCompShip,useSonar,chooseRand,useSonar,
			moveCompShip,timeSinceSonar,moveCompShip,chooseRand,useSonar,moveCompShip
		};
	ship->dtree = NULL;
	ship->dtree = makeTree(num_ars,subship_decar,ship->dtree,sizeof(num_ars)/sizeof(int));
}

static void makeCargoDecTree(SHIP *const ship) {
	const int num_ars[] = { 10,9,7,10,11 };
	const func_pointer cargoship_decar[] = { isLocKnown,timeSinceKnown,moveAway,moveCompShip,moveCompShip };
	ship->dtree = NULL;
	ship->dtree = makeTree(num_ars,cargoship_decar,ship->dtree,sizeof(num_ars)/sizeof(int));
}

void makeDecTrees(void) {
	ENEMIES *head = enemies;
	while(head != NULL) { //make a dec tree for each enemy in the enemies linked list
		switch(head->ship->type) {
			case SUBMARINE: makeSubDecTree(head->ship);
				break;
			case SURFACESHIP: makeSurDecTree(head->ship);
				break;
			case CARGOSHIP: makeCargoDecTree(head->ship);
				break;
			default: //do nothing. should never get here
				break;
		}
		head = head->next;
	}

}
