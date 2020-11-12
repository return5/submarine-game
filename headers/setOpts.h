//---------------------------------------- include guard ------------------------------------------
#ifndef SETOPT_M
#define SETOPTS_M

#include <stddef.h>
#include "units.h"
#include "macros.h"
#include <ncurses.h>

//---------------------------------------- prototypes ----------------------------------------------
void useTurbo(void);
void repairShip(void);
static void useSonar(void);
void useAOE(void);
void checkMouseLocation(const int x, const int y);


//---------------------------------------- prototypes in other files ------------------------------

void checkIfDetected(const SHIP *const sub); //weapons.c
void updateShotAt(void); //weapons.c
void shipDetected(SHIP *const ship); //weapons.c
void setTorpedoFireLine(void); //weapons.c
void printOptWin(void); //printStuff.c
void printToOptWin(const int x, const int y, const char *const str, const int color); //printStuff.c
void printToTxtScr(const int x, const int y, const char *const str); //printStuff.c
void displaySonar(SHIP *const sub); //printStuff.c
void printLastDetected(SHIP *const ship); //printStuff.c

#endif

