#include"game_server.h"
#include<iostream>
#include "../common/defines.h"


Server::Server(char * map_file,  int port, int max_players):m(map_file){
	Server::port = port; 
	Server::max_players= max_players;
}


struct sockaddr_in getSelfAddress(){
	struct ifaddrs *ifap, *ifa;
	struct sockaddr_in *sa;
	struct sockaddr_in ret;
	cout << "Inside get self address \n";
	getifaddrs (&ifap);
	cout << "getifaddrs done \n";

	for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr->sa_family==AF_INET) {
			sa = (struct sockaddr_in *) ifa->ifa_addr;
			ret = *sa;
		}
	}
	cout << "returning from getself address \n";
	freeifaddrs(ifap);
	return ret;
}

void Server::startServer(struct sockaddr_in addr){
	struct sockaddr_in  client ; 
	int server_sock, client_sock ;
	socklen_t addr_size = sizeof(client);
	addr.sin_port=htons(Server::port);

	cout << "Start server called " << endl; 
	if ( ( server_sock = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
		cout<< "socket() : Failed to create socket. ";
		exit(0);
	}
	int iSetOption = 1;
	setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,
	        sizeof(iSetOption));
	cout << "Trying to open socket on " << inet_ntoa(addr.sin_addr )  << ntohs(addr.sin_port) << endl;
	if( bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		cout << "bind() : Failed to bind. ";
		exit(0);
	}
	cout << "Bind successful. Now listening to connections \n";
	if( listen(server_sock, 100) < 0 ) {
		cout << "Failed to listen on socket. ";
		exit(0);
	}
	while(1){
		client_sock = accept(server_sock, (struct sockaddr *)&client, &addr_size);
		serve_connection(client, client_sock);
	}
}


void Server::serve_connection(sockaddr_in client , int sock_fd) 
{
	header head, reply;
	intdata d ; 
	recv(sock_fd, &head, sizeof(head), 0); 
	reply.type = OK;
	send(sock_fd, &reply, sizeof(reply), 0);
	if ( head.type == OPEN_INPUT_CHANNEL ) {
		// allow only 4 players to connect
		if ( player_count >= 4){
			reply.type = REJECTED;
			if ( debug ) cout << "debug :: All teams full. Rejected \n" ;
			send(sock_fd, &reply, sizeof(reply), 0);
			return;
		}
			if ( debug ) cout << " debug :: Request for opening input channel \n";
		recv(sock_fd, &d, sizeof(d), 0); 
		head.type = OK;
		send(sock_fd, &head, sizeof(head), 0);
		if ( debug ) cout << "HID : " << d.data; 
		Hero h(d.data); // create hero 
		if ( debug ) cout << "Hero created \n";
		if ( debug ) cout << "player_count " << player_count << "socket fd : " << sock_fd << endl; 
		// get the team choice
		while(1){
			recv(sock_fd, &d, sizeof(d), 0);
			int teamid = d.data; 
			if ( debug ) cout << "Received team id " << teamid << endl ; 
			if (teamid == TEAM_A){
				if( team_a.size() < 2){
					team_a.push_back(player_count);
					reply.type = ACCEPTED; 
					send(sock_fd, &reply, sizeof(reply), 0);
					break;
				}else
				{
					reply.type = REJECTED;
					if ( debug ) cout << "debug :: team a full. Rejected \n" ;
					send(sock_fd, &reply, sizeof(reply), 0);
				}
			}else if ( teamid == TEAM_B ) 
			{
				if( team_b.size() < 2){
					team_b.push_back(player_count);
					reply.type = ACCEPTED; 
					send(sock_fd, &reply, sizeof(reply), 0);
					break;
				}else
				{
					reply.type = REJECTED;
					if ( debug ) cout << "debug :: team a full. Rejected \n" ;
					send(sock_fd, &h, sizeof(h), 0);
				}
			}else
			{
				if ( debug ) cout << " debug :: invalid teamid received " << endl; 
			}
		}


		Player p(player_count, sock_fd, client,  h);
		players[player_count] = p;
		player_count++;
	}else if ( head.type == OPEN_BCAST_CHANNEL){
		recv(sock_fd, &d, sizeof(d), 0); 
		if ( debug ) cout << "debug :: Request for bcast channel received from pid " << d.data << endl; 
		players[d.data].setBcastChannel(sock_fd);
		if ( debug ) cout << "debug :: Bcast channel updated for  " << d.data << endl; 

	} else
	{
		cout << "Unknown packet received " << head.type << endl;
	}

}
using namespace std; 
int main(int argc, char *argv[]){

	struct sockaddr_in addr  = getSelfAddress();
	cout << "Starting the game server\n"; 
	Server s("maps/m1" ,  8181, 4);

	s.startServer(addr);

}
