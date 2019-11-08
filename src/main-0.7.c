// Submarine game
//author: github/return5
//license: GPL 2.0
//version: 0.7
//currently a work in progress.
//turned based game. player controls a submarine and is in a fight against an enemy submarine and an enemy destroyer.  also includes a cargo ship which can be destroyed for extra points


//---------------------------------------- headers ------------------------------------------------
#include <stdlib.h>
#include <ncurses.h>
#include "printStuff.h"
#include "createPieces.h"
#include "makeWin.h"
#include "playerturn.h"

//---------------------------------------- prototypes ----------------------------------------------

void gameLoop(void);
void exitGame(void);
void playerTurn(void);

//---------------------------------------- typedefs,enums,consts ----------------------------------


//----------------------------------------  global vars -------------------------------------------
static int play = 1;

//---------------------------------------- code ---------------------------------------------------

void gameLoop(void) {
	while(play == 1) {
		playerTurn();  //playerturn.h
	}
}

//when the game is over.
void exitGame(void) {
	endwin();
}

int main(void) {	
	makeWin();   //makewin.h
	createPieces(); //createPieces.h
	printOptWin(); //printstuff.h
	updateBoard(); //printstuff.h
	gameLoop();
	exitGame();
	return EXIT_SUCCESS;
}