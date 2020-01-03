//---------------------------------------- headers ------------------------------------------------
#include "units.h"
#include "macros.h"
#include <time.h>
#include <ncurses.h>

//---------------------------------------- prototypes ----------------------------------------------
void updateAPDisplay(void);
void updateLastDetected(void);
void updateAOETorDisplay(void);
void updateHealthDisplay(void);
void updateLocationDisplay(void);
void printPlayerSub(void);
void printOptWin(void);
void updateBoard(void);
void printToMain(const int x, const int y, const char *const str, const int color);
void printToTxtScr(const int x, const int y, const char *const str);
void printPieces(void);
void displayAOE(const int x,const int y,const int color, const int delay);
void displaySonar(SHIP *const ship);
static void printNumSign(const int i,const int j, const int x, const int y, const int color);
void animateTorpedo(const int limit);
void printToOptWin(const int x, const int y, const char *const str, const int color);
void printOptWin(void);
void printLastDetected(SHIP *const ship);

//---------------------------------------- prototypes in other files ------------------------------
void checkIfDetected(const SHIP *const sub);

//--------------------------------------- global vars -----------------------------------------------
extern WINDOW *main_win,*opt_win,*status_win,*text_win;