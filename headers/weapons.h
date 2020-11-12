//---------------------------------------- include guard ------------------------------------------
#ifndef WEAPONS_M
#define WEAPONS_M
//---------------------------------------- headers ------------------------------------------------

#include "units.h"
#include "macros.h"
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>

//---------------------------------------- prototypes ----------------------------------------------


static void getSingleTargetX(SHIP **targets, const int index,const int abs,const int limit);
static void getSingleTargetY(SHIP **targets, const int index,const int abs,const int limit);
void shootDepthCharge(SHIP *const ship, const int x_off,const int y_off,const int z);
static void checkIfTargetHitX(SHIP *const target, const int start, const int end);
static void checkIfTargetHitY(SHIP *const target, const int start, const int end);
static void checkAOE(SHIP *const target,const int x, const int y,const int z);
void subShootAOE(SHIP *const ship, const int x_off, const int y_off);
static void damageTarget(SHIP *const target, const int damage);
static void checkBlastRadius(const int x,const int y,const int z);
void shootCannon(const int x_off,const int y_off); 
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
void computerShootTor(const int hit_or_miss);
void checkIfDetected(const SHIP *const sub);
static void damagedShip(SHIP *const ship);
static int checkZ(const SHIP *const ship);
int shootAOE(SHIP *const ship, void *dec);  
int shootTor(SHIP *const ship, void *dec); 
static void fireTorpedo(const int limit);
static void decreaseAOEDIst(int limit);
static void increaseAOEDist(int limit);
static void useAOETor(const int limit);
static void checkForEnemySubs(void);
void shipDetected(SHIP *const ship);
void setTorpedoFireLine(void);
static int getLimit(void);
int getX(const int limit);
int getY(const int limit);
void updateShotAt(void); 

//---------------------------------------- prototypes in other files ------------------------------

void printToOptWin(const int x, const int y, const char *const str, const int color);    //printStuff.c
void printToMain(const int x, const int y, const char *const str, const int color);     //printStuff.c
void displayAOE(const int x,const int y,const int color, const int delay);  		   //printStuff.c
void printToTxtScr(const int x, const int y, const char *const str);  				  //printStuff.c
void animateTorpedo(const int limit);  												 //printStuff.c
void printPieces(void);  															//printStuff.c
void printOptWin(void);  														   //printStuff.c
void updateHealthDisplay(void);   												  //printStuff.c
void printLastDetected(SHIP *const ship);										 //printStuff.c
void checkMouseLocation(const int x, const int y); 							    //setOpts.c

//---------------------------------------- global vars ----------------------------------------------

extern int play;

#endif

