/*
	Joshua Miner	- jmm299
	Ethan Welsh		- 
	CS1652 Fall 2014
	Project 1- http_client.cc
*/

#include "minet_socket.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
<<<<<<< HEAD
#include <assert.h>
=======
#import <assert.h>
>>>>>>> 96c09fa8b7148c656448fc47fda3b95a6446eb6f
#include <unistd.h>
#include <iostream>

using namespace std;

#define BUFSIZE 1024

<<<<<<< HEAD

int main(int argc, char * argv[])
{
    char * server_name = NULL;
    int server_port    = -1;
    char * server_path = NULL;
    char * req         = NULL;


    int socketID;
    int len;
    int res;
    char buf[BUFSIZE];
    struct hostent *hp;
    struct sockaddr_in saddr;

=======
int main(int argc, char * argv[])
{
	char * server_name = NULL;	// Store the server name
    int server_port    = -1;	// Store the port for the server
    char * server_path = NULL;	// Path to the server
    char * req         = NULL;	
    bool ok            = false;	// Used to make sure that everything is good

    int socketID;		// id of the socket
    int len;		// length
    int res;	// store the result
    char buf[BUFSIZE];		// buffer
    struct hostent *hp;
    struct sockaddr_in saddr;
	int socket_fd;	// File descriptor
	fd_set fds;
	string response = "";	// Hold response 
	string header = "";	// Hold the header for processing
	string status;		// Hold the expected status code
	string::size_type position;
	
    /*parse args */
>>>>>>> 96c09fa8b7148c656448fc47fda3b95a6446eb6f
    if (argc != 5)
    {
        fprintf(stderr, "usage: http_client k|u server port path\n");
        exit(-1);
    }

    server_name = argv[2];
    server_port = atoi(argv[3]);
    server_path = argv[4];

    req = (char *)malloc(strlen("GET  HTTP/1.0\r\n\r\n")+strlen(server_path)+1);

<<<<<<< HEAD
    if (toupper(*(argv[1])) == 'K')
    {

    }
    else if (toupper(*(argv[1])) == 'U')
    {

=======
	if (server_port<= 0 || server_port> 65535) 
	{
		// If the port number is not valid
    	fprintf(stderr, "Port number is invalid. Aborting.\n");
		free(req);
		return -1;
    }
    
	/* initialize */
    if (toupper(*(argv[1])) == 'K')
    {
		minet_init(MINET_KERNEL);
    }
    else if (toupper(*(argv[1])) == 'U')
    {
        minet_init(MINET_USER);
>>>>>>> 96c09fa8b7148c656448fc47fda3b95a6446eb6f
    }
    else
    {
        fprintf(stderr, "First argument must be k or u\n");	// Improper option
		free(req);	// Free space used in malloc
        exit(-1);	// return error
    }

<<<<<<< HEAD

    if ((socketID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
=======
    /* make socket */
    if ((socket_fd= minet_socket(SOCK_STREAM)) < 0)
>>>>>>> 96c09fa8b7148c656448fc47fda3b95a6446eb6f
    { //error processing;
        printf("Failed to establish socket.\n");	// Encountered error
		free(req);	// Free space in malloc
        return -1;	// return error
    }


    if ((hp = gethostbyname(server_name)) == NULL)
    {  //error processing;
        printf("DNS could not locate the page you asked for.\n");
        free(req);
		return -1;
    }

    saddr.sin_family = AF_INET;
    memcpy(&saddr.sin_addr.s_addr, hp->h_addr, hp->h_length);  /* TODO CHANGED LENGTH TO H_LENGTH*/
    saddr.sin_port = htons(server_port);

    printf("Connecting to Server...\n");

<<<<<<< HEAD

    if (connect(socketID, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
=======
    /* connect to the server socket */
    if (minet_connect(socket_fd, &saddr)< 0)
>>>>>>> 96c09fa8b7148c656448fc47fda3b95a6446eb6f
    { //error processing;
        printf("We couldn't establish a connection\n");
        free(req);
		return -1;
    }
    printf("Connection has Been Established\n");
<<<<<<< HEAD


    sprintf(req, "GET %s HTTP/1.0\r\n\r\n", server_path);

    printf("Writing the following request to host:\n-----------------\n%s\n-----------------\n", req);

    send(socketID, req, strlen(req), 0);


    read(socketID, buf, BUFSIZE);

    printf("%s", buf);




    return 0;
}
=======
	// Set up of connection is completed
	
    /* send request message */
    printf("Processing the request and sending it");
	sprintf(req, "GET %s HTTP/1.0\r\n\r\n", server_path);
	if(minet_write(socket_fd, req, strlen(req))<= 0)
	{
		printf("Writing the following request to host:\n %s", req);
		fprintf(stderr, "There was an issue sending the message to the server.\n");
		free(req);
		return -1;
	}
	
	printf("Written\n");
	
	//wait till socket can be read.	====	use select(), and ignore timeout for now. 
	FD_ZERO(&fds); // Clear out fd
	FD_SET(socket_fd, &fds); // Adds socket file descriptor
    if(minet_select(socket_fd+1, &fds, NULL, NULL, NULL)<0)
	{
		fprintf(stderr, "Error choosing a socket.\n");
		free(req);
		minet_close(socket_fd); // Close the socket
		minet_deinit();	// De initialize the socket
		return -1;
	}
	
	// First loop
	while(res= minet_read(socket_fd, buf, BUFSIZE-1)> 0)	// Read first portion
	{
		printf("I READ STUFF\n");
	}
	

	// examine return code

	//Skip "HTTP/1.0"
	//remove the '\0'

	// Normal reply has return code 200
	// print first part of response: header, error code, etc.
	
	// second read loop -- print out the rest of the response: real web content

    // close socket and deinitialize 
	minet_close(socket_fd);
	minet_deinit();
	free(req);

    if(ok)
	{
        return 0;
    }
	else
	{
        return -1;
    }
}
>>>>>>> 96c09fa8b7148c656448fc47fda3b95a6446eb6f
