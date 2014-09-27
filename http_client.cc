/*
	Joshua Miner	- jmm299@pitt.edu
	Ethan Welsh		- ejw45@pitt.edu
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

using namespace std;

int main(int argc, char * argv[])
{
    char * server_name = NULL;	// Store the name of the server
    int server_port    = -1;	// Port connecting on
    char * server_path = NULL;	// Path using
    char * req         = NULL;	// Store the request being made
    bool ok            = false;	// Used to make sure that everything is good

    int socketID;	// Store the sockets id
    int len;	// Length
    int res;	// Results
    char buf[BUFSIZE];	// Buffer
    struct hostent *hp;
    struct sockaddr_in saddr;
    fd_set fds;		// File Descriptor set
	
    string response = "";	// Hold response
    string header = "";	// Hold the header for processing
    string status;		// Hold the expected status code
    string::size_type position;

    // Check that all arguments were provided.
    if (argc != 5)
    {
        fprintf(stderr, "usage: http_client k|u server port path\n");	// Print the proper usage for them to see
        exit(-1);		// Close the program
    }

    server_name = argv[2];	// Store the server's name
    server_port = atoi(argv[3]);	// Convert to int and store
    server_path = argv[4];		// Store the server's path

    req = (char *)malloc(strlen("GET  HTTP/1.0\r\n\r\n")+strlen(server_path)+1); // Malloc space to store and build the request

    if (server_port<= 0 || server_port> 65535)	// Check the port has a valid number
    {
        // If the port number is not valid
        fprintf(stderr, "Port number is invalid. Aborting.\n");
        free(req);	// Free the space that was malloced
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
        fprintf(stderr, "First argument must be k or u\n");	// Invalid option entered
        free(req);	// Free the space that was malloced
        exit(-1);	// Close the program
    }


    if ((socketID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    { //error processing;
        printf("Failed to establish socket.\n");
        free(req);	// Free the space that was malloced
        return -1;	// close the program
    }

    if ((hp = gethostbyname(server_name)) == NULL)
    {  //error processing;
        printf("DNS could not locate the page you asked for.\n");
        free(req);
        return -1;
    }

    // Set the address
    saddr.sin_family = AF_INET;
    memcpy(&saddr.sin_addr.s_addr, hp->h_addr, hp->h_length);  // CHANGED LENGTH TO H_LENGTH
    saddr.sin_port = htons(server_port);

    // printf("Connecting to Server...\n");

    if (connect(socketID, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    { //error processing;
        printf("We couldn't establish a connection\n");
        free(req);		// Free the space that was malloced
        return -1;		// Close the program
    }

    // printf("Connection has Been Established\n");

    /* send request message */
    sprintf(req, "GET /%s HTTP/1.0\r\n\r\n", server_path);
    // printf("\n----------------------------\n%s\n--------------------------\n",req);

    if(write(socketID, req, strlen(req))<= 0)
    {
        fprintf(stderr, "There was an issue sending the message to the server.\n");
        free(req);		// Free the space that was malloced
        return -1;		// Close the program
    }

    //wait till socket can be read.	====	use select(), and ignore timeout for now.
    FD_ZERO(&fds); // Clear out fd
    FD_SET(socketID, &fds); // Adds socket file descriptor
    if(select(socketID+1, &fds, NULL, NULL, NULL)<0)
    {
        fprintf(stderr, "Error choosing a socket.\n");
        free(req);		// Free the space that was malloced
        close(socketID); // Close the socket
        return -1;
    }

    // First loop
    while((res= read(socketID, buf, BUFSIZE-1))> 0)	// Read first portion
    {
        buf[res]= '\0'; // Make empty string
        response+= std::string(buf);
        position= response.find("\r\n\r\n", 0); // Find the position of this string

        if(position!= std::string::npos)	// Found the header
        {
            header= response.substr(0, position); // Get the header
            response= response.substr(position+4); // Get the response
            break;	// have what I need now so move on
        }
        res= read(socketID, buf, BUFSIZE-1); // Read in the next portion
    }

    // examine return code
    //Skip "HTTP/1.0"
    //remove the '\0'

    status = header.substr(header.find(" ") + 1); // Get status code
    status = status.substr(0, status.find(" "));

    // Normal reply has return code 200
    if(strcmp(status.c_str(), "200") == 0)
    {
        ok= true; // No errors
        std::cout << response; // Output the first response data
    }
    else
    {
        ok = false;
        std::cerr << header + "\r\n\r\n" + response; // Error encountered
    }
    // second read loop -- print out the rest of the response: real web content
    while( (res = read(socketID, buf, BUFSIZE-1) ) > 0)
    {
        buf[res] = '\0';
        if(ok)
        {
            printf("%s", buf);
        }
        else
        {
            fprintf(stderr, buf);
        }
    }

    // close socket and deinitialize
    close(socketID);
    free(req);	// Free the malloced space

    if(ok)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
