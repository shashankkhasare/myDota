/**
  * Returns the current position of hero with h_id.
**/  
point_t get_curr_pos(int h_id);

/**
  * Listens for commands;
**/  
void recv_cmd();




class Server {


	// map 
	// player array
	// 2 threads, 1 constantly broadcasting the rendering information
	// 	      2 constantly reading the input buffer, which would be filled by the client only. Redirecting each command received 
	// 	        to the appropriate player connected to the server
	// Each player is an independent thread running on the server machine, which reads its input buffer which is being filled by the server
	// and accordingly updating the game data structures, for eg the map, the item status and so on. 
	



}
