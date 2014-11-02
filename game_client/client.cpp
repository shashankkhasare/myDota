#include "client.h"
#include "../common/communication_structures.h"
#include "../common/defines.h"
#include "../Widget.h"

using namespace std; 
int getSocket(char *ip, int  port);

void send_hid_and_settle_team(int sock)
{

	SDL_Init(SDL_INIT_EVERYTHING);
	intdata d; 

	SDL_Surface *screen;
	screen = SDL_SetVideoMode(750 , 750 ,32,SDL_SWSURFACE);




	Widget menu("images/menu.bmp", NULL);
	menu.setLocation(0, 0);
	menu.setDim(750 , 750 );




	Widget h1("images/h1.bmp", NULL, 10, 10, 200, 200);
	Widget h2("images/h2.bmp", NULL, 250, 10, 200, 200);
	Widget h3("images/h3.bmp", NULL, 10, 250, 200, 200);
	Widget h4("images/h4.bmp", NULL, 250, 250, 200, 200);



	Widget teama("images/teama.bmp", NULL, 50, 50, 400, 87);
	Widget teamb("images/teamb.bmp", NULL, 50, 150, 400, 87);

	Widget teama_full("images/full.bmp", NULL, 50, 50, 400, 87);
	Widget teamb_full("images/full.bmp", NULL, 50, 150, 400, 87);
	Widget allteamsfull("images/allteamsfull.bmp", NULL, 50, 150, 441, 87);


	SDL_BlitSurface(menu.getSurface(), NULL, screen, menu.getRectAddr());

	SDL_BlitSurface(h1.getSurface(), NULL, screen, h1.getRectAddr());
	SDL_BlitSurface(h2.getSurface(), NULL, screen, h2.getRectAddr());
	SDL_BlitSurface(h3.getSurface(), NULL, screen, h3.getRectAddr());
	SDL_BlitSurface(h4.getSurface(), NULL, screen, h4.getRectAddr());



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

				case SDL_MOUSEBUTTONDOWN: 
					if ( areallteamsfull)
					{
						running = 0 ; 
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
								team_screen =1	;
							}else if ( h2.isInRange(e.button.x, e.button.y )){
								d.data = 2; 
								send(sock, &d, sizeof(d), 0);
								recv(sock, &d, sizeof(d), 0);
								if ( debug ) if ( d.data == OK ) cout << "debug :: Ok received for hid \n";
								if ( d.data == REJECTED)
									areallteamsfull = 1 ; 
								team_screen =1	;
							}else if ( h2.isInRange(e.button.x, e.button.y )){
								d.data = 3; 
								send(sock, &d, sizeof(d), 0);
								recv(sock, &d, sizeof(d), 0);
								if ( debug ) if ( d.data == OK ) cout << "debug :: Ok received for hid \n";
								if ( d.data == REJECTED)
									areallteamsfull = 1 ; 
								team_screen =1	;
							}else if ( h4.isInRange(e.button.x, e.button.y )){
								d.data = 4; 
								send(sock, &d, sizeof(d), 0);
								recv(sock, &d, sizeof(d), 0);
								if ( debug ) if ( d.data == OK ) cout << "debug :: Ok received for hid \n";
								if ( d.data == REJECTED)
									areallteamsfull = 1 ; 
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
				case SDL_MOUSEMOTION:
					break;

			}
		}
		if ( team_screen)
		{
			SDL_BlitSurface(menu.getSurface(), NULL, screen, menu.getRectAddr());
			if (isteamafull)
				SDL_BlitSurface(teama_full.getSurface(), NULL, screen, teama_full.getRectAddr());
			else
				SDL_BlitSurface(teama.getSurface(), NULL, screen, teama.getRectAddr());
			if ( isteambfull)
				SDL_BlitSurface(teamb_full.getSurface(), NULL, screen, teamb_full.getRectAddr());
			else
				SDL_BlitSurface(teamb.getSurface(), NULL, screen, teamb.getRectAddr());

		}
		if ( areallteamsfull){

			SDL_BlitSurface(menu.getSurface(), NULL, screen, menu.getRectAddr());
			SDL_BlitSurface(allteamsfull.getSurface(), NULL, screen, allteamsfull.getRectAddr());
		}
		SDL_Flip(screen);

		if(1000/30 > SDL_GetTicks()-start)
			SDL_Delay(1000/30 - (SDL_GetTicks()-start));
	}
	SDL_Quit();
}


Client::Client(char * address, int port ,  int mode){
	// setting vars 
	Client::mode = mode; 

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
}
void Client::start(){


	//pthread_t tid ; 
	//pthread_create(&tid, NULL, renderer , NULL);
	char map[75][76] = {
		"wwwwwwwwwwwwW....J...JJ....J.J........J....8........2....3.......BBBBBBBBBB",
		"wwwwwwwwwwJwwWW.J.......................J........JJ..............BBBBBBBBBB",
		"wwwwwwwwwwwwwWW......J....J................................J.....BBBBBBBBBB",
		"wwwwwwwwwwwwJJwWW...............9.J...7...J......4................BBBBBBBBB",
		"wwwwwwwwwwJwJwwWWJ...................J...................J........BBBBBBBBB",
		"wwwwJww6wwwwwwwwwWW..JJ....J.........................J.........2.5.BBBBBBBB",
		"wwwwwwwwwwwwwwwwJWW..............J..............7.4...........J....BBBBBBBB",
		"wwwwwwwwwwwwwwwwwwwW....................................9.......J...BBBBBBB",
		"wwJwwwwwwwwwwwwwwwww............J.................................J.6.BBBBB",
		"wwwwwwwwwwwwwwwwwwwwwW.J..........................JJ....................BBB",
		"wwwwwwwwwwwwwwwwwwwwwwW.7....J.............J.J.........8.J7......J.....J.J.",
		"wwwwwwww6wwwwwJwJwwwwwwW5.............J....J...............................",
		"WwwwwwwwwwwwwwwwwwwwwwwwW........J.......................J..........8.J....",
		".WwwwwwwwwwwwwwJ7wwwwwwwwW............J.........J..................J.......",
		"..WwwJwwwwwwwwwwwwwwwwwwwwW.......J........J..J...J..........J...4.........",
		"..JWwJwwwwJwww5wwJwwwwwwwwwW....J.........5.......J..J....J................",
		"....WwwwwwJwwwwwwwwwJwww4ww4W............2..........J5......J....J.J....J..",
		".....WwwwwwwwwwwwwwwwwwwwwwwwW...........J..5..........................JJ..",
		"......WwwwJwwwwwwwwwwJJwwwwJww...J.........................................",
		".......WwwwwwwwwwwwwwwwwwwJwwJwWJ..5.J......6..............................",
		".........wwwwwwwwwwwwwwwwwwwwwwwW....J................J....4...............",
		".........WwwwwwwwwwwwwwwwaaawwwwwW..........JJ.........................9...",
		"J.JJJ.....WwwwwwwwwwwwwwaaaaawwwwwW.....................J..................",
		"...6....5..WwJwwwwwwwwwaaaaaaawwwwwW.............J...............J...9JJ...",
		".J.......J8.WwwwwwwwwwaaaaaaaaawwwwwW....6..7.................J....J.......",
		".4...........WwwwwwwwwaaaaaaaaawwwwJwW........J...J.............5........J.",
		"......J.......WwwwwwJ4aaaaaaaaawwwwwwwW...8...J........J............J......",
		"...............Wwwwww8JaaaaaaawwwwwwwwwW.........J.........J....3...J......",
		".J5.............Wwwwwwwwaaaaawwwwwwwwwww...................................",
		".......J9.45..2..WwwwwwwwaaawwwwwJwwwwwwwW............J5...........J.......",
		"....6..............wwwwwww2wwwwwwwwwJwwwwwW..................7.............",
		".J.................WwwwwwwwwwwwwwwwwwwwwwwwWJ.......J......................",
		"....................WwwwwwwwwwwwwwwwwwwwwwwwW..............................",
		"........J.......5....Wwwwwwwwwww7wwwwwwwwwwwwW....J.............3JJ...6....",
		"J.............J.......WwwwwwwwwwwwwJJJwwwwwwwwW............................",
		"..1...................JWwwwwwJwwwwwwJwwwwwwwwwwW..J.....J.............1....",
		"...J..........J.........Wwwwwwwww1wwwwwwwwwwwwwwW.........J................",
		"......5..J.......J.......WwwwwwwwwwwwwwwwwwwwwwwwW..........8..J...........",
		"....J.....J...............Www5w3wwwwwwwwwwwwwwwww7.....7.9.................",
		"3....7...J............J....WwwwwwwwwwwwwwwwwwwwwwwwW.......J...............",
		"J..............J..J.J.J..2...wJwwwwww7wwwwwwwwwwwwwwW.........J............",
		".4.J7....3...................WwwwwwJwwwwwwwww4wwwwwwwW...5....J......3.....",
		"................J.............WwwwwwwwwwwwwwwJwwwwwwwwW.................J..",
		"..J..............JJ............WJwwJJwwwwwww4wwwwwwJwwwW.............J.....",
		"...........J....................WwwJwwwwwwwwwwwwwwwwww1wW.............8....",
		"......5.J.J.............J4..J....WwwwwwwwwwwwwwwwbbbwwwwwW.................",
		"......J........J.....J.........7..WwwwwwwwwwwwwwbbbbbwwwwwW................",
		"J..........................J.......WwwwwwwwwwwwbbbbbbbwwwwwW...............",
		"..................J...J........J....Wwwwwwwwwwbbbbbbbbbwwwww............J..",
		".............J.......J...............W4wwwwwwwbbbbbbbbbwwwwwwW.............",
		"........J...............J.......J..J...wJwwwwwbbbbbbbbbwwwwwwwWJ...........",
		"....................3..................WwwJwwwwbbbbbbbwwwwwww6wW...........",
		".......................6..J...........J.Wwww9wwwbbbbbwwwwwwwwwwwW..........",
		"......8.................J................WwwwwwwwbbbwwJwwwJwwwww7W.........",
		"..........................J...............WwwwJwwwwwwwwwwwwJwwwwwwW.J......",
		"........J................................J.WwwwwwwwwwwwwwwwJJww4wwwW.......",
		"...J.............................6..........WwwwwwwwwwwwwwJwwwwwwwwwW......",
		"...............J.......8....................JWwwwwwwwwwwwwJwwwJwwwwwwW..J..",
		"......................J......J................WwwwwwwJwwwwwwwwwwwwwwww.....",
		".J.............JJ..J................J..J.......WwwwwwJwwwJwwwwwwwwwwwwwW...",
		"J........J......................J................wwwww8wwwwwwwwwwwwwwwwwW..",
		"....J.............J...........J...........J....J.Www7wwwwwwwwwwwwwwwwwwwwW.",
		".J......J........J....3..J...............J...7....WwwwwwwwwwwwwwwwwwwwwwwwW",
		"................................J............4.....WwwwwwwwwwwJwwwwwwwJwwww",
		"........J..............J...........................JWwwwJwJwwwwwwwwwJwwwwww",
		"AAA.............J..7..J..............................Wwwwwwwwwwwwwwwwwwwwww",
		"AAAAA....J.........................J.8......7.........Wwwwwwwwwwwwwwwwwwwww",
		"AAAAAAA.........J...................J..................Wwwwwwwwwwwwwwww8www",
		"AAAAAAAA.9......J..................................J....Wwwwwwwwwwwwwwwwwww",
		"AAAAAAAA.......5....8.............J......................Wwwwwwwwwwwwwwwwww",
		"AAAAAAAAA...............J........J..............J..........wwwwwwwwwwww3www",
		"AAAAAAAAA.J.......J......7.4..........J6.J.9........J......WJwJwwwwwwwJwwww",
		"AAAAAAAAAA....................J.....................J..J....WwJwwwwJwwwwwww",
		"AAAAAAAAAA............J...4......9.......J....J..............WwwwwwJwwwwwww",
		"AAAAAAAAAA....................................................Wwwwwwwwwwwww"};
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface *screen, *fw, *tw, *w, *t, *it, *frame;
	screen = SDL_SetVideoMode(750,750,32,SDL_SWSURFACE);

	fw = SDL_DisplayFormat(SDL_LoadBMP("renderer/wfree750.bmp"));  //Required for transparnecy
	tw = SDL_DisplayFormat(SDL_LoadBMP("renderer/wtree.bmp"));
	w = SDL_DisplayFormat(SDL_LoadBMP("renderer/btree15_c.bmp"));
	t = SDL_DisplayFormat(SDL_LoadBMP("renderer/Temple1.bmp"));
	it = SDL_DisplayFormat(SDL_LoadBMP("renderer/items.bmp"));
	//frame= SDL_DisplayFormat(SDL_LoadBMP("renderer/frame.bmp"));

	if ( debug )  cout << "Done till here " << endl; 
	SDL_SetColorKey(tw, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	
	SDL_SetColorKey(w, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	
	SDL_SetColorKey(t, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	
	//SDL_SetColorKey(frame, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 0xff, 0xff, 0xff));	

	//Background Grass	
	int i, j;
	SDL_Rect r;
	r.w = 750;
	r.h = 750;
	r.x = 12;
	r.y = 12;
	SDL_BlitSurface(fw, NULL, screen, &r);
	// frame
	//	r.w = 774;
	//	r.h = 768;
	//	r.x = 0;
	//	r.y = 0;
	//	SDL_BlitSurface(frame, NULL, screen, &r);

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
	for(i=0;i<75;i++)
		for(j=0;j<75;j++)
			if(map[i][j] >= '1' && map[i][j] <= '9'){
				r.x = 10*j;
				r.y = 10*i;
				SDL_BlitSurface(it, &item[map[i][j]-'1'], screen, &r);
			}

	//Jungle
	r.w = 15;
	r.h = 15;
	for(i=0; i<75; i++){
		for(j = 0; j<75; j++){
			if(map[i][j] == 'J'){
				r.x = 10*j;
				r.y = 10*i;
				SDL_BlitSurface(tw, NULL, screen, &r);
			}
			else if	(map[i][j] == 'W'){
				r.x = 10*j;
				r.y = 10*i;
				SDL_BlitSurface(w, NULL, screen, &r);
			}
		}	
	}		

	int running = 1;
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

