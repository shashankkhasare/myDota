#ifndef bcast_h
#define bcast_h


#define BCAST_TERRAIN 		5
#define BCAST_GAME_STATE 	7
#define BCAST_PLAYER_DATA 	10
	
typedef struct player_data_t {

	int id; 
	int health; 
	int nitro; 
	int hid; 
	int team ; 


}player_data_t ; 


#endif 

