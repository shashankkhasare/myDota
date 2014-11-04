#include<iostream>
#include "../common/defines.h"
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
#include<unistd.h>
#include<pthread.h>

#define debug 1
using namespace std; 

int port; 
int available_pid=0;
int max_players;
int connections_served =0 ;
Player players[4];
Map m("maps/m1");
vector<int> team_a; 
vector<int> team_b; 



void startServer(struct sockaddr_in);
void serve_connection(struct sockaddr_in client , int sock_fd) ;
void * bcast_sender(void * ) ; 

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

void startServer(struct sockaddr_in addr){
	struct sockaddr_in  client ; 
	int server_sock, client_sock ;
	socklen_t addr_size = sizeof(client);
	addr.sin_port=htons(port);

	if ( debug ) cout << "debug :: Start server called " << endl; 
	if ( ( server_sock = socket(AF_INET, SOCK_STREAM, 0) ) < 0 ){
		cout<< "socket() : Failed to create socket. ";
		exit(0);
	}
	int iSetOption = 1;
	setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&iSetOption,
			sizeof(iSetOption));
	if ( debug ) cout << "debug :: Trying to open socket on " << inet_ntoa(addr.sin_addr )  << ntohs(addr.sin_port) << endl;
	if( bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0 ){
		cout << "bind() : Failed to bind. ";
		exit(0);
	}
	if ( debug ) cout << "debug :: Bind successful. Now listening to connections \n";
	if( listen(server_sock, 100) < 0 ) {
		cout << "Failed to listen on socket. ";
		exit(0);
	}
	while(1){
		if ( connections_served >= 8 ) {
			if ( debug ) cout << "debug :: created all connections to the client successfully. Now returning \n";
			return;
		}

		client_sock = accept(server_sock, (struct sockaddr *)&client, &addr_size);
		serve_connection(client, client_sock);
	}
}


void serve_connection(sockaddr_in client , int sock_fd) 
{
	header head, reply;
	intdata d ; 
	recv(sock_fd, &head, sizeof(head), 0); 
	reply.type = OK;
	send(sock_fd, &reply, sizeof(reply), 0);
	if ( head.type == OPEN_INPUT_CHANNEL ) {
		// allow only 4 players to connect
		if ( available_pid >= 4){
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
		if ( debug ) cout << "available_pid " << available_pid << "socket fd : " << sock_fd << endl; 
		// get the team choice
		while(1){
			recv(sock_fd, &d, sizeof(d), 0);
			int teamid = d.data; 
			if ( debug ) cout << "Received team id " << teamid << endl ; 
			if (teamid == TEAM_A){
				if( team_a.size() < 2){
					team_a.push_back(available_pid);
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
					team_b.push_back(available_pid);
					reply.type = ACCEPTED; 
					send(sock_fd, &reply, sizeof(reply), 0);
					break;
				}else
				{
					reply.type = REJECTED;
					if ( debug ) cout << "debug :: team a full. Rejected \n" ;
					send(sock_fd, &reply, sizeof(reply), 0);
				}
			}else
			{
				if ( debug ) cout << " debug :: invalid teamid received " << endl; 
			}
		}


		Player p(available_pid, sock_fd, client,  h);
		players[available_pid] = p;
		connections_served += 1; 
		available_pid++;
	}else if ( head.type == OPEN_BCAST_CHANNEL){
		recv(sock_fd, &d, sizeof(d), 0); 
		if ( debug ) cout << "debug :: Request for bcast channel received from pid " << d.data << endl; 
		players[d.data].setBcastChannel(sock_fd);
		if ( debug ) cout << "debug :: Bcast channel updated for  " << d.data << endl; 
		connections_served += 1; 

	} else
	{
		cout << "Unknown packet received " << head.type << endl;
	}

}

void * bcast_sender(void *T){

	header h, reply ; 
	health_t heal; 
	int count  =0 ; 
	while(1){

		//usleep(1000000);
		sleep(1);
		count ++ ; 
		if ( debug ) cout << "Broadcast iteration************************************* :: " << count << endl; 
		for(int i =0 ; i < 4 ; i ++ ) {
			// send terrain 
			reply.type = BCAST_TERRAIN;
			send(players[i].bcast_sock, &reply, sizeof(reply), NULL);
			recv(players[i].bcast_sock, &h, sizeof(h), NULL);
			if( h.type == OK){
				if ( debug ) cout << "debug ::reply for BCAST TERRAIN received : ok \n"; 
			}
			else 
				cout << "Unknown reply for BCAST terrain from client of player : " << i  << endl ; 
			send(players[i].bcast_sock, m.terrain, 75 * 75, NULL);
			recv(players[i].bcast_sock, &h, sizeof(h), NULL);
			if( h.type == OK){
				if ( debug ) cout << "debug ::terrain sent received : ok \n"; 
			}
			else 
				cout << "Unknown reply for terrain data  from client of player : " << i  << endl ; 
			// send health of  all players 
			if ( debug ) cout << "debug :: sending health of all players \n";
			for( int j =0 ; j < 4 ; j++){

				if ( debug ) cout << "debug :: sending health of player " << j << " *********\n" ;
				heal.id = j;
				heal.value = players[j].hero.health;  
				reply.type = BCAST_HEALTH;
				send(players[i].bcast_sock, &reply, sizeof(reply), NULL);
				recv(players[i].bcast_sock, &h, sizeof(h), NULL);
				if( h.type == OK){
					if ( debug ) cout << "debug ::reply for BCAST Health received : ok \n"; 
				}
				else 
					cout << "Unknown reply for BCAST health from client of player : " << i  << endl ; 
				send(players[i].bcast_sock, &heal, sizeof(heal) , NULL);
				recv(players[i].bcast_sock, &h, sizeof(h), NULL);
				if( h.type == OK){
					if ( debug ) cout << "debug :: health sent received : ok \n"; 
				}
				else 
					cout << "Unknown reply for health_t from client of player : " << i  << endl ; 
			}


			// send health of temple a

			if ( debug ) cout << "debug :: sending health of temple a\n"; 

			heal.id = TEMPLE_A_ID;
			heal.value = m.temple_a.health; 
			reply.type = BCAST_HEALTH;
			send(players[i].bcast_sock, &reply, sizeof(reply), NULL);
			recv(players[i].bcast_sock, &h, sizeof(h), NULL);
			if( h.type == OK){
				if ( debug ) cout << "debug ::reply for BCAST temple a Health received : ok \n"; 
			}
			else 
				cout << "Unknown reply for BCAST temple a  health from client of player : " << i  << endl ; 
			send(players[i].bcast_sock, &heal, sizeof(heal) , NULL);
			recv(players[i].bcast_sock, &h, sizeof(h), NULL);
			if( h.type == OK){
				if ( debug ) cout << "debug :: temple a health sent received : ok \n"; 
			}
			else 
				cout << "Unknown reply for health_t from client of player : " << i  << endl ; 


			if ( debug ) cout << "debug :: sending health of temple b\n"; 

			// send health of temple b 
			heal.id = TEMPLE_B_ID;
			heal.value = m.temple_b.health; 
			reply.type = BCAST_HEALTH;
			send(players[i].bcast_sock, &reply, sizeof(reply), NULL);
			recv(players[i].bcast_sock, &h, sizeof(h), NULL);
			if( h.type == OK){
				if ( debug ) cout << "debug ::reply for BCAST temple b Health received : ok \n"; 
			}
			else 
				cout << "Unknown reply for BCAST temple b  health from client of player : " << i  << endl ; 
			send(players[i].bcast_sock, &heal, sizeof(heal) , NULL);
			recv(players[i].bcast_sock, &h, sizeof(h), NULL);
			if( h.type == OK){
				if ( debug ) cout << "debug :: temple b health sent received : ok \n"; 
			}
			else 
				cout << "Unknown reply for health_t from client of player : " << i  << endl ; 


		}
	}


}
int main(int argc, char *argv[]){

	struct sockaddr_in addr  = getSelfAddress();
	port =8181;
	max_players=4;

	cout << "Starting the game server\n"; 


	startServer(addr);
	// start bcast sender thread 
	pthread_t tid ; 
	pthread_create ( &tid, NULL, bcast_sender, NULL);
	// start the input receiver 
	while(1){
		if ( debug ) cout << "debug :: main waiting \n";
		//usleep(1000000);
		sleep(1);
	}
}
