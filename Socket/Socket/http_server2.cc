/*---------------------------------------------*/
/* Ethan Welsh (EJW45) & Josh Miner (JMM299)   */
/* September 26th, 2014                        */
/* Networking Server 2                         */
/*---------------------------------------------*/

/* INCLUDES */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

/* DEFINES */
#define BUFSIZE 1024
#define FILENAMESIZE 100
#define NUMBEROFMAXIMUMCONNECTIONS 20

/* FUNCTION HEADERS */
int handle_connection(int sock);
FILE *getFile(char *request);

/* GLOBALS */
char buf[BUFSIZE];
int sockets[NUMBEROFMAXIMUMCONNECTIONS];
fd_set master_bag;
int current_socket;
int max_socket;


int main(int argc, char *argv[])
{
    int server_port;
    int master_socket;

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
    if ((master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
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
    if (bind(master_socket, (struct sockaddr *) &saddr, sizeof(saddr)) < 0)
    {
        printf("I can't bind correctly.\n");
        return -1;
    }

    /* start listening */
    if (listen(master_socket, 32) < 0)
    {
        printf("I'm a bad listener.\n");
        return -1;
    }

    int i;

    while (1)
    {
        /* Add the master socket which will sit and wait for additional connections to arrive */
        FD_ZERO(&master_bag);
        FD_SET(master_socket, &master_bag);
        max_socket = master_socket;

        /* Iterate through all connections and add everything to our FD_SET*/
        for (i = 0; i < NUMBEROFMAXIMUMCONNECTIONS; i++)
        {
            current_socket = sockets[i];
            if (current_socket > 0) FD_SET(current_socket, &master_bag);
            if (current_socket > max_socket) max_socket = current_socket; // Update the max socket
        }

        /* Select on the FD SET */
        select(max_socket + 1, &master_bag, NULL, NULL, NULL);

        /* If a new connection is detected, add it to our list*/
        if (FD_ISSET(master_socket, &master_bag))
        {
            /* Accept the new socket and store it*/
            int new_socket = accept(master_socket, NULL, NULL);
            if (new_socket < 0)
            {
                fprintf(stderr, "Error while accepting the socket.\n");
                return -1;
            }

            /* Put the socket in the next open slot in our array*/
            for (i = 0; i < NUMBEROFMAXIMUMCONNECTIONS; i++)
            {
                if (sockets[i] == 0)
                {
                    sockets[i] = new_socket;
                    break;
                }
            }
        }

        /* Step through the connections and handle them if they've registered any changes*/
        for (i = 0; i < NUMBEROFMAXIMUMCONNECTIONS; i++)
        {
            if (FD_ISSET(sockets[i], &master_bag))
                handle_connection(sockets[i]);
        }
    }
}


int handle_connection(int sock)
{
    const char *ok_response_f = "HTTP/1.0 200 OK\r\n"    \
    "Content-type: text/plain\r\n"            \
    "Content-length: %d \r\n\r\n";

    const char *notok_response = "HTTP/1.0 404 FILE NOT FOUND\r\n"    \
    "Content-type: text/html\r\n\r\n"            \
    "<html><body bgColor=black text=white>\n"        \
    "<h2>404 FILE NOT FOUND</h2>\n"
            "</body></html>\n";

    /* Receive data and read it into the buffer */
    int len = recv(sock, buf, sizeof(buf) - 1, 0);
    buf[len] = '\0';

    /* Gets the file pointer to the file user requested. NULL if not found. */
    FILE *fileTheUserRequested = getFile(buf);

    /* If the file actually exists... */
    if (fileTheUserRequested != NULL)
    {
        /* Figure out how big the file is*/
        fseek(fileTheUserRequested, 0, SEEK_END);
        long sizeOfFile = ftell(fileTheUserRequested);
        rewind(fileTheUserRequested);

        /* Read the file into a buffer*/
        char fileContent[sizeOfFile];
        fread(fileContent, 1, sizeOfFile, fileTheUserRequested);

        /* Then write it out to the client*/
        write(sock, fileContent, sizeOfFile);
        return 0;
    }
    else
    {
        /* Send back a 404 response if the file can't be found */
        write(sock, notok_response, 140);
        return -1;
    }
}


FILE *getFile(char *request)
{
    /* Parse file name and return file pointer from HTTP request*/
    /* What follow is bad, bad, bad programming. All those of faint heart, avert your eyes */
    int lengthOfRequest = strlen(request);
    char fileName[BUFSIZE];
    int numberOfSlash = 0;

    /* Find to see if they included a leading / in the path*/
    for (int i = 0; i < lengthOfRequest; i++)
    {
        if (request[i] == '/')
        {
            numberOfSlash++;
        }
    }

    int indexOfFirstSpace = 0;
    int indexOfSecondSpace = 0;

    /* If they put their request in the "/index" format...*/
    if (numberOfSlash == 2)
    {
        for (int i = 0; i < lengthOfRequest; i++)
        {
            if (request[i] == '/' && indexOfFirstSpace == 0)
            {
                indexOfFirstSpace = i;
            }
            else if (request[i] == ' ' && indexOfSecondSpace == 0)
            {
                indexOfSecondSpace = i;
            }
        }
    }
    /* Otherwise if the request is in "/path/to/file.html" or "file.html"...*/
    else
    {
        for (int i = 0; i < lengthOfRequest; i++)
        {
            if (request[i] == ' ' && indexOfFirstSpace == 0)
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

    /* Copy over the path into a separate buffer*/
    for (int i = indexOfFirstSpace + 1; i <= indexOfSecondSpace; i++)
    {
        fileName[j] = request[i];
        j++;
    }

    fileName[j - 1] = '\0';

    /* try opening the file */
    return fopen(fileName, "rb");
}
