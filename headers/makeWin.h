//---------------------------------------- include guard ------------------------------------------
#ifndef MAKEWIN_M
#define MAKEWIN_M

#include <ncurses.h> 
#include "macros.h"
//---------------------------------------- prototypes ----------------------------------------------
void makeWin(void);
static void createWindows(void);
static void initScreen(void);
static void initColors(void);
static int checkClickLocation(const int x, const int y);
static void getInput(void);
static void makeStartScr(void);
static void clearStartScr(void);

//--------------------------------------- global vars -----------------------------------------------
extern WINDOW *main_win,*opt_win,*status_win,*text_win,*log_win;
extern int difficulty;

#endif

