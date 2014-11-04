#ifndef COMM_STRUCT
#define COMM_STRUCT

#define OK 			0
#define OPEN_INPUT_CHANNEL 	1
#define OPEN_BCAST_CHANNEL 	2
#define ACCEPTED 		3 
#define REJECTED 		4 



#define BCAST_TERRAIN 		5
#define BCAST_HEALTH 		6
#define BCAST_GAME_STATE 	7
	

/*health of entity defined by type */
typedef struct health_t{
	int id  ; 
	int value; 
}health_t; 

			/* header and intdata */
typedef struct header{
	int type ; 
} header;

typedef struct intdata{
	int data ; 
}intdata; 


#endif
