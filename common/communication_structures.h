#ifndef COMM_STRUCT
#define COMM_STRUCT

#define OK 			0
#define OPEN_INPUT_CHANNEL 	1
#define OPEN_BCAST_CHANNEL 	2
#define COMMAND_FOLLOWS 	3


typedef struct header{
	int type ; 
} header;

typedef struct intdata{
	int data ; 
}intdata; 


#endif
	