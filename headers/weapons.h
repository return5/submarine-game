//---------------------------------------- include guard ------------------------------------------
#ifndef WEAPONS_M
#define WEAPONS_M
//---------------------------------------- headers ------------------------------------------------
#include "units.h"

//---------------------------------------- prototypes ----------------------------------------------
void shootDepthCharge(SHIP *const ship, const int x_off,const int y_off,const int z);
void subShootAOE(SHIP *const ship, const int x_off, const int y_off);
void shootCannon(const int x_off,const int y_off); 
void computerShootTor(const int hit_or_miss);
void checkIfDetected(const SHIP *const sub);
int shootAOE(SHIP *const ship, void *dec);  
int shootTor(SHIP *const ship, void *dec); 
void shipDetected(SHIP *const ship);
void setTorpedoFireLine(void);
int getX(const int limit);
int getY(const int limit);
void updateShotAt(void); 

#endif

