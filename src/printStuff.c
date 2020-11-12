//---------------------------------------- headers ------------------------------------------------
#include <time.h>
#include <ncurses.h>
#include "printStuff.h"
#include "makeWin.h"
#include "macros.h"

//---------------------------------------- prototypes ----------------------------------------------
static void printNumSign(const int i,const int j, const int x, const int y, const int color);
//---------------------------------------- code ---------------------------------------------------

void animateTorpedo(const int limit) {
	for(int i = 1; i <= limit; i++) {
		switch (player_sub->direction_facing) {
			case FORWARD: printToMain(player_sub->x,player_sub->y-i,"|",0);
				break;
			case BACK: printToMain(player_sub->x,player_sub->y+i,"|",0);
				break;
			case LEFT: printToMain(player_sub->x-(i * X_NORM),player_sub->y,"-",0);
				break; 
			case RIGHT: printToMain(player_sub->x+(i * X_NORM),player_sub->y,"-",0);
				break;
			default: //do nothing should never reach this one.
				break;
		}			
		nanosleep((const struct timespec[]){{0, 100000000L}}, NULL);
		printPieces();
	}
}

//updates the display to show ammount of action points you have left
void updateAPDisplay(void) {
	mvwprintw(status_win,4,0,"AP points: %d ",player_sub->ap);
	wrefresh(status_win);
}
//update the display to show how many turns since last detected
void updateLastDetected(void) {
	if(player_sub->detected) {
		PLAYER->last_detected++;
	}
	mvwprintw(status_win,3,0,"detected: %d ",PLAYER->last_detected);
	wrefresh(status_win);
}
//updates the display to show how many AOE torpedoes you have left
void updateAOETorDisplay(void) {
	mvwprintw(status_win,2,0,"AOE Tor: %d",PLAYER->num_aoetor);
	wrefresh(status_win);
}

//update the display to show current health
void updateHealthDisplay(void) {
	mvwprintw(status_win,1,0,"Health: %d  ",player_sub->health);
	wrefresh(status_win);
}

//updates display to show current location as x y z
void updateLocationDisplay(void) {
	mvwprintw(status_win,0,10,"          ");  //blank line to clear line 0,0
	mvwprintw(status_win,0,0,"Locations: %d %d %d",(player_sub->x / X_NORM),player_sub->y,player_sub->z);
	wrefresh(status_win);
}

void printToOptWin(const int x, const int y, const char *const str, const int color) {
	if(color != 0) {
		wattron(opt_win,COLOR_PAIR(color));
	}
	mvwprintw(opt_win,y,x,"%s",str);
	wrefresh(opt_win);
	wattroff(opt_win,COLOR_PAIR(color));
}

//print str to main window at x,y
void printToMain(const int x, const int y, const char *const str, const int color) {	
	if(color != 0) {
		wattron(main_win,COLOR_PAIR(color));
	}
	mvwprintw(main_win,y,x,"%s",str);
	wrefresh(main_win);
	wattroff(main_win,COLOR_PAIR(color));
}

//print str to text_win at location x,y
void printToTxtScr(const int x, const int y, const char *const str) {
	wclear(text_win);
	mvwprintw(text_win,y,x,"%s",str);
	wrefresh(text_win);
}

void printLastDetected(SHIP *const ship) {
	if(ship->detected) {
		printToMain(ship->last_knownx,ship->last_knowny,"O",BLUE_COLOR); //display blue icon where sub is located
	}
}

static void printNumSign(const int i, const int j, const int x, const int y, const int color) {
	printToMain(x + j,y - i,"#",color);
	printToMain(x - j,y - i,"#",color);
	printToMain(x + j,y + i,"#",color);
	printToMain(x - j,y + i,"#",color);
}

void displaySonar(SHIP *const ship) {
	int offset = 6;
	for(int i = 0; i < 7; i++) {
		for(int j = 0; j <= offset * X_NORM; j += X_NORM) {
			printNumSign(i,j,ship->x,ship->y,GREEN_COLOR);
			nanosleep((const struct timespec[]){{0, 4500000L}}, NULL);
		}
		offset--;
	}
	getch();
}

void displayAOE(const int x,const int y,const int color,const int delay) {
	const long int sleep_time = 10000000L * delay; //how long to wait before printing next '#'
	int offset = 2;
	for(int i = 0; i < 3; i++) {
		for(int j = offset * X_NORM; j >= 0; j -= X_NORM) {
			printNumSign(i,j,x,y,color);
			nanosleep((const struct timespec[]){{0, sleep_time}}, NULL);
		}
		offset--;
	}
}

void printOptWin(void) {
	mvwprintw(opt_win,0,1,"[ ]  turbo");
	mvwprintw(opt_win,1,1,"[ ]  AOE tor");
	mvwprintw(opt_win,2,1,"[ ]  sonar");
	mvwprintw(opt_win,3,1,"[ ]  repair");
	wrefresh(opt_win);
}

void printPlayerSub(void) {
	mvwprintw(main_win,player_sub->y,player_sub->x,"x");
	wrefresh(main_win);
}

void updateBoard(void) {
	printPieces();
	updateLocationDisplay();
	updateHealthDisplay();
	updateAOETorDisplay();
	updateAPDisplay();
}

void printPieces(void) {
	wclear(main_win);
	printToMain(player_sub->x,player_sub->y,"X",0);
	if(player_sub->detected != 0) {
		printToMain(player_sub->last_knownx,player_sub->last_knowny,"x",BLUE_COLOR);
	}
	ENEMIES *head = enemies;
	while(head != NULL) {
		switch(head->ship->type) {
		case SUBMARINE: printLastDetected(head->ship);	
			break;
		case SURFACESHIP: printToMain(head->ship->x,head->ship->y,"z",0);
			break;
		case CARGOSHIP: printToMain(head->ship->x,head->ship->y,"c",0);
			break;
		default: printToMain(head->ship->x,head->ship->y,"c",0);
			break;			
		}
		head = head->next;
	}
}
