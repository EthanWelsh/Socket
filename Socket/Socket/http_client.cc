/*
	Joshua Miner	- jmm299
	Ethan Welsh		- 
	CS1652 Fall 2014
	Project 1- http_client.cc
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <assert.h>
#include <unistd.h>
#include <iostream>

#define BUFSIZE 1024


int main(int argc, char * argv[])
{
    char * server_name = NULL;
    int server_port    = -1;
    char * server_path = NULL;
    char * req         = NULL;
	bool ok            = false;	// Used to make sure that everything is good

    int socketID;
    int len;
    int res;
    char buf[BUFSIZE];
    struct hostent *hp;
    struct sockaddr_in saddr;
	fd_set fds;
	
    string response = "";	// Hold response 
	string header = "";	// Hold the header for processing
	string status;		// Hold the expected status code
	string::size_type position;
	
	// Check that all arguments were provided.
	if (argc != 5)
    {
        fprintf(stderr, "usage: http_client k|u server port path\n");
        exit(-1);
    }

    server_name = argv[2];
    server_port = atoi(argv[3]);
    server_path = argv[4];

    req = (char *)malloc(strlen("GET  HTTP/1.0\r\n\r\n")+strlen(server_path)+1);

	if (server_port<= 0 || server_port> 65535) 
	{
		// If the port number is not valid
    	fprintf(stderr, "Port number is invalid. Aborting.\n");
		free(req);
		return -1;
    }
	
	// For initializing minet kernel
    if (toupper(*(argv[1])) == 'K')
    {

    }
    else if (toupper(*(argv[1])) == 'U')
    {

    }
    else
    {
        fprintf(stderr, "First argument must be k or u\n");
        free(req);
		exit(-1);
    }


    if ((socketID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    { //error processing;
        printf("Failed to establish socket.\n");
		free(req);
		return -1;
    }


    if ((hp = gethostbyname(server_name)) == NULL)
    {  //error processing;
        printf("DNS could not locate the page you asked for.\n");
        free(req);
		return -1;
    }

	// Set the address
    saddr.sin_family = AF_INET;
    memcpy(&saddr.sin_addr.s_addr, hp->h_addr, hp->h_length);  /* TODO CHANGED LENGTH TO H_LENGTH*/
    saddr.sin_port = htons(server_port);

    printf("Connecting to Server...\n");


    if (connect(socketID, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    { //error processing;
        printf("We couldn't establish a connection\n");
        return -1;
    }

    printf("Connection has Been Established\n");


    sprintf(req, "GET %s HTTP/1.0\r\n\r\n", server_path);

    printf("Writing the following request to host:\n-----------------\n%s\n-----------------\n", req);

    send(socketID, req, strlen(req), 0);
    read(socketID, buf, BUFSIZE);

    printf("%s", buf);

    return 0;
}
