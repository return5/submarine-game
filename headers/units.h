#include <ncurses.h> 

enum TYPE {SUBMARINE,SURFACESHIP,CARGOSHIP};
enum DIRECTION {UP,DOWN,FORWARD,BACK,LEFT,RIGHT};

typedef struct SUB {
	int num_aoetor,last_detected,using_aoe;
}SUB;

typedef struct SURFACE {
	int num_charges,charges_dropped,shots_fired;
}SURFACE;

typedef union SHIPS{
	SUB *sub;  //submarine
	SURFACE *surface;  //surface ship
}SHIPS;

typedef struct SHIP {
	int health,last_knownx,last_knowny,last_knownz,x,y,z,turbo,ap,direction_facing,detected;
	enum TYPE type;  //type of ship. sub,surface ship, or cargo ship
	SHIPS *ship; //union which hlds pointer to sub or surface struct depending on what type SHIP is
}SHIP;

typedef struct ENEMIES { //used to make linked list to hold enemy ships
	SHIP *ship;
	struct ENEMIES *next;
}ENEMIES;

WINDOW *main_win,*opt_win,*status_win,*text_win;
SHIP *player_sub; //player controlled submarine
ENEMIES *enemies; //head of linked list which holds enemy ships