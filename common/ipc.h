#ifndef ipc_h 
#define ipc_h 


#define CMD_GOTO_X_Y 		0
#define CMD_ATTACK_PID 		1
#define CMD_USEITEM_X 		2
#define CMD_GRABITEM_X_Y 	3
#define CMD_QUIT 		4

typedef struct cmd_t {

	int command ; 
	int pid; 
	int x ; 
	int y ; 
}cmd_t; 



#endif 
