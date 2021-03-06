#ifndef PLAYER_H
#define PLAYER_H

#include<netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include "../heros/hero.h"
#include <iostream>
class Player{

	public:
	int pid; 
	sockaddr_in address;
	Hero hero; 

	int input_fd;
	int bcast_sock; 
	Player(int playerid, int ip_fd, sockaddr_in addr,  Hero h);
	void init(int playerid, int ip_fd, sockaddr_in addr,  Hero h);
	Player();
	void setBcastChannel(int sock);
};
#endif
