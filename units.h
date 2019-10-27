enum TYPE {SUBMARINE,SURFACESHIP,CARGOSHIP};

typedef struct SUB {
	int num_aoetor,last_detected;
}SUB;

typedef struct SURFACE {
	int num_charges,charges_dropped,shots_fired;
}SURFACE;

typedef union SHIPS{
	SUB *sub;  //submarine
	SURFACE *surface;  //surface ship
}SHIPS;

typedef struct SHIP {
	int health,last_knownx,last_knowny,last_knownz,x,y,z,turbo;
	enum TYPE type;  //type of ship. sub,surface ship, or cargo ship
	SHIPS *ship; //union which hlds pointer to sub or surface struct depending on what type SHIP is
}SHIP;