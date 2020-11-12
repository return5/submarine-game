//----------------------------------------- include guards ---------------------------------------
#ifndef MAIN_M
#define MAIN_M
//---------------------------------------- headers ------------------------------------------------

#include "units.h"
#include <stdlib.h>
#include <ncurses.h>

//---------------------------------------- prototypes ----------------------------------------------
static void gameLoop(void);
static void exitGame(void);

//---------------------------------------- prototypes in other files ------------------------------

void playerTurn(void);         //playerTurn.c
void computerTurn(void);      //computerTurn.c
void makeWin(void);          //makeWin.c
void updateBoard(void);     //printStuff.c
void printOptWin(void);    //printStuff.c
void createPieces(void);  //createPieces.c


//---------------------------------------- global vars ----------------------------------------------

extern int play;

#endif

