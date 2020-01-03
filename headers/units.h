//---------------------------------------- typedefs,enums,consts ----------------------------------

enum TYPE {SUBMARINE,SURFACESHIP,CARGOSHIP};
enum DIRECTION {UP,DOWN,FORWARD,BACK,LEFT,RIGHT};

typedef struct SHIP SHIP;

typedef struct SUB {
	int num_aoetor,last_detected,using_aoe,turns_sonar,used_sonar;
}SUB;

typedef struct SURFACE {
	int num_charges, dropped_charge;
}SURFACE;

typedef union SHIPTYPE{
	SUB      *sub;  //submarine
	SURFACE  *surface;  //surface ship
}SHIPTYPE;

typedef struct dectree { //decision tree
	int    (*decFunc)(SHIP *const ship, void *dec);
	char    *c;
	int     num;
	struct  dectree *leftdec,*rightdec;
}dectree;

struct SHIP {
	int        health,last_knownx,last_knowny,last_knownz,x,y,z,prev_x,prev_y,prev_z,turbo,ap,detected,shot_at;
	enum       TYPE type;  //type of ship. sub,surface ship, or cargo ship
	enum       DIRECTION direction_facing;  //direction the ship is currently facing
	SHIPTYPE  *typeship; //union which holds pointer to sub or surface struct depending on what type SHIP is
	dectree   *dtree;
};

typedef struct ENEMIES { //used to make linked list to hold enemy ships
	SHIP    *ship;
	struct   ENEMIES *next;
}ENEMIES;



//----------------------------------------  global vars -------------------------------------------

extern SHIP *player_sub; //player controlled submarine
extern ENEMIES *enemies; //head of linked list which holds enemy ships
extern int num_enemies;
