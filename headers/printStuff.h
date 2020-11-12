//---------------------------------------- include guard ------------------------------------------
#ifndef PRINTSTUFF_M
#define PRINTSTUFF_M

//--------------------------------------- headers -------------------------------------------------
#include "units.h"

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
void animateTorpedo(const int limit);
void printToOptWin(const int x, const int y, const char *const str, const int color);
void printOptWin(void);
void printLastDetected(SHIP *const ship);

#endif

