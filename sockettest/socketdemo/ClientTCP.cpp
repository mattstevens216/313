#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int client (char * server_name, char* port)
{
	struct addrinfo hints, *res;
	int sockfd;

	// first, load up address structs with getaddrinfo():

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int status;
	//getaddrinfo("www.example.com", "3490", &hints, &res);
	if ((status = getaddrinfo(server_name, port, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return -1;
    }

	// make a socket:
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd < 0)
	{
		perror ("Error creating socket\n");	
		return -1;
	}

	// connect!
	if (connect(sockfd, res->ai_addr, res->ai_addrlen)<0)
	{
		perror ("connect error\n");
		return -1;
	}
	printf ("Successfully connected to the server %s\n", server_name);
	printf ("Now Attempting to send a message to the server\n", server_name);
	char buf [1024];
	sprintf (buf, "hello");
	send (sockfd, buf, strlen (buf)+1, 0);
	recv (sockfd, buf, 1024, 0);
	printf ("Received %s from the server\n", buf);
	return 0;
}


int main (int ac, char** av)
{
	client (av [1], av [2]);
}