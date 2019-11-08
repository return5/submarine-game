//---------------------------------------- headers ------------------------------------------------
#include "makeWin.h"
#include "units.h"
#include "macros.h"

//---------------------------------------- prototypes ----------------------------------------------

void createWindows(void);
void initColors(void);
void initScreen(void);

//---------------------------------------- code ---------------------------------------------------

void createWindows(void) {
	#define OPTW 15  //opt window width
	#define OPTH 4 //opt window height
	#define STATW 19 //status window width
	#define STATH 5 //status window height
	#define TEXTH 3 //text window height
	#define TEXTW X_EDGE //text window width

	WINDOW *game_border,*status_border,*opt_border,*text_border;
	main_win = newwin(Y_EDGE,X_EDGE,1,1);
	opt_win = newwin(OPTH,OPTW,Y_EDGE-OPTH+1,X_EDGE+3);
	status_win = newwin(STATH,STATW,1,X_EDGE+3);
	text_win = newwin(TEXTH,TEXTW,Y_EDGE+3,1);
	game_border = newwin(Y_EDGE+2,X_EDGE+2,0,0);
	wborder(game_border,'|','|','-', '-', '+', '+', '+', '+');
	status_border = newwin(STATH+2,STATW+2,0,X_EDGE+2);
	wborder(status_border,'|','|','-', '-', '+', '+', '+', '+');
	opt_border = newwin(OPTH+2,OPTW+2,Y_EDGE-OPTH,X_EDGE+2);
	wborder(opt_border,'|','|','-', '-', '+', '+', '+', '+');
	text_border = newwin(TEXTH+2,TEXTW+2,Y_EDGE+2,0);
	wborder(text_border,'|','|','-', '-', '+', '+', '+', '+');
	wrefresh(game_border);
	wrefresh(status_border);
	wrefresh(opt_border);
	wrefresh(text_border);
	wrefresh(main_win);
}

void initColors(void) {
	start_color();
	init_pair(RED_COLOR,COLOR_RED,COLOR_BLACK);
	init_pair(GREEN_COLOR,COLOR_GREEN,COLOR_BLACK);
	init_pair(BLUE_COLOR,COLOR_BLUE,COLOR_BLACK);
	init_pair(YELLOW_COLOR,COLOR_YELLOW,COLOR_BLACK);
}

//initialize ncurses
void initScreen(void) {
	start_color();
	initscr();        //start ncurses
	noecho();	      //dont display key strokes
	cbreak();	     //disable line buffering
	curs_set(0);    //hide cursor
	keypad(stdscr,TRUE);  //enable keypad, needed for mouse.
	mousemask(ALL_MOUSE_EVENTS,NULL); //enable all mouse events
	refresh();
}

void makeWin(void) {
	initScreen();
	initColors();
	createWindows();
}