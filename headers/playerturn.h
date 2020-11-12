//---------------------------------------- include guard ------------------------------------------
#ifndef PLAYERTURN_M
#define PLAYERTURN_M

#include "units.h"
#include "macros.h"
#include <ncurses.h>

//---------------------------------------- prototypes ----------------------------------------------
static int getUserInput(void);
static void movePlayer(void);
void playerTurn(void);
static void checkIfWin(void);

//---------------------------------------- prototypes in other files ------------------------------
void updateBoard(void);  //printStuff.c
void printToTxtScr(const int x, const int y, const char *const str);  //printStuff.c
void updateLastDetected(void);  //printStuff.c
void resetAP(SHIP *const ship); //movement.c
int moveShip(SHIP *const ship, const int new_x, const int new_y, const int new_z); //movement.c
void setTorpedoFireLine(void);  //weapons.c
void setShipAp(SHIP *const ship); //movement.c
void updateShotAt(void);  //weapons.c
void checkMouseLocation(const int x, const int y);  //setOpts.c

//---------------------------------------- global vars ----------------------------------------------

extern int play;

#endif

