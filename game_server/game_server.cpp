#include"game_server.h"
#include<iostream>


Server::Server(char * map_file,  int port):m(map_file){
	Server::port = port; 

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
	header h ; 
	intdata d; 
	recv(sock_fd, &h, sizeof(h), 0); 
	if ( h.type == OPEN_INPUT_CHANNEL ) {
		// fetch hero ID
		cout << "Request for opening input channel \n";
		recv(sock_fd, &d, sizeof(d), 0); 
		Hero h(d.data);
		Player *p  = new Player(Server::player_count, sock_fd, client,  h);
		Server::player_count++;
		Server::players.push_back(p);
	}else
	{
		cout << "Unknown packet received \n";
	}

}
using namespace std; 
int main(int argc, char *argv[]){

	struct sockaddr_in addr  = getSelfAddress();
	cout << "Starting the game server\n"; 
	Server s("maps/m1" ,  8181);

	s.startServer(addr);

}
