#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>

#define BUFSIZE 1024
#define FILENAMESIZE 100

int handle_connection(int sock);
FILE* getFile(char* request);

int main(int argc, char * argv[])
{
    int server_port = -1;
    int rc          =  0;
    int socketID;

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

    /* initialize and make socket */
    if ((socketID = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    { //error processing;
        printf("Failed to establish socket.\n");
        return -1;
    }

    /* set server address*/

    struct sockaddr_in saddr;

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(server_port);

    /* bind listening socket */
    if (bind(socketID, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
    {
        printf("I can't bind correctly.\n");
        return -1;
    }

    /* start listening */
    if (listen(socketID, 32) < 0)
    {
        printf("I'm a bad listener.\n");
        return -1;
    }

    /* connection handling loop: wait to accept connection */
    int new_socket;
    while (1)
    {
        new_socket = accept(socketID, NULL, NULL);

	    /* handle connections */
        if(new_socket < 0)
        {
            fprintf(stderr, "Error while accepting the socket.\n");
            return -1;
        }
        rc = handle_connection(new_socket);
    }
}

int handle_connection(int sock)
{
    bool ok = false;
    int len;
    char buf[BUFSIZE];

    const char * ok_response_f = "HTTP/1.0 200 OK\r\n"	\
	"Content-type: text/plain\r\n"			\
	"Content-length: %d \r\n\r\n";

    const char * notok_response = "HTTP/1.0 404 FILE NOT FOUND\r\n"	\
	"Content-type: text/html\r\n\r\n"			\
	"<html><body bgColor=black text=white>\n"		\
	"<h2>404 FILE NOT FOUND</h2>\n"
	"</body></html>\n";

    /* first read loop -- get request and headers*/
    char data_received[BUFSIZE*1024];
	int next_posit = 0;	// track where to write data to

    len = recv(sock, buf, sizeof(buf)-1, 0);	// Do a receive of data for request
    buf[len] = '\0';


    FILE* fileTheUserRequested = getFile(buf); // Gets the file pointer to the file user requested. NULL if not found.

    if(fileTheUserRequested != NULL)
    {
        fseek(fileTheUserRequested, 0, SEEK_END);
        long sizeOfFile = ftell(fileTheUserRequested);
        rewind(fileTheUserRequested);
        char fileContent[sizeOfFile];
        fread(fileContent, 1, sizeOfFile, fileTheUserRequested);

        write(sock, fileContent, sizeOfFile);
    }
    else
    {
        write(sock, notok_response, 140);
    }


    do
	{
		if (len > 0)	// If there is data being read in
		{
			memcpy((data_received+next_posit), buf, len);	// Copy into one location
			next_posit+= len;
			if(len< BUFSIZE)	// At the last block
			{
				break;	// Break out of the loop
			}
		}

			len = recv(sock, buf, sizeof(buf)-1, 0);

    } while(len > 0);


    /* close socket and free space */
	close(sock);

    if (ok)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

FILE* getFile(char* request)
{
    int lengthOfRequest = strlen(request);




    char fileName[BUFSIZE];

    // If there is only one slash in the file. do stuff another way


    int numberOfSlash = 0;

    for(int i = 0; i < lengthOfRequest; i++)
    {
        if(request[i] == '/')
        {
            numberOfSlash++;
        }
    }


    int indexOfFirstSpace = 0;
    int indexOfSecondSpace = 0;

    if(numberOfSlash == 2)
    {

        for(int i = 0; i < lengthOfRequest; i++)
        {
            if(request[i] == '/' && indexOfFirstSpace == 0)
            {
                indexOfFirstSpace = i;
            }
            else if (request[i] == ' ' && indexOfSecondSpace == 0)
            {
                indexOfSecondSpace = i;
            }
        }
    }
    else
    {
        for(int i = 0; i < lengthOfRequest; i++)
        {
            if(request[i] == ' ' && indexOfFirstSpace == 0)
            {
                indexOfFirstSpace = i;
            }
            else if (request[i] == ' ' && indexOfSecondSpace == 0)
            {
                indexOfSecondSpace = i;
            }
        }
    }

    int j = 0;

    for(int i = indexOfFirstSpace + 1; i<= indexOfSecondSpace; i++)
    {
        fileName[j] = request[i];
        j++;
    }

    fileName[j - 1] = '\0';
	/* try opening the file */
    return fopen(fileName, "rb");


}