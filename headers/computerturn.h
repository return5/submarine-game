#include "units.h"
#include"macros.h"
#include <stdlib.h>
#include <ncurses.h>

//---------------------------------------- prototypes ---------------------------------------------
 
void loopDecTree(SHIP *const ship);
void computerTurn(void);
static int getRandom(const int start, const int end);
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

//---------------------------------------- prototypes in other files ------------------------------

void updateBoard(void); //printStuff.c
void resetAP(SHIP *const ship);  //movement.c
void setShipAp(SHIP *const ship);  //movement.c
int moveShip(SHIP *const ship, const int new_x, const int new_y, const int new_z); //movement.c
int getDistance(const SHIP *const ship1,const SHIP *const ship2); //movement.c
void shootCannon(const int x_off,const int y_off);  //weapons.c
void shootDepthCharge(SHIP *const ship, const int x_off,const int y_off,const int z); //weapons.c
void subShootAOE(SHIP *const ship, const int x_off, const int y_off); //weapons.c
int moveCompShip(SHIP *const ship,void *dec); //movement.c
int moveRandDirection(SHIP *const ship); //movement.c
int moveSameDirection(SHIP *const ship); //movement.c
int moveTowards(SHIP *const ship, void *dec); //movement.c
int moveAway(SHIP *const ship, void *dec); //movement.c
void displaySonar(SHIP *const ship); // printStuff.c
void shipDetected(SHIP *const ship); //weapons.c
void computerShootTor(const int hit_or_miss); //weapons.c
void checkIfDetected(const SHIP *const sub);  //weapons.c

//---------------------------------------- global vars ----------------------------------------------
extern int play;
