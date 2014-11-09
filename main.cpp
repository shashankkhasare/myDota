#include <SDL/SDL.h>
#include<iostream>
#include "Widget.h"

#include<netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include<string.h>
#include<unistd.h>
#include<fstream>

using namespace std;

int running;
bool multiplayerChosen = false;
void pollAndServeEvents();

struct sockaddr_in getSelfAddress(){
	struct ifaddrs *ifap, *ifa;
	struct sockaddr_in *sa;
	struct sockaddr_in ret;
	getifaddrs (&ifap);

	for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr->sa_family==AF_INET) {
			sa = (struct sockaddr_in *) ifa->ifa_addr;
			ret = *sa;
		}
	}
	freeifaddrs(ifap);
	return ret;
}
void fork_local_server(){

	char command[] = "./gameserver 2>&1 > logs/gameserver.log & " ; 
	system(command);

}
void fork_local_client(int mode, int no){

	struct sockaddr_in addr;
	char command[50]  = "./client ";
	char *ip ;
	addr = getSelfAddress();
	ip = inet_ntoa(addr.sin_addr);
	sprintf(command, "./client %s %d 2>&1 > logs/client%d.log &" , ip, mode, no);
	cout << command << endl;
	system(command);
}


int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Surface *screen;
	screen = SDL_SetVideoMode(916, 663,32,SDL_SWSURFACE);

		
	

	Widget menu("images/menu.bmp", NULL);
	menu.setLocation(0, 0);
	menu.setDim(916, 663);

	// instantiate logo 
	Widget logo("images/gamelogo.bmp", "images/gamelogo_hover.bmp", 106 , 180, 749 , 84 ) ; 
	logo.deleteWhite(screen);

	//adding the menu background 
	//and logo 


	// instantiate single player 
	Widget singlePlayerButton("images/single_player.bmp", "images/single_player_hover.bmp", 350, 290 , 297, 51);
	singlePlayerButton.deleteWhite(screen);

	// instantiate multiplayer 
	Widget multiPlayerButton("images/multiplayer.bmp", "images/multiplayer_hover.bmp", 370, 350 , 261, 51);
	multiPlayerButton.deleteWhite(screen);

	// instantiate exit
	Widget exitButton("images/exit.bmp", "images/exit_hover.bmp", 450, 410 , 90, 51);
	exitButton.deleteWhite(screen);

	// instantiate joinremote
	Widget joinremote("images/joinremoteserver.bmp", "images/joinremoteserver_hover.bmp", 230, 300 , 519, 60);
	joinremote.deleteWhite(screen);

	// instantiate hostserver 
	Widget hostserver("images/hostserver.bmp", "images/hostserver_hover.bmp", 330, 220 , 328, 60);
	hostserver.deleteWhite(screen);

	// instantiate back  
	Widget back("images/back.bmp", "images/back_hover.bmp", 400, 380, 137, 60);
	back.deleteWhite(screen);
	running = 1;
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
					cout << e.button.x << " " << e.button.y << endl; 
					if ( e.button.button == SDL_BUTTON_LEFT){
						if ( !multiplayerChosen && exitButton.isInRange(e.button.x, e.button.y ))
							running = 0 ; 
						else if ( !multiplayerChosen && singlePlayerButton.isInRange(e.button.x, e.button.y )){
							// fork a server 
							fork_local_server();
							// fork a client in human mode
							// fork         (mode, id)
							fork_local_client(0 , 1); 
							// fork 3 clients in AI mode
							fork_local_client(1 , 2); 
							fork_local_client(1 , 3); 
							fork_local_client(1 , 4); 
						}
						else if ( !multiplayerChosen && multiPlayerButton.isInRange(e.button.x, e.button.y )){
							multiplayerChosen = true;
						}else if ( multiplayerChosen && hostserver.isInRange(e.button.x, e.button.y )){
							// fork a server 
							fork_local_server();
							// fork client in human mode
							sleep(2);
							fork_local_client(0, 1);
							running = 0 ; 
						}else if ( multiplayerChosen && joinremote.isInRange(e.button.x, e.button.y ) ) {
							system("gnome-terminal --hide-menubar -t \"Minion Dota :: Host IP address required\" -x $PWD/remote_client.sh");
							// check if the address is supplied 
							while(true){
								ifstream f("address"); 
								if(f.good()){
									break;
								}else
								{
									f.close();
									sleep(1);
								}
							}
							system("./client `cat address` 0 >logs/client.log &");
							system("rm address");
							running = 0 ; 
						}else if (multiplayerChosen && back.isInRange(e.button.x , e.button.y )){
							multiplayerChosen = false;
						}

					}else
					{
						cout << "Right button click on  : (" << e.button.x << " , " << e.button.y << ")" << endl; 
					}
					break;
				case SDL_MOUSEMOTION:
					if ( multiplayerChosen){
						back.handleHover(e);
						joinremote.handleHover(e);
						hostserver.handleHover(e);
					}else{

						singlePlayerButton.handleHover(e);
						multiPlayerButton.handleHover(e);
						logo.handleHover(e);
						exitButton.handleHover(e);
					}
					break;

			}
		}
		SDL_BlitSurface(menu.getSurface(), NULL, screen, menu.getRectAddr());
		if (multiplayerChosen){

			SDL_BlitSurface(joinremote.getSurface(), NULL, screen, joinremote.getRectAddr());
			SDL_BlitSurface(hostserver.getSurface(), NULL, screen, hostserver.getRectAddr());
			SDL_BlitSurface(back.getSurface(), NULL, screen, back.getRectAddr());
		}else
		{
			SDL_BlitSurface(logo.getSurface(), NULL, screen, logo.getRectAddr());
			SDL_BlitSurface(singlePlayerButton.getSurface(), NULL, screen, singlePlayerButton.getRectAddr());
			SDL_BlitSurface(multiPlayerButton.getSurface(), NULL, screen, multiPlayerButton.getRectAddr());
			SDL_BlitSurface(exitButton.getSurface(), NULL, screen, exitButton.getRectAddr());
		}
		SDL_Flip(screen);

		if(1000/30 > SDL_GetTicks()-start)
			SDL_Delay(1000/30 - (SDL_GetTicks()-start));
	}

	SDL_Quit();



}


