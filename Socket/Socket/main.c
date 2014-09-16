#import <stdlib.h>
#import <stdio.h>
#import <string.h>

/*
	Joshua Miner and Ethan Welsh
	CS1652 Fall 2014
	Project 1: Part 1
	HTTP Client
*/

int main(int argc, char **argv) // http_client k|u server_name server_port server_path
{
    printf("There are %d args\n", argc);
    printf("ARG 1: %c\n", atoi(argv[2]));


    if(argc != 6)	// If there are not 5 arguments on the command line
    {
        printf("Improperly formatted command line. Try again \n");		// Print what the error is
        printf("Format is http_client k|u server_name server_port server_path\n");		// Format
        return EXIT_FAILURE;
    }
    else
    {
        int http_client = atoi(argv[1]);
        char* mode = argv[2];
        char* server_name = argv[3];
        int server_port = atoi(argv[4]);
        char* server_path = argv[5];

        printf("CLIENT: %d\n", http_client);
        printf("MODE: %s\n", mode);
        printf("SERVER NAME: %s\n", server_name);
        printf("SERVER PORT: %d\n", server_port);
        printf("SERVER PATH: %s\n", server_path);

        if(strncmp("k", mode, 1) == 0)	// Operating in the Kernel Stack
        {
            printf("K MODE\n");
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


}