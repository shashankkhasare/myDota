#include<iostream>
#include "../common/defines.h"
#include "../maps/map.h"
#include "../player/player.h"
#include "../common/communication_structures.h"
#include "../common/bcast.h"
#include "../common/ipc.h"
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
#include<poll.h>
#include<fcntl.h>

#define inputdebug 1
#define bcastdebug 0
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

int gamemode = STATE_BEFORE_START;



void startServer(struct sockaddr_in);
void serve_connection(struct sockaddr_in client , int sock_fd) ;
void * bcast_sender(void * ) ; 


int fd_set_blocking(int fd, int blocking) {
	/* Save the current flags */
	int flags = fcntl(fd, F_GETFL, 0);
	if (flags == -1)
		return 0;

	if (blocking)
		flags &= ~O_NONBLOCK;
	else
		flags |= O_NONBLOCK;
	return fcntl(fd, F_SETFL, flags) != -1;
}
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
void read_and_display_command(int sock){

	cmd_t cmd; 
	header reply; 
	if ( -1 == recv(sock, &cmd, sizeof(cmd), 0)){
		return;
	}
	reply.type = OK;
	send(sock, &reply, sizeof(reply), 0);

	switch(cmd.command){
		case CMD_GOTO_X_Y:
			if ( inputdebug ) cout << "inputdebug :: command CMD_GOTO_X_Y received " << cmd.x << " "  << cmd.y << endl;
			break;
		case CMD_ATTACK_PID:
			if ( inputdebug ){
				if ( cmd.attack_mode == MAGIC)
					cout << "inputdebug :: command CMD_ATTACK_PID magic mode received " <<  cmd.pid << endl;
				else
					cout << "inputdebug :: command CMD_ATTACK_PID melee mode received " <<  cmd.pid << endl;
			}

			break;
		case CMD_USEITEM_X:
			if ( inputdebug ) cout << "inputdebug :: command CMD_USEITEM_X received " <<  cmd.x << endl;
			break;
		case CMD_GRABITEM_X_Y:
			if ( inputdebug ) cout << "inputdebug :: command CMD_GRABITEM_X_Y received " <<  cmd.x << " " << cmd.y  << endl;
			break;
		case CMD_QUIT:
			if ( inputdebug ) cout << "inputdebug :: command CMD_QUIT  received\n";
			break;
		default:
			cout << "Error:: Unknown command received from the client \n";
	}
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

		// make the ipc socket nonblocking 
		fd_set_blocking(sock_fd, 0 ) ; 
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
	int count  =0 ; 
	while(1){

		//usleep(1000000);
		sleep(1);
		count ++ ; 
		if ( bcastdebug ) cout << "Broadcast iteration************************************* :: " << count << endl; 
		for(int i =0 ; i < 4 ; i ++ ) {
			// send terrain 
			reply.type = BCAST_TERRAIN;
			send(players[i].bcast_sock, &reply, sizeof(reply), NULL);
			recv(players[i].bcast_sock, &h, sizeof(h), NULL);
			if( h.type == OK){
				if ( bcastdebug ) cout << "bcastdebug ::reply for BCAST TERRAIN received : ok \n"; 
			}
			else 
				cout << "Unknown reply for BCAST terrain from client of player : " << i  << endl ; 
			send(players[i].bcast_sock, m.terrain, 75 * 75, NULL);
			recv(players[i].bcast_sock, &h, sizeof(h), NULL);
			if( h.type == OK){
				if ( bcastdebug ) cout << "bcastdebug ::terrain sent received : ok \n"; 
			}
			else 
				cout << "Unknown reply for terrain data  from client of player : " << i  << endl ; 
			// send health of  all players 
			if ( bcastdebug ) cout << "bcastdebug :: sending data of all players \n";
			for( int j =0 ; j < 4 ; j++){

				if ( bcastdebug ) cout << "bcastdebug :: sending data of player " << j << " *********\n" ;
				player_data_t pdata; // id health nitro hid team  
				pdata.id = j;
				pdata.health = players[j].hero.health;  
				pdata.nitro = players[j].hero.nitro.value;
				pdata.hid = players[j].hero.hid; 
				if ( bcastdebug ) cout << "pdata.id : " << pdata.id; 
				if ( bcastdebug ) cout << " pdata.health: " << pdata.health; 
				if ( bcastdebug ) cout << " pdata.nitro : " << pdata.nitro; 
				if ( bcastdebug ) cout << " pdata.hid : " << pdata.hid; 
				if ( bcastdebug ) cout << " pdata.team : " << pdata.team << endl; 
				if ( team_a[0] == j || team_a[1] == j){
					pdata.team = TEAM_A;
				}else
				{
					pdata.team = TEAM_B;
				}

				reply.type = BCAST_PLAYER_DATA;
				send(players[i].bcast_sock, &reply, sizeof(reply), NULL);
				recv(players[i].bcast_sock, &h, sizeof(h), NULL);
				if( h.type == OK){
					if ( bcastdebug ) cout << "bcastdebug ::reply for BCAST player data received\n"; 
				}
				else 
					cout << "Unknown reply for BCAST health from client of player : " << i  << endl ; 
				send(players[i].bcast_sock, &pdata, sizeof(pdata) , NULL);
				recv(players[i].bcast_sock, &h, sizeof(h), NULL);
				if( h.type == OK){
					if ( bcastdebug ) cout << "bcastdebug :: player data sent received : ok \n"; 
				}
				else 
					cout << "Unknown reply for player data from client of player : " << i  << endl ; 
			}


			// send data for temple a

			if ( bcastdebug ) cout << "bcastdebug :: sending temple a data \n"; 
			player_data_t pdata; 
			pdata.id = TEMPLE_A_ID;
			pdata.health = m.temple_a.health;
			pdata.nitro = 0;
			pdata.hid = -1;
			pdata.team = TEAM_A;

			reply.type = BCAST_PLAYER_DATA;
			send(players[i].bcast_sock, &reply, sizeof(reply), NULL);
			recv(players[i].bcast_sock, &h, sizeof(h), NULL);
			if( h.type == OK){
				if ( bcastdebug ) cout << "bcastdebug ::reply for BCAST temple a data received : ok \n"; 
			}
			else 
				cout << "Unknown reply for BCAST temple a data from client of player : " << i  << endl ; 
			send(players[i].bcast_sock, &pdata, sizeof(pdata) , NULL);
			recv(players[i].bcast_sock, &h, sizeof(h), NULL);
			if( h.type == OK){
				if ( bcastdebug ) cout << "bcastdebug :: temple a data sent received : ok \n"; 
			}
			else 
				cout << "Unknown reply for temple data from client of player : " << i  << endl ; 


			// send data for temple b

			if ( bcastdebug ) cout << "bcastdebug :: sending temple b data \n"; 
			pdata.id = TEMPLE_B_ID;
			pdata.health = m.temple_b.health;
			pdata.nitro = 0;
			pdata.hid = -1;
			pdata.team = TEAM_B;

			reply.type = BCAST_PLAYER_DATA;
			send(players[i].bcast_sock, &reply, sizeof(reply), NULL);
			recv(players[i].bcast_sock, &h, sizeof(h), NULL);
			if( h.type == OK){
				if ( bcastdebug ) cout << "bcastdebug ::reply for BCAST temple b data received : ok \n"; 
			}
			else 
				cout << "Unknown reply for BCAST temple b data from client of player : " << i  << endl ; 
			send(players[i].bcast_sock, &pdata, sizeof(pdata) , NULL);
			recv(players[i].bcast_sock, &h, sizeof(h), NULL);
			if( h.type == OK){
				if ( bcastdebug ) cout << "bcastdebug :: temple b data sent received : ok \n"; 
			}
			else 
				cout << "Unknown reply for temple data from client of player : " << i  << endl ; 

		}
	}


}
int main(int argc, char *argv[]){

	struct sockaddr_in addr  = getSelfAddress();
	port =8181;
	max_players=4;

	if ( debug ) cout << "Starting the game server\n"; 
	cmd_t cmd; 
	header reply;

	startServer(addr);
	// start bcast sender thread 
	pthread_t tid ; 
	pthread_create ( &tid, NULL, bcast_sender, NULL);
	// start the input receiver 
	while(1){
		// check round robin 
		for ( int i = 0 ; i < 4 ; i ++ ) {
			int ipc = players[i].input_fd;
			read_and_display_command(ipc);
		}
		// all game code here 

		usleep(30000);
	}
}
