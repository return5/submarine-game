//---------------------------------------- include guard ------------------------------------------
#ifndef MAKEWIN_M
#define MAKEWIN_M

//---------------------------------------- headers -------------------------------------------------
#include <ncurses.h>

//---------------------------------------- prototypes ----------------------------------------------
void makeWin(void);

//--------------------------------------- global vars -----------------------------------------------
extern WINDOW *main_win,*opt_win,*status_win,*text_win,*log_win;
extern int difficulty;

#endif

