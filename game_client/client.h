#ifndef client_h 
#define client_h 

#include <SDL/SDL.h>
#include<iostream>
#include "../common/mode.h"
#include "../common/communication_structures.h"
#include "../common/defines.h"
#include<netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>

#define debug 1

class Client {


	// Client is a simple class which will look after the following tasks:
	// 	1. Listening the video channel and render it on the screen of the client. The information will be just the location 
	// 		of differnt objects , players items and so on. 
	// 	2. Define an interface to the command generation logic which could a human player or a simple bot generating the input commands.
	// 		Sending these commands to the input buffer of the server ( through another channel differnt from the above video channel). 
	// 		So there are 2 socket connections to the server for each client class. 
	



	/*
	 * Data structures 
	 * */


public:


	pthread_t tid; 


	Client(char * server_address, int port ,  int mode);
	void start();

};



#endif
