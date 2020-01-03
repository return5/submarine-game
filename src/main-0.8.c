// Submarine game
//author: github/return5
//license: GPL 2.0
//version: 0.8
//currently a work in progress.
//turned based game. player controls a submarine and is in a fight against an enemy submarine and an enemy destroyer.  also includes a cargo ship which can be destroyed for extra points
 /* 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

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
