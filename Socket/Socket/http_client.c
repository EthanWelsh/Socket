#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>


/*
	Joshua Miner and Ethan Welsh
	CS1652 Fall 2014
	Project 1: Part 1
	HTTP Client
*/

const int MAX_LINE = 500;

int main(int argc, char **argv) // http_client k|u server_name server_port server_path
{

    int s, len, res;
    char buf[MAX_LINE];
    struct hostent *hp;
    struct sockaddr_in saddr;

    if(argc != 5)	// If there are not 5 arguments on the command line
    {
        printf("Improperly formatted command line. Try again \n");		// Print what the error is
        printf("Format is http_client k|u server_name server_port server_path\n");		// Format
        return EXIT_FAILURE;
    }



    char* mode = argv[1];
    char* server_name = argv[2];
    int server_port = atoi(argv[3]);
    char* server_path = argv[4];

    if ((server_port <= 0) || (server_port > 65535))
    {
        printf("Invalid server port!");
        return -1;
    }

    if(strncmp("k", mode, 1) == 0)	// Operating in the Kernel Stack
    {
        if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        { //error processing;
            printf("Failed to establish socket.\n");
            return -1;

        }

        if ((hp = gethostbyname(argv[1])) == NULL)
        {  //error processing;
            printf("DNS could not locate the page you asked for.\n");
            return -1;
        }


        saddr.sin_family = AF_INET;
        memcpy(&saddr.sin_addr.s_addr, hp->h_addr, hp->h_length);  /* TODO CHANGED LENGTH TO H_LENGTH*/
        saddr.sin_port = htons(server_port);

        if (connect(s, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
        { //error processing;
            printf("We couldn't establish a connection\n");
            return -1;
        }

        printf("MODE: %s\n", mode);
        printf("SERVER NAME: %s\n", server_name);
        printf("SERVER PORT: %d\n", server_port);
        printf("SERVER PATH: %s\n", server_path);


        return 0;	// All operations were successful
    }

    else if(strncmp("u", mode, 1) == 0)
    {
        printf("U MODE\n");
        // Operating in the User (Minet) Stack
        // We don't have to build this one for the project?
        return 0;	// Operations were successful
    }
    else
    {
        return -1;		// There was an issue encountered
    }



}