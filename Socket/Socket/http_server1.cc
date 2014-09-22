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
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netdb.h>

#define BUFSIZE 1024
#define FILENAMESIZE 100

int handle_connection(int sock);

int main(int argc, char * argv[])
{
    int server_port = -1;	// hold the server's port number
    int rc          =  0;
    int sock        = -1;	// Hold the socket value from making it

	sockaddr_in server_ours;
    /* parse command line args */
    if (argc != 3) 
	{
		fprintf(stderr, "usage: http_server1 k|u port\n");
		exit(-1);
    }

    server_port = atoi(argv[2]);

    if (server_port < 1500)
	{
		fprintf(stderr, "INVALID PORT NUMBER: %d; can't be < 1500\n", server_port);
		exit(-1);
    }

    /* initialize */
    if (toupper(*(argv[1])) == 'K')
	{ 
	}
	else if (toupper(*(argv[1])) == 'U')
	{ 
	}
	else
	{
		fprintf(stderr, "First argument must be k or u\n");
		exit(-1);
    }

    /* initialize and make socket */
	if(server_port<0 || server_port>65535)
	{
		fprintf(stderr, "You have entered an invalid port number\n");
		return -1;
	}
	if((sock= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))< 0)
	{
		fprintf(stderr, "Encountered an issue while trying to init the socket.\n");
		return -1;
	}
    /* set server address*/
	memset(&server_ours, 0, sizeof(server_ours));
	server_ours.sin_family = AF_INET;
	server_ours.sin_addr.s_addr = INADDR_ANY;
	server_ours.sin_port = htons(server_port);

    /* bind listening socket */
	if (bind(s, (struct sockaddr *)&server_ours, sizeof(server_ours)) < 0)
	{
		fprintf(stderr, "Failed to bind the socket.\n");
		return -1;
	}

    /* start listening */
	if (listen(sock, 32) < 0)
	{
		fprintf(stderr, "Unable to listen.\n");
		return -1;
	}
	
    /* connection handling loop: wait to accept connection */
	while (1)
	{
		/* handle connections */
		rc = handle_connection(sock);
    }
	close(sock);	// Close the socket
	fprintf(stderr, "Server was killed.\n");
	return 0;
}

int handle_connection(int sock)
{
    bool ok = false;

    const char * ok_response_f = "HTTP/1.0 200 OK\r\n"	\
	"Content-type: text/plain\r\n"			\
	"Content-length: %d \r\n\r\n";
 
    const char * notok_response = "HTTP/1.0 404 FILE NOT FOUND\r\n"	\
	"Content-type: text/html\r\n\r\n"			\
	"<html><body bgColor=black text=white>\n"		\
	"<h2>404 FILE NOT FOUND</h2>\n"
	"</body></html>\n";
    
    /* first read loop -- get request and headers*/
    
    /* parse request to get file name */
    /* Assumption: this is a GET request and filename contains no spaces*/

    /* try opening the file */

    /* send response */
    
	if (ok)
	{
		/* send headers */
	
		/* send file */
	
    }
	else
	{
		// send error response
    }
    
    /* close socket and free space */
  
    if (ok)
	{
		return 0;
    }
	else
	{
		return -1;
    }
}
