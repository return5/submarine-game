// Submarine game
//author: github/return5
//license: GPL 2.0
//version: 0.8
//currently a work in progress.
//turned based game. player controls a submarine and is in a fight against an enemy submarine and an enemy destroyer.  also includes a cargo ship which can be destroyed for extra points


//---------------------------------------- headers ------------------------------------------------
#include <stdlib.h>
#include <ncurses.h>
#include "createPieces.h"
#include "makeWin.h"
#include "playerturn.h"
#include "computerturn.h"
#include "printStuff.h"

//---------------------------------------- prototypes ----------------------------------------------

void gameLoop(void);
void exitGame(void);
void playerTurn(void);

//---------------------------------------- typedefs,enums,consts ----------------------------------


//----------------------------------------  global vars -------------------------------------------
SHIP *player_sub; //player controlled submarine
ENEMIES *enemies; //head of linked list which holds enemy ships
int play = 1;
FILE *file; 

//---------------------------------------- code ---------------------------------------------------

void gameLoop(void) {
	while(play == 1) {
		playerTurn();  //playerturn.c
		computerTurn(); //computerturn.c
	}
}

//when the game is over.
void exitGame(void) {
	endwin();
}

int main(void) {	
	file = fopen("log.txt","w");
	makeWin();   //makewin.c
	createPieces(); //createPieces.c
	printOptWin(); //printstuff.c
	updateBoard(); //printstuff.c
	gameLoop();
	exitGame();
	return EXIT_SUCCESS;
}
