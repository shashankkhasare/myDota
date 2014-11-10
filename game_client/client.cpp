#include "client.h"
#include "../common/communication_structures.h"
#include "../common/bcast.h"
#include "../common/defines.h"
#include "../common/ipc.h"
#include "../common/defines.h"
#include "../Widget.h"
#include "../Entity.h"
#include<stdlib.h>
#include<unistd.h>
#include<vector>
#include<map>

#define TEAM_A_NAME "MinioN-Dota Team-A"
#define TEAM_B_NAME "MinioN-Dota Team-B"

#define TA_X 	22
#define TA_Y 	21
#define TB_X 	46
#define TB_Y 	45

#define debug 		1
#define debugcommand 	1
#define debugbcast 		0

#define MAGIC 2 
#define MELEE 1

#define P0CHAR 		'P'
#define P1CHAR 		'Q'
#define P2CHAR 		'R'
#define P3CHAR 		'S'

#define M1_IMAGE "images/m1.bmp"
#define M2_IMAGE "images/m2.bmp"
#define M3_IMAGE "images/m3.bmp"
#define M4_IMAGE "images/m4.bmp"

#define WIN 10 
#define LOSS 11
#define INPROGRESS 12

using namespace std; 
int getSocket(char *ip, int  port);



player_data_t playerdata[4] = {0};
player_data_t temple_a_data = {TEMPLE_A_ID, 2000, 0, -1, TEAM_A};
player_data_t temple_b_data = {TEMPLE_B_ID, 2000, 0, -1, TEAM_B}; 
magic_data_t magicdata;


//Entity p0_e, p1_e, p2_e, p3_e; 


// input command socket and bcast socket 
int ipc_sock; 
int bcast_sock;

int pid;
int gamemode ;
int gamestate =INPROGRESS;

bool setMouseTarget = false; 
bool setMouseGrab = false; 
bool isMdown = false; 
bool isShift = false;
int mouse_x  =0 , mouse_y=0; 
vector<Entity*> enemies;

//map<int , map<int, Widget* > > itemmap;

int state = STATE_BEFORE_START; 

char terrain[75][75];

void print_terrain()
{
	for ( int i =0 ; i < 75 ; i ++ ) 
	{ 
		for ( int j =  0 ; j < 75  ;  j++)
			cout << terrain[i][j] << " " ; 
		cout << endl; 
	}
}

void setGameState(){
	int selfteam = playerdata[pid].team;
	if(temple_a_data.health ==0 || temple_b_data.health ==0){
		if ( temple_a_data.health == 0){
			if ( temple_a_data.team == selfteam)
				gamestate = LOSS;
			else 
				gamestate = WIN;
		}else{
			if ( temple_b_data.team = selfteam) 
				gamestate = LOSS; 
			else 
				gamestate = WIN;
		}
	}
}




bool point_not_invisible(int x , int y ){

	int selfteam = playerdata[pid].team;
	if ( selfteam == TEAM_A){
		if ( x - y > 12 ) 
			return false;
		else 
			return true;

	}else
	{
		if ( y - x > 12)
			return false;
		else 
			return true;

	}
}
void update_entities(Entity *p0_e, Entity *p1_e,Entity *p2_e,Entity *p3_e, Entity * ta_e, Entity * tb_e){

	p0_e->setHealth(playerdata[0].health);
	p1_e->setHealth(playerdata[1].health);
	p2_e->setHealth(playerdata[2].health);
	p3_e->setHealth(playerdata[3].health);

	p0_e->setNitro(playerdata[0].nitro * 20 );
	p1_e->setNitro(playerdata[1].nitro * 20 );
	p2_e->setNitro(playerdata[2].nitro * 20 );
	p3_e->setNitro(playerdata[3].nitro * 20 );

	ta_e->setHealth(temple_a_data.health  / 20 );
	tb_e->setHealth(temple_b_data.health  / 20 );

}
void identify_enemies(Entity *p0_e, Entity *p1_e,Entity *p2_e,Entity *p3_e, Entity * ta_e, Entity * tb_e){
	int selfteam; 
	selfteam = playerdata[pid].team ; 
	if ( playerdata[0].team != selfteam){
		enemies.push_back(p0_e);
	}
	if ( playerdata[1].team != selfteam){
		enemies.push_back(p1_e);
	}
	if ( playerdata[2].team != selfteam){
		enemies.push_back(p2_e);
	}
	if ( playerdata[3].team != selfteam){
		enemies.push_back(p3_e);
	}
	if ( temple_a_data.team != selfteam){
		enemies.push_back(ta_e);
	}
	if ( temple_b_data.team != selfteam){
		enemies.push_back(tb_e);
	}
	if ( debug ) cout << "Enemies identified and saved in the enemies list \n";
	if ( debug ) cout << "Their locations are :  \n";
	if ( enemies.size() != 3)
		cout << "Error:: Enemy count mismatch!!! Found " << enemies.size() << " enemies \n";
	else{
		cout << (enemies[0] -> getRectAddr()) -> x << (enemies[0] -> getRectAddr())->y << endl;
		cout << (enemies[1] -> getRectAddr())->x << (enemies[0] -> getRectAddr())->y << endl;
		cout << (enemies[2] -> getRectAddr())->x << (enemies[0] -> getRectAddr())->y << endl;
	}
}
void send_attack_pid_command(int p){
	cmd_t cmd; 
	header head;
	cmd.command = CMD_ATTACK_PID;
	if ( isMdown){
		cmd.attack_mode = MAGIC;
		if ( debugcommand ) cout << "Magic attack \n";
	}
	else{
		cmd.attack_mode = MELEE;
		if ( debugcommand ) cout << "melee attack \n";
	}
	cmd.pid = p; 
	if ( debugcommand) cout << "debugcommand:: sending the command to server CMD_ATTACK_PID \n";
	send(ipc_sock, &cmd, sizeof(cmd), 0);
	recv(ipc_sock, &head, sizeof(head), 0 ) ; 
	if ( head.type == OK ) 
	{
		if ( debugcommand) cout << "debugcommand:: send the command and received ok from server \n";
	}else 
		cout << "Error :: unknown reply from server for the CMD_ATTACK_PID command \n";

}
void send_useitem_x_command(int item ) 
{
	cmd_t cmd; 
	header head; 

	cmd.command = CMD_USEITEM_X;
	cmd.x = item; 
	if ( debugcommand ) cout << "debugcommand :: sending the command to server CMD_USEITEM_X "<< item << endl;
	send(ipc_sock, &cmd, sizeof(cmd), 0);
	recv(ipc_sock, &head, sizeof(head), 0 ) ; 
	if ( head.type == OK ) 
	{
		if ( debugcommand ) cout << "debugcommand :: send the command and received ok from server \n";
	}else 
		cout << "Error :: unknown reply from server for the CMD_USEITEM_X command \n";

}

void send_grab_item_x_y_command( int x, int y ) {
	cmd_t cmd; 
	header head; 
	cmd.command = CMD_GRABITEM_X_Y;
	cmd.x = x; 
	cmd.y = y; 
	if ( debugcommand ) cout << "debugcommand :: sending the command to server CMD_GRABITEM_X_Y\n";
	send(ipc_sock, &cmd, sizeof(cmd), 0);
	recv(ipc_sock, &head, sizeof(head), 0 ) ; 
	if ( head.type == OK ) 
	{
		if ( debugcommand ) cout << "debugcommand :: send the command and received ok from server \n";
	}else 
		cout << "Error :: unknown reply from server for the CMD_GRABITEM_X_Y command \n";

}
void send_goto_x_y_command(int x, int y ) {
	cmd_t cmd; 
	header head; 
	cmd.command = CMD_GOTO_X_Y ;
	cmd.x = x; 
	cmd.y = y; 
	if ( debugcommand ) cout << "debugcommand :: sending the command to server CMD_GOTO_X_Y\n";
	send(ipc_sock, &cmd, sizeof(cmd), 0);
	recv(ipc_sock, &head, sizeof(head), 0 ) ; 
	if ( head.type == OK ) 
	{
		if ( debugcommand ) cout << "debugcommand :: send the command and received ok from server \n";
	}else 
		cout << "Error :: unknown reply from server for the CMD_GOTO_X_Y command \n";

}

void send_hid_and_settle_team(int sock)
{

	SDL_Init(SDL_INIT_EVERYTHING);
	intdata d; 

	SDL_Surface *screen;
	screen = SDL_SetVideoMode(750 , 750 ,32 , SDL_SWSURFACE);




	Widget menu("images/wfree750.bmp", NULL);
	menu.setLocation(0, 0);
	menu.setDim(750 , 750 );




	Widget h1("images/h1.bmp", "images/h1_hover.bmp", 125, 125, 200, 200);
	Widget h2("images/h2.bmp", "images/h2_hover.bmp", 125, 425, 200, 200);
	Widget h3("images/h3.bmp", "images/h3_hover.bmp", 425, 125, 200, 200);
	Widget h4("images/h4.bmp", "images/h4_hover.bmp", 425, 425, 200, 200);



	Widget teama("images/teama.bmp", "images/teama_hover.bmp", 222,240, 324, 86);
	Widget teamb("images/teamb.bmp", "images/teamb_hover.bmp", 222,426, 306, 86);

	Widget teama_full("images/full.bmp", NULL, 222, 240, 381, 86);
	Widget teamb_full("images/full.bmp", NULL, 222, 426, 381, 86);
	Widget allteamsfull("images/allteamsfull.bmp", NULL, 95, 333, 561, 86);




	int running = 1;
	int team_screen = 0 ; 
	int isteamafull = 0 ; 
	int isteambfull = 0 ; 
	int areallteamsfull = 0;

	while(running){
		Uint32 start = SDL_GetTicks();


		SDL_Event e; 
		while(SDL_PollEvent(&e)){
			switch(e.type){
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					if(e.key.keysym.sym == SDLK_ESCAPE)
						running = 0;
					break;	

				case SDL_MOUSEMOTION:
					h1.handleHover(e);
					h2.handleHover(e);
					h3.handleHover(e);
					h4.handleHover(e);
					teama.handleHover(e);
					teamb.handleHover(e);
					break;

				case SDL_MOUSEBUTTONDOWN: 
					if ( areallteamsfull)
					{
						running = 0 ; 
						exit(0);
						break;
					}
					if ( e.button.button == SDL_BUTTON_LEFT){
						cout << e.button.x << "  " << e.button.y ; 
						if ( !team_screen){
							if ( h1.isInRange(e.button.x, e.button.y )){
								d.data = 0; 
								send(sock, &d, sizeof(d), 0);
								recv(sock, &d, sizeof(d), 0);
								if ( debug ) if ( d.data == OK ) cout << "debug :: Ok received for hid \n";
								if ( d.data == REJECTED)
									areallteamsfull = 1 ; 
								else
									team_screen =1	;
							}else if ( h2.isInRange(e.button.x, e.button.y )){
								d.data = 1; 
								send(sock, &d, sizeof(d), 0);
								recv(sock, &d, sizeof(d), 0);
								if ( debug ) if ( d.data == OK ) cout << "debug :: Ok received for hid \n";
								if ( d.data == REJECTED)
									areallteamsfull = 1 ; 
								else
									team_screen =1	;
							}else if ( h3.isInRange(e.button.x, e.button.y )){
								d.data = 2; 
								send(sock, &d, sizeof(d), 0);
								recv(sock, &d, sizeof(d), 0);
								if ( debug ) if ( d.data == OK ) cout << "debug :: Ok received for hid \n";
								if ( d.data == REJECTED)
									areallteamsfull = 1 ; 
								else
									team_screen =1	;
							}else if ( h4.isInRange(e.button.x, e.button.y )){
								d.data = 3; 
								send(sock, &d, sizeof(d), 0);
								recv(sock, &d, sizeof(d), 0);
								if ( debug ) if ( d.data == OK ) cout << "debug :: Ok received for hid \n";
								if ( d.data == REJECTED)
									areallteamsfull = 1 ; 
								else
									team_screen =1	;
							} 
						}
						else{
							if (debug) cout << "debug :: Click in team screen " << endl; 

							if ( teama.isInRange(e.button.x, e.button.y )){
								if (debug) cout << "debug :: Team 1 selected " << endl; 
								d.data = TEAM_A; 
								send(sock, &d, sizeof(d), 0);
								recv(sock, &d, sizeof(d), 0);
								if ( d.data == REJECTED)
								{
									if (debug) cout << "debug :: TEam 1 full" << endl; 
									isteamafull = 1; 
								}else
								{

									if (debug) cout << "debug :: Team 1 selection successful " << endl; 
									running = 0 ; 
								}

							}else  if ( teamb.isInRange(e.button.x, e.button.y )){
								if (debug) cout << "debug :: Team 2 selected " << endl; 
								d.data = TEAM_B; 
								send(sock, &d, sizeof(d), 0);
								recv(sock, &d, sizeof(d), 0);
								if ( d.data == REJECTED)
								{
									if (debug) cout << "debug :: Team b full" << endl; 
									isteambfull = 1; 
								}else
								{
									if (debug) cout << "debug :: Team b selection successful " << endl; 
									running = 0 ; 
								}
							}
						}



					}else
					{
						cout << "Right button click on  : (" << e.button.x << " , " << e.button.y << ")" << endl; 
					}
					break;

			}
		}
		SDL_BlitSurface(menu.getSurface(), NULL, screen, menu.getRectAddr());
		if ( team_screen)
		{
			if (isteamafull)
				SDL_BlitSurface(teama_full.getSurface(), NULL, screen, teama_full.getRectAddr());
			else
				SDL_BlitSurface(teama.getSurface(), NULL, screen, teama.getRectAddr());
			if ( isteambfull)
				SDL_BlitSurface(teamb_full.getSurface(), NULL, screen, teamb_full.getRectAddr());
			else
				SDL_BlitSurface(teamb.getSurface(), NULL, screen, teamb.getRectAddr());

		} else if ( areallteamsfull){

			SDL_BlitSurface(allteamsfull.getSurface(), NULL, screen, allteamsfull.getRectAddr());
		}else 
		{

			SDL_BlitSurface(h1.getSurface(), NULL, screen, h1.getRectAddr());
			SDL_BlitSurface(h2.getSurface(), NULL, screen, h2.getRectAddr());
			SDL_BlitSurface(h3.getSurface(), NULL, screen, h3.getRectAddr());
			SDL_BlitSurface(h4.getSurface(), NULL, screen, h4.getRectAddr());
		}
		SDL_Flip(screen);

		if(1000/30 > SDL_GetTicks()-start)
			SDL_Delay(1000/30 - (SDL_GetTicks()-start));
	}
	SDL_Quit();
}


Client::Client(char * address, int port){
	// setting vars 
	//gamemode = m; 


	// open input channel and send hid 
	ipc_sock = getSocket(address, port );	
	if ( ipc_sock == -1)
		exit(0);
	header h ; 
	intdata d ; 
	h.type = OPEN_INPUT_CHANNEL; 
	send(ipc_sock, &h, sizeof(h), 0);
	recv(ipc_sock, &d, sizeof(d), 0); 

	if ( debug && d.data== OK)
		cout << "debug :: Received ok "<< endl;
	// get hid and settle team 
	send_hid_and_settle_team(ipc_sock);
	send(ipc_sock, &d, sizeof(d), 0);
	// receive pid and store it 
	recv(ipc_sock, &d, sizeof(d), 0); 
	pid = d.data; 
	cout << "Player id : " << pid << endl ; 
	// open bcast channel and start receiving the data ( use a thread to do it) 
	bcast_sock = getSocket(address, port); 	
	if ( bcast_sock == -1)
		exit(0);
	h.type = OPEN_BCAST_CHANNEL;
	send(bcast_sock, &h, sizeof(h), 0);
	send(bcast_sock, &d, sizeof(d), 0);
	recv(bcast_sock, &h, sizeof(h), 0); 
	if(h.type == OK ){
		if ( debug ) cout << "debug :: received OK for opening bcast channel \n";
	}
	else 
		cout << "Invalid reply from server for open bcast channel \n";
}


void *  bcast_receiver(void *This){

	header h, reply ;  
	if ( debugbcast) cout << "debugbcast:: Started the bcast receiver " << endl; 
	while ( true){
		recv(bcast_sock, &h, sizeof(h), 0);
		reply.type = OK;
		send(bcast_sock, &reply, sizeof(reply), 0); 

		if (h.type == BCAST_TERRAIN){
			if ( debugbcast) cout << "debugbcast:: Ok sent waiting for terrain bcast data \n";
			int actual, expected;
			int index, offset, ret; 
			expected = 75 * 75; 
			actual = 0 ; 
			while(actual != expected){
				index = actual / 75 ; 
				offset = actual % 75;
				ret = recv(bcast_sock, terrain[index] + offset  , 75 * 75 - actual , 0); 
				if ( ret != -1 ) 
				{
					actual += ret ; 

				}else {
					cout << "Error :: recv returned -1 \n";

				}
				if ( debugbcast) cout << "Terrain received : " << actual << " / " << expected  << endl; 
			}
			reply.type = OK;
			send(bcast_sock, &reply, sizeof(reply), 0); 
			if ( debugbcast) cout << "debugbcast:: received terrain and replied ok to it\n";

		}else if ( h.type == BCAST_PLAYER_DATA){
			player_data_t pdata;
			if ( debugbcast) cout << "debugbcast:: Ok sent waiting for the player bcast data  \n";
			recv(bcast_sock, &pdata,sizeof(pdata) , 0); 
			reply.type = OK;
			send(bcast_sock, &reply, sizeof(reply), 0); 
			if ( debugbcast) cout << "debugbcast:: received player data and replied ok\n";
			if ( pdata.id >= 0 && pdata.id <= 3) {
				if ( debugbcast) cout << "Received data of player " << pdata.id;
				if ( debugbcast) cout << " Health : " << pdata.health; 
				if ( debugbcast) cout << " Nitro  : " << pdata.nitro; 
				if ( debugbcast) cout << " HID    : " << pdata.hid; 
				if ( debugbcast) cout << " Team   : " << (pdata.team ? "TEAM B " : "TEAM A" ) << endl; 

				playerdata[pdata.id] = pdata;

			}else if ( pdata.id == TEMPLE_A_ID ) {
				if ( debugbcast) cout << "Received data of temple a \n";
				temple_a_data = pdata;
				if ( debugbcast) cout << "Received data temple a \n";
				if ( debugbcast) cout << " Health : " << pdata.health; 
				if ( debugbcast) cout << " Nitro  : " << pdata.nitro; 
				if ( debugbcast) cout << " HID    : " << pdata.hid; 
				if ( debugbcast) cout << " Team   : " << (pdata.team ? "TEAM B " : "TEAM A") ; 
			}else if ( pdata.id == TEMPLE_B_ID ) {
				if ( debugbcast) cout << "Received data of temple b \n";
				temple_b_data = pdata;
			}else {
				if(debugbcast) cout << "Received data  with invalid id : " << pdata.id << endl;
			}
		}else if ( h.type == BCAST_GAME_STATE ){
			if(debugbcast) cout << "Game state change bcast received " << endl; 
			intdata d; 
			recv(bcast_sock, &d, sizeof(d), 0);
			reply.type = OK;
			send(bcast_sock, &reply, sizeof(reply), 0); 
			if ( d.data == STATE_BEFORE_START ) 
			{
				if ( debugbcast) cout << "Game state changed to : " << "before start\n";
				state = d.data; 
			}else if ( d.data == STATE_STARTED ) {
				if ( debugbcast) cout << "Game state changed to : " <<  " started \n " ; 
				state = d.data; 

			}else if ( d.data == STATE_PAUSED){
				if ( debugbcast) cout << "Game state changed to : " << "paused \n "; 
				state = d.data; 
			}else {
				if ( debugbcast) cout << "Unknown game state received \n";  
			}	
		}else if ( h.type == BCAST_MAGIC){
			if ( debugbcast) cout << "debugbcast:: Ok sent waiting for the magic bcast data  \n";
			recv(bcast_sock, &magicdata,sizeof(magicdata) , 0); 
			reply.type = OK;
			send(bcast_sock, &reply, sizeof(reply), 0); 

		}
		else {
			if(debugbcast) cout << " Received unknown bcast message : " << h.type << endl; 
		}

	}
}
void Client::startAI(){

	pthread_t tid; 
	cmd_t cmd; 
	header head; 
	pthread_create(&tid, NULL , bcast_receiver, NULL);
	SDL_Init(SDL_INIT_EVERYTHING);
	while(1){
		int x = rand() % 74;
		sleep(3);
		send_goto_x_y_command(x, x);
	}

}
void Client::start(){

	// 1. start the thread for receiving the updates from the server and update the said data structures 
	// 		this thread will reply OK to each bcast message, which will be as a heartbeat to the server
	//
	// 2. This thread itself must read all the data structures and render the screen and also send the commands to the server on the ipc 
	//

	pthread_t tid; 
	cmd_t cmd; 
	header head; 
	pthread_create(&tid, NULL , bcast_receiver, NULL);
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface *screen; 
	screen = SDL_SetVideoMode(750,750,32,SDL_SWSURFACE);


	Widget fw("images/wfree750.bmp", NULL,0,0,750, 750 );
	Widget tw("images/tree.bmp",NULL, 0, 0, 15, 15 );
	Widget w("images/pillar.bmp", NULL, 0, 0, 15, 15);


	tw.deleteWhite(screen);
	w.deleteWhite(screen);






	//Items
	Widget item[7] = {Widget("images/items/nike.bmp", NULL), 
		Widget("images/items/supernike.bmp", NULL),
		Widget("images/items/hammer.bmp", NULL),
		Widget("images/items/superhammer.bmp", NULL),
		Widget("images/items/banana.bmp", NULL),
		Widget("images/items/redhot.bmp", NULL),
		Widget("images/items/toolbox.bmp", NULL)
	};
	for(int i=0;i<7;i++){
		item[i].setDim(20, 20) ; 
		item[i].deleteWhite(screen);
	}
	

	Entity p0_e(NULL, 170 , 190 , 30, 30) ; 
	Entity p1_e(NULL, 170 , 190 , 30, 30) ; 
	Entity p2_e(NULL, 170 , 190 , 30, 30) ; 
	Entity p3_e(NULL, 170 , 190 , 30, 30) ; 

	SDL_Surface * loading ; 
	loading = SDL_DisplayFormat(SDL_LoadBMP("images/loading.bmp"));
	SDL_BlitSurface(loading, NULL, screen,NULL);
	SDL_Flip(screen);

	// wait till the first bcast 
	// fix for captain america bug 
	while( playerdata[0].health == 0&& playerdata[1].health== 0  && playerdata[2].health == 0 && playerdata[3].health == 0){
		cout << "Health is " << playerdata[0].health + playerdata[1].health + playerdata[2].health + playerdata[3].health << endl;
		sleep(2);
	}
	SDL_FreeSurface(loading);

	//create_map();

	switch(playerdata[0].hid){
		case 0: 
			p0_e.setImage(M1_IMAGE);
			break;
		case 1: 
			p0_e.setImage(M2_IMAGE);
			break;
		case 2: 
			p0_e.setImage(M3_IMAGE);
			break;
		case 3: 
			p0_e.setImage(M4_IMAGE);
			break;
		default:
			cout << "FATAL ERROR : invalid HID \n";
	}
	switch(playerdata[1].hid){
		case 0: 
			p1_e.setImage(M1_IMAGE);
			break;
		case 1: 
			p1_e.setImage(M2_IMAGE);
			break;
		case 2: 
			p1_e.setImage(M3_IMAGE);
			break;
		case 3: 
			p1_e.setImage(M4_IMAGE);
			break;
		default:
			cout << "FATAL ERROR : invalid HID \n";
	}

	switch(playerdata[2].hid){
		case 0: 
			p2_e.setImage(M1_IMAGE);
			break;
		case 1: 
			p2_e.setImage(M2_IMAGE);
			break;
		case 2: 
			p2_e.setImage(M3_IMAGE);
			break;
		case 3: 
			p2_e.setImage(M4_IMAGE);
			break;
		default:
			cout << "FATAL ERROR : invalid HID \n";
	}
	switch(playerdata[3].hid){
		case 0: 
			p3_e.setImage(M1_IMAGE);
			break;
		case 1: 
			p3_e.setImage(M2_IMAGE);
			break;
		case 2: 
			p3_e.setImage(M3_IMAGE);
			break;
		case 3: 
			p3_e.setImage(M4_IMAGE);
			break;
		default:
			cout << "FATAL ERROR : invalid HID \n";
	}
	p0_e.setHealth(100); 
	p0_e.setNitro(100);
	p0_e.setBGScreen(screen);

	p1_e.setHealth(100); 
	p1_e.setNitro(100);
	p1_e.setBGScreen(screen);

	p2_e.setHealth(100); 
	p2_e.setNitro(100);
	p2_e.setBGScreen(screen);


	p3_e.setHealth(100); 
	p3_e.setNitro(100);
	p3_e.setBGScreen(screen);

	p0_e.load_image();
	p1_e.load_image();
	p2_e.load_image();
	p3_e.load_image();

	//Temples
	Entity ta_e("images/temple1.bmp", TA_X * 10 , TA_Y * 10 , 90, 95);
	ta_e.setHealth(100);
	ta_e.setNitro(0);
	ta_e.setBGScreen(screen);

	Entity tb_e("images/temple2.bmp",TB_X * 10 , TB_Y  * 10 , 90, 95);
	tb_e.setHealth(100);
	tb_e.setNitro(0);
	tb_e.setBGScreen(screen);

	ta_e.load_image();
	tb_e.load_image();

	// identify the enemies on the map
	identify_enemies(&p0_e, &p1_e, &p2_e, &p3_e, &ta_e, &tb_e);
	Widget target("images/target.bmp" , NULL, 0 , 0 , 20 , 20  ) ; 
	Widget grab("images/grab.bmp" , NULL, 0 , 0 , 30 , 19  ) ; 
	Widget magic("images/magic.bmp" , NULL, 0 , 0 , 30 , 30  ) ; 

	if ( playerdata[pid].team == TEAM_A){
		SDL_WM_SetCaption(TEAM_A_NAME, NULL);
	}else{
		SDL_WM_SetCaption(TEAM_B_NAME, NULL);
	}

	int running = 1;
	while(running){
		Uint32 start = SDL_GetTicks();
		SDL_Event e;

		//Background Grass	
		SDL_BlitSurface(fw.getSurface(), NULL, screen, fw.getRectAddr());
		update_entities(&p0_e, &p1_e, &p2_e, &p3_e, &ta_e, &tb_e);
		//update_itemmap();

		// Blit the terrain and the players 
		for(int i=0;i<75;i++){
			for(int j=0;j<75;j++){
				// items 
				if(terrain[i][j] >= '1' && terrain[i][j] <= '7'){
					item[terrain[i][j] -'1'].setLocation(10 * j, 10 * i);
					SDL_BlitSurface(item[terrain[i][j] - '1'].getSurface(),
					NULL, screen,
					item[terrain[i][j] - '1'].getRectAddr() );

				}else if(terrain[i][j] == 'J'){ // Jungle 
					tw.setLocation(10*j, 10*i);
					SDL_BlitSurface(tw.getSurface(), NULL, screen, tw.getRectAddr());
				}else if(terrain[i][j] == 'W'){ // Pillar 
					w.setLocation(10*j, 10*i);
					SDL_BlitSurface(w.getSurface(), NULL, screen, w.getRectAddr());
				}else if(terrain[i][j] == P0CHAR){
					p0_e.setLocation(10 * j , 10 * i);
					SDL_BlitSurface(p0_e.getSurface(), NULL, screen, p0_e.getRectAddr());
				}else if(terrain[i][j] == P1CHAR){
					p1_e.setLocation(10 * j , 10 * i);
					SDL_BlitSurface(p1_e.getSurface(), NULL, screen, p1_e.getRectAddr());
				}else if(terrain[i][j] == P2CHAR){
					p2_e.setLocation(10 * j , 10 * i);
					SDL_BlitSurface(p2_e.getSurface(), NULL, screen, p2_e.getRectAddr());
				}else if(terrain[i][j] == P3CHAR){
					p3_e.setLocation(10 * j , 10 * i);
					SDL_BlitSurface(p3_e.getSurface(), NULL, screen, p3_e.getRectAddr());
				}
			}
		}
		// temples 
		SDL_BlitSurface(ta_e.getSurface(), NULL, screen, ta_e.getRectAddr());
		SDL_BlitSurface(tb_e.getSurface(), NULL, screen, tb_e.getRectAddr());
		if ( setMouseTarget){
			SDL_ShowCursor(0);
			target.setLocation(mouse_x -10, mouse_y -10);
			target.deleteWhite(screen);
			magic.deleteWhite(screen);
			SDL_BlitSurface(target.getSurface(), NULL, screen, target.getRectAddr());

		}else if (setMouseGrab) {
			SDL_ShowCursor(0);
			grab.setLocation(mouse_x -10, mouse_y -10);
			grab.deleteWhite(screen);
			SDL_BlitSurface(grab.getSurface(), NULL, screen, grab.getRectAddr());
		}else {

			SDL_ShowCursor(1);
		}
	// magic spell 
		for ( int i = 0; i < 20; i++)
		{
			int x,  y ; 
			x = magicdata.x[i];
			y = magicdata.y[i];
			cout << x << " " << y << "---------------------------------->" << endl;
			if ( x < 0 || y < 0 ) 
				continue;
			magic.setLocation(x * 10, y*10);
			SDL_BlitSurface(magic.getSurface(), NULL, screen, magic.getRectAddr());
		}

		while(SDL_PollEvent(&e)){
			switch(e.type){
				case SDL_QUIT:
					running = 0;

					cmd.command = CMD_QUIT;
					if ( debugcommand) cout << "debugcommand:: sending the command to server CMD_QUIT \n";
					send(ipc_sock, &cmd, sizeof(cmd), 0);
					recv(ipc_sock, &head, sizeof(head), 0 ) ; 
					if ( head.type == OK ) 
					{
						if ( debugcommand) cout << "debugcommand:: send the command and received ok from server \n";
					}else 
						cout << "Error :: unknown reply from server for the CMD_QUIT command \n";
					break;
				case SDL_KEYDOWN:
					switch(e.key.keysym.sym ) 
					{
						case SDLK_ESCAPE : 
							running = 0 ; 
							break; 
							/*
						case SDLK_1:
							send_useitem_x_command(1);
							break;
						case SDLK_2:
							send_useitem_x_command(2);
							break;
						case SDLK_3:
							send_useitem_x_command(3);
							break;
						case SDLK_4:
							send_useitem_x_command(4);
							break;
						case SDLK_5:
							send_useitem_x_command(5);
							break;
						case SDLK_6:
							send_useitem_x_command(6);
							break;
						case SDLK_7:
							send_useitem_x_command(7);
							break;
							*/
						case SDLK_n:
							if ( isShift)
								send_useitem_x_command(2);
							else
								send_useitem_x_command(1);
							break;
						case SDLK_h:
							if ( isShift)
								send_useitem_x_command(4);
							else
								send_useitem_x_command(3);
							break;
						case SDLK_b:
							send_useitem_x_command(5);
							break;
						case SDLK_r:
							send_useitem_x_command(6);
							break;
						case SDLK_t:
							send_useitem_x_command(7);
							break;
							
						case SDLK_LSHIFT:
						case SDLK_RSHIFT:
							isShift = true;
							break;
						case SDLK_m:
							isMdown = true;
							cout << "M is down " << endl;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					if ( e.key.keysym.sym == SDLK_m){
						isMdown = false;
						cout << "M is up " << endl;
					}else if ( e.key.keysym.sym == SDLK_RSHIFT || e.key.keysym.sym == SDLK_LSHIFT){
						isShift = false;
					}
					break;

				case SDL_MOUSEBUTTONDOWN: 
					if ( e.button.button == SDL_BUTTON_LEFT){

						int gridx, gridy; 
						gridx = gridy = -1; 
						for ( int i = 0 ; i < 3; i++){
							if(enemies[i] -> isInRange(e.button.x, e.button.y )){
								gridx = (enemies[i] -> getRectAddr() -> x) / 10.;
								gridy = (enemies[i] -> getRectAddr() -> y) / 10.;
							}
						}
						if ( debugcommand ) cout << "Enemy found at gridx, gridy " << gridx << " "  << gridy << endl; 
						if ( gridx != -1 && gridy != -1 ) {

							if ( gridx == TA_X && gridy == TA_Y)
							{
								if ( debugcommand) cout << "debug :: Before calling send_attack_pid_command temple a \n";
								send_attack_pid_command(TEMPLE_A_ID);
							}else if ( gridx == TB_X && gridy == TB_Y){
								if ( debugcommand) cout << "debug :: Before calling send_attack_pid_command temple b \n";
								send_attack_pid_command(TEMPLE_B_ID);
							}else if ( terrain[gridy][gridx] == P0CHAR){
								if ( debugcommand) cout << "debug :: Before calling send_attack_pid_command. p0\n";
								send_attack_pid_command(0);
							}else if ( terrain[gridy][gridx] == P1CHAR){
								if ( debugcommand) cout << "debug :: Before calling send_attack_pid_command. p1\n";
								send_attack_pid_command(1);
							}else if ( terrain[gridy][gridx] == P2CHAR){
								if ( debugcommand) cout << "debug :: Before calling send_attack_pid_command p2\n";
								send_attack_pid_command(2);
							}else if ( terrain[gridy][gridx] == P3CHAR){
								if ( debugcommand) cout << "debug :: Before calling send_attack_pid_command p3\n";
								send_attack_pid_command(3);
							}
						}


						// check if item grab attempted


						gridx = (int) (e.button.x / 10.);
						gridy = (int) (e.button.y / 10.);
						if (terrain[gridy][gridx] <= '7' && terrain[gridy][gridx] >= '1'){
							//if( point_not_invisible(gridx, gridy))
							send_grab_item_x_y_command(gridx, gridy);
						}else if (terrain[gridy][gridx - 1] <= '7' && terrain[gridy][gridx - 1] >= '1'){
							//if( point_not_invisible(gridx, gridy))
							send_grab_item_x_y_command(gridx - 1, gridy);
						}else if (terrain[gridy -1 ][gridx] <= '7' && terrain[gridy- 1][gridx] >= '1'){
							//if( point_not_invisible(gridx, gridy))
							send_grab_item_x_y_command(gridx, gridy -1 );
						}else if (terrain[gridy -1 ][gridx - 1] <= '7' && terrain[gridy- 1][gridx - 1] >= '1'){
							//if( point_not_invisible(gridx, gridy))
							send_grab_item_x_y_command(gridx -1 , gridy - 1 );
						}



					}else
					{
						// GOTO_X_Y  command 

						int gridx, gridy; 
						gridx = (int) (e.button.x / 10.);
						gridy = (int) (e.button.y / 10.);
						if( (terrain[gridy][gridx] == '.' || terrain[gridy][gridx] == 'w' ) 
								/*&& point_not_invisible(gridx, gridy)*/){
							if ( debugcommand) cout << "Coordinates " << e.button.x << " " << e.button.y << endl; 
							if ( debugcommand) cout << "Grid points " << gridx << " " << gridy << endl; 
							send_goto_x_y_command(gridx, gridy);
						}
					}


					break;
				case SDL_MOUSEMOTION:
					// set cursor to attack mode if entity under cursor is enemy
					mouse_x = e.button.x; 
					mouse_y = e.button.y ; 
					if(enemies[0] -> isInRange(e.button.x, e.button.y ) || 
							enemies[1] -> isInRange(e.button.x, e.button.y ) || 
							enemies[2] -> isInRange(e.button.x, e.button.y ))
					{
						setMouseTarget = true;
					}else{
						setMouseTarget = false;
					}

					// set cursor to grab if the widget under cursor is item
					int gridx = (int) (e.button.x / 10.);
					int gridy = (int) (e.button.y / 10.);
					if (terrain[gridy][gridx] <= '7' && terrain[gridy][gridx] >= '1'){
						setMouseGrab = true;
					}else if (terrain[gridy][gridx - 1] <= '7' && terrain[gridy][gridx - 1] >= '1'){
						setMouseGrab = true;
					}else if (terrain[gridy -1 ][gridx] <= '7' && terrain[gridy- 1][gridx] >= '1'){
						setMouseGrab = true;
					}else if (terrain[gridy -1 ][gridx - 1] <= '7' && terrain[gridy- 1][gridx - 1] >= '1'){
						setMouseGrab = true;
					}else{
						setMouseGrab = false;
					}
					break;
			}
		}
		setGameState();
		if ( gamestate != INPROGRESS ){
			SDL_Surface * result; 
			if ( gamestate == WIN )
				result = SDL_DisplayFormat(SDL_LoadBMP("images/win.bmp"));
			else
				result  = SDL_DisplayFormat(SDL_LoadBMP("images/loss.bmp"));
			SDL_BlitSurface(result, NULL, screen,NULL);
			SDL_Flip(screen);
			sleep(3);
			break;

		}
		SDL_Flip(screen);

		if(1000/30 > SDL_GetTicks()-start)
			SDL_Delay(1000/30 - (SDL_GetTicks()-start));
		//SDL_Delay(100);
	}
	SDL_Quit();

}

int main(int argc, char * argv[])
{

	if ( argc < 3 ) 
	{
		cout << "IP address and mode ( human / AIbot mode  )  exptected \n";
		cout << "Usage : ./client serverIP mode \n";
		exit(0);
	}

	char * address = argv[1];

	struct sockaddr_in server; 

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(address);
	server.sin_port = htons(8181);


	int gamemode = atoi(argv[2]);

	Client c(address, 8181);
	if ( gamemode == HUMAN){
		c.start();
	}else if ( gamemode == AI )
	{
		c.startAI();

	}else
	{
		cout << "Unknown mode specified \n";
	}
}

int getSocket(char *ip, int  port){
	int sock_fd;
	struct sockaddr_in server;
	if ( ( sock_fd = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
		cout << "Failed to open the socket \n";
	}
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr( ip );
	server.sin_port = htons(port);
	if( connect(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
		cout << "Unable to connect to " <<  ip << " "<< port;
		return -1;
	}
	return sock_fd;
}

