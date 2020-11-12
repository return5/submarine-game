//----------------------------------------- include guards ---------------------------------------
#ifndef CREATE_PIECES_M
#define CREATE_PIECES_M

#include "units.h"
#include "macros.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

//---------------------------------------- prototypes ----------------------------------------------

static int getRandom(const int start, const int end);
static void makeBoat(SHIP *const ship);
static void makeSub(SHIP *const ship);
static void makeShip(SHIP *const ship,const int x_start, const int y_start, const int x_end, const int y_end,enum TYPE type);
//static int checkPiecesLocation(SHIP *const cur_ship, SHIP *const next_ship);
//static void checkShipsDontOverlap(void);
//static void moveShips(const SHIP *const cur_ship, SHIP *const next_ship);
void createPieces(void);
void makeDecTrees(void);
static void setFacingDirection(SHIP *const ship);
static ENEMIES *makeNewNode(const int type);
static ENEMIES *addEnemy(ENEMIES *enemy_list,const int type);
//--------------------------------------- global vars -----------------------------------------------
extern int difficulty;
#endif

