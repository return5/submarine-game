//---------------------------------------- headers ------------------------------------------------
#include "units.h"
#include "macros.h"
#include "dectreefuncs.h"
#include "makedectree.h"
#include <stdlib.h>

//---------------------------------------- typedefs,enums,consts ----------------------------------
typedef void (*func_pointer)(void *ship, void *dec);
static const int SIZE_DECTREE = sizeof(dectree);

//---------------------------------------- prototypes ----------------------------------------------
void makeCargoDecTree(SHIP *const ship);
void makeSubDecTree(SHIP *const ship);
void makeSurDecTree(SHIP *const ship);
dectree *makeTree(const int *const num_ars,const func_pointer *const functions,dectree *dec);
dectree *addToTree(dectree *dec,const int num, const int y, const int *const ar, const func_pointer func);


//---------------------------------------- code ---------------------------------------------------


dectree *addToTree(dectree *dec,const int num, const int y, const int *const ar, const func_pointer func) {
	if(dec == NULL) {
		dec = malloc(SIZE_DECTREE);
		dec->decFunc = func;
		dec->leftdec = dec->rightdec = NULL;
	}
	else if(num <= ar[y] ){
		dec->leftdec = addToTree(dec->leftdec,num,y + 1,ar,func);
	}
	else if(num > ar[y]) {
		dec->rightdec = addToTree(dec->rightdec,num,y + 1,ar,func);
	}
	return dec;
}

dectree *makeTree(const int *const num_ars,const func_pointer *const functions,dectree *dec) {
	const int limit = sizeof(num_ars) * sizeof(int);
	for(int i = 0; i < limit; i++) {
		dec = addToTree(dec,num_ars[i],0,num_ars,functions[i]);
	}
	return dec;
}

void makeSurDecTree(SHIP *const ship) {
	const int ship_nums[] = { 3,2,1 };
	const func_pointer surShip_decar[] = { startFunc,secondFunc,moveCompShip };
	ship->dtree = NULL;
	ship->dtree = makeTree(ship_nums,surShip_decar,ship->dtree);
}

void makeSubDecTree(SHIP *const ship) {
	const int sub_nums[] = { 3,2,1 };
	const func_pointer subShip_decar[] = { startFunc,secondFunc,moveCompShip };
	ship->dtree = NULL;
	ship->dtree = makeTree(sub_nums,subShip_decar,ship->dtree);
}

void makeCargoDecTree(SHIP *const ship) {
	const int cargo_nums[] = { 3,2,1 };
	const func_pointer cargoShip_decar[] = { startFunc,secondFunc,moveCompShip };
	ship->dtree = NULL;
	ship->dtree = makeTree(cargo_nums,cargoShip_decar,ship->dtree);
}

void makeDecTrees(void) {
	ENEMIES *head = enemies;
	while(head != NULL) { //make a dec tree for each enemy in the enemies linked list
		switch(head->ship->type) {
			case SUBMARINE: makeSubDecTree(head->ship);
				break;
			case SURFACESHIP:makeSurDecTree(head->ship);
				break;
			case CARGOSHIP:makeCargoDecTree(head->ship);
				break;
			default: //do nothing. should never get here
				break;
		}
		head = head->next;
	}

}