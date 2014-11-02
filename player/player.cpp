#include "player.h"
#include "../common/communication_structures.h"
Player::Player(int playerid, int ip_fd, sockaddr_in addr,  Hero h):hero(h){
	pid = playerid; 
	input_fd = ip_fd;
	address = addr; 



	// now tell the client, its player id 
	intdata d; 
	d.data = playerid; 
	send(ip_fd, &d, sizeof(d), 0);
}

Player::Player(){}

void Player::init(int playerid, int ip_fd, sockaddr_in addr,  Hero h){
	pid = playerid; 
	input_fd = ip_fd;
	address = addr; 
	hero = h; 
}

void Player::setBcastChannel(int sock){
	bcast_fd = sock;
}

