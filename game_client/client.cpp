#include "client.h"
#include "../common/communication_structures.h"
#include "../common/defines.h"
#include "../Widget.h"
#include<stdlib.h>

using namespace std; 
int getSocket(char *ip, int  port);

// healths 
int temple_a_health; 
int temple_b_health; 

int p0_health; 
int p1_health; 
int p2_health; 
int p3_health; 

// input command socket and bcast socket 
int ipc_sock; 
int bcast_sock;


int hid; 
int teamid; 
int mode; 
int pid; 

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
void send_hid_and_settle_team(int sock)
{

	SDL_Init(SDL_INIT_EVERYTHING);
	intdata d; 

	SDL_Surface *screen;
	screen = SDL_SetVideoMode(750 , 750 ,32,SDL_SWSURFACE);




	Widget menu("images/menu.bmp", NULL);
	menu.setLocation(0, 0);
	menu.setDim(750 , 750 );




	Widget h1("images/h1.bmp", "images/h1_hover.bmp", 10, 10, 200, 200);
	Widget h2("images/h2.bmp", "images/h2_hover.bmp", 250, 10, 200, 200);
	Widget h3("images/h3.bmp", "images/h3_hover.bmp", 10, 250, 200, 200);
	Widget h4("images/h4.bmp", "images/h4_hover.bmp", 250, 250, 200, 200);



	Widget teama("images/teama.bmp", NULL, 50, 50, 400, 87);
	Widget teamb("images/teamb.bmp", NULL, 50, 150, 400, 87);

	Widget teama_full("images/full.bmp", NULL, 50, 50, 400, 87);
	Widget teamb_full("images/full.bmp", NULL, 50, 150, 400, 87);
	Widget allteamsfull("images/allteamsfull.bmp", NULL, 50, 150, 441, 87);




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
								d.data = 1; 
								send(sock, &d, sizeof(d), 0);
								recv(sock, &d, sizeof(d), 0);
								if ( debug ) if ( d.data == OK ) cout << "debug :: Ok received for hid \n";
								if ( d.data == REJECTED)
									areallteamsfull = 1 ; 
								else
									team_screen =1	;
							}else if ( h2.isInRange(e.button.x, e.button.y )){
								d.data = 2; 
								send(sock, &d, sizeof(d), 0);
								recv(sock, &d, sizeof(d), 0);
								if ( debug ) if ( d.data == OK ) cout << "debug :: Ok received for hid \n";
								if ( d.data == REJECTED)
									areallteamsfull = 1 ; 
								else
									team_screen =1	;
							}else if ( h3.isInRange(e.button.x, e.button.y )){
								d.data = 3; 
								send(sock, &d, sizeof(d), 0);
								recv(sock, &d, sizeof(d), 0);
								if ( debug ) if ( d.data == OK ) cout << "debug :: Ok received for hid \n";
								if ( d.data == REJECTED)
									areallteamsfull = 1 ; 
								else
									team_screen =1	;
							}else if ( h4.isInRange(e.button.x, e.button.y )){
								d.data = 4; 
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


Client::Client(char * address, int port ,  int m){
	// setting vars 
	mode = m; 


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
	health_t health;
	if ( debug ) cout << "debug :: Started the bcast receiver " << endl; 
	while ( true){
		recv(bcast_sock, &h, sizeof(h), 0);
		reply.type = OK;
		send(bcast_sock, &reply, sizeof(reply), 0); 

		if (h.type == BCAST_TERRAIN){
			if ( debug ) cout << "debug :: Ok sent waiting for terrain bcast data \n";
			recv(bcast_sock, terrain, 75 * 75 , 0); 
			reply.type = OK;
			send(bcast_sock, &reply, sizeof(reply), 0); 
			if ( debug ) cout << "debug :: received terrain and replied ok to it\n";
			print_terrain();

		}else if ( h.type == BCAST_HEALTH){
			if ( debug ) cout << "debug :: Ok sent waiting for health bcast data \n";
			recv(bcast_sock, &health,sizeof(health) , 0); 
			reply.type = OK;
			send(bcast_sock, &reply, sizeof(reply), 0); 
			if ( health.id == 0 ) {
				if ( debug) cout << "Received health of player 0 : " << health.value << endl;
				p0_health = health.value;
			}else if ( health.id == 1 ) {
				if ( debug) cout << "Received health of player 1 : " << health.value << endl;
				p1_health = health.value;
			}else if ( health.id == 2 ) {
				if ( debug) cout << "Received health of player 2 : " << health.value << endl;
				p2_health = health.value;
			}else if ( health.id == 3 ) {
				if ( debug) cout << "Received health of player 3 : " << health.value << endl;
				p3_health = health.value;
			}else if ( health.id == TEMPLE_A_ID ) {
				if ( debug) cout << "Received health of temple a  : " << health.value << endl;
				temple_a_health = health.value;
			}else if ( health.id == TEMPLE_B_ID ) {
				if ( debug) cout << "Received health of temple b : " << health.value << endl;
				temple_b_health= health.value;
			}else {
				if(debug) cout << "Received health with unknown id : " << health.id << endl; 
			}
		}else if ( h.type == BCAST_GAME_STATE ){
			if(debug) cout << "Game state change bcast received " << endl; 
			intdata d; 
			recv(bcast_sock, &d, sizeof(d), 0);
			reply.type = OK;
			send(bcast_sock, &reply, sizeof(reply), 0); 
			if ( d.data == STATE_BEFORE_START ) 
			{
				if ( debug ) cout << "Game state changed to : " << "before start\n";
				state = d.data; 
			}else if ( d.data == STATE_STARTED ) {
				if ( debug ) cout << "Game state changed to : " <<  " started \n " ; 
				state = d.data; 

			}else if ( d.data == STATE_PAUSED){
				if ( debug ) cout << "Game state changed to : " << "paused \n "; 
				state = d.data; 
			}else {
				if ( debug ) cout << "Unknown game state received \n";  
			}

		}else {
			if(debug) cout << " Received unknown bcast message : " << h.type << endl; 
		}
	}
}
void Client::start(){

	// 1. start the thread for receiving the updates from the server and update the said data structures 
	// 		this thread will reply OK to each bcast message, which will be as a heartbeat to the server
	//
	// 2. This thread itself must read all the data structures and render the screen and also send the commands to the server on the ipc 
	//

	pthread_t tid; 
	pthread_create(&tid, NULL , bcast_receiver, NULL);
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface *screen, *fw, *tw, *w, *t, *it, *frame;
	screen = SDL_SetVideoMode(750,750,32,SDL_SWSURFACE);

	fw = SDL_DisplayFormat(SDL_LoadBMP("renderer/wfree750.bmp"));  //Required for transparnecy
	tw = SDL_DisplayFormat(SDL_LoadBMP("renderer/wtree.bmp"));
	w = SDL_DisplayFormat(SDL_LoadBMP("renderer/btree15_c.bmp"));
	t = SDL_DisplayFormat(SDL_LoadBMP("renderer/Temple1.bmp"));
	it = SDL_DisplayFormat(SDL_LoadBMP("renderer/items.bmp"));

	SDL_SetColorKey(tw, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	
	SDL_SetColorKey(w, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	
	SDL_SetColorKey(t, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	

	//Background Grass	
	int i, j;
	SDL_Rect r;
	r.w = 750;
	r.h = 750;
	r.x = 12;
	r.y = 12;
	SDL_BlitSurface(fw, NULL, screen, &r);

	//Temples
	r.w = 90;
	r.h = 90;
	r.x = 22*10;
	r.y = 21*10;
	SDL_BlitSurface(t, NULL, screen, &r);
	r.x = 46*10;
	r.y = 45*10;
	SDL_BlitSurface(t, NULL, screen, &r);	

	//Items
	SDL_Rect item[9];
	for(i=0;i<9;i++){
		item[i].x = 10*i;
		item[i].y = 0;
		item[i].w = 10;
		item[i].h = 10;
	}

	int running = 1;
	while(running){
		Uint32 start = SDL_GetTicks();
		SDL_Event e;

		for(i=0;i<75;i++)
			for(j=0;j<75;j++)
				if(terrain[i][j] >= '1' && terrain[i][j] <= '9'){
					r.x = 10*j;
					r.y = 10*i;
					SDL_BlitSurface(it, &item[terrain[i][j]-'1'], screen, &r);
				}

		//Jungle
		r.w = 15;
		r.h = 15;
		for(i=0; i<75; i++){
			for(j = 0; j<75; j++){
				if(terrain[i][j] == 'J'){
					r.x = 10*j;
					r.y = 10*i;
					SDL_BlitSurface(tw, NULL, screen, &r);
				}
				else if	(terrain[i][j] == 'W'){
					r.x = 10*j;
					r.y = 10*i;
					SDL_BlitSurface(w, NULL, screen, &r);
				}
			}	
		}		
		while(SDL_PollEvent(&e)){
			switch(e.type){
				case SDL_QUIT:
					running = 0;
					break;
				case SDL_KEYDOWN:
					if(e.key.keysym.sym == SDLK_ESCAPE)
						running = 0;
					break;	
				case SDL_MOUSEBUTTONDOWN: 
					if ( e.button.button == SDL_BUTTON_LEFT){
						// attack command 
					}else
					{
						// move 
					}
					break;

			}
		}
		SDL_Flip(screen);

		if(1000/30 > SDL_GetTicks()-start)
			SDL_Delay(1000/30 - (SDL_GetTicks()-start));
	}

}

int main(int argc, char * argv[])
{

	if ( argc < 3 ) 
	{
		cout << "IP address and mode ( human / AIbot mode  ) and heroid  and teamid exptected \n";
		cout << "Usage : ./client serverIP mode \n";
		exit(0);
	}

	char * address = argv[1];

	struct sockaddr_in server; 

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(address);
	server.sin_port = htons(8181);


	int mode = atoi(argv[2]);

	if ( mode == HUMAN){
		Client c(address, 8181,  HUMAN);
		c.start();


	}else if ( mode == AI )
	{

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

