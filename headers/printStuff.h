#include "units.h"
void updateBoard(void);
void printToMain(const int x, const int y, const char *const str);
void printToTxtScr(const int x, const int y, const char *const str);
void printPieces(void);
void displayAOE(const int limit,const int color, const int delay);
void displaySonar(void);
void printNumSign(const int i,const int j, const int x, const int y, const int color);
void animateTorpedo(const int limit);
void printToOptWin(const int x, const int y, const char *const str);
void printOptWin(void);
void printLastDetected(SHIP *const ship);
