//---------------------------------------- headers ------------------------------------------------
#include "makeWin.h"
#include "macros.h"
//---------------------------------------- prototypes ----------------------------------------------
static void createWindows(void);
static void initScreen(void);
static void initColors(void);
static int checkClickLocation(const int x, const int y);
static void getInput(void);
static void makeStartScr(void);
static void clearStartScr(void);

//----------------------------------------  global vars -------------------------------------------

WINDOW *main_win,*opt_win,*status_win,*text_win,*start_win,*start_border;
int DIFFICULTY = 2;

//---------------------------------------- code ---------------------------------------------------

static void createWindows(void) {
	#define OPTW 15  //opt window width
	#define OPTH 4   //opt window height
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

//checks location of mouse click to see if user clicked the appropriate box for diccifulty settings
static int checkClickLocation(const int x, const int y) {
	if(x >= (X_EDGE/2)-17 && x <= (X_EDGE/2)-15) {  //if mouse x is within x coordinates for checkbox
		switch(y) {
			case (Y_EDGE/2) + 1: 
				DIFFICULTY = 1;
				return 0;
			case (Y_EDGE/2) + 2: 
				DIFFICULTY = 2;
				return 0;
			case (Y_EDGE/2) + 3: 
				DIFFICULTY = 3;
				return 0;
			case (Y_EDGE/2) + 4: 
				DIFFICULTY = 4;
				return 0;
			default: return 1;
		}
	}
	else {
		return 1;
	}
}

//gets user input for DIFFICULTY of game
static void getInput(void){
	int loop_control = 1;  //controls whole loop. 1 = loop, 0 = stop looping
	MEVENT event;
	while (loop_control) {
		switch(getch()) {  //get user input to select diffculty. will keep looping utill correct choice is entered
			case KEY_MOUSE:
				switch(getmouse(&event)) { 
					case OK:
						if(event.bstate & BUTTON1_CLICKED){ //if left mouse button was clicked
							wmouse_trafo(start_win, &event.y, &event.x,FALSE);  //converts x and y to window relative points
							loop_control = checkClickLocation(event.x,event.y);
						}
						break;
					default: //do nothing
						break;
				}
				break;
			case '1': 
				DIFFICULTY = 1;
				loop_control = 0;
				break;
			case '2': 
				DIFFICULTY = 2;
				loop_control = 0;
				break;
			case '3': 
				DIFFICULTY = 3;
				loop_control = 0;
				break;
			case '4': 
				DIFFICULTY = 4;
				loop_control = 0;
				break;
			default: //do nothing
				break;
		}
	}
}


//makes screen which allows user to select DIFFICULTY
static void makeStartScr(void) {
	start_win = newwin(Y_EDGE,X_EDGE,1,1);
	start_border = newwin(Y_EDGE+2,X_EDGE+2,0,0);
	wborder(start_border,'|','|','-', '-', '+', '+', '+', '+');	
	wrefresh(start_win);
	wrefresh(start_border);
	mvwprintw(start_win,Y_EDGE/2,(X_EDGE/2)-23,"please choose DIFFICULTY (click box or type number):");
	mvwprintw(start_win,(Y_EDGE/2) + 1,(X_EDGE/2)-17,"[ ](1) easy");
	mvwprintw(start_win,(Y_EDGE/2) + 2,(X_EDGE/2)-17,"[ ](2) normal");
	mvwprintw(start_win,(Y_EDGE/2) + 3,(X_EDGE/2)-17,"[ ](3) difficult");
	mvwprintw(start_win,(Y_EDGE/2) + 4,(X_EDGE/2)-17,"[ ](4) very difficult");
	wrefresh(start_win);
	wrefresh(start_border);
}	

static void clearStartScr(void) {
	wclear(start_win);
	wclear(start_border);
	wrefresh(start_win);
	wrefresh(start_border);
}

static void initColors(void) {
	start_color();
	init_pair(RED_COLOR,COLOR_RED,COLOR_BLACK);
	init_pair(GREEN_COLOR,COLOR_GREEN,COLOR_BLACK);
	init_pair(BLUE_COLOR,COLOR_BLUE,COLOR_BLACK);
	init_pair(YELLOW_COLOR,COLOR_YELLOW,COLOR_BLACK);
}

//initialize ncurses
static void initScreen(void) { 
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
	makeStartScr();
	getInput();
	clearStartScr();
	createWindows();
}
