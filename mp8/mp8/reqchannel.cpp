/* 
File: requestchannel.C

Author: R. Bettati
Department of Computer Science
Texas A&M University
Date  : 2012/07/11

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <cassert>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
//my includes
#include "reqchannel.h"
#include <netdb.h>
#include <sstream>
using namespace std;

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */
/*--------------------------------------------------------------------------*/

/*struct addrinfo {
    int              ai_flags;     // AI_PASSIVE, AI_CANONNAME, etc.
    int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC
    int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM
    int              ai_protocol;  // use 0 for "any"
    size_t           ai_addrlen;   // size of ai_addr in bytes
    struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
    char            *ai_canonname; // full canonical hostname

    struct addrinfo *ai_next;      // linked list, next node
};*/



//Create a NetworkRequestChannel based on the given socket
 NetworkRequestChannel::NetworkRequestChannel(int _socket){
	this->sock = _socket;
	//creates a new socket thats stored in the network request channel. Constructor automatically creates new NRC's.  
}

//client
NetworkRequestChannel::NetworkRequestChannel(const string _server_host_name, const unsigned short _port_no) 
{ 
	struct addrinfo hints, *res;
	char portStr[6];
	//int sockfd;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	//hints.ai_flags = AI_PASSIVE; // use my IP
	int status;
	sprintf(portStr, "%u", _port_no);
	//std::cout << "serverhost: " << _server_host_name << std::endl;
	if((status = getaddrinfo(_server_host_name.c_str() /*NULL*/, portStr, &hints, &res)) != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		exit(0);
	}
	sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol); //getaddrinfo(3) man page
	if(sock < 0){
		perror("socket error upon creation.");
		exit(0);
	}
	if(connect(sock, res->ai_addr, res->ai_addrlen) < 0){
		perror("connect error\n");
		exit(0);
	}
	std::cout << "Successfully connected to the server " << _server_host_name << endl;
}

//Constructor used in Server Side to Create Channel with Specific Port Number and back log.
NetworkRequestChannel::NetworkRequestChannel(const unsigned short _port_no, void * (*connection_handler) (void*), int back_log)
{  
	struct addrinfo hints, *res;
	int new_fd;
	char port[6];
	struct sockaddr_storage their_addr; //connectors address for the server to accept
	socklen_t sin_size; //32 bit unsigned address
	int rv;

/*	stringstream ss;
	ss << _port_no;
	string port = ss.str();	*/
	sprintf(port, "%u", _port_no);

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; //use current ip of user
	
	if((rv = getaddrinfo(NULL, port, &hints, &res)) != 0){
		perror("getaddrinfo error");
		exit(0);
	}
	if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
        perror("server: socket");
		exit(0);
	}
	if((bind(sock, res->ai_addr, res->ai_addrlen)) == -1){
		perror("Server:socket err");
		exit(0);
	}
	freeaddrinfo(res);

	if(listen(sock, back_log) == -1){
		perror("error listening");
		exit(1);
	}
	printf("server: waiting for connections...\n");
	
	while(1){
		sin_size = sizeof(their_addr);
		//new_fd = accept(sock, (struct sockaddr *)&their_addr, &sin_size);
		//if(new_fd == -1){
		//	perror("accept error");
		//	continue;
		//}
		int *temp_socket = new int;
		*temp_socket = accept(sock, (struct sockaddr *)&their_addr, &sin_size);
		printf("Successfully connected.");
		pthread_t new_thread;
		pthread_create(&new_thread, 0, connection_handler, (void *)temp_socket);
	
	}
	cout << "connection complete\n"; 
}

//close socket
NetworkRequestChannel::~NetworkRequestChannel() 
{     
	close(sock);
}

/*--------------------------------------------------------------------------*/
/* READ/WRITE FROM/TO REQUEST CHANNELS  */
/*--------------------------------------------------------------------------*/

const int MAX_MESSAGE = 255;

string NetworkRequestChannel::send_request(string _request) 
{
	cwrite(_request);
	string s = cread();
	return s;
	
}

string NetworkRequestChannel::cread() 
{
	char buf[MAX_MESSAGE];
	
	if(read(sock, buf, MAX_MESSAGE) < 0){
		perror(string("Error reading from socket.").c_str());
	}
	string s = buf;
	return s;
}

int NetworkRequestChannel::cwrite(string _msg) 
{
	if(_msg.length() > MAX_MESSAGE){
		perror("Error in write. It's too long.");
		return -1;
	}
	else{
		const char *s = _msg.c_str();
		if(write(sock, s, strlen(s)+1) < 0){
			perror(string("Error writing to socket.").c_str());
		}
		int counter = sizeof(&s);
		return counter;
	}
}
