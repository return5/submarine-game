//---------------------------------------- include guard ------------------------------------------
#ifndef MAKEDEC_M
#define MAKEDEC_M

#include "units.h"
#include <stdlib.h>
//---------------------------------------- typedefs,enums,consts ----------------------------------
typedef int (*func_pointer)(SHIP *const ship, void *dec);

//---------------------------------------- prototypes ----------------------------------------------

void makeDecTrees(void);
static void makeCargoDecTree(SHIP *const ship);
static void makeSubDecTree(SHIP *const ship);
static void makeSurDecTree(SHIP *const ship);
static dectree *makeTree(const int *const num_ars,const func_pointer *const functions,dectree *dec,const int limit);
static dectree *addToTree(dectree *dec,const int num,const func_pointer func);

//---------------------------------------- prototypes in other files ------------------------------

int moveCompShip(SHIP *const ship,void *dec); //movement.c
int moveRandDirection(SHIP *const ship); //movement.c
int moveSameDirection(SHIP *const ship); //movement.c
int moveTowards(SHIP *const ship, void *dec); //movement.c
int moveAway(SHIP *const ship, void *dec); //movement.c
int chooseRandTwo(SHIP *const ship, void *dec); //computerTurn.c
int timeSinceKnown(SHIP *const ship, void *dec); //computerTurn.c
int isInRange(SHIP *const ship, void *dec); //computerTurn.c
int haveShotAt(SHIP *const ship, void *dec); //computerTurn.c
int chooseRandOne(SHIP *const ship, void *dec); //computerTurn.c
int isLocKnown(SHIP *const ship, void *dec); //computerTurn.c
int surShipShootAt(SHIP *const ship, void *dec); //computerTurn.c
int distTowards (SHIP *const ship, void *dec); //computerTurn.c
int chooseRandThree(SHIP *const ship, void *dec); //computerTurn.c
int chooseRand(SHIP *const ship, void *dec);  //computerturn.c
int isInTorRange(SHIP *const ship, void *dec);  //computerturn.c
int isInRegTorRange(SHIP *const ship, void *dec);  //computerturn.c
int haveAOE(SHIP *const ship, void *dec);  //computerturn.c
int useSonar(SHIP *const ship, void *dec);  //computerturn.c
int timeSinceSonar(SHIP *const ship, void *dec);  //computerturn.c
int shootAOE(SHIP *const ship, void *dec);  //weapons.c
int shootTor(SHIP *const ship, void *dec);  //weapons.c

#endif

