/*
point_t get_curr_pos(int h_id);


void recv_cmd();
*/

#include "../maps/map.h"


	// map 
	// player array
	// 2 threads, 1 constantly broadcasting the rendering information
	// 	      2 constantly reading the input buffer, which would be filled by the client only. Redirecting each command received 
	// 	        to the appropriate player connected to the server
	// Each player is an independent thread running on the server machine, which reads its input buffer which is being filled by the server
	// and accordingly updating the game data structures, for eg the map, the item status and so on. 
	

class Server {

	// members 
	Map map; 
	int port; 

	// constructor
	Server(char *map_file, int port);

	/* Start server, 
	 * this will bind a socket to the said port and start listening to the commands
	 *
	 * */
	bool StartServer();

	/* Start game, 
	 * will check if the clients have joined in and ready 
	 * Will start the actual play of the game 
	 * */
	bool StartServer(){


	}




}


Server::Server(char *map_file, int port){
	// Instantiate map
	Map m(map_file);
	Server::map = m;

	Server::port = port; 
}


bool Server:StartServer(){


}

