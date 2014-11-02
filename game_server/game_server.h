#ifndef GAME_SERV
#define GAME_SERV
#include "../maps/map.h"
#include "../player/player.h"
#include "../common/communication_structures.h"
#include "../heros/hero.h"
#include<netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include<unistd.h>
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<string>
#include<stdlib.h>

#define debug 1

using namespace std; 
class Server {

	Map m;
	int port; 
	int player_count =0;
	int max_players;
	vector<int> team_a; 
	vector<int> team_b; 

	Player players[4];

	public : 
	Server(char * map_file, int port, int max_players);
	/* Start game, 
	 * will check if the clients have joined in and ready 
	 * Will start the actual play of the game 
	 * */
	void startServer(struct sockaddr_in);
	void serve_connection(struct sockaddr_in client , int sock_fd) ;
};





#endif 
