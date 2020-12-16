//----------------------------------------- include guards ---------------------------------------
#ifndef COMP_TURN_H

#define COMP_TURN_H
//--------------------------------------- headers ------------------------------------------------
#include "units.h"

//---------------------------------------- prototypes ---------------------------------------------
 
void loopDecTree(SHIP *const ship);
void computerTurn(void);
void loopDecTree(SHIP *const ship);
int chooseRandTwo(SHIP *const ship, void *dec);
int timeSinceKnown(SHIP *const ship, void *dec);
int isInRange(SHIP *const ship, void *dec);
int haveShotAt(SHIP *const ship, void *dec);
int chooseRandOne(SHIP *const ship, void *dec);
int isLocKnown(SHIP *const ship, void *dec);
int surShipShootAt(SHIP *const ship, void *dec);
int distTowards (SHIP *const ship, void *dec);
int chooseRandThree(SHIP *const ship, void *dec);
int chooseRand(SHIP *const ship, void *dec);
int isInTorRange(SHIP *const ship, void *dec);
int isInRegTorRange(SHIP *const ship, void *dec);
int haveAOE(SHIP *const ship, void *dec);
int useSonar(SHIP *const ship, void *dec);
int shootAOE(SHIP *const ship, void *dec);
int shootTor(SHIP *const ship, void *dec);
int timeSinceSonar(SHIP *const ship, void *dec);
int getXOff(const SHIP *const ship);
int getYOff(const SHIP *const ship);
void updateShip(SHIP *const ship);

#endif

