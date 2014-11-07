#ifndef ipc_h 
#define ipc_h 


#define CMD_GOTO_X_Y 		100
#define CMD_ATTACK_PID 		101
#define CMD_USEITEM_X 		102
#define CMD_GRABITEM_X_Y 	103
#define CMD_QUIT 		104

typedef struct cmd_t {

	int command ; 
	int pid; 
	int x ; 
	int y ; 
}cmd_t; 



#endif 
